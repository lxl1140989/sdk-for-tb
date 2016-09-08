#include "repair.h"

//char Repair_History[32];
int  Repair_Flag;



void del_n(char *str)
{
	int i=0;
	while(str[i])
	{
		if('\n' == str[i])
		{
			str[i]=0;
			if(i>0 && str[i-1]=='\r')
				str[i-1]=0;
		}
		i++;
	}
}

int get_cfg_str(char *param,char *ret_str)
{
	char get_str[32]={0};
	char tmp[32]={0};
	FILE *fp;
	sprintf(get_str, "cfg get \'%s\'",param);
	fp=popen(get_str, "r");
	fgets(tmp, 32, fp);
	fclose(fp);
	del_n(tmp);
	if(strlen(tmp)<=1)
		return 0;
	else
	{
		strcpy(ret_str,tmp+strlen(param)+1);
		return 1;
	}
}


int check_dev_group_info(void)
{
	FILE *mntfile;	
	struct mntent *mntent;
	int count = 0;
	
	mntfile = setmntent(MTAB_PATH, "r");
	if (!mntfile) {
			printf("Failed to read mtab file, error [%s]\n",
							strerror(errno));
			return -1;
	}  
	while(mntent = getmntent(mntfile))
	{
		/*printf("%s, %s, %s, %s\n",
						mntent->mnt_dir,
						mntent->mnt_fsname,
						mntent->mnt_type,
						mntent->mnt_opts);*/
		if((NULL != strstr(mntent->mnt_fsname, DEV_NODE_HEAD)) && (NULL != strstr(mntent->mnt_dir, MOUNT_PATH)))
		{
			count++;
		}
	}
	
	endmntent(mntfile);
	return count;
}


int get_dev_group_info(dev_group_t *dev_group_info)
{
	FILE *mntfile;	
	//int flag = 0;
	struct mntent *mntent;

	dev_group_info->count = 0;
	mntfile = setmntent(MTAB_PATH, "r");
	if (!mntfile) {
			printf("Failed to read mtab file, error [%s]\n",
							strerror(errno));
			return -1;
	}  
	while(mntent = getmntent(mntfile))
	{
		/*printf("%s, %s, %s, %s\n",
						mntent->mnt_dir,
						mntent->mnt_fsname,
						mntent->mnt_type,
						mntent->mnt_opts);*/
		if((NULL != strstr(mntent->mnt_fsname, DEV_NODE_HEAD)) && (NULL != strstr(mntent->mnt_dir, MOUNT_PATH)))
		{
			memset(dev_group_info->g_dev[dev_group_info->count].mount_dir, 0, 32);
			memcpy(dev_group_info->g_dev[dev_group_info->count].mount_dir, mntent->mnt_dir, strlen(mntent->mnt_dir));
			memset(dev_group_info->g_dev[dev_group_info->count].dev_node, 0, 32);
			memcpy(dev_group_info->g_dev[dev_group_info->count].dev_node, mntent->mnt_fsname, strlen(mntent->mnt_fsname));
			memset(dev_group_info->g_dev[dev_group_info->count].fs_type, 0, 32);
			memcpy(dev_group_info->g_dev[dev_group_info->count].fs_type, mntent->mnt_type, strlen(mntent->mnt_type));
			memset(dev_group_info->g_dev[dev_group_info->count].rw_flag, 0, 32);
			memcpy(dev_group_info->g_dev[dev_group_info->count].rw_flag, mntent->mnt_opts, 2);
			dev_group_info->count++;
		}
	}
	
	endmntent(mntfile);
	return 0;
}


