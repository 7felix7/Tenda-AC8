#include <stdio.h>
#include <stdlib.h>
#include "flash_cgi.h"
#include "cgi_handle_module.h"
#include "cgi_lib_config.h"
/*************************************************************************
  ��int�͵�4���ֽڷֳ������ֽ�һ�飬�������ֽڹ���16λ��0-15λ����ÿһλ��ʾ֧�ֵ�һ����λ��
  ����������ֽ�Ϊ0x0007�����ʾ֧�ָ��е�3�������������ֽ�Ϊ0x0005�����ʾ֧�ָߺ͵�������
  ���������ֽ�Ϊ0x0003�����ʾ֧���к͵��������������ֽڵ�ÿһλ���ʾ��ǰ��wifiǿ�ȣ�
  ����������ֽ�Ϊ0x0004,0x0002,0x0001,��ֱ��ʾ��ǰwifiǿ��Ϊ���е�
  
  0x0007 0001����ʾ֧�ָ��е�3����λ����ǰwifiǿ��Ϊ�ͣ�
  0x0007 0002����ʾ֧�ָ��е�3����λ����ǰwifiǿ��Ϊ�У�
  0x0007 0004����ʾ֧�ָ��е�3����λ����ǰwifiǿ��Ϊ�ߣ�
  0x0005 0001����ʾ֧�ָߺ͵�2����λ����ǰwifiǿ��Ϊ�ͣ�
  0x0005 0004����ʾ֧�ָߺ͵�2����λ����ǰwifiǿ��Ϊ��;
 ************************************************************************/
#define THIRD_GEAR      458752  /*0x0007 0000 ֧�ָߡ��С���������λ*/
#define SECOND_GEAR     393216  /*0x0006 0000 ֧����ǿ����ͨ������λ*/ 
#define HIGH_POWER		4 
#define NOMAL_POWER		2
#define LOW_POWER		1	

//֧�ֵĹ��ʵȼ�,����ҳ����ʾ���ʵȼ�
#define SURPOT_HIGH_POWER    "no_hide"
#define SURPOT_NORMAL_POWER  "hide_normal"
#define SURPOT_LOW_POWER     "hide_power"

extern int app_msg_op_code_2g;
extern int app_msg_op_code_5g;
/****************************************************************************
������  :security_mode_format
����    :����app��ȡ/���� ���߼��ܷ�ʽ ��ʽ���̨��ʽ��ת��
����    :
    	char *sec_mode
        char *sec_mode_format
��    ��   : 2017��11��15��
��    ��   : luorilin
�޸�����   : �½�����
****************************************************************************/ 
static void security_mode_format(char *sec_mode,char *sec_mode_format) 
{ 
	if(NULL == sec_mode || NULL == sec_mode_format)
	{
		return;
	}
	
	if(0 == strcmp(sec_mode,"wpa-psk"))
	{
		sprintf(sec_mode_format,"%s","WPA/AES");
	}
	else if(0 == strcmp(sec_mode,"wpa2-psk"))
	{
		sprintf(sec_mode_format,"%s","WPA2/AES");
	}
	else if(0 == strcmp(sec_mode,"wpa&wpa2") 
		|| (0 == strcmp(sec_mode,"WPA/WPA2-PSK")))
	{
		sprintf(sec_mode_format,"%s","WPAWPA2/AES");
	}
    else if(0 == strcmp(sec_mode,"WPA/AES"))
	{
		sprintf(sec_mode_format,"%s","wpa-psk");
	}
	else if(0 == strcmp(sec_mode,"WPA2/AES"))
	{
		sprintf(sec_mode_format,"%s","wpa2-psk");
	}
	else if(0 == strcmp(sec_mode,"WPAWPA2/AES"))
	{
		sprintf(sec_mode_format,"%s","wpa&wpa2");
	}
	else
	{
		sprintf(sec_mode_format,"%s","NONE");
	}
	
	return;
}

