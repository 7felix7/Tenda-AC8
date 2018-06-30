#include "cgi_handle_module.h"
#include "cgi_lib_config.h"

/*****************************************************************************
 �� �� ��  : app_get_wifi_timer
 ��������  : app��ȡ��ǰwifi��ʱ������
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
void app_get_wifi_timer(cJSON *recv_root,cJSON *send_root, void *info)
{
	CGI_LIB_INFO get_info;
	cJSON *obj_wifi_timer = NULL;
	PIU8 modules[] = 
	{
		MODULE_GET_WIFI_SCHED,
	};

	if(NULL == send_root)
	{
	 	printf("[%s][%d]obj send_root is NULL\n",__func__,__LINE__);
        return ;
	}
	
	obj_wifi_timer = cJSON_CreateObject();
	if(NULL == obj_wifi_timer)
	{
	 	printf("[%s][%d]create obj fail\n",__func__,__LINE__);
        return ;
	}
	
	get_info.wp = NULL;
	get_info.root = obj_wifi_timer;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
	
	char wifi_sched_enable[PI_BUFLEN_16] = {0};
	char wifi_sched_times[PI_BUFLEN_32] = {0};
	char wifi_sched_weeks[PI_BUFLEN_16] = {0};
	char wifi_7day_weeks[PI_BUFLEN_16]={0};
	int start_hour = 0,start_min = 0,end_hour = 0,end_min = 0;
	int i = 0,timer_status = 0,intweeks=0;

	//wifi_sched_enable 1/0 	
	timer_status = strcmp(cjson_get_value(obj_wifi_timer, LIB_WIFI_TIME_EN,"false"),"true") ? 0 : 1; 
	cJSON_AddNumberToObject(send_root, "timer_status",timer_status);

	//time
	strcpy(wifi_sched_times,cjson_get_value(obj_wifi_timer, LIB_WIFI_TIME_CLOSE,"00:00-00:00"));
	sscanf(wifi_sched_times, "%d:%d-%d:%d", &start_hour, &start_min,&end_hour,&end_min); 

	//weeks
	strcpy(wifi_sched_weeks,cjson_get_value(obj_wifi_timer, LIB_WIFI_TIME_DATE,"10101010"));

	/*webҳ���nvram�����ڵĲ����ǰ�λ�����飬��һλ��ʾ�ǲ���ȫѡ�������ڣ�APPֻ��Ҫ����λ*/
	for(i=0;i<8;i++)
	{
		wifi_7day_weeks[i] = wifi_sched_weeks[i+1];
	}

	/*APP��Ҫ��7λ���飬�ֱ��Ǳ�ʾ���գ���һ��������������web��һ�£�web��ʾ������һ������*/
	char value = wifi_7day_weeks[6];

	for(i=6;i>0;i--)
	{	
		wifi_7day_weeks[i] = wifi_7day_weeks[i-1];
	}
	wifi_7day_weeks[0] = value;
	
	/* day��6λ����0λ�ֱ��ʾ���ա�һ����...������λΪ1��ʾ��Ч��0��ʾ����Ч����day = 2����ʾ��������Ч */
	/*��7λchar�����飬ת����һ��int�͵�����(int���ĸ��ֽڣ�2��ʾֻ�����屻ѡ��)������APP����*/
	for(i=0;i<7;i++)
	{
		intweeks <<=1;
		intweeks |=((wifi_7day_weeks[i] - '0')&1);
	}
	
	cJSON_AddNumberToObject(send_root, "timer_date",intweeks);
	cJSON_AddNumberToObject(send_root, "start_time",start_hour*60 + start_min);
	cJSON_AddNumberToObject(send_root, "end_time",end_hour*60 + end_min);

	cJSON_Delete(obj_wifi_timer);
	return;
}



