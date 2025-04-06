
/**
  ******************************************************************************
  * @file           : ds3231.c
  * @brief          : An STM32 HAL library written for the DS3231 real-time clock IC
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

#include "ds3231.h"
#include "main.h"
#ifdef __cplusplus
extern "C"{
#endif

I2C_HandleTypeDef *_ds3231_ui2c;

/**
 * @brief Initializes the DS3231 module. Set clock halt bit to 0 to start timing.
 * @param hi2c User I2C handle pointer.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_init(I2C_HandleTypeDef *hi2c)
{
    uint8_t retval = 0;
	_ds3231_ui2c = hi2c;
	if (ds3231_enable_alarm_1(DS3231_DISABLED) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_enable_alarm_2(DS3231_DISABLED) != 0)
	    {
	        retval = 1;
	    }
	    else
	    {
	        if (ds3231_clear_alarm_1_flag() != 0)
	        {
	            retval = 1;
	        }
	        else
	        {
	            if (ds3231_clear_alarm_2_flag() != 0)
	            {
	                retval = 1;
	            }
	            else
	            {
	                if (ds3231_set_interrupt_mode(DS3231_ALARM_INTERRUPT) != 0)
	                {
	                    retval = 1;;
	                }
	            }
	        }
	    }
	}

	return retval;
}

/**
 * @brief Set the byte in the designated DS3231 register to value.
 * @param reg_addr Register address to write.
 * @param val Value to set, 0 to 255.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_reg_byte(uint8_t reg_addr, uint8_t val)
{
    uint8_t retval = 0;
	uint8_t bytes[2] = { reg_addr, val };
	if (HAL_I2C_Master_Transmit(_ds3231_ui2c, DS3231_I2C_ADDR << 1, bytes, 2, DS3231_TIMEOUT) != HAL_OK)
	{
	    retval = 1;
	}
	return retval;
}

/**
 * @brief Gets the byte in the designated DS3231 register.
 * @param reg_addr Register address to read.
 * @param reg_value stored in the register, 0 to 255.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_get_reg_byte(uint8_t reg_addr, uint8_t *reg_value)
{
    uint8_t retval = 0;
	if (HAL_I2C_Master_Transmit(_ds3231_ui2c, DS3231_I2C_ADDR << 1, &reg_addr, 1, DS3231_TIMEOUT) != HAL_OK)
	{
	    retval = 1;
	}
	else
	{
	    if (HAL_I2C_Master_Receive(_ds3231_ui2c, DS3231_I2C_ADDR << 1, reg_value, 1, DS3231_TIMEOUT) != HAL_OK)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Enables battery-backed square wave output at the INT#/SQW pin.
 * @param enable Enable, DS3231_ENABLED or DS3231_DISABLED.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_enable_battery_square_wave(ds3231_state enable)
{
    uint8_t retval = 0;
	uint8_t control;
	if (ds3231_get_reg_byte(DS3231_REG_CONTROL, &control) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_set_reg_byte(DS3231_REG_CONTROL, (control & 0xbf) | ((enable & 0x01) << DS3231_BBSQW)) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set the interrupt mode to either alarm interrupt or square wave interrupt.
 * @param mode Interrupt mode to set, DS3231_ALARM_INTERRUPT or DS3231_SQUARE_WAVE_INTERRUPT.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_interrupt_mode(ds3231_interrupt_mode mode)
{
    uint8_t retval = 0;
	uint8_t control;
	if (ds3231_get_reg_byte(DS3231_REG_CONTROL, &control) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_set_reg_byte(DS3231_REG_CONTROL, (control & 0xfb) | ((mode & 0x01) << DS3231_INTCN)) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set frequency of the square wave output
 * @param rate Frequency to set, DS3231_1HZ, DS3231_1024HZ, DS3231_4096HZ or DS3231_8192HZ.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_rate_select(ds3231_rate rate)
{
    uint8_t retval = 0;
	uint8_t control;
	if (ds3231_get_reg_byte(DS3231_REG_CONTROL, &control) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_set_reg_byte(DS3231_REG_CONTROL, (control & 0xe7) | ((rate & 0x03) << DS3231_RS1)) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Enables clock oscillator.
 * @param enable Enable, DS3231_ENABLED or DS3231_DISABLED.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_enable_oscillator(ds3231_state enable)
{
    uint8_t retval = 0;
	uint8_t control;
	if (ds3231_get_reg_byte(DS3231_REG_CONTROL, &control) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_set_reg_byte(DS3231_REG_CONTROL, (control & 0x7f) | ((!enable & 0x01) << DS3231_EOSC)) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Enables alarm 2.
 * @param enable Enable, DS3231_ENABLED or DS3231_DISABLED.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_enable_alarm_2(ds3231_state enable)
{
    uint8_t retval = 0;

	uint8_t control;
	if (ds3231_get_reg_byte(DS3231_REG_CONTROL, &control) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_set_reg_byte(DS3231_REG_CONTROL, (control & 0xfd) | ((enable & 0x01) << DS3231_A2IE)) != 0)
	    {
	        retval = 1;
	    }
	    else
	    {
	        if (ds3231_set_interrupt_mode(DS3231_ALARM_INTERRUPT) != 0)
	        {
	            retval = 1;
	        }
	    }
	}

	return retval;
}

/**
 * @brief Clears alarm 2 matched flag. Matched flags must be cleared before the next match or the next interrupt will be masked.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_clear_alarm_2_flag(void)
{
    uint8_t retval = 0;
	uint8_t status;
	if (ds3231_get_reg_byte(DS3231_REG_STATUS, &status) != 0)
	{
	    retval = 1;
	}
	else
	{
	    status &= 0xfd;
	    if (ds3231_set_reg_byte(DS3231_REG_STATUS, status & ~(0x01 << DS3231_A2F)) != 0)
	    {
	        retval = 1;
	    }
	}
	return retval;
}

/**
 * @brief Set alarm 2 minute to match. Does not change alarm 2 matching mode.
 * @param minute Minute, 0 to 59.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_2_minute(uint8_t minute)
{
    uint8_t retval = 0;
	uint8_t temp_minute;
	if (ds3231_get_reg_byte(DS3231_A2_MINUTE, &temp_minute) != 0)
	{
	    retval = 1;
	}
	temp_minute &= 0x80;
	uint8_t a2m2 = temp_minute | (ds3231_encode_BCD(minute) & 0x3f);
	if (ds3231_set_reg_byte(DS3231_A2_MINUTE, a2m2) != 0)
	{
	    retval = 1;
	}

	return retval;
}

/**
 * @brief Set alarm 2 hour to match. Does not change alarm 2 matching mode.
 * @param hour Hour to match in 24h format, 0 to 23.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_2_hour(uint8_t hour_24mode)
{
    uint8_t retval = 0;
	uint8_t temp_hour;
	if (ds3231_get_reg_byte(DS3231_A2_HOUR, &temp_hour) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_hour &= 0x80;
	    uint8_t a2m3 = temp_hour | (ds3231_encode_BCD(hour_24mode) & 0x3f);
	    if (ds3231_set_reg_byte(DS3231_A2_HOUR, a2m3) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set alarm 2 date. Alarm 2 can only be set to match either date or day. Does not change alarm 2 matching mode.
 * @param date Date, 0 to 31.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_2_date(uint8_t date)
{
    uint8_t retval = 0;
	uint8_t temp_date;
	if (ds3231_get_reg_byte(DS3231_A2_DATE, &temp_date) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_date &= 0x80;
	    uint8_t a2m4 = temp_date | (ds3231_encode_BCD(date) & 0x3f);
	    if (ds3231_set_reg_byte(DS3231_A2_DATE, a2m4) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set alarm 2 day. Alarm 2 can only be set to match either date or day. Does not change alarm 2 matching mode.
 * @param day Days since last Sunday, 1 to 7.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_2_day(uint8_t day)
{
    uint8_t retval = 0;
	uint8_t temp_day;
	if (ds3231_get_reg_byte(DS3231_A2_DATE, &temp_day) != 0)
	{
	    retval  = 1;
	}
	else
	{
	    temp_day &= 0x80;
	    uint8_t a2m4 = temp_day | (0x01 << DS3231_DYDT) | (ds3231_encode_BCD(day) & 0x3f);
	    if (ds3231_set_reg_byte(DS3231_A2_DATE, a2m4) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set alarm 2 mode.
 * @param alarm_mode Alarm 2 mode, DS3231_A2_EVERY_M, DS3231_A2_MATCH_M, DS3231_A2_MATCH_M_H, DS3231_A2_MATCH_M_H_DATE or DS3231_A2_MATCH_M_H_DAY.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_2_mode(ds3231_alarm_2_mode alarm_mode)
{
    uint8_t retval = 0;
	uint8_t temp_alarm_mode;
	if (ds3231_get_reg_byte(DS3231_A1_MINUTE, &temp_alarm_mode) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_alarm_mode &= 0x7f;
	    if (ds3231_set_reg_byte(DS3231_A2_MINUTE, temp_alarm_mode | (((alarm_mode >> 0) & 0x01) << DS3231_AXMY)) != 0)
	    {
	        retval = 1;
	    }
	    else
	    {
	        uint8_t temp_hour;
	        if (ds3231_get_reg_byte(DS3231_A1_HOUR, &temp_hour) != 0)
	        {
	            retval = 1;
	        }
	        else
	        {
	            temp_hour &= 0x7f;
	            if (ds3231_set_reg_byte(DS3231_A2_HOUR, temp_hour | (((alarm_mode >> 1) & 0x01) << DS3231_AXMY)) != 0)
	            {
	                retval = 1;
	            }
	            else
	            {
	                uint8_t temp_date;
	                if (ds3231_get_reg_byte(DS3231_A1_DATE, &temp_date) != 0)
	                {
	                    retval = 1;
	                }
	                else
	                {
	                    temp_date &= 0x7f;
	                    if (ds3231_set_reg_byte(DS3231_A2_DATE,
	                            temp_date |(((alarm_mode >> 2) & 0x01) << DS3231_AXMY) | (alarm_mode & 0x80)) != 0)
	                    {
	                        retval = 1;
	                    }
	                }
	            }
	        }
	    }
	}

	return retval;
}

/**
 * @brief Enables alarm 1.
 * @param enable Enable, DS3231_ENABLED or DS3231_DISABLED.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_enable_alarm_1(ds3231_state enable)
{
    uint8_t retval = 0;
	uint8_t control;
	if (ds3231_get_reg_byte(DS3231_REG_CONTROL, &control) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_set_reg_byte(DS3231_REG_CONTROL, (control & 0xfe) | ((enable & 0x01) << DS3231_A1IE)) != 0)
	    {
	        retval = 1;
	    }
	    else
	    {
	        if (ds3231_set_interrupt_mode(DS3231_ALARM_INTERRUPT) != 0)
	        {
	            retval = 1;
	        }
	    }
	}

	return retval;
}

/**
 * @brief Clears alarm 1 matched flag. Matched flags must be cleared before the next match or the next interrupt will be masked.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_clear_alarm_1_flag(void)
{
    uint8_t retval = 0;
	uint8_t status;
	if (ds3231_get_reg_byte(DS3231_REG_STATUS, &status) != 0)
	{
	    retval = 1;
	}
	else
	{
	    status &= 0xfe;
	    if (ds3231_set_reg_byte(DS3231_REG_STATUS, status & ~(0x01 << DS3231_A1F)) != 0)
	    {
	        retval = 1;
	    }
	}
	return retval;
}

/**
 * @brief Set alarm 1 second to match. Does not change alarm 1 matching mode.
 * @param second Second, 0 to 59.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_1_second(uint8_t second)
{
    uint8_t retval = 0;
	uint8_t temp_second;
	if (ds3231_get_reg_byte(DS3231_A1_SECOND, &temp_second) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_second &= 0x80;
	    uint8_t a1m1 = temp_second | (ds3231_encode_BCD(second) & 0x3f);
	    if (ds3231_set_reg_byte(DS3231_A1_SECOND, a1m1) != 0)
	    {
	        retval = 1;
	    }
	}
	return retval;
}

/**
 * @brief Set alarm 1 minute to match. Does not change alarm 1 matching mode.
 * @param minute Minute, 0 to 59.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_1_minute(uint8_t minute)
{
    uint8_t retval = 0;
	uint8_t temp_minute;
	if (ds3231_get_reg_byte(DS3231_A1_MINUTE, &temp_minute) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_minute &= 0x80;
	    uint8_t a1m2 = temp_minute | (ds3231_encode_BCD(minute) & 0x3f);
	    if (ds3231_set_reg_byte(DS3231_A1_MINUTE, a1m2) != 0)
	    {
	        retval = 1;
	    }
	}
	return retval;
}

/**
 * @brief Set alarm 1 hour to match. Does not change alarm 1 matching mode.
 * @param hour Hour, 0 to 59.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_1_hour(uint8_t hour_24mode)
{
    uint8_t retval = 0;
	uint8_t temp_hour;
	if (ds3231_get_reg_byte(DS3231_A1_HOUR, &temp_hour) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_hour &= 0x80;
	    uint8_t a1m3 = temp_hour | (ds3231_encode_BCD(hour_24mode) & 0x3f);
	    if (ds3231_set_reg_byte(DS3231_A1_HOUR, a1m3) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set alarm 1 date. Alarm 1 can only be set to match either date or day. Does not change alarm 1 matching mode.
 * @param date Date, 0 to 31.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_1_date(uint8_t date)
{
    uint8_t retval = 0;
	uint8_t temp_date;
	if (ds3231_get_reg_byte(DS3231_A1_DATE, &temp_date) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_date &= 0x80;
	    uint8_t a1m4 = temp_date | (ds3231_encode_BCD(date) & 0x3f);
	    if (ds3231_set_reg_byte(DS3231_A1_DATE, a1m4) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set alarm 1 day. Alarm 1 can only be set to match either date or day. Does not change alarm 1 matching mode.
 * @param day Days since last Sunday, 1 to 7.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_1_day(uint8_t day)
{
    uint8_t retval = 0;
	uint8_t temp_day;
	if (ds3231_get_reg_byte(DS3231_A1_DATE, &temp_day) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_day &= 0x80;
	    uint8_t a1m4 = temp_day | (0x01 << DS3231_DYDT) | (ds3231_encode_BCD(day) & 0x3f);
	    if (ds3231_set_reg_byte(DS3231_A1_DATE, a1m4) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set alarm 1 mode.
 * @param alarm_mode Alarm 1 mode, DS3231_A1_EVERY_S, DS3231_A1_MATCH_S, DS3231_A1_MATCH_S_M, DS3231_A1_MATCH_S_M_H, DS3231_A1_MATCH_S_M_H_DATE or DS3231_A1_MATCH_S_M_H_DAY.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_alarm_1_mode(ds3231_alarm_1_mode alarm_mode)
{
    uint8_t retval = 0;
	uint8_t temp_alarm_mode;
	if (ds3231_get_reg_byte(DS3231_A1_SECOND, &temp_alarm_mode) != 0)
	{
	    retval = 1;
	}
	else
	{
	    temp_alarm_mode &= 0x7f;
	    if (ds3231_set_reg_byte(DS3231_A1_SECOND, temp_alarm_mode | (((alarm_mode >> 0) & 0x01) << DS3231_AXMY)) != 0)
	    {
	        retval = 1;
	    }
	    else
	    {
	        if (ds3231_get_reg_byte(DS3231_A1_MINUTE, &temp_alarm_mode) != 0)
	        {
	            retval = 1;
	        }
	        else
	        {
	            temp_alarm_mode &= 0x7f;
	            if (ds3231_set_reg_byte(DS3231_A1_MINUTE, temp_alarm_mode | (((alarm_mode >> 1) & 0x01) << DS3231_AXMY)) != 0)
	            {
	                retval = 1;
	            }
	            else
	            {
	                if (ds3231_get_reg_byte(DS3231_A1_HOUR, &temp_alarm_mode) != 0)
	                {
	                    retval = 1;
	                }
	                else
	                {
	                    temp_alarm_mode &= 0x7f;
	                    if (ds3231_set_reg_byte(DS3231_A1_HOUR, temp_alarm_mode | (((alarm_mode >> 2) & 0x01) << DS3231_AXMY)) != 0)
	                    {
	                        retval = 1;
	                    }
	                    else
	                    {
	                        if (ds3231_get_reg_byte(DS3231_A1_DATE, &temp_alarm_mode) != 0)
	                        {
	                            retval = 1;
	                        }
	                        else
	                        {
	                            temp_alarm_mode &= 0x7f;
	                            if (ds3231_set_reg_byte(DS3231_A1_DATE, temp_alarm_mode | (((alarm_mode >> 3) & 0x01) << DS3231_AXMY) | (alarm_mode & 0x80)) != 0)
	                            {
	                                retval = 1;
	                            }
	                        }
	                    }
	                }
	            }
	        }
	    }
	}

	return retval;
}

/**
 * @brief Check whether the clock oscillator is stopped.
 * @return Oscillator stopped flag (OSF) bit, 0 or 1.
 */