/*****************************************************************************
 �� �� ��  : app_get_wl_power
 ��������  : app��ȡDUT���߹��ʵȼ�
 �������  : cJSON *recv_root  
             cJSON *send_root  
             void *info        
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��31��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���
  2.��	  ��   : 2017��11��17��
    ��	  ��   : luorilin
    �޸�����   : ��ӻ�ȡ5G����

*****************************************************************************/
void app_get_wl_power(cJSON *recv_root,cJSON *send_root, void *info)
{
	CGI_LIB_INFO get_info;
	int app_wifi_power = 0;
	int app_wifi_power_5g = 0;
    PI8 err_code[PI_BUFLEN_32] = {0};
	PIU8 modules[] = 
	{
		MODULE_GET_WIFI_POWER,
	};

	get_info.wp = NULL;
	get_info.root = recv_root;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);

	//2.4G֧�ֵĹ��ʵȼ�
	cJSON *obj = cJSON_GetObjectItem(get_info.root, LIB_WIFI_POWER_GEAR);
	if(strcmp(obj->valuestring,SURPOT_HIGH_POWER) == 0)
		app_wifi_power += THIRD_GEAR;
	else if(strcmp(obj->valuestring,SURPOT_NORMAL_POWER) == 0)//��ʾ֧����������
		app_wifi_power += SECOND_GEAR;
	else if(strcmp(obj->valuestring,SURPOT_LOW_POWER) == 0)//��ʾ��֧�ֹ��ʵ���
		app_wifi_power = 0;

	//2.4G��ȡ��ǰ����
	obj = cJSON_GetObjectItem(get_info.root, LIB_WIFI_POWER);
	if(strcmp(obj->valuestring,"high") == 0)
		app_wifi_power += HIGH_POWER;
	else if(strcmp("normal",obj->valuestring) == 0 || app_wifi_power == SECOND_GEAR)
		app_wifi_power += NOMAL_POWER;
	else if(strcmp(obj->valuestring,"low") == 0)
		app_wifi_power += LOW_POWER;

	cJSON_AddNumberToObject(send_root, "wifi_2g_power",app_wifi_power);

	//5G֧�ֵĹ��ʵȼ�
	cJSON *obj_5g = cJSON_GetObjectItem(get_info.root, LIB_WIFI_POWER_GEAR);
	if(strcmp(obj_5g->valuestring,SURPOT_HIGH_POWER) == 0)
		app_wifi_power += THIRD_GEAR;
	else if(strcmp(obj_5g->valuestring,SURPOT_NORMAL_POWER) == 0)//��ʾ֧����������
		app_wifi_power += SECOND_GEAR;
	else if(strcmp(obj_5g->valuestring,SURPOT_LOW_POWER) == 0)//��ʾ��֧�ֹ��ʵ���
		app_wifi_power = 0;

	//5G��ȡ��ǰ����
	obj_5g = cJSON_GetObjectItem(get_info.root, LIB_WIFI_POWER);
	if(strcmp(obj_5g->valuestring,"high") == 0)
		app_wifi_power += HIGH_POWER;
	else if(strcmp(obj_5g->valuestring,"low") == 0)
		app_wifi_power += LOW_POWER;

	cJSON_AddNumberToObject(send_root, "wifi_5g_power",app_wifi_power_5g);
	
}


/*****************************************************************************
 �� �� ��  : app_set_wl_power_process
 ��������  : ʹset ���ʵȼ���Ч�ĺ�������ʵ���Ƿ�����Ϣ��DUT����ģ��
 �������  : cJSON *send_root     
             CGI_MSG_MODULE *msg  
             int *result_code     
             void *info           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��31��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���
  1.��    ��   : 2017��11��17��
    ��    ��   : luorilin
    �޸�����   : ����wifi���÷���Ϣ��������Ϣ

*****************************************************************************/
void app_set_wl_power_process(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	send_wifi_msg_handle(TENDA_WLAN24_AP_IFNAME,app_msg_op_code_2g,msg);
	app_msg_op_code_2g = COMMON_MSG_MAX;
	send_wifi_msg_handle(TENDA_WLAN5_AP_IFNAME,app_msg_op_code_5g,msg);
	app_msg_op_code_5g = COMMON_MSG_MAX;
}

