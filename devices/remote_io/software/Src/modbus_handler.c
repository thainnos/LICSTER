/*
   MODBUS HANDLER
   */
#include "lwip/opt.h"
#include "mbedtls/md.h"

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"
#include <stdbool.h>
#include <string.h>
#include "logging.h"
#define TCPECHO_THREAD_PRIO  ( tskIDLE_PRIORITY + 5 )
#define VENDORNAME "HSA_innos"
#define PRODUCTCODE "sensorSTM"
#define MAJORMINORREVISION "Version 0.2"
#define SENDBUFFERSIZE 128
#define CRYPTO_ENABLED 0
extern char discrete_input_buf;
extern char discrete_output_buf;
int parse_modbus_header(void* data, u16_t len, u16_t* transactionID, u16_t* protocolID, u16_t* length, char* unitID, char* functioncode);
int function_code_handler(void* data, char* functioncode, char* senddata, char* senddatalen,  char* unitID);
char logmsg[256];


extern QueueHandle_t loggingQueue;
// #ifdef CRYPTO_ENABLED
mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
const size_t keyLength = 32;
const unsigned char key[32] = {0x2d, 0xe1, 0x1d, 0xd8, 0x4d, 0x4d, 0x46, 0x74, 0x0c, 0x0c, 0x85, 0x3c, 0xd3, 0x79, 0xc8, 0xdd, 0xad, 0x3b, 0x7f, 0x1e, 0x45, 0x71, 0x8d, 0x8c, 0x71, 0x52, 0xf5, 0xd, 0x4a, 0x05a, 0x21, 0xd7};
// #endif

/*!
  \fn static void modbus_serve(struct netconn *conn)
  \brief Handles a new netconn connection. Receives new messages, calls cmd handler function, and replies answer message.
  \param conn The new netconn connection.
  \return None
  */
static void modbus_serve(struct netconn *conn){
  // #ifdef CRYPTO_ENABLED
  uint8_t hmacResult[32];
  mbedtls_md_context_t cryptoctx;
  // #endif
    err_t recv_err;
    err_t err;
    struct netbuf *buf;
    void *data;
    u16_t len;
    u16_t transactionID;
    u16_t protocolID;
    u16_t length;
    char unitID;
    char functioncode = 0;
    int brcvMsgOK = 0;
    char sendbuffer[SENDBUFFERSIZE];
    osDelay(1);
    netconn_set_recvtimeout(conn, 500); //500
    recv_err = netconn_recv(conn, &buf);
    logger(LOG_INF, 100, "Modbus client connected");
    while ( recv_err == ERR_OK)
    {
        do
        {
            char senddatalen = 0;
            err = netbuf_data(buf, &data, &len);
            if(err != ERR_OK){
                logger(LOG_ERR, 100, "Netbuf error");
            }
            if (len < 8) {
                logger(LOG_INF, 1, "Recived incomplete message.");
            }
            else{
                brcvMsgOK = parse_modbus_header(data, len, &transactionID, &protocolID, &length, &unitID, &functioncode);
            }
            if (brcvMsgOK == 0){
                for (int i = 0; i<SENDBUFFERSIZE; i++) {
                    sendbuffer[i] = 0; /*clear send buffer*/
                }
                brcvMsgOK = function_code_handler(data, &functioncode, sendbuffer, &senddatalen, &unitID);
                if (brcvMsgOK != 0){
                    logger(LOG_ERR, 1, "Error in function_code_handler.");
                }
                //#ifdef CRYPTO_ENABLED//create and append hmac for response message
                if(CRYPTO_ENABLED == 1){
                  mbedtls_md_init(&cryptoctx);
                  mbedtls_md_setup(&cryptoctx, mbedtls_md_info_from_type(md_type), 1);
                  mbedtls_md_hmac_starts(&cryptoctx, key, keyLength);
                  mbedtls_md_hmac_update(&cryptoctx, (const unsigned char *) sendbuffer,senddatalen);
                  mbedtls_md_hmac_finish(&cryptoctx, hmacResult);
                  mbedtls_md_free(&cryptoctx);
                  memcpy(&sendbuffer[(unsigned int)senddatalen],hmacResult,32);
                  senddatalen += 32;
                }
                // #endif

                if(netconn_write(conn, sendbuffer, senddatalen, NETCONN_COPY) != ERR_OK){
                    logger(LOG_ERR, 1, "Error netconn_write.");
                }
            }
            osDelay(1);
        }
        while (netbuf_next(buf) >= 0);
        netbuf_free(buf);
        netbuf_delete(buf);
        recv_err = netconn_recv(conn, &buf);
        osDelay(1);
    }
    /* Close connection and discard connection identifier. */
    logger(LOG_INF, 100, "Modbus client disconnected");
    netconn_close(conn);
    netconn_delete(conn);
    conn = NULL;
    vTaskDelete( NULL );
    osDelay(5);
}