uint8_t ds3231_is_oscillator_stopped(void)
{
    uint8_t oscillator_stopped = 0;
	ds3231_get_reg_byte(DS3231_REG_STATUS, &oscillator_stopped);
	oscillator_stopped = (oscillator_stopped >> DS3231_OSF) & 0x01;

	return oscillator_stopped;
}

/**
 * @brief Check whether the 32kHz output is enabled.
 * @return EN32kHz flag bit, 0 or 1.
 */
uint8_t ds3231_is_32kHz_enabled(void)
{
    uint8_t is_32khz_enabled = 0;

	ds3231_get_reg_byte(DS3231_REG_STATUS, &is_32khz_enabled);
	is_32khz_enabled = (is_32khz_enabled >> DS3231_EN32KHZ) & 0x01;

	return is_32khz_enabled;
}

/**
 * @brief Check if alarm 1 is triggered.
 * @return A1F flag bit, 0 or 1.
 */
uint8_t ds3231_is_alarm_1_triggered(void)
{
    uint8_t is_alarm1_triggered = 0;

	ds3231_get_reg_byte(DS3231_REG_STATUS, &is_alarm1_triggered);
	is_alarm1_triggered = (is_alarm1_triggered>> DS3231_A1F) & 0x01;

	return is_alarm1_triggered;
}

