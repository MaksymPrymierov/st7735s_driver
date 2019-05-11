#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "st7735s"

static int major = 0;
static int minor = 0;
static int count = 1;

static struct cdev *cdev;
//size_t size_octal_buffer;

static char *octal_buffer;

static struct file_operations fops = {
        .owner = THIS_MODULE,
};

void st7735s_cdev_remove(void)
{
        dev_t devno = MKDEV(major, minor);
        if (cdev) {
                cdev_del(cdev);
                cdev = NULL;
        }

        if (octal_buffer) {
                octal_buffer = NULL;
        }

        unregister_chrdev_region(devno, count);

        pr_info("st7735s: character device removed\n");
}

int st7735s_cdev_init(u16 *hex_buffer) {
        int ret;
        dev_t dev = 0;

  //      size_octal_buffer = size_hex_buffer * 2;

        ret = alloc_chrdev_region(&dev, minor, count, DEVICE_NAME);
        major = MAJOR(dev);

        if (ret < 0) {
                pr_err("st7735s: can't dev a majori\n");
                goto out;
        }

        cdev = cdev_alloc();
        if (!cdev) {
                ret = -ENOMEM;
                pr_err("st7735s: can't alloc cdev\n");
                goto out;
        }

        cdev_init(cdev, &fops);
        cdev->owner = THIS_MODULE;

        ret = cdev_add(cdev, dev, count);
        if (ret) {
                pr_err("st7735s: cdev_add error\n");
                goto out;
        }
        pr_info("st7735s: character device created major %d minor %d\n", minor, major);

        octal_buffer = (u8*)hex_buffer;

        return 0;

out:
        pr_err("st7735s: character device register error\n");
        return ret;
}