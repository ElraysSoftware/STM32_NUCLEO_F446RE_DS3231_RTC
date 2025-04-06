/**
  ******************************************************************************
  * @file           : serial_menu.c
  * @brief          : Provides a text serial port menu
  * @note           : STM32CubeIDE Environment
  ******************************************************************************
  * @attention
  *
  * MIT License
  *
  * Copyright (c) 2024 Elray's Software LLC, elrays@sbcglobal.net
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "serial_menu.h"
#include "usart.h"
#include "ds3231.h"
#ifdef DEBUG_LOG
#include "logger.h"
#endif /* DEBUG_LOG */

/* Input buffer for RS-232 received line terminated with '\n' */
char rs_232_input_line[MAX_RS_232_INPUT_LINE];
uint32_t rs_232_input_line_index = 0;

/* current menu state */
rs_232_menu_state_t curr_menu_state = MAIN_MENU_STATE;

void rs_232_main_menu(void);
void rs_232_rtc_menu(void);
void rs_232_menu_start(char *menu_title);
void rs_232_menu_item(char menu_item_selector, char *menu_item_string);
void rs_232_menu_end(char *list_of_selectors);
uint32_t get_rs_232_input(char *rs_232_input_line, uint32_t input_line_size);
void rs_232_printf(char *format, ...);

/*
 * RS-232 Menu - drives the RS-232 Menu State Machine
 * @param - none
 * @return - m
 */
void rs_232_menu(void)
{
    switch (curr_menu_state)
    {
    case MAIN_MENU_STATE:
        /* intentional fall through */
    case MAIN_MENU_STATE_WAITING:
        /* intentional fall through */
    default:
        rs_232_main_menu();
        break;
    case RTC_MENU_STATE:
        /* intentional fall through */
    case RTC_MENU_STATE_WAITING:
        rs_232_rtc_menu();
        break;
    }
}

/*
 * RS-232 Main Menu
 */
void rs_232_main_menu(void)
{
    uint32_t chars_read;
    char ch;

    /* print the menu */
    if (curr_menu_state == MAIN_MENU_STATE)
    {
        rs_232_menu_start("Main Menu");

        rs_232_menu_item('r', "RTC Menu");
        rs_232_menu_item('q', "Quit Menu");

        rs_232_menu_end("rq");

        /* now in waiting state */
        curr_menu_state = MAIN_MENU_STATE_WAITING;
    }
    /* wait for user input */
    chars_read = get_rs_232_input(rs_232_input_line, MAX_RS_232_INPUT_LINE);

    if (chars_read > 0)
    {
        ch = rs_232_input_line[0];

        switch (ch)
        {
        case 'q':
            curr_menu_state = MAIN_MENU_STATE;
            break;
        case 'r':
            curr_menu_state = RTC_MENU_STATE;
            break;
        default:
            rs_232_printf("\r\nUnknown selection: %c\r\n", ch);
            curr_menu_state = MAIN_MENU_STATE;
            break;
        }
    }
}

/*
 * RS-232 RTC Menu
 */