/**
 * @brief Check if alarm 2 is triggered.
 * @return A2F flag bit, 0 or 1.
 */
uint8_t ds3231_is_alarm_2_triggered(void)
{
    uint8_t is_alarm2_triggered = 0;

	ds3231_get_reg_byte(DS3231_REG_STATUS, &is_alarm2_triggered);
	is_alarm2_triggered = (is_alarm2_triggered >> DS3231_A2F) & 0x01;

	return is_alarm2_triggered;
}

/**
 * @brief Gets the current day of week.
 * @return Days from last Sunday, 0 to 6.
 */
uint8_t ds3231_get_day_of_week(void)
{
    uint8_t day_of_week = 0;
    ds3231_get_reg_byte(DS3231_REG_DOW, &day_of_week);
	day_of_week = ds3231_decode_BCD(day_of_week);

	return day_of_week;
}

/**
 * @brief Gets the current day of month.
 * @return Day of month, 1 to 31.
 */
uint8_t ds3231_get_date(void)
{
    uint8_t day_of_month = 0;

    ds3231_get_reg_byte(DS3231_REG_DATE, &day_of_month);
	day_of_month = ds3231_decode_BCD(day_of_month);

	return day_of_month;
}

/**
 * @brief Gets the current month.
 * @return Month, 1 to 12.
 */
