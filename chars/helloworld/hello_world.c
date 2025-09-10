#include <linux/module.h>      // Core header for loading LKMs into the kernel
#include <linux/init.h>        // Macros like __init and __exit
#include <linux/fs.h>          // For register_chrdev and struct file_operations

#define DEVICE_NAME "Hello World"   // Device name shown in /proc/devices
#define DRIVER_AUTHOR "Kevin Abraham Wilson"
#define DRIVER_DESC   "A simple Hello World Character Driver"

static int major_num;   // Stores the device major number

// ─────────────── Function Prototypes ───────────────

// Called when the device file is opened
static int hello_open(struct inode *inode, struct file *file);

// Called when the device file is closed/released
static int hello_release(struct inode *inode, struct file *file);

// ─────────────── File Operations Table ───────────────
static struct file_operations fops = {
    .owner   = THIS_MODULE,   // Prevents unloading while in use
    .open    = hello_open,
    .release = hello_release,
};

// ─────────────── Open ───────────────
static int hello_open(struct inode *inode, struct file *file)
{
    pr_info("hello_chardev: Device opened\n");
    return 0;   // Success
}

// ─────────────── Release ───────────────
static int hello_release(struct inode *inode, struct file *file)
{
    pr_info("hello_chardev: Device closed\n");
    return 0;   // Success
}

// ─────────────── Module Init ───────────────
static int __init hello_init(void)
{
    // Register char device driver with dynamic major number
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        pr_err("hello_chardev: Failed to register driver\n");
        return major_num;
    }

    pr_info("hello_chardev: Registered with major number %d\n", major_num);
    pr_info("hello_chardev: Create device node with: mknod /dev/%s c %d 0\n", 
             DEVICE_NAME, major_num);
    return 0;  // Success
}

// ─────────────── Module Exit ───────────────
static void __exit hello_exit(void)
{
    unregister_chrdev(major_num, DEVICE_NAME);
    pr_info("hello_chardev: Unregistered driver\n");
}

// ─────────────── Module Metadata ───────────────
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");

module_init(hello_init);
module_exit(hello_exit);
