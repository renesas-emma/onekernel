/*
 *  File Name       : arch/arm/mach-emxx/include/mach/emev_board.h
 *  Function        : emxx_board
 *  Release Version : Ver 1.00
 *  Release Date    : 2010/02/05
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
 * Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA.
 *
 */

#ifndef __ASM_ARCH_EMEV_BOARD_H
#define __ASM_ARCH_EMEV_BOARD_H

#include <mach/hardware.h>

/* CS0 */
#define EMEV_FLASH_BASE		EMXX_BANK0_BASE
#define EMEV_FLASH_SIZE		(SZ_64M)

/* CS1 */
#define EMEV_ETHER_OFFSET	0x00000000
#define EMEV_ETHER_SIZE		SZ_4K
#define EMEV_ETHER_BASE		(EMXX_BANK1A_BASE + EMEV_ETHER_OFFSET)

/* CS2 */
#define EMEV_NAND_OFFSET	0x10000000
#define EMEV_NAND_SIZE		SZ_128M
#define EMEV_NAND_BASE		(EMXX_BANK0_BASE + EMEV_NAND_OFFSET)

#define EMEV_NAND_DATA_BASE	(EMEV_NAND_BASE + 0x00000000)
#define EMEV_NAND_COMMAND_BASE	(EMEV_NAND_BASE + 0x00020000)
#define EMEV_NAND_ADDRESS_BASE	(EMEV_NAND_BASE + 0x00040000)

/* I2C name & slave address */
#define I2C_SLAVE_RTC_NAME	"rtc8564"
#define I2C_SLAVE_RTC_ADDR	0x51
#define I2C_SLAVE_EXTIO1_NAME	"max7318_1"
#define I2C_SLAVE_EXTIO1_ADDR	0x20
//#define I2C_SLAVE_EXTIO2_NAME	"max7318_2"
//#define I2C_SLAVE_EXTIO2_ADDR	0x21
#define I2C_SLAVE_CAM_NAME	"camera"
#define I2C_SLAVE_CAM_ADDR	0x50

#define I2C_SLAVE_CAM_NAME_GC0307       "gc0307"
#define I2C_SLAVE_CAM_ADDR_GC0307       0x21
#define I2C_SLAVE_CAM_AF_NAME	"camera_af"
#define I2C_SLAVE_CAM_AF_ADDR	0x0C
#define I2C_SLAVE_HDMI_NAME	"adv7523"
#define I2C_SLAVE_HDMI_ADDR	0x39
#define I2C_SLAVE_SPDIF_NAME	"cs8427"
#define I2C_SLAVE_SPDIF_ADDR	0x10
//#define I2C_SLAVE_CODEC_NAME	"ak4648"
//#define I2C_SLAVE_CODEC_ADDR	0x13
#define I2C_SLAVE_CODEC_NAME	"ALC5621"
#define I2C_SLAVE_CODEC_ADDR	0x1a
#define I2C_SLAVE_NTSC_ENC_NAME	"adv7179"
#define I2C_SLAVE_NTSC_ENC_ADDR	0x2B
#define I2C_SLAVE_NTSC_DEC_NAME	"ad8856"
#define I2C_SLAVE_NTSC_DEC_ADDR	0x44
#define I2C_SLAVE_I2C_PIXCIR_NAME	"pixcir168"
#define I2C_SLAVE_I2C_PIXCIR_ADDR	0x5c
#if defined(CONFIG_INPUT_TOUCH_BUTTON)
#define I2C_SLAVE_I2C_BUTTON_NAME	"tb_i2c"
#define I2C_SLAVE_I2C_BUTTON_ADDR	0x10
#endif
#if defined(CONFIG_AXP192)
#define I2C_SLAVE_I2C_AXP192_NAME	"axp192"
#define I2C_SLAVE_I2C_AXP192_ADDR	0x34
#endif
#if defined(CONFIG_EMXX_SENSORS)
#define	EMXXSENSORS_PLATFORM_NAME	"emxx_sensors_plat"
#endif
#if defined(CONFIG_INPUT_BMA220_G_SENSORS)//st g_sensor
#define I2C_SLAVE_BMA220_ADDR		0x0B
#endif                                                  


#endif	/* __ASM_ARCH_EMEV_BOARD_H */