void rs_232_rtc_menu(void)
{
    uint32_t chars_read;
    char ch;

    /* print the menu */
    if (curr_menu_state == RTC_MENU_STATE)
    {
        rs_232_menu_start("RTC Menu");

        rs_232_menu_item('g', "Get time/date info");
        rs_232_menu_item('D', "Set day of week (1-7)");
        rs_232_menu_item('d', "Set day of month (1-31)");
        rs_232_menu_item('m', "Set month of year (1-12)");
        rs_232_menu_item('y', "Set year (4 digits)");
        rs_232_menu_item('H', "Set hour (0-23)");
        rs_232_menu_item('M', "Set Minute (0-59)");
        rs_232_menu_item('S', "Set Second (0-59)");
        rs_232_menu_item('q', "Quit Menu");

        rs_232_menu_end("gDdmyHMSq");

        /* now in waiting state */
        curr_menu_state = RTC_MENU_STATE_WAITING;
    }
    /* wait for user input */
    chars_read = get_rs_232_input(rs_232_input_line, MAX_RS_232_INPUT_LINE);

    if (chars_read > 0)
    {
        ch = rs_232_input_line[0];

        switch (ch)
        {
        case 'q':
            curr_menu_state = MAIN_MENU_STATE;
            break;
        case 'g':
            curr_menu_state = RTC_MENU_STATE;
            char *day[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };
            uint8_t temp_whole;
            uint8_t temp_frac;
            ds3231_get_temperature_integer(&temp_whole);
            ds3231_get_temperature_fraction(&temp_frac);
            rs_232_printf("%04d-%02d-%02dT%02d:%02d:%02d %s %d.%02dC\r\n",
                            ds3231_get_year(),
                            ds3231_get_month(),
                            ds3231_get_date(),
                            ds3231_get_hour(),
                            ds3231_get_minute(),
                            ds3231_get_second(),
                            day[ds3231_get_day_of_week()-1],
                            temp_whole,
                            temp_frac);
            break;
        case 'D':
            curr_menu_state = RTC_MENU_STATE;
            uint8_t day_of_week = atoi(&rs_232_input_line[2]);
            if (ds3231_set_day_of_week(day_of_week) != 0)
            {
                rs_232_printf("Set Day of Week %d FAILED\r\n", day_of_week);
            }
            else
            {
                rs_232_printf("Set Day of Week %d PASSED\r\n", day_of_week);
            }
            break;
        case 'd':
            curr_menu_state = RTC_MENU_STATE;
            uint8_t day_of_month = atoi(&rs_232_input_line[2]);
            if (ds3231_set_date(day_of_month) != 0)
            {
                rs_232_printf("Set Day of Month %d FAILED\r\n", day_of_month);
            }
            else
            {
                rs_232_printf("Set Day of Month %d Passed\r\n", day_of_month);
            }
            break;
        case 'm':
            curr_menu_state = RTC_MENU_STATE;
            uint8_t month_of_year = atoi(&rs_232_input_line[2]);
            if (ds3231_set_month(month_of_year) != 0)
            {
                rs_232_printf("Set Month of Year %d FAILED\r\n", month_of_year);
            }
            else
            {
                rs_232_printf("Set Month of Year %d Passed\r\n", month_of_year);
            }
            break;
        case 'y':
            curr_menu_state = RTC_MENU_STATE;
            uint8_t year = atoi(&rs_232_input_line[2]);
            if (ds3231_set_year(year) != 0)
            {
                rs_232_printf("Set Year %d FAILED\r\n", year);
            }
            else
            {
                rs_232_printf("Set Year %d Passed\r\n", year);
            }
            break;
        case 'H':
            curr_menu_state = RTC_MENU_STATE;
            uint8_t hour = atoi(&rs_232_input_line[2]);
            if (ds3231_set_hour(hour) != 0)
            {
                rs_232_printf("Set Hour %d FAILED\r\n", hour);
            }
            else
            {
                rs_232_printf("Set Hour %d Passed\r\n", hour);
            }
            break;
        case 'M':
            curr_menu_state = RTC_MENU_STATE;
            uint8_t minute = atoi(&rs_232_input_line[2]);
            if (ds3231_set_minute(minute) != 0)
            {
                rs_232_printf("Set Minute %d FAILED\r\n", minute);
            }
            else
            {
                rs_232_printf("Set Minute %d Passed\r\n", minute);
            }
            break;
        case 'S':
            curr_menu_state = RTC_MENU_STATE;
            uint8_t second = atoi(&rs_232_input_line[2]);
            if (ds3231_set_second(second) != 0)
            {
                rs_232_printf("Set Second %d FAILED\r\n", second);
            }
            else
            {
                rs_232_printf("Set Second %d Passed\r\n", second);
            }
            break;
        default:
            rs_232_printf("\r\nUnknown selection: %c\r\n", ch);
            curr_menu_state = RTC_MENU_STATE;
            break;
        }
    }
}

