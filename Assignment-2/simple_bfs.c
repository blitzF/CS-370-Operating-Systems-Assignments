#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/sched.h>


void BFS(struct task_struct *task)
{
	struct task_struct *nxt;
	struct list_head *list;
	list_for_each(list,&task->children)
	{
	nxt = list_entry(list,struct task_struct, sibling);
  printk(KERN_INFO "pName: %s  - pState: %ld  - pid: %d\n",                                  task->comm,task->state,task->pid);
	BFS(nxt);
	}
	
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
	
	printk(KERN_INFO "Loading Module\n");
	BFS(&init_task);
	//list_for_each(list,&task)
	//{
	//printk(KERN_INFO "pName: %s  - pState: %ld  - pid: %d\n",                       task->comm,task->state,task->pid);
	//task = list_entry(list,struct task_struct, sibling);
	//}
	

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

