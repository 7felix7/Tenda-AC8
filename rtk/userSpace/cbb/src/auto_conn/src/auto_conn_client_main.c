#include <stdio.h>
#include <bcmnvram.h>
#include <iflib.h>
#include <ecos_oslib.h>
#include <net/if_var.h>

#include <sys/mbuf.h>
#include <net/route.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <net/if_dl.h>
#include <net/radix.h>
#include <net/route.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <sys/sockio.h>
#include <autoconf.h>
#include "wifi.h"
//#include  "../rc/rc.h"

#include "pi_common.h"


#include  "auto_conn_common.h"

#if defined(RTL819X)
#include <wl_utility_rltk.h>
#endif

#if defined(BCM535X)
extern void sys_restart(void);
extern int get_wl_bssid(struct ether_addr *wl_bssid);
extern void auto_conn_sys_restart();
#endif

extern void copy_config_24G(unsigned char *recv_data);
extern void copy_config_5G(unsigned char *recv_data);
extern int auto_conn_extend_status;
extern int AutoConnDebugLevel;
//extern int auto_conn_getting_ip;
extern void tpi_auto_conn_client_set_restarting_tag(PIU8 tag);

#include "cJSON.h"
static int auto_client_stop =0;
extern int device_name_24G; 
extern int device_name_5G; 


/*****************************************************************************
 �� �� ��  : cjson_get
 ��������  : ��������cjson����ؼ��ֵ�ֵ
 �������  : cJSON *root
             char *name
             char *defaultGetValue
 �������  : ��
 �� �� ֵ  : char

 �޸���ʷ      :
  1.��    ��   : 2016��11��26��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
char *cjson_get(cJSON *root, char *name, char *defaultGetValue)
{
	if (!name)
	{
		return NULL;
	}

	cJSON *obj = cJSON_GetObjectItem(root, name);

	if (!obj)
	{
		return defaultGetValue;
	}
	else
	{
		if (obj->valuestring)
		{
			AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "%s = %s\n", name, obj->valuestring);
			return obj->valuestring;
		}
		else
		{
			return "";
		}
	}
}

//�Զ�ͬ��������־������ҳ�����жϣ�Ĭ��0��ͬ���ɹ�����Ϊ1
int auto_sync_end = 0;
int get_wantype(char *wan_mode)
{
	if(NULL == wan_mode )
	{
		return WAN_NONE_MODE;
	}
	else if(strcmp(wan_mode, "pppoe") == 0)
	{
		return WAN_PPPOE_MODE;
	}
	else if(strcmp(wan_mode, "dhcp") == 0)
	{
		return WAN_DHCP_MODE;
	}
	else
		return WAN_NONE_MODE;
	
}
/*****************************************************************************
 �� �� ��  : configure_to_Internet
 ��������  : ����WAN����
 �������  : char *cjson
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��12��6��
    ��    ��   : kg
    �޸�����   : �����ɺ���

*****************************************************************************/
void configure_to_Internet(char *cjson)
{
	char msg_param[PI_BUFLEN_32] = {0};
	char *pppoe_name = NULL;
	char *pppoe_passwd = NULL;
	char *pppoe_service = NULL;
	char *proto = NULL;
	int wantype = 0;
	cJSON *root = cJSON_Parse(cjson);	
	proto = cjson_get(root, WAN0_PROTO, "dhcp");
	wantype = get_wantype(proto);

	if((!nvram_match(WAN0_PPPOE_USERNAME, "")) 
		|| (!nvram_match(WAN0_PPPOE_PASSWD, "")))
	{
		return ;
	}
	
	switch(wantype)
	{
		case WAN_PPPOE_MODE:
			pppoe_name = cjson_get(root, WAN0_PPPOE_USERNAME, "");
			pppoe_passwd = cjson_get(root, WAN0_PPPOE_PASSWD, "");
			pppoe_service = cjson_get(root, WAN0_PPPOE_SERVICE, "");
			if(strcmp("",pppoe_name) && strcmp("",pppoe_passwd))
			{
				nvram_set(WAN0_PPPOE_USERNAME, pppoe_name);
				nvram_set(WAN0_PPPOE_PASSWD, pppoe_passwd);
				nvram_set(WAN0_PPPOE_SERVICE, pppoe_service);
				nvram_set(WAN0_PROTO, proto);
			}
			break;
			   
		case WAN_DHCP_MODE:
			nvram_set(WAN0_PROTO, proto);
			break;
		
		default:
			break;
	}
	memset(msg_param,0,sizeof(msg_param));
	sprintf(msg_param, "op=%d", OP_RESTART);
	msg_send(MODULE_RC, RC_WAN_MODULE, msg_param);
	
}
/*****************************************************************************
 �� �� ��  : configure_5G_reinfo
 ��������  : �ϼ���5Gʱ������ʵ��5G��Ϣ
 �������  : char *cjson
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��12��6��
    ��    ��   : kg
    �޸�����   : �����ɺ���

*****************************************************************************/

