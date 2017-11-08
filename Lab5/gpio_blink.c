#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>

MODULE_AUTHOR("Julia Desmazes");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Blink LED modules");

//timer
struct timer_list my_timer;
struct tty_driver *my_driver;
char kbledstatus = 0;

<<<<<<< HEAD
#define BLINK_DELAY HZ/5
=======
// arm_freq = 1200 for RPIv3
//#define HZ 1200
>>>>>>> b59e16af645a2befd52007d4faea9fbef0608674
#define LED_ON 0x01
#define LED_OFF 0x00
//gpio defined
#define GPIO_PIN_NUMB 4

static void my_timer_func(unsigned long ptr) {
    printk(KERN_INFO "Blink led");
    int *pstatus = (int *)ptr;
    *pstatus= ((*pstatus == LED_ON) ? LED_OFF : LED_ON);
    gpio_set_value(GPIO_PIN_NUMB, *pstatus);
    my_timer.expires = jiffies + HZ;
    add_timer(&my_timer);
}

static int __init start_blink(void) {
    int err;
    
    printk(KERN_INFO "Loading blink led module, blinking shall start at a rate of 1 second.\n");
    
    //init gpio
    err = gpio_direction_output(GPIO_PIN_NUMB, LED_OFF);

    if(err < 0){
<<<<<<< HEAD
        printk(KERN_ALERT "Could not configure pin to output init failed");
        return -1;
=======
        printk(KERN_ALERT"Could not configure pin to output init failed");
        return -1;}
>>>>>>> b59e16af645a2befd52007d4faea9fbef0608674
    //init timer
    init_timer(&my_timer);
    my_timer.function = my_timer_func;
    my_timer.data = (unsigned long)&kbledstatus;
    my_timer.expires = jiffies + HZ;
    add_timer(&my_timer);
    return 0;
}

static void __exit stop_blink(void) {

    printk(KERN_INFO "Blink module is beeing unloaded , blinking will stop\n");
    
    //delete timer
    del_timer(&my_timer);

    //turn off led
    gpio_set_value(GPIO_PIN_NUMB, LED_OFF);
}

module_init(start_blink);
module_exit(stop_blink);
