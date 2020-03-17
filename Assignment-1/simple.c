#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>



struct birthday {
int day;
int month;
int year;
struct list_head list;
};

LIST_HEAD(birthday_list);




/* This function is called when the module is loaded. */
int simple_init(void)
{
	struct birthday* person;
	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 1;
	person->month = 2;
	person->year = 1999;
	INIT_LIST_HEAD(&person->list);
	list_add(&(person->list), &(birthday_list));
	int i;
	for(i=0; i<4;i++)
	{
		//struct birthday *person;
		person = kmalloc(sizeof(*person), GFP_KERNEL);
		person->day = i;
		person->month = i+1;
		person->year=i+2000;
		//INIT_LIST_HEAD(&person->list);
		list_add(&(person->list), &(birthday_list));

	}



	struct birthday *ptr;
	list_for_each_entry(ptr, &birthday_list, list){
		printk(KERN_INFO "%d-%d-%d", ptr->day, ptr->month, ptr->year);
	} 




	printk(KERN_INFO "Loading Module\n");
	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {

	struct birthday *ptr, *next;
	list_for_each_entry_safe(ptr,next,&birthday_list,list){
	printk(KERN_INFO "Deleting Entry\n");
	printk(KERN_INFO "%d-%d-%d", ptr->day, ptr->month, ptr->year);
	list_del(&ptr->list);
	kfree(ptr);
	}
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

