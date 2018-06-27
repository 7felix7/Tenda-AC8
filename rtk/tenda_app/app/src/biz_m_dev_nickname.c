#include "cgi_module_interface.h"
#include "biz_m_dev_nickname.h"

/*************************************************************************
  ����: ��ȡ�豸����(��ȡ��ע)
  ����: info �������ݵĽṹ
  	param ָ��Ҫ��ȡ��Щ�豸�ı���
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
  
  cj_query����ʽΪ:{"module":["APP_GET_DEV_REMARK"],"extern_data":{"mac_list":
                   ["44:37:E6:9E:7E:A2", "90:67:1c:f8:d8:4a", "C8:3A:35:48:B2:02"],"cnt":3}}
                
  cj_get����ʽΪ:{"name_pair_s":[{"id":"44:37:E6:9E:7E:A2","nickname":"aaffhdwferhwer"}, 
                    {"id":"90:67:1c:f8:d8:4a","nickname":"55681223"}],"cnt":2}
 ************************************************************************/
int biz_m_dev_nickname_info_get(const api_cmd_t *cmd,
 										 dev_name_t *param,
										 nickname_info_t *info,
										 void *privdata)
{
 	int ret = 0,i = 0,cnt = 0;
    char *id = NULL,*nickname = NULL;
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    cJSON *extern_data = NULL;
    cJSON *name_pair_s = NULL;
    cJSON *mac_list = NULL;
    cJSON *pairitem = NULL;

    if ((NULL == param) || (NULL == info)) 
	{
        printf("param NULL or privdata NULL\n");
 		return 1;
	}
    
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
    
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_DEV_REMARK)); //��ȡGET_DEV_REMARKģ�������
    //��Ҫ��ӻ�ȡ��������ģ��������������
    //cJSON_AddItemToArray(module,cJSON_CreateString(MODULE_NAME)); 

    cJSON_AddItemToObject(cj_query, "extern_data", extern_data = cJSON_CreateObject());//getʱ������extern_data,�����ʽΪ:�ؼ���:ֵ
    cJSON_AddItemToObject(extern_data,"mac_list",mac_list = cJSON_CreateArray());    //���mac_list���󣬴�ŵ���mac��ַ
    for(i = 0; i < param->cnt; i++)
    {
        cJSON_AddItemToArray(mac_list,cJSON_CreateString(param->ids[i].val));
    }
    cJSON_AddNumberToObject(extern_data,"cnt",param->cnt); //���mac��ַ����

   
	ret = app_get_module_param(cj_query,cj_get);	//���ù���get������ȡ����
   
    
    cnt = cjson_get_number(cj_get,"cnt",0);		//��ȡ��������
    info->cnt = cnt;

    name_pair_s = cJSON_GetObjectItem(cj_get,"name_pair_s");  //name_pair_sȡ����Ϊ���飬��ߴ�ŵ��Ǳ�����
    if(NULL == name_pair_s)
    {
        cJSON_Delete(cj_query);
        cJSON_Delete(cj_get);
        return 1;
    }
    for(i = 0; i < cnt; i++)
    {
        pairitem = cJSON_GetArrayItem(name_pair_s,i);
        if(NULL == pairitem)
        {
            cJSON_Delete(cj_query);
            cJSON_Delete(cj_get);
            return 1;
        }
        id = cjson_get_value(pairitem,"id","");		//��ȡmac��ַ
        nickname = cjson_get_value(pairitem,"nickname","");	//��ȡ����
        if((NULL != id) && (NULL != nickname))
        {
            strcpy(info->name_pairs[i].id.val,id);      //���mac��ַ
            strcpy(info->name_pairs[i].nickname,nickname);  //������
        }
    }
    
    cJSON_Delete(cj_query);
    cJSON_Delete(cj_get);

	return 0;
}

/*************************************************************************
  ����: �����豸����(�޸ı�ע)
  ����: set_info ָ��Ҫ������Щ�豸�ı���
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
  cj_query��ʽ:{"module":["APP_SET_DEV_REMARK"]}
  cj_get��ʽ:{"name_pairs":[{"id":"44:37:e6:9e:7e:a2","nickname":"1111111111"}, 
             {"id":"90:67:1c:f8:d8:4a","nickname":"2222222222222222"}, 
             {"id":"C8:3A:35:48:B2:02","nickname":"3333333333333333333"}],"cnt":3}
 ************************************************************************/
int biz_m_dev_nickname_info_set(const api_cmd_t *cmd, 
									     nickname_info_t *set_info,
									     void *privdata)
{      
    int ret = 0,i = 0;
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    cJSON *cj_set = NULL;
    cJSON *name_pairs = NULL;
    cJSON *obj = NULL;

    if(NULL == set_info) 
    {
        printf("nickname set info is error\n");
		return 1;
	}
    
    cj_query = cJSON_CreateObject();
    cj_set = cJSON_CreateObject();
    cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
    cJSON_AddItemToArray(module,cJSON_CreateString(SET_DEV_REMARK)); //�������Ҫ�������ģ�飬�մ˷�ʽ��module�����

    //cj_set���ڴ�����õ�����
    cJSON_AddItemToObject(cj_set,"name_pairs",name_pairs = cJSON_CreateArray()); //���name_pairs���󣬴�ű�����
    for(i = 0; i < set_info->cnt; i++)
    {
        cJSON_AddItemToArray(name_pairs,obj = cJSON_CreateObject());
        if(NULL == obj)
        {
            printf("func:%s line:%d is error\n",__func__,__LINE__);
			cJSON_Delete(cj_query);
			cJSON_Delete(cj_set);
            return 1;
        }
        cJSON_AddStringToObject(obj,"id",set_info->name_pairs[i].id.val);   //�����������id
        cJSON_AddStringToObject(obj,"nickname",set_info->name_pairs[i].nickname);  //����������ӱ���
    }
    cJSON_AddNumberToObject(cj_set,"cnt",set_info->cnt); //����������ӱ�������

    app_set_module_param(cj_query,cj_set);	//���ù���set������������
   
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);   
	return 0;
}

