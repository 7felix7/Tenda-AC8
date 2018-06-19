<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>Bandwidth | Control</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<SCRIPT language=JavaScript>
var LANIP = "<%get_tc_othe("lanip");%>";
var maxlist = 10;
var ispUp = "<%get_tc_othe("isp_uprate");%>";
var ispDown = "<%get_tc_othe("isp_downrate");%>";
var check_en = "<%get_tc_othe("tc_en");%>";//�������:0:δ���ã�1:����

//0port,1sip,2eip,3(0:up,1:down),4minrate,5maxrate,6(1:enable,0:disable),7(0:TCP&UDP,1:tcp,2:udp)
//ex:'21,123,124,0,512,512,1,1','65535,111,125,1,123,456,0,2'
//0:�������ж˿�
var resList;

var netip=LANIP.replace(/\.\d{1,3}$/,".");
var editTag = 0;//0:add n:edit
var btnVal = ["������б�","�������б�"];
var pro = ["TCP&UDP","TCP","UDP"];


var Pro_Por_Ser = new Array
(
//0:both;1:tcp;2:udp

         new Array(0,"0","���з���"),
		 new Array(1,"0","���з���(TCP)"),
		 new Array(2,"0","���з���(UDP)"),

         new Array(2,"53","DNS"),

         new Array(1,"21","FTP"),

         new Array(1,"80","HTTP"),

         new Array(1,"8080","HTTP Secondary"),

         new Array(1,"443","HTTPS"),

         new Array(1,"8443","HTTPS Secondary"),

         new Array(2,"69","TFTP"),

         new Array(1,"143","IMAP"),

         new Array(1,"119","NNTP"),

         new Array(1,"110","POP3"),

         new Array(2,"161","SNMP"),

         new Array(1,"25","SMTP"),

         new Array(1,"23","TELNET"),

         new Array(1,"8023","TELNET Secondary"),

         new Array(1,"992","TELNETSSL"),

         new Array(2,"67","DHCP")

)	 		

function init(f)
{
    
	init2();
	//f.elements["up_Band"].value = ispUp;
	//f.elements["down_Band"].value = ispDown;
    	resList = new Array(<%get_tc_list();%>);
	showList();
}
function init2()
{
   if(check_en == 1)
		document.getElementById("check").checked = true;
	else
		document.getElementById("check").checked = false;
	onCheck();
}
//�ͻ��˹����Ƿ�����
function onCheck()
{
	if(document.getElementById("check").checked)
	{
		//document.getElementById("table1").style.display = "";
		document.getElementById("table2").style.display = "";
		document.getElementById("fluxCtlList").style.display = "";
	}
	else
	{
		//document.getElementById("table1").style.display = "none";
		document.getElementById("table2").style.display = "none";
		document.getElementById("fluxCtlList").style.display = "none";
	}
}


function preSubmit(f) 
{
	//var upbw = parseInt(f.up_Band.value);	
	//var downbw = parseInt(f.down_Band.value);
	var upbw = 12800;
	var downbw =12800;
	if(isNaN(upbw) || isNaN(downbw) )
	{
	    alert("WAN�ڴ�����Ϊ��!");
		return;
	}
	if((0 == upbw)||(0 == downbw))
	{
	    alert("������Ϊ0,����������!");
		return;
	}
	var loc = "/goform/trafficForm?GO=net_tc.asp";
	if (document.getElementById("check").checked)
	   loc += "&tc_enable=1";//
	else
	    loc += "&tc_enable=0";  
	loc += "&up_Band=" + upbw;
	loc += "&down_Band=" + downbw;
	loc += "&cur_number=" + resList.length;
	for(var j=0; j<resList.length; j++)
	{
		loc += "&tc_list_" + eval(j+1) + "=" + resList[j] ;
	}

	var code = 'location="' + loc + '"';
	eval(code);
}

