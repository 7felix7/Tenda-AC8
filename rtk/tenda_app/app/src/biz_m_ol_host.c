#include "cgi_module_interface.h"

#include "biz_m_ol_host.h"


/*************************************************************************
  ����: ��ȡ�����豸��Ϣ
  ����: ol_host_common_ack_t  �����豸��Ϣ�Ľṹ
  ����ֵ: 0-�ɹ���1-ʧ�ܣ�80-������������
  �Ƿ���Ҫ�û�ʵ��: ��
  ע��: �ú���ģ���и����Ĳ���ȫ����Ҫʹ��
 ************************************************************************/
int biz_m_ol_host_info_get_cb(const api_cmd_t *cmd, 
								  ol_host_common_ack_t ** hosts_info,
								  void *privdata)
{
	int ret = 0;
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_OL_HOST_INFO));

	ret = app_get_module_param(cj_query,cj_get);

    cJSON *hostInfo_obj = NULL;
	cJSON *child_obj = NULL;
	int count_num = 0;
	int i = 0;

	hostInfo_obj = cJSON_GetObjectItem(cj_get,"hostInfo");

	if(hostInfo_obj)
	{
		count_num = cJSON_GetArraySize(hostInfo_obj);  /*�����豸�������豸������*/
	}
	else
	{
		count_num = 0;
	}
	/*�����ڴ�*/
	int len = sizeof(ol_host_common_ack_t) +(count_num)* sizeof(ol_hosts_dev_t);
	*hosts_info =(ol_host_common_ack_t *) malloc(len);
	if(NULL == (*hosts_info))
	{
		cJSON_Delete(cj_get);
		cJSON_Delete(cj_query);
		return 1; 
	}
	memset((char *)(*hosts_info), 0, len);
	ol_hosts_dev_t *hosts = (*hosts_info)->info.hosts;

	/*�������߻������б��������*/
	for(i = 0; i < count_num; i++)
    {
        child_obj = cJSON_GetArrayItem(hostInfo_obj,i);
	
		strcpy(hosts[i].host_name, cjson_get_value(child_obj,"hostname","")); 
	    strcpy(hosts[i].host_alias, cjson_get_value(child_obj,"remark",""));
	    SET_OL_HOSTS_DEV_ALIAS(&hosts[i]); 	                 //����host_alias��־λ
	    strcpy(hosts[i].ip,cjson_get_value(child_obj,"iP",""));
	    strcpy(hosts[i].mac, cjson_get_value(child_obj,"mac",""));

		hosts[i].access_type = cjson_get_number(child_obj,"ConnectType",1);  /*/�������� 1:����*/
		SET_OL_HOSTS_DEV_TYPE(&hosts[i]);
		
        hosts[i].trust = cjson_get_number(child_obj,"is_mac_trust",0); /*1��ʾ�������б� 0���������б�*/
        SET_OL_HOSTS_DEV_TRUST(&hosts[i]); 	    //����trust��־λ          
        
		hosts[i].online_time = cjson_get_number(child_obj,"ConnetTime",0);
		SET_OL_HOSTS_ONLINE_TIME(&hosts[i]);        /*����online_time��־λ*/

		/*�ϴ�������������*/		
		hosts[i].up_limit = cjson_get_number(child_obj,"up_limit",-1);
		hosts[i].down_limit = cjson_get_number(child_obj,"down_limit",-1);
		hosts[i].bw_limited = cjson_get_number(child_obj,"bw_limited",0);
		SET_OL_HOSTS_UP_LIMIT(&hosts[i]);	        /* ����up_limit��־λ */
	    SET_OL_HOSTS_DOWN_LIMIT(&hosts[i]);	        /* ����down_limit��־λ */
        SET_OL_HOSTS_BW_LIMIT(&hosts[i]);	        /* ����bw_limited��־λ */

		/*��ǰ����*/		
		hosts[i].curr_up_rate =  cjson_get_number(child_obj,"curr_up_rate",0.0);
		hosts[i].curr_down_rate =  cjson_get_number(child_obj,"curr_down_rate",0.0);

		/*�Ƿ��ں�������*/
	    hosts[i].mac_blocked =  cjson_get_number(child_obj,"mac_blocked",0); 
	    SET_OL_HOSTS_MAC_BLOCK(&hosts[i]); 	    // ����mac_blocked��־λ 

	    hosts[i].host_type = cjson_get_number(child_obj,"hostTpye",0);  //�豸����
        SET_OL_HOSTS_DEV_TYPE(&hosts[i]);	        //����host_type ��־λ 

        //��ȡ�豸�ĳ���
        strcpy(hosts[i].manufactory_desc,cjson_get_value(child_obj,"manufactory",""));
        SET_OL_HOSTS_DEV_MANUFACTORY_DESC(&hosts[i]);

        hosts[i].online = cjson_get_number(child_obj,"online",0); /*/�豸���� Ĭ��ֵΪ0*/
        SET_OL_HOSTS_DEV_OL(&hosts[i]);		   //����online��־λ 

	}	
	
	(*hosts_info)->info.hosts_count = count_num;
	(*hosts_info)->info.mem_len = ((*hosts_info)->info.hosts_count) * sizeof(ol_hosts_dev_t);	

	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	
	return ret;
}

