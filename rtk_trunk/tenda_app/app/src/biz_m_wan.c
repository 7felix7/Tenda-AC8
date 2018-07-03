#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/endian.h>

#include "cgi_module_interface.h"
#include "biz_m_wan.h"
/*************************************************************************
  ����: ʵ�ֻ�ȡ�������õĺ���
  ����: ��Ҫ�������ݵ�wan_basic_info_t�ṹ
  ����ֵ: 2-��������δ֪��0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wan_basic_get_cb(const api_cmd_t *cmd, 
								 wan_basic_info_t *basic_info,
								 void *privdata)
{
    cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    cJSON *wan = NULL;
    cJSON *wanitem = NULL;
    char *adsl_name = NULL,*adsl_pwd = NULL;
    int ret = 0,i = 0;
    int n_wan = 0,wan_count = 0;
    unsigned int ip_addr = 0,netmask = 0,gateway = 0,primary_dns = 0,backup_dns = 0,interfacenum = 0;
    char *wanIp = NULL,*wanMask = NULL,*wanGateway = NULL,*wanDns1 = NULL,*wanDns2 = NULL;
    char *wanConnectTime = NULL;
    int type = 0;
    int sta = 0,err = 0;
    int conn_time = 0;
    int wanConnectStatus = 0;
    
    cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
  
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_WAN_BASIC)); //��ȡGET_DEV_REMARKģ�������
  	
	ret = app_get_module_param(cj_query,cj_get);

    //��ȡwan�ڸ���
    n_wan = cjson_get_number(cj_get,"n_wan",0);
    basic_info->n_wan = n_wan;

    //��ȡ���ڴ�Ÿ���wan��Ϣ������
    wan = cJSON_GetObjectItem(cj_get,"wan");
    if(NULL == wan)
    {
        printf("get wan error\n");
        cJSON_Delete(cj_query);
        cJSON_Delete(cj_get);
        return 1;
    }

    wan_count = cJSON_GetArraySize(wan);
    if(n_wan != wan_count)      //wan�ڸ�����ƥ��
    {
        printf("wan number not match\n");
        cJSON_Delete(cj_query);
        cJSON_Delete(cj_get);
        return 1;
    }
    basic_info->n_wan = wan_count;      //��ȡwan�ڵ��ܸ���
    //��ȡ����wan�ڵ���ϸ��Ϣ

    for(i = 0; i < wan_count; i++)
    {
        wanitem = cJSON_GetArrayItem(wan,i);
        if(NULL == wanitem)
        {
            printf("get wan detail error\n");
            cJSON_Delete(cj_query);
            cJSON_Delete(cj_get);
            return 1;
        }
        //interfacenum��ʾ�ڼ���wan��
        interfacenum = cjson_get_number(wanitem,"interfacenum",0);    
        basic_info->wan[i].interfacenum= interfacenum;
        
        //��ȡ��������
        type = cjson_get_number(wanitem,"type",0);      //Ĭ�Ϸ���δ֪���ͣ���̨�Ѿ���0����δ֪���ͣ�appά����Ա��ע��
        if(0 == type)
        {
            printf("wan_type:unknow\n");
            cJSON_Delete(cj_query);
            cJSON_Delete(cj_get);
		    return 2;   //��������δ֪ 
        }
        basic_info->wan[i].type = type;

        //��ȡ�˺�����
        adsl_name = cjson_get_value(wanitem,"adsl_name","");
        adsl_pwd = cjson_get_value(wanitem,"adsl_pwd","");

        if((NULL != adsl_name) || (NULL != adsl_pwd))
        {
            strcpy(basic_info->wan[i].adsl_info.adsl_name,adsl_name); // ����˺� 
	        strcpy(basic_info->wan[i].adsl_info.adsl_pwd,adsl_pwd);    //�������
	        SET_WAN_BASIC_WAN_ADSL_INFO(&basic_info->wan[i]);    //���ñ�־λ
        }
        
        //��ȡwan�ڻ�����Ϣ
        wanIp = cjson_get_value(wanitem,"ip_addr","0.0.0.0");
        wanMask = cjson_get_value(wanitem,"netmask","0.0.0.0");
        wanGateway = cjson_get_value(wanitem,"gateway","0.0.0.0");
        wanDns1 = cjson_get_value(wanitem,"primary_dns","");
        wanDns2 = cjson_get_value(wanitem,"backup_dns","");
        wanConnectTime = cjson_get_value(wanitem,"conn_time","0");

        inet_aton(wanIp,&ip_addr);
        inet_aton(wanMask,&netmask);
        inet_aton(wanGateway,&gateway);
        inet_aton(wanDns1,&primary_dns);
        inet_aton(wanDns2,&backup_dns);
        inet_aton(wanConnectTime,&conn_time);
        conn_time = atoi(wanConnectTime);
        
        basic_info->wan[i].netaddr_info.ip_addr  = ntohl(ip_addr); /*��� 32λ�޷�����ip��ַ "114.114.114.114" */
  		basic_info->wan[i].netaddr_info.netmask = ntohl(netmask); /*��� 32λ�޷������������� "255.255.255.0" */
  		basic_info->wan[i].netaddr_info.gateway = ntohl(gateway); /* ���32λ�޷��������� "192.168.10.1" */
  		basic_info->wan[i].netaddr_info.primary_dns = ntohl(primary_dns); /*��� ��ѡdns1 */
  		basic_info->wan[i].netaddr_info.backup_dns= ntohl(backup_dns); /* �������dns1 */
  		basic_info->wan[i].netaddr_info.conn_time = conn_time; /* ��������ʱ���־λ*/
        SET_WAN_CONN_TIME(&basic_info->wan[i].netaddr_info); /* ���ñ�־λ */
		SET_WAN_BASIC_WAN_NET_ADDR_INFO(&basic_info->wan[i]); /* ���ñ�־λ */


        //��ȡ������״̬
        sta = cjson_get_number(wanitem,"sta",0);    //0-δ�����ߣ�1-δ������2-�����У�3-������ 
        basic_info->wan[i].wan_status.sta = sta;
        SET_WAN_BASIC_WAN_STA(&basic_info->wan[i]); /* ���ñ�־λ */
        
        //ֻ��δ����������²��ܹ�ȥ��ȡ�����룬���������쳣,appά����Ա��ע��
        if(1 == sta)
        {
            err = cjson_get_number(wanitem,"err",0);    //0-������ 566-�޷����ӵ�����  ����549-�û������������548-Զ�˷���������Ӧ
            basic_info->wan[i].wan_status.err = err;
        }
        
    }

    cJSON_Delete(cj_query);
    cJSON_Delete(cj_get);

	return 0;
}

