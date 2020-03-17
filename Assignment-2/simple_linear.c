#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
//#include <linux/list.h>
//#include <linux/types.h>
//#include <linux/slab.h>
#include <linux/sched.h>





LIST_HEAD(birthday_list);




/* This function is called when the module is loaded. */
int simple_init(void)
{
	
	printk(KERN_INFO "Loading Module\n");
	struct task_struct *task;	
	for_each_process(task)
	{
	printk(KERN_INFO "pName: %s  - pState: %ld  - pid: %d\n", task->comm,              task->state,task->pid);
	}

	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) 
{

	
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

