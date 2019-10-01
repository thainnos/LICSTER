// http://engsta.com/lwip-stack-bandwithperformance-testing/
// No license

#include "lwip/opt.h"
#include "lwip/tcp.h"

/**                                                                     
  * @brief Iperf receive function    
  * @param  arg:                 
  * @param  pcb:
  * @param  p:     
  * @param  err:     
  * @retval err_t: Return the success state of the function                                                         
  */ 
static err_t iperf_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    LWIP_UNUSED_ARG(arg);

    if (err == ERR_OK && p != NULL) {
        tcp_recved(pcb, p->tot_len);
        pbuf_free(p);
    } 
    else if (err != ERR_OK && p != NULL) {
        pbuf_free(p);
    }
    else  if (err == ERR_OK && p == NULL) {
        tcp_arg(pcb, NULL);
        tcp_sent(pcb, NULL);
        tcp_recv(pcb, NULL);
        tcp_close(pcb);
    }
    return ERR_OK;
}

/**                                                                     
  * @brief Iperf accept function    
  * @param  arg:                 
  * @param  pcb:
  * @param  p:     
  * @param  err:     
  * @retval err_t: Return the success state of the function                                                         
  */ 
static err_t iperf_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);

    tcp_arg(pcb, NULL);
    tcp_sent(pcb, NULL);
    tcp_recv(pcb, iperf_recv);
    return ERR_OK;
}


/**                                                                     
  * @brief Minimalistic implementation of IPERF
  *        Init function
  *        Testing with iperf -c <IP> -t <TIME>
  * @retval None                                                         
  */ 
void iperf_init(void)
{
    struct tcp_pcb *pcb;

    pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, 5001); // bind to iperf port
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, iperf_accept);
}