int mount_dir_to_dev_info(char *mount_dir, dev_group_t *dev_group_info, dev_info_t *dev_op_info)
{
	FILE *mntfile;	
	int flag = 0;
	struct mntent *mntent;

	dev_group_info->count = 0;
	mntfile = setmntent(MTAB_PATH, "r");
	if (!mntfile) {
			printf("Failed to read mtab file, error [%s]\n",
							strerror(errno));
			return -1;
	}  
	while(mntent = getmntent(mntfile))
	{
		/*printf("%s, %s, %s, %s\n",
						mntent->mnt_dir,
						mntent->mnt_fsname,
						mntent->mnt_type,
						mntent->mnt_opts);*/
		if((NULL != strstr(mntent->mnt_fsname, DEV_NODE_HEAD)) && (NULL != strstr(mntent->mnt_dir, MOUNT_PATH)))
		{
			memset(dev_group_info->g_dev[dev_group_info->count].mount_dir, 0, 32);
			memcpy(dev_group_info->g_dev[dev_group_info->count].mount_dir, mntent->mnt_dir, strlen(mntent->mnt_dir));
			memset(dev_group_info->g_dev[dev_group_info->count].dev_node, 0, 32);
			memcpy(dev_group_info->g_dev[dev_group_info->count].dev_node, mntent->mnt_fsname, strlen(mntent->mnt_fsname));
			memset(dev_group_info->g_dev[dev_group_info->count].fs_type, 0, 32);
			memcpy(dev_group_info->g_dev[dev_group_info->count].fs_type, mntent->mnt_type, strlen(mntent->mnt_type));
			memset(dev_group_info->g_dev[dev_group_info->count].rw_flag, 0, 32);
			memcpy(dev_group_info->g_dev[dev_group_info->count].rw_flag, mntent->mnt_opts, 2);
			dev_group_info->count++;
			if(NULL != strstr(mntent->mnt_dir, mount_dir))
			{
				memset(dev_op_info->mount_dir, 0, 32);
				memcpy(dev_op_info->mount_dir, mntent->mnt_dir, strlen(mntent->mnt_dir));
				memset(dev_op_info->dev_node, 0, 32);
				memcpy(dev_op_info->dev_node, mntent->mnt_fsname, strlen(mntent->mnt_fsname));
				memset(dev_op_info->fs_type, 0, 32);
				memcpy(dev_op_info->fs_type, mntent->mnt_type, strlen(mntent->mnt_type));
				memset(dev_op_info->rw_flag, 0, 32);
				memcpy(dev_op_info->rw_flag, mntent->mnt_opts, 2);
				flag = 1;
			}
		}
	}
	
	endmntent(mntfile);
	if(0 == flag)
		return 1;//on dev node for the mount dir
	
	return 0;
}



int del_fsck_rec_file(void)
{
	dev_group_t g_dev_group;	
	int ret = 0;
	int i;
	char cmd[32];
	int rm_flag = 0;
	ret = get_dev_group_info(&g_dev_group);
	if(-1 == ret)
	{
		DMCLOG_E("get dev group info error");
		return ret;
	}

	for(i = 0; g_dev_group.count > i; i++)
	{
		if(NULL != strstr(g_dev_group.g_dev[i].fs_type, FS_TYPE_VFAT))
		{	
			memset(cmd, 0 ,32);
			sprintf(cmd, "rm -f %s/FSCK*.REC", g_dev_group.g_dev[i].mount_dir);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);
			usleep(100000);
			rm_flag = 1;
		}
	}

	if(rm_flag)
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "sync");
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);	
	}
	
	return ret;	
}



int all_repair(dev_group_t *dev_group_info)
{
	int i;
	int ret = 0;
	char cmd[32];
	int vfat_flag = 0;
	int repair_ok_flag = 0;
	FILE *repair_fp = NULL;
	char cmd_buf[1024];
	int check_count = 0;

	for(i = 0; dev_group_info->count > i; i++)
	{
		if(NULL != strstr(dev_group_info->g_dev[i].fs_type, FS_TYPE_VFAT))
		{
			vfat_flag = 1;
			break;
		}
	}
	
	if(vfat_flag)
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "touch %s", REPAIR_DOING);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
		/*
		repair_fp = popen(cmd, "r");
		if(NULL == repair_fp)
		{
			DMCLOG_E("handle %s error", cmd);
			return -1;
		}
		safe_pclose(repair_fp);
		*/

		for(i = 0; dev_group_info->count > i; i++)
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "fuser -km %s", dev_group_info->g_dev[i].mount_dir);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);

			/*
			memset(cmd, 0 ,32);
			sprintf(cmd, "umount %s", dev_group_info->g_dev[i].mount_dir);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);
			
			memset(cmd, 0 ,32);
			sprintf(cmd, "rm -r %s", dev_group_info->g_dev[i].mount_dir);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);
			*/
		}
		
		memset(cmd, 0 ,32);
		sprintf(cmd, "block umount");
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
		usleep(500000);
		
		check_count = check_dev_group_info();
		if(0 == check_count)
		{
			for(i = 0; dev_group_info->count > i; i++)
			{
				if(NULL != strstr(dev_group_info->g_dev[i].fs_type, FS_TYPE_VFAT))
				{
					memset(cmd, 0 ,32);
					sprintf(cmd, "fsck.fat -a %s", dev_group_info->g_dev[i].dev_node);
					DMCLOG_D("cmd = %s", cmd);
					repair_fp = popen(cmd, "r");
					if(NULL == repair_fp)
					{
						DMCLOG_E("handle %s error", cmd);
						ret = -1;
						break;
					}
					memset(cmd_buf, 0, 1024);
					//fread(cmd_buf, sizeof(char), 1024, repair_fp);  
					//DMCLOG_D("cmd_buf = %s", cmd_buf);	
					while (fgets(cmd_buf, 1024, repair_fp) != NULL) {
						DMCLOG_D("cmd_buf = %s", cmd_buf);
			       		if (strstr(cmd_buf, REPAIR_OK_PRINT)) {
			            	DMCLOG_D("repair success!!!!!!!!!!!!!!!!!!!!!!");
							repair_ok_flag = 1;
				        }
						memset(cmd_buf, 0, 1024);
				    	}
					safe_pclose(repair_fp);

					memset(cmd, 0 ,32);
					sprintf(cmd, "echo %s >> /tmp/repair_log", dev_group_info->g_dev[i].dev_node);
					DMCLOG_D("cmd = %s", cmd);
					system(cmd);
				}
			}
		}
		else
		{
			DMCLOG_D("umount fail!");
			ret = 1; //can't umount
		}
		
		for(i = 0; dev_group_info->count > i; i++)
		{
			if(NULL != strstr(dev_group_info->g_dev[i].mount_dir, PRIV_MOUNT_HEAD))
			{
				memset(cmd, 0 ,32);
				sprintf(cmd, "block myhotplug add %s", dev_group_info->g_dev[i].dev_node);
				DMCLOG_D("cmd = %s", cmd);
				system(cmd);
				usleep(100000);
			}
		}
		
		memset(cmd, 0 ,32);
		sprintf(cmd, "block mount");
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);

		if(repair_ok_flag == 1)
		{
			usleep(500000);
			del_fsck_rec_file();
		}
		
		memset(cmd, 0 ,32);
		sprintf(cmd, "rm %s", REPAIR_DOING);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
	}
	return ret;
}


