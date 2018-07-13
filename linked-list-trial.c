#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/list.h>
#include<linux/slab.h>

struct fox{
    int val;
    struct list_head list;
};

static LIST_HEAD(fox_list);

int
linked_list_init(void)
{
    int i;
    for(i = 0; i < 5; i++)
    {
        struct fox *f = kmalloc(sizeof(*f), GFP_KERNEL);
        f->val = i;
        INIT_LIST_HEAD(&(f->list));
        list_add(&f->list, &fox_list);
    }

    /**
     * Traverse
     */
    struct fox *fentry;
    list_for_each_entry(fentry, &fox_list, list) {
        printk(KERN_INFO "fox #%d\n", fentry->val);
    }
    return 0;
}

void
linked_list_exit(void)
{
    struct fox *fentry;
    struct fox *next;
    list_for_each_entry_safe_reverse(fentry, next, &fox_list, list) {
        printk(KERN_INFO "deleting %d\n", fentry->val);
        list_del(&fentry->list);
        kfree(fentry);
    }
}

module_init(linked_list_init);
module_exit(linked_list_exit);