void configure_5G_reinfo(char *cjson)
{
	cJSON *root = cJSON_Parse(cjson);
	char  *ssid = NULL, *psk = NULL, *security_mode = NULL;
	if (!root)
	{
		printf("get root faild!\n");
		return;
	}
	ssid = cjson_get(root, WLAN5G_SSID, "");
	psk = cjson_get(root, WLAN5G_PASSWD, "");
	security_mode = cjson_get(root, WLAN5G_AKM, "");

	if (strcmp(ssid, ""))
	{
		nvram_set(WLAN5G_SSID, ssid);
	
		if ((strlen(psk) >= 8) && strcmp(security_mode, ""))
		{
				nvram_set(WLAN5G_PASSWD, psk);
				nvram_set(WLAN5G_AKM, security_mode);
				nvram_set(WLAN5G_CRYPTO, "aes"); //ҳ�治֧��ѡ��Ĭ��aes
		}
		else
		{
				nvram_set(WLAN5G_AKM, "");
		}
	}
}
/*****************************************************************************
 �� �� ��  : configure_5G_fr24G
 ��������  : �ϼ�û��5Gʱ�����յ���2.4G����Ϣ������5G��
 �������  : char *cjson
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��12��6��
    ��    ��   : kg
    �޸�����   : �����ɺ���

*****************************************************************************/

void configure_5G_fr24G(char *cjson)
{
	char tmp[PI_BUFLEN_64] = {0};
	char prefix_ssid[PI_BUFLEN_32] = {0};
	char prefix[PI_BUFLEN_16] = {0};
	cJSON *root = cJSON_Parse(cjson);
	char  *ssid = NULL, *psk = NULL, *security_mode = NULL;
	if (!root)
	{
		printf("get root faild!\n");
	}
	if(strcmp("ap", cjson_get(root, WLAN24G_WORK_MODE, "")))
	{
		sprintf(prefix,"%s_" ,WL_24G_REPEATER);
	}
	else
	{
		sprintf(prefix,"%s_", WL_24G);
	}
		
	ssid = cjson_get(root, strcat_r(prefix, "ssid", tmp), "");
	psk = cjson_get(root, strcat_r(prefix, "wpa_psk", tmp), "");
	security_mode = cjson_get(root, strcat_r(prefix, "akm", tmp), "");
	if (strcmp(ssid, ""))
	{	
		memset(tmp, 0, sizeof(tmp));
		if(strlen(ssid) > 29*sizeof(char))
		{
			get_ssid_prefix(ssid,prefix_ssid,sizeof(char)*29);
			strcat_r(prefix_ssid, "_5G", tmp);
		}
		else
		{
			strcat_r(ssid, "_5G", tmp);
		}
		nvram_set(WLAN5G_SSID, tmp);
		
#ifdef __CONFIG_WIFI_DOUBLEBAND_UNITY__
		/*add by lrl 2018/4/9 ˫Ƶ��ѡ���ܿ���ʱ 2.4G �� 5G ������Ϣһ��*/
		if(nvram_match(WLAN_PUBLIC_DOUBLEBANDUN_ENBALE,"1"))
			nvram_set(WLAN5G_SSID, ssid);
#endif

		if ((strlen(psk) >= 8) && strcmp(security_mode, ""))
		{
			nvram_set(WLAN5G_PASSWD, psk);
			nvram_set(WLAN5G_AKM, security_mode);
			nvram_set(WLAN5G_CRYPTO, "aes"); //ҳ�治֧��ѡ��Ĭ��aes
		}
		else
		{
			nvram_set(WLAN5G_AKM, "");
		}
	}

}
/*****************************************************************************
 �� �� ��  : configure_24G_info
 ��������  : �����յ���2.4G����Ϣȥ����2.4G
 �������  : char *cjson
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��12��6��
    ��    ��   : kg
    �޸�����   : �����ɺ���

*****************************************************************************/

