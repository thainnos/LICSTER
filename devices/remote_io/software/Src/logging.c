/* Thread logging function
 *
 */

#include <stdarg.h>
#include "logging.h"
#include "stdio.h"
#include "stdint.h"
#include "cmsis_os.h"
#include "global.h"
#include "colors.h"

extern QueueHandle_t loggingQueue;  

void logging(unsigned char logLevel, unsigned int waitTicks, const char *fmt, ...){
    char buf[LOGLEN];
    int bufUsed = 0;
    va_list ap;
    va_start(ap, fmt);

    if(showLogLevel <= logLevel){
        if(logLevel == LOG_ERR){  
            bufUsed += snprintf(buf + bufUsed, (int)LOGLEN - bufUsed, "%sERR: ", CRED);
        }
        else if(logLevel == LOG_WAR){
            bufUsed += snprintf(buf + bufUsed, (int)LOGLEN - bufUsed, "%sWAR: ", CYEL);
        }
        else if(logLevel == LOG_INF){
            bufUsed += snprintf(buf + bufUsed, (int)LOGLEN - bufUsed, "%sINF: ", CBLU);
        }
        else if(logLevel == LOG_DEB){
            bufUsed += snprintf(buf + bufUsed, (int)LOGLEN - bufUsed, "%sDEB: ", CMAG);
        }
        else{
            bufUsed += snprintf(buf + bufUsed, (int)LOGLEN - bufUsed, "%s???: ", CRED);
        }

        bufUsed += snprintf(buf + bufUsed, (int)LOGLEN - bufUsed, "%s", CRST);
        bufUsed += vsnprintf(buf + bufUsed, (int) LOGLEN - bufUsed, fmt, ap);

        xQueueSend(loggingQueue, buf, ( TickType_t ) waitTicks);
    }
  
    va_end(ap);
}
