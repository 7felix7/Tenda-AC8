#include "phupdate.h"
#include "log.h"
#include <signal.h>     /* for singal handle */
#ifndef WIN32
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <netdb.h>
#include <unistd.h>     /* for close() */
#endif

#define ORAY_CLIENTINFO	0x20063150
#define ORAY_CHALLENGEKEY 0x10101010

PHGlobal global;
static void my_handleSIG (int sig)
{
	if (sig == SIGINT)
	{
#ifndef WIN32
		remove("/var/run/phddns.pid");
#endif
		LOG ("signal = SIGINT\n");
		phddns_stop(&global);
		exit(0);
	}
	if (sig == SIGTERM)
	{
#ifndef WIN32
		remove("/var/run/phddns.pid");
#endif
		LOG ("signal = SIGTERM\n");
		phddns_stop(&global);
	}
	signal (sig, my_handleSIG);
}

//״̬���»ص�
static void myOnStatusChanged(int status, long data)
{
	LOG("myOnStatusChanged %s", convert_status_code(status));
	if (status == okKeepAliveRecved)
	{
		LOG(", IP: %d", data);
	}
	if (status == okDomainsRegistered)
	{
		LOG(", UserType: %d", data);
	}
	LOG("\n");
}

//����ע��ص�
static void myOnDomainRegistered(char *domain)
{
	LOG("myOnDomainRegistered %s\n", domain);
}

//�û���ϢXML���ݻص�
static void myOnUserInfo(char *userInfo, int len)
{
	LOG("myOnUserInfo %s\n", userInfo);
}

//������ϢXML���ݻص�
static void myOnAccountDomainInfo(char *domainInfo, int len)
{
	LOG("myOnAccountDomainInfo %s\n", domainInfo);
}

int register_oray(int argc, char *argv[])
{
	void (*ohandler) (int);
#ifdef WIN32
	WORD VersionRequested;		// passed to WSAStartup
	WSADATA  WsaData;			// receives data from WSAStartup
	int error;

	VersionRequested = MAKEWORD(2, 0);

	//start Winsock 2
	error = WSAStartup(VersionRequested, &WsaData); 
//	log_open("c:\\phclientlog.log", 1);	//empty file will cause we printf to stdout
#else

	if (argc < 4)
	{
		LOG("This is a phddns sample by Oray\r\n\trun with argument: phddns phddns60.oray.net <account> <password>\r\n");
		return -1;
	}

	
//	log_open("/var/log/phddns.log", 1);	//empty file will cause we printf to stdout
//	create_pidfile();
#endif


	ohandler = signal (SIGINT, my_handleSIG);
	if (ohandler != SIG_DFL) {
		LOG ("previous signal handler for SIGINT is not a default handler\n");
		signal (SIGINT, ohandler);
	}

	init_global(&global);

	global.cbOnStatusChanged = myOnStatusChanged;
	global.cbOnDomainRegistered = myOnDomainRegistered;
	global.cbOnUserInfo = myOnUserInfo;
	global.cbOnAccountDomainInfo = myOnAccountDomainInfo;

	set_default_callback(&global);
	//ע�⣡����������������������������������������������
	//��������ֵ�û�����ʱ���ԣ�Oray������ʱɾ�����޸ģ�����ʽ����ǰ����д����ʵ�ʷ���ֵ
	global.clientinfo = ORAY_CLIENTINFO; 		//������д�ղŵڶ��������ֵ
	global.challengekey = ORAY_CHALLENGEKEY;	//������дǶ��ʽ��֤��
	//ע�⣡����������������������������������������������

//	strcpy(global.szHost, 
//		argv[1]);			//�����õ��ķ�������ַ
	strcpy(global.szUserID, 
		argv[3]);							//Oray�˺�
	strcpy(global.szUserPWD, 
		argv[5]);							//��Ӧ������
	
//	for (;;)
#ifdef HAVE_SYSTEM_REINIT
	extern int rtldd_quitting;
	extern cyg_sem_t oray_sem_load;
#endif
	while(1
#ifdef HAVE_SYSTEM_REINIT
		&& (rtldd_quitting == 0)
#endif
		)
	{
		int next = phddns_step(&global);
#ifdef HAVE_SYSTEM_REINIT
		cyg_semaphore_timed_wait(&oray_sem_load,cyg_current_time()+next*100);
#else
		sleep(next);
#endif
	}
	phddns_stop(&global);
	return 0;
}
