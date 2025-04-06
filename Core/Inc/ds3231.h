
/**
  ******************************************************************************
  * @file           : ds3231.h
  * @brief          : Header file for ds3231.c
  * @note           : STM32CubeIDE Environment
  *                 : Library by @eepj www.github.com/eepj
  *
  ******************************************************************************
  * @attention
  *
  * MIT License
  *
  * Copyright (c) 2020 Pan
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  ******************************************************************************
  */

#ifndef DS3231_H
#define DS3231_H
#include "main.h"
/*----------------------------------------------------------------------------*/
#define DS3231_I2C_ADDR 	0x68

#define DS3231_REG_SECOND 	0x00
#define DS3231_REG_MINUTE 	0x01
#define DS3231_REG_HOUR  	0x02
#define DS3231_REG_DOW    	0x03

#define DS3231_REG_DATE   	0x04
#define DS3231_REG_MONTH  	0x05
#define DS3231_CENTURY 		7
#define DS3231_REG_YEAR   	0x06

#define DS3231_A1_SECOND	0x07
#define DS3231_A1_MINUTE	0x08
#define DS3231_A1_HOUR		0x09
#define DS3231_A1_DATE		0x0a

#define DS3231_A2_MINUTE	0x0b
#define DS3231_A2_HOUR		0x0c
#define DS3231_A2_DATE		0x0d

#define DS3231_AXMY			7
#define DS3231_DYDT			6

#define DS3231_REG_CONTROL 	0x0e
#define DS3231_EOSC			7
#define DS3231_BBSQW		6
#define DS3231_CONV			5
#define DS3231_RS2			4
#define DS3231_RS1			3
#define DS3231_INTCN		2
#define DS3231_A2IE			1
#define DS3231_A1IE			0

#define DS3231_REG_STATUS	0x0f
#define DS3231_OSF			7
#define DS3231_EN32KHZ		3
#define DS3231_BSY			2
#define DS3231_A2F			1
#define DS3231_A1F			0

#define DS3231_AGING		0x10

#define DS3231_TEMP_MSB		0x11
#define DS3231_TEMP_LSB		0x12

#define DS3231_TIMEOUT		HAL_MAX_DELAY
/*----------------------------------------------------------------------------*/
typedef enum d3231_rate
{
	DS3231_1HZ, DS3231_1024HZ, DS3231_4096HZ, DS3231_8192HZ
}ds3231_rate;

typedef enum d3231_interrupt_mode
{
	ds3231_SQUARE_WAVE_INTERRUPT, DS3231_ALARM_INTERRUPT
}ds3231_interrupt_mode;

typedef enum d3231_state
{
	DS3231_DISABLED, DS3231_ENABLED
}ds3231_state;

typedef enum d3231_alarm_1_mode
{
	DS3231_A1_EVERY_S = 0x0f, DS3231_A1_MATCH_S = 0x0e, DS3231_A1_MATCH_S_M = 0x0c, DS3231_A1_MATCH_S_M_H = 0x08, DS3231_A1_MATCH_S_M_H_DATE = 0x00, DS3231_A1_MATCH_S_M_H_DAY = 0x80,
}ds3231_alarm_1_mode;

typedef enum d3231_alarm_2_mode
{
	DS3231_A2_EVERY_M = 0x07, DS3231_A2_MATCH_M = 0x06, DS3231_A2_MATCH_M_H = 0x04, DS3231_A2_MATCH_M_H_DATE = 0x00, DS3231_A2_MATCH_M_H_DAY = 0x80,
}ds3231_alarm_2_mode;

extern I2C_HandleTypeDef *_ds3231_ui2c;

extern uint8_t ds3231_init(I2C_HandleTypeDef *hi2c);
extern uint8_t ds3231_set_reg_byte(uint8_t reg_addr, uint8_t val);
extern uint8_t ds3231_get_reg_byte(uint8_t reg_addr, uint8_t *reg_value);
extern uint8_t ds3231_get_day_of_week(void);
extern uint8_t ds3231_get_date(void);
extern uint8_t ds3231_get_month(void);
extern uint16_t ds3231_get_year(void);
extern uint8_t ds3231_get_hour(void);
extern uint8_t ds3231_get_minute(void);
extern uint8_t ds3231_get_second(void);
extern uint8_t ds3231_set_day_of_week(uint8_t dow);
extern uint8_t ds3231_set_date(uint8_t date);
extern uint8_t ds3231_set_month(uint8_t month);
extern uint8_t ds3231_set_year(uint16_t year);
extern uint8_t ds3231_set_hour(uint8_t hour_24mode);
extern uint8_t ds3231_set_minute(uint8_t minute);
extern uint8_t ds3231_set_second(uint8_t second);
extern uint8_t ds3231_set_full_time(uint8_t hour_24mode, uint8_t minute, uint8_t second);
extern uint8_t ds3231_set_full_date(uint8_t date, uint8_t month, uint8_t dow, uint16_t year);
extern uint8_t ds3231_decode_BCD(uint8_t bin);
extern uint8_t ds3231_encode_BCD(uint8_t dec);
extern uint8_t ds3231_enable_battery_square_wave(ds3231_state enable);
extern uint8_t ds3231_set_interrupt_mode(ds3231_interrupt_mode mode);
extern uint8_t ds3231_set_rate_select(ds3231_rate rate);
extern uint8_t ds3231_enable_oscillator(ds3231_state enable);
extern uint8_t ds3231_enable_alarm_2(ds3231_state enable);
extern uint8_t ds3231_set_alarm_2_mode(ds3231_alarm_2_mode alarm_mode);
extern uint8_t ds3231_clear_alarm_2_flag(void);
extern uint8_t ds3231_set_alarm_2_minute(uint8_t minute);
extern uint8_t ds3231_set_alarm_2_hour(uint8_t hour_24mode);
extern uint8_t ds3231_set_alarm_2_date(uint8_t date);
extern uint8_t ds3231_set_alarm_2_day(uint8_t day);
extern uint8_t ds3231_enable_alarm_1(ds3231_state enable);
extern uint8_t ds3231_set_alarm_1_mode(ds3231_alarm_1_mode alarm_mode);
extern uint8_t ds3231_clear_alarm_1_flag(void);
extern uint8_t ds3231_set_alarm_1_second(uint8_t second);
extern uint8_t ds3231_set_alarm_1_minute(uint8_t minute);
extern uint8_t ds3231_set_alarm_1_hour(uint8_t hour_24mode);
extern uint8_t ds3231_set_alarm_1_date(uint8_t date);
extern uint8_t ds3231_set_alarm_1_day(uint8_t day);
extern uint8_t ds3231_enable_32kHz_output(ds3231_state enable);
extern uint8_t ds3231_is_oscillator_stopped(void);
extern uint8_t ds3231_is_32kHz_enabled(void);
extern uint8_t ds3231_is_alarm_1_triggered(void);
extern uint8_t ds3231_is_alarm_2_triggered(void);
extern int8_t ds3231_get_temperature_integer(uint8_t *temp_whole);
extern uint8_t ds3231_get_temperature_fraction(uint8_t *temp_frac);

#endif /* DS3231_H */
