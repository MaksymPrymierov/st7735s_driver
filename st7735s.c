#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maxim Primerov <primerovmax@gmail.com");
MODULE_DESCRIPTION("Driver for st7735 display");

static void __exit st7735s_exit(void)
{
        pr_info("st7735s: module exited\n");
}

static int __init st7735s_init(void)
{
        pr_info("st7735s: module loaded\n");

        return 0;
}

module_init(st7735s_init);
module_exit(st7735s_exit);
