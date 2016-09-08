#include "repair.h"
#include <signal.h>

int Repair_Flag = REPAIR_FAIL_TIME;

int main(void)
{
	dev_group_t g_dev_group;	
	int ret = 0;
	char cmd[32];
	char repair_auto[32];
	int repair_try_time = 0;

	signal(SIGCHLD, SIG_IGN);
	#if 0
	memset(repair_auto, 0, 32);
	ret = get_cfg_str("repair_auto", repair_auto);
	DMCLOG_D("repair_auto = %s", repair_auto);
	if(0 == strcmp(repair_auto, "off"))
	{
		DMCLOG_M("repair_auto is off");
		return 0;
	}
	#endif

	#if 0
	ret = get_dev_group_info(&g_dev_group);
	if(-1 == ret)
	{
		DMCLOG_E("get dev group info error");
		goto EXIT1;
	}	
	DMCLOG_D("power on g_dev_group.count = %d", g_dev_group.count);
	if(g_dev_group.count > 0)
	{
		ret = all_repair(&g_dev_group);
	}
	#endif
	while(Repair_Flag)
	{
		sleep(3);
		DMCLOG_D("start Repair_Flag = %d", Repair_Flag);
		//memset(&g_dev_group, 0, sizeof(struct dev_group_t));
		ret = get_dev_group_info(&g_dev_group);
		if(-1 == ret)
		{
			DMCLOG_E("get dev group info error");
			goto EXIT1;
		}	
		DMCLOG_D("g_dev_group.count = %d", g_dev_group.count);
		if(g_dev_group.count > 0)
		{
			for(repair_try_time = 0 ; repair_try_time < REPAIR_TRY_TIME; repair_try_time++)
			{
				ret = check_repair(&g_dev_group);
				if(ret == 0)
					break;
			}
			if(repair_try_time == REPAIR_TRY_TIME)
			{
				DMCLOG_E("umount device node error!");
				Repair_Flag = 0;
			}
		}
	}
	
	if(0 == Repair_Flag)
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "touch %s", REPAIR_FAIL);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
		if(repair_try_time == REPAIR_TRY_TIME)
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "echo umount device node error >> /tmp/repair_log");
			DMCLOG_M("cmd = %s", cmd);
			system(cmd);
		}else
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "echo fsck device error >> /tmp/repair_log");
			DMCLOG_M("cmd = %s", cmd);
			system(cmd);
		}
	}
	
EXIT1:
	return ret;			
}
