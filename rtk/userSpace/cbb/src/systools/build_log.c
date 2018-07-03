/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : build_log.c
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2017��5��19��
  ����޸�   :
  ��������   :

  ��������   : ʵ��build������û��ṩ������صĻ�����Ϣ

  �޸���ʷ   :
  1.��    ��   : 2017��5��19��
    ��    ��   : 
    �޸�����   : �����ļ�

******************************************************************************/

#include "build_log.h"
#include "version.h"


extern void print_build_tag(void);

int show_build_info(int argc, char *argv[])
{
	print_build_tag();
	printf("hard version:%s\n",W311R_ECOS_HV);
	printf("soft version:%s\n",W311R_ECOS_SV);
	printf("language:%s\n",__CONFIG_WEB_VERSION__);
	printf("svn version:%s\n",SVN_VERSION);

	return 0;
}