int check_repair(dev_group_t *dev_group_info)
{
	int i;
	int ret = 0;
	char cmd[32];
	int vfat_ro_flag = 0;
	int repair_ok_flag = 0;
	FILE *repair_fp = NULL;
	char cmd_buf[1024];
	int check_count = 0;

	for(i = 0; dev_group_info->count > i; i++)
	{
		if((NULL != strstr(dev_group_info->g_dev[i].rw_flag, RW_RO)) && \
				(NULL != strstr(dev_group_info->g_dev[i].fs_type, FS_TYPE_VFAT)))
		{
			vfat_ro_flag = 1;
			break;
		}
	}

	if(vfat_ro_flag)
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "touch %s", REPAIR_DOING);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
		/*
		repair_fp = popen(cmd, "r");
		if(NULL == repair_fp)
		{
			DMCLOG_D("handle %s error", cmd);
			return -1;
		}
		safe_pclose(repair_fp);
		*/

		for(i = 0; dev_group_info->count > i; i++)
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "fuser -km %s", dev_group_info->g_dev[i].mount_dir);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);

			/*
			memset(cmd, 0 ,32);
			sprintf(cmd, "umount %s", dev_group_info->g_dev[i].mount_dir);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);
			
			memset(cmd, 0 ,32);
			sprintf(cmd, "rm -r %s", dev_group_info->g_dev[i].mount_dir);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);
			*/
		}
		
		memset(cmd, 0 ,32);
		sprintf(cmd, "block umount");
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
		usleep(500000);
		
		check_count = check_dev_group_info();
		if(0 == check_count)
		{
			for(i = 0; dev_group_info->count > i; i++)
			{
				if((NULL != strstr(dev_group_info->g_dev[i].rw_flag, RW_RO)) && \
					(NULL != strstr(dev_group_info->g_dev[i].fs_type, FS_TYPE_VFAT)))
				{	
					memset(cmd, 0 ,32);
					sprintf(cmd, "fsck.vfat -a %s", dev_group_info->g_dev[i].dev_node);
					DMCLOG_D("cmd = %s", cmd);
					repair_fp = popen(cmd, "r");
					if(NULL == repair_fp)
					{
						DMCLOG_D("handle %s error", cmd);
						ret = -1;
						break;
					}
					memset(cmd_buf, 0, 1024);
					//fread(cmd_buf, sizeof(char), 1024, repair_fp);  
					//DMCLOG_D("cmd_buf = %s", cmd_buf);	
					while (fgets(cmd_buf, 1024, repair_fp) != NULL) {
						DMCLOG_D("cmd_buf = %s", cmd_buf);
			       		if (strstr(cmd_buf, REPAIR_OK_PRINT)) {
			            	DMCLOG_D("repair success!!!!!!!!!!!!!!!!!!!!!!");
							repair_ok_flag = 1;
				        }
						memset(cmd_buf, 0, 1024);
				    }
					safe_pclose(repair_fp);

					if(repair_ok_flag)
					{
						memset(cmd, 0 ,32);
						sprintf(cmd, "echo %s >> /tmp/repair_log", dev_group_info->g_dev[i].dev_node);
						DMCLOG_D("cmd = %s", cmd);
						system(cmd);
						Repair_Flag = REPAIR_FAIL_TIME;
					}
					else
					{
						Repair_Flag--;
					}
				}
			}
		}
		else
		{
			DMCLOG_E("umount fail!");
			ret = 1; //can't umount
			//goto EXIT1;
		}

		for(i = 0; dev_group_info->count > i; i++)
		{
			if(NULL != strstr(dev_group_info->g_dev[i].mount_dir, PRIV_MOUNT_HEAD))
			{
				memset(cmd, 0 ,32);
				sprintf(cmd, "block myhotplug add %s", dev_group_info->g_dev[i].dev_node);
				DMCLOG_D("cmd = %s", cmd);
				system(cmd);
				usleep(100000);
			}
		}
		
		memset(cmd, 0 ,32);
		sprintf(cmd, "block mount");
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);

		if(repair_ok_flag == 1)
		{
			usleep(500000);
			del_fsck_rec_file();
		}

		memset(cmd, 0 ,32);
		sprintf(cmd, "rm %s", REPAIR_DOING);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
	}
	return ret;
}