/* not used !!!! */
void echo_server_task(void *arg)
{
  osDelay(10);
  struct netconn *conn, *newconn;
  err_t err;
  err_t accept_err, recv_err;
  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);
  logger(LOG_DEB, 1, "Creating modbus_cmd_handler_task.");
  if (conn!=NULL)
  {
      /* Bind connection to modbus port 502. */
      err = netconn_bind(conn, NULL, 502);
      if(err != ERR_OK){
          /* Check for error */
          logger(LOG_ERR, 100, "netconn_bind failed.");
      }
      /* Tell connection to go into listening mode. */
      err = netconn_listen(conn);
      if(err != ERR_OK){
          logger(LOG_ERR, 100, "Listen error");
      }

      logger(LOG_DEB, 1, "Before netconn_accept.");
      accept_err = netconn_accept(conn, &newconn);
      logger(LOG_DEB, 1, "AFTER netconn_accept.");
      if (accept_err != ERR_OK) {
          logger(LOG_ERR, 100, "netconn_accept failed.");
      }
      else{
        struct netbuf *buf;
        void *data;
        u16_t len;
        recv_err = netconn_recv(conn, &buf);
        if(recv_err == ERR_OK) {
          netbuf_data(buf, &data, &len);
          snprintf(logmsg, sizeof(logmsg), "data len: %d", len);
          logger(LOG_DEB, 1, logmsg);
          if(netconn_write(conn, data, len, NETCONN_COPY) != ERR_OK){
              logger(LOG_ERR, 1, "Error netconn_write.");
          }
        }
      }
    }
}

/*!
  \fn void modbus_cmd_handler_task(void *arg)
  \brief Modbus communication handler task. Listens on port 502 and creates a new task after an accept.
  \param arg not used.
  \return None
  */
void modbus_cmd_handler_task(void *arg)
{
    osDelay(1);
    struct netconn *conn, *newconn;
    err_t err;
    err_t accept_err;
    LWIP_UNUSED_ARG(arg);
    /* Create a new connection identifier. */
    conn = netconn_new(NETCONN_TCP);
    logger(LOG_DEB, 1, "Creating modbus_cmd_handler_task.");
    if (conn!=NULL)
    {
        /* Bind connection to modbus port 502. */
        err = netconn_bind(conn, NULL, 502);
        if(err != ERR_OK){
            /* Check for error */
            logger(LOG_ERR, 100, "netconn_bind failed.");
        }
        /* Tell connection to go into listening mode. */
        err = netconn_listen(conn);
        if(err != ERR_OK){
            logger(LOG_ERR, 100, "Listen error");
        }

        while(1){
            //netconn_set_recvtimeout(conn, 200);
            logger(LOG_DEB, 1, "Before netconn_accept.");
            accept_err = netconn_accept(conn, &newconn);
            logger(LOG_DEB, 1, "AFTER netconn_accept.");
            if (accept_err != ERR_OK) {
                logger(LOG_ERR, 100, "netconn_accept failed.");
                osDelay(10);
            }
            else{        /* Grab new connection. */
                xTaskCreate((void*) modbus_serve, "servetask", 4096, (void *)newconn, 3, NULL);
                // modbus_serve(newconn);
                logger(LOG_DEB, 1, "After xTaskCreate(modbus_serve).");
                osDelay(1);
            }
            osDelay(1);
            //modbus_serve(newconn);
        }
        netconn_close(conn);
        netconn_delete(conn);
    }
}


/*-----------------------------------------------------------------------------------*/
// parses the Modbus handler for later payload analysis.
/*!
  \fn int parse_modbus_header(void* data, u16_t len, u16_t* transactionID, u16_t* protocolID, u16_t* length, char* unitID, char* functioncode)
  \brief Parses the Modbus handler for later payload analysis.
  \param data Input data buffer of recieved message.
  \param len Length of received data buffer.
  \param transactionID Received transactionID.
  \param protocolID Received protocolID.
  \param length Received length.
  \param unitID Received unitID.
  \param functioncode Received functioncode.
  \return NULL if success, else ERRORCODE.
  */