void configure_24G_info(char *cjson)
{
	char tmp[PI_BUFLEN_64] = {0};
	char prefix[PI_BUFLEN_16] = {0};
	char  *ssid = NULL, *psk = NULL, *security_mode = NULL;
	cJSON *root = cJSON_Parse(cjson);
	if (!root)
	{
		printf("get root faild!\n");
		return;
	}
	if(strcmp("ap", cjson_get(root, WLAN24G_WORK_MODE, "")))
	{
		sprintf(prefix,"%s_" ,WL_24G_REPEATER);
	}
	else
	{
		sprintf(prefix,"%s_", WL_24G);
	}
	
	ssid = cjson_get(root, strcat_r(prefix, "ssid", tmp), "");
	psk = cjson_get(root, strcat_r(prefix, "wpa_psk", tmp), "");
	security_mode = cjson_get(root, strcat_r(prefix, "akm", tmp), "");
	if (strcmp(ssid, ""))
	{
		nvram_set(WLAN24G_SSID, ssid);

		if ((strlen(psk) >= 8) && strcmp(security_mode, ""))
		{
			nvram_set(WLAN24G_PASSWD, psk);
			nvram_set(WLAN24G_AKM, security_mode);
			nvram_set(WLAN24G_CRYPTO, "aes"); //ҳ�治֧��ѡ��Ĭ��aes
		}
		else
		{
			nvram_set(WLAN24G_AKM, "");
		}
	}
}
/*****************************************************************************
 �� �� ��  : configure_5G_info
 ��������  : �����Ƿ��յ�5G��Ϣȥ������Ӧ��5G��Ϣ
 �������  : char *cjson
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��12��6��
    ��    ��   : kg
    �޸�����   : �����ɺ���

*****************************************************************************/

void configure_5G_info(char *cjson)
{
	cJSON *root = cJSON_Parse(cjson);
	if (!root)
	{
		printf("get root faild!\n");
		return;
	}
	if ( strcmp("", cjson_get(root, WLAN5G_SSID, "")))
	{
		configure_5G_reinfo(cjson);	
	}
	else
	{
		configure_5G_fr24G(cjson);
	}
}


static void nvram_to_default()
{
		nvram_set(WLAN24G_REPEATER_ENABLE, "0");
		nvram_set(WLAN24G_REPEATER_SSID, "Tenda_repeater");
		nvram_set(WLAN24G_REPEATER_WEP, "disabled");
		nvram_set(WLAN24G_REPEATER_AKM, "");
		nvram_set(WLAN24G_REPEATER_CRYPTO, "");
		nvram_set(WLAN24G_REPEATER_PASSWD, "");
		nvram_set(WLAN24G_REPEATER_WORK_MODE, "client");
		nvram_set(WLAN5G_REPEATER_ENABLE, "0");
		nvram_set(WLAN5G_REPEATER_SSID, "Tenda_repeater");
		nvram_set(WLAN5G_REPEATER_WEP, "disabled");
		nvram_set(WLAN5G_REPEATER_AKM, "");
		nvram_set(WLAN5G_REPEATER_CRYPTO, "");
		nvram_set(WLAN5G_REPEATER_PASSWD, "");
		nvram_set(WLAN5G_REPEATER_WORK_MODE, "client");
}
/*****************************************************************************
 �� �� ��  : analysis_cjson_data
 ��������  : ���ݽ��յ������ݽ��н�����������Ӧ��nvramֵ
 �������  : char *cjson
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��11��26��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
void analysis_cjson_data(char *cjson)
{
	cJSON *root = cJSON_Parse(cjson);
	char msg_param[PI_BUFLEN_32] = {0};
	if (!root)
	{
		printf("get root faild!\n");
		return;
	}

	if (nvram_match(SYSCONFIG_QUICK_SET_ENABLE, "0"))
	{
		printf("%s quick_set had over!\n", __func__);
		cJSON_Delete(root);
		return;
	}

	//��������
	configure_to_Internet(cjson);
	if(atoi(nvram_safe_get(WLAN24G_ENABLE)))
	{
		configure_24G_info(cjson);
	}
	if(atoi(nvram_safe_get(WLAN5G_ENABLE)))
	{
#ifdef __CONFIG_WIFI_DOUBLEBAND_UNITY__
		/*add by lrl 2018/4/9 ˫Ƶ��ѡ���� 2.4G �� 5G ������Ϣһ��*/
		if(nvram_match(WLAN_PUBLIC_DOUBLEBANDUN_ENBALE,"1"))
			configure_5G_fr24G(cjson);
		else
			configure_5G_info(cjson);	
#else
		configure_5G_info(cjson);
#endif
	}
	memset(msg_param,0,sizeof(msg_param));
	sprintf(msg_param, "op=%d", OP_RESTART);
	msg_send(MODULE_RC, RC_WIFI_MODULE, msg_param);
	nvram_set(SYSCONFIG_QUICK_SET_ENABLE, "0");
	auto_sync_end =1;
	cJSON_Delete(root);

#ifdef __CONFIG_INDICATE_LED__
	set_indicate_led_on();
