#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>   // For kmalloc() and kfree()
#include <linux/list.h>   // For linked list operations

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Module using Linked List");
MODULE_AUTHOR("Chinthan");

/* Define a structure for birthdays */
struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;  // Linked list pointer
};

/* Declare and initialize the linked list head */
static LIST_HEAD(birthday_list);

/* Module entry function */
static int __init birthday_init(void) {
    struct birthday *person;
    int i;
    
    printk(KERN_INFO "Loading Module: Creating birthday linked list\n");

    // Creating 5 birthday entries
    for (i = 0; i < 5; i++) {
        person = kmalloc(sizeof(*person), GFP_KERNEL);  // Allocate memory
        if (!person) {
            printk(KERN_ALERT "Memory allocation failed!\n");
            return -ENOMEM;
        }

        person->day = i + 1;
        person->month = 1 + i;
        person->year = 2000 + i;
        INIT_LIST_HEAD(&person->list);  // Initialize list node
        list_add_tail(&person->list, &birthday_list); // Add to linked list

        printk(KERN_INFO "Added: %d-%d-%d\n", person->day, person->month, person->year);
    }

    return 0;
}

/* Module exit function */
static void __exit birthday_exit(void) {
    struct birthday *ptr, *next;

    printk(KERN_INFO "Removing Module: Deleting birthday linked list\n");

    list_for_each_entry_safe(ptr, next, &birthday_list, list) {
        printk(KERN_INFO "Removing: %d-%d-%d\n", ptr->day, ptr->month, ptr->year);
        list_del(&ptr->list);  // Remove from list
        kfree(ptr);  // Free allocated memory
    }
}

module_init(birthday_init);
module_exit(birthday_exit);
