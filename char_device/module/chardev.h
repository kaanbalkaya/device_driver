#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>

#define MAJOR_NUM 100   	//ioctl'de kullanılacağından önceden belirlenmiş olmalı
#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char*)
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char*)
#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)
#define DEVICE_FILE_NAME "mychardev"

#endif

