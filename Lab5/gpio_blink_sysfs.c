#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>

static struct kobject *blink_kobject;

static ssize_t set_blink(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count) {
    u8 x = 0;
    u8 y = 0;
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    sscanf(buff, "%hhd %hhd %hhd %hhd %hhd", &x, &y, &r, &g, &b);
    pix_dot(x,y,r,g,b);
    return count;
}

static struct kobj_attribute blink_attribute =__ATTR(dot, (S_IWUSR | S_IRUGO), NULL, set_blink);

void blink_sysfs_init(void) {

    printk(KERN_INFO "BLINK: starting sysfs...");
    blink_kobject = kobject_create_and_add("pix", NULL);
    if (sysfs_create_file(blink_kobject, &blink_attribute.attr)) {
        pr_debug("failed to create pix sysfs!\n");
    }
    printk(KERN_INFO "BLINK: starting sysfs done.");
}

void blink_sysfs_exit(void){
    printk(KERN_INFO "BLINK: stopping sysfs...");
    kobject_put(pix_kobject);
    printk(KERN_INFO "BLINK: stopping sysfs done.");
}

