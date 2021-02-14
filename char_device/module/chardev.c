//i/o karakter cihazı yaratacak

#include <linux/kernel.h> 
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>  //get_user ve put_user için 
#include "chardev.h"

#define SUCCESS 0
#define DEVICE_NAME "mychardev"
#define BUF_LEN 80

static int Device_Open = 0;  //cihaz açık mı değil mi
static char Message[BUF_LEN];
static char *Message_Ptr;     

static int device_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "device open");
	if(Device_Open)
		return -EBUSY;
	Device_Open++;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "device release");
	Device_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset){
	int bytes_read = 0;   	
	printk(KERN_INFO "device read");
	if(*Message_Ptr == 0){
		printk(KERN_INFO "there isn't any messages (device_read)");
		return 0;
	}
	while (*Message_Ptr!='\0'){
		put_user(*Message_Ptr++, buffer++);
		length--;
		bytes_read++;
	}
	return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset){
	int i;
	char ch='\0';
	printk(KERN_INFO "device write");
	if(length>BUF_LEN)
		printk(KERN_ALERT "length>BUF_LEN");
	for(i=0; i<length;++i){   
		get_user(Message[i], buffer+i);
	}
	get_user(Message[i], &ch);
	Message_Ptr = Message;
	return i;
}


/* 3.7 den sonra file_operations yapısı içerisindeki ioctl yerini aşağıdakilere bırakmıştır: 
* long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
* long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
*/
/*long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param){
	int i;
	char *temp;
	char ch;
	switch (ioctl_num){
		case IOCTL_SET_MSG:
			temp=(char *)ioctl_param;
			get_user(ch, temp);
			for(i=0; ch&&i<BUF_LEN; i++,temp++)
				get_user(ch, temp);
			device_write(file, (char*)ioctl_param, i, 0);
			break;
		case IOCTL_GET_MSG:
			i=device_read(file, (char *)ioctl_param, 99,0);
			put_user('\0', (char *)ioctl_param+i);
			break;
		case IOCTL_GET_NTH_BYTE:
			return (long) Message[ioctl_param];
			break;
	}
	return (long) SUCCESS;
}*/

struct file_operations fops = {
	.read=device_read,
	.write=device_write,
	//.unlocked_ioctl=device_ioctl,
	.open=device_open,
	.release=device_release
};

int init_module(){
	int ret_val;
	ret_val= register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
	if (ret_val<0){
		printk(KERN_ALERT "%s failed with %d\n", "sorry, registering the character device ", ret_val);
		return ret_val;
	}

	printk(KERN_INFO "%s The major device number is %d. \n", "Registeration is a success", MAJOR_NUM);
	return 0;
}


void cleanup_module(){
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	printk(KERN_INFO "unregister_chrdev is ALWAYS a success bye bye!!! ");
}






