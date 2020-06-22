#include "cmsis_os.h"
#include <stdbool.h>
#include <string.h>

#include "heap_mem.h"

#include "logging.h"
#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/x509.h"
#include "mbedtls/certs.h"
#include "mbedtls/error.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_time            time
#define mbedtls_time_t          time_t
#define mbedtls_fprintf         fprintf
#define mbedtls_printf          printf
#define mbedtls_exit            exit
#define MBEDTLS_EXIT_SUCCESS    EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE    EXIT_FAILURE
#endif /* MBEDTLS_PLATFORM_C */

#if !defined(MBEDTLS_BIGNUM_C) || !defined(MBEDTLS_ENTROPY_C) ||  \
    !defined(MBEDTLS_SSL_TLS_C) || !defined(MBEDTLS_SSL_CLI_C) || \
    !defined(MBEDTLS_NET_C) || !defined(MBEDTLS_RSA_C) ||         \
    !defined(MBEDTLS_CERTS_C) || !defined(MBEDTLS_PEM_PARSE_C) || \
    !defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_X509_CRT_PARSE_C)
#error "MBEDTLS_BIGNUM_C and/or MBEDTLS_ENTROPY_C and/or MBEDTLS_SSL_TLS_C and/or MBEDTLS_SSL_CLI_C and/or MBEDTLS_NET_C and/or MBEDTLS_RSA_C and/or MBEDTLS_CTR_DRBG_C and/or MBEDTLS_X509_CRT_PARSE_C not defined.\n"
#endif

#if defined(MBEDTLS_SSL_CACHE_C)
#include "mbedtls/ssl_cache.h"
#endif

#if defined(MBEDTLS_DEBUG_C)
#include "mbedtls/debug.h"
/*
 *	0 No debug
 *	1 Error
 *	2 State change
 *	3 Informational
 *	4 Verbose
 */
#define MBEDTLS_DEBUG_LEVEL 3
#endif

/*
 * other defines
 */
#define VENDORNAME "HSA_innos"
#define PRODUCTCODE "sensorSTM"
#define MAJORMINORREVISION "Version 0.2"
#define RECVBUFFERSIZE 1024
#define SENDBUFFERSIZE 128
#define RECVTIMEOUT 2500