/*****************************************************************************
 �� �� ��  : app_set_wl_power
 ��������  : app����·�������ʵȼ���ֻ�Ǳ������ã�������Ч������Ƴ���Ϣ
 �������  : cJSON *send_root     
             CGI_MSG_MODULE *msg  
             int *result_code     
             void *info           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��31��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���
  2.��	  ��   : 2017��11��17��
  	��	  ��   : luorilin
    �޸�����   : �������5G����

*****************************************************************************/
void app_set_wl_power(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	CGI_LIB_INFO set_info;
	int wifi_2g_power = 0;
	int wifi_5g_power = 0;
	int current_power_2g = 0;
	int current_power_5g = 0;
    PI8 err_code[PI_BUFLEN_32] = {0};
	PI8 wl_power_2g[PI_BUFLEN_8] = {0};
	PI8 wl_power_5g[PI_BUFLEN_8] = {0};
	CGI_MSG_MODULE remov_msg;
	PIU8 modules[] = 
	{
		MODULE_SET_WIFI_POWER,	
	};

	//2.4G��������
	cJSON *obj = cJSON_GetObjectItem(send_root, "wifi_2g_power");
	if(NULL != obj)
	{
		/* 0x0007 0001����ʾ֧�ָ��е�3����λ����ǰwifiǿ��Ϊ��*/
		wifi_2g_power = obj->valueint;
		
		if(THIRD_GEAR == (wifi_2g_power & THIRD_GEAR))/*֧�ָ��е͹���*/
		{		 
			current_power_2g = wifi_2g_power - THIRD_GEAR; /*��ǰ����*/
			
			if(HIGH_POWER == current_power_2g)
				strcpy(wl_power_2g,"high");
			else if(NOMAL_POWER == current_power_2g)
				strcpy(wl_power_2g,"normal");
			else if(LOW_POWER == current_power_2g)
				strcpy(wl_power_2g,"low");
		}
		else if(SECOND_GEAR == (wifi_2g_power &SECOND_GEAR))/*֧�ָߵ͹���*/
		{
			current_power_2g = wifi_2g_power - SECOND_GEAR;
			
			if(HIGH_POWER == current_power_2g)
				strcpy(wl_power_2g,"high");
			else if(NOMAL_POWER == current_power_2g)
				strcpy(wl_power_2g,"low");
		}
		cJSON_AddStringToObject(send_root, LIB_WIFI_POWER,wl_power_2g);
	}
	/*5G�������� Ŀǰapp��֧��5G��������*/
	obj = cJSON_GetObjectItem(send_root, "wifi_5g_power");
	if(NULL != obj)
	{
		/* 0x0007 0001����ʾ֧�ָ��е�3����λ����ǰwifiǿ��Ϊ��*/
		wifi_5g_power = obj->valueint;

		if(THIRD_GEAR == (wifi_5g_power &THIRD_GEAR))/*֧�ָ��е͹���*/
		{		 
			current_power_5g = wifi_5g_power - THIRD_GEAR; /*��ǰ����*/
			
			if(HIGH_POWER == current_power_5g)
				strcpy(wl_power_5g,"high");
			else if(NOMAL_POWER == current_power_5g)
				strcpy(wl_power_5g,"normal");
			else if(LOW_POWER == current_power_5g)
				strcpy(wl_power_5g,"low");
		}
		else if(SECOND_GEAR == (wifi_5g_power &SECOND_GEAR))/*֧�ָߵ͹���*/
		{
			current_power_5g = wifi_5g_power - SECOND_GEAR;
			
			if(HIGH_POWER == current_power_5g)
				strcpy(wl_power_5g,"high");
			else if(NOMAL_POWER == current_power_5g)
				strcpy(wl_power_5g,"low");
		}
		cJSON_AddStringToObject(send_root, LIB_WIFI_POWER_5G,wl_power_5g);
	}
    
	set_info.wp = NULL;
	set_info.root = send_root;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);

	//��Ϊ����������Ҫ����Ч���룬��ɾ����Ϣ�б��е���Ϣ����app����set_process�����¼�����Ϣ�б�
	memset(remov_msg.msg,0x0,MAX_MODULE_MSG_MAX_LEN);
	remov_msg.id = RC_WIFI_MODULE;
	remove_msg_to_list(msg,remov_msg);

	//���ش�����
	if(strcmp(err_code,"0"))
		*result_code = 1;

}


