#ifndef  _REPAIR_H
#define  _REPAIR_H

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>        
#include <linux/wireless.h>
#include <mntent.h>
#include <errno.h>
#include "dmclog.h"

//#define MTAB_PATH "/etc/mtab"
//#define MTAB_PATH "/tmp/mounts"
#define MTAB_PATH "/proc/mounts"
#define SD_MOUNT_HEAD "SD-disk-"
#define USB_MOUNT_HEAD "USB-disk-"
#define PRIV_MOUNT_HEAD "Private-disk-"
#define MOUNT_PATH "/tmp/usb/"
#define DEV_NODE_HEAD "/dev/"
#define FS_TYPE "type"
#define FS_TYPE_VFAT "vfat"
#define RW_RO "ro"
#define MTAB_TAIL "0 0"
#define REPAIR_DOING "/tmp/repair_doing"
#define REPAIR_FAIL "/tmp/repair_fail"
#define REPAIR_OK_PRINT "Performing changes"
#define REPAIR_FAIL_PRINT "Leaving filesystem unchanged"
#define REPAIR_FAIL_TIME 3
#define REPAIR_TRY_TIME 3


extern int Repair_Flag;

enum mount_rw_flag
{
	rw,
	ro,
	other
};

typedef struct dev_info {
	char mount_dir[32];
	char dev_node[32];
	char fs_type[32];
	int rw_flag[32];
}dev_info_t;

typedef struct dev_group{
	dev_info_t g_dev[16];
	int count;
}dev_group_t;

#define safe_free(p) do{\
	if((p) != NULL)\
	{\
		free((p));\
		(p) = NULL;\
	}\
	}while(0)

#define safe_fclose(f) do{\
	if((f) != NULL)\
	{\
		fclose((f));\
		(f) = NULL;\
	}\
	}while(0)

#define safe_close(fd) do{\
	if((fd) > 0)\
	{\
		close((fd));\
		(fd) = -1;\
	}\
	}while(0)

#define safe_pclose(fd) do{\
		if((fd) > 0)\
		{\
			pclose((fd));\
			(fd) = -1;\
		}\
		}while(0)



int get_dev_group_info(dev_group_t *dev_group_info);
int check_repair(dev_group_t *dev_group_info);


#endif