/*
 * Menu Start - print the start of the menu
 * @param - menu_title
 * @return - none
 */
void rs_232_menu_start(char *menu_title)
{
    rs_232_printf("%s", "\r\n");
    rs_232_printf("%s", "--------------------------------------------------\r\n");
    rs_232_printf("%s", "           RTC Example Menu\r\n");
    rs_232_printf("%s", "--------------------------------------------------\r\n");
    rs_232_printf("    %s\r\n", menu_title);
}

/*
 * Menu Item - print a single menu item
 * @param - menu_item_selector - character that selects menu item
 * @param - menu_item_string - character string that describes menu item
 * @return - none
 */
void rs_232_menu_item(char menu_item_selector, char *menu_item_string)
{
    rs_232_printf("     %c:    %s\r\n", menu_item_selector, menu_item_string);
}

/*
 * Menu End - print the end of the menu
 * @param list_of_selectors - string contains all the valid selectors for this menu
 * @return - none
 */
void rs_232_menu_end(char *list_of_selectors)
{
    rs_232_printf("Enter letter (%s) followed by Enter Key\r\n", list_of_selectors);
}

/*
 * Get RS-232 Input and perform echo processing
 * @param rs_232_input_line - buffer to put input line
 * @param input_line_size   - size of buffer in characters
 * @return -                  number of characters placed in input buffer
 */
uint32_t get_rs_232_input(char *rs_232_input_line, uint32_t input_line_size)
{
    char read_ch;
    uint32_t end_of_line = 0;
    uint32_t ret_val = 0;

    /* as long as data in the cyclic buffer and room in line buffer */
    while ((rs_232_tail != rs_232_head) && (rs_232_input_line_index < input_line_size))
    {
        /* read a single character */
        read_ch = rs_232_rx_buffer[rs_232_tail];

        /* increment read pointer */
        rs_232_tail = (rs_232_tail + 1) % RS_232_CYCBUFFLENGTH;

        /* de-assert the flag */
        rs_232_rx_buffer_full = 0;

        /* echo to terminal processing */

        /* carriage return */
        if (read_ch == '\r')
        {
            /* send CRLF */
            rs_232_printf("%s", "\n\r");
        }
        /* backspace */
        else if ((read_ch == '') || (read_ch == 0x7F))
        {
            /* backspace out last character */
            if (rs_232_input_line_index > 0)
            {
                rs_232_printf("%c%c%c",'', ' ', '');
                rs_232_input_line_index--;
                rs_232_input_line[rs_232_input_line_index] = '\0';
            }
            continue;
        }
        /* echo everything else */
        else
        {
            rs_232_printf("%c", read_ch);
        }

        /* character processing */
        if (read_ch == '\r')
        {
            read_ch = '\n';
        }

        if (read_ch == '\n')
        {
            /* end of line */
            end_of_line = 1;
            break;
        }

        if (rs_232_input_line_index < input_line_size)
        {
            rs_232_input_line[rs_232_input_line_index] = read_ch;
            rs_232_input_line_index++;
        }
    }

    if (end_of_line == 1)
    {
        ret_val = rs_232_input_line_index;
        rs_232_input_line_index = 0;
    }

    return ret_val;
}

/*
 * Output characters to RS-232 in printf format
 * @param format -            printf format string
 * @param ... -               printf arguments
 * @return -                  number of characters sent to UART
 * @note -                    uses va_args
 */
void rs_232_printf(char *format, ...)
{
    char    tmp_line[128];
    int32_t len;
    va_list args;

    va_start(args, format);
    len = vsnprintf(tmp_line, sizeof(tmp_line)-1, format, args);
    va_end(args);

    HAL_UART_Transmit(&huart3, (uint8_t *)tmp_line, len, 1000);
}
