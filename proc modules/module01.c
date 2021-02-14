#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define procfs_name "helloworld"

int shared_counter;
EXPORT_SYMBOL(shared_counter);
struct proc_dir_entry *my_proc_file;


static int procfile_show(struct seq_file *m, void *v){
	seq_printf(m, "%s\n", "Merhaba Dunya!!!");
	++shared_counter;
	return 0;
} 

static int procfile_open(struct inode *inode, struct file *file){
	return single_open(file, procfile_show, NULL);
}

static const struct file_operations procfile_fops={
	.owner = THIS_MODULE,
	.open  = procfile_open,
	.read  = seq_read
};


int init_module(){
	shared_counter=0;
	my_proc_file = proc_create(procfs_name, 0644, NULL, &procfile_fops);
	if (my_proc_file == NULL) {
		remove_proc_entry(procfs_name, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}
	printk(KERN_INFO "/proc/%s created\n", procfs_name);
	return 0;
}

void cleanup_module(){
	remove_proc_entry(procfs_name, NULL);
	printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}



