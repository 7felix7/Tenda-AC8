#ifndef _CGI_TC_H_
#define _CGI_TC_H_

#include "arp_clients.h"

#define UP_RATE_LIMIT       301.0f       /*��λ��Mbps = 128KB/s*/
#define DOWN_RATE_LIMIT     301.0f       /*��λ��Mbps = 128KB/s*/

/*�ṩ��appd,����app�����ϴ����ص��������*/
#define UP_RATE_LIMIT_MAX       300.0f       /*��λ��Mbps = 128KB/s*/
#define DOWN_RATE_LIMIT_MAX     300.0f       /*��λ��Mbps = 128KB/s*/

#define TC_CLIENT_NUMBER 	255
#define STREAM_CLIENT_NUMBER TC_CLIENT_NUMBER
#define TIMES 1
#define TC_RULE_NUMBER_MAX 20
#define ETHER_ADDR_LEN 	6
#define AP_IFNAME    "wlan0"

#define WL0_MACLIST_MAX       (17*10 + 10)  /*����wl0_maclist�е�mac��ַ������󳤶�*/ 
#define RUB_NET_BLACKLIST_MAX 10     /* ����һ�������������ֵ������10�� */
#define TR_RULE_NUMBER_MAX    20     /*����һ�������б�����ֵ*/    
#define _FW_TRUST_MAC(i)           racat("trust_list", i)

#endif
