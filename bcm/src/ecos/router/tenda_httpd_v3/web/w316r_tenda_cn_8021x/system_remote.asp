<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>System | Remote Management</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<SCRIPT language=JavaScript>
var def_RMEN = "<%getfirewall("wan","wanweben");%>";//1:����;0:�ر�
var def_RMPORT = "<%getfirewall("wan","webport");%>";//�˿�
var def_RMIP = "<%getfirewall("wan","rmanip");%>";//IP ��ַ:��ʼIP1;����IP1p
var lan_ip = "<%aspTendaGetStatus("lan", "lanip");%>";
var lan_ipmask = "<%aspTendaGetStatus("lan", "lanmask");%>";
function init(f)
{
	var en = parseInt(def_RMEN);
	f.RMEN.checked = en;
	onSwitch();
//	if(en)
//	{
//
//		var ip = def_RMIP;
//		f.RMsIP1.value = (ip.split(";"))[0];
//		f.RMeIP1.value = (ip.split(";"))[1];
//	}
    f.RMsIP1.value = def_RMIP;
	f.RMPORT.value = def_RMPORT;

//	else
//		f.RMPORT.value = "";
}

function preSubmit(f) {     
	var sip1 = f.RMsIP1.value;
	//var eip1 = f.RMeIP1.value;
	var port = f.RMPORT.value;
	var loc = "/goform/SafeWanWebMan?GO=system_remote.asp";
	
	if(f.RMEN.checked)
	{	
		if (!rangeCheck(f.RMPORT,1,65535,"�˿�")) return;
		loc += "&RMEN=1&port=" + port;
		if (!verifyIP0(f.RMsIP1,"IP��ַ")) return ;
		//if (!verifyIP0(f.RMeIP1,"������ַ")) return ;
		if(sip1 == "")
			sip1 = "0.0.0.0";
		//if(eip1 == "")
			//eip1 = "0.0.0.0";
		//add by stanley
		//alert(wan_ip+wan_ipmask);
		if (ipCheck(lan_ip,sip1,lan_ipmask)) {
			alert(f.RMsIP1.value+ "������LAN��IPͬ����!");
			f.RMsIP1.value="0.0.0.0";
			return ;
			}
		
//		if (ipCheck(lan_ip,eip1,lan_ipmask))	{
//			alert(f.RMeIP1.value+ "������LAN��IPͬ����!");
//			f.RMeIP1.value="0.0.0.0";
//			return ;
//			}
		//end
//		if(!orderIP(sip1,eip1))	
//		{
//			alert("��ʼIP���ɴ��ڽ���IP��");
//			return ;
//		}
		
		loc += "&IP=" + sip1;		
	}
	else
	{
		loc += "&RMEN=0";
	}
	
   var code = 'location="' + loc + '"';
   eval(code);
}

function onSwitch()
{
	if(document.frmSetup.RMEN.checked)
	{

		document.getElementById("RMPORT").disabled = false;	
		document.getElementById("ipTab").style.display = "";
	}
	else
	{
		document.getElementById("ipTab").style.display = "none";
		document.getElementById("RMPORT").disabled = true;
	}
}
</SCRIPT>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0" onLoad="init(document.frmSetup);" class="bg">
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
				<form name=frmSetup method="POST" action=/goform/SafeWanWebMan>
				<input type=hidden id=GO value= system_remote.asp>
				<table class=content1>
					<tr> 
					  <td height="30">����&nbsp;&nbsp;<input type="checkbox" id=RMEN onClick="onSwitch()"></td>
					</tr>

				</table>
				<table cellpadding="0" cellspacing="0" class="content1" id="ipTab" style="margin-top:0px;">
								
					<tr>
					  <td width="100" height="30" align="right">�˿�</td>
					  <td height="30">
					    &nbsp;&nbsp;&nbsp;&nbsp;<input class=text id=RMPORT size=5 maxlength=5></td>
					</tr>
					<tr>
					  <td height="30" align="right">IP��ַ</td>
					  <td height="30">
					    &nbsp;&nbsp;&nbsp;&nbsp;<input class=text id=RMsIP1 size=15 maxlength=15 ></td>
					</tr>
				</table>	
				<SCRIPT>tbl_tail_save("document.frmSetup");</SCRIPT>
				
			</FORM>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('ʹ�ô˹��ܿ���ͨ��Internet��Զ��λ�ù���·������Ҫ���ô˹��ܣ���ѡ�����ã�Ȼ��ʹ�� PC �ϵ�ָ���˿���Զ�̹���·������\
			<br>&nbsp;&nbsp;&nbsp;&nbsp;�˿ڣ�����ִ��Զ�̹���Ķ˿ںš�\
			<br>&nbsp;&nbsp;&nbsp;&nbsp;IP��ַ���������п���ִ��Զ��WEB����ļ������IP��ַ��'
			);</script>

		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('ipTab');
    </script>
</BODY>
</HTML>





