<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>WAN | WAN Settings</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<SCRIPT language=JavaScript>
def_WANT="<%aspTendaGetStatus("wan","connecttype");%>";
def_PUN="<%aspTendaGetStatus("ppoe","userid");%>";
def_PPW="<%aspTendaGetStatus("ppoe","pwd");%>";
def_PIDL="<%aspTendaGetStatus("ppoe","idletime");%>";
def_PCM="<%aspTendaGetStatus("ppoe","conmode");%>";
def_MTU="<%aspTendaGetStatus("ppoe","mtu");%>";
def_SVC="<%aspTendaGetStatus("ppoe","sev");%>";
def_AC="<%aspTendaGetStatus("ppoe","ac");%>";
def_WANIP="<%aspTendaGetStatus("wan","wanip");%>";
def_WANMSK="<%aspTendaGetStatus("wan","wanmask");%>";
def_WANGW="<%aspTendaGetStatus("wan","staticgateway");%>";
def_DNS1="<%aspTendaGetStatus("wan","dns1");%>";
def_DNS2="<%aspTendaGetStatus("wan","dns2");%>";
def_l2tpSIP="<%aspTendaGetStatus("wan","l2tpIP");%>";
def_l2tpPUN="<%aspTendaGetStatus("wan","l2tpPUN");%>";
def_l2tpPPW="<%aspTendaGetStatus("wan","l2tpPPW");%>";
def_l2tpMTU="<%aspTendaGetStatus("wan","l2tpMTU");%>";
def_l2tpAdrMode="<%aspTendaGetStatus("wan","l2tpAdrMode");%>";
def_l2tpIP="<%aspTendaGetStatus("wan","l2tpWANIP");%>";
def_l2tpMSK="<%aspTendaGetStatus("wan","l2tpWANMSK");%>";
def_l2tpWGW="<%aspTendaGetStatus("wan","l2tpWANGW");%>";
def_pptpSIP="<%aspTendaGetStatus("wan","pptpIP");%>";
def_pptpPUN="<%aspTendaGetStatus("wan","pptpPUN");%>";
def_pptpPPW="<%aspTendaGetStatus("wan","pptpPPW");%>";
def_pptpMTU="<%aspTendaGetStatus("wan","pptpMTU");%>";
def_pptpAdrMode="<%aspTendaGetStatus("wan","pptpAdrMode");%>";
def_pptpIP="<%aspTendaGetStatus("wan","pptpWANIP");%>";
def_pptpMSK="<%aspTendaGetStatus("wan","pptpWANMSK");%>";
def_pptpWGW="<%aspTendaGetStatus("wan","pptpWANGW");%>";
def_pptpMPPE="<%aspTendaGetStatus("wan","pptpMPPE");%>";
def_dynamicMTU="<%aspTendaGetStatus("wan","dynamicMTU");%>";
def_staticMTU="<%aspTendaGetStatus("wan","staticMTU");%>";
//huangxiaoli add for 8021x
def_X1UN ="<%aspTendaGetStatus("wan","x1name");%>";
def_X1PW ="<%aspTendaGetStatus("wan","x1pwd");%>";
def_X1mode ="<%aspTendaGetStatus("wan","x1mode");%>";
def_X1mtu ="<%aspTendaGetStatus("wan","x1mtu");%>";
//end add
def_WANT = parseInt(def_WANT) -1;
addCfg("WANT",34,def_WANT);
addCfg("PUN", 50, def_PUN);
addCfg("PPW", 54, def_PPW );
addCfg("PIDL", 53, def_PIDL);
addCfg("PCM", 55, def_PCM );
addCfg("MTU", 56, def_MTU );
addCfg("SVC", 57, def_SVC);
addCfg("AC", 58, def_AC);
addCfg("l2tpIP",41,def_l2tpSIP);
addCfg("l2tpPUN",42,def_l2tpPUN);
addCfg("l2tpPPW",43,def_l2tpPPW);
addCfg("l2tpMTU",44,def_l2tpMTU);
addCfg("l2tpAdrMode",45,def_l2tpAdrMode);
addCfg("l2tpWANIP",46,def_l2tpIP);
addCfg("l2tpWANMSK",47,def_l2tpMSK);
addCfg("l2tpWANGW",48,def_l2tpWGW);
addCfg("pptpIP",49,def_pptpSIP);
addCfg("pptpPUN",50,def_pptpPUN);
addCfg("pptpPPW",51,def_pptpPPW);
addCfg("pptpMTU",52,def_pptpMTU);
addCfg("pptpAdrMode",53,def_pptpAdrMode);
addCfg("pptpWANIP",54,def_pptpIP);
addCfg("pptpWANMSK",55,def_pptpMSK);
addCfg("pptpWANGW",56,def_pptpWGW);
addCfg("mppeEn",56,def_pptpMPPE);
addCfg("dynamicMTU",52,def_dynamicMTU);
addCfg("staticMTU",52,def_staticMTU);
//huangxiaoli add for 8021x
addCfg("x1_MTU",52,def_X1mtu);