#endif
	nvram_to_default();	
	nvram_commit();
}
extern int get_wlan_id(char *ifname);
extern int
send_ether_packet(struct ifnet *test_ifnet, struct ether_header *eh_rcv_pkt, char *msg, int len);
int send_req_pkt(struct ifnet *send_ifnet)
{
	struct sockaddr dst;
	struct ether_header *eh;
	int s;
	char *router_mac = NULL;
	struct ether_addr router_addr;
	unsigned int br0_mac = 0;
	unsigned char br0_macaddr[8] = {0};
	char if_name[PI_BUFLEN_16]={0};
	/*
	* Set destination to send and tell
	* the ether_output() to do raw send
	* without routing for us.
	*/
	memset(&dst, 0, sizeof(dst));
	dst.sa_len = 16;
	dst.sa_family = AF_UNSPEC;

	eh = (struct ether_header *)dst.sa_data;
	eh->ether_type = ntohs(PORT_ATUO_CONN);

#if defined(BCM535X)

	if (get_wl_bssid(&router_addr) < 0)
#elif defined(RTL819X)
	bss_info bss;
	int get_bss_ret = 0;
	if(device_name_24G == AUTO_CONN_VIF_24G && device_name_5G != AUTO_CONN_VIF_5G)
	{
		get_bss_ret = getWlBssInfo(TENDA_WLAN24_AP_IFNAME, &bss);
	}
	if(device_name_5G == AUTO_CONN_VIF_5G && device_name_24G != AUTO_CONN_VIF_24G)
	{
		get_bss_ret = getWlBssInfo(TENDA_WLAN5_AP_IFNAME, &bss);
	}
	memcpy(&router_addr, &bss.bssid, 6);

	if (get_bss_ret  < 0)
#endif
	{
		router_mac = nvram_recv_safe_get("auto_conn_prev_mac");

		if (strcmp(router_mac, "") == 0 || ether_atoe(router_mac, &router_addr) == 0)
		{
			memcpy(eh->ether_dhost, "\xff\xff\xff\xff\xff\xff", sizeof(eh->ether_dhost));
		}
		else
		{
			memcpy(br0_macaddr, router_addr.octet, sizeof(eh->ether_dhost));
			br0_mac = (br0_macaddr[3] & 0xFF);
			br0_mac = (br0_mac << 8) + (br0_macaddr[4] & 0xFF);
			br0_mac = (br0_mac << 8) + (br0_macaddr[5] & 0xFF);
			br0_mac -= 1;
			br0_macaddr[3] = (br0_mac >> 16) & 0xFF;
			br0_macaddr[4] = (br0_mac >> 8) & 0xFF;
			br0_macaddr[5] = (br0_mac & 0xFF);
			memcpy(eh->ether_dhost, br0_macaddr, sizeof(eh->ether_dhost));
		}
	}
	else
	{
		memcpy(br0_macaddr, router_addr.octet, sizeof(eh->ether_dhost));
		br0_mac = (br0_macaddr[3] & 0xFF);
		br0_mac = (br0_mac << 8) + (br0_macaddr[4] & 0xFF);
		br0_mac = (br0_mac << 8) + (br0_macaddr[5] & 0xFF);
		br0_mac -= 3;
		br0_macaddr[3] = (br0_mac >> 16) & 0xFF;
		br0_macaddr[4] = (br0_mac >> 8) & 0xFF;
		br0_macaddr[5] = (br0_mac & 0xFF);
		memcpy(eh->ether_dhost, br0_macaddr, sizeof(eh->ether_dhost));
	}

	/*
	*send msg
	*/
	char sndBuf[32] = {0};
	int snd_len = 0;
	auto_conn_pkt *tmp = (auto_conn_pkt *)sndBuf;
	if(device_name_24G == AUTO_CONN_VIF_24G && device_name_5G != AUTO_CONN_VIF_5G)
	{
		if (nvram_match(WLAN24G_WORK_MODE, "ap"))
		{
			tmp->rnd = htonl(0x1235);//����ͬ��
		}
		else 
		{
			tmp->rnd = htonl(0x1234);//�Զ��Ž�
		}
	}
	if(device_name_5G == AUTO_CONN_VIF_5G && device_name_24G != AUTO_CONN_VIF_24G)
	{
		if (nvram_match(WLAN5G_WORK_MODE, "ap"))
		{
			tmp->rnd = htonl(0x1235);//����ͬ��
		}
		else
		{
			tmp->rnd = htonl(0x1234);//�Զ��Ž�
		}
	}
	tmp->ack = htonl(0x0);
	tmp->dataLen = 0;
	snd_len = sizeof(auto_conn_pkt);
	crypt_decrypt_rc4(sndBuf, snd_len, eh);	//rc4 encrypt

	AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "###dhost=%02x:%02x:%02x:%02x:%02x:%02x\n",
	                   eh->ether_dhost[0], eh->ether_dhost[1], eh->ether_dhost[2],
	                   eh->ether_dhost[3], eh->ether_dhost[4], eh->ether_dhost[5]);

	/*send packet*/
	send_ether_packet(send_ifnet, eh, sndBuf, snd_len);

	return 1;
}
void process_auto_conn(unsigned char *recv_data)
{
	
	if(device_name_24G == AUTO_CONN_VIF_24G && device_name_5G != AUTO_CONN_VIF_5G)	
	{
		copy_config_24G(recv_data);
	}
	if(device_name_5G == AUTO_CONN_VIF_5G && device_name_24G != AUTO_CONN_VIF_24G)
	{
		copy_config_5G(recv_data);
	}

#if defined(BCM535X)
	auto_conn_sys_restart();
#elif defined(RTL819X)
	char msg_param[PI_BUFLEN_256] = {0};
	sprintf(msg_param, "op=%d", OP_RESTART);
	msg_send(MODULE_RC, RC_AUTO_CONN_CLIENT_MODULE, msg_param);
#endif
}
void process_auto_quickset(unsigned char *recv_data)
{
	analysis_cjson_data(recv_data);
}

