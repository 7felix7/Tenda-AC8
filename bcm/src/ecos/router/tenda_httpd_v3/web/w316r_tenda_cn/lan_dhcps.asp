<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>DHCP | Server</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<SCRIPT language=JavaScript>

def_LANIP = "<%aspTendaGetStatus("lan","lanip");%>";

var LANIP=def_LANIP;
var netip=LANIP.replace(/\.\d{1,3}$/,".");

function init()
{
	document.LANDhcpsSet.DHEN.checked = <%aspTendaGetStatus("lan","dhcps");%>;//�Ƿ����ã�0���رգ�1�����ã�
	document.LANDhcpsSet.DHLT.value = "<%aspTendaGetStatus("lan","lease_time");%>";//����ʱ�䣺3600��һСʱ��7200����Сʱ�� 10800����Сʱ�� 86400��һ�죻 172800�����죻 604800��һ��

	document.LANDhcpsSet.dips.value = (("<%aspTendaGetStatus("lan","dhcps_start");%>").split("."))[3];//IP�ؿ�ʼ��ַ
	document.LANDhcpsSet.dipe.value = (("<%aspTendaGetStatus("lan","dhcps_end");%>").split("."))[3];//IP�ؽ�����ַ
}

function preSubmit(f) {

	var loc = "/goform/DhcpSetSer?GO=lan_dhcps.asp";

	if (!rangeCheck(f.dips,1,254,"IP�ؿ�ʼ��ַ")) return ;
	if (!rangeCheck(f.dipe,1,254,"IP����ֹ��ַ")) return ;
   
   	if (Number(f.dips.value)>Number(f.dipe.value)) {
      alert("IP�ؿ�ʼ��ַ���ܴ��ڽ�����ַ !!!");
      return ;
   	}

	if(f.DHEN.checked)
	{
		loc += "&dhcpEn=1";
	}
	else
	{
		loc += "&dhcpEn=0";
	}
	
	loc += "&dips=" + netip + f.dips.value;
	loc += "&dipe=" + netip + f.dipe.value;
	loc += "&DHLT=" + f.DHLT.value;
		
	var code = 'location="' + loc + '"';
	eval(code);
}
</SCRIPT>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0" onLoad="init()" class="bg">
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
				<form name=LANDhcpsSet method=POST action=/goform/DhcpSetSer>
				<input type=hidden name=GO  value="lan_dhcps.asp">
				<table cellpadding="0" cellspacing="0" class="content1" id="table1">
					<tr> 
					  <td width="100" align="right">DHCP������</td> 
					  <td>&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name=DHEN value=1>����</td>
					</tr>
					<tr> 
					  <td align="right">IP�ؿ�ʼ��ַ</td> 
					  <td>&nbsp;&nbsp;&nbsp;&nbsp;<SCRIPT>document.write(netip);</SCRIPT>
						<input NAME=dips class=text  SIZE="3"></td>
					</tr>
					<tr> 
						<td align="right">IP�ؽ�����ַ</td> 
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<SCRIPT>document.write(netip);</SCRIPT>
						<input NAME=dipe class=text  SIZE=3></td>
					</tr>
					<tr> 
					  <td align="right">����ʱ��</td> 
					  <td>
						&nbsp;&nbsp;&nbsp;&nbsp;<select NAME=DHLT SIZE=1>
						<option VALUE="3600">һСʱ</option>
						<option VALUE="7200">��Сʱ</option>
						<option VALUE="10800">��Сʱ</option>
						<option VALUE="86400">һ��</option>
						<option VALUE="172800">����</option>
						<option VALUE="604800">һ��</option>
						</select>
					  </td>
					</tr>
				</table>  
				<input type=hidden name=dhcpEn>
					<SCRIPT>tbl_tail_save("document.LANDhcpsSet");</SCRIPT>
				</form>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('DHCP�������ṩ��Ϊ�ͻ����Զ�����IP��ַ�Ĺ��ܣ������ʹ�ñ�·������DHCP���������ܵĻ�����������DHCP�������Զ��������þ������и��������TCP/IPЭ�顣<br>\		&nbsp;&nbsp;&nbsp;&nbsp;IP��ַ��:����һ����ʼIP��ַ��һ����ֹIP��ַ���γɷ��䶯̬IP��ַ�ķ�Χ��\
		<br>&nbsp;&nbsp;&nbsp;&nbsp;����ʱ��: ����(DHCP�ͻ���)Ҫ��ʱ�ŷ������ʱ�䡣');
		</script>

		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table1');
    </script>
</BODY>
</HTML>





