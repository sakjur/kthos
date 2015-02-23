#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define DEVICE_NAME "int_count"
#define BUFLEN 10
#define DIGIT_BUFLEN 48
static int irq;
static char interface[BUFLEN];
static struct proc_dir_entry* proc_int_count;
static int interrupt_counter = 0;

module_param_string(interface, interface, BUFLEN, 0);
MODULE_PARM_DESC(interface, "A interface which generates interrupts");
module_param(irq, int, 0);
MODULE_PARM_DESC(irq, "The IRQ of the interface");

static irqreturn_t int_handler(int irq, void *dev_id, struct pt_regs *regs) {
    interrupt_counter++;
    return IRQ_NONE;
}

static int device_read(struct seq_file *filep, void *v)
{
    seq_printf(filep, "%d\n", interrupt_counter);
    return 0;
}

static int device_open(struct inode *inode, struct file *file)
{
    return single_open(file, device_read, NULL);
}

static const struct file_operations counter_fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = single_release,
    .llseek= seq_lseek,
    .read = seq_read 
};

static int __init mymodule_init(void) {
    if (request_irq(
            irq,
            (irq_handler_t) &int_handler,
            IRQF_SHARED,
            interface,
            &irq)
        ) {
        printk("Initializing interrupt handler failed for IRQ %d\n", irq);
        return -EIO;
    }
    printk("Successfully initialized interrupt handler for IRQ %d\n", irq);;

    proc_int_count = proc_create(DEVICE_NAME, 0, NULL, &counter_fops);

    if (!proc_int_count) {
        free_irq(irq, &irq);       
        return -ENOMEM;
    }

    return 0;
}

static void __exit mymodule_exit(void) {
    remove_proc_entry(DEVICE_NAME, NULL);

    free_irq(irq, &irq);
    printk("Freeing IRQ %d\n", irq);
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Emil Tullstedt <emiltu@kth.se>");
MODULE_DESCRIPTION("An interrupt counter");

