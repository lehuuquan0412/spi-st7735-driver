#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/gpio/consumer.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>



#define DRIVER_AUTHOR                   "quan0412 lehuuquan0412@gmail.com"
#define DRIVER_DESC                     "st7735 driver for LCD-TFT"
#define DRIVER_LICENSE                  "GPL"

#define DEVICE_NAME                     "keyboard"
#define DEVICE_CLASS                    "keyboard_class"

struct keyboard_device
{
    struct device *kdev;
    struct gpio_desc *up;
    struct gpio_desc *down;
    struct gpio_desc *left;
    struct gpio_desc *right;
    int irq_button[4];
    struct cdev cdev;
    struct class *class;
    dev_t dev;
};

volatile int button_value = 0;


static const struct of_device_id keyboard_dt_ids[] = {
    {.compatible = "keyboard",},
    {}
};

static ssize_t keyboard_read(struct file *file, char __user *buffer, size_t len, loff_t *offset)
{
    int ret;
    ret = copy_to_user(buffer, &button_value, sizeof(button_value));

    if (ret != 0)
    {
        pr_info("Fail to send %d characters to user\n", ret);
        return -EFAULT;
    }else {
        button_value = 0;
    }

    return ret;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = keyboard_read,
};


static irqreturn_t button_isr(int irq, void *device_id)
{
    struct keyboard_device *keyboard = device_id;
    
    int i = 0;

    while (i < 4)
    {
        if (irq == keyboard->irq_button[i])
        {
            button_value = i + 1;
            break;
        }else i++;
    }

    return IRQ_HANDLED;
}

static char *my_devnode(struct device *dev, umode_t *mode) {
    if (mode) {
        *mode = 0666; // Set the permissions to 0666
    }
    return NULL;
}

static int button_pdrv_probe(struct platform_device *device)
{
    struct keyboard_device *keyboard;
    int ret;

    keyboard = kmalloc(sizeof(*keyboard), GFP_KERNEL);
    keyboard->kdev = &device->dev;

    if (!(keyboard))
    {
        pr_err("kmalloc failed\n");
        return -1;
    }

    keyboard->up = gpiod_get(keyboard->kdev, "up", GPIOD_IN);
    keyboard->down = gpiod_get(keyboard->kdev, "down", GPIOD_IN);
    keyboard->left = gpiod_get(keyboard->kdev, "left", GPIOD_IN);
    keyboard->right = gpiod_get(keyboard->kdev, "right", GPIOD_IN);

    keyboard->irq_button[0] = gpiod_to_irq(keyboard->up);
    keyboard->irq_button[1] = gpiod_to_irq(keyboard->down);
    keyboard->irq_button[2] = gpiod_to_irq(keyboard->left);
    keyboard->irq_button[3] = gpiod_to_irq(keyboard->right);

    ret = request_irq(keyboard->irq_button[0], button_isr, IRQF_TRIGGER_RISING, "button_irq_up", keyboard);
    ret = request_irq(keyboard->irq_button[1], button_isr, IRQF_TRIGGER_RISING, "button_irq_down", keyboard);
    ret = request_irq(keyboard->irq_button[2], button_isr, IRQF_TRIGGER_RISING, "button_irq_left", keyboard);
    ret = request_irq(keyboard->irq_button[3], button_isr, IRQF_TRIGGER_RISING, "button_irq_right", keyboard);

    ret = alloc_chrdev_region(&keyboard->dev, 0, 1, DEVICE_NAME);
    if (ret < 0)
    {
        pr_err("Failed to allocate character device region\n");
        return ret;
    }

    cdev_init(&keyboard->cdev, &fops);
    keyboard->cdev.owner = THIS_MODULE;

    ret = cdev_add(&keyboard->cdev, keyboard->dev, 1);
    if (ret)
    {
        pr_err("Failed to add character device\n");
        unregister_chrdev_region(keyboard->dev, 1);
        return ret;
    }

    keyboard->class = class_create(THIS_MODULE, DEVICE_CLASS);
    if (IS_ERR(keyboard->class))
    {
        pr_err("Failed to create class\n");
        cdev_del(&keyboard->cdev);
        unregister_chrdev_region(keyboard->dev, 1);
        return PTR_ERR(keyboard->class);
    }

    keyboard->class->devnode = my_devnode;
    keyboard->kdev = device_create(keyboard->class, NULL, keyboard->dev, NULL, DEVICE_NAME);

    platform_set_drvdata(device, keyboard);
    pr_info("Keyboard success !!!\n");

    return 0;
}

static int button_pdrv_remove(struct platform_device *device)
{
    struct keyboard_device *keyboard = platform_get_drvdata(device);

    device_destroy(keyboard->class, keyboard->dev);
    class_destroy(keyboard->class);
    cdev_del(&keyboard->cdev);
    unregister_chrdev_region(keyboard->dev, 1);

    free_irq(keyboard->irq_button[0], keyboard);
    free_irq(keyboard->irq_button[1], keyboard);
    free_irq(keyboard->irq_button[2], keyboard);
    free_irq(keyboard->irq_button[3], keyboard);

    gpiod_put(keyboard->up);
    gpiod_put(keyboard->down);
    gpiod_put(keyboard->right);
    gpiod_put(keyboard->left);
    
    pr_info("Exit !!!\n");
    return 0;
}

MODULE_DEVICE_TABLE(of, keyboard_dt_ids);

static struct platform_driver button_driver = {
    .probe = button_pdrv_probe,
    .remove = button_pdrv_remove,
    .driver = {
        .name = "keyboard",
        .of_match_table = of_match_ptr(keyboard_dt_ids),
        .owner = THIS_MODULE, 
    },
};

module_platform_driver(button_driver);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_VERSION("1.0");