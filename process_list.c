#include<linux/init.h>
#include<linux/module.h>
#include<linux/miscdevice.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/kernel.h>

#define BUFFER_SIZE 250
#define DEVICE_NAME "process_list"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ambar");
MODULE_DESCRIPTION("HW2: Process List Character Device.");

/*Declaring the functions*/
static ssize_t proc_list_read(struct file *file, char __user * out, size_t size, loff_t * off);
static int proc_list_open(struct inode*,struct file *);
static int proc_list_close(struct inode*, struct file *);
static char* getState(long);

/*Pointer pointing to current process of task_struct.*/
static struct task_struct * curr;

/*
* @brief: Structure defining the File operation.
*/
static struct file_operations process_list_fops = {
	.owner = THIS_MODULE,	
	.open = proc_list_open,
	.read = proc_list_read,
	.release = proc_list_close
};

/*
* @brief: Declaring a Device Struct
*/
static struct miscdevice process_list_device  = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &process_list_fops
};

/*
* @brief: Registers the miscellanous device driver.
*/
static int __init proc_list_init(void){
	int reg_status;
	printk(KERN_INFO "Process_List: Initializing the Process List KO\n");
	reg_status = misc_register(&process_list_device);
	if(reg_status<0){
		printk(KERN_ALERT "Process_List: Registration FAILED!");
		return 0;
	}
	curr=next_task(&init_task);
	return 0;
}

/*
* @brief: exit function which deregisters the device driver.
*/
static void __exit proc_list_exit(void) {
	printk(KERN_INFO "Process_List: Exiting from the Process List KO\n");
	misc_deregister(&process_list_device);
}

/* 
* @brief:Read function of the driver.
* @param :file pointing to file object
* @param: out buffer where data is written
* @param: length
* @param: offset
*/
static ssize_t proc_list_read(struct file *file, char *out, size_t size, loff_t * off)
{
	int status;
	struct task_struct * task;
	char proc[BUFFER_SIZE];
	
	memset(proc,0,sizeof(char)*BUFFER_SIZE);
	printk(KERN_INFO "Process_List: Reading the Prcoess Information...\n");

	for_each_process(task) {
		if(curr==task){
			char* state=getState(task->state);
			memset(proc,0,sizeof(char)*BUFFER_SIZE);
			sprintf(proc, "PID=%d PPID=%d CPU=%d STATE=%s",task->pid,task->parent->pid,task_cpu(task),state);
			printk(KERN_INFO "Sending: %s",proc);
			status = copy_to_user(out, proc, strlen(proc));
			if(status !=0)
			{	
				printk(KERN_ALERT "Process_List: Error in copying data to user!");
				/*Returning negative BAD ADDRESS error code.*/
				return -EFAULT;
			}
			curr=next_task(task);
			break;
		}	
	}
	return strlen(proc);
}

/** 
* @brief: Opens the Character Driver.  
* @param:inode pointing to inode object
* @param: file is the pointer to file object
*/
static int proc_list_open(struct inode * inode,struct file * file) {
	printk(KERN_INFO "Process_List: Device Opened\n");
	/*During opening initializing the current pointer to start with the init_process*/
	curr=next_task(&init_task);
	return 0;
}

/* 
* @brief:Called when the driver is closed
* @param: inode pointing to inode object
* @param: file is the pointer to file object
*/
static int proc_list_close(struct inode * inode,struct file *file) {
	printk(KERN_INFO "Process_List: Device Closed!\n");
	/*While closing reset the current pointer to initailization position.*/
	curr = &init_task;
	return 0;
}

/* 
* @brief: Function converting integer state to string state.
* @param: state representing the integer value
* @return: string represent the process State 
*/
char* getState(long state){
	switch(state) {
		case 0:
			return "TASK_RUNNING";
		case 1:
			return "TASK_INTERRUPTIBLE";
		case 2:
			return "TASK_UNINTERRUPTIBLE";
		case 3:
			return "TASK_INTERRUPTIBLE , TASK_UNINTERRUPTIBLE";
		case 4:
			return "__TASK_STOPPED";
		case 8:
			return "__TASK_TRACED";
		case 15:
			return "TASK_NORMAL , __TASK_STOPPED , __TASK_TRACED";
		case 16:
			return "EXIT_DEAD";
		case 32:
			return "EXIT_ZOMBIE";
		case 63:
			return "TASK_RUNNING , TASK_INTERRUPTIBLE , TASK_UNINTERRUPTIBLE , __TASK_STOPPED , __TASK_TRACED , EXIT_ZOMBIE , EXIT_DEAD";
		case 64:
			return "TASK_DEAD";
		case 96:
			return "EXIT_ZOMBIE , EXIT_DEAD";
		case 128:
			return "TASK_WAKEKILL";	
		case 130:
			return "TASK_WAKEKILL , TASK_UNINTERRUPTIBLE";
		case 132:
			return "TASK_WAKEKILL , __TASK_STOPPED";
		case 136:
			return "TASK_WAKEKILL , __TASK_TRACED";
		case 256:
			return "TASK_WAKING";
		case 512:
			return "TASK_PARKED";
		case 1024:
			return "TASK_NOLOAD";
		case 1026:
			return "TASK_UNINTERRUPTIBLE , TASK_NOLOAD";
		case 2048:
			return "TASK_NEW";
		case 4096:
			return "TASK_STATE_MAX";
		default:
			return "INVALID";
	}
}
/*Register init and exit process*/
module_init(proc_list_init);
module_exit(proc_list_exit);
