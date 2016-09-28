#include "check_disk.h"
#include <signal.h>



/* 检测磁盘挂载状态状态
 * dev_group_info->count 的值大于0，表示磁盘已挂载
 *
 *
 */
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

/* 检测磁盘读写状态
 * 返回0 表示正常，返回-1 表示磁盘为只读状态
 *
 *
 */

int check_disk_rw(dev_group_t *dev_group_info)
{
	int i;
	int ret = 0;
	char cmd[32];
	int vfat_ro_flag = 0;

	for(i = 0; dev_group_info->count > i; i++)
	{
		if((NULL != strstr(dev_group_info->g_dev[i].rw_flag, RW_RO)) && \
				(NULL != strstr(dev_group_info->g_dev[i].fs_type, FS_TYPE_VFAT)))
		{
			printf("disk is read only\n");
			vfat_ro_flag = 1;
			break;
		}
	}
	
	if(vfat_ro_flag)
		ret = -1;
	else
		ret = 0;
	
	return ret;
		
}

/* 检测磁盘挂载和读写状态
 * 返回0 表示磁盘正常，返回-1 表示磁盘无挂载或者磁盘为只读状态
 *
 *
 */
int check_disk()
{
	dev_group_t g_dev_group;
	int ret = 0;
	
	ret = get_dev_group_info(&g_dev_group);
	if(ret == -1)
	{
		printf("get dev group info error\n");
		return ret;
	}
	
	if(g_dev_group.count > 0)
	{
		
		ret = check_disk_rw(&g_dev_group);
		
	}
	else
	{
		printf("no disk is mounted\n");
		return -1;
	}
	
	return ret;
	
}

/*
int main(void)
{
	if(check_disk() == 0)
		printf("disk is ok\n");
	else
		printf("disk is not ok\n");
	
	return 0;
}
*/
