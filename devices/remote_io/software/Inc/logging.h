/*
 * Logging Defines
 */
#ifndef __THREADLOGGER_H__
#define __THREADLOGGER_H__

#include <stdio.h>
#include <stdarg.h>



#define logger(a, b, args...) logging(a, b, "%s(%s:%d) %s",  __func__,__FILE__, __LINE__, ##args)
void logging(unsigned char logLevel, unsigned int waitTicks, const char *fmt, ...);

#define LOG_ERR 0x03
#define LOG_WAR 0x02
#define LOG_INF 0x01
#define LOG_DEB 0x00

#define showLogLevel LOG_DEB

#endif
