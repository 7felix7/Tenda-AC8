#include "flash_cgi.h"
#include "cJSON.h"


#define NEED_MALLOC_MEM  136 *1024 //��Ҫ����400KB�ڴ���������

extern int net_mbuf_free_mem(void);
extern int get_kern_free_mem(void); /*��ȡ�ں�ʣ���ڴ�*/
extern int get_kern_mbuf_free_mem();
extern int get_kern_misc_free_mem();

extern char *cjson_get_value(cJSON *root, char *name, char *defaultGetValue);
extern int cjson_get_number(cJSON *root, char *name, int defaultGetValue);


/*****************************************************************************
 �� �� ��  : app_get_upgrade_path
 ��������  : ��ȡ����������������ĵ�ַ
 �������  : cJSON *recv_root	
			 cJSON *send_root 
			 void *info

 �� �� ֵ  : 
 
 �޸���ʷ	   :
  1.��	  ��   : 2016��12��13��
	��	  ��   : luorilin
	�޸�����   : �����ɺ���
*****************************************************************************/
void  app_get_upgrade_path(cJSON *recv_root,cJSON *send_root,void *info)
{
	char path[] = "/tmp/fv9.bin"; /* ��������ĵ�ַ  */	
	int len = strlen(path);

	cJSON_AddNumberToObject(send_root,"uprade_image_path_len",len);
	cJSON_AddStringToObject(send_root,"uprade_image_path",path);
	
	return;
}

/*****************************************************************************
 �� �� ��  : app_get_upgrade_memory_state
 ��������  : ��������ǰ����ڴ��Ƿ����
 �������  : cJSON *recv_root	
			 cJSON *send_root 
			 void *info
 �������  : ��
 �� �� ֵ  : 
 �޸���ʷ	   :
  1.��	  ��   : 2016��12��13��
	��	  ��   : luorilin
	�޸�����   : �����ɺ���
*****************************************************************************/
void  app_get_upgrade_memory_state (cJSON *recv_root,cJSON *send_root,void *info)
{
	int free_malloc, free_clone_mem, mbuf_mem, misc_mem;
    	int enough_memory = 0;
	int image_size = 0;
	cJSON *extern_data = NULL;
	
	if(NULL == recv_root)
	{
		printf("[%s][%d]recv_root is null!\n",__func__,__LINE__);
		return;
	}
	extern_data = cJSON_GetObjectItem(recv_root,"extern_data");
	if(NULL == extern_data)
	{
		printf("[%s][%d]extern_data is null!\n",__func__,__LINE__);
		return;
	}
	image_size = cjson_get_number(extern_data,"img_size",0);
	
	free_malloc = net_mbuf_free_mem();  //����������Ҫ�Ķ�����ڴ�
	free_clone_mem = get_kern_free_mem(); //�ں� �ڴ�ʣ���С�����ڴ�ž����ļ�
	mbuf_mem = get_kern_mbuf_free_mem();
	misc_mem = get_kern_misc_free_mem(); //��ȡmisc�ڴ�
		
       if((free_malloc >=NEED_MALLOC_MEM) && 
		(  ((free_malloc - NEED_MALLOC_MEM) + (free_clone_mem - 5*1024) + (mbuf_mem - 5*1024) + (misc_mem - 50*1024) )  >= image_size))
	{
		enough_memory = 1;
	}
	else 
	{
		enough_memory = 0;		
	}
	
	cJSON_AddNumberToObject(send_root,"memory_state",enough_memory);		
	return ;
}