#define PRV_KEY \
	"-----BEGIN RSA PRIVATE KEY-----\r\n" \
	"MIIEogIBAAKCAQEA0smD0jIMTEk4pYiyfehIgZzyKQPs2uF8IUL1Z8rmAU64GcmQ\r\n" \
	"4pK1dpOap9LZ63jByv3VWS7Mee75Yi8Hc1cInIdGS+pvOGksDCJuS8im+gBwzeFP\r\n" \
	"BtesuUorH4c+lEWlutvojhxcJFxcqQRtxlWzllbKW0dEDsjWD1+6BB/vB0SCYcSx\r\n" \
	"jdMONlMXLuh0aPTJtc7r+ShPlyuHjE4l/Y0CVkBhR0Gil3S+MVa8JpX1yLF9JBGP\r\n" \
	"5XdKAiA81DfKShhXSksLjE0+aGdz3FPOpsp1jpuT/PSb7D7zjxhMtGzFFFdcEZ4E\r\n" \
	"U71UP9RwB9KA1D1h8yGZmD0rWb6mS8NQtWkUlwIDAQABAoIBAAPc34aNlj4bpWcb\r\n" \
	"LzIxfIqrhqCLyt5j/XNal8a0KCzj9Wm7kOUmDR9EKzRiH84540tGBY/h1KtzXqyW\r\n" \
	"8gsCEE1WCUrA3BLrknwScXUAE1dHhMmxFFKqxyaKHmowf0wWTqgy+XsJOAMOkkAi\r\n" \
	"raRR072wj3rGvo+KzKlxdqY2s5mtdjNLvY2ycEN7C3CtB6y2c1meoIlb2SThepUq\r\n" \
	"T6R9fdx3HObO08QYEdscQsAuQGkQQu852p4ErQZe0jC2VKVPQIcsTSwuzvA01rN8\r\n" \
	"oqH0xiv8wCUPAzpYFS2gzdH7YHkVNHXEdhPRoxjVk98oUzZ1zLZnqOE2glU7YAEf\r\n" \
	"ciKeXQECgYEA9XzI9Y3uknGTvUZ4YFL2qkDfWPQFbi7yHaeYeUnhOCSaECwq45nj\r\n" \
	"2tvxl8nCT7lMwjEbyaTOfsYejMKbylb0+Q9uCUCCe435y45qgoW3OT6ma7kNHAFG\r\n" \
	"3ukhG5X3NT/s8+bxE97pITHuQWsYCHqOVIqK7XtPo1Vy3gs9f4qJM4ECgYEA29BR\r\n" \
	"4aKIMgitrRkmdwuFdp9b0vhDz/xyp1usVjP2UcUosJ3PpJNzDpDXNb9N38IWgKdV\r\n" \
	"ulfUsdvn/VSkcyFqK58VNVmmtkYemKyxo2Wf5B3h8fOC1EBg14jEXYBBdHKlVddZ\r\n" \
	"M8D8gPgDcnPCtEr5MZn2GvsqlzS+n27lCus/dBcCgYBWWvUeXDVGUqnT9Uw3Jjgb\r\n" \
	"IALxebXHUsBc6i3azMBrTYTcl4zmz6W1wZU0/gZaoSZ9UWUjA9SYB/2LKpI/FWPL\r\n" \
	"kT/2BTlhWUo77vdMvtj7TCyizE/uQAy8MWoyGAkENF6YVe8MPGfqA3R6kr+7i9Ex\r\n" \
	"sCK685vKKy7jKTUS/prRAQKBgG/QSyaEn9UbtwTbS0a5FJ3fuDzbztropoXdSw/a\r\n" \
	"hMzZhrY4wFwOKTPal66eboWI3Bt7g08zyFzk6OBjgveRoskntgkaXoRPrNaq2f2s\r\n" \
	"/yVx0fJalU1Upi5dQs3xJNqQSuwcjCKYY5lTD6XTntJJZPNHRvo4k1SxHtpI9beV\r\n" \
	"LQTnAoGADVbwv3G+iFq1Cqf8TEqgs46xZSb3jUM8WLkTzNF8Q6c3j+23G7cK0oc+\r\n" \
	"SNpmLAkY3C0eoOl+z6FtbNkbJZ/epfMK/iXFWORAV0mcyYdnB4L1+DPvL4ncut4j\r\n" \
	"tk5AabeHG6zuqD67tWSh0oV5EfXyjjTtBIrWlFQuNtPBaU62/68=\r\n" \
	"-----END RSA PRIVATE KEY-----\r\n"

