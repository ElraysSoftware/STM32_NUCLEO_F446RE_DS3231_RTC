/**
  ******************************************************************************
  * @file           : logger.c
  * @brief          : Provides debug message to an UART for development purposes
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

#include "logger.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "get_time.h"
#include "ds3231.h"

/* Deactivate this code for Release Configurations */
#ifdef DEBUG_LOG
/*
 * logger_printf_fn
 * @brief Logger printf function
 * @param [ in] type - log type
 * @param [ in] typestring - string corresponding to log type
 * @param [ in] file - name of file logging the buffer
 * @param [ in] line - file line number logging the buffer
 * @param [ in] format - printf like format string
 * @param [ in ] ... - printf like arguments
 * @retval - None
 */
void logger_printf_fn(t_log_type type,
                      char const * typestring,
                      char const * file,
                      int line,
                      char const * format, ...)
{
    char buf[LOGGER_MAX_BUF_LENGTH];
    unsigned int buf_loc = 0;
    va_list args;
    char const * filename = file;
    char * pt_c;

    va_start(args, format);

    /* Separate UNIX path from file name */
    if (file != NULL)
    {
        pt_c = (char *)file;

        while ((pt_c = strchr(pt_c, (int)'/')) != NULL)
        {
            pt_c++;
            filename = pt_c;
        }

        /* UNIX path not found? */
        if ((filename == pt_c) && (NULL != pt_c))
        {
            /* Try DOS path */
            while ((pt_c = strchr(pt_c, (int)'\\')) != NULL)
            {
                pt_c++;
                filename = pt_c;
            }
        }
    }

#ifdef HAVE_DS3231_RTC
    /* Format the timestamp */
    buf_loc += snprintf(buf + buf_loc, sizeof(buf) - buf_loc - 1,
                         "%04u-%02u-%02u-%02u:%02u:%02u:%012lu: ",
                         ds3231_get_year(),
                         ds3231_get_month(),
                         ds3231_get_date(),
                         ds3231_get_hour(),
                         ds3231_get_minute(),
                         ds3231_get_second(),
                         get_micros());
#endif

    /* Format the log header */
    buf_loc += snprintf(buf + buf_loc, sizeof(buf) - buf_loc - 1,
                       "%-8s: %-16s:%d ",
                       typestring, (filename == NULL ? "" : filename), line);

    /* Format the user's format string and args adding to the output buffer */
    buf_loc += vsnprintf(buf + buf_loc, sizeof(buf) - buf_loc - 1,
                         format, args);

    /* If no newline, add it */
    if (buf[buf_loc -1] != '\n')
    {
        buf[buf_loc] = '\r';
        buf_loc++;
        buf[buf_loc] = '\n';
        buf_loc++;
        buf[buf_loc] = '\0';
        buf_loc++;
    }

    /* Call logger output function (Transmit out a Serial UART) */
    if (HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_loc - 1, HAL_MAX_DELAY) != HAL_OK)
    {
        errorHandler(ERR_CODE_SYS);
    }

    va_end(args);
}
#endif /* DEBUG_LOG */