addCfg("x1AdrMode",31,def_X1mode);
addCfg("x1_name",31,def_X1UN);
addCfg("x1_pwd",31,def_X1PW);
//end add
addCfg("WANIP",31,def_WANIP);
addCfg("WANMSK",32,def_WANMSK);
addCfg("WANGW",33,def_WANGW);
addCfg("DS1",1,def_DNS1);
addCfg("DS2",2,def_DNS2);
var page=0;

var illegal_user_pass = new Array("\\r","\\n","\\","'","\"");

function init(f)
{
	m=getCfg("WANT");
	document.frmSetup.WANT1.value=m;
	if (m<7 && m>=0)
	{
		document.getElementById("wan_sec").innerHTML= pages[m];
	}	
	ispSelectChange(f);     //weige modify
}

/*huang add*/
function chgPPW(val)
{
	if(document.getElementById("PPW").type == "password"){
		document.getElementById("td_PPW").innerHTML='&nbsp;&nbsp;&nbsp;&nbsp;<input name=PPW id=PPW maxLength=50 type=text class=text size=25 onBlur="chgPPW(this.value)" value='+val+'>';
		document.getElementById("PPW").focus();
		document.getElementById("PPW").focus();
		document.getElementById("PPW").value=val;
		}
	else if(document.getElementById("PPW").type == "text"){
		document.getElementById("td_PPW").innerHTML='&nbsp;&nbsp;&nbsp;&nbsp;<input name=PPW id=PPW maxLength=50 type=password class=text size=25 onFocus="chgPPW(this.value)" value='+val+'>';
		}
}
/*end add*/