#define CRT \
	"-----BEGIN CERTIFICATE-----\r\n" \
	"MIIDCDCCAfCgAwIBAgIBATANBgkqhkiG9w0BAQsFADAgMREwDwYDVQQDDAh0ZXN0\r\n" \
	"LmNvbTELMAkGA1UEBhMCREUwHhcNMTMwMTAxMDAwMDAwWhcNMjUwMTAxMDAwMDAw\r\n" \
	"WjAgMREwDwYDVQQDDAh0ZXN0LmNvbTELMAkGA1UEBhMCREUwggEiMA0GCSqGSIb3\r\n" \
	"DQEBAQUAA4IBDwAwggEKAoIBAQDSyYPSMgxMSTiliLJ96EiBnPIpA+za4XwhQvVn\r\n" \
	"yuYBTrgZyZDikrV2k5qn0tnreMHK/dVZLsx57vliLwdzVwich0ZL6m84aSwMIm5L\r\n" \
	"yKb6AHDN4U8G16y5Sisfhz6URaW62+iOHFwkXFypBG3GVbOWVspbR0QOyNYPX7oE\r\n" \
	"H+8HRIJhxLGN0w42Uxcu6HRo9Mm1zuv5KE+XK4eMTiX9jQJWQGFHQaKXdL4xVrwm\r\n" \
	"lfXIsX0kEY/ld0oCIDzUN8pKGFdKSwuMTT5oZ3PcU86mynWOm5P89JvsPvOPGEy0\r\n" \
	"bMUUV1wRngRTvVQ/1HAH0oDUPWHzIZmYPStZvqZLw1C1aRSXAgMBAAGjTTBLMAkG\r\n" \
	"A1UdEwQCMAAwHQYDVR0OBBYEFAv8obB0888/jY1FDdrWXe7w9aOWMB8GA1UdIwQY\r\n" \
	"MBaAFAv8obB0888/jY1FDdrWXe7w9aOWMA0GCSqGSIb3DQEBCwUAA4IBAQAg3Xso\r\n" \
	"NPHZbJcE0EKIFBKfxnlTUYTKK2zZlmCGtHPE4yRVyate8sZ7BGOpULnqUM32TVl4\r\n" \
	"3hDFsbPaDbeY6Gp5RnLAvPz0Fhn9yF6Jn4e86lwZIeEjO9IGZN+W0MZdQFDtug0t\r\n" \
	"yHcBOOdcQnyBXIzuAGX+1CZ2Mmp8AZGWrRXF9hHPE/L6TwJW9UXt7rj6QkLiV8NE\r\n" \
	"kgJLOD93c7pW5+PJROed6HH3yFKCgpWK9UvUupjo5fzNGMef6qO2BmiNWhtH9KoM\r\n" \
	"fiwpHqu5s8Ei3uSjpScvFWz/t+pA9RZVJNh9+oWBq9ehro6+sq+jRcMS61MIy9kz\r\n" \
	"2rxRVTgYso25/j17\r\n" \
	"-----END CERTIFICATE-----\r\n"


// extern variables
extern unsigned int deviceID;
extern char discrete_input_buf;
extern char discrete_output_buf;

// function declarations
int parse_modbus_header(void* data, uint16_t len, uint16_t* transactionID, uint16_t* protocolID, uint16_t* length, char* unitID, char* functioncode);
void modbus_serve_connection(mbedtls_net_context *client_fd);
int function_code_handler(void* data, char* functioncode, char* replyMessage, char* senddatalen,  char* unitID);

#ifdef MBEDTLS_DEBUG_C
static void my_debug(void *ctx, int level, const char *file, int line, const char *str);
#endif
#ifdef USE_TLS
static void modbus_serve(mbedtls_ssl_context *ssl);
#else
static void modbus_serve(mbedtls_net_context *ctx);
#endif


// function definitions

#ifdef MBEDTLS_DEBUG_C
static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void) level);

    fprintf( (FILE *) ctx, "\r%s:%04d: %s", file, line, str );
    fflush(  (FILE *) ctx  );
}
#endif

/*!
  \fn void modbus_cmd_handler_task(void *arg)
  \brief Modbus communication handler task. Listens on port 502 and creates a new task after an accept.
  \param arg not used.
  \return None
  */
void modbus_cmd_handler_task()
{
	int ret;
	mbedtls_net_context listen_fd;
	mbedtls_net_context *client_fd;

	mbedtls_net_init( &listen_fd );

	if( ( ret = mbedtls_net_bind( &listen_fd, NULL, "502", MBEDTLS_NET_PROTO_TCP ) ) == 0 )
	{
		while(1)
		{
			client_fd = rmalloc(sizeof(mbedtls_net_context));
			mbedtls_net_init( client_fd );
			if( ( ret = mbedtls_net_accept( &listen_fd, client_fd, NULL, 0, NULL ) ) == 0 )
			{
				// run the mb serve task in realtime priority
				xTaskCreate((void*) modbus_serve_connection, "servetask", 4096, (void *)client_fd, osPriorityRealtime, NULL);
				logger(LOG_DEB, 1, "After xTaskCreate(modbus_serve_connection).");
				osDelay(1);
			}
			else
			{
				logger(LOG_ERR, 100, "mbedtls_net_accept returned failed!");
			}
		}
	}

	mbedtls_net_free( &listen_fd );
}