int handle_repair(dev_group_t *dev_group_info, dev_info_t *dev_op_info)
{
	int i = 0;
	char cmd[32];
	int repair_fail_flag = 0;
	FILE *repair_fp = NULL;
	char cmd_buf[1024];
	int check_count = 0;
	int ret_value = 0;  //default ok
	
	memset(cmd, 0 ,32);
	sprintf(cmd, "touch %s", REPAIR_DOING);
	DMCLOG_D("cmd = %s", cmd);
	system(cmd);
	/*
	repair_fp = popen(cmd, "r");
	if(NULL == repair_fp)
	{
		DMCLOG_E("handle %s error", cmd);
		return -1;
	}
	safe_pclose(repair_fp);
	*/
	
	for(i = 0; dev_group_info->count > i; i++)
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "fuser -km %s", dev_group_info->g_dev[i].mount_dir);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);

		/*
		memset(cmd, 0 ,32);
		sprintf(cmd, "umount %s", dev_group_info->g_dev[i].mount_dir);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
		
		memset(cmd, 0 ,32);
		sprintf(cmd, "rm -r %s", dev_group_info->g_dev[i].mount_dir);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
		*/
	}
	memset(cmd, 0 ,32);
	sprintf(cmd, "block umount");
	DMCLOG_D("cmd = %s", cmd);
	system(cmd);
	usleep(500000);
	
	check_count = check_dev_group_info();
	if(0 == check_count)
	{
		memset(cmd, 0 ,32);
		sprintf(cmd, "fsck.fat -a %s", dev_op_info->dev_node);
		DMCLOG_D("cmd = %s", cmd);
		repair_fp = popen(cmd, "r");
		if(NULL == repair_fp)
		{
			DMCLOG_E("handle %s error", cmd);
			ret_value = -1;
			goto EXIT1;
		}
		memset(cmd_buf, 0, 1024);
		//fread(cmd_buf, sizeof(char), 1024, repair_fp);  
		//DMCLOG_D("cmd_buf = %s", cmd_buf);	
		while (fgets(cmd_buf, 1024, repair_fp) != NULL) {
			DMCLOG_D("cmd_buf = %s", cmd_buf);
       		if (NULL != strstr(cmd_buf, REPAIR_FAIL_PRINT)) {
            	DMCLOG_D("repair fail!!!!!!!!!!!!!!!!!!!!!!");
				repair_fail_flag = 1;
				ret_value = 2;//fsck fail
	        }
			memset(cmd_buf, 0, 1024);
	    }
		safe_pclose(repair_fp);

		memset(cmd, 0 ,32);
		sprintf(cmd, "echo %s >> /tmp/repair_log", dev_op_info->dev_node);
		DMCLOG_D("cmd = %s", cmd);
		system(cmd);
	}	
	else
	{
		DMCLOG_E("umount fail!");
		ret_value = 1; //can't umount
	}	

EXIT1:
	for(i = 0; dev_group_info->count > i; i++)
	{
		if(NULL != strstr(dev_group_info->g_dev[i].mount_dir, PRIV_MOUNT_HEAD))
		{
			memset(cmd, 0 ,32);
			sprintf(cmd, "block myhotplug add %s", dev_group_info->g_dev[i].dev_node);
			DMCLOG_D("cmd = %s", cmd);
			system(cmd);
			usleep(100000);
		}
	}
	
	memset(cmd, 0 ,32);
	sprintf(cmd, "block mount");
	DMCLOG_D("cmd = %s", cmd);
	system(cmd);

	if(repair_fail_flag != 1)
	{
		usleep(500000);
		del_fsck_rec_file();
	}
	
	memset(cmd, 0 ,32);
	sprintf(cmd, "rm %s", REPAIR_DOING);
	DMCLOG_D("cmd = %s", cmd);
	system(cmd);
	
	return ret_value;
}


