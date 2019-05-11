#include <linux/init.h>
#include <linux/module.h>

#include <linux/device.h>
#include "st7735s_types.h"

static struct st7735s_device_functions device_functions;

static ssize_t draw_rect_store(struct class *class, struct class_attribute *attr, const char *buf, size_t size)
{
        u16 color = 0xf0f0;
        int x;
        int y;
        int w;
        int h;

        sscanf(buf, "%hx %d %d %d %d", &color, &x, &y, &w, &h);
                device_functions.draw_rect(x, y, w, h, color);

        return size;
}

static ssize_t fill_screen_store(struct class *class, struct class_attribute *attr, const char *buf, size_t size)
{
        u16 color = 0x0000;

        sscanf(buf, "%hx", &color);

        device_functions.fill_screen(color);

        return size;
}

static struct class *attr_class;
static struct class_attribute class_attr_draw_rect = __ATTR_WO(draw_rect);
static struct class_attribute class_attr_fill_screen = __ATTR_WO(fill_screen);

void st7735s_sysfs_remove(void)
{
        if (attr_class) {
                class_remove_file(attr_class, &class_attr_draw_rect);
                class_remove_file(attr_class, &class_attr_fill_screen);
                pr_info("st7735s: sysfs class attributes removed\n");

                class_destroy(attr_class);
                pr_info("st7735s: sysfs class destroyed\n");
        }
}

int st7735s_sysfs_init(struct st7735s_device_functions functions)
{
        int ret;
        device_functions = functions;

        attr_class = class_create(THIS_MODULE, "st7735s");
        if (IS_ERR(attr_class)) {
                ret = PTR_ERR(attr_class);
                pr_err("st7735s: failed to create sysfs class: %d\n", ret);
                goto out;
        }
        pr_info("st7735s: sysfs class created\n");

        class_attr_fill_screen.attr.mode = 0222;
        ret = class_create_file(attr_class, &class_attr_fill_screen);
        if (ret) {
                pr_err("st7735s: failed to created sysfs class attribute direction: %d\n", ret);
                goto out;
        }
        
        class_attr_draw_rect.attr.mode = 0222;
        ret = class_create_file(attr_class, &class_attr_draw_rect);
        if (ret) {
                pr_err("st7735s: failed to created sysfs class attribute direction: %d\n", ret);
                goto out;
        }

        pr_info("st7735s: sysfs class attributes created\n");

        return 0;

out:
        st7735s_sysfs_remove();
        return ret;
}

