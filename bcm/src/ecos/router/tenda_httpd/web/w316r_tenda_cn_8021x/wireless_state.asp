<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>Wireless | State</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<script language="javascript">
var enablewireless="<% get_wireless_basic("WirelessEnable"); %>";
function init(){
	if(enablewireless==1){
	}else{
		alert("�������߹��ܺ�ſ���ʹ�ñ����ܣ�");
		top.mainFrame.location.href="wireless_basic.asp";
	}
}


//function refresh(){
// window.location="wireless_state.asp";
//}
//setTimeout("refresh()", 300);


</script>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0" onLoad="init();" class="bg">
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
				<FORM name=formLan action="/cgi-bin/cgi_soho" method=post onSubmit="return doSubmit();">
					<table class=content1>
					   <TR> <TD valign="top">��ҳ��ʾ����·������������Ϣ��
					   </TR>
						
						<TR> <TD>��ǰ���ӵ������б�
							  <input type=button class=button2 value="ˢ��" onMouseOver="style.color='#FF9933'" onMouseOut="style.color='#000000'" onclick=refresh("wireless_state.asp")></TD>
						</TR>
							<tr><td>	
							 <TABLE align=center cellPadding=0 cellSpacing=0 width=450>
									<TBODY>
									  <TR>
										<TD width=450><TABLE align=center border=1 cellPadding=0 cellSpacing=0 class=content1 style="margin-top:0px;" id="table1">
											<TBODY>
											  <TR>
												<TD align=middle width=20%>���</TD>
												<TD align=middle width=50%>MAC��ַ</TD>
												<TD align=middle width=30%>����</TD>
											  </TR>
                              <%  get_wireless_station("stationinfo"); %>
											 
											</TBODY>
										</TABLE></TD>
									  </TR>
						 
								  </TABLE>
								  <br>
								  </td>
								<td class=vline rowspan=15 width="27"><br></td>
							  </tr>
											
							 <TR><TD class=hline></TD></TR>   
						   </tbody>
						</table>
				</FORM>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('��ҳ����Բ鿴���������ӵĿͻ�����Ϣ��<br>\
			MAC��ַ����ǰ��������������MAC��ַ��<br>\�����ŵ�Ƶ�ʿ�ȡ�'
		);</script>

		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table1');
    </script>
</BODY>
</HTML>