function showList()
{
	var m='<table border=1 cellpadding="0" cellspacing="0" cellspacing=0 class=content1 id=showTab>';
	m+='<tr class=item1 align=center height=30>';
	m+='<th nowrap>���</th>';
	//m+='<th nowrap>�˿�</th>';
	m+='<th nowrap>IP��</th>';
	m+='<th nowrap>Ŀ��</th>';
	m+='<th nowrap>����Χ</th>';
	m+='<th nowrap>����</th>';
	m+='<th nowrap>�༭</th>';
	m+='<th nowrap>ɾ��</th>';
	m+='</tr>';
	for (i=0;i<resList.length;i++) {
		var s=resList[i].split(",");
		if (s.length!=8) 
			break;

		m+='<tr align=center>';
		m+='<td>'+eval(i+1)+'</td>';
		//var p = s[0];
		//if ( '0' == p)
		//{p="���ж˿�";}
		//m+='<td>'+pro[s[7]]+'/'+p+'</td>';
		m+='<td>'+netip+s[1]+'~'+s[2]+'</td>';
		m+='<td>'+(parseInt(s[3],10)? "����" : "�ϴ�")+'</td>';//0,�ϴ�,1,����
		m+='<td>'+s[4]+'~'+s[5]+'</td>';
		m+='<td>'+(parseInt(s[6],2) ? "&radic;" : "&times;")+'</td>';
		//m+='<td><a href=Javascript:onEdit('+i+')>�༭</a></td>';
		m+='<td><input type=button class=button  onMouseOver="style.color=\'#FF9933\'" onMouseOut="style.color=\'#000000\'" value="�༭" onclick="onEdit(' + i +  ')"></td>';
		m+='<td><input type=button class=button  onMouseOver="style.color=\'#FF9933\'" onMouseOut="style.color=\'#000000\'" value="ɾ��" onclick="OnDel(this,' + i +  ')"></td>';
		m+='</tr>';
	}
	document.getElementById("fluxCtlList").innerHTML = m;

}