/*****************************************************************************
 �� �� ��  : days2string
 ��������  : ��int�͵��������֣�ת����char�����飨7λ��
 �������  : char day[]  
             int days    
 �������  : ��
 �� �� ֵ  : static
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��29��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
static void days2string(char day[], int days)
{
	int i, len = 0;

	if (NULL == day) {
		return; 
	}
	
	for (i = 6; i >= 0; i --) {
		if (i > 0) {
			len += sprintf(day+len,"%d", (days>>i)&0x01);
		}else{
			len += sprintf(day+len,"%d", (days>>i)&0x01);
			day[len] = '\0';  
		}
	}
}


/*****************************************************************************
 �� �� ��  : app_set_wifi_timer
 ��������  : app����wifi��ʱ�����ú���
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
  2.��	  ��   : 2017��11��17��
    ��	  ��   : luorilin
    �޸�����   : �޸�

*****************************************************************************/
void app_set_wifi_timer(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	char timer_status[4] = {0};
	int start_time = 0, end_time = 0, day = 0,binary_day = 0,day7 = 0,i = 0;
	char days[32] = {0},days8[16] = {0};
	char time_interval[32] = {0};
	cJSON *obj = NULL;
	
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
	
	cJSON_AddStringToObject(obj, LIB_WIFI_TIME_EN,cjson_get_number(send_root,"timer_status",0) ? "true" : "false");

	day7 = cjson_get_number(send_root,"timer_date",62);
	day7 = ((day7 & 0x3f) << 1) | (day7 >> 6); 	
	days2string(days, day7);  //����λ��ʾ������ת�����ַ���"1,1,1,1,1,1,1"

	/*ת����web���ݸ�ʽһ�� ÿ����һ������*/
	if(day7 == 127)
	{
		for(i = 0;i < 8;i++)
		{
			days8[i+1] = days[i];
		}
		days8[0] = '1';
	}
	else
	{
		for(i = 0;i < 8;i++)
		{
			days8[i+1] = days[i];
		}
		days8[0] = '0';
	}
	cJSON_AddStringToObject(obj, LIB_WIFI_TIME_DATE,days8);

	//��ʼʱ�䣬����ʱ���ת�� Ĭ�ϣ�00:00 ~ 07:00 (��һ������)
	start_time = cjson_get_number(send_root,"start_time",0);
	end_time = cjson_get_number(send_root,"end_time",420);

	sprintf(time_interval,"%02d:%02d-%02d:%02d",start_time/60,start_time%60,end_time/60,end_time%60);
	cJSON_AddStringToObject(obj, LIB_WIFI_TIME_CLOSE,time_interval);
	
	CGI_LIB_INFO set_info;
    PI8 err_code[PI_BUFLEN_32] = {0};
	PIU8 modules[] = 
	{
		MODULE_SET_WIFI_SCHED,	
	};
	
	set_info.wp = NULL;
	set_info.root = obj;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);

	//�жϵ���lib�⺯���Ƿ��г���
    if(strcmp(err_code,"0"))
    {
        *result_code = 1;
    }
	
	cJSON_Delete(obj);

	return;
}

#ifdef __CONFIG_LED__
/*****************************************************************************
 �� �� ��  : app_get_led_status
 ��������  : app��ȡLED�ĵ�ǰ����״̬
 �������  : cJSON *recv_root  
             cJSON *send_root  
             void *info        
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��20��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/

void app_get_led_status(cJSON *recv_root,cJSON *send_root, void *info)
{
	CGI_LIB_INFO get_info;
	cJSON *led_sta = NULL;
	PIU8 modules[] = 
	{
		MODULE_GET_LED_INFO_APP,
	};
	get_info.wp = NULL;
	get_info.root = recv_root;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
	led_sta = cJSON_GetObjectItem(get_info.root,LIB_LED_STATUS);
	if(NULL == led_sta)
	{
		return ;
	}
	if(0 == strcmp(led_sta->valuestring,"1"))
	{
		cJSON_AddNumberToObject(send_root,"led_status",0);
	}
	else	//LED����
	{
		cJSON_AddNumberToObject(send_root,"led_status",1);
	}
	return ;
}
/*****************************************************************************
 �� �� ��  : app_set_led_status
 ��������  : app����LED����״̬
 �������  : cJSON *send_root     
             CGI_MSG_MODULE *msg  
             int *result_code     
             void *info           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��20��
    ��    ��   : ������
    �޸�����   : �����ɺ���
*****************************************************************************/
void app_set_led_status(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	int status = 1;
	cJSON *obj = NULL;
	CGI_LIB_INFO set_info;
    PI8 err_code[PI_BUFLEN_32] = {0};
	obj = cJSON_CreateObject();
	if(NULL == obj)
	{
		printf("func:%s line:%d create obj fail\n",__func__,__LINE__);
		return;
	}
	status = cjson_get_number(send_root,"led_status",0);
	if(1 == status)
	{
		cJSON_AddStringToObject(obj,LIB_LED_STATUS,"0");
	}
	else
	{
		cJSON_AddStringToObject(obj,LIB_LED_STATUS,"1");
	}
	cJSON_AddStringToObject(obj,LIB_LED_CLOSE_TIME,"");
	PIU8 modules[] = 
	{
		MODULE_SET_LED_INFO,	
	};
	set_info.wp = NULL;
	set_info.root = obj;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);
    if(strcmp(err_code,"0"))
    {
        *result_code = 1;
    }
	cJSON_Delete(obj);
	return;
}
#endif
