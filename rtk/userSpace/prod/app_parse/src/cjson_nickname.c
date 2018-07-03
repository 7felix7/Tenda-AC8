/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cjson_nickname.c
  �� �� ��   : ����
  ��    ��   : liusongming
  ��������   : 2016��12��16��
  ����޸�   :
  ��������   :

  ��������   : app��ȡ�������豸����

  �޸���ʷ   :
  1.��    ��   : 2016��12��16��
    ��    ��   : liusongming
    �޸�����   : �����ļ�

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "cjson_handle.h"
#include "cgi_lib_config.h"

/****************************************************************************
������  :app_get_dev_remark
����    :����app��ȡ�豸����
����    :
    recv_root:�������������ģ����module������:extern_data
        ��ʽ:{"module":["APP_GET_DEV_REMARK"],"extern_data":{"mac_list":["44:37:E6:9E:7E:A2", 
            "90:67:1c:f8:d8:4a", "C8:3A:35:48:B2:02"],"cnt":3}}
        
    send_root:���ں����ش����ݣ�
        ��ʽ:{"name_pair_s":[{"id":"44:37:E6:9E:7E:A2","nickname":"aaffhdwferhwer"}, 
             {"id":"90:67:1c:f8:d8:4a","nickname":"55681223"}]}
        
    info:��
��    ��   : 2016��12��23��
��    ��   : liusongming
�޸�����   : �½�����
****************************************************************************/

void app_get_dev_remark(cJSON *recv_root,cJSON *send_root, void *info)
{
  int count = 0,size = 0,i = 0;
  int number = 0;
  cJSON *extern_data = NULL;
  cJSON *mac_list = NULL;
  cJSON *mac_item = NULL;
  cJSON *name_pair_s = NULL;
  cJSON *obj = NULL;
  char *m_remark = NULL;

  if(NULL == recv_root)
  {
      printf("func:%s line:%d recv_root is null\n",__func__,__LINE__);
      return;
  }

  extern_data = cJSON_GetObjectItem(recv_root,"extern_data");
  if(NULL == extern_data)
  {
     printf("func:%s line:%d get extern_data fail\n",__func__,__LINE__);
     return ;
  }

  count = cjson_get_number(extern_data,"cnt",0);        //ȡ��app������mac��ַ����
  
  mac_list = cJSON_GetObjectItem(extern_data,"mac_list");//��ȡapp��������mac��ַ�б�
  if(NULL == mac_list)
  {
    printf("func:%s line:%d get mac_list fail\n",__func__,__LINE__);
    return;
  }

 

  cJSON_AddItemToObject(send_root,"name_pair_s",name_pair_s = cJSON_CreateArray()); //name_pair_s���ڱ�����
  for(i =0; i < count; i++)
  {
    mac_item = cJSON_GetArrayItem(mac_list,i);
    if(mac_item->valuestring != NULL)
    {
        m_remark = get_remark(mac_item->valuestring);
        if(NULL == m_remark)
        {
            break;
        }
        cJSON_AddItemToArray(name_pair_s,obj = cJSON_CreateObject());
        cJSON_AddStringToObject(obj,"id",mac_item->valuestring); //���mac��ַ
        cJSON_AddStringToObject(obj,"nickname",m_remark);	//��ӱ���
        number++;
    }
  }

  cJSON_AddNumberToObject(send_root,"cnt",number);  //����豸������

    
  return ;
}

/****************************************************************************
������  :app_set_dev_remark
����    :����app�����豸�ı���
����    :
    send_root:
        ��ʽ:{"name_pairs":[{"id":"44:37:e6:9e:7e:a2","nickname":"1111111111"}, 
             {"id":"90:67:1c:f8:d8:4a","nickname":"2222222222222222"}, {"id":"C8:3A:35:48:B2:02","nickname":"3333333333333333333"}],"cnt":3}
        
          msg:���ģ����Ϣ
     err_code:��Ŵ������
        
         info:��
��    ��   : 2016��12��24��
��    ��   : liusongming
�޸�����   : �½�����
****************************************************************************/

void app_set_dev_remark(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
    cJSON *name_pairs = NULL,*obj = NULL;
    char *mac_string = NULL,*dev_remark = NULL;
    int count = 0,size = 0,i = 0;

    if(NULL == send_root)
    {
        printf("func:%s line:%d send_root is null\n",__func__,__LINE__);
        return;
    }

    name_pairs = cJSON_GetObjectItem(send_root,"name_pairs");  //��ȡapp�������ı�����
    if(NULL == name_pairs)
    {
        printf("func:%s line:%d get name_pairs fail\n",__func__,__LINE__);
        return ;
    }
    size = cJSON_GetArraySize(name_pairs);   
    count = cjson_get_number(send_root,"cnt",0);    //��ȡapp�����ı�������
   
    for(i = 0; i < size; i++)
    {
        obj = cJSON_GetArrayItem(name_pairs,i);
        if(NULL == obj)
        {
            printf("func:%s line:%d get Arrayitem fail\n",__func__,__LINE__);
            return ;
        }
        mac_string = cjson_get_value(obj,"id","");	//��ȡmac��ַ
        dev_remark = cjson_get_value(obj,"nickname",""); //��ȡ����
        if((NULL == mac_string) || (NULL == dev_remark))
        {
            printf("func:%s line:%d\n",__func__,__LINE__);
            return;
        }
        
        add_remark(mac_string, dev_remark);     //���ñ���
      
    }
   
    return ;
}