function ispSelectChange(f)
{
 var m=document.frmSetup.WANT1.value;
 document.getElementById("wan_sec").innerHTML= pages[m];
 document.getElementById("message").innerHTML=help[m];
 	cfg2Form(f);
 //----------------weige modify----------------------
	if(f.l2tpAdrMode)
		f.l2tpAdrMode.value=def_l2tpAdrMode;
	if(f.pptpAdrMode)
		f.pptpAdrMode.value=def_pptpAdrMode;
 //------------end------------------
	if(m == 4)//l2tp
		onl2tpArdMode(f);
	else if(m == 3)//pptp
		onpptpArdMode(f);
	else if(m == 5)
		onx1ArdMode(f);
 table_onload1('table1');
}
function preSubmit(f) {
	var m=document.frmSetup.WANT1.value;
	var mtu;
	if (m==2)//pppoe
	{
		var da = new Date();
		f.v12_time.value = da.getTime()/1000;
		mtu = f.MTU.value;
		if(f.PCM[1].checked)
		{
			if (!rangeCheck(f.PIDL,60,3600,"����ʱ��")) return ;
		}	
		if(f.PUN.value == "" || f.PPW.value == "")
		{
			alert("�û���������Ϊ��!");
			return ;
		}
		if(!ill_check(f.PUN.value,illegal_user_pass,"�ʺ�")) return;
		if(!ill_check(f.PPW.value,illegal_user_pass,"����")) return;
		if (!chkStrLen(f.PUN,0,255,"�û���")) return ;
		if (!chkStrLen(f.PPW,0,255,"����")) return ;
		if(f.PCM[3].checked)
		{
			if (!rangeCheck(f.hour1,0,23,"ʱ��")) return ;
			if (!rangeCheck(f.minute1,0,59,"ʱ��")) return ;
			if (!rangeCheck(f.hour2,0,24,"ʱ��")) return ;
			if (!rangeCheck(f.minute2,0,59,"ʱ��")) return ;
			if((Number(f.hour1.value)*60+Number(f.minute1.value) > 1440)||(Number(f.hour2.value)*60+Number(f.minute2.value) > 1440))
			{
				alert("ʱ�䳬���涨��Χ");
				return;
			}
			if((Number(f.hour1.value)*60+Number(f.minute1.value)) >= (Number(f.hour2.value)*60+Number(f.minute2.value)))
			{
				alert("��ʼʱ�����С�ڽ���ʱ��");
				return;
			}
		}	
		setCfg("PST",Number(f.hour1.value)*60+Number(f.minute1.value));
		setCfg("PET",Number(f.hour2.value)*60+Number(f.minute2.value));
	}
	else if (m==0 || m == 5)
	{
		if(m==0)
			mtu = f.staticMTU.value;
		else if(m==5)
			mtu = f.x1_MTU.value;
		
		if(m == 5)
		{
			if(!ill_check(f.elements['x1_name'].value,illegal_user_pass,"�û���")) return;	
			if(!ill_check(f.elements['x1_pwd'].value,illegal_user_pass,"����")) return;
		}
		if((m==0) || ((m==5)&&(f.x1AdrMode.value == "1")))
		{
			if (!verifyIP2(f.WANIP,"IP ��ַ")) return ;
			if (!ipMskChk(f.WANMSK,"��������")) return ;
			if (!verifyIP2(f.WANGW,"���ص�ַ")) return ;
			if (!verifyIP2(f.DS1,"��DNS��ַ")) return ;
			if (!verifyIP0(f.DS2,"����DNS��ַ")) return ;
		}
		/*mtu = f.staticMTU.value;
		if (!verifyIP2(f.WANIP,"IP ��ַ")) return ;
		if (!ipMskChk(f.WANMSK,"��������")) return ;
		if (!verifyIP2(f.WANGW,"���ص�ַ")) return ;
		if (!verifyIP2(f.DS1,"��DNS��ַ")) return ;
		if (!verifyIP0(f.DS2,"����DNS��ַ")) return ;*/		
	}
	else if (m == 4)
	{
		mtu = f.l2tpMTU.value;
		if(f.elements['l2tpPUN'].value == "" || f.elements['l2tpPPW'].value == "")
		{
			alert("�û���������Ϊ��!");
			return ;
		}
		if(!ill_check(f.elements['l2tpPUN'].value,illegal_user_pass,"�û���")) return;	
		if(!ill_check(f.elements['l2tpPPW'].value,illegal_user_pass,"����")) return;
		if(f.l2tpIP.value == "")
		{
			alert("��������ַΪ��!");
			return ;
		}
		if(!ill_check(f.l2tpIP.value,illegal_user_pass,"��������ַ")) return;
		
		if(f.l2tpAdrMode.value == "1")
		{
			if (!verifyIP2(f.l2tpWANIP,"IP ��ַ")) return false;
			if (!ipMskChk(f.l2tpWANMSK,"��������")) return false;
			if (f.l2tpWANGW.value != "" && !verifyIP2(f.l2tpWANGW,"ISP ���ص�ַ")) return false;
		}
	}
	else if (m == 3)
	{
		mtu = f.pptpMTU.value;
		if(f.elements['pptpPUN'].value == "" || f.elements['pptpPPW'].value == "")
		{
			alert("�û���������Ϊ��!");
			return ;
		}
		if(!ill_check(f.elements['pptpPUN'].value,illegal_user_pass,"�û���")) return;	
	  if(!ill_check(f.elements['pptpPPW'].value,illegal_user_pass,"����")) return;
		if(f.pptpIP.value == "")
		{
			alert("��������ַΪ��!");
			return ;
		}
		if(!ill_check(f.pptpIP.value,illegal_user_pass,"��������ַ")) return;

		if(f.pptpAdrMode.value == "1")
		{
			if (!verifyIP2(f.pptpWANIP,"IP ��ַ")) return false;
			if (!ipMskChk(f.pptpWANMSK,"��������")) return false;
			if (f.pptpWANGW.value != "" && !verifyIP2(f.pptpWANGW,"ISP ���ص�ַ")) return false;
		}
	}
	else if(m == 1)
	{
		mtu = f.dynamicMTU.value;
	}
	
	if(m != 7){
		if (!IsNumCheck(mtu)) return ;
		if(parseInt(mtu,10) < 256 || parseInt(mtu,10) > 1500)
		{
			alert("MTUֵ��Χ��256~1500");
			return ;
		}
	}	
	form2Cfg(f);
	document.getElementById("WANT2").value = parseInt(m) + 1;
	f.submit();
} 
function onl2tpArdMode(f) 
{ 
	if(f.l2tpAdrMode.selectedIndex == 0){
		f.l2tpWANIP.disabled = false;
		f.l2tpWANMSK.disabled = false;
		f.l2tpWANGW.disabled = false;
	}
	else{
		f.l2tpWANIP.disabled = true;
		f.l2tpWANMSK.disabled = true;
		f.l2tpWANGW.disabled = true;
	}
}
function onpptpArdMode(f)
{
	if(f.pptpAdrMode.selectedIndex == 0){
		f.pptpWANIP.disabled = false;
		f.pptpWANMSK.disabled = false;
		f.pptpWANGW.disabled = false;
	}else{
		f.pptpWANIP.disabled = true;
		f.pptpWANMSK.disabled = true;
		f.pptpWANGW.disabled = true;
	}
}

