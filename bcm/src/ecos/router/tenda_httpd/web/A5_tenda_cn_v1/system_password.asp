<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>System | Administrator Settings</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<SCRIPT language=JavaScript>

addCfg("SYSUN",104, "<%aspTendaGetStatus("sys","username");%>");
addCfg("SYSPS", 106, "" );
addCfg("SYSOPS", 106, "" );
addCfg("SYSPS2", 106, "" );

function init(f){
	cfg2Form(f);
}

function preSubmit(f) { 

	if (!chkStrLen(f.SYSPS,3,12,"���� ")) return ;
	form2Cfg(f);
	if (f.SYSPSC.value=='1') // if passwd changed, send old passwd too
		addFormElm("CPW",f.SYSOPS.value);
	if(check_text(f.SYSOPS.value)&&check_text(f.SYSPS.value))
	{
			if (!chkPwdUpdate(f.SYSPS,f.SYSPS2,f.SYSPSC)){
    	 	return;
		  }
    	f.submit();
	}else
	{
		alert("����ֻ������ĸ���������!");
		return;
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
			<table width="98%" border="0" align="center" cellpadding="0" cellspacing="0"  height="100%">
              <tr>
                <td align="center" valign="top">
					<form name=frmSetup method=POST action=/goform/SysToolChangePwd>
					<input type=hidden name=GO value="system_password.asp">
					<input type=hidden name="SYSPSC" value=0>
					<table cellpadding="0" cellspacing="0" class="content2">
					<tr>
					  <td height="25" colspan=2 valign="top" nowrap>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ��ҳ�޸�ϵͳ����Ա�����롣</td>
					</tr>
					<tr>
					  <td height="25" colspan=2 valign="top" nowrap>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ע�⣺����ֻ�������֡���ĸ��ɡ�</td>
					</tr>
					</table>
				<table cellpadding="0" cellspacing="0" class="content3" id="table1">
					<tr><td width="100" align="right">������</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input class="text" type=text maxlength="12" name="SYSOPS" size="15" onKeyPress=chkPwd1Chr2(this,this.form.SYSPS,this.form.SYSPS2,this.form.SYSPSC);></td></tr>
					<tr><td width="100" align="right">������</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input class="text" type=text maxlength="12" name="SYSPS" size="15" onKeyPress=chkPwd1Chr2(this.form.SYSOPS,this,this.form.SYSPS2,this.form.SYSPSC);></td></tr>
					<tr><td width="100" align="right">ȷ��������</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input class="text" type=text maxlength="12" name="SYSPS2" size="15"  onKeyPress=chkPwd1Chr2(this.form.SYSOPS,this.form.SYSPS,this,this.form.SYSPSC);></td></tr>
				</table>
					
					<SCRIPT>
						tbl_tail_save("document.frmSetup");
					</SCRIPT>
				
				</FORM>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('·����Ĭ������Ϊ admin �������·�������롣������������룬�������ϵ������û�ֻҪ������������ admin ���ɷ���·������<br>\
		&nbsp;&nbsp;&nbsp;&nbsp;������:��������롣�״�ʹ��·����ʱ��Ĭ�������� admin ��( ע�⣺������ʧ�����ǾͲ��ָܻ������������ʧ�����ǣ���������·������λ������Ĭ������)��<br>\
		&nbsp;&nbsp;&nbsp;&nbsp; ������:���������롣�������Ϊ3 �� 12 ���ַ������Ҳ����ո�<br>\
		&nbsp;&nbsp;&nbsp;&nbsp;ȷ��������:�����������������ȷ�ϡ�'
		);</script>

		</td>
      </tr>
    </table> 
	<script type="text/javascript">
	  table_onload('table1');
    </script>
</BODY>
</HTML>





