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

/* External Variables */
extern unsigned int deviceID;
extern uint8_t IP_ADDRESS[4];
extern uint8_t setLED;


const char *webpart1 = {
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
};
const char *webpart2 = {
    "<body><div class='main'>"
        "<h2>STM32F767 MODBUS/TCP Edge Node</h2>"
        "<div style=\"padding-left:10px;\">"
        "Called URL: %s<br>"
        "Build:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; %s %s<br>"
        "Device ID:&nbsp; %010d<br>"
        "Uptime MS:&nbsp; %010d ms<br>"
        "Free heap:&nbsp; %010d bytes<br>"
        "Own IP:&nbsp;&nbsp; &nbsp;&nbsp;%03d.%03d.%03d.%03d <br>"
        "<button onclick=\"location.href='/'\" type='button'>"
        "Refresh</button> &emsp;"
        "<button onclick=\"location.href='/sysrst'\" type='button'>"
        "Soft Reset</button> &emsp;"
        "<button onclick=\"location.href='/set'\" type='button'>"
        "Set LED</button> &emsp;"
        "<button onclick=\"location.href='/reset'\" type='button'>"
        "Reset LED</button>"
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
    err_t err;
    struct netconn *client = NULL;
    struct netconn *nc = netconn_new(NETCONN_TCP);
    char buf[4*BUFLEN];

    if (nc == NULL) {
        logger(LOG_ERR, 100, "Failed to allocate socket");
        vTaskDelete(NULL);
    }
    err = netconn_bind(nc, IP_ADDR_ANY, 80);
    if(err != ERR_OK){
        logger(LOG_ERR, 100, "Failed binding");
    }
    err = netconn_listen(nc);
    if(err != ERR_OK){
        logger(LOG_ERR, 100, "Failed listening");
    }
    /* HTTP Server Listening */
    logger(LOG_DEB, 0, "Starting HTTPD");
    void *data;
    char *getStart = NULL;
    char uri[20];
    u16_t len;
    struct netbuf *nb;
    while (1) {
        osDelay(1);
        err_t err = netconn_accept(nc, &client);
        if (err == ERR_OK) {
            //struct netbuf * nb = netbuf_new ();
            if ((err = netconn_recv(client, &nb)) == ERR_OK) {
                logger(LOG_INF, 100, "HTTP client request");
                osDelay(50);
                err = netbuf_data(nb, &data, &len);
                if(err != ERR_OK){
                    logger(LOG_ERR, 100, "Netbuf_data");
                }
                /* check for a GET request */
                getStart = strstr(data, "GET ");
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
                    logger(LOG_DEB, 1, "Before creating webpart1");
                    snprintf(buf, sizeof(buf), webpart1);
                    err = netconn_write(client, buf, strlen(buf), NETCONN_COPY);
                    if(err != ERR_OK){
                        logger(LOG_ERR, 100, "Error write webpar1");
                    }
                    else{
                        osDelay(50);
                        logger(LOG_DEB, 1, "Before creating webpart2");
                        snprintf(buf, sizeof(buf), webpart2,
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
                        err = netconn_write(client, buf, strlen(buf), NETCONN_COPY);
                        if(err != ERR_OK){
                            logger(LOG_ERR, 100, "Error write webpart2");
                        }
                    }
                    osDelay(50);
                }
                netbuf_free(nb);
            }
            else{
                logger(LOG_ERR, 100, "Error accept");
            }
            netbuf_delete(nb);
            logger(LOG_INF, 100, "Closing http connection");
            netconn_close(client);
            netconn_delete(client);
        }
        osDelay(50);
    }
    logger(LOG_ERR, 100, "HTTP server unexpected closed");
    netconn_close(nc);
    netconn_delete(nc);
}
