/**
  ******************************************************************************
  * @file           : get_time.h
  * @brief          : Header for get_time.c file.
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
#ifndef INC_GET_TIME_H_
#define INC_GET_TIME_H_

#include <stdint.h>

#define get_millis() HAL_GetTick()
#define delay_ms(t) HAL_Delay(t)

uint32_t get_micros(void);
uint32_t get_micros_isr(void);

#endif /* INC_GET_TIME_H_ */
