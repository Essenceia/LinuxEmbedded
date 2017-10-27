#define MODULE   
#include <linux/module.h>       
#include <linux/kernel.h>  
#include <linux/init.h>   

MODULE_AUTHOR("Julia Desmazes");
MODULE_LICENSE("GPLv3");
MODULE_DESCRIPTION("A short and winey hello world");

static int __init hello_hummans(void)
{
printk("Loading hello module...\n");
printk("Hello world I am a module and I am loaded, chears and cookies \n");
return 0;
}

static void __exit hello_end(void)
{
printk(KERN_INFO "Goodbye cruel world, oh why don't you love me?\n");
printk("Module has been murdered...\n");
}

module_init(hello_hummans);
module_exit(hello_end);
