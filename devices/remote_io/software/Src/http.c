/* Standard Includes */
#include <stdio.h>
#include <string.h>

/* OS Includes */
#include "cmsis_os.h"

/* LwIP Includes */
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"


/* Project Specific Includes */
#include "global.h"
#include "logging.h"
#define BUFLEN 2048 

/* mbedTLS Includes*/
#include "mbedtls/error.h"
#include "mbedtls/net.h"

/* External Variables */
extern unsigned int deviceID;
extern uint8_t IP_ADDRESS[4];
extern uint8_t setLED;

const char *webresponse = {
	"HTTP/1.1 200 OK\r\n"
	"Content-type: text/html\r\n\r\n"
		"<html><head><title>STM32 MODBUS/TCP Edge Node</title>"
		"<style> div.main {"
			"font-family: Monospace;"
			"color: white;"
			"padding: 1%% 1%% 1%% 1%%;"
			"overflow: hidden;"
			"background-color: black;}"
			"div.full {"
			"padding: 1%% 1%% 1%% 0%%;"
			"width: 99%%;}"
			"div.left {"
			"float: left;"
			"width: 49%%;}"
			"div.right {"
			"width: 49%%;"
			"margin-left: 50%%;}"
		"</style></head>"
		"<body><div class='main'>"
			"<h2>STM32F767 MODBUS/TCP Edge Node</h2>"
			"<div style=\"padding-left:10px;\">"
			"Called URL: %s<br>"
			"Build:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; %s %s<br>"
			"Device ID:&nbsp; %010d<br>"
			"Uptime MS:&nbsp; %010d ms<br>"
			"Free heap:&nbsp; %010d bytes<br>"
			"Own IP:&nbsp;&nbsp; &nbsp;&nbsp;%03d.%03d.%03d.%03d <br>"
			"<button onclick=\"location.href='/'\" type='button'>Refresh</button> &emsp;"
			"<button onclick=\"location.href='/sysrst'\" type='button'>Soft Reset</button> &emsp;"
			"<button onclick=\"location.href='/set'\" type='button'>Set LED</button> &emsp;"
			"<button onclick=\"location.href='/reset'\" type='button'>Reset LED</button>"
			"</div>"
		"</div></body></html>"
};

/**                                                                     
 * @brief HTTP Server Task Listening on Port 80
 * @param  pvParameters:                  
 * @retval None                                                         
 */ 
void httpd_task(void *pvParameters)
{
    int ret;
	mbedtls_net_context listen_fd, client_fd;
	mbedtls_net_init( &listen_fd );
	mbedtls_net_init( &client_fd );
    char buf[2*BUFLEN];

    /*
	 * 1. Setup the listening TCP socket
	 */
	if( ( ret = mbedtls_net_bind( &listen_fd, NULL, "80", MBEDTLS_NET_PROTO_TCP ) ) != 0 )
		goto exit;


    /* HTTP Server Listening */
    logger(LOG_DEB, 0, "Starting HTTPD");
    char *getStart = NULL;
    char uri[20];
    while (1)
    {
reset:
		mbedtls_net_free( &client_fd );
		mbedtls_net_init( &client_fd );
        osDelay(1);
        if( ( ret = mbedtls_net_accept( &listen_fd, &client_fd, NULL, 0, NULL ) ) != 0 )
		{
        	logger(LOG_ERR, 100, "Error accept");
			goto reset;
		}
        // TODO: make this timeout!
        if( ( ret = mbedtls_net_recv(&client_fd, (unsigned char*) buf, sizeof(buf)) ) < 0 )
        {
        	logger(LOG_ERR, 100, "Error recieve");
        	goto reset;
        }
		logger(LOG_INF, 100, "HTTP client request");
		osDelay(50);
		/* check for a GET request */
		getStart = strstr(buf, "GET ");
		if (getStart) {
			strncpy(uri, getStart+4, 8);
			logger(LOG_DEB, 0, uri);
			if (!strncmp(uri, "/set", 4)){
				/* set RED led, for example to find board */
				setLED = 1;
			}
			else if (!strncmp(uri, "/reset", 6)){
				/* reset RED led */
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
				setLED = 0;
			}
			else if(!strncmp(uri, "/sysrst", 7)){
				/* Softreset controller after 1 second */
				logger(LOG_DEB, 100, "SOFTRESET");
				osDelay(1000);
				NVIC_SystemReset();
			}

			/* Return Webpage */
			logger(LOG_DEB, 1, "Before creating webparts");
			snprintf(buf, sizeof(buf), webresponse,
					uri,
					__DATE__, __TIME__,
					deviceID,
					HAL_GetTick(),
					(int) xPortGetFreeHeapSize(),
					IP_ADDRESS[0],
					IP_ADDRESS[1],
					IP_ADDRESS[2],
					IP_ADDRESS[3]
					);
			if ( ( ret = mbedtls_net_send(&client_fd, (unsigned char*) buf, strlen(buf)) ) <= 0 )
			{
				logger(LOG_ERR, 100, "Error write webparts");
				goto reset;
			}
			logger(LOG_INF, 100, "HTTP response sent");
			osDelay(50);
		}
	}
	osDelay(50);
    logger(LOG_ERR, 100, "HTTP server unexpected closed");

exit:

#ifdef MBEDTLS_ERROR_C
	if( ret != 0 )
	{
		char msg[200];
		char error_buf[100];
		mbedtls_strerror( ret, error_buf, 100 );
		snprintf(msg, 200, "\n\rLast error was: %d - %s\n", ret, error_buf);
		logger(LOG_INF, 100, msg);
	}
#endif

	mbedtls_net_free( &client_fd );
	mbedtls_net_free( &listen_fd );

}