/*****************************************************************************
 �� �� ��  : app_get_wl_channel_grade
 ��������  : app��ȡwifi���ź����ӵȼ�����Ҫ�����ŵ���ȡ������:20��ߣ�10���
 �������  : cJSON *recv_root  
             cJSON *send_root  
             void *info        
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��31��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
static unsigned int app_current_time = 0;
void app_get_wl_channel_grade(cJSON *recv_root,cJSON *send_root, void *info)
{
	unsigned int current_time = cyg_current_time()/100;

	if(NULL == send_root)
	{
		printf("func:%s line:%d send_root is NULL\n",__func__,__LINE__);
        return;
	}
	/*����:���һ���Ż����Ժ�����������ٽ�������棬����100��*/
	if((current_time - app_current_time) < 300 && app_current_time!=0)
	{
		cJSON_AddNumberToObject(send_root, "channel_2g_sta",20);
		return;
	}
	
	CGI_LIB_INFO get_info;
	cJSON *obj = NULL;
	PIU8 modules[] = 
	{
		MODULE_GET_WLFI_CHANNEL_GRADE,

	};

	obj = cJSON_CreateObject();
    if(NULL == obj)
    {
        printf("[%s][%d]create obj fail\n",__func__,__LINE__);
        return ;
    }
	get_info.wp = NULL;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);

	cJSON_AddNumberToObject(send_root, "channel_2g_sta",cjson_get_number(obj,LIB_CHANNEL_2G_GRADE,15));
	cJSON_Delete(obj);
	return;
}


/*****************************************************************************
 �� �� ��  : app_set_wl_channel_grade
 ��������  : app��wifi�źŵ������ţ���ʵ���ǽ��ŵ������Զ�ѡ��һ�Σ�ѡ�����
             �ŵ�
 �������  : cJSON *send_root     
             CGI_MSG_MODULE *msg  
             int *result_code     
             void *info           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��31��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
void app_set_wl_channel_grade(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	/*��ʼ��ʱ*/
	app_current_time =  cyg_current_time()/100;
	CGI_LIB_INFO set_info;
	
    PI8 err_code[PI_BUFLEN_32] = {0};
	PIU8 modules[] = 
	{
		MODULE_SET_WLFI_CHANNEL_GRADE,	
	};
	
	set_info.wp = NULL;
	set_info.root = send_root;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);
}


/****************************************************************************
������  :app_get_wifi_basic_info
����    :����app��ȡwifi������Ϣ
����    :
    send_root:���ں����ش����ݣ�
        
    info:��
1.��    ��   : 2016��12��23��
  ��    ��   : liusongming
  �޸�����   : �½�����
2.��	��	 : 2017��11��17��
  ��	��	 : luorilin
  �޸�����	 : ��ӻ�ȡ5G������Ϣ

****************************************************************************/
void app_get_wifi_basic_info(cJSON *recv_root,cJSON *send_root, void *info)
{
	int ssid_hide_en = 0;
	int wifi_enable = 1;
    int i = 0;
	char sec_mode[PI_BUFLEN_16] = {0};
    int n_sec_option = 0;
    cJSON *sec_option,* item;
	char *sec[4] = {"NONE","wpa-psk","wpa2-psk","wpa&wpa2"};    //·������֧�ֵļ��ܷ�ʽ��Ŀǰ·����ֻ֧��4��
    cJSON *obj;
	CGI_LIB_INFO get_info;
    
    if(NULL == send_root)
    {
        printf("[%s][%d] send_root is NULL\n",__func__,__LINE__);
        return;
    }

    //����lib���ȡ��Ӧ������
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_BASIC,
	};

    obj = cJSON_CreateObject();
    if(NULL == obj)
    {
        printf("[%s][%d]create obj fail\n",__func__,__LINE__);
        return ;
    }
	get_info.wp = NULL;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);   //���ù���get�������л�ȡ��Ϣ

	/********************2.4G ���߻�������*************************/
    //ssid
    cJSON_AddStringToObject(send_root,"ssid",cjson_get_value(obj,LIB_WIFI_AP_SSID,""));
    //wifi security
    security_mode_format(cjson_get_value(obj,LIB_WIFI_AP_SEC_MODE,"NONE"),sec_mode);
    cJSON_AddStringToObject(send_root,"sec",sec_mode);
    //wifi password
    cJSON_AddStringToObject(send_root,"passwd",cjson_get_value(obj,LIB_WIFI_AP_PWD,""));
    //WiFi hide
    ssid_hide_en = strncmp(cjson_get_value(obj,LIB_WIFI_HIDE_SSID,"false"),"true",strlen(true)) ? 0 : 1;
    cJSON_AddNumberToObject(send_root,"ssid_hide",ssid_hide_en);
	//wifi enable
    wifi_enable = strncmp(cjson_get_value(obj,LIB_WIFI_EN,"true"),"true",strlen("true")) ? 0 : 1;  
    cJSON_AddNumberToObject(send_root,"enable",wifi_enable);

	//surppot security mode
    cJSON_AddItemToObject(send_root,"sec_option",sec_option = cJSON_CreateArray());
    for(i = 0; i < 4; i++)
    {
        cJSON_AddItemToArray(sec_option,item = cJSON_CreateObject());
        cJSON_AddStringToObject(item,"sec",sec[i]);
    }
    //��ȡ·�����ɼ��ܷ�ʽ������
    n_sec_option = cJSON_GetArraySize(sec_option);
    cJSON_AddNumberToObject(send_root,"n_sec_option",n_sec_option);
    
	/********************5G ���߻�������*************************/
	//ssid
    cJSON_AddStringToObject(send_root,"ssid_5g",cjson_get_value(obj,LIB_WIFI_AP_SSID_5G,""));
    //wifi security
    memset(sec_mode,0x0,sizeof(sec_mode));
    security_mode_format(cjson_get_value(obj,LIB_WIFI_AP_SEC_MODE_5G,"NONE"),sec_mode);
    cJSON_AddStringToObject(send_root,"sec_5g",sec_mode);
    //wifi password
    cJSON_AddStringToObject(send_root,"passwd_5g",cjson_get_value(obj,LIB_WIFI_AP_PWD_5G,""));
    //WiFi hide
    ssid_hide_en = strncmp(cjson_get_value(obj,LIB_WIFI_HIDE_SSID_5G,"false"),"true",strlen(true)) ? 0 : 1;
    cJSON_AddNumberToObject(send_root,"ssid_hide_5g",ssid_hide_en);
	//wifi enable
    wifi_enable = strncmp(cjson_get_value(obj,LIB_WIFI_EN_5G,"true"),"true",strlen("true")) ? 0 : 1;  
    cJSON_AddNumberToObject(send_root,"enable_5g",wifi_enable);

	//surppot security mode
    cJSON_AddItemToObject(send_root,"sec_option_5g",sec_option = cJSON_CreateArray());
    for(i = 0; i < 4; i++)
    {
        cJSON_AddItemToArray(sec_option,item = cJSON_CreateObject());
        cJSON_AddStringToObject(item,"sec_5g",sec[i]);
    }
    //��ȡ·�����ɼ��ܷ�ʽ������
    n_sec_option = cJSON_GetArraySize(sec_option);
    cJSON_AddNumberToObject(send_root,"n_sec_option_5g",n_sec_option);
	
    cJSON_Delete(obj);

    return ;
}