void recv_conf_pkt(struct mbuf *m_rcv, struct ether_header *eh)
{
	auto_conn_pkt *recv_buf = NULL;
	char buf[CONF_BUF_LEN] = {0};
	if (m_rcv == NULL)
	{
		printf("###func=%s,recv mbuf erroe!\n", __FUNCTION__);
		return;
	}

	memcpy(buf, m_rcv->m_data, m_rcv->m_len);
	recv_buf = (auto_conn_pkt *)buf;

	//decrypt rc4
	crypt_decrypt_rc4(recv_buf, m_rcv->m_len, eh);

	if (0x4321 == ntohl(recv_buf->ack) && 0x1234 == ntohl(recv_buf->rnd))
	{
		/*׼���������ߣ�����־λ�ó�1����־����������*/
		tpi_auto_conn_client_set_restarting_tag(1);
		auto_conn_extend_status = AUTO_CONN_VIF_EXTEND_DONE;
		//auto_conn_getting_ip = 1;
	}
	else if (0x4321 != ntohl(recv_buf->ack))
	{
		printf("###func=%s, the request is illegal!\n", __FUNCTION__);
		return;
	}

	AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "rnd=%08x, ack=%08x, dataLen=%08x\n",
	                   ntohl(recv_buf->rnd), ntohl(recv_buf->ack), ntohl(recv_buf->dataLen));

	//printf("pkt_addr=%08x,data_addr=%08x,pkt_len=%d\n", recv_buf, recv_buf, ntohl(recv_buf->dataLen));
	//print_pkt(recv_buf, ntohl(recv_buf->dataLen));
	//printf("pakcet_data:");
	if (0x1234 == ntohl(recv_buf->rnd))
	{
		process_auto_conn(&(recv_buf->data));
	}
	else if (0x1235 == ntohl(recv_buf->rnd))
	{
		process_auto_quickset(&(recv_buf->data));
	}

}
/*****************************************************************************
 �� �� ��  : set_5G_vxd_default
 ��������  : �����ڻ�ȡ������״̬ʱ�޸�vxd�ӿڲ���
 �������  : ��
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��11��25��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
void set_5G_vxd_default()
{
	char msg_tmp[PI_BUFLEN_32] = {0};

	nvram_set(WLAN5G_REPEATER_ENABLE, "1");
	nvram_set(WLAN5G_REPEATER_WORK_MODE, "sta");
	nvram_set(WLAN5G_REPEATER_SSID, "Tenda_Extender_default");
	memset(msg_tmp,0,sizeof(msg_tmp));
	sprintf(msg_tmp,"op=%d,wlan_ifname=%s",OP_RESTART,TENDA_WLAN5_REPEATER_IFNAME);
	msg_send(MODULE_RC,RC_WIFI_MODULE,msg_tmp);	
}
/*****************************************************************************
 �� �� ��  : set_24G_vxd_default
 ��������  : �����ڻ�ȡ������״̬ʱ�޸�vxd�ӿڲ���
 �������  : ��
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��11��25��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
void set_24G_vxd_default()
{
	char msg_tmp[PI_BUFLEN_32] = {0};
	
	nvram_set(WLAN24G_REPEATER_ENABLE, "1");
	nvram_set(WLAN24G_REPEATER_WORK_MODE, "sta");
	nvram_set(WLAN24G_REPEATER_SSID, "Tenda_Extender_default");
	sprintf(msg_tmp,"op=%d,wlan_ifname=%s",OP_RESTART,TENDA_WLAN24_REPEATER_IFNAME);
	msg_send(MODULE_RC,RC_WIFI_MODULE,msg_tmp);	
}

/*****************************************************************************
 �� �� ��  : set_vxd_conf
 ��������  : �Զ��Žӿͻ�������Լ�����߲���
 �������  : ��
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��11��25��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
void set_vxd_conf()
{
	AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "%s switch to AUTO_CONN_VIF_EXTEND_DOING.\n", __func__);
	char msg_tmp[PI_BUFLEN_32] = {0};
	if(device_name_24G == AUTO_CONN_VIF_24G && device_name_5G != AUTO_CONN_VIF_5G)
	{
		nvram_set(WLAN24G_REPEATER_ENABLE, "1");
		nvram_set(WLAN24G_REPEATER_SSID, "Tenda_Extender_1");
		nvram_set(WLAN24G_REPEATER_WEP, "disabled");
		nvram_set(WLAN24G_REPEATER_AKM, "psk2");
		nvram_set(WLAN24G_REPEATER_CRYPTO, "aes");
		nvram_set(WLAN24G_REPEATER_PASSWD, "Extender123456");
		nvram_set(WLAN24G_REPEATER_WORK_MODE, "sta");
		sprintf(msg_tmp,"op=%d,wlan_ifname=%s",OP_RESTART,TENDA_WLAN24_REPEATER_IFNAME);
		msg_send(MODULE_RC, RC_WIFI_MODULE, msg_tmp);
	}
	if(device_name_5G == AUTO_CONN_VIF_5G && device_name_24G != AUTO_CONN_VIF_24G)
	{
		nvram_set(WLAN5G_REPEATER_ENABLE, "1");
		nvram_set(WLAN5G_REPEATER_SSID, "Tenda_Extender_1");
		nvram_set(WLAN5G_REPEATER_WEP, "disabled");
		nvram_set(WLAN5G_REPEATER_AKM, "psk2");
		nvram_set(WLAN5G_REPEATER_CRYPTO, "aes");
		nvram_set(WLAN5G_REPEATER_PASSWD, "Extender123456");
		nvram_set(WLAN5G_REPEATER_WORK_MODE, "sta");
		sprintf(msg_tmp,"op=%d,wlan_ifname=%s",OP_RESTART,TENDA_WLAN5_REPEATER_IFNAME);
		msg_send(MODULE_RC, RC_WIFI_MODULE, msg_tmp);
	}
	auto_conn_extend_status = AUTO_CONN_VIF_EXTEND_DOING;
}


void set_default_conf()
{
	auto_conn_extend_status = AUTO_CONN_VIF_EXTEND_DOING;

	//if(strcmp(nvram_recv_safe_get("wl0_ssid"), "Tenda_Extender_1") == 0)
	//	return;

	nvram_set("wl_ssid", "Tenda_Extender_1");
	//nvram_set("wl_wps_mode", "disabled");
	nvram_set("wl_auth", "0");
	nvram_set("wl_wep", "disabled");
	nvram_set("wl_akm", "psk2");
	nvram_set("wl_crypto", "aes");
	nvram_set("wl_wpa_psk", "Extender123456");
	nvram_set("wl0_ssid", "Tenda_Extender_1");
	nvram_set("wps_mode", "enabled");
	nvram_set("wl0_auth", "0");
	nvram_set("wl0_wep", "disabled");
	nvram_set("wl0_akm", "psk2");
	nvram_set("wl0_crypto", "aes");
	nvram_set("wl0_wpa_psk", "Extender123456");

#if defined(BCM535X)
	sys_restart();
#elif defined(RTL819X)
	char msg_param[PI_BUFLEN_256] = {0};
	sprintf(msg_param, "op=%d", OP_RESTART);
	msg_send(MODULE_RC, RC_WIFI_MODULE, msg_param);
#endif
}

void auto_conn_check_status()
{
			
			static time_t	auto_conn_time = 0;
			static int		last_auto_status = AUTO_CONN_VIF_EXTEND_UNDO;
			
			if (auto_conn_extend_status == AUTO_CONN_VIF_EXTEND_DOING)
			{
				if (last_auto_status != AUTO_CONN_VIF_EXTEND_DOING)
				{
					auto_conn_time = time(NULL);
				}
				else if (time(NULL) - auto_conn_time > 60)
				{
					auto_conn_extend_status = AUTO_CONN_VIF_EXTEND_UNDO;
#ifdef __CONFIG_EXTEND_LED__
			extend_led_stop_blink();
#endif
#ifdef __CONFIG_INDICATE_LED__
			set_indicate_led_off(0);
#endif
#ifdef __CONFIG_A9__
			restore_wl_conf();//�Զ��Žӳ�ʱ��ԭ�ֶ��Žӵ���������

		//����Զ��Žӳ�ʱ��SSID�����ȻΪTenda_Extender_1��Ҫ�޸�Ϊ����ֵ
			if (0 == strcmp(nvram_safe_get("wl0_ssid"), "Tenda_Extender_1"))
			{
				nvram_set("wl0_ssid", "Tenda_Extender_default");
				char msg_tmp[PI_BUFLEN_32] = {0};
				sprintf(msg_tmp,"op=%d,wlan_ifname=%s",OP_START,TENDA_WLAN5_REPEATER_IFNAME);
				msg_send(MODULE_RC,RC_WIFI_MODULE,msg_tmp);
			}
#else
			set_24G_vxd_default();
			set_5G_vxd_default();
#endif
			AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "wait for 1min, have no response from router, switch to AUTO_CONN_VIF_EXTEND_UNDO.\n");
		}
	}
	else
	{
		auto_conn_time = time(NULL);
	}

	last_auto_status = auto_conn_extend_status;
#ifndef __CONFIG_A9__

	//·�����ϼ�⵽�������ý��������client�߳�,������server�߳�
	if (nvram_match("restore_quick_set", "0"))
	{
		msg_send(MODULE_RC, RC_AUTO_CONN_CLIENT_MODULE, "op=2");//stop client
		msg_send(MODULE_RC, RC_AUTO_CONN_SERVER_MODULE, "op=1");//start serve
	}

#endif
}

extern struct ifnet *ifunit2(const char *name);

int exist_auto_server = 0;
void set_exist_auto_server(int i)
{
	exist_auto_server = i;
	AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "set_exist_auto_server(%d)\n", i);
}

int no_exist_count = 0;
int no_auto_server()
{
	if (1 == exist_auto_server)
	{
		no_exist_count = 0;
	}
	else
	{
		no_exist_count++;
	}

	AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "exist_auto_server:%d  no_exist_count:%d\n", exist_auto_server, no_exist_count);
	return no_exist_count;
}

/************************************************************
Function:	 cp_wl_conf
Description: ���Ƶ������߲���
Input:
	���߲���ǰ׺����wl0_,wl0.3_
	���߲������ƣ���ssid��channel
Output:
Return:
Others:
************************************************************/
void cp_wl_conf(char *from, char *to, char *name)
{
	AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "cp:%s%s to %s%s\n", from, name, to, name);

	if (NULL == name)
	{
		return;
	}

	char from_tmp[128], to_tmp[128];
	nvram_set(strcat_r(to, name, to_tmp), nvram_safe_get(strcat_r(from, name, from_tmp)));
}

