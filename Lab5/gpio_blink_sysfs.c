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
#define CLASS_NAME	"external_led"
#define GPIO_PIN_NUMB 4

#define LED_ON 0x01
#define LED_OFF 0x00

struct timer_list my_timer;
// LED is initially ON
unsigned int led_status = LED_ON;
// Period of time in ms
unsigned int period = 1000;
// Used to verify
unsigned int result = 1;

struct class *external_leds = NULL;
struct device *blink = NULL;

// Function prototype
static void blink_timer(void);

// Store the value of the input in the buffer, then store in the global period
static ssize_t store_period(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count) {

    unsigned int temp;
    sscanf(buf,"%du",&temp);
    period = temp;

    return count;
}

// Device attribute descriptor: <attribute name, permission, NULL, function>
static struct kobj_attribute period_attr = __ATTR(period, 0644, NULL, store_period);

// Init function
static int __init start_blink(void) {

    printk("Blink :: start module :\n");

    // Request a single GPIO with initial configuration HIGH: <PIN, FLAGS, label>
    result = gpio_request_one(GPIO_PIN_NUMB, GPIOF_OUT_INIT_HIGH, "sysfs");

    if (result) {

        printk(KERN_INFO "ERROR :: Failed to initialise the GPIO PIN: %d\n", GPIO_PIN_NUMB);

        return result;
    }

    printk(KERN_INFO "SUCCESS :: GPIO PIN was successfully allocated\n");

    // Initialise the timer
    init_timer(&my_timer);

    // Set expiration time in jiffies - convert period to milliseconds
    my_timer.expires = jiffies + HZ*period/1000;

    // Define the function to be executed upon timer expire
    my_timer.function = blink_timer;

    my_timer.data = 0;

    printk(KERN_INFO "STATUS :: The timer has been initialized\n");

    // Start the timer
    add_timer(&my_timer);

    // Create a struct class structure <Module that owns the class, String for the name of the class>
    external_leds = class_create(THIS_MODULE, CLASS_NAME);

    if (IS_ERR(external_leds)) {

        printk(KERN_ERR "ERROR :: Failed to create device class\n");

        return PTR_ERR(external_leds);
    } else {

        printk(KERN_INFO "SUCCESS :: EXTERNAL_LEDS :: Device class was successfully created\n");
    }

    // Create a device and register it with sysfs:
    // <Class it should be registered to, Parent struct device, dev_t to be added, String for device name>
    blink = device_create(external_leds, NULL, 0, NULL, DEVICE_NAME);

    if (IS_ERR(blink)) {

        // Destroy the struct class structure if the was an error
        class_destroy(external_leds);

        printk(KERN_ERR "ERROR :: Failed to create the device\n");

        return PTR_ERR(blink);
    } else {

        printk(KERN_INFO "SUCCESS :: Device class created successfully\n");
    }

    // Create sysfs attribute file for device: <device, device attribute descriptor>
    result = device_create_file(blink, &period_attr);

    if (result) {

        printk(KERN_ERR "ERROR :: Failed to create the sysfs file\n");
    } else {

        printk(KERN_INFO "SUCCESS :: sysfs file successfully created\n");
    }

    return 0;
}

static void __exit stop_blink(void) {

    // Turn off LED
    gpio_set_value(GPIO_PIN_NUMB, LED_OFF);

    printk(KERN_INFO "BLINK :: LED turned OFF\n");

    // Free the GPIO PIN
    gpio_free(GPIO_PIN_NUMB);

    printk(KERN_INFO "BLINK :: GPIO freed\n");

    // Deactivate the timer
    del_timer(&my_timer);

    printk(KERN_INFO "BLINK :: Timer deactivated\n");

    // Remove the device created by device_create
    device_destroy(external_leds, 0);

    // Unregister the previously registered class
    class_unregister(external_leds);

    // Destroy the struct class structure
    class_destroy(external_leds);

    // Remove sysfs attribute file from its own method
    device_remove_file(blink, &period_attr);

    printk(KERN_INFO "BLINK :: The module has been unloaded\n");
}

static void blink_timer() {

    // Investigate initial value of jiffies
    printk(KERN_INFO "Jiffies %lu :", jiffies);

    // If the LED is on - turn it off / else turn it on
    if (led_status) {

        gpio_set_value(GPIO_PIN_NUMB, LED_OFF);
        led_status = 0;
        printk(KERN_INFO "SET :: LED OFF\n");
    } else {

        gpio_set_value(GPIO_PIN_NUMB, LED_ON);
        led_status = 1;
        printk(KERN_INFO "SET :: LED ON\n");
    }

    // Set expiration time in jiffies - convert period to milliseconds
    my_timer.expires = jiffies + HZ * period / 1000;

    // Start the timer
    add_timer(&my_timer);
}

module_init(start_blink);
module_exit(stop_blink);