/*!
  \fn void modbus_serve_connection(mbedtls_net_context *client_fd)
  \brief Provides the proper connection type to the modbus_serve function.
  \param client_fd client filedescriptor in form of an mbedtls_net_context pointer.
  \return None
  */
void modbus_serve_connection(mbedtls_net_context *client_fd)
{
	int ret = 0;
#ifdef USE_TLS
	const char *pers = "ssl_server";
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt srvcert;
	mbedtls_pk_context pkey;
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_context cache;
#endif


	mbedtls_ssl_init( &ssl );
	mbedtls_ssl_config_init( &conf );
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_init( &cache );
#endif
	mbedtls_x509_crt_init( &srvcert );
	mbedtls_pk_init( &pkey );
	mbedtls_entropy_init( &entropy );
	mbedtls_ctr_drbg_init( &ctr_drbg );

	const unsigned char *prv_key;
	const unsigned char *crt;

	switch(deviceID)
	{
	case 1:
		prv_key = IO1_KEY;
		crt 	= IO1_CRT;
		break;
	case 2:
		prv_key = IO2_KEY;
		crt 	= IO2_CRT;
		break;
	default:
		logger(LOG_WAR, 1, "USING DEFAULT CERT/KEY, THIS SHOULD NOT HAPPEN!");
		prv_key = PRV_KEY;
		crt 	= CRT;
		break;
	}

	/*
	 * 1. Load the certificates and private RSA key
	 */
	ret =  mbedtls_pk_parse_key( &pkey, (const unsigned char *) prv_key,
			strlen(prv_key) + 1, NULL, 0 );

	if( ret != 0 )
	{
		logger(LOG_ERR, 1, "mbedtls_pk_parse_key failed");
		goto exit;
	}

	ret = mbedtls_x509_crt_parse( &srvcert, (const unsigned char *) crt, strlen(crt) + 1 );
	if( ret != 0 )
		goto exit;

	/*
	 * 3. Seed the RNG
	 */
	if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
							   (const unsigned char *) pers,
							   strlen( pers ) ) ) != 0 )
		goto exit;

	/*
	 * 4. Setup stuff
	 */
	if( ( ret = mbedtls_ssl_config_defaults( &conf,
					MBEDTLS_SSL_IS_SERVER,
					MBEDTLS_SSL_TRANSPORT_STREAM,
					MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
		goto exit;

	mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
#ifdef MBEDTLS_DEBUG_C
	mbedtls_debug_set_threshold(MBEDTLS_DEBUG_LEVEL);
	mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );
#endif

#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_conf_session_cache( &conf, &cache,
								   mbedtls_ssl_cache_get,
								   mbedtls_ssl_cache_set );
#endif
	mbedtls_ssl_conf_ca_chain( &conf, srvcert.next, NULL );
	/* TODO: implement To Way Authentication, based on the parent cert of own cert
	mbedtls_ssl_conf_authmode( &conf, MBEDTLS_SSL_VERIFY_REQUIRED );
	mbedtls_ssl_conf_cert_req_ca_list( &conf, MBEDTLS_SSL_CERT_REQ_CA_LIST_ENABLED );
	 */
	if( ( ret = mbedtls_ssl_conf_own_cert( &conf, &srvcert, &pkey ) ) != 0 )
		goto exit;

	if( ( ret = mbedtls_ssl_setup( &ssl, &conf ) ) != 0 )
		goto exit;

	mbedtls_ssl_set_bio( &ssl, client_fd, mbedtls_net_send, mbedtls_net_recv, NULL );

	/*
	 * 5. Handshake
	 */
	mbedtls_printf( "\rPerforming the SSL/TLS handshake..." );
	fflush( stdout );

    mbedtls_ssl_conf_read_timeout(&conf, RECVTIMEOUT);
    //TODO: why is there a while?
	while( ( ret = mbedtls_ssl_handshake( &ssl ) ) != 0 )
	{
			if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
			{
				logger(LOG_ERR, 1, "Error in Handshake!");
				goto exit;
			}

	}
	/*
	 * 6. Serve modbus client
	 */

	modbus_serve(&ssl);

#else
	/*
	 * 6. Serve modbus client
	 */

	modbus_serve(client_fd);
#endif

exit:

#ifdef MBEDTLS_ERROR_C
	if( ret != 0 )
	{
		char msg[200];
		char error_buf[100];
		mbedtls_strerror( ret, error_buf, 100 );
		snprintf(msg, 200, "Last error was: %d - %s\n", ret, error_buf);
		logger(LOG_ERR, 100, msg);
	}
#endif

	mbedtls_net_free( client_fd );
#ifdef USE_TLS
	mbedtls_x509_crt_free( &srvcert );
	mbedtls_pk_free( &pkey );
	mbedtls_ssl_free( &ssl );
	mbedtls_ssl_config_free( &conf );
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_free( &cache );
#endif
	mbedtls_ctr_drbg_free( &ctr_drbg );
	mbedtls_entropy_free( &entropy );
#endif

    vTaskDelete( NULL );
    osDelay(5);
}
#ifdef USE_TLS
static void modbus_serve(mbedtls_ssl_context *ssl)
#else
static void modbus_serve(struct mbedtls_net_context *ctx)
#endif
{
    int recv_err;
    char buf[RECVBUFFERSIZE] = {0};
    uint16_t len;
    uint16_t transactionID;
    uint16_t protocolID;
    uint16_t length;
    char unitID;
    char functioncode = 0;
    int brcvMsgOK = 0;
    char sendbuffer[SENDBUFFERSIZE];
    osDelay(1);
    logger(LOG_INF, 100, "Modbus client connected");
#ifdef USE_TLS
    recv_err = mbedtls_ssl_read(ssl, (unsigned char*) buf, RECVBUFFERSIZE);
#else
    recv_err = mbedtls_net_recv_timeout(ctx, (unsigned char*) buf, RECVBUFFERSIZE, RECVTIMEOUT);
#endif
    while ( recv_err > 0 )
    {
    	len = recv_err;

		char senddatalen = 0;
		if (len < 8) {
			logger(LOG_INF, 1, "Received incomplete message.");
		}
		else{
			brcvMsgOK = parse_modbus_header(buf, len, &transactionID, &protocolID, &length, &unitID, &functioncode);
		}
		if (brcvMsgOK == 0)
		{
			for (int i = 0; i<SENDBUFFERSIZE; i++) {
				sendbuffer[i] = 0; /*clear send buffer*/
			}
			logger(LOG_DEB, 1, "Creating response");
			brcvMsgOK = function_code_handler(buf, &functioncode, sendbuffer, &senddatalen, &unitID);
			logger(LOG_DEB, 1, "Created response");

			if (brcvMsgOK != 0){
				logger(LOG_ERR, 1, "Error in function_code_handler.");
			}
#ifdef USE_TLS
			if(mbedtls_ssl_write(ssl, (unsigned char*) sendbuffer, senddatalen) != senddatalen)
			{
				logger(LOG_ERR, 1, "Error mbedtls_ssl_write.");
			}
#else
			if(mbedtls_net_send(ctx, (unsigned char*) sendbuffer, senddatalen) != senddatalen)
			{
				logger(LOG_ERR, 1, "Error mbedtls_net_send.");
			}
#endif
			logger(LOG_DEB, 1, "Sent response");
		}
		osDelay(1);
#ifdef USE_TLS
	    recv_err = mbedtls_ssl_read(ssl, (unsigned char*) buf, RECVBUFFERSIZE);
#else
	    recv_err = mbedtls_net_recv_timeout(ctx, (unsigned char*) buf, RECVBUFFERSIZE, RECVTIMEOUT);
#endif
        osDelay(1);
    }
    if (recv_err == 0)
    	logger(LOG_INF, 100, "Connection was closed by peer");
    else if(recv_err == MBEDTLS_ERR_SSL_TIMEOUT)
    	logger(LOG_INF, 100, "MBEDTLS_ERR_SSL_TIMEOUT");
    else if (recv_err == MBEDTLS_ERR_NET_RECV_FAILED)
    	logger(LOG_INF, 100, "MBEDTLS_ERR_NET_RECV_FAILED");
    else if (recv_err == MBEDTLS_ERR_SSL_WANT_READ)
    	logger(LOG_INF, 100, "MBEDTLS_ERR_SSL_WANT_READ");
    else if (recv_err == MBEDTLS_ERR_NET_INVALID_CONTEXT)
    	logger(LOG_INF, 100, "MBEDTLS_ERR_NET_INVALID_CONTEXT");
    else
    {
    	char msg[200];
    	snprintf(msg, 200, "Unknown error in connection: %i", recv_err);
    	logger(LOG_INF, 100, msg);
    }
    /* Close connection and discard connection identifier. */
    logger(LOG_INF, 100, "Modbus client disconnected");

    osDelay(5);
}