/************************************************************
Function:	 cp_wl_confs
Description: ���ƶ������߲���
Input:
	���߲���ǰ׺����wl0_,wl0.3_
Output:
Return:
Others:
************************************************************/
void cp_wl_confs(char *from, char *to)
{
	if (NULL == from || NULL == to)
	{
		return;
	}

	cp_wl_conf(from, to, "relaytype"); //��¼�������Žӷ�ʽ�Ĳ���
	cp_wl_conf(from, to, "ssid");
	cp_wl_conf(from, to, "channel");
	cp_wl_conf(from, to, "nctrlsb"); //��չ����
	//������Ϣ
	cp_wl_conf(from, to, "wpa_psk");
	cp_wl_conf(from, to, "crypto");
	cp_wl_conf(from, to, "akm");
	cp_wl_conf(from, to, "auth");
	cp_wl_conf(from, to, "wep");
	//bgnģʽ
	cp_wl_conf(from, to, "nmode");
	cp_wl_conf(from, to, "gmode");
	cp_wl_conf(from, to, "plcphdr");
}
void back_wl_conf()
{
	if (0 == strcmp(nvram_safe_get("wl0_ssid"), "Tenda_Extender_1") ||
	    0 == strcmp(nvram_safe_get("wl0_relaytype"), "auto") ||
	    0 == strcmp(nvram_safe_get("wl0_relaytype"), "")
	   )
	{
		AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "not need back wl config!\n");
		return;
	}

	AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "back_wl_conf!\n");
	/*�Զ��Ž��޸�����ǰ����ԭ�ϼ��ӿڲ���wl0��wl0.3*/
	cp_wl_confs("wl0_", "wl0.3_");
	nvram_commit();
}