/*************************************************************************
  ����: �жϲ����������
  ����:   ��
  ����ֵ: ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
static void end_the_wizard(void)
{
	printf("end the wizard successfully !\n");
	
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;

	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WIZARD_SUCCEED));
	app_set_module_param(cj_query,cj_set);
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
}

/*************************************************************************
  ����: �����������ͺ�ʹ������Ч�ĺ���
  ����: ��ʹ��
  ����ֵ: ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
#ifdef CONFIG_APP_COSTDOWN
void biz_m_wan_basic_set_process(void)
#else
static void biz_m_wan_basic_set_process(
											const api_cmd_t *cmd,
											int data_len,
											int ret)
#endif
{
	int biz_ret = 100;
	cJSON *cj_set	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WAN_BASIC_PROCESS));
	//���ù���set����
	app_set_module_param(cj_query,cj_set);
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);

    //���������
    end_the_wizard();
}


/*************************************************************************
  ����: ʵ�������������͵ĺ���
  ����: ��wan_basic_info_t�����ݱ��浽·����
  ����ֵ: 2-��������δ֪��10-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wan_basic_set_cb(const api_cmd_t *cmd, 
								 	 wan_basic_info_t *basic_info,
								 	 void *privdata)
{
    int ret = 0,i = 0;
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    cJSON *cj_set = NULL;
    cJSON *wan = NULL;
    cJSON *wanitem = NULL;
    char wan_ip[16] = {0}, wan_gateway[16] = {0};
    char wan_msk[16] = {0},dns1[16] = {0}, dns2[16] = {0};
    struct in_addr ina;

    if(NULL == basic_info)
    {
        printf("func:%s line:%d basic_info is NULL\n",__func__,__LINE__);
        return 1;
    }

    cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WAN_BASIC));
    
    cJSON_AddNumberToObject(cj_set,"n_wan",basic_info->n_wan);  //���wan�ڵĸ���

    cJSON_AddItemToObject(cj_set,"wan",wan = cJSON_CreateArray()); //������ڴ��wan����ϸ���ݵ�����

    //�������ݵ�ת���Լ���װ��IP��ַ����������δ��͵�cjson������ᵼ�½ضϣ����Զ���IP��ַ������
    //biz�����ת�����ַ�������ʽ
     if(basic_info->wan[0].type == STATIC) 
     {
        if (HAS_WAN_BASIC_WAN_NET_ADDR_INFO(&basic_info->wan[0]))
        {
            cJSON_AddItemToArray(wan,wanitem = cJSON_CreateObject());
            if(NULL == wanitem)
            {
                cJSON_Delete(cj_query);
                cJSON_Delete(cj_get);
                return 1;
            }
           	//��IPת�����ַ�������ʽ
            ina.s_addr = ntohl(basic_info->wan[0].netaddr_info.ip_addr);
            sprintf(wan_ip, "%s", inet_ntoa(ina));

			//ת����������
            ina.s_addr = ntohl(basic_info->wan[0].netaddr_info.netmask);
            sprintf(wan_msk, "%s", inet_ntoa(ina));

			//ת������
            ina.s_addr = ntohl(basic_info->wan[0].netaddr_info.gateway);
            sprintf(wan_gateway, "%s", inet_ntoa(ina));

			//ת��DNS
            ina.s_addr = ntohl(basic_info->wan[0].netaddr_info.primary_dns);
            sprintf(dns1, "%s", inet_ntoa(ina));

            ina.s_addr = ntohl(basic_info->wan[0].netaddr_info.backup_dns);
            sprintf(dns2, "%s", inet_ntoa(ina));

            //��app��������ӵ�cjson����
            cJSON_AddNumberToObject(wanitem, "type", basic_info->wan[0].type);
            cJSON_AddStringToObject(wanitem,"ip_addr",wan_ip);
            cJSON_AddStringToObject(wanitem,"netmask",wan_msk);
            cJSON_AddStringToObject(wanitem,"gateway",wan_gateway);
            cJSON_AddStringToObject(wanitem,"primary_dns",dns1);
            cJSON_AddStringToObject(wanitem,"backup_dns",dns2);
        }
     }
     else if(basic_info->wan[0].type == DYNAMIC)	//��̬����
     {
        cJSON_AddItemToArray(wan,wanitem = cJSON_CreateObject());
        if(NULL == wanitem)
        {
            cJSON_Delete(cj_query);
            cJSON_Delete(cj_get);
            return 1;
        }
        cJSON_AddNumberToObject(wanitem, "type", basic_info->wan[0].type);
     }
     else if(basic_info->wan[0].type == ADSL)	//pppoe����
     {
        if (HAS_WAN_BASIC_WAN_ADSL_INFO(&basic_info->wan[0]))
        {
            cJSON_AddItemToArray(wan,wanitem = cJSON_CreateObject());
            if(NULL == wanitem)
            {
                cJSON_Delete(cj_query);
                cJSON_Delete(cj_get);
                return 1;
            }
			//���˻�����������ӵ�������
            cJSON_AddNumberToObject(wanitem, "type", basic_info->wan[0].type);
            cJSON_AddStringToObject(wanitem, "adsl_name", basic_info->wan[0].adsl_info.adsl_name);
            cJSON_AddStringToObject(wanitem, "adsl_pwd", basic_info->wan[0].adsl_info.adsl_pwd);
        }
     }
     else	//δ֪����
     {
        cJSON_Delete(cj_query);
        cJSON_Delete(cj_get);
        return 2;
     }
    
   ret = app_set_module_param(cj_query,cj_set);

   //�ײ�ִ�г���
   if(ret != 0)
   {
       cJSON_Delete(cj_query);
       cJSON_Delete(cj_get);
       return 1; 
   }
   #ifndef CONFIG_APP_COSTDOWN
	//2.ʹ������Ч���ô��뱣���������޸�
	wan_common_ack_t wan_ack_t = {0};
	wan_ack_t.err_code = 0;
	uc_api_lib_cmd_resp_notify(cmd,0, sizeof(wan_common_ack_t), &wan_ack_t, biz_m_wan_basic_set_process);
	#endif

    cJSON_Delete(cj_query);
    cJSON_Delete(cj_get);
  #ifdef CONFIG_APP_COSTDOWN
  	return 0;
  #else  
	return COMPLETE_RET; /* 10 */
  #endif
}

