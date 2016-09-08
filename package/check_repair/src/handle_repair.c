#include "repair.h" 
#include <signal.h>

int main(int argc, char *argv[])
{
	char mount_dir[32];
	char cmd[32];
	int ret = 0;
	dev_group_t g_dev_group;
	dev_info_t g_dev_op;

	signal(SIGCHLD, SIG_IGN);
	if(argc != 2)
	{
		DMCLOG_E("invalid argument! \nplease input as :handle_repair /tmp/mnt/USB-disk-1");
		memset(cmd, 0 ,32);
		sprintf(cmd, "touch %s", REPAIR_FAIL);
		DMCLOG_M("cmd = %s", cmd);
		system(cmd);

		memset(cmd, 0 ,32);
		sprintf(cmd, "echo argument error >> /tmp/repair_log");
		DMCLOG_E("cmd = %s", cmd);
		system(cmd);
		return -1;
	}else
	{	
		if(access(REPAIR_DOING, 0) != -1)
		{
			DMCLOG_M("fsck repair is doing");
			return -1;
		}
		if(access(REPAIR_FAIL, 0) != -1)
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "rm %s", REPAIR_FAIL);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);
		}
	}
	
	memset(mount_dir, 0, 32);
	strcpy(mount_dir, argv[1]);
	ret = mount_dir_to_dev_info(mount_dir, &g_dev_group, &g_dev_op);
	if(0 != ret)
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "touch %s", REPAIR_FAIL);
		DMCLOG_M("cmd = %s", cmd);
		system(cmd);
		if(-1 == ret)
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "echo Get device info error >> /tmp/repair_log");
			DMCLOG_E("cmd = %s", cmd);
			system(cmd);
		}
		if(1 == ret)
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "echo Don't have dev node for %s >> /tmp/repair_log", mount_dir);
			DMCLOG_E("cmd = %s", cmd);
			system(cmd);
		}
		return -1;
	}
	DMCLOG_D("g_dev_op.dev_node = %s", g_dev_op.dev_node);
	
	ret = handle_repair(&g_dev_group, &g_dev_op);
	if(ret == 0)
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "echo fsck repair succsee >> /tmp/repair_log");
		DMCLOG_M("cmd = %s", cmd);
		system(cmd);
		return 0;
	}else 
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "touch %s", REPAIR_FAIL);
		DMCLOG_M("cmd = %s", cmd);
		system(cmd);
		if(ret == 1)
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "echo umount device node error >> /tmp/repair_log");
			DMCLOG_E("cmd = %s", cmd);
			system(cmd);
		}
		if(ret == 2)
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "echo fsck device error >> /tmp/repair_log");
			DMCLOG_E("cmd = %s", cmd);
			system(cmd);
		}
		return -1;
	}	
}