function onx1ArdMode(f)
{
	if(f.x1AdrMode.selectedIndex == 0)
	{
		f.WANIP.disabled = false;
		f.WANMSK.disabled = false;
		f.WANGW.disabled = false;
		f.DS1.disabled = false;
		f.DS2.disabled = false;
	}
	else
	{
		f.WANIP.disabled = true;
		f.WANMSK.disabled = true;
		f.WANGW.disabled = true;
		f.DS1.disabled = true;
		f.DS2.disabled = true;
	}
}
	
var help=new Array('&nbsp;&nbsp;&nbsp;&nbsp;��̬IP��������Ŀ���������ṩ���ǹ̶�IP����ѡ��̬IPģʽ��','&nbsp;&nbsp;&nbsp;&nbsp;��̬IP�������� ISP �������� DHCP ��������ѡ���Զ���ȡ IP����� ISP ���Զ�������Щֵ������ DNS ����������','&nbsp;&nbsp;&nbsp;&nbsp;PPPoE�� ���ѯ�����ISP��ȷ���Ƿ��ʹ��PPPoE���������ȷ��ʹ�� PPPoE�� ��������û��������롣','&nbsp;&nbsp;&nbsp;&nbsp;PPTP������ISP�ṩ��PPTP������IP��ַ���û��������룬����WAN��IP ��ַ���������롢Ĭ�����ص�ַ������ѡ��̬��ȡ�����ֶ�����ISP�ṩ����Ϣ��MTU�������������ӵ�MTUֵ���������������ʹ��Ĭ��ֵ��','&nbsp;&nbsp;&nbsp;&nbsp;L2TP������ISP�ṩ��PPTP������IP��ַ���û��������룬����WAN��IP ��ַ���������롢Ĭ�����ص�ַ������ѡ��̬��ȡ�����ֶ�����ISP�ṩ����Ϣ��MTU�������������ӵ�MTUֵ���������������ʹ��Ĭ��ֵ��',
'&nbsp;&nbsp;&nbsp;&nbsp;802.1X������802.1X�����ṩ���û��������롣֧��CHAP��MD5������֤��ʽ��MTU�������������ӵ�MTUֵ���������������ʹ��Ĭ��ֵ���������һ��ָ�� WAN IP ��ַ���������롢Ĭ�����ص�ַ�� DNS ��������ѡ��̬ IP��');
var pages=new Array(
'<TABLE class="content1" id=\"table1\" cellpadding="0" cellspacing="0" style="margin-top:0px;">\
	<TR><TD width="100" align="right">IP��ַ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=15 name=WANIP size=16></TD></TR>\
	<TR><TD align="right">��������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=15 name=WANMSK size=16></TD></TR>\
	<TR><TD align="right">����</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=15 name=WANGW size=16></TD></TR>\
	<TR><TD height=21 align="right">DNS������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=15 name=DS1 size=16></TD></TR>\
	<TR><TD align="right">����DNS������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=15 name=DS2 size=16>����ѡ��</TD></TR>\
	<tr><td align="right">MTU</td>\
		<td>&nbsp;&nbsp;&nbsp;&nbsp;<input class=text name=staticMTU size=4 maxlength="4" value="1500">(��Ǳ�Ҫ������Ķ���Ĭ��ֵ1500)</td></tr>\
	</TABLE>'

,'<table class="content1" id=\"table1\" cellpadding="0" cellspacing="0" style="margin-top:0px;" >\
		<tr><td width="100" align="right">MTU</td>\
		<td>&nbsp;&nbsp;&nbsp;&nbsp;<input class=text name=dynamicMTU size=4 maxlength="4" value="1500">(��Ǳ�Ҫ������Ķ���Ĭ��ֵ1500)</td></tr>\
	</table>'

,'<table  class="content1" id=\"table1\" cellpadding="0" cellspacing="0" style="margin-top:0px;">\
	<tr><td width="100" align="right">�����˺�</td>\
		<td>&nbsp;&nbsp;&nbsp;&nbsp;<input name=PUN maxLength=128 class=text size=25></td></tr>\
	<tr><td width="100" align="right">��������</td>\
		<td id=td_PPW>&nbsp;&nbsp;&nbsp;&nbsp;<input name=PPW id=PPW maxLength=50 type=password class=text size=25 onFocus="chgPPW(this.value)"></td></tr>\
	<tr><td width="100" align="right">MTU</td>\
		<td>&nbsp;&nbsp;&nbsp;&nbsp;<input class=text name=MTU size=4 value="1492">(��Ǳ�Ҫ������Ķ���Ĭ��ֵ1492)</td></tr>\
	<tr><td width="100" align="right">������</td>\
		<td>&nbsp;&nbsp;&nbsp;&nbsp;<input class=text name=SVC maxLength=50 size=25>(��Ǳ�Ҫ��������д)</td></tr>\
	<tr><td width="100" align="right">����������</td>\
		<td>&nbsp;&nbsp;&nbsp;&nbsp;<input class=text name=AC maxLength=50 size=25>(��Ǳ�Ҫ��������д)</td></tr>\
	<TR><TD colSpan=2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;����������Ҫ����ѡ���Ӧ����ģʽ:</TD></TR>\
	<TR><TD colspan=2 height=30>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<INPUT name=PCM type=radio class="radio" value=0>�Զ����ӣ��ڿ����Ͷ��ߺ��Զ��������ӡ�</TD></TR>\
	<TR><TD colspan=2 height=30>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<INPUT name=PCM type=radio class="radio" value=1>�������ӣ����з�������ʱ�Զ��������ӡ�</TD></TR>\
	<TR><td colspan=2 height=30> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;�Զ����ߵȴ�ʱ�䣺 \
				<INPUT class=text maxLength=4 name=PIDL size=4> (60-3600,��) </TD></TR>\
	<TR><TD colspan=2 height=30>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<INPUT name=PCM type=radio class="radio" value=2>�ֶ����ӣ����û��ֶ��������ӡ�</TD></TR>\
	<TR><TD colspan=2 height=30>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<INPUT name=PCM type=radio class="radio" value=3>��ʱ���ӣ���ָ����ʱ���Զ��������ӡ�</TD></TR>\
	<TR><td colspan=2 height=30>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ע�⣺ֻ�е�������ϵͳ���ߡ��˵��ġ�ʱ�����á�<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;�������˵�ǰʱ��󣬡���ʱ���ӡ����ܲ�����Ч��</TD></TR>\
	<TR><td colspan=2 height=30>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;����ʱ�Σ���  \
		<INPUT class=text maxLength=2 name=hour1 size=2 value="<%aspTendaGetStatus("ppoe","h_s");%>"> ʱ \
		<INPUT class=text maxLength=2 name=minute1 size=2 value="<%aspTendaGetStatus("ppoe","m_s");%>"> �ֵ� \
		<INPUT class=text maxLength=2 name=hour2 size=2 value="<%aspTendaGetStatus("ppoe","h_e");%>"> ʱ \
		<INPUT class=text maxLength=2 name=minute2 size=2 value="<%aspTendaGetStatus("ppoe","m_e");%>"> ��</TD></TR>\
	</table>'
	
,'<TABLE class="content1" id=\"table1\" cellpadding="0" cellspacing="0" style="margin-top:0px;">\
	<TR><TD width=105 align="right">PPTP��������ַ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text name="pptpIP" size=15 maxlength="15"></TD></TR>\
	<TR><TD align="right">�û���</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=50 name="pptpPUN" size=25></TD></TR>\
	<TR><TD align="right">����</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=50 name="pptpPPW" size=25 type=password></TD></TR>\
	<TR><TD align="right">MTU</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text name="pptpMTU" size=23 value="1492"></TD></TR>\
	<TR><TD align="right">��ַģʽ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<SELECT name="pptpAdrMode" onChange="onpptpArdMode(document.frmSetup)"><option value="1">Static</option>\
			<option value="2">Dynamic</option>\
			</SELECT></TD></TR>\
	<TR><TD align="right">IP��ַ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="pptpWANIP" maxlength="15" size="15"></TD></TR>\
	<TR><TD align="right">��������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="pptpWANMSK" size="15" maxlength="15"></TD></TR>\
	<TR><TD align="right">Ĭ������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="pptpWANGW" size="15" maxlength="15"></TD></TR>\
	<TR><TD align="right">MPPE</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT type="checkbox" name="mppeEn" value="1"></TD></TR>\
	</TABLE>'
	
	,'<TABLE class="content1" id=\"table1\" cellpadding="0" cellspacing="0" style="margin-top:0px;">\
	<TR><TD align="right" width=105>L2TP��������ַ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text name="l2tpIP" size="15" maxlength="15"></TD></TR>\
	<TR><TD align="right">�û���</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=50 name="l2tpPUN" size=25></TD></TR>\
	<TR><TD align="right">����</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=50 name="l2tpPPW" size=25 type=password></TD></TR>\
	<TR><TD align="right">MTU</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text name="l2tpMTU" size=23 value="1492"></TD></TR>\
	<TR><TD align="right">��ַģʽ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<SELECT name="l2tpAdrMode" id="l2tpAdrMode" onChange="onl2tpArdMode(document.frmSetup)"><option value="1">Static</option>\
			<option value="2">Dynamic</option>\
			</SELECT></TD></TR>\
	<TR><TD align="right">IP��ַ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="l2tpWANIP" size="15" maxlength="15"></TD></TR>\
	<TR><TD align="right">��������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="l2tpWANMSK" size="15" maxlength="15"></TD></TR>\
	<TR><TD align="right">Ĭ������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="l2tpWANGW" size="15" maxlength="15"></TD></TR>\
	</TABLE>'
	
	,'<TABLE class="content1" id=\"table1\" cellpadding="0" cellspacing="0" style="margin-top:0px;">\
	<TR><TD align="right" width=105>�û���</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=50 name="x1_name" size=25></TD></TR>\
	<TR><TD align="right">����</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=50 name="x1_pwd" size=25 type=password></TD></TR>\
	<TR><TD align="right">MTU</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text name="x1_MTU" size=23 value="1492">(��Ǳ�Ҫ������Ķ���Ĭ��ֵ1500)</TD></TR>\
	<TR><TD align="right">��ַģʽ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<SELECT name="x1AdrMode" id="x1AdrMode" onChange="onx1ArdMode(document.frmSetup)"><option value="1">Static</option>\
			<option value="2">Dynamic</option>\
			</SELECT></TD></TR>\
	<TR><TD align="right">IP��ַ</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="WANIP" size="15" maxlength="15"></TD></TR>\
	<TR><TD align="right">��������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="WANMSK" size="15" maxlength="15"></TD></TR>\
	<TR><TD align="right">Ĭ������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class="text" name="WANGW" size="15" maxlength="15"></TD></TR>\
	<TR><TD height=21 align="right">DNS������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=15 name=DS1 size=16></TD></TR>\
	<TR><TD height=21 align="right">����DNS������</TD>\
		<TD>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT class=text maxLength=15 name=DS2 size=16>����ѡ��</TD></TR>\
	</TABLE>'
);

