/**
  ******************************************************************************
  * @file           : logger.h
  * @brief          : Header for logger.c file.
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

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

/**
 *  @brief Maximum Logger Buffer Length
 */
#define LOGGER_MAX_BUF_LENGTH 256

/**
 *  @enum t_log_type
 *  @brief Type of log output
 *  @note Limited to 32 types
 */
typedef enum
{
   LOG_CRITICAL = 0,   /*!< critical: system will not function */
   LOG_ERROR,          /*!< error: system will function but in degraded mode */
   LOG_WARNING,        /*!< warning: system will function but there was an unexpected system state detected */
   LOG_MSG,            /*!< msg: Informational message */
   LOG_DEBUG,          /*!< debug: Debug messages */

   MAX_LOG_TYPE        /*!< must be the last t_log_type */
} t_log_type;

/**
 *  @fn logger_printf_fn(t_log_type type, char const * typestring, char const * file, int line, char const * format, ...)
 *  @brief Logger printf backend function (which is used when Logger singleton object not yet instantiated)
 *  @param [ in] type - log type
 *  @param [ in] typestring - string corresponding to log type
 *  @param [ in] file - name of file logging the buffer
 *  @param [ in] line - file line number logging the buffer
 *  @param [ in] format - printf like format string
 *  @param [ in ] ... - printf like arguments
 */
extern void logger_printf_fn(t_log_type type,
                             char const * typestring,
                             char const * file,
                             int line,
                             char const * format, ...);

/**
 *  @def LOG()
 *  @brief LOG Macro used globally to log messages and errors
 *  @param [ in] type - log type
 *  @param [ in] format - printf format string
 *  @param [ in] args... - printf arguments (optional)
 *  @note GNU gcc specific
 */
#define LOG(type, format, args...)                        \
{                                                         \
   	logger_printf_fn(type, #type + sizeof("LOG_")-1,      \
                     __FILE__, __LINE__, format,          \
                     ##args);                             \
}

#endif /* INC_LOGGER_H_ */
