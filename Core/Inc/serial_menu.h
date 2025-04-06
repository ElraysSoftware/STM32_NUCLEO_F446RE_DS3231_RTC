/**
  ******************************************************************************
  * @file           : serial_menu.h
  * @brief          : Header for serial_menu.c file.
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
#ifndef INC_SERIAL_MENU_H_
#define INC_SERIAL_MENU_H_

/* RS-232 Input Line Size max */
#define MAX_RS_232_INPUT_LINE 80

typedef enum
{
    MAIN_MENU_STATE = 0,
    MAIN_MENU_STATE_WAITING,
    RTC_MENU_STATE,
    RTC_MENU_STATE_WAITING,
    END_RS_232_STATE
}rs_232_menu_state_t;

extern void rs_232_menu(void);

#endif /* INC_SERIAL_MENU_H_ */