</SCRIPT>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0"  onLoad="init(document.frmSetup);" class="bg">
	<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
      <tr>
        <td width="33">&nbsp;</td>
        <td width="679" valign="top">
		<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" height="100%">
          <tr>
            <td align="center" valign="top">
			<table width="98%" border="0" align="center" cellpadding="0" cellspacing="0" height="100%">
              <tr>
                <td align="center" valign="top">
			<FORM name=frmSetup method=POST action=/goform/AdvSetWan>
			<INPUT type=hidden name=GO value=wan_connectd.asp >
			<input type="hidden" id="rebootTag" name="rebootTag">
			<input type="hidden" id="v12_time" name="v12_time">
			<input type="hidden" name="WANT2" id="WANT2">
			<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="content1" id="table2">
			  <tr>
				<td width="100" align="right">ģ&nbsp; ʽ</td>
			    <td>&nbsp;&nbsp;&nbsp;&nbsp;<select name="WANT1" id="WANT1" onChange="ispSelectChange(document.frmSetup)">
					<option value=2>PPPOE</option>
					<option value=0>��̬IP</option>
					<option value=1>DHCP</option>
					<option value="5">802.1x</option>
<!--
					<option value=3>PPTP</option>
					<option value=4>L2TP</option>
-->
				 </select>
				 </td>
			  </tr>
			</table>

				<div id=wan_sec style="visibility:visible;">
				</div>  
				
				<SCRIPT>tbl_tail_save('document.frmSetup')</SCRIPT>
          </FORM>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
	<table border="0" cellpadding="0" cellspacing="0"  class="left1" style="margin-top:25px;">
	<tr><td align="center"><b>������Ϣ</b></td></tr>
	<tr><td><div id="message"></div></td></tr>
	</table>
		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table2');
    </script>
</BODY>
</HTML>