uint8_t ds3231_get_month(void)
{
    uint8_t month = 0;

    ds3231_get_reg_byte(DS3231_REG_MONTH, &month);
    month &= 0x7f;
    month = ds3231_decode_BCD(month);

    return month;
}

/**
 * @brief Gets the current year.
 * @return Year, 2000 to 2199.
 */
uint16_t ds3231_get_year(void)
{
	uint8_t decYear;
	ds3231_get_reg_byte(DS3231_REG_YEAR, &decYear);
	decYear = ds3231_decode_BCD(decYear);
	uint8_t month;
	ds3231_get_reg_byte(DS3231_REG_MONTH, &month);
	uint16_t century = (month >> DS3231_CENTURY) * 100 + 2000;
	return century + decYear;
}

/**
 * @brief Gets the current hour in 24h format.
 * @return Hour in 24h format, 0 to 23.
 */
uint8_t ds3231_get_hour(void)
{
    uint8_t hour;
    ds3231_get_reg_byte(DS3231_REG_HOUR, &hour);

	return ds3231_decode_BCD(hour);
}

/**
 * @brief Gets the current minute.
 * @return Minute, 0 to 59.
 */
uint8_t ds3231_get_minute(void)
{
    uint8_t minute;
    ds3231_get_reg_byte(DS3231_REG_MINUTE, &minute);
	return ds3231_decode_BCD(minute);
}

