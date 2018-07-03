/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cgi_wifi_scan.c
  �� �� ��   : ����
  ��    ��   : liquan
  ��������   : 2017��10��24��
  ����޸�   :
  ��������   : 

  ��������   : ����ɨ��

  �޸���ʷ   :
  1.��    ��   : 2017��10��24��
    ��    ��   : liquan
    �޸�����   : �����ļ�

******************************************************************************/

#include "webs.h"
#include "cJSON.h"
#include "wifi.h"
#include "wan.h"
#include<wl_utility_rltk.h>
#include<pi_common.h>


//���߿ͻ��˵ĸ���
#define WLAN_SCAN_RESULT_CLIENT_NUM  64	//origin 50 ,pxy revise to 64 as same sa BCM
//��ǰɨ��״̬
typedef enum scan_stat
{
	SCANING = 0,
	SCAN_COMPLETED
}SCAN_STAT;
//��ǵ�ǰɨ��״̬
static SCAN_STAT scanstat = SCAN_COMPLETED;
//ɨ������ŵ�����
static wlan_sscan_list_info_t wlan24_scan_result[WLAN_SCAN_RESULT_CLIENT_NUM];
static wlan_sscan_list_info_t wlan5_scan_result[WLAN_SCAN_RESULT_CLIENT_NUM];

/*****************************************************************************
 �� �� ��  : package_ssan_result
 ��������  : ɨ������װ
 �������  : cJSON* root                          
             wlan_sscan_list_info_t *client_list  
             char *ifname                         
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��24��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
void package_ssan_result(cJSON* root,wlan_sscan_list_info_t *client_list,char *ifname)
{
	int channel_num = 0;
	cJSON *obj = NULL;
	char tmp[256] = {0};
	WLAN_RATE_TYPE wl_type = WLAN_RATE_24G;
	
	wlan_sscan_list_info_t *tmp_client_list = client_list;
	
	if(0 == strcmp(ifname,TENDA_WLAN5_AP_IFNAME))
		wl_type = WLAN_RATE_5G;

	for(;tmp_client_list->used == TRUE;tmp_client_list++)
	{
		if(!gpi_wifi_channles_in_country(wl_type,tmp_client_list->channel)
			|| strstr(tmp_client_list->SecurityMode,"UNKNOW")
			|| strcmp(tmp_client_list->encode,"UTF-8"))
		{
			continue;
		}

		cJSON_AddItemToArray(root, obj = cJSON_CreateObject());
		cJSON_AddStringToObject(obj, "wifiScanSSID",tmp_client_list->ssid);
		cJSON_AddStringToObject(obj, "wifiScanMAC", tmp_client_list->bssid);
		//cJSON_AddStringToObject(obj, "wifiScanEncode", tmp_client_list->encode);
		memset(tmp,0x0,256);
		sprintf(tmp,"%d",tmp_client_list->channel);
		cJSON_AddStringToObject(obj, "wifiScanChannel",tmp );
		cJSON_AddStringToObject(obj, "wifiScanSecurityMode", tmp_client_list->SecurityMode);
		memset(tmp,0x0,256);
		sprintf(tmp, "%d\%", (tmp_client_list->SignalStrength));
		cJSON_AddStringToObject(obj, "wifiScanSignalStrength", tmp);
		if(wl_type == WLAN_RATE_24G)
		{
			cJSON_AddStringToObject(obj, "wifiScanChkHz", "24G");
		}else
		{
			cJSON_AddStringToObject(obj, "wifiScanChkHz", "5G");
		}
		
	}

}
/*****************************************************************************
 �� �� ��  : cgi_wifi_scanresult_get
 ��������  : ����ɨ��ҳ��Ϳ�������ɨ��������װ
 				�����ҷ��ص�ǰɨ���״̬��
           			  ɨ����  ɨ��ʧ��   ɨ��ɹ�
 �������  : cJSON *root  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��24��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
int cgi_wifi_scanresult_get(cJSON *root)
{
	cJSON *array = NULL;

	NETWORK_INTERFACE_STATE wlan24_stat = INTERFACE_STAT_UNKNOW;
	NETWORK_INTERFACE_STATE wlan5_stat = INTERFACE_STAT_UNKNOW;

	if(scanstat == SCANING)
		return -1;
	
	scanstat = SCANING;

	if(root)
		cJSON_AddItemToObject(root, "wifiScan", array = cJSON_CreateArray());
	else
		return -1;
	
	wlan24_stat = get_interface_state(TENDA_WLAN24_AP_IFNAME);
	wlan5_stat = get_interface_state(TENDA_WLAN5_AP_IFNAME);

	if(INTERFACE_UP != wlan24_stat && INTERFACE_UP != wlan5_stat)
	{
		scanstat = SCAN_COMPLETED;
		return 0;
	}
	
	if(INTERFACE_UP == wlan24_stat 
		&& !gpi_get_wifi_scan_result(wlan24_scan_result,TENDA_WLAN24_AP_IFNAME) )
	{
		scanstat = SCAN_COMPLETED;
		return 0;
	}
	
	if(INTERFACE_UP == wlan5_stat 
	&& !gpi_get_wifi_scan_result(wlan5_scan_result,TENDA_WLAN5_AP_IFNAME) )
	{
		scanstat = SCAN_COMPLETED;
		return 0;
	}

	if(INTERFACE_UP == wlan24_stat )
		package_ssan_result(array,wlan24_scan_result,TENDA_WLAN24_AP_IFNAME);
	
	if(INTERFACE_UP == wlan5_stat )
		package_ssan_result(array,wlan5_scan_result,TENDA_WLAN5_AP_IFNAME);

	scanstat = SCAN_COMPLETED;
	return 0;

}
/*****************************************************************************
 �� �� ��  : formGetWifiScan
 ��������  : ɨ��ҳ���goform����
 �������  : webs_t wp      
             char_t *path   
             char_t *query  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��24��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
void formGetWifiScan(webs_t wp, char_t *path, char_t *query)
{
	char *out = NULL;
	cJSON *root = NULL;
	int ret = 0;
	root = cJSON_CreateObject();

	ret = cgi_wifi_scanresult_get(root);

	if(ret == -1)
	{
		cJSON_Delete(root);
		return;
	}else if(ret == 0)
	{
		websWrite(wp, T("stanley\n"));
		websDone(wp,200);
		return;
	}
	
	out = cJSON_Print(root);
	cJSON_Delete(root);
	websWrite(wp, T("HTTP/1.0 200 OK\r\n\r\n"));
	websWriteLongString(wp, out);
	free(out);
	websDone(wp, 200);
	return ;

}

