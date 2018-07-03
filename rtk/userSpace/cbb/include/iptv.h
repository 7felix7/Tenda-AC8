#ifndef __IPTV_H__
#define __IPTV_H__


//vlan��������
#define VLAN_AREA_DEFAULT	"0"		//Ĭ��
#define VLAN_AREA_SHANGHAI	"1"		//�Ϻ�����
#define VLAN_AREA_CUSTOM	"2"		//�Զ���



typedef enum{
	DEFAULT = 0,
	SHANGHAI,
	CUSTOM,
}VLAN_AREA;

typedef struct iptv_info{
	int iptv_enable;
	int vlan_area;		//vlan���� 0:Ĭ��  1:�Ϻ� 2:�Զ���
	int vlan_id;
	int vlan_sel;
}IPTV_INFO_STRUCT,*P_IPTV_INFO_STRUCT;


#endif
