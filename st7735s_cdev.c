#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "st7735s_sysfs.c"
#include "st7735s_types.h"

static int major = 0;
static int minor = 0;
static int count = 1;

static struct cdev *cdev;
size_t size_buffer;

static char *buffer;

static struct st7735s_device_functions device_functions;

static ssize_t st7735s_cdev_open(struct inode *inode, struct file *filp)
{
        return 0;
}

static ssize_t st7735s_cdev_release(struct inode *inode, struct file *filp)
{
        return 0;
}

static ssize_t st7735s_cdev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
        ssize_t ret;

        int remain = size_buffer - (int) (*f_pos);
        if (count > remain) {
                count = remain;
        }

        if (raw_copy_to_user(buf, buffer + *f_pos, count)) {
                ret = -EFAULT;
                goto out;
        }

        *f_pos += count;
        
        return count;

out:
        pr_err("st7735s: character dev read error\n");
        return ret;
}

static ssize_t st7735s_cdev_write(struct file *filp, const char __user *buf,
                                  size_t count, loff_t *f_pos)
{
        ssize_t ret;

        int remain = size_buffer - (int) (*f_pos);

        if (count > remain)
        {
                ret = -EIO;
                goto out;
        }

        if (raw_copy_from_user(buffer + *f_pos, buf, count)) {
                ret = -EFAULT;
                goto out;
        }
        *f_pos += count;

        return count;

out:
        pr_err("st7735s: character dev write error\n");
        return ret;
}

static struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = st7735s_cdev_open,
        .release = st7735s_cdev_release,
        .read = st7735s_cdev_read,
        .write = st7735s_cdev_write,
};

void st7735s_cdev_remove(void)
{
        dev_t devno = MKDEV(major, minor);

        st7735s_sysfs_remove();

        if (cdev) {
                cdev_del(cdev);
                cdev = NULL;
        }

        if (buffer) {
                buffer = NULL;
        }

        unregister_chrdev_region(devno, count);

        pr_info("st7735s: character device removed\n");
}

int st7735s_cdev_init(struct st7735s_device_functions st7735s_functions, char *char_buffer, size_t size) {
        int ret;
        dev_t dev = 0;
        device_functions = st7735s_functions;

        size_buffer = size;
        printk("size buffer %d\n", size_buffer);

        ret = alloc_chrdev_region(&dev, minor, count, ST7735S_DEVICE_NAME);
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
        pr_info("st7735s: character device created major %d minor %d\n", major, minor);

        buffer = char_buffer;

        ret = st7735s_sysfs_init(st7735s_functions, dev);
        if (ret) {
                goto out;
        }

        return 0;

out:
        pr_err("st7735s: character device register error\n");
        return ret;
}
