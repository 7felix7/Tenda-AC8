<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>LAN | LAN Settings</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<SCRIPT language=JavaScript>
	

addCfg("LANIP",0,"<%aspTendaGetStatus("lan","lanip");%>");
addCfg("LANMASK",1,"<%aspTendaGetStatus("lan","lanmask");%>");

function same_net_with_lan(f)
{
	var new_mask = f.LANMASK.value.split(".");
	var new_ip = f.LANIP.value.split(".");

	var old_mask = getCfg("LANMASK").split(".");
	var old_ip = getCfg("LANIP").split(".");

	var i_new_mask = new_mask[0]<<24|new_mask[1]<<16|new_mask[2]<<8|new_mask[3];
	var i_new_ip = new_ip[0]<<24|new_ip[1]<<16|new_ip[2]<<8|new_ip[3];

	var i_old_mask = old_mask[0]<<24|old_mask[1]<<16|old_mask[2]<<8|old_mask[3];
	var i_old_ip = old_ip[0]<<24|old_ip[1]<<16|old_ip[2]<<8|old_ip[3];

	if((i_new_mask&i_new_ip) == (i_old_mask&i_old_ip))
		return true;
	else
		return false;
}

function init(f)
{	
	cfg2Form(f);
}

function preSubmit(f) {  
	var str="";
	if (!verifyIP2(f.LANIP,"IP ��ַ")) return ;
	//������֧���������֣�������ۼ��ж�
	if ( !tenda_ipMskChk(f.LANMASK,"��������", f.LANIP)) return ;	
	
	if(!same_net_with_lan(f)){
		str+="·����IP��ַ�����ѷ����ı䣬������������ز�����";
	}

	form2Cfg(f);
	
	str+="���ҳ��û���Զ�ˢ�£�����������Ե��������ã�Ȼ�����µ�IP��½��";
	if (window.confirm(str))
	{
		f.submit();				
	}else
		return;
}

</SCRIPT>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0" onLoad="init(LanSet);" class="bg">
	<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
      <tr>
        <td width="33">&nbsp;</td>
        <td width="679" valign="top">
		<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" height="100%">
          <tr>
            <td align="center" valign="top"><table width="98%" border="0" align="center" cellpadding="0" cellspacing="0" height="100%">
                <tr>
                  <td align="center" valign="top"><form name=LanSet method=POST action=/goform/AdvSetLanip>
                      <input type=hidden name=GO value=lan.asp> 
					  <table cellpadding="0" cellspacing="0" class="content2">                       
					    <tr>
                          <td colspan=2 align="left" valign="top">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ��ҳ����LAN�ڵĻ������������</td>
                        </tr>
					  </table>
                      <table cellpadding="0" cellspacing="0" class="content1" id="table1" style="margin-top:0px;">
                        <tr>
                          <td width=100 align="right">MAC ��ַ</td>
                          <td width=272><%aspTendaGetStatus("sys","lanmac");%></td>
                        </tr>
                        <tr>
                          <td width="100" align="right">IP��ַ</td>
                          <td><input class=text maxlength=15 name=LANIP size=15 ></td>
                        </tr>
                        <tr>
                          <td width="100" align="right">��������</td>
                          <td><input class=text maxlength=15 name=LANMASK size=15></td>
                        </tr>
                      </table>
                    <script>tbl_tail_save("document.LanSet");</script>
                  </form></td>
                </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('����·������LAN IP��ַ���������롣Ĭ��IP��ַ�� 192.168.0.1��Ĭ������������ 255.255.255.0��'
		);</script>		
		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table1');
    </script>
</BODY>
</HTML>