function onAdd()
{
	var f = document.forms[0];
	//var upbw = f.up_Band.value;	
	//var downbw = f.down_Band.value;
	var upbw =12800;
	var downbw =12800;
	
	//if( ((upbw>12800)||(upbw<1) )  || ( (downbw>12800)||(downbw<1)))
	//{
	//    alert("WAN������Чֵ��ΧΪ1~12800,���������룡");
	//	return;
	//}
	
	
	if (resList.length == maxlist && editTag == 0)
	{
	    alert("���ֻ�ܱ���"+maxlist+"��������Ƽ�¼!");
		return;
	}
	
	//var pt = f.servicePort.value;
	var pt = 80;
	var sip = parseInt(f.startIP.value,10);
	var eip = parseInt(f.endIP.value,10);
	if (isNaN(sip)||isNaN(eip))
	{
	    alert("��������ȷ��IP��ַ!");
		return;
	}

	if (!rangeCheck(f.startIP,1,254,"��ʼIP")||
			!rangeCheck(f.endIP,1,254,"����IP")) return ;
	if(eip < sip)
	{
		alert("��ʼIP���ڽ���IP");
		return ;
	}
	
	var linkTy = f.RC_link.value;
	var sbw = parseInt(f.minRate.value);
	var ebw = parseInt(f.maxRate.value);
	if(isNaN(sbw) || isNaN(ebw))
	{
	    alert("��������ȷ�Ĵ���!");
		return;
	}
	if((0 == sbw)||(0 == ebw))
	{
	    alert("������Ϊ0,����������!");
		return;
	}
//huangxiaoli add
	if (!rangeCheck(f.minRate,0,999999,"��ʹ���")||
			!rangeCheck(f.maxRate,0,999999,"��ߴ���")) return ;
//end add
	var en = (f.RC_valid.checked) ? 1 : 0;
	//var pro = f.protocol.value;
	var pro = 1;
	if(pt > 65535 || pt < 0)
	{
		alert("�˿ڳ�����Χ0~65535");
		return ;
	}
	
	if(eip < sip)
	{
		alert("��ʼIP���ڽ���IP");
		return ;
	}
	
	if(ebw < sbw)
	{
		alert("�������뷶Χ����С����");
		return ;
	}
	
   	//if (0 == linkTy)//�ϴ�
   	//{
   	//  if(0 < ebw - upbw)
   	//	  {
	//      alert("���������WAN�ڵ��ϴ�����");
	//	  return;
	// }
  	// }	
  	// else
  	// {
   	//   if(0 < ebw - downbw)
	//  {
	//     alert("���������WAN�ڵ����ش���");
	//	  return;
	// }
   	//}

   	//0port,1sip,2eip,3(0:up,1:down),4minrate,5maxrate,6(1:enable,0:disable),7(0:TCP&UDP,1:tcp,2:udp)
   	
	var allinfo = pt+','+sip+','+eip+','+linkTy+','+sbw+','+ebw+','+en+','+pro;
	var comp1 = allinfo.split(",");
	var comp2;
	for (var k=0;k<resList.length;k++) 
	{
		if(editTag){
			if(k == editTag - 1)
				continue;
		}
		comp2 = resList[k].split(",");
		/*if(comp1[3] == comp2[3]){
				//������ͬ
			if(parseInt(comp1[1])>=parseInt(comp2[1])&&parseInt(comp1[1])<=parseInt(comp2[2])
				|| parseInt(comp1[2])>=parseInt(comp2[1])&&parseInt(comp1[2])<=parseInt(comp2[2])){
				//��ԭ�е�IP��Χ��
					alert("�Ѵ���ͬ����������Ϣ���������õ���Ϣ���ص�������������!");
					return ;
			}
		}*/
		
		if(comp1[3] == comp2[3] && (comp1[0] == comp2[0]) && (comp1[7] == comp2[7])
		   || comp1[3] == comp2[3] && ((parseInt(comp1[0]) == 0 && parseInt(comp2[0]) != 0) || (parseInt(comp1[0]) != 0 && parseInt(comp2[0]) == 0))
		   || comp1[3] == comp2[3] && ((parseInt(comp1[7]) == 0 && parseInt(comp2[7]) != 0) || (parseInt(comp1[7]) != 0 && parseInt(comp2[7]) == 0)))
		{
			if(parseInt(comp1[1])>=parseInt(comp2[1])&&parseInt(comp1[1])<=parseInt(comp2[2])||
			   parseInt(comp1[2])>=parseInt(comp2[1])&&parseInt(comp1[2])<=parseInt(comp2[2]) ||
			   parseInt(comp1[1])<parseInt(comp2[1])&&parseInt(comp1[2])>parseInt(comp2[2]))
			{
				alert("�Ѵ���ͬ����������Ϣ���������õ���Ϣ���ص�������������!");
				return ;
			}   
		}

	}
		
	if(editTag == 0)
	{
		resList[resList.length]=allinfo;
	}
	else
	{
		resList[editTag-1]=allinfo;
		f.add_modify.value  = btnVal[0];
	}
	editTag = 0;
	showList();
}

function onEdit(n)
{
	var f = document.forms[0];
	var s = (resList[n]).split(",");
	//0port,1sip,2eip,3(0:up,1:down),4minrate,5maxrate,6(1:enable,0:disable),7(0:TCP&UDP,1:tcp,2:udp)
	//f.servicePort.value = s[0];
	f.startIP.value 	= s[1];
	f.endIP.value		= s[2];
	f.RC_link.value 	= s[3];
	f.minRate.value 	= s[4];
	f.maxRate.value		= s[5];
	f.RC_valid.checked	= parseInt(s[6],2);
	//f.protocol.value	= s[7];
	editTag = n+1;
	f.add_modify.value  = btnVal[1];
}

function OnDel(obj,dex)
{
	document.getElementById("showTab").deleteRow(dex+1);
	var i=0;
	for(i=dex;i<resList.length;i++)
	{
		resList[i] = resList[eval(i+1)];
	}
	resList.length--;
	document.forms[0].add_modify.value  = btnVal[0];
	showList();
}

function onselPort()
{
	var f = document.forms[0];
	var i1;


   for(i1=0; i1<Pro_Por_Ser.length; i1++)
   {
		      if (i1 == f.elements["selPort"].value )
			  {
				  break;
			  }
		
    }	
	f.elements["protocol"].selectedIndex = parseInt (Pro_Por_Ser[i1][0]);
	f.elements["servicePort"].value = Pro_Por_Ser[i1][1];
	
}

