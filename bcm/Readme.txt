include projects: 
N3,W316R
W303R C0
W308R C0


0,���û�н�����뻷����Ҫ���汾�������ϵ�InputĿ¼����bcm_ecos_gnutools.tgz
Ȼ�󣬽�ѹ������/optĿ¼��
tar -xzvf bcm_ecos_gnutools.tgz -C /opt

1,��src\ecos\router\config\xxxx.config����src\ecos\routerĿ¼�µ�.config

2,��src\ecos\router\config\cfg�ļ��н������OEM�����ļ�

3,��src\ecos\routerĿ¼��
make menuconfig����һ��menuconfig�˳�ʱ�����棬
���ڶ���menuconfigʱ�������������ѡ��
�����N3����Device Drivers Options��ѡ��Wireless feature variantΪap
�����W303R����Device Drivers Options��ѡ��Wireless feature variantΪapsta

4,make oldconfig;make clean;make all
===============================================================================
2011/08/16 Modify:
include projects: 
N3,W316R->8MB_AP
W303R C0->16MB
W308R C0->16MB


0,���û�н�����뻷����Ҫ���汾�������ϵ�InputĿ¼����bcm_ecos_gnutools.tgz
Ȼ�󣬽�ѹ������/optĿ¼��
tar -xzvf bcm_ecos_gnutools.tgz -C /opt

1,��src\ecos\router\config\cfg�ļ��н������OEM�����ļ�

2,��src\ecos\router\config\xxxx.config����src\ecos\routerĿ¼�µ�.config

3,must do this if you have just finished step 2.
��src\ecos\routerĿ¼��
ѡ�� "8MB_AP" or "8MB_APSTA" or "16MB"
make PROFILE=8MB_AP oldconfig

4,make clean;make all

===============================================================================
2011/11/10 Modify:
����A5 V1->8MB_AP
�л���SDK:ecos-router-5.110.27.21.tgz

2013/5/28 Modify:
����ϵ綪�������⣬����flash˫���ݷ�������Ҫ�޸Ĵ������£�
���������ļ� sys_backupcfg.c sys_backupcfg.h ��nvramĿ¼�£��ڲ���flashʱ������������rc.c������Ӷ�ʱ��������Ƿ���Ҫ����

2013/06/03 �ر�汾:
���⣺���⻷���У�PPPOE����������Ӧά������echo reply����magic number�ֶ��쳣�����������һ�£�������ppp��·���ҹ�˾·���������Ͽ����ӡ���Դ����⣬�������ر�汾
���ֽ������������1�������н�echo����Ϊ����ģʽ����������ά���������μ������ļ�w316r_cn_PASSIVE_ECHO_MODE.config
	           ����2����lcp.c�ļ���lcp_received_echo_reply���� ���жϡ�������ͻ�Ӧ��magic number�Ƿ���ͬ��������ǣ���ֱ��return��------------�ɽ���return���ע����