/**
 * @brief Gets the current second. Clock halt bit not included.
 * @return Second, 0 to 59.
 */
uint8_t ds3231_get_second(void)
{
    uint8_t second;
    ds3231_get_reg_byte(DS3231_REG_SECOND, &second);
	return ds3231_decode_BCD(second);
}

/**
 * @brief Set the current day of week.
 * @param dayOfWeek Days since last Sunday, 1 to 7.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_day_of_week(uint8_t dayOfWeek)
{
    uint8_t retval = 0;
	if (ds3231_set_reg_byte(DS3231_REG_DOW, ds3231_encode_BCD(dayOfWeek)) != 0)
	{
	    retval = 1;
	}

	return retval;
}

/**
 * @brief Set the current day of month.
 * @param date Day of month, 1 to 31.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_date(uint8_t date)
{
    uint8_t retval = 0;
	if (ds3231_set_reg_byte(DS3231_REG_DATE, ds3231_encode_BCD(date)) != 0)
	{
	    retval = 1;
	}

	return retval;
}

/**
 * @brief Set the current month.
 * @param month Month, 1 to 12.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_month(uint8_t month)
{
    uint8_t retval = 0;
	uint8_t century;
	if (ds3231_get_reg_byte(DS3231_REG_MONTH, &century) != 0)
	{
	    retval = 1;
	}
	else
	{
	    century &= 0x80;
	    if (ds3231_set_reg_byte(DS3231_REG_MONTH, ds3231_encode_BCD(month) | century) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Set the current year.
 * @param year Year, 2000 to 2199.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_year(uint16_t year)
{
    uint8_t retval = 0;
	uint8_t century = (year / 100) % 20;
	uint8_t monthReg;
	if (ds3231_get_reg_byte(DS3231_REG_MONTH, &monthReg) != 0)
	{
	    retval = 1;
	}
	else
	{
	    monthReg = (monthReg & 0x7f) | (century << DS3231_CENTURY);
	    if (ds3231_set_reg_byte(DS3231_REG_MONTH, monthReg) != 0)
	    {
	        retval = 1;
	    }
	    else
	    {
	        if (ds3231_set_reg_byte(DS3231_REG_YEAR, ds3231_encode_BCD(year % 100)) != 0)
	        {
	            retval = 1;
	        }
	    }
	}

	return retval;
}

/**
 * @brief Set the current hour, in 24h format.
 * @param hour_24mode Hour in 24h format, 0 to 23.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_hour(uint8_t hour_24mode)
{
    uint8_t retval = 0;
	if (ds3231_set_reg_byte(DS3231_REG_HOUR, ds3231_encode_BCD(hour_24mode & 0x3f)) != 0)
	{
	    retval = 1;
	}

	return retval;
}

/**
 * @brief Set the current minute.
 * @param minute Minute, 0 to 59.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_minute(uint8_t minute)
{
    uint8_t retval = 0;
	if (ds3231_set_reg_byte(DS3231_REG_MINUTE, ds3231_encode_BCD(minute)) != 0)
	{
	    retval = 1;
	}

	return retval;
}

/**
 * @brief Set the current second.
 * @param second Second, 0 to 59.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_second(uint8_t second)
{
    uint8_t retval = 0;
	if (ds3231_set_reg_byte(DS3231_REG_SECOND, ds3231_encode_BCD(second)) != 0)
	{
	    retval = 1;
	}

	return retval;
}

/**
 * @brief Set the current time.
 * @param hour_24mode Hour in 24h format, 0 to 23.
 * @param minute  Minute, 0 to 59.
 * @param second Second, 0 to 59.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_full_time(uint8_t  hour_24mode, uint8_t minute, uint8_t second)
{
    uint8_t retval = 0;

	if (ds3231_set_hour(hour_24mode) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_set_minute(minute) != 0)
	    {
	        retval = 1;
	    }
	    else
	    {
	        if (ds3231_set_second(second) != 0)
	        {
	            retval = 1;
	        }
	    }
	}

	return retval;
}

/**
 * @brief Set the current date, month, day of week and year.
 * @param date Date, 0 to 31.
 * @param month Month, 1 to 12.
 * @param dow Days since last Sunday, 1 to 7.
 * @param year Year, 2000 to 2199.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_set_full_date(uint8_t date, uint8_t month, uint8_t dow, uint16_t year)
{
    uint8_t retval = 0;
	if (ds3231_set_date(date) != 0)
	{
	    retval = 1;
	}
	else
	{
	    if (ds3231_set_month(month) != 0)
	    {
	        retval = 1;
	    }
	    else
	    {
	        if (ds3231_set_day_of_week(dow) != 0)
	        {
	            retval = 1;
	        }
	        else
	        {
	            if (ds3231_set_year(year) != 0)
	            {
	                retval = 1;
	            }
	        }
	    }
	}

	return retval;
}

/**
 * @brief Decodes the raw binary value stored in registers to decimal format.
 * @param bin Binary-coded decimal value retrieved from register, 0 to 255.
 * @return Decoded decimal value.
 */
uint8_t ds3231_decode_BCD(uint8_t bin)
{
	return (((bin & 0xf0) >> 4) * 10) + (bin & 0x0f);
}

/**
 * @brief Encodes a decimal number to binaty-coded decimal for storage in registers.
 * @param dec Decimal number to encode.
 * @return Encoded binary-coded decimal value.
 */
uint8_t ds3231_encode_BCD(uint8_t dec)
{
	return (dec % 10 + ((dec / 10) << 4));
}

/**
 * @brief Enable the 32kHz output.
 * @param enable Enable, DS3231_ENABLE or DS3231_DISABLE.
 * @return 0 = success, otherwise = failure
 */
uint8_t ds3231_enable_32kHz_output(ds3231_state enable)
{
    uint8_t retval = 0;

	uint8_t status;
	if (ds3231_get_reg_byte(DS3231_REG_STATUS, &status) != 0)
	{
	    retval = 1;
	}
	else
	{
	    status &= 0xfb;
	    if (ds3231_set_reg_byte(DS3231_REG_STATUS, status | (enable << DS3231_EN32KHZ)) != 0)
	    {
	        retval = 1;
	    }
	}

	return retval;
}

/**
 * @brief Get the integer part of the temperature.
 * @param temp_whole - Integer part of the temperature, -127 to 127.
 * @return 0 = success, otherwise = failure
 */
int8_t ds3231_get_temperature_integer(uint8_t *temp_whole)
{
    uint8_t retval = 0;
	if (ds3231_get_reg_byte(DS3231_TEMP_MSB, temp_whole) != 0)
	{
	    retval = 1;
	}
	return retval;
}

/**
 * @brief Get the fractional part of the temperature to 2 decimal places.
 * @param Fractional part of the temperature, 0, 25, 50 or 75.
 */
uint8_t ds3231_get_temperature_fraction(uint8_t *temp_frac)
{
    uint8_t retval = 0;

	if (ds3231_get_reg_byte(DS3231_TEMP_LSB, temp_frac) != 0)
	{
	    retval = 1;
	}
	else
	{
	    *temp_frac = (*temp_frac >> 6) * 25;
	}
	return retval;
}

#ifdef __cplusplus
}
#endif
