<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>System | Backup/Restore</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<script language="JavaScript">

function UpLoadCfg()
{
	if (document.frmSetup.fileCfg.value == "")
	{
		alert("������ѡ������ļ���");
		return ;
	}
	if(confirm("�������Ҫ����������"))
	{
		document.frmSetup.submit() ;
	}
}

function DownLoadCfg()
{
	if(confirm("�������ò�������ָ�����������·����"))
	{
		refresh("/cgi-bin/DownloadCfg/RouterCfm.cfg");
	}
}

</SCRIPT>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0" class="bg">
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
		<input type=hidden name=GO value=system_backup.asp>
          <table class=space width=100%>
            <tr>
              <td><div align="center" id="waitfor"> 
			      <table cellpadding="0" cellspacing="0" class="content2">
			        <tr>
                      <td colspan="2" valign="top">&nbsp;&nbsp;�����Ա���/�ָ�·�����ĵ�ǰ����</td>
                    </tr>
				  </table>
                  <table cellpadding="0" cellspacing="0" class="content3" id="table1">
                   
                    <tr>
                      <td height="30">&nbsp;&nbsp;��ѡ����Ҫ�������ò������ļ�Ŀ¼:
                        <input type="button" class=button2 value="����" onMouseOver="style.color='#FF9933'" onMouseOut="style.color='#000000'" onClick="DownLoadCfg()"/></td>
                    </tr> 
					<form name=frmSetup method="POST" action="/cgi-bin/UploadCfg" enctype="multipart/form-data">
                    <tr>
                      <td>&nbsp;&nbsp;ѡ������Ҫ����������ļ�:<br>         
					  &nbsp;&nbsp;<input type="file" name="fileCfg" id="fileCfg"/>&nbsp;&nbsp;
					  <input class=button2 type="button" value="�ָ�" onMouseOver="style.color='#FF9933'" onMouseOut="style.color='#000000'" onClick="UpLoadCfg()"/>
					</td></tr></form>
                  </table>
              </div></td>
            </tr>
            <tr>
              <td></td>
            </tr>
          </table>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('���������ݡ�����Խ���ǰ�������ļ�����ʽ���ݵ���Ӧ��Ŀ¼������һ��ϵͳ���õı����ļ���\
		ͬ����ֻ��Ҫ������������ѡȡ��ӦĿ¼�е������ļ���������ָ�������ɺ���������·�ɽ����Իָ�������Ҫ��ϵͳ���á� '
		);</script>

		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table1');
    </script>
</BODY>
</HTML>





