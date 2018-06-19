<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<TITLE>System | Time Settings</TITLE>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<SCRIPT language=JavaScript>
addCfg("HN",0,"ROUTER");
addCfg("DN",1,"DOMAIN.COM");

var tz = "<%aspTendaGetStatus("sys","timezone");%>";//ʱ��
var time = "<%aspTendaGetStatus("sys","manualTime");%>";
var mode = "<%aspTendaGetStatus("sys","timeMode");%>";//0:�ر��ֶ����� 1�������ֶ�����

function init(f)
{
/*
	var t = time.split("-");
	f.year.value = t[0];
	f.month.value = t[1];
	f.day.value = t[2];
	f.hour.value = t[3];
	f.minute.value = t[4];
	f.second.value = t[5];
*/
	var today = new Date();
	f.year.value = today.getFullYear();
	f.month.value = today.getMonth()+1;
	f.day.value = today.getDate();
	f.hour.value = today.getHours();
	f.minute.value = today.getMinutes();
	f.second.value = today.getSeconds();

	if(mode == 2)
	{
		f.manualEN.checked = true;	
	}
	else
	{		
		f.manualEN.checked = false;
	}
	
	f.TZ.selectedIndex = parseInt(tz);
	onManualSet();
}

function preSubmit(f) {
	if (!strCheck(f.HN,"������")) return;
	if (!strCheck(f.DN,"����")) return;
	var loc = "/goform/SysToolTime?GO=system_hostname.asp";

	if(f.manualEN.checked == true)
	{
		var sy = f.year.value;
		var smo = f.month.value;
		var sd = f.day.value;
		var sh = f.hour.value;
		var smi = f.minute.value;
		var ss = f.second.value;
		var t = /^[0-9]{1,4}$/;
		
		if(!t.test(sy) || !t.test(smo) || !t.test(sd) || !t.test(sh) || !t.test(smi) || !t.test(ss))
		{
			alert("ʱ���ʽ����");
			return false;
		}
		
		if(sy < 1971 || sy > 2037)
		{
			alert("ʱ�����ó�����Χ��");
			return false;
		}

		if(sy.length<4 || Number(sy)<1971 || Number(sy)>2999 || Number(smo)>12 || Number(smo)<1 || smo.length<1 || sd.length<1 || Number(sd)<1)
		{
			alert("������Ϸ���������");
			return ;
		}
		if(Number(smo) == 2)//2��
		{
		    if(Number(sy)%400==0||(Number(sy)%4==0&&Number(sy)%100!=0))
			{
				if(Number(sd) > 29)
				{
					alert("������Ϸ�����");
					return ;
				}
			}
			else if(Number(sd) > 28)
			{
			     alert("������Ϸ�����");
				 return ;
			}
		}
		
		else if(Number(smo)==4 || Number(smo)==6 || Number(smo)==9 || Number(smo)==11)//4.6.9.11
		{
			if(Number(sd)>30)
			{
				alert("������Ϸ�����");
				return ;
			}
		}
		else//1.3.5.7.8.10.12
		{
			if(Number(sd)>31)
			{
				alert("������Ϸ�����");
				return ;
			}
		}
		
		if(Number(sh)>23 || Number(smi)>59 || Number(ss)>59)
		{
			alert("������Ϸ���ʱ��");
			return ;
		}
		if(sh == "")
			sh = "00";
		if(smi == "")
			smi = "00";
		if(ss == "")
			ss = "00";
		
		loc += "&manualEN=2";
		//loc += "&time=" + sy + "-" + smo + "-" + sd + "-" + sh + "-" + smi + "-" + ss;
		var curdate = new Date(Date.UTC(sy, smo-1 ,sd, sh, smi,ss));
		var computer = parseInt(curdate.getTime()/1000) ;
		loc += "&time=" + computer;
	}
	else
	{
		loc += "&manualEN=0";
		loc += "&time=";
	}
	loc += "&TZ=" + f.TZ.value;
	//loc += "&TZSel=" + f.TZ.selectedIndex;
	
	var code = 'location="' + loc + '"';
	eval(code);
}