/*************************************************************************
  ����: ��ȡWAN��������
  ����: wan_rate_info_t ��Ҫ�������ݵĽṹ
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wan_rate_get_cb(const api_cmd_t *cmd,
 									wan_rate_info_t *rate_info,
 									void *privdata)
{
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    int interfacenum = 0,cur_uplink = 0,cur_downlink = 0,max_uplink = 0,max_downlink =0;


    cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
    
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_WAN_RATE)); //��ȡGET_DEV_REMARKģ�������

    //���ù���get����
	app_get_module_param(cj_query,cj_get);
 	//ȡ������
    interfacenum = cjson_get_number(cj_get,"interfacenum",0);
    cur_uplink = cjson_get_number(cj_get,"cur_uplink",0);
    cur_downlink = cjson_get_number(cj_get,"cur_downlink",0);
    max_uplink = cjson_get_number(cj_get,"max_uplink",0);
    max_downlink = cjson_get_number(cj_get,"max_downlink",0);

    rate_info->n_wan = 1;                   //wan�ڵ�����
    rate_info->wan[0].cur_uplink = cur_uplink;   /* WAN�ڵ�ǰ�������� byte/s */
	rate_info->wan[0].cur_downlink = cur_downlink; /* WAN�ڵ�ǰ�������� byte/s */
	rate_info->wan[0].interfacenum = interfacenum;  /* ��һ��WAN�� */
	rate_info->wan[0].max_uplink = max_uplink;    /* ����0���� */
	rate_info->wan[0].max_downlink = max_downlink;  /* ����0���� */

    cJSON_Delete(cj_query);
    cJSON_Delete(cj_get);

	return 0;
}

/*************************************************************************
  ����: ���������򵼵�ʱ�򣬻�ȡwan������״̬
  ����: type_info ��Ҫ�������ݵĽṹ
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wan_detect_type_cb (
						const api_cmd_t *cmd, 
						wan_detecttype_info_t *type_info,
						void *privdata)
{
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    int detect_type = 0;

    cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
    
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_WAN_DETECT)); 

    //���ù���������ȡ����
	app_get_module_param(cj_query,cj_get);
    
 	detect_type = cjson_get_number(cj_get,"detect_type",0);
    type_info->wan[0].detect_type = detect_type;
    type_info->n_wan = 1;	//WAN�ڸ���
    
    cJSON_Delete(cj_query);
    cJSON_Delete(cj_get);
 	return 0;
}