function show_Option_Pro()
{
   var i,in_html;
   for (i=0; i<pro.length; i++)
   {
      in_html += "<option value="+i+">"+pro[i]+"</option>";
   }
   document.write(in_html);
}
function show_Option_Ser()
{
   var i1,in_html='';
   for(i1=0; i1<Pro_Por_Ser.length; i1++)
   {
        
		      in_html += "<option value="+i1+">"+Pro_Por_Ser[i1][2]+"</option>";
	
    }
   document.write(in_html);
}

</SCRIPT>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0" onLoad="init(document.trafficCtl);" class="bg">
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
				<form name=trafficCtl method=POST action=/goform/TrafficCtl>
				<input type=hidden name=GO value=net_tc.asp >
				<table cellpadding="0" cellspacing="0" class="content2"> 
					<tr><td valign="top">���ô������ 
					  <input type=CHECKBOX id="check"  onClick="onCheck()">
					����</td>
					</tr>
				</table>
				<!--
				 <table id="table1" width="80%">
					 <tr >
					  <td width="25%">�ӿ�</td>
					  <td>�ϴ�����&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;���ش���</td>	</tr>
					<tr ><td>WAN:</td>
						<td><input type="text" name="up_Band" size="5" maxlength="5">(KByte/s)&nbsp;<input type="text" name="down_Band" size="5" maxlength="5">(KByte/s)</td>
						<td></td></tr>
					<tr><td height="6" colspan="3"><hr></td></tr>
				</table> 
				-->
				
				<table id="table2" cellpadding="0" cellspacing="0" class="content1" style="margin-top:0px;">
				<!--
					<tr>
						<td rowspan="2">����:</td>
						<td>Э��&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;�˿�&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp����</td>
					<tr>
					<td>
					<select name="protocol"  style= "width:80">
					<script>show_Option_Pro();</script>
					</select>&nbsp;
					<input type="text" name="servicePort" size="5" maxlength="5" value="0">&nbsp;&lt;��
					<select name="selPort" onChange="onselPort()" style= "width:120" >
					<script>show_Option_Ser();</script>
					</select>	 
					</td>
					</tr>
					<tr></tr>
				-->
				   <tr >
					<td align="right">IP��ַ:</td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;<script>document.write(netip);</script>
						<input type="text" name="startIP" size="3" maxlength="3"><b>~</b>
						<input type="text" name="endIP" size="3" maxlength="3"></td></tr>
				   <tr>
					<td width="25%" align="right" >���ٷ���:</td>
					<td width="75%">&nbsp;&nbsp;&nbsp;&nbsp;<select name="RC_link">
					  <option value="0">�ϴ�</option>
					  <option value="1">����</option>
					</select></td>
				   </tr>
				  <tr><td width="25%" align="right">����Χ:</td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type="text" name="minRate" maxlength="5" size="5" >~
						<input type="text" name="maxRate" maxlength="5" size="5">
						(KByte/s)</td>
				  </tr>
				  <tr>
					<td width="25%" align="right">����:</td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="RC_valid" value="1"> </td></tr>
					<tr><td height="30" colspan='3' align='center'>
					<INPUT  class=button2 type=button value="������б�" onMouseOver="style.color='#FF9933'" onMouseOut="style.color='#000000'" name="add_modify"onClick="onAdd()">
					</td>
					</tr>
				</table>  
				
				<div id="fluxCtlList" style="position:relative;visibility:visible;"></div>  
				  
				<SCRIPT>tbl_tail_save("document.trafficCtl");</SCRIPT>
				
				</FORM>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo('������ܿ����������������������ͨ��������<br>\&nbsp;&nbsp;&nbsp;&nbsp;���ٷ�������WAN�ڵ��ϴ���������ش���<br>\&nbsp;&nbsp;&nbsp;&nbsp;����Χ��ָ��IP��Χ�ڵ������ϴ�/������С�����������������λKB/s ���ϴ�/���ص����޲��ó���WAN�ڵ����޷�Χ��<br><br><br>');</script>

		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table2');
    </script>
</BODY>
</HTML>