//ѡ������ʱ��
function onManualSet()
{
	var f = document.frmSetup;
	if(f.manualEN.checked == true)
	{
		document.getElementById("setTab").disabled = false;
		document.getElementById("year").disabled = document.getElementById("month").disabled = document.getElementById("day").disabled = document.getElementById("hour").disabled = 
		document.getElementById("minute").disabled = document.getElementById("second").disabled = document.getElementById("year").disabled = document.getElementById("year").disabled = false;
	}
	else
	{
		document.getElementById("setTab").disabled = true;
		document.getElementById("year").disabled = document.getElementById("month").disabled = document.getElementById("day").disabled = document.getElementById("hour").disabled = 
		document.getElementById("minute").disabled = document.getElementById("second").disabled = document.getElementById("year").disabled = document.getElementById("year").disabled = true;
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
				<form name=frmSetup id="frmSetup" method=POST action=/goform/SysToolTime>
				<input type=hidden id=GO value="system_hostname.asp">
				<table cellpadding="0" cellspacing="0" class="content1" id="table1">
				
					<tr><td height="30">&nbsp;&nbsp;ʱ���� 
					  <select class="list" id="TZ">
										<option value="0">��GMT-12��00���������ڱ��������</option>                               
										<option value="1">��GMT-11��00����;������Ħ��Ⱥ��</option>                              
										<option value="2">��GMT-10��00�������ģ���ū³³</option>                                
										<option value="3">��GMT-09��00������˹��</option>                                        
										<option value="4">��GMT-08��00��̫ƽ��ʱ�䣨�����ͼ��ô�;�ٻ���</option>               
										<option value="5">��GMT-07��00��ɽ��ʱ�䣨�����ͼ��ô�</option>                        
										<option value="6">��GMT-07��00�������ߣ�����˹����������</option>                        
										<option value="7">��GMT-07��00������ɣ��</option>                                        
										<option value="8">��GMT-06��00���в�ʱ�䣨�����ͼ��ô�</option>                        
										<option value="9">��GMT-06��00���ϴ���������ī����ǣ�������</option>              
										<option value="10">��GMT-05��00������ʱ�䣨�����ͼ��ô�</option>                 
										<option value="11">��GMT-05��00��ӡ�ذ��ǣ�����</option>                               
										<option value="12">��GMT-05��00���������������</option>                           
										<option value="13">��GMT-04��00��������ʱ�䣨���ô�</option>                         
										<option value="14">��GMT-04��00�����˹</option>                                       
										<option value="15">��GMT-04��00��������˹������˹</option>                             
										<option value="16">��GMT-03��30��Ŧ��������������</option>                             
										<option value="17">��GMT-03��00����������</option>                                     
										<option value="18">��GMT-03��00��������</option>                                       
										<option value="19">��GMT-03��00������ŵ˹����˹�����ζ�</option>                       
										<option value="20">��GMT-02��00���������в�</option>                                   
										<option value="21">��GMT-01��00�����ٶ�Ⱥ��</option>                                   
										<option value="22">��GMT-01��00����ý�Ⱥ��</option>                               
										<option value="23">��GMT���������α�׼ʱ�䣺�����֣�����������˹�����׶�</option>      
										<option value="24">��GMT������������������ά��</option>                                
										<option value="25">��GMT+01:00�����������£�������˹������������˹��¬�������ǣ�������</option>
										<option value="26">��GMT+01:00���������ѣ�˹�������ɳ�������ղ�</option>            
										<option value="27">��GMT+01:00����³�������籾��������������</option>              
										<option value="28">��GMT+01:00�����з�</option>                                        
										<option value="29">��GMT+01:00����ķ˹�ص������֣������ᣬ����˹�¸��Ħ��άҲ��</option>
										<option value="30">��GMT+02:00���ŵ䣬������˹�أ���˹̹����</option>                  
										<option value="31">��GMT+02:00����˹��</option>                                        
										<option value="32">��GMT+02:00������</option>                                          
										<option value="33">��GMT+02:00���ն���������������ӣ������ǣ����֣�ά��Ŧ˹</option>    
										<option value="34">��GMT+02:00��Ү·����</option>                                      
										<option value="35">��GMT+02:00���µúͿ�</option>                                  
										<option value="36">��GMT+02:00�������ף�����������</option>                          
										<option value="37">��GMT+03:00��Ī˹�ƣ�ʥ�˵ñ��������Ӹ���</option>                
										<option value="38">��GMT+03:00�������أ����ŵ�</option>                              
										<option value="39">��GMT+03:00���͸��</option>                                      
										<option value="40">��GMT+03:00�����ޱ�</option>                                      
										<option value="41">��GMT+03:30���º���</option>                                      
										<option value="42">��GMT+04:00���������ȣ���˹����</option>                          
										<option value="43">��GMT+04:00���Ϳ�</option>                                        
										<option value="44">��GMT+04:00��������</option>                                      
										<option value="45">��GMT+04:00���ڱ���˹</option>                                  
										<option value="46">��GMT+04:30��������</option>                                    
										<option value="47">��GMT+05:00����˹�����������棬��ʲ��</option>                  
										<option value="48">��GMT+05:00��Ҷ�����ձ�</option>                                
										<option value="49">��GMT+05:30�����Σ��Ӷ����������µ���</option>              
										<option value="50">��GMT+05:45���ӵ�����</option>                                    
										<option value="51">��GMT+06:00����˹���ɣ��￨</option>                            
										<option value="52">��GMT+06:00������ľͼ������������</option>                      
										<option value="53">��GMT+06:00��������</option>                                      
										<option value="54">��GMT+06:30�����⣨���⣩</option>                              
										<option value="55">��GMT+07:00������˹ŵ�Ƕ�˹��</option>                            
										<option value="56">��GMT+07:00�����ȣ����ڣ��żӴ�</option>                          
										<option value="57">��GMT+08:00�����������죬����ر�����������³ľ��</option>        
										<option value="58">��GMT+08:00��������οˣ���������</option>                        
										<option value="59">��GMT+08:00����¡�£��¼���</option>                                
										<option value="60">��GMT+08:00����˹</option>                                        
										<option value="61">��GMT+09:00���ſ�Ŀ�</option>                                    
										<option value="62">��GMT+09:00�����棬���ϣ�����</option>                          
										<option value="63">��GMT+09:00���׶�</option>                                        
										<option value="64">��GMT+09:30�������</option>                                      
										<option value="65">��GMT+09:30����������</option>                                    
										<option value="66">��GMT+10:00������˹��</option>                                    
										<option value="67">��GMT+10:00����������ī������Ϥ��</option>                      
										<option value="68">��GMT+10:00���ص���Ī���ȱȸ�</option>                            
										<option value="69">��GMT+10:00����������˹�п�</option>                              
										<option value="70">��GMT+10:00��������</option>                                      
										<option value="71">��GMT+11:00����ӵ���������Ⱥ�����¿����</option>              
										<option value="72">��GMT+12:00��쳼�Ⱥ��������Ӱ뵺�����ܶ�Ⱥ��</option>          
										<option value="73">��GMT+12:00���¿����������</option>                            
										<option value="74">��GMT+13:00��Ŭ�Ⱒ�巨</option>                                
					</select>
					</td></tr>
					</table>
				<table cellpadding="0" cellspacing="0" class="content3">
					<tr><td height="30">&nbsp;&nbsp;��ע�⣺�������ϻ���������ܻ�ȡGMTʱ�䡣��</td>
					</tr>
				</table>
				<table cellpadding="0" cellspacing="0" class="content3" id="table2">
					<tr><td height="30">&nbsp;&nbsp;�Զ���ʱ�䣺
					  <input type="checkbox" id="manualEN" onClick="onManualSet()">
					<tr><td height="30" id="setTab">
						&nbsp;&nbsp;<input type="text" class="text" id="year" size="4" maxlength="4">��
						<input type="text" class="text" id="month" size="2" maxlength="2">��
						<input type="text" class="text" id="day" size="2" maxlength="2">��
						<input type="text" class="text" id="hour" size="2" maxlength="2">ʱ
						<input type="text" class="text" id="minute" size="2" maxlength="2">��
						<input type="text" class="text" id="second" size="2" maxlength="2">��
					</td>
					</tr>
				</table>
					
					<SCRIPT>tbl_tail_save("document.frmSetup");</SCRIPT>
					
				</FORM>
				</td>
              </tr>
            </table></td>
          </tr>
        </table></td>
        <td width="264" align="center" valign="top" height="100%">
		<script>helpInfo('��ҳ����·������ϵͳʱ�䣬������ѡ���Լ�����ʱ����ߴӻ������ϻ�ȡ��׼��GMTʱ�䣬Ȼ��ϵͳ���Զ�����NTP����������ʱ��ͬ����<br>&nbsp;&nbsp;&nbsp;&nbsp;ע�⣺�ر�·������Դ��ʱ����Ϣ�ᶪʧ�������´ο�������Internet��·���������Զ���ȡGMTʱ�䡣������������Internet��ȡGMTʱ��򵽴�ҳ����ʱ����������ܣ������ǽ���е�ʱ���޶�������Ч��'
		);</script>

		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table1');
	  table_onload('table2');
    </script>
</BODY>
</HTML>