/****************************************************************************
������  :app_set_wifi_basic_info
����    :����app����wifi������Ϣ
����    :
    send_root:���ں����ش����ݣ�
        
    info:��
1.��    ��   : 2016��12��23��
  ��    ��   : liusongming
  �޸�����   : �½�����
2.��	��	 : 2017��11��17��
  ��	��	 : luorilin
  �޸�����	 : �������5G������Ϣ

****************************************************************************/  
void app_set_wifi_basic_info(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
    cJSON *obj = NULL;
    char *ssid_hide = NULL;
	char *wifi_enable = NULL;
	char sec_mode[PI_BUFLEN_32] = {0};
    PI8 err_code[PI_BUFLEN_32] = {0};
    CGI_MSG_MODULE remov_msg;
	
    if(NULL == send_root)
    {
        printf("func:%s line:%d send_root is NULL\n",__func__,__LINE__);
        return;
    }
    obj = cJSON_CreateObject();
    if(NULL == obj)
    {
    	printf("func:%s line:%d create obj fail\n",__func__,__LINE__);
        return;
    }

	/*��װ����,��ʽ��ҳ���·������ݸ�ʽ*/ 
	/*************************2.4G��������**************************/
	//wifi hide app:1/0 web:true/false
    ssid_hide = cjson_get_number(send_root,"ssid_hide",0) ? "true" : "false"; 
    cJSON_AddStringToObject(obj,LIB_WIFI_HIDE_SSID,ssid_hide);	
    //wifi enable  app:1/0 web:true/false
    wifi_enable = cjson_get_number(send_root,"enable",1) ? "true" : "false";
    cJSON_AddStringToObject(obj,LIB_WIFI_EN,wifi_enable);
    cJSON_AddStringToObject(obj,LIB_WIFI_AP_SSID,cjson_get_value(send_root,"ssid",""));    
    cJSON_AddStringToObject(obj,LIB_WIFI_AP_PWD,cjson_get_value(send_root,"passwd",""));    

	/*app������ ���ܷ�ʽ�·��ĸ�ʽ��wpa-psk��wpa2-psk��wpa&wpa2��NONE*/
	/*ע�⣬��������Ҫ��webҳ���·��ĸ�ʽ����һ�£�WPA/AES��WPA2/AES��WPAWPA2/AES��NONE*/
	security_mode_format(cjson_get_value(send_root,"sec","NONE"),sec_mode);
    cJSON_AddStringToObject(obj,LIB_WIFI_AP_SEC_MODE,sec_mode);  
    /*************************5G��������**************************/
	//wifi hide app:1/0 web:true/false
    ssid_hide = cjson_get_number(send_root,"ssid_hide_5g",0) ? "true" : "false"; 
    cJSON_AddStringToObject(obj,LIB_WIFI_HIDE_SSID_5G,ssid_hide);	
    //wifi enable  app:1/0 web:true/false
    wifi_enable = cjson_get_number(send_root,"enable_5g",1) ? "true" : "false";
    cJSON_AddStringToObject(obj,LIB_WIFI_EN_5G,wifi_enable);
    cJSON_AddStringToObject(obj,LIB_WIFI_AP_SSID_5G,cjson_get_value(send_root,"ssid_5g",""));    
    cJSON_AddStringToObject(obj,LIB_WIFI_AP_PWD_5G,cjson_get_value(send_root,"passwd_5g",""));    

	/*app������ ���ܷ�ʽ�·��ĸ�ʽ��WPA-PSK��WPA2-PSK��WPA/WPA2-PSK��NONE*/
	/*ע�⣬��������Ҫ��webҳ���·��ĸ�ʽ����һ�£�WPA/AES��WPA2/AES��WPAWPA2/AES��NONE*/
	memset(sec_mode,0x0,sizeof(sec_mode));
	security_mode_format(cjson_get_value(send_root,"sec_5g","NONE"),sec_mode);
    cJSON_AddStringToObject(obj,LIB_WIFI_AP_SEC_MODE_5G,sec_mode); 

	//����lib�⺯���������߻���������Ч
    CGI_LIB_INFO set_info;
    PIU8 modules[] =
	{
        MODULE_SET_WIFI_BASIC,
	};
    set_info.wp = NULL;
	set_info.root = obj;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, info);

    //�жϵ���lib�⺯���Ƿ��г���
    if(strcmp(err_code,"0"))
    {
        *result_code = 1;
    }
    
    memset(remov_msg.msg,0x0,MAX_MODULE_MSG_MAX_LEN);
	remov_msg.id = RC_WIFI_MODULE;
	remove_msg_to_list(msg,remov_msg);

    cJSON_Delete(obj);

    return ;
}

