#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Blink LED module: 1s period");
MODULE_AUTHOR("Michael Nissen");

#define DEVICE_NAME	"blink"
#define CLASS_NAME	"extleds"

struct timer_list timer;
unsigned int gpioLED = 4;
// LED is initially ON
unsigned int ledStatus = 1;
unsigned int result = 1; //used to check is operations worked
unsigned int period = 1000; //period of time in milliseconds

struct class *extleds = NULL;
struct device *blink = NULL;

static void blinking_timer(void);

// FUNCTION THAT ALLOWS TO STORE A NEW ATTRIBUTE VALUE
//static ssize_t dev_attribute_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf);

//funtion called when period changes value
static ssize_t store_period(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count);

//this function allows to store un buffer the value of the changed attribute (period) --> then we store the new value in period
static ssize_t store_period(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count){
    unsigned int temp;
    sscanf(buf,"%du",&temp);
    period = temp;

    return count;
}

//(attribute name, premission, NULL, function to be called when the attribute is canghed
static struct kobj_attribute period_attr = __ATTR(period,0644,NULL,store_period);


static void blinking_timer(){
    printk(KERN_INFO "Jiffies %lu :", jiffies); //look at the starting value for jiffies
    if(ledStatus){
        //the LED is ON
        gpio_set_value(gpioLED,0); // turn it OFF
        ledStatus = 0;
        printk(KERN_INFO "LED OFF...\n");
    }
    else{
        //the LED is OFF
        gpio_set_value(gpioLED,1); //set it ON
        ledStatus = 1;
        printk(KERN_INFO "LED ON...\n");
    }

    //sets new end to timer: HZ*period/1000 allows to convert in milliseconds. EX/ if period = 1000 we will have HZ*1000/1000 = 1*HZ that
    //corresponds to 1 seconds. So putting period = 200 we will have 0.2 HZ that corresponds to 200 ms
    timer.expires = jiffies + HZ*period/1000;

    //set the timer
    add_timer(&timer);
}

static int __init hello_init(void)//init function
{
    printk("Start blinking module :\n");

    //allocate GPIO 4 to OUTPUT as HIGH as first state. This GPIO is connected to the LED
    result = gpio_request_one(gpioLED, GPIOF_OUT_INIT_HIGH, "sysfs"); //gpioLED = 4, GPIOF_OUT_INIT_HIGH allows to start with high value
    if(result){
        printk(KERN_INFO "An error occured initializing the GPIO %d\n",gpioLED);
        return result;
    }
    printk(KERN_INFO "GPIO successfully allocated\n");

    //init timer
    init_timer(&timer);
    timer.expires = jiffies + HZ*period/1000; //when does the timer have to expier? same formula as before
    timer.function = blinking_timer; //the function to be activated once the timer reache its end
    timer.data = 0;
    printk(KERN_INFO "The timer has been initialized\n");

    //set timer
    add_timer(&timer);

    //create the extleds class
    extleds = class_create (THIS_MODULE,CLASS_NAME); //struct (owner of the class, name of the class)
    if(IS_ERR(extleds)){
        printk(KERN_ERR "Failed to register device class\n");
        return PTR_ERR(extleds);
    }
    printk(KERN_INFO "EXTLEDS: device class created correctly\n");

    //create the device blink of the class extleds
    blink = device_create(extleds,NULL,0,NULL, DEVICE_NAME); // struct (class, parent, number, name);
    if(IS_ERR(blink)){
        class_destroy(extleds);
        printk(KERN_ERR "Failed to create the device\n");
        return PTR_ERR(blink);
    }
    printk(KERN_INFO "Device class created successfully !\n");

    //create the period file
    result = device_create_file(blink,&period_attr); //struct ( device, device attribute)
    if(result){
        printk(KERN_ERR "Failed to create the attribute file\n");
    }
    printk(KERN_INFO "Period attribute file created successfully\n");

    return 0;
}

static void __exit hello_exit(void)
{
    //set the led OFF
    gpio_set_value(gpioLED,0);
    printk(KERN_INFO "LED switched OFF\n");
    //deallocate GPIO
    gpio_free(gpioLED);
    printk(KERN_INFO "GPIO deallocated\n");
    //delete timer
    del_timer(&timer);
    printk(KERN_INFO "Timer deleted\n");

    device_destroy(extleds,0);     // remove the device
    class_unregister(extleds);                          // unregister the device class
    class_destroy(extleds);                             // remove the device class
    device_remove_file(blink,&period_attr);			// remove the attribute file

    printk(KERN_INFO "The blinking module is stopped\n");
}

module_init(hello_init);
module_exit(hello_exit);