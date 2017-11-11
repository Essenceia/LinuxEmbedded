//
// Created by pookie on 11/8/17.
//
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
//#include <linux/spinlock_types.h>
#include <linux/slab.h>
#include <linux/sched.h>
MODULE_AUTHOR("Julia Desmazes");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Blink LED modules with interupt button");

#define LED_ON 0x01
#define LED_OFF 0x00
//gpio defined
#define GPIO_PIN_NUMB 4
#define BLINK_DELAY HZ/10 /*100 ms*/
#define MIN_BLINK_DELAY HZ/1000
//button define
#define BUTTON_PIN_NUM 5
#define BUTTON_DEFAULT_STATE ((int)0)

typedef struct led_data{
    long blink_delay;
    int led_status;
};


//button irq
static int irq_button_number = 0;
static struct task_struct * kthread= NULL;

inline long decreas_by_10ms(long expires){
    return ((expires > MIN_BLINK_DELAY)? expires-HZ/100 : BLINK_DELAY);
}
void led_handler_processe(int pstatus) {
    printk(KERN_INFO"Blink led");
    //set led status
    pstatus= ((pstatus == LED_ON) ? LED_OFF : LED_ON);
    gpio_set_value(GPIO_PIN_NUMB, pstatus);
}
//irq hanler
irqreturn_t func_irq_button_handler(int irq, int* pstatus, struct led_data* ldata ){
/*
* Button status
*/
    long tmp;
    printk(KERN_INFO"Button pressed");
    //update blink time
    //spin_lock(led_data);
    tmp = (long)(*ldata).blink_delay;
    (*ldata).blink_delay =(long) decreas_by_10ms(tmp);
    //spin_unlock(led_data);
    return IRQ_HANDLED;
}
static int kthread_blink(struct led_data* led_data){
    while(!kthread_should_stop()){
        do{
            long timeout= led_data->blink_delay;
            while(timeout>0){
                timeout= shedule_timeout(timeout);
            }
            led_handler_processe(led_data->led_status);
        }while(0);
        }
    kfree(led_data);
    return 0;
    }


static int __init start_blink(void) {
    int err1,err2;
    struct led_data *data = (struct led_data*)kmalloc(sizeof(struct led_data),GFP_KERNEL);
    data->led_status=0;
    data->blink_delay=BLINK_DELAY;

    printk(KERN_INFO "Loading blink led module, blinking shall start at a rate of 1 second.\n");
    //request gpio
    err1 = gpio_request_one(GPIO_PIN_NUMB, GPIOF_INIT_LOW, "Led pin");//led
    err2 = gpio_request_one(BUTTON_PIN_NUM, GPIOF_DIR_IN, "Button pin");//led

    if(err1 < 0){
        printk(KERN_ALERT"Could not configure pin to output init failed");
        return -1;}
    if(err2 < 0){
        printk(KERN_ALERT"Could not configure button pin to input init failed");
        return -1;}
    //map gpio to irq
    irq_button_number = gpio_to_irq(BUTTON_PIN_NUM);
    if(irq_button_number<0){
        printk(KERN_ALERT"Could link button pin to irq handler");
        return -1;
    }
    //set irq handler
    if(request_irq(irq_button_number,/* The number of the keyboard IRQ on PCs */
                func_irq_button_handler, /* our handler */
                IRQF_SHARED, /* we are willing to share our irq*/
                "button irq handler",
               (void *)(data))<0){
        //error
    }//to allow the removal of our interupt handler because we are using irsq_shared
    //set kthread
    if(kthread= kthread_run(kthread_blink,"Kthread handler for led blink",(void*)&data)<0){
        printk(KERN_ALERT"Could run kthread");
        return -1;
    }
    return 0;
}

static void __exit stop_blink(void) {
    printk(KERN_INFO "Blink module is beeing unloaded , blinking will stop\n");

    //turn off led
    gpio_set_value(GPIO_PIN_NUMB, LED_OFF);
    gpio_free(GPIO_PIN_NUMB);
    gpio_free(BUTTON_PIN_NUM);
    kthread_stop(kthread);
    free_irq(irq_button_number,NULL);
}

module_init(start_blink);
module_exit(stop_blink);

