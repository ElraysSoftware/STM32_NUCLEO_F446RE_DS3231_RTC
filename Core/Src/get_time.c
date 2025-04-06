/**
  ******************************************************************************
  * @file           : get_time.c
  * @brief          : Provides get time services
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
#include "get_time.h"
#include "stm32f4xx_hal.h"

/**
 * @brief Get Microseconds since start
 * @return Number of microseconds since start up
 */
uint32_t get_micros(void)
{
    uint32_t ms;
    uint32_t st;
    uint32_t range;

    /* Retreive SysTick->VAL, retry on a ms transition because that will reload SysTick */
    do
    {
        ms = get_millis();
        st = SysTick->VAL;
        asm volatile("nop");
        asm volatile("nop");
    } while (ms != get_millis());

    range = (SysTick->LOAD + 1);
    return (ms * 1000) + ((range - st) / (range / 1000));
}
/**
 * @brief Get Microseconds since start
 * @return Number of microseconds since start up
 * @note Used when called from an ISR
 */
uint32_t get_micros_isr(void)
{
    uint32_t st = SysTick->VAL;
    uint32_t pending = SCB->ICSR & SCB_ICSR_PENDSTSET_Msk;
    uint32_t ms = get_millis();

    if (pending == 0)
        ms++;

    uint32_t range = (SysTick->LOAD + 1);
    return (ms * 1000) + ((range - st) / (range / 1000));
}
