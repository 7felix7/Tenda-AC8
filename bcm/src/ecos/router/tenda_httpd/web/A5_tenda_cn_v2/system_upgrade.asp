<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>System | Firmware Upgrade</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<SCRIPT language=JavaScript>

var pc = 0;

FirwareVerion="<%aspTendaGetStatus("sys","sysver");%>";//�������汾
FirwareDate="<%aspTendaGetStatus("sys","compimetime");%>";//��������

function init(f){
	f.reset();
}

function setWidth(windowObj, el, newwidth)
{
    if (document.all)
	{
	  if (windowObj.document.all(el) )
        windowObj.document.all(el).style.width = newwidth ;
	}
	else if (document.getElementById)
	{
	  if (windowObj.document.getElementById(el) )
	    windowObj.document.getElementById(el).style.width = newwidth;
	}
}

function OnUpGrade()
{
	pc+=1; 	
	
	if (pc > 100) 
	{      
		clearTimeout(time); 
		return;
	}
	
	//setWidth(self, "table_lpc", pc + "%");
	document.getElementById("table_lpc").style.width= pc+"%";
	document.getElementById("table_lpc_msg").innerHTML ="������..." + pc + "%";
	
	setTimeout("OnUpGrade()",150);
}

function UpGrade(){        
	if (document.frmSetup.upgradeFile.value == "")
	{
		alert("������ѡ�������ļ���");
		return ;
	}
	if(confirm('��ȷ��Ҫ������?')){
	   document.getElementById("fwsubmit").disabled = true;
	   document.frmSetup.submit() ;
	   document.getElementById("table_lpc").style.display = "";
	   OnUpGrade();
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
				<table cellpadding="0" cellspacing="0" class="content2">
				<tr><td colspan=2 valign="top">&nbsp;&nbsp;ͨ��������·�������������������µĹ��ܡ�</td>
				</tr>
				</table>				

				<table cellpadding="0" cellspacing="0" class="content3" id="table1">				
				<form name=frmSetup method="POST" action="/cgi-bin/upgrade" enctype="multipart/form-data">
					<tr><td nowrap>&nbsp;&nbsp;ѡ��̼��ļ�:<br>
					&nbsp;&nbsp;<input type="file" name="upgradeFile"/>&nbsp;&nbsp;&nbsp;&nbsp;
					<input id=fwsubmit type=button class=button2 value=���� onMouseOver="style.color='#FF9933'" onMouseOut="style.color='#000000'" onClick="UpGrade()"/>
					</td>
					</tr>
					</form>
				</table>
				
				<table cellpadding="0" cellspacing="0" class="content3">
					<tr><td>&nbsp;&nbsp;��ǰϵͳ�汾:
						<SCRIPT>document.write( FirwareVerion+';  ��������:'+FirwareDate )</SCRIPT></td></tr>
					<tr><td colspan=2>&nbsp;&nbsp;ע�⣺�������̲��ܹر�·������Դ�����򽫵���·�����𻵶��޷�ʹ�á������ɹ���·�������Զ���������������Լ�����ӣ���Ⱥ�</td></tr>
					<tr><td colspan="2" id="table_lpc_msg"  style="font-size:18px; color:#CC0000">
					</td></tr>
					<tr><td colspan=2>
					<table id=table_lpc bgcolor="#CC0000" height=20  style="display:none">
					<tr>
					  <td></TD>
				    </TR>
					</table> 
					</td></tr>
				</table>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('��¼���ǹ�˾����վ��www.tenda.com.cn�������ظ��߰汾�������<br>\
			&nbsp;&nbsp;&nbsp;&nbsp;ͨ������ҵ���Ӧ�������������<br>\
			&nbsp;&nbsp;&nbsp;&nbsp;�����������������ɺ�·�������Զ�������'
			);</script>

		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table1');
    </script>
</BODY>
</HTML>