/****************************************************************************
������  :app_set_wifi_process
����    :����app����wifi������Ϣ��ʹ������Ч�ĺ���
����    :
    send_root:
        
    info:��
1.��    ��   : 2016��12��23��
  ��    ��   : liusongming
  �޸�����   : �½�����
2.��	��	 : 2017��11��17��
  ��	��	 : luorilin
  �޸�����	 : ͨ��wifi���÷���Ϣ��������Ϣ

****************************************************************************/  
void app_set_wifi_process(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	send_wifi_msg_handle(TENDA_WLAN24_AP_IFNAME,app_msg_op_code_2g,msg);
	app_msg_op_code_2g = COMMON_MSG_MAX;
	send_wifi_msg_handle(TENDA_WLAN5_AP_IFNAME,app_msg_op_code_5g,msg);
	app_msg_op_code_5g = COMMON_MSG_MAX;
}

void app_get_wifi_push_info(cJSON *recv_root,cJSON *send_root, void *info)
{
	CGI_LIB_INFO get_info;
	cJSON *wifi_info = NULL;
	wifi_info =cJSON_CreateObject();

	if(NULL == wifi_info)
	{
		printf("func:%s line:%d wifi_info created error!\n",__func__,__LINE__);
        return;
	}
	
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_BASIC,
		MODULE_GET_WIFI_EN,	
	};
	
	get_info.wp = NULL;
	get_info.root = wifi_info;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);

    int wifi_enable = 1;
	
	cJSON_AddNumberToObject(send_root,"wifi_type",0);    
	cJSON_AddStringToObject(send_root,"ssid",cjson_get_value(wifi_info,LIB_WIFI_AP_SSID,""));
    wifi_enable = strncmp(cjson_get_value(wifi_info,LIB_WIFI_EN,""),"true",strlen("true")) ? 0 : 1;
	cJSON_AddNumberToObject(send_root,"wifi_enable",wifi_enable);

	cJSON_AddNumberToObject(send_root,"wifi_type_5g",1);  
	cJSON_AddStringToObject(send_root,"ssid_5g",cjson_get_value(wifi_info,LIB_WIFI_AP_SSID_5G,""));
    wifi_enable = strncmp(cjson_get_value(wifi_info,LIB_WIFI_EN_5G,""),"true",strlen("true")) ? 0 : 1;
	cJSON_AddNumberToObject(send_root,"wifi_enable_5g",wifi_enable);

	cJSON_Delete(wifi_info);
	return;
}

