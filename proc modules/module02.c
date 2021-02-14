#include <linux/module.h>
#include <linux/kernel.h>

extern int shared_counter;

int init_module(){
	
	printk(KERN_INFO "module02 created and shared_counter : %d\n", shared_counter);
	return 0;
}

void cleanup_module(){
	
	printk(KERN_INFO "module02 removed\n");
}

