/***********************************************************
Copyright (C), 2013, Tenda Tech. Co., Ltd.
FileName: cgi_web.c
Description: ����������绷���߳� 
Author: hqw;
Version : 1.0
Date: 2013-11-05
Function List:   
 

History:
<author>   <time>   <version >   <desc>
hqw        2013-11-05   1.0       ��̨���ݰ���ǰ̨Ҫ���ʽ����ת��
************************************************************/
#include <stdio.h>
#include <string.h>
extern void string_cat(char *str , char *str1 , char *str2);
extern void json_cat(char *str, char*json);

/************************************************************
Function:	 string_cat               
Description: 	 ��str2�ӵ�str1���沢�Ұ���webҪ��ĸ�ʽ����ͷ����β��

Input:          �ַ���ָ��str1���ַ���ָ��str2    ,type��־�Ƿ�Ҫ��','                           

Output: 	      ��ʽΪstr+"str1":"str2"

Return:      

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/

void string_cat(char *str , char *str1 , char *str2)
{ 
	//"str1"
	strncat(str,"\"",strlen("\""));
	strncat(str,str1,strlen(str1));
	strncat(str,"\"",strlen("\""));
	
	strncat(str,":",strlen(":"));
	//"str2"
	strncat(str,"\"",strlen("\""));
	strncat(str,str2,strlen(str2));
	strncat(str,"\"",strlen("\""));
	return ;
}

void json_cat(char *str, char*json)
{
	char *char_p = NULL, *char_q = NULL;

	if(NULL==str || NULL==json)
	{
		diag_printf("[%s][%d] invalid para!!!\n", __FUNCTION__, __LINE__);
		return;
	}

	char_p = strchr(json, '{');

	if(NULL == char_p)
	{
		diag_printf("[%s][%d] invalid json!!!\n", __FUNCTION__, __LINE__);
		return;
	}
	
	char_p++;
	
	char_q = strrchr(char_p, '}');
	*char_q = '\0';
	
	strcat(str, char_p);

	return;
}