#ifdef __CONFIG_GUEST__
/****************************************************************************
������  :app_get_wifi_guest_info
����    :����app��ȡguest wifi������Ϣ
����    :
    send_root:
        
    info:��
��    ��   : 2017��11��15��
��    ��   : luorilin
�޸�����   : �½�����
****************************************************************************/ 
void app_get_wifi_guest_info(cJSON *recv_root,cJSON *send_root, void *info)
{
	char *wifi_guest_enable = NULL;
	char security_mode[PI_BUFLEN_32] = {0};
	CGI_LIB_INFO get_info;
	cJSON *wifi_guest_info = NULL;
	wifi_guest_info = cJSON_CreateObject();

	if(NULL == wifi_guest_info)
	{
		printf("[%s][%d] wifi_guest_info created error!\n",__func__,__LINE__);
		return;
	}
	
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_GUEST_INFO,
	};
	
	get_info.wp = NULL;
	get_info.root = wifi_guest_info;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);

	//2.4G wifi guest
	wifi_guest_enable = strcmp(cjson_get_value(wifi_guest_info, LIB_WIFI_GUEST_EN,"false"),"true") ? "0" : "1";
	cJSON_AddNumberToObject(send_root,"wifi_guest_en",atoi(wifi_guest_enable));
	cJSON_AddStringToObject(send_root,"wifi_guest_ssid",cjson_get_value(wifi_guest_info,LIB_WIFI_GUEST_SSID,"Tenda_VIP"));
    cJSON_AddStringToObject(send_root,"wifi_guest_passwd",cjson_get_value(wifi_guest_info,LIB_WIFI_GUEST_PWD,""));	
	/*security mode*/
	security_mode_format(cjson_get_value(wifi_guest_info,LIB_WIFI_GUEST_SEC_MODE,"NONE"),security_mode);
	cJSON_AddStringToObject(send_root, "wifi_guest_sec", security_mode);

	//5G wifi guest
	wifi_guest_enable = strcmp(cjson_get_value(wifi_guest_info, LIB_WIFI_GUEST_EN_5G,"false"),"true") ? "0" : "1";
	cJSON_AddNumberToObject(send_root,"wifi_guest_en_5g",atoi(wifi_guest_enable));
	cJSON_AddStringToObject(send_root,"wifi_guest_ssid_5g",cjson_get_value(wifi_guest_info,LIB_WIFI_GUEST_SSID_5G,"Tenda_VIP_5G"));
    cJSON_AddStringToObject(send_root,"wifi_guest_passwd_5g",cjson_get_value(wifi_guest_info,LIB_WIFI_GUEST_PWD_5G,""));	
	/*security mode*/
	memset(security_mode,0x0,sizeof(security_mode));
	security_mode_format(cjson_get_value(wifi_guest_info,LIB_WIFI_GUEST_SEC_MODE_5G,"NONE"),security_mode);
	cJSON_AddStringToObject(send_root, "wifi_guest_sec_5g", security_mode);
	
	/*wifi guest effect_time �aapp�� effect_time ���ӱ�ʾ*/
	cJSON_AddNumberToObject(send_root,"wifi_guest_timeout",atoi(cjson_get_value(wifi_guest_info,LIB_WIFI_GUEST_EFFECT_TIME,"8"))*60);
	/*wifi guest down speed limit /Mbps*/
	cJSON_AddNumberToObject(send_root,"wifi_guest_rate",atoi(cjson_get_value(wifi_guest_info,LIB_WIFI_GUEST_SHARE_SPEED,"0")));

	cJSON_Delete(wifi_guest_info);
	
	return;
}