void restore_wl_conf()
{
	if (0 == strcmp(nvram_safe_get("wl0.3_ssid"), "") ||
	    (0 != strcmp(nvram_safe_get("wl0_relaytype"), "auto") &&
	     0 == strcmp(nvram_safe_get("wl0.3_ssid"), nvram_safe_get("wl0_ssid")) &&
	     0 == strcmp(nvram_safe_get("wl0.3_wpa_psk"), nvram_safe_get("wl0_wpa_psk")) &&
	     0 == strcmp(nvram_safe_get("wl0.3_crypto"), nvram_safe_get("wl0_crypto")) )
	   )
	{
		AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "not need restore wl_conf!  wl0.3_conf not exist or same!\n");
		return;
	}

	AUTO_CONN_DBGPRINT(AUTO_CONN_DEBUG_ERR, "restore_wl_conf!\n");
	/*�Զ��Žӷ�����������ʱ��ԭ�ϼ��ӿ�wl0.3��wl0*/
	cp_wl_confs("wl0.3_", "wl0_");
	msg_send(MODULE_RC, RC_WIFI_MODULE, "op=3");
}
void do_doing_state(struct ifnet *send_ifnet_24G,struct ifnet *send_ifnet_5G)
{
	if(NULL == send_ifnet_24G || NULL == send_ifnet_5G)
	{
		return;
	}
	if(device_name_24G == AUTO_CONN_VIF_24G && device_name_5G != AUTO_CONN_VIF_5G)
	{	
		send_req_pkt(send_ifnet_24G);
	}
	if(device_name_5G == AUTO_CONN_VIF_5G && device_name_24G != AUTO_CONN_VIF_24G)
	{
		send_req_pkt(send_ifnet_5G);
	}
}
void do_setdef_state(struct ifnet *send_ifnet_24G,struct ifnet *send_ifnet_5G)
{
#ifdef __CONFIG_EXTEND_LED__
	extend_led_start_blink();
#endif
#ifdef __CONFIG_INDICATE_LED__
	if (1 == set_indicate_led_blink(0))
#endif
	{
#ifdef __CONFIG_A9__
		back_wl_conf();//�Զ��Ž��޸����߲���ǰ�����ֶ��Žӵ����߲���
		set_default_conf();
#else
		set_vxd_conf();
#endif
		
	}

}
void do_undo_state(struct ifnet *send_ifnet_24G,struct ifnet *send_ifnet_5G)
{
#ifdef __CONFIG_A9__
	if (auto_conn_extend_status == AUTO_CONN_VIF_EXTEND_UNDO &&
		 memcmp(nvram_safe_get("manual_set"), "0", 1) == 0 &&
		 no_auto_server() >= 8
	   )
		{
			restore_wl_conf();//δ�Ž�״̬�£����Զ��Žӷ����������ڵ�ʱ�򣬻ָ��ֶ��ŽӵĲ���
			nvram_set("manual_set", "1"); //��ʱͣ���Զ��Ž�
		}
#endif
return;
}
CLIENT_STATE do_state[3] = 
{
	{AUTO_CONN_VIF_EXTEND_UNDO, do_undo_state},
	{AUTO_CONN_VIF_EXTEND_SETDEF, do_setdef_state},
	{AUTO_CONN_VIF_EXTEND_DOING , do_doing_state},
};