int parse_modbus_header(void* data, u16_t len, u16_t* transactionID, u16_t* protocolID, u16_t* length, char* unitID, char* functioncode){
    char* cdata = data;
    *transactionID = cdata[1] | (cdata[0] << 8);
    *protocolID = cdata[3] | (cdata[2] << 8);
    *length = cdata[5] | (cdata[4] << 8);
    *unitID = cdata[6];
    *functioncode = cdata[7];
    logger(LOG_DEB, 1, "parse_modbus_header");


    osDelay(1);

    if( len == (*length + 6)){ //modbusheader - UID + Payload length. TODO length gets set here!!
        logger(LOG_DEB, 1, "Message length OK.");
        snprintf(logmsg, sizeof(logmsg), "data len: %d", len);
        logger(LOG_DEB, 1, logmsg);
        if(len > 32){// #ifdef CRYPTO_ENABLED
        uint8_t recv_hmac[32];
          size_t hmac_offset = len - 32; //12
          for(size_t t=0; t< 32; t++){
            recv_hmac[t] = cdata[hmac_offset + t ];
          }

          uint8_t hmacResult[32];

          mbedtls_md_context_t cryptoctx;
          mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
          const size_t keyLength = 32;
          mbedtls_md_init(&cryptoctx);
          mbedtls_md_setup(&cryptoctx, mbedtls_md_info_from_type(md_type), 1);
          mbedtls_md_hmac_starts(&cryptoctx, key, keyLength);
          mbedtls_md_hmac_update(&cryptoctx, (const unsigned char *) data,hmac_offset);
          mbedtls_md_hmac_finish(&cryptoctx, hmacResult);
          mbedtls_md_free(&cryptoctx);
          // for (size_t i = 0; i < 32; i++)
          //     {
          //       printf("[%.2X]", hmacResult[i]);
          //     }
          // printf("\n");
	  if( memcmp(recv_hmac, hmacResult, 32) == 0){
            logger(LOG_INF, 1, "HMAC OK");
          }
          else{
            logger(LOG_DEB, 1, "HMAC not OK.");
            return -1;
          }
        }// #endif
        return 0;
    }



    return -1;
}

/*!
  \fn int function_code_handler(void* data, char* functioncode, char* replyMessage, char* senddatalen)
  \brief Funtcion to parse Payload of modbus message according to its function code
  \param data Input data buffer of recieved message.
  \param functioncode Received functioncode.
  \param replyMessage Reply message buffer.
  \param senddatalen Length of the replyMessage.
  \return NULL if success, else ERRORCODE.
  */