/*****************************************************************************
 �� �� ��  : app_set_wifi_guest_info_process
 ��������  : ʹ���÷ÿ�������Ч
 �������  : cJSON *send_root     
             CGI_MSG_MODULE *msg  
             int *result_code     
             void *info           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��15��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
void app_set_wifi_guest_info_process(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	send_wifi_msg_handle(TENDA_WLAN24_GUEST_IFNAME,app_msg_op_code_2g,msg);
	app_msg_op_code_2g = COMMON_MSG_MAX;
	send_wifi_msg_handle(TENDA_WLAN5_GUEST_IFNAME,app_msg_op_code_5g,msg);
	app_msg_op_code_5g = COMMON_MSG_MAX;
}

/****************************************************************************
������  :app_set_wifi_guest_info
����    :����app����guest wifi������Ϣ��ʹ������Ч�ĺ���
����    :
    send_root:
        
    info:��
��    ��   : 2017��11��15��
��    ��   : luorilin
�޸�����   : �½�����
****************************************************************************/ 
void app_set_wifi_guest_info(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	char security_mode[PI_BUFLEN_32] = {0}; 
	char effect_time_str[PI_BUFLEN_32] = {0};
	char down_speed_limit_str[PI_BUFLEN_32] = {0};
	PI8 err_code[PI_BUFLEN_16] = {0};
	cJSON *obj = NULL;
	CGI_MSG_MODULE remov_msg;
	
	if(NULL == send_root)
    {
        printf("[%s][%d] send_root is NULL\n",__func__,__LINE__);
        return;
    }
    obj = cJSON_CreateObject();
    if(NULL == obj)
    {
    	printf("[%s][%d] create obj fail\n",__func__,__LINE__);
        return;
    }

	//2.4G wifi guest	
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_EN,cjson_get_number(send_root,"wifi_guest_en",0) ? "true" : "false");
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_SSID,cjson_get_value(send_root,"wifi_guest_ssid","Tenda_VIP"));
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_PWD,cjson_get_value(send_root,"wifi_guest_passwd",""));
	/*security mode*/
	get_wlan_security_mode(cjson_get_value(send_root,"wifi_guest_sec","NONE"),security_mode);
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_SEC_MODE, security_mode);

	//5G wifi guest	
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_EN_5G,cjson_get_number(send_root,"wifi_guest_en_5g",0) ? "true" : "false");
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_SSID_5G,cjson_get_value(send_root,"wifi_guest_ssid_5g","Tenda_VIP_5G"));
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_PWD_5G,cjson_get_value(send_root,"wifi_guest_passwd_5g",""));
	/*security mode*/
	get_wlan_security_mode(cjson_get_value(send_root,"wifi_guest_sec_5g","NONE"),security_mode);
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_SEC_MODE_5G, security_mode);
	/*effect  time app�·���ʱ����� ��̨ͳһ��Сʱת��Ϊ��*/
	snprintf(effect_time_str,sizeof(effect_time_str),"%d",cjson_get_number(send_root,"wifi_guest_timeout",8)/60);   
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_EFFECT_TIME, effect_time_str);
	/*down_speed limit ��256KBs �ϴ���������*/
	snprintf(down_speed_limit_str,sizeof(down_speed_limit_str),"%d",cjson_get_number(send_root,"wifi_guest_rate",0));  
	cJSON_AddStringToObject(obj,LIB_WIFI_GUEST_SHARE_SPEED, down_speed_limit_str);
	
	//����lib�⺯���������߻���������Ч
    CGI_LIB_INFO set_info;
    PIU8 modules[] =
	{
        MODULE_SET_WIFI_GUEST_INFO,
	};
    set_info.wp = NULL;
	set_info.root = obj;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, info);

    //�жϵ���lib�⺯���Ƿ��г���
    if(strcmp(err_code,"0"))
    {
        *result_code = 1;
    }
    
    memset(remov_msg.msg,0x0,MAX_MODULE_MSG_MAX_LEN);
	remov_msg.id = RC_WIFI_MODULE;
	remove_msg_to_list(msg,remov_msg);

    cJSON_Delete(obj);

	return;
}
#endif 
