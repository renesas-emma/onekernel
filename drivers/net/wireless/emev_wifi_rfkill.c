/*
 *  File Name          : driver/net/emev-rfkill.c
 *  Function           : emev_board
 *  Release Version    : Ver 1.07
 *  Release Date       : 2010/10/29
 *
 * Copyright (C) 2010 Renesas Electronics Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <mach/hardware.h>
#include <mach/smu.h>
#include <linux/gpio.h>
#include <linux/rfkill.h>
#include <linux/platform_device.h>
#include <linux/emev-rfkill.h>
#include <linux/delay.h>

//#define DEBUG
#define TAG "WIFI_RFKILL "

#ifdef DEBUG
#define debug_print(fmt, args...)	\
		printk( TAG "%s:%d: " fmt , __FUNCTION__ , __LINE__ , ## args);
#else
#define debug_print(fmt, args...)
#endif

#define WIFI_BT_EN	BCM_WLAN_BT_EN
#define WIFI_RST	BCM_WLAN_RST
#define BT_RST		BCM_BT_RST
#define WIFI_PDN 	BCM_WLAN_PDN	

extern void sdio1_connect(void);
extern void sdio1_disconnect(void);

static void emev_wifi_poweroff(int op)
{
	debug_print("emev_wifi_poweroff\n");

	u32 pull_value = 0;

	if (2 == op) {
		debug_print("emev_wifi_resetoff\n");
		gpio_direction_output(BCM_WLAN_BT_EN, 1);
		gpio_direction_output(BCM_WLAN_RST, 0);
		return;
	}

	gpio_direction_output(WIFI_RST, 0);

	if (!gpio_get_value(BT_RST)) {
		gpio_direction_output(WIFI_BT_EN,0);
	} else {
		debug_print("emev_wifi_poweroff,BT UNBLOCKED,EN not change\n");
	}

	pull_value = readl(CHG_PULL16);
	debug_print("\n CHG_PULL16 cur value = %x\n", pull_value);
	pull_value &= 0xFFFFF777;
	pull_value |= 0x00000777;
	writel(pull_value,CHG_PULL16);
	debug_print("\n CHG_PULL16 cur value = %x\n", readl(CHG_PULL16));

	sdio1_disconnect();    
}

static void emev_wifi_poweron(int op)
{
	debug_print("emev_wifi_poweron\n");

	u32 pull_value = 0;

	if (2 == op) {
		debug_print("emev_wifi_reseton\n");
		gpio_direction_output(BCM_WLAN_BT_EN, 1);
		gpio_direction_output(BCM_WLAN_RST, 1);
		return;
	}

	debug_print("\n WIFI_RST def value = %d\n", gpio_get_value(WIFI_RST));
	gpio_direction_output(WIFI_RST, 0);
	mdelay(10);
	debug_print("\n WIFI_RST set value = %d\n", gpio_get_value(WIFI_RST));

	if (!gpio_get_value(WIFI_BT_EN)) {
		gpio_direction_output(WIFI_BT_EN,1);
	}

	debug_print("\n WIFI_BT_EN  value = %d\n", gpio_get_value(WIFI_BT_EN));
	debug_print("\n BT_RST set value = %d\n", gpio_get_value(BT_RST));
	gpio_direction_output(WIFI_RST, 1);
	debug_print("\n WIFI_RST set value = %d\n", gpio_get_value(WIFI_RST));
	pull_value = readl(CHG_PULL16);
	debug_print("\n CHG_PULL16 cur value = %x\n", pull_value);
	pull_value &= 0xFFFFF555;
	pull_value |= 0x00000555;
	writel(pull_value,CHG_PULL16);
	debug_print("\n CHG_PULL16 cur value = %x\n", readl(CHG_PULL16));

	sdio1_connect();
}

int emev_rfkill_set_power(void *data, enum rfkill_user_states state)
{
	debug_print("Rfkill wifi set power:\n");
	
	switch (state) {
		case RFKILL_USER_STATE_UNBLOCKED:
			debug_print("rfkill wifi Unblocked\n");
			emev_wifi_poweron(1);
			break;
		case RFKILL_USER_STATE_SOFT_BLOCKED:
			debug_print("rfkill wifi Blocked\n");
			emev_wifi_poweroff(1);
			break;
		default:
			printk(KERN_ERR "invalid  rfkill state %d\n", state);
	}
	return 0;
}

EXPORT_SYMBOL(emev_wifi_poweron);
EXPORT_SYMBOL(emev_wifi_poweroff);
EXPORT_SYMBOL(emev_rfkill_set_power);

static void wifi_hw_init(void)
{
	writel(readl(CHG_PINSEL_G096)|0x003C0000, CHG_PINSEL_G096);
	writel(readl(CHG_PINSEL_G064)&~0x0000000F, CHG_PINSEL_G064);
	writel(readl(CHG_PINSEL_G032)&~0xE0000000, CHG_PINSEL_G032);
	gpio_direction_output(WIFI_RST, 0);
}

static int emev_rfkill_set_radio_block(void *data, bool blocked)
{
	debug_print("Rfkill wifi set power: %d\n", blocked);
	return 0;
}

static const struct rfkill_ops emev_wifi_rfkill_ops = {
	.set_block = emev_rfkill_set_radio_block,
};

static int emev_rfkill_probe(struct platform_device *pdev)
{
	debug_print("Rfkill wifi probe\n");
	
	int rc=0;
	struct emev_rfkill_platform_data *pdata = pdev->dev.platform_data;

	wifi_hw_init();

	pdata->rfkill = rfkill_alloc("emev_wifi", 
								&pdev->dev, 
								RFKILL_TYPE_WLAN, 
								&emev_wifi_rfkill_ops, 
								NULL);

	if (unlikely(!pdata->rfkill)) {
		return -ENOMEM;
	}

	/* set default status */
	rfkill_set_states(pdata->rfkill, true, true);

	rc = rfkill_register(pdata->rfkill);

	if (unlikely(rc)) {
		rfkill_destroy(pdata->rfkill);
	}

	return 0;
}


static int emev_rfkill_remove(struct platform_device *pdev)
{
	struct emev_rfkill_platform_data *pdata = pdev->dev.platform_data;

	rfkill_unregister(pdata->rfkill);
	rfkill_destroy(pdata->rfkill);

	return 0;
}

static struct platform_driver emev_rfkill_platform_driver = {
	.probe = emev_rfkill_probe,
	.remove = emev_rfkill_remove,
	.driver.name = "emev-wifirfkill",
	.driver.bus  = &platform_bus_type,
};

static int __init emev_rfkill_init(void)
{
	int ret;
	ret=platform_driver_register(&emev_rfkill_platform_driver);
	debug_print("Rfkill wifi init %d\n", ret);
	
	return ret;
}

static void __exit emev_rfkill_exit(void)
{
	platform_driver_unregister(&emev_rfkill_platform_driver);
}

module_init(emev_rfkill_init);
module_exit(emev_rfkill_exit);

MODULE_ALIAS("platform:emev");
MODULE_DESCRIPTION("emev-wifirfkill");
MODULE_LICENSE("GPL");