int function_code_handler(void* data, char* functioncode, char* replyMessage, char* senddatalen,  char* unitID){
    logger(LOG_DEB, 1, "function_code_handler");
    char* cdata = data;
    // printf("[FC: %d]\n", *functioncode);
    // Read multiple registers
    if (*functioncode == 1) {
        //xQueueSend(loggingQueue, "Recieved read coils FC.", ( TickType_t ) 100000);
        //u16_t starting_address = cdata[9] | (cdata[8] << 8);
        u16_t quantity_of_coils = cdata[11] | (cdata[10] << 8);
        //sanity check if quantity_of_coils between 1 and 0x7d0 (2000)
        char byte_count = quantity_of_coils / 8; //full number of bytes
        if(quantity_of_coils % 8){ // fi remainder not null, add 1 byte
            byte_count++;
        }

        u16_t payloadLen = 1 + 2 + byte_count;
        char messageLen = 6 + payloadLen;

        u8_t bytecnt = 0;
        replyMessage[bytecnt++] = cdata[0];
        replyMessage[bytecnt++] = cdata[1];
        replyMessage[bytecnt++] = 0;
        replyMessage[bytecnt++] = 0;
        char* cpayloadlen = (char *) &payloadLen;
        replyMessage[bytecnt++] = cpayloadlen[1];
        replyMessage[bytecnt++] = cpayloadlen[0];
        replyMessage[bytecnt++] = *unitID; //cdata[6]; //unitID

        replyMessage[bytecnt++] = *functioncode;
        replyMessage[bytecnt++] = byte_count;
        u8_t tmp_coil = 0;
        for(int i=0; i<(int)(quantity_of_coils); i++){
            if(discrete_output_buf & (1 << i)){
                tmp_coil |= 1UL << i;
            }
        }
        //char loggingm[20];
        //sprintf(loggingm, "Output: %i", (int)tmp_coil);
        //xQueueSend(loggingQueue, loggingm, ( TickType_t ) 100000);
        replyMessage[bytecnt++] = tmp_coil;

        *senddatalen = messageLen;
        return 0;
    }
    //Read descrete inputs
    else if (*functioncode == 2) {
        logger(LOG_DEB, 1, "Received read discrete input.");
        //u16_t starting_address = cdata[9] | (cdata[8] << 8);
        u16_t quantity_of_coils = cdata[11] | (cdata[10] << 8);
        //sanity check if quantity_of_coils between 1 and 0x7d0 (2000)
        char byte_count = quantity_of_coils / 8; //full number of bytes
        if(quantity_of_coils % 8){ // if remainder not null, add 1 byte
            byte_count++;
        }

        u16_t payloadLen = 1 + 2 + byte_count;

        char messageLen = 6 + payloadLen;
        if(CRYPTO_ENABLED == 1){
	    payloadLen = payloadLen+32; // SIZE + HMAC SIZE
	}
	else{
	    payloadLen = payloadLen; // SIZE + HMAC SIZE
	}
	u8_t bytecnt = 0;
        replyMessage[bytecnt++] = cdata[0];
        replyMessage[bytecnt++] = cdata[1];
        replyMessage[bytecnt++] = 0;
        replyMessage[bytecnt++] = 0;
        char* cpayloadlen = (char *) &payloadLen;
        replyMessage[bytecnt++] = cpayloadlen[1];
        replyMessage[bytecnt++] = cpayloadlen[0];
        replyMessage[bytecnt++] = *unitID; // cdata[6]; //unitID

        replyMessage[bytecnt++] = *functioncode;
        replyMessage[bytecnt++] = byte_count;
        replyMessage[bytecnt++] = discrete_input_buf;

        // for(int i=0; i<(int)(byte_count); i++){ //TODO FIXTHIS: just works for the first 8 inputs.
        //     replyMessage[bytecnt++] = discrete_input_buf;
        // }

        *senddatalen = messageLen;
        return 0;
    }

    else if (*functioncode == 3) {
        logger(LOG_DEB, 1, "Received read multiple registers FC.");
        return 0;
    }

    else if (*functioncode == 4) {
        logger(LOG_DEB, 1, "Received Read Input Registers FC TBD.");
        return 0;
    }

    else if (*functioncode == 5) {
        logger(LOG_DEB, 1, "Received Write Single Coil FC.");
        u16_t output_address = cdata[9] | (cdata[8] << 8);
        u16_t output_value = cdata[11] | (cdata[10] << 8);

        if(output_address < 8){ //TODO make define for output buf size
            if(output_value == 0x0000){
                discrete_output_buf &= ~(1UL << output_address);
            }
            else if(output_value == 0xFF00){
                discrete_output_buf |= 1UL << output_address;
            }
            else{
                logger(LOG_ERR, 1, "output_value is INVALID!");
            }
            u16_t payloadLen = 1 + 1 + 2 + 2; //unitID + FC + output_address + output_value
            char messageLen = 6 + payloadLen;
            u8_t bytecnt = 0;
            replyMessage[bytecnt++] = cdata[0];
            replyMessage[bytecnt++] = cdata[1];
            replyMessage[bytecnt++] = 0;
            replyMessage[bytecnt++] = 0;
            char* cpayloadlen = (char *) &payloadLen;
            replyMessage[bytecnt++] = cpayloadlen[1];
            replyMessage[bytecnt++] = cpayloadlen[0];
            replyMessage[bytecnt++] = cdata[6]; //unitID
            replyMessage[bytecnt++] = *functioncode;
            replyMessage[bytecnt++] = cdata[8]; //output_address
            replyMessage[bytecnt++] = cdata[9];
            replyMessage[bytecnt++] = cdata[10]; // output_value
            replyMessage[bytecnt++] = cdata[11];
            *senddatalen = messageLen;
        }
        return 0;
    }

    else if(*functioncode == 15) {
        logger(LOG_DEB, 1, "Received write Multiple Coils FC.");
        //u16_t starting_address = cdata[9] | (cdata[8] << 8);
        u16_t quantity_of_coils = cdata[11] | (cdata[10] << 8);
        //sanity check if quantity_of_coils between 1 and 0x7d0 (2000)
        char byte_count = cdata[12]; //full number of bytes
        char output_value = cdata[13];
        // memcpy(output_value, (void*)&cdata[13], sizeof(output_value));
        // printf("output_value: %d\n", (int)output_value);
        if (byte_count == 1 && quantity_of_coils <= 8) {
            //TODO fix this for more than one byte of coils.
            for (int k = 0; k < quantity_of_coils; k++) {
                if(output_value & (1UL << k))
                {
                    discrete_output_buf |= 1UL << k;
                }
                else{
                    discrete_output_buf &= ~(1UL << k);
                }
            }
            u16_t payloadLen = 1 + 1 + 2 + 2; //unitID + FC + output_address + output_value
            char messageLen = 6 + payloadLen;
            u8_t bytecnt = 0;
            replyMessage[bytecnt++] = cdata[0];
            replyMessage[bytecnt++] = cdata[1];
            replyMessage[bytecnt++] = 0;
            replyMessage[bytecnt++] = 0;
            char* cpayloadlen = (char *) &payloadLen;
            replyMessage[bytecnt++] = cpayloadlen[1];
            replyMessage[bytecnt++] = cpayloadlen[0];
            replyMessage[bytecnt++] = cdata[6]; //unitID
            replyMessage[bytecnt++] = *functioncode;
            replyMessage[bytecnt++] = cdata[8]; //output_address
            replyMessage[bytecnt++] = cdata[9];
            replyMessage[bytecnt++] = cdata[10]; // output_value
            replyMessage[bytecnt++] = cdata[11];
            *senddatalen = messageLen;
        }
        return 0;
    }

    else if(*functioncode == 16) {
        logger(LOG_DEB, 1, "Received write multiple registers FC");
        return 0;
    }
    else if(*functioncode == 43) {
        logger(LOG_DEB, 1, "Received Read Device Identification");
        char function = 43;
        char MEItype = cdata[8];
        char ReadDevID = cdata[9];
        //char ObjectID = cdata[10];
        if ((MEItype == 0x0E) && (ReadDevID == 1)){
            logger(LOG_DEB, 1, "Read Device Identification");
            // return Basic description: VendorName ProductCode MajorMinorRevision
            char VendorNameID = 0;
            const char* VendorName = VENDORNAME;
            char VendorNameLen = strlen(VendorName);
            // printf("VendorNameLen: %d\r\n", VendorNameLen );

            char ProductCodeID = 1;
            const char* ProductCode = PRODUCTCODE;
            char ProductCodeLen = strlen(ProductCode);

            char MajorMinorRevisionID = 2;
            const char* MajorMinorRevision = MAJORMINORREVISION;
            char MajorMinorRevisionLen = strlen(MajorMinorRevision);

            char ConformityLevel = 1;
            char MoreFollows = 0;
            char NextObjectID = 0;
            char NumberOfObjects = 3;

            u16_t payloadLen = 1 + 13 + VendorNameLen + ProductCodeLen + MajorMinorRevisionLen;
            char messageLen = 6 + payloadLen;

            u8_t bytecnt = 0;
            replyMessage[bytecnt++] = cdata[0];
            replyMessage[bytecnt++] = cdata[1];
            replyMessage[bytecnt++] = 0;
            replyMessage[bytecnt++] = 0;
            char* cpayloadlen = (char *) &payloadLen;
            replyMessage[bytecnt++] = cpayloadlen[0];
            replyMessage[bytecnt++] = cpayloadlen[1];
            replyMessage[bytecnt++] = cdata[6];

            replyMessage[bytecnt++] = function;
            replyMessage[bytecnt++] = MEItype;
            replyMessage[bytecnt++] = ReadDevID;
            replyMessage[bytecnt++] = ConformityLevel;
            replyMessage[bytecnt++] = MoreFollows;
            replyMessage[bytecnt++] = NextObjectID;
            replyMessage[bytecnt++] = NumberOfObjects;
            replyMessage[bytecnt++] = VendorNameID;
            replyMessage[bytecnt++] = VendorNameLen;
            strncpy(&replyMessage[bytecnt], VendorName, VendorNameLen+1);
            bytecnt += VendorNameLen;
            replyMessage[bytecnt++] = ProductCodeID;
            replyMessage[bytecnt++] = ProductCodeLen;
            strncpy(&replyMessage[bytecnt], ProductCode, ProductCodeLen+1);
            bytecnt+=ProductCodeLen;
            replyMessage[bytecnt++] = MajorMinorRevisionID;
            replyMessage[bytecnt++] = MajorMinorRevisionLen;
            strncpy(&replyMessage[bytecnt], MajorMinorRevision, MajorMinorRevisionLen+1);
            bytecnt += MajorMinorRevisionLen;
            *senddatalen = messageLen;
            return true;
        }

    }
    else{
        logger(LOG_ERR, 100, "Received Invalid FC!");
        return false;
    }
    return false;
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