void auto_conn_extend_main()
{
	auto_client_stop = 0;
	int recv_conf = 0;
	struct ifnet *send_ifnet = NULL;
	struct ifnet *send_ifnet_24G = NULL;
	struct ifnet *send_ifnet_5G = NULL;
	int state_index= -1;
	if(nvram_match(WLAN24G_WORK_MODE, "ap"))
	{
		set_24G_vxd_default();
	}
	if(nvram_match(WLAN5G_WORK_MODE, "ap"))
	{
		set_5G_vxd_default();
	}
	cyg_thread_delay(300);

#if defined(BCM535X)
	send_ifnet = ifunit2("br0");
	if (send_ifnet == NULL || send_ifnet == 0)
	{
		printf("###func=%s,look up ifnet error!!!\n", __FUNCTION__);
		return;
	}

#elif defined(RTL819X)
	send_ifnet_24G = ifunit2(TENDA_WLAN24_REPEATER_IFNAME);
	send_ifnet_5G = ifunit2(TENDA_WLAN5_REPEATER_IFNAME);

	if (send_ifnet_24G == NULL || send_ifnet_24G == 0 || send_ifnet_5G == NULL || send_ifnet_5G == 0)
	{
		printf("###func=%s,look up ifnet error!!!\n", __FUNCTION__);
		return;
	}
	
#endif
	auto_conn_extend_status = AUTO_CONN_VIF_EXTEND_UNDO;
	while (1)
	{
		if(auto_client_stop)
		{
			break;
		}
		for(state_index = 0;state_index < 3;state_index++)
		{	
			if(do_state[state_index].state == auto_conn_extend_status)
			{
				DO_DOING(do_state[state_index])(send_ifnet_24G,send_ifnet_5G);
			}
		}
		auto_conn_check_status();

		cyg_thread_delay(300);
	}
}


void auto_conn_extend_stop()
{
	auto_client_stop = 1;
}


#if defined(BCM535X)
static cyg_handle_t auto_conn_extend_handle;
static cyg_thread 	auto_conn_extend_thread;
static char 		auto_conn_extend_stack[1024 * 64];

void auto_conn_extend_start()
{
	cyg_thread_create(
	    8,
	    (cyg_thread_entry_t *)auto_conn_extend_main,
	    0,
	    "auto_conn_extend",
	    (void *)&auto_conn_extend_stack[0],
	    sizeof(auto_conn_extend_stack),
	    &auto_conn_extend_handle,
	    &auto_conn_extend_thread);

	cyg_thread_resume(auto_conn_extend_handle);
}
#endif