/*-----------------------------------------------------------------------------------*/
// parses the Modbus handler for later payload analysis.
/*!
  \fn int parse_modbus_header(void* data, uint16_t len, uint16_t* transactionID, uint16_t* protocolID, uint16_t* length, char* unitID, char* functioncode)
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
int parse_modbus_header(void* data, uint16_t len, uint16_t* transactionID, uint16_t* protocolID, uint16_t* length, char* unitID, char* functioncode){
	char logmsg[256];

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
        //uint16_t starting_address = cdata[9] | (cdata[8] << 8);
        uint16_t quantity_of_coils = cdata[11] | (cdata[10] << 8);
        //sanity check if quantity_of_coils between 1 and 0x7d0 (2000)
        char byte_count = quantity_of_coils / 8; //full number of bytes
        if(quantity_of_coils % 8){ // fi remainder not null, add 1 byte
            byte_count++;
        }

        uint16_t payloadLen = 1 + 2 + byte_count;
        char messageLen = 6 + payloadLen;

        uint8_t bytecnt = 0;
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
        uint8_t tmp_coil = 0;
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
        //uint16_t starting_address = cdata[9] | (cdata[8] << 8);
        uint16_t quantity_of_coils = cdata[11] | (cdata[10] << 8);
        //sanity check if quantity_of_coils between 1 and 0x7d0 (2000)
        char byte_count = quantity_of_coils / 8; //full number of bytes
        if(quantity_of_coils % 8){ // if remainder not null, add 1 byte
            byte_count++;
        }

        uint16_t payloadLen = 1 + 2 + byte_count;

        char messageLen = 6 + payloadLen;

	uint8_t bytecnt = 0;
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
        uint16_t output_address = cdata[9] | (cdata[8] << 8);
        uint16_t output_value = cdata[11] | (cdata[10] << 8);

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
            uint16_t payloadLen = 1 + 1 + 2 + 2; //unitID + FC + output_address + output_value
            char messageLen = 6 + payloadLen;
            uint8_t bytecnt = 0;
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
        //uint16_t starting_address = cdata[9] | (cdata[8] << 8);
        uint16_t quantity_of_coils = cdata[11] | (cdata[10] << 8);
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
            uint16_t payloadLen = 1 + 1 + 2 + 2; //unitID + FC + output_address + output_value
            char messageLen = 6 + payloadLen;
            uint8_t bytecnt = 0;
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

            uint16_t payloadLen = 1 + 13 + VendorNameLen + ProductCodeLen + MajorMinorRevisionLen;
            char messageLen = 6 + payloadLen;

            uint8_t bytecnt = 0;
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
