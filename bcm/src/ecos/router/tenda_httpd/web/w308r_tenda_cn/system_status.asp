<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>System | Status</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<script language="JavaScript" type="text/javascript">
conType=new Array("��̬ IP","��̬ IP","PPPoE","PPTP","L2TP");
state=new Array("����","����");
conStat=new Array("δ����", "������...","������");

cableDSL="<%sysTendaGetStatus("wan","contstatus");%>";
subMask="<%sysTendaGetStatus("wan","wanmask");%>";
wanIP = "<%sysTendaGetStatus("wan","wanip");%>";
gateWay="<%sysTendaGetStatus("wan","gateway");%>";
dns1="<%sysTendaGetStatus("wan","dns1");%>";
dns2="<%sysTendaGetStatus("wan","dns2");%>";
conntime="<%sysTendaGetStatus("wan","connetctime");%>";


conTypeIdx="<%aspTendaGetStatus("wan","connecttype");%>";
run_code_ver="<%aspTendaGetStatus("sys","sysver");%>";
boot_code_ver="<%aspTendaGetStatus("sys","bootloadver");%>";
hw_ver="<%aspTendaGetStatus("sys","hardwarever");%>";
clients="<%aspTendaGetStatus("sys","conclient");%>";
uptime= "<%aspTendaGetStatus("sys","runtime");%>";
systime = "<%aspTendaGetStatus("sys","systime");%>";
lan_mac="<%aspTendaGetStatus("sys","lanmac");%>";
wan_mac="<%aspTendaGetStatus("sys","wanmac");%>";
function preSubmit(idx) {   
   var f=document.systemStatus;

   f.action.value=idx;
   f.submit() ;
}
function timeStr(t)
{
	if(t < 0)
	{
		str='00:00:00';
		return str;
	}
	var s=t%60;
	var m=parseInt(t/60)%60;
	var h=parseInt(t/3600)%24;
	var d=parseInt(t/86400);

	var str='';
	if (d > 999) { return '����'; }
	if (d) str+=d+'�� ';
	str+=fit2(h)+':';
	str+=fit2(m)+':';
	str+=fit2(s);
	return str;
}
</script>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0" class="bg">
	<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
      <tr>
        <td width="33">&nbsp;</td>
        <td width="679" valign="top">
		<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" height="100%">
          <tr>
            <td align="center" valign="top"><table width="98%" border="0" align="center" cellpadding="0" cellspacing="0" height="100%">
                <tr>
                  <td align="center" valign="top">
				  <form name=systemStatus method=POST action=/goform/SysStatusHandle>
				  <INPUT type=hidden name=CMD value=WAN_CON>
				  <INPUT type=hidden name=GO value=system_status.asp>
				  <INPUT type=hidden name=action>
				  <table border="0" align="center" cellpadding="0" cellspacing="0" class="content1">
                    <tr>
                      <td colspan="2" bgcolor="#9D9D9D">&nbsp;&nbsp;<strong>WAN��״̬��</strong></td>
                    </tr>
				</table>
				<div style="width:75%; height:1px; background-color:#c0c7cd; overflow:hidden; padding:0px; margin-top:1px;"></div>
				<table border="0" align="center" cellpadding="0" cellspacing="0" class="content3" id="table1">
                    <tr>
                      <td width="100" align="right">����״̬</td>
                      <td>&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write(conStat[cableDSL]);</script></td>
                    </tr>
                    <tr>
                      <td width="100" align="right" >WAN IP</td>
                      <td >&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write(wanIP);</script></td>
                    </tr>
                    <tr>
                      <td width="100" align="right" >��������</td>
                      <td >&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write(subMask);</script></td>
                    </tr>
                    <tr>
                      <td width="100" align="right" >����</td>
                      <td >&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write(gateWay);</script></td>
                    </tr>
                    <tr>
                      <td width="100" align="right" >����������</td>
                      <td >&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write(dns1);</script></td>
                    </tr>
                    <tr>
                      <td width="100" align="right" >��������������</td>
                      <td >&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write(dns2);</script></td>
                    </tr>
                    <tr>
                      <td width="100" align="right" >���ӷ�ʽ</td>
                      <td >&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write(conType[conTypeIdx-1])</script></td>
                    </tr>
                    <script>
					if ((conTypeIdx==2)||(conTypeIdx==3)||(conTypeIdx==4) ||(conTypeIdx==5))
					{
						document.write("<tr><td align=\"right\" width=\"100\" >����ʱ��</td>");
						document.write("<td >&nbsp;&nbsp;&nbsp;&nbsp;");
						document.write(timeStr(conntime));
						document.write("</td></tr>");
					}  
					</script>
                    </table>
					<table border="0" align="center" cellpadding="0" cellspacing="0" class="content3">
                      <script>
					  if (conTypeIdx==2) {//dhcp
						 document.write('<tr height="30"><td colspan="2">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  <input type=button class=button2 value="�ͷ�" onclick=preSubmit(1);>');
						 document.write('&nbsp;&nbsp;&nbsp;&nbsp;<input type=button class=button2 value="����" onclick=preSubmit(2);></td></tr>');
					  }
					  else if (conTypeIdx == 3) 
					  { //pppoe
						if(cableDSL == 0)//unlink
						{
							document.write('<tr height="30"><td colspan="2">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <input type=button class=button2 value="����" onclick=preSubmit(3);>');
							document.write('&nbsp;&nbsp;&nbsp;&nbsp; <input type=button class=button2 value="�Ͽ�" disabled="disabled"></td></tr>');
						}
						//else if(cableDSL == 1)//linked
						//{
						//	document.write('<td>&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp; <input type=button class=button2 value="����" disabled="disabled"></td>');
						//	document.write('<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type=button class=button2 value="�Ͽ�" onclick=preSubmit(4);></td>');
						//}
						else//linked ||linking
						{
							 document.write('<tr height="30"><td colspan="2">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <input type=button class=button2 value="����"  disabled="disabled">');
							 document.write(' &nbsp;&nbsp;&nbsp;&nbsp;<input type=button class=button2 value="�Ͽ�" onclick=preSubmit(4);></td></tr>');
						}
					  }
			</script>
                    
                  </table>
					<table border="0" align="center" cellpadding="0" cellspacing="0" class="content1">
					  <tr>
						<td colspan="2" bgcolor="#9D9D9D">&nbsp;&nbsp;<strong>ϵͳ״̬��</strong></td>
					  </tr>
					</table>
					<div style="width:75%; height:1px; background-color:#c0c7cd; overflow:hidden; padding:0px; margin-top:1px;"></div>
					<table border="0" align="center" cellpadding="0" cellspacing="0" class="content3" id="table2">
					  <tr>
					    <td width="100" align="right">LAN MAC ��ַ</td>
					    <td>&nbsp;&nbsp;&nbsp;&nbsp;<SCRIPT>document.write( lan_mac );</SCRIPT></td>
				      </tr>
					  <tr>
					    <td width="100" align="right">WAN MAC ��ַ</td>
					    <td>&nbsp;&nbsp;&nbsp;&nbsp;<SCRIPT>document.write( wan_mac );</SCRIPT></td>
				      </tr>
					  <tr>
					  <td width="100" align="right">ϵͳʱ��</td>
					  <td>&nbsp;&nbsp;&nbsp;&nbsp;<SCRIPT>document.write(systime);</SCRIPT></td>
					</tr>
					 <tr>
					  <td width="100" align="right">����ʱ��</td>
					  <td>&nbsp;&nbsp;&nbsp;&nbsp;<SCRIPT>document.write(timeStr(uptime));</SCRIPT></td>
					</tr>
					<tr>
					  <td width="100" align="right" >�ͻ��˸���</td> 
					  <td >
						&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write( clients );</script>		  </td>
					</tr>
					<tr>
					  <td width="100" align="right" >����汾��</td> 
					  <td >
						&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write( run_code_ver );</script>		  </td>
					</tr>
			
					<tr>
					  <td width="100" align="right" >Ӳ���汾��</td> 
					  <td >
						&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write( hw_ver );</script>		  </td>
					</tr>
					</table>
					</form>
		</td>
                </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('���ӷ�ʽ����ʾWAN�ڵĵ�ǰ���뷽ʽ��<br>\
&nbsp;&nbsp;&nbsp;&nbsp;����ʱ�䣺���뷽ʽΪ��̬IP��PPPOEʱ��·����WAN�������ӵ�ʱ�䡣<br>\
&nbsp;&nbsp;&nbsp;&nbsp;����汾��·�����̼��汾��'
		);</script>		
		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload2('table1');
	  table_onload2('table2');
    </script>
</BODY>
</HTML>





