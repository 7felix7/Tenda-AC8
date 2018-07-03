/*****************************************************************************
Copyright (C), �����ڴ�������а�Ȩ
File name ��tenda_wlan_proc.c
Description : ��������PROC�ļ������ƺͲ鿴tenda����˽����ӵĹ��ܣ��磺band steering
Author ��dengxingde
Version ��V1.0
Date ��2017-5-4
Others ��
History ���� �� �� ʷ �� ¼ �� ��
1) ���ڣ� �޸��ߣ�
  �޸����ݣ�
2��...
*****************************************************************************/

//#include <linux/module.h>
//#include <linux/version.h>
//#include <linux/proc_fs.h>
//#include <linux/time.h>
//#include <linux/seq_file.h>

#include <cyg/io/eth/rltk/819x/wrapper/sys_support.h>
#include <cyg/io/eth/rltk/819x/wrapper/skbuff.h>
#include <cyg/io/eth/rltk/819x/wrapper/timer.h>
#include <cyg/io/eth/rltk/819x/wrapper/wrapper.h>
#include "./8192cd_cfg.h"
#include "./8192cd.h"
#include "./8192cd_hw.h"
#include "./8192cd_headers.h"
#include "./8192cd_debug.h"

#include "tenda_sta_steering.h"
#include "tenda_wlan_proc_ecos.h"


extern struct sta_steer g_steer;

void sta_steer_write_proc(char      input[][MAX_ARGV],unsigned long   input_num,
                           unsigned char   flag,char    *output,unsigned long   out_len)

{
    char tmp[32];
    
    if (strncmp(input[0],"1",1) == 0) {
        g_steer.enable = 1;
    } else if (strncmp(input[0],"0",1) == 0) {
        g_steer.enable = 0;
    } else {
        char cmd[32];
        int val;

        sscanf(input[1],"%s",cmd);
        sscanf(input[2],"%d",&val);
        
        if (strcmp(cmd,"sta_balance_enable") == 0) {
            g_steer.sta_balance_enable = val;
        } else if (strcmp(cmd,"rssi_lmt_2g") == 0) {
            g_steer.rssi_lmt_2g = val;
        } else if (strcmp(cmd,"rssi_lmt_5g") == 0) {
            g_steer.rssi_lmt_5g = val;
        } else if (strcmp(cmd,"auth_reject_limit") == 0) {
            g_steer.auth_reject_limit = val;
        } else if (strcmp(cmd,"auth_status") == 0) {
        	g_steer.auth_status = val;
        } else if (strcmp(cmd,"assoc_status") == 0) {
        	g_steer.assoc_status = val;
        } else if (strcmp(cmd,"debug") == 0) {
        	g_steer.debug = val;
        } 		
    }
    //g_steer.enable = simple_strtoul(tmp, NULL, 0);
	
}


#define MAX_ARGC_ 20
#define MAX_ARGV_ 20 


unsigned int steer_cmd(char		*input, unsigned int in_len, 
                                unsigned char flag, char *output, unsigned long out_len)
{
	char *token;
	unsigned long Argc = 0;
	char		Argv[MAX_ARGC][MAX_ARGV];
    
	do {
		token = strsep(&input, ", ");
		if (token) {
			strcpy(Argv[Argc], token);
			Argc++;
		} else
			break;
	} while (Argc < MAX_ARGC);
    
    sta_steer_write_proc(Argv, Argc, flag, output, out_len);

	return 0;
}

void sta_steer_read_proc()
{
    sta_steer_proc_show();

    return 0;
}




