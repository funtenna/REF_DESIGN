/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/ioport.h> /* request_region */
#include <asm/io.h> /* ioremap */
#include <linux/ioctl.h> /* .ioctl fops */

#include "gpio.h"
#define DEVICE_NAME "gpio"
MODULE_LICENSE("DUAL BSD/GPL");

/* Declaration of functions */
int gpio_init(void);
void gpio_exit(void);

/* User commands to kernel module */
ssize_t gpio_write(struct file *filp, const char *buf, size_t count,
                   loff_t *f_pos);
long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

/* File operations, has unlocked_ioctl */
struct file_operations gpio_fops = {
    .write = gpio_write,
    .unlocked_ioctl = gpio_ioctl,
};


/* Global variables of the driver */
#define MMAP_OFFSET 0x44c00000
#define MMAP_SIZE   (0x48ffffff-MMAP_OFFSET)
#define GPIO_PIN_SW 38  // GPIO1.6, Beaglebone P8.3

static void *baseAddr;
static void *gpioSetAddr;
static void *gpioClearAddr;
static void *directionAddr;
static long cyclesPerBit;

int gpio_major = 60;

/* Declaration of the init and exit functions */
module_init(gpio_init);
module_exit(gpio_exit);

/* Initialize the pointers and register char device */
int gpio_init(void)
{
    printk("Init GPIO module\n");

    // Register the char device
    int result = register_chrdev(gpio_major, "gpio", &gpio_fops);

    if (result < 0) {
        printk("<1>gpio: cannot obtain major number %d\n", gpio_major);
        return result;
    }

    baseAddr = ioremap(MMAP_OFFSET, MMAP_SIZE);

    // GPIO configuration
    gpioSetAddr = baseAddr + (0x4804c194 - MMAP_OFFSET);
    gpioClearAddr = baseAddr + (0x4804c190 - MMAP_OFFSET);
    directionAddr = baseAddr + (0x4804c134 - MMAP_OFFSET);
    cyclesPerBit = 20000000;

    // Set GPIO38 Pin to OUTPUT (drive data)
    *((volatile unsigned int *)directionAddr) &= 0xffffffbf;

    return 0;
}

/* Unregister the device and undo the memory mapping. */
void gpio_exit(void)
{
    // Unmap the ioremap
    iounmap(baseAddr);

    // Unregister the char device
    unregister_chrdev(gpio_major, "gpio");

    printk("Removed GPIO module\n");
}

/* Write the count bytes of data in buf to GPIO pin 38,
 *
 * For a nonzero value, the pin will be toggled on and off for the configured
 * number of cycles; for a zero, the pin will be held off for the given number
 * of cycles.
 */
ssize_t gpio_write(struct file *filp, const char *buf, size_t count,
                   loff_t *f_pos)
{
    int j, k;

    for (j = 0; j < count; ++j) {
        if (buf[j]) {
            for (k = 0; k < cyclesPerBit; ++k) {
                *((volatile unsigned int *)gpioSetAddr) = 0x40;
                *((volatile unsigned int *)gpioClearAddr) = 0x40;
            }
        } else {
            for (k = 0; k < cyclesPerBit; ++k) {
                *((volatile unsigned int *)gpioClearAddr) = 0x40;
                *((volatile unsigned int *)gpioClearAddr) = 0x40;
            }
        }
    }

    return 0;
}

/* The only valid ioctl option is IOC_SET, which sets the number of cycles per
 * broadcast bit.
 */
long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
        case IOC_SET:
            cyclesPerBit = arg;
            break;

        default:
            return -ENOTTY;
    }

    return 0;
}
