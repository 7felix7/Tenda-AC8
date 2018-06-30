#include <stdio.h>
#include <stdlib.h>
#include <bcmnvram.h>

#include "cgi_handle_module.h"
#include "cgi_lib_config.h"
#include "autoconf.h"
#include "time.h"
#include "systools.h"

/*****************************************************************************
 �� �� ��  : app_get_sys_advance_info
 ��������  : app��ȡϵͳ�߼���Ϣ
 			 ע:����cpuƵ�����δʵ��
 �������  : cJSON *recv_root  
             cJSON *send_root  
             void *info        
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��28��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
void app_get_sys_advance_info(cJSON *recv_root,cJSON *send_root, void *info)
{
	CGI_LIB_INFO get_info;
	struct mallinfo mem_info;
	mem_info = mallinfo();

	PIU8 modules[] = 
	{
		
	};

	get_info.wp = NULL;
	get_info.root = recv_root;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);

	cJSON_AddNumberToObject(send_root, "systime",(uint32_t)time(NULL));//unixʱ���
	cJSON_AddNumberToObject(send_root, "uptime",cyg_current_time()/100);//����ʱ��
	cJSON_AddNumberToObject(send_root, "cpu_max_freq",300);//��ʱδʵ��
	cJSON_AddNumberToObject(send_root, "cpu_curr_idle",30);//��ʱδʵ��
	cJSON_AddNumberToObject(send_root, "mem_total",mem_info.arena);//���ڴ�
	cJSON_AddNumberToObject(send_root, "mem_used",mem_info.arena - mem_info.fordblks);//�����ڴ�
}


/*****************************************************************************
 �� �� ��  : app_get_sys_basic_info
 ��������  : app��ȡϵͳ������Ϣ
 			 ע:���б���ʱ���δʵ�֣�û���õ�
 �������  : cJSON *recv_root  
             cJSON *send_root  
             void *info        
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��28��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
#define 	DUT_MODE_AP   			   	1
#define 	DUT_MODE_ROUTER			2
void app_get_sys_basic_info(cJSON *recv_root,cJSON *send_root, void *info)
{
	CGI_LIB_INFO get_info;
	int wl_mode = 0;
	int dev_mode = DUT_MODE_ROUTER;
	char hard_var[8] = {0};
	char product_name[8] = {0};
	PIU8 modules[] = 
	{
		MODULE_GET_WIFI_RELAY_TYPE,
		MODULE_GET_WAN_MAC,
		MODULE_GET_FIREWARE,
	};

	get_info.wp = NULL;
	get_info.root = recv_root;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);

	//��ȡϵͳMAC��ַ
	cJSON *obj = cJSON_GetObjectItem(get_info.root, LIB_WAN_MAC_CUR_WAN);
	cJSON_AddStringToObject(send_root, "mac",obj->valuestring);

	//��ȡϵͳ����ģʽ
	obj = cJSON_GetObjectItem(get_info.root, LIB_WIFI_RELAY_TYPE);
	if(strcmp(obj->valuestring,"wisp") == 0)
		wl_mode = 3;
	else if(strcmp(obj->valuestring,"client+ap") == 0)
		wl_mode = 2;
	else if(strcmp(obj->valuestring,"ap") == 0){
		/*���ݳ����ṩ����Ϣ�����ΪAP ģʽ����dev_mode = 1,����Ϊ2*/
		dev_mode = DUT_MODE_AP;
		wl_mode = 0;
	}
	else{
		wl_mode = 0;
	}
		
	cJSON_AddNumberToObject(send_root, "wl_mode",wl_mode);
	cJSON_AddNumberToObject(send_root, "dev_mode",dev_mode);
	//��ȡuc_sn��
	cJSON_AddStringToObject(send_root, "sn_number", nvram_safe_get("uc_serialnum"));
	
	//��ȡӲ���汾��
	sscanf(nvram_safe_get("BOARD_NAME"),"%*[^_]_%s",hard_var);
	cJSON_AddStringToObject(send_root,"hard_ver" ,hard_var);

	//��ȡ����汾��
	obj = cJSON_GetObjectItem(get_info.root, LIB_SOFT_VER);
	cJSON_AddStringToObject(send_root, "soft_ver",obj->valuestring);

	//��ȡ�������ʱ��(δʵ�֣�û���ô�)
	cJSON_AddNumberToObject(send_root, "release_date",1478742773);

	//��ȡ��˾����
	cJSON_AddStringToObject(send_root, "product_firm",COMPANY_NAME);
	
	//��ȡ��Ʒ�ͺ�
	//sscanf(nvram_safe_get("BOARD_NAME"),"%*[^_]_%s",product_name);
	sscanf(nvram_safe_get("BOARD_NAME"),"%[^_]",product_name);
	cJSON_AddStringToObject(send_root, "product_model",product_name);

	//��ȡ�Ƿ���й���������ҳ��״̬
	if(strcmp(nvram_safe_get("restore_quick_set"),"1") == 0)
	{
		cJSON_AddNumberToObject(send_root,"guid_done",0);
	}
		
	else
	{
		cJSON_AddNumberToObject(send_root,"guid_done",1);
	}
	//WAN��֧�ֵ�������ʽ
	cJSON_AddStringToObject(send_root,"supt_wan_type","static,dhcp,adsl");

}


/*****************************************************************************
 �� �� ��  : app_set_reboot
 ��������  : app��������
 �������  : cJSON *send_root     
             CGI_MSG_MODULE *msg  
             int *result_code     
             void *info           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��28��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
void app_set_reboot(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	CGI_LIB_INFO set_info;
    PI8 err_code[PI_BUFLEN_32] = {0};

	PIU8 modules[] = 
	{
		MODULE_SET_OPERATE,	
	};
	
	set_info.wp = NULL;
	set_info.root = send_root;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);

}


/*****************************************************************************
 �� �� ��  : app_set_reset
 ��������  : app���ûָ���������
 �������  : cJSON *send_root     
             CGI_MSG_MODULE *msg  
             int *result_code     
             void *info           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��28��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
void app_set_reset(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	CGI_LIB_INFO set_info;
    PI8 err_code[PI_BUFLEN_32] = {0};
	
	PIU8 modules[] = 
	{
		MODULE_SET_OPERATE,	
	};
	
	set_info.wp = NULL;
	set_info.root = send_root;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);

}

/*****************************************************************************
 �� �� ��  : app_set_wizard_succeed
 ��������  : app����������ɺ���
 �������  : cJSON *send_root     
             CGI_MSG_MODULE *msg  
             int *result_code     
             void *info           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��1��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void app_set_wizard_succeed(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	CGI_LIB_INFO set_info;
    PI8 err_code[PI_BUFLEN_32] = {0};
	
	PIU8 modules[] = 
	{
		MODULE_SET_WIZARD_SUCCEED,	
		
	};
	
	set_info.wp = NULL;
	set_info.root = send_root;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);	//����lib�⺯����������
	
	return ;
}
	



