<HTML> 
<HEAD>
<META http-equiv="Pragma" content="no-cache">
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>���߻�������1</title>
<SCRIPT language=JavaScript src="gozila.js"></SCRIPT>
<SCRIPT language=JavaScript src="menu.js"></SCRIPT>
<SCRIPT language=JavaScript src="table.js"></SCRIPT>
<script language="JavaScript" type="text/javascript">

var ssid000 = "<% get_wireless_basic("SSID"); %>";
var ssid111 = "<% get_wireless_basic("SSID1"); %>";

var WirelessWorkMode;
var wdsList;
var PhyMode;
var broadcastssidEnable;
var ap_isolate;
var channel_index;
var countrycode;
var ht_bw;
var ht_extcha;
var enablewireless;
var mode;
var wmmCapable;
var APSDCapable;
var SSID0;
var SSID1;
var wireless11bchannels;
var wireless11gchannels;
var wl0_mode;

var request = GetReqObj();
ChannelList_24G = new Array(14);
ChannelList_24G[0] = "2412MHz (Channel 1)";
ChannelList_24G[1] = "2417MHz (Channel 2)";
ChannelList_24G[2] = "2422MHz (Channel 3)";
ChannelList_24G[3] = "2427MHz (Channel 4)";
ChannelList_24G[4] = "2432MHz (Channel 5)";
ChannelList_24G[5] = "2437MHz (Channel 6)";
ChannelList_24G[6] = "2442MHz (Channel 7)";
ChannelList_24G[7] = "2447MHz (Channel 8)";
ChannelList_24G[8] = "2452MHz (Channel 9)";
ChannelList_24G[9] = "2457MHz (Channel 10)";
ChannelList_24G[10] = "2462MHz (Channel 11)";
ChannelList_24G[11] = "2467MHz (Channel 12)";
ChannelList_24G[12] = "2472MHz (Channel 13)";
ChannelList_24G[13] = "2484MHz (Channel 14)";
function init()
{
    //if(WirelessWorkMode==0)
//	{
//	  document.wireless_basic.WirelessT[0].checked = true;
//	  changeWds(0);
//	  initWds();
//	}
//	else if(WirelessWorkMode==1)
//	{
//	 document.wireless_basic.WirelessT[1].checked = true;
//	 changeWds(1);
//	 initWds();
//	}
	//if(enablewireless==1){
		//document.getElementById("enablewireless").checked=true;
    	//document.getElementById("divwieless").style.display="block";
		initValue();
	//}else{
		//document.getElementById("enablewireless").checked=false;
		//document.getElementById("divwieless").style.display="none";
		//initValue();
	//}
	if(mode==1 || wl0_mode == "sta"){
		document.getElementById("sz11bChannel").disabled=true;
		document.getElementById("sz11gChannel").disabled=true;
		//document.getElementById("enablewireless").disabled=true;
	}
	if(wl0_mode == "sta"){
		//������apclientʱ������wds��
		//document.wireless_basic.WirelessT[1].disabled = true;
		//document.getElementById("enablewireless").disabled=true;
		document.wireless_basic.broadcastssid[1].disabled = true;
		document.wireless_basic.ssid.disabled = true;
	}
}

//+++++++++++stanley
function showChannel()
{
	var idx,bstr,gstr;
	if(channel_index == 0){
	 	bstr='&nbsp;&nbsp;&nbsp;&nbsp;<select id="sz11bChannel" name="sz11bChannel" size="1" onChange="ChannelOnChange()"><option value=0 selected>AutoSelect</option>';
	 	gstr='&nbsp;&nbsp;&nbsp;&nbsp;<select id="sz11gChannel" name="sz11gChannel" size="1" onChange="ChannelOnChange()"><option value=0 selected>AutoSelect</option>';
	 }
	else{
		bstr='&nbsp;&nbsp;&nbsp;&nbsp;<select id="sz11bChannel" name="sz11bChannel" size="1" onChange="ChannelOnChange()"><option value=0>AutoSelect</option>';
	 	gstr='&nbsp;&nbsp;&nbsp;&nbsp;<select id="sz11gChannel" name="sz11gChannel" size="1" onChange="ChannelOnChange()"><option value=0>AutoSelect</option>'; 
	}
	 if(wireless11bchannels == 11)
	{
		  for (idx = 0; idx < 11; idx++)
		  {
			   if(channel_index==idx  + 1)
			   {
			    bstr+='<option value='+(idx+1)+' selected>'+(2412+5*idx)+'MHz (Channel '+(idx+1)+')</option>';
			    gstr+='<option value='+(idx+1)+' selected>'+(2412+5*idx)+'MHz (Channel '+(idx+1)+')</option>';
			   }
			   else
			   {
			    bstr+='<option value='+(idx+1)+'>'+(2412+5*idx)+'MHz (Channel '+(idx+1)+')</option>';
			    gstr+='<option value='+(idx+1)+'>'+(2412+5*idx)+'MHz (Channel '+(idx+1)+')</option>';
			   }
		  }
	}
	else if(wireless11bchannels == 13)
	{
		  for (idx = 0; idx < 13; idx++)
		  {
			   if(channel_index==idx  + 1)
			   {
			    bstr+='<option value='+(idx+1)+' selected>'+(2412+5*idx)+ 'MHz (Channel '+(idx+1)+')</option>';
			    gstr+='<option value='+(idx+1)+' selected>'+(2412+5*idx)+ 'MHz (Channel '+(idx+1)+')</option>';
			   }
			   else
			   {
			    bstr+='<option value='+(idx+1)+'>'+(2412+5*idx)+ 'MHz (Channel '+(idx+1)+')</option>';
			    gstr+='<option value='+(idx+1)+'>'+(2412+5*idx)+ 'MHz (Channel '+(idx+1)+')</option>';
			   }
		  }
	}
	else if(wireless11bchannels == 14)
	{
		  for (idx = 0; idx < 13; idx++)
		  {
			   if(channel_index ==idx + 1)
			   {
			    bstr+='<option value='+(idx+1)+' selected>'+(2412+5*idx)+ 'MHz (Channel '+(idx+1)+')</option>';
			    gstr+='<option value='+(idx+1)+' selected>'+(2412+5*idx)+ 'MHz (Channel '+(idx+1)+')</option>';
			   }
			   else
			   {
			    bstr+='<option value='+(idx+1)+'>'+(2412+5*idx)+ 'MHz (Channel '+(idx+1)+')</option>';
			    gstr+='<option value='+(idx+1)+'>'+(2412+5*idx)+ 'MHz (Channel '+(idx+1)+')</option>';
			   }
		  }
		  if(channel_index==14)
		  {
			   bstr=+'<option value=14 selected>2484MHz (Channel 14)</option>';
			   gstr=+'<option value=14 selected>2484MHz (Channel 14)</option>';
		  }
		  else
		  {
			   bstr=+'<option value=14>2484MHz (Channel 14)</option>';
			   gstr=+'<option value=14>2484MHz (Channel 14)</option>';
		  }
	}
	bstr+='</select>';
	gstr+='</select>';
	document.getElementById("11b").innerHTML=bstr;
	document.getElementById("11g").innerHTML=gstr;
}

var http_request2 = false;
function makeRequest2(url, content) {
	http_request2 = false;
	if (window.XMLHttpRequest) { // Mozilla, Safari,...
		http_request2 = new XMLHttpRequest();
		if (http_request2.overrideMimeType) {
			http_request2.overrideMimeType('text/xml');
		}
	} else if (window.ActiveXObject) { // IE
		try {
			http_request2 = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
			http_request2 = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
	if (!http_request2) {
		alert('Giving up :( Cannot create an XMLHTTP instance');
		return false;
	}
	http_request2.onreadystatechange = alertContents2;
	http_request2.open('POST', url, true);
	http_request2.send(content);
}
function alertContents2() {
	if (http_request2.readyState == 4) {
		if (http_request2.status == 200) {
		 	var str=http_request2.responseText.split("\r");
			WirelessWorkMode = str[0];
			wdsList = str[1];
			PhyMode = str[2];
			broadcastssidEnable = str[3];
			channel_index = str[4];
			countrycode = str[5];
			ht_bw = str[6];
			ht_extcha = str[7];
			enablewireless = str[8];
			mode = str[9];
			wmmCapable = str[10];
			APSDCapable = str[11];
			SSID0 = ssid000;//str[12];
			SSID1 = ssid111;//str[13];
			wireless11bchannels = str[14];
			wireless11gchannels = str[15];
			ap_isolate = str[16];
			wl0_mode = str[17];
			showChannel();
			init();
		} else {
		}
	}
}

function initbasic(){
		makeRequest2("/goform/wirelessInitBasic", "something");
}
//++++++++++++
/*function initWds()
{
 var wdslistArray;
 if (wdsList != "")
	{
		wdslistArray = wdsList.split(" ");
		for(i = 1; i <= wdslistArray.length; i++)
			eval("document.wireless_basic.wds_"+i).value = wdslistArray[i - 1];
	}
 document.getElementById("wdsScanTab").style.display = "none";
 document.getElementById("wlSurveyBtn").value="����ɨ��";
}*/


/*function SurveyClose(){
	var tbl = document.getElementById("wdsScanTab").style;
	if (tbl.display == ""){
		tbl.display = "none";
		document.getElementById("wlSurveyBtn").value="����ɨ��";
	}else{
		document.body.className = "onwdsscan";
		tbl.display="";
		document.getElementById("wlSurveyBtn").value="�ر�ɨ��";
	    var code="/goform/WDSScan";
		request.open("GET", code, true);
		request.onreadystatechange = RequestRes;
		request.setRequestHeader("If-Modified-Since","0");
		request.send(null);			
	}	
}*/

function RequestRes()
{
	if (request.readyState == 4)
	{
		document.body.className = "bg";
		initScan(request.responseText);
	}
}


/*function initScan(scanInfo)
{	
	if(scanInfo != '')
	{
	    var iserror=scanInfo.split("\n");
		if(iserror[0]!="stanley")
		{
			var str1 = scanInfo.split("\r");
			var len = str1.length;
			document.getElementById("wdsScanTab").style.display = "";
			document.getElementById("wlSurveyBtn").value="�ر�ɨ��";
			
			var tbl = document.getElementById("wdsScanTab").getElementsByTagName('tbody')[0];
	
			var maxcell = tbl.rows.length;
			for (var j = maxcell; j > 1; j --)
			{
				tbl.deleteRow(j - 1);
			}	
		
			var cont = parseInt(len);
			for (i = 0; i < cont; i ++)
			{
				var str = str1[i].split("\t");
				nr=document.createElement('tr');
				nc=document.createElement('td');
				nr.appendChild(nc);
	
				nc.innerHTML = "<input type=\"radio\" name=\"wlsSlt\" value=\"radiobutton\" onclick=\"macAcc()\"/>";
				
				nc=document.createElement('td');
				nr.appendChild(nc);
				nc.innerHTML = str[0];
				
				nc=document.createElement('td');
				nr.appendChild(nc);
				nc.innerHTML = str[1];
			
				nc=document.createElement('td');
				nr.appendChild(nc);
				nc.innerHTML = str[2];
				
				nc=document.createElement('td');
				nr.appendChild(nc);
				nc.innerHTML = str[3];
				
				nc=document.createElement('td');
				nr.appendChild(nc);
				nc.innerHTML = str[4];
				
				nr.align = "center";
				tbl.appendChild(nr);
			}
		}
	}
	else
	{
		document.getElementById("wdsScanTab").style.display = "none";
		document.getElementById("wlSurveyBtn").value="����ɨ��";
	}
}
var max_wds_num = 2;
function macAcc()
{
	if(!confirm("ȷ����Ҫ���Ӵ�AP��"))
	{
		return ;
	}
	var tbl = document.getElementById("wdsScanTab");
	var Slt = document.wireless_basic.wlsSlt;
	var mac,sc,ssid,schannel;
	var maxcell = tbl.rows.length;
	
	for (var r = maxcell; r > 1; r --)
	{
		if (maxcell == 2)
			sc = Slt.checked;
		else
			sc = Slt[r - 2].checked;
		if (sc)
		{
			mac = tbl.rows[r - 1].cells[2].innerHTML;
			ssid = decodeSSID(tbl.rows[r - 1].cells[1].innerHTML);
			schannel = tbl.rows[r - 1].cells[3].innerHTML;
			if(checkRepeat((mac)) == false)
				return;
			for(var k=1;k<=max_wds_num;k++)
			{   
				if(document.forms[0].elements["wds_"+k].value == "")	
				{
					document.forms[0].elements["wds_"+k].value = mac;
					document.forms[0].elements["ssid_"+k].value = ssid;
					document.forms[0].elements["schannel_"+k].value = schannel;
//��SSID ���ŵ��ĳɺͶԷ�һ��
					document.forms[0].elements["ssid"].value = ssid;
					document.forms[0].elements["sz11bChannel"].options.value = schannel;
					document.forms[0].elements["sz11gChannel"].options.value = schannel;
					ChannelOnChange();
					
					return true;
				}
				if(k == max_wds_num)
				{
					alert("AP MAC����,�������Ҫ����Ϣ");
					return ;
				}
			}
		}
	}
}

function checkRepeat(mac)
{
	for(var i=1;i<=max_wds_num;i++)
	{
		if(mac.toUpperCase() == document.forms[0].elements["wds_"+i].value.toUpperCase())
		{
			alert("�˵�ַ�Ѵ���!");
			return false;
		}
	}
}*/
/*function onenablewirelesschange(){
	if(document.getElementById("enablewireless").checked){
		document.getElementById("divwieless").style.display="block";
	}else{
		document.getElementById("divwieless").style.display="none";
	}
}
*/
function style_display_on()
{
	if (window.ActiveXObject)
	{ // IE
		return "block";
	}
	else if (window.XMLHttpRequest)
	{ // Mozilla, Safari,...
		return "table-row";
	}
}

function insertChannelOption(vChannel, band)
{
	var y = document.createElement('option');

	if (1*band == 24)
	{
		y.text = ChannelList_24G[1*vChannel - 1];
		y.value = 1*vChannel;
	}

	if (1*band == 24)
		var x=document.getElementById("sz11gChannel");

	try
	{
		x.add(y,null); 
	}
	catch(ex)
	{
		x.add(y); 
	}
}

function CreateExtChannelOption(vChannel)
{
	var y = document.createElement('option');

	y.text = ChannelList_24G[1*vChannel - 1];
	y.value = 1;
	var x = document.getElementById("n_extcha");
	try{
		x.add(y,null); 
	}
	catch(ex){
		x.add(y); 
	}
}

function insertExtChannelOption()
{
	var wmode = document.wireless_basic.wirelessmode.options.selectedIndex;
	var option_length; 
	var CurrentCh;
	if ((1*wmode == 3))//5->3
	{
		var x = document.getElementById("n_extcha");
		var length = document.wireless_basic.n_extcha.options.length;

		if (length > 1)
		{
			x.selectedIndex = 1;
			x.remove(x.selectedIndex);
		}
		
		if (1*wmode == 3)//3->4
		{
			CurrentCh = document.wireless_basic.sz11gChannel.value;
			option_length = document.wireless_basic.sz11gChannel.options.length;

			if ((CurrentCh >=1) && (CurrentCh <= 4))
			{
				x.options[0].text = ChannelList_24G[1*CurrentCh + 4 - 1];
				x.options[0].value = 1*CurrentCh + 4;
			}
			else if ((CurrentCh >= 5) && (CurrentCh <= 7))
			{
				x.options[0].text = ChannelList_24G[1*CurrentCh - 4 - 1];
				x.options[0].value = 0; //1*CurrentCh - 4;
				CurrentCh = 1*CurrentCh;
				CurrentCh += 4;
				CreateExtChannelOption(CurrentCh);
			}
			else if ((CurrentCh >= 8) && (CurrentCh <= 9))
			{
				x.options[0].text = ChannelList_24G[1*CurrentCh - 4 - 1];
				x.options[0].value = 0; //1*CurrentCh - 4;

				if (option_length >=14)
				{
					CurrentCh = 1*CurrentCh;
					CurrentCh += 4;
					CreateExtChannelOption(CurrentCh);
				}
			}
			else if (CurrentCh == 10)
			{
				x.options[0].text = ChannelList_24G[1*CurrentCh - 4 - 1];
				x.options[0].value = 0; //1*CurrentCh - 4;

				if (option_length > 14)
				{
					CurrentCh = 1*CurrentCh;
					CurrentCh += 4;
					CreateExtChannelOption(CurrentCh);
				}

			}
			else if (CurrentCh >= 11)
			{
				x.options[0].text = ChannelList_24G[1*CurrentCh - 4 - 1];
				x.options[0].value = 0; //1*CurrentCh - 4;
			}
			else
			{
				x.options[0].text = "Auto Select";
				x.options[0].value = 0;
			}
		}
	}
}

function ChannelOnChange()
{
	if (document.wireless_basic.n_bandwidth[1].checked == true)
	{
		var w_mode = document.wireless_basic.wirelessmode.options.selectedIndex;
		
		if (1*w_mode == 3)//5->3
		{
			if (document.wireless_basic.n_bandwidth[1].checked == true)
			{
				document.wireless_basic.n_extcha.disabled = false;
			}

			if (document.wireless_basic.sz11gChannel.options.selectedIndex == 0)
				document.wireless_basic.n_extcha.disabled = true;
		}
	}

	insertExtChannelOption();
}

function Channel_BandWidth_onClick()
{
	var w_mode = document.wireless_basic.wirelessmode.options.selectedIndex;

	if (document.wireless_basic.n_bandwidth[0].checked == true)
	{
		document.wireless_basic.n_extcha.disabled = true;
		if (1*w_mode == 3)//4->3
			Check5GBandChannelException();
	}
	else
	{
		document.wireless_basic.n_extcha.disabled = false;

		if (1*w_mode == 3)//4->3
		{
			Check5GBandChannelException();
		}
	}
}
function initValue()
{
	var ssidArray;
	var broadcastssidArray;
	var ap_isolateArray;
	var channel_11a_index;
	var current_channel_length;

	if (countrycode == '')
		countrycode = 'NONE';
	document.getElementById("div_11b_channel").style.visibility = "hidden";
	document.getElementById("div_11b_channel").style.display = "none";
	document.wireless_basic.sz11bChannel.disabled = true;
	document.getElementById("div_11g_channel").style.visibility = "hidden";
	document.getElementById("div_11g_channel").style.display = "none";
	document.wireless_basic.sz11gChannel.disabled = true;
	document.getElementById("div_11n").style.display = "none";
	document.wireless_basic.n_bandwidth.disabled = true;
    	document.getElementById("ssid").value = decodeSSID(SSID0);
	document.getElementById("mssid_1").value = decodeSSID(SSID1);
	PhyMode = 1*PhyMode;

	if (PhyMode >= 8)
	{
		document.getElementById("div_11n").style.display = "block";
		document.wireless_basic.n_bandwidth.disabled = false;
	}

	if ((PhyMode == 0) || (PhyMode == 4) || (PhyMode == 9))
	{
		if (PhyMode == 0)
			document.wireless_basic.wirelessmode.options.selectedIndex = 0;
		else if (PhyMode == 4)
			document.wireless_basic.wirelessmode.options.selectedIndex = 2;
		else if (PhyMode == 9)
			document.wireless_basic.wirelessmode.options.selectedIndex = 3;//5->3

		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display ="";
		document.wireless_basic.sz11gChannel.disabled = false;
	}
	else if (PhyMode == 1)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 1;
		document.getElementById("div_11b_channel").style.visibility = "visible";
		document.getElementById("div_11b_channel").style.display = "";
		document.wireless_basic.sz11bChannel.disabled = false;
	}
	else if ((PhyMode == 2) || (PhyMode == 8))
	{
		if (PhyMode == 8)
			document.wireless_basic.wirelessmode.options.selectedIndex = 3;//4->3
	}
	if (wmmCapable == "on")
	{
		document.wireless_basic.wmm_capable[0].checked = true;
		document.wireless_basic.wmm_capable[1].checked = false;
	}
	else
	{
		document.wireless_basic.wmm_capable[0].checked = false;
		document.wireless_basic.wmm_capable[1].checked = true;
	}
	document.getElementById("div_apsd_capable").style.visibility = "hidden";
	document.getElementById("div_apsd_capable").style.display = "none";
	document.wireless_basic.apsd_capable.disabled = true;

	if (wmmCapable == "on")
	{
		document.getElementById("div_apsd_capable").style.visibility = "visible";
		document.getElementById("div_apsd_capable").style.display = style_display_on();
		document.wireless_basic.apsd_capable.disabled = false;
	}
	if (APSDCapable == "on")
	{
		document.wireless_basic.apsd_capable[0].checked = true;
		document.wireless_basic.apsd_capable[1].checked = false;
	}
	else
	{
		document.wireless_basic.apsd_capable[0].checked = false;
		document.wireless_basic.apsd_capable[1].checked = true;
	}
	broadcastssidArray = broadcastssidEnable.split(";");

	if (1*broadcastssidArray[0] == 0)
		document.wireless_basic.broadcastssid[0].checked = true;
	else
		document.wireless_basic.broadcastssid[1].checked = true;
  
	ap_isolateArray = ap_isolate.split(";");

	if (1*ap_isolateArray[0] == 0)
		document.wireless_basic.ap_isolate[1].checked = true;
	else
		document.wireless_basic.ap_isolate[0].checked = true;
	
	if (1*ht_bw == 0)
	{
		document.wireless_basic.n_bandwidth[0].checked = true;
		document.wireless_basic.n_extcha.disabled = true;
	}
	else
	{
		document.wireless_basic.n_bandwidth[1].checked = true;
		document.wireless_basic.n_extcha.disabled = false;
	}

	channel_index = 1*channel_index;

	if ((PhyMode == 0) || (PhyMode == 4) || (PhyMode == 9))
	{
		document.wireless_basic.sz11gChannel.options.selectedIndex = channel_index;

		current_channel_length = document.wireless_basic.sz11gChannel.options.length;

		if ((channel_index + 1) > current_channel_length)
			document.wireless_basic.sz11gChannel.options.selectedIndex = 0;
	}
	else if (PhyMode == 1)
	{
		document.wireless_basic.sz11bChannel.options.selectedIndex = channel_index;

		current_channel_length = document.wireless_basic.sz11bChannel.options.length;

		if ((channel_index + 1) > current_channel_length)
			document.wireless_basic.sz11bChannel.options.selectedIndex = 0;
	}
	else if ((PhyMode == 2) || (PhyMode == 8))
	{
		if (countrycode == 'NONE')
		{
			if (channel_index <= 64)
			{
				channel_11a_index = channel_index;
				channel_11a_index = channel_11a_index / 4;
				if (channel_11a_index != 0)
					channel_11a_index = channel_11a_index - 8;
			}
			else if ((channel_index >= 100) && (channel_index <= 140))
			{
				channel_11a_index = channel_index;
				channel_11a_index = channel_11a_index / 4;
				channel_11a_index = channel_11a_index - 16;
			}
			else if (channel_index >= 149)
			{
				channel_11a_index = channel_index - 1;
				channel_11a_index = channel_11a_index / 4;
				channel_11a_index = channel_11a_index - 17;

				if (document.wireless_basic.n_bandwidth[1].checked == true)
				{
					channel_11a_index = channel_11a_index - 1;
				}
			}
			else
			{
				channel_11a_index = 0;
			}
		}
		else if ((countrycode == 'US') || (countrycode == 'HK') || (countrycode == 'FR') || (countrycode == 'IE'))
		{
			if (channel_index <= 64)
			{
				channel_11a_index = channel_index;
				channel_11a_index = channel_11a_index / 4;
				if (channel_11a_index != 0)
					channel_11a_index = channel_11a_index - 8;
			}
			else if (channel_index >= 149)
			{
				channel_11a_index = channel_index - 1;
				channel_11a_index = channel_11a_index / 4;
				channel_11a_index = channel_11a_index - 28;
			}
			else
			{
				channel_11a_index = 0;
			}
		}
		else if (countrycode == 'JP')
		{
			if (channel_index <= 48)
			{
				channel_11a_index = channel_index;
				channel_11a_index = channel_11a_index / 4;
				if (channel_11a_index != 0)
					channel_11a_index = channel_11a_index - 8;
			}
			else
			{
				channel_11a_index = 0;
			}
		}
		else if (countrycode == 'TW')
		{
			if (channel_index <= 64)
			{
				channel_11a_index = channel_index;
				channel_11a_index = channel_11a_index / 4;
				if (channel_11a_index != 0)
					channel_11a_index = channel_11a_index - 12;
			}
			else if (channel_index >= 149)
			{
				channel_11a_index = channel_index - 1;
				channel_11a_index = channel_11a_index / 4;
				channel_11a_index = channel_11a_index - 32;
			}
			else
			{
				channel_11a_index = 0;
			}
		}
		else if (countrycode == 'CN')
		{
			if (channel_index >= 149)
			{
				channel_11a_index = channel_index - 1;
				channel_11a_index = channel_11a_index / 4;
				channel_11a_index = channel_11a_index - 36;
			}
			else
			{
				channel_11a_index = 0;
			}
		}
		else
		{
			channel_11a_index = 0;
		}

		Check5GBandChannelException();
	}
	insertExtChannelOption();
	var option_length = document.wireless_basic.n_extcha.options.length;
	if(ht_extcha == 'upper')
	{
		if (option_length > 1)
			document.wireless_basic.n_extcha.options.selectedIndex = 0;
	}
	else if(ht_extcha == 'lower' )
	{
		if (option_length > 1)
			document.wireless_basic.n_extcha.options.selectedIndex = 1;
	}
	else
	{
		document.wireless_basic.n_extcha.options.selectedIndex = 0;
	}

	if (1*PhyMode == 8)
	{
		document.wireless_basic.n_extcha.disabled = true;
	}
	else if (1*PhyMode == 9)
	{
		if (document.wireless_basic.sz11gChannel.options.selectedIndex == 0)
			document.wireless_basic.n_extcha.disabled = true;
	}

	
}
/*function changeWds(x)
{
	if(x==0)
	{
		document.getElementById("wdsMode").style.display = "none";
	}
	else if(x==1)
	{
		 document.getElementById("wdsMode").style.display = "";
	}
}*/
function wmm_capable_enable_switch()
{
	document.getElementById("div_apsd_capable").style.visibility = "hidden";
	document.getElementById("div_apsd_capable").style.display = "none";
	document.wireless_basic.apsd_capable.disabled = true;

	if (document.wireless_basic.wmm_capable[0].checked == true)
	{
		document.getElementById("div_apsd_capable").style.visibility = "visible";
		document.getElementById("div_apsd_capable").style.display = style_display_on();
		document.wireless_basic.apsd_capable.disabled = false;
	}
}
function wirelessModeChange()
{
	var wmode;
	document.getElementById("div_11b_channel").style.visibility = "hidden";
	document.getElementById("div_11b_channel").style.display = "none";
	document.wireless_basic.sz11bChannel.disabled = true;
	document.getElementById("div_11g_channel").style.visibility = "hidden";
	document.getElementById("div_11g_channel").style.display = "none";
	document.wireless_basic.sz11gChannel.disabled = true;
	document.getElementById("div_11n").style.display = "none";
	document.wireless_basic.n_bandwidth.disabled = true;

	wmode = document.wireless_basic.wirelessmode.options.selectedIndex;

	wmode = 1*wmode;
	if (wmode == 0)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 0;
		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display = "";
		document.wireless_basic.sz11gChannel.disabled = false;
	}
	else if (wmode == 1)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 1;
		document.getElementById("div_11b_channel").style.visibility = "visible";
		document.getElementById("div_11b_channel").style.display = "";
		document.wireless_basic.sz11bChannel.disabled = false;
	}
	else if (wmode == 2)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 2;
		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display = "";
		document.wireless_basic.sz11gChannel.disabled = false;
	}
	else if (wmode == 3)//5->3
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 3;
		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display = "";
		document.wireless_basic.sz11gChannel.disabled = false;
		document.getElementById("div_11n").style.display = "block";
		document.wireless_basic.n_bandwidth.disabled = false;

		if (document.wireless_basic.sz11gChannel.options.selectedIndex == 0)
			document.wireless_basic.n_extcha.disabled = true;

		insertExtChannelOption();
	}
	if(mode==1 || wl0_mode == "sta"){
		document.getElementById("sz11bChannel").disabled=true;
		document.getElementById("sz11gChannel").disabled=true;
	}
}
function CheckValue()
{
    var wireless_mode;
	var submit_ssid_num;
	var channel_11a_index;
	//var re =/^[\w`~!@#$^*()\-+={}\[\]\|:'<>.?/]+$/;	
	var sid = document.wireless_basic.ssid.value;
	var sid1 = document.wireless_basic.mssid_1.value;

	if(!checkSSID(sid))
	{
		alert("��SSID��Ч��������1-32��ASCII��!");
		document.wireless_basic.ssid.focus();
		document.wireless_basic.ssid.select();
		return false;
	}
	if(wl0_mode == "sta")
	{
		if(!checkSSID(sid1)){
			alert("��SSID��Ч��������1-32��ASCII��!");
			document.wireless_basic.mssid_1.focus();
			document.wireless_basic.mssid_1.select();
			return false;
		}
	}else{
		if(sid1 != "" && !checkSSID(sid1)){
			alert("��SSID��Ч��������1-32��ASCII��!");
			document.wireless_basic.mssid_1.focus();
			document.wireless_basic.mssid_1.select();
			return false;
		}
	}

	submit_ssid_num = 1;

	document.wireless_basic.bssid_num.value = submit_ssid_num;
	/*var all_wds_list;
	all_wds_list = '';
	if (document.wireless_basic.WirelessT[1].checked==true)
	{
		var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
		for (i = 1; i <= max_wds_num; i++)
		{
			if (eval("document.wireless_basic.wds_"+i).value == "")
				continue;
		var mac1=eval("document.wireless_basic.wds_"+i).value;
    	 if( (mac1.charAt(1) == "1") || (mac1.charAt(1) == "3") ||
				 (mac1.charAt(1) == "5") || (mac1.charAt(1) == "7") ||
				 (mac1.charAt(1) == "9") || (mac1.charAt(1).toLowerCase() == "b")|| 
				 (mac1.charAt(1).toLowerCase() == "d") || (mac1.charAt(1).toLowerCase() == "f") )
   		 	{
      		  	alert("�����뵥��MAC��ַ.");
       		 	return false;
   			}
		 
		 for (j=1; j<i; j++)
		 { 
		 	if( eval("document.wireless_basic.wds_"+i).value == "")
				continue ;
			
		 	if( eval("document.wireless_basic.wds_"+i).value == eval("document.wireless_basic.wds_"+j).value )
			{	alert("����������ͬ��MAC��ַ!");
				return false;
		 	}
		 }
			if (!re.test(eval("document.wireless_basic.wds_"+i).value)) {
				alert("��������ȷ��MAC��ַ��");
				return false;
			}
			else {
				all_wds_list += eval("document.wireless_basic.wds_"+i).value;
				all_wds_list += ' ';
			}
		}
		if (all_wds_list == "")
		{
			alert("AP MAC��ַΪ�� !!!");
			document.wireless_basic.wds_1.focus();
			document.wireless_basic.wds_1.select(); 
			return false;
		}
		else
		{
			document.wireless_basic.wds_list.value = all_wds_list;
		}
		
	}*/

	return true;
}

function preSubmit(f)
{  
   var fm = document.forms[0];
  /* if(fm.elements["enablewireless"].checked)
	  fm.elements["enablewirelessEx"].value = "1";
   else
      fm.elements["enablewirelessEx"].value = "0";*/
	
   if (true==CheckValue())
      {
	     f.submit();
	  }
   
}

</script>
<style type="text/css">
.style2 {color: #FF0000}
.onwdsscan {cursor: wait}
</style>
<link rel=stylesheet type=text/css href=style.css>
</HEAD>

<BODY leftMargin=0 topMargin=0 MARGINHEIGHT="0" MARGINWIDTH="0" onLoad="initbasic();" class="bg">
	<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
      <tr>
        <td width="33">&nbsp;</td>
        <td width="679" valign="top">
		<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" height="100%">
          <tr>
            <td align="center" valign="top"><table width="98%" border="0" align="center" cellpadding="0" cellspacing="0" height="100%">
                <tr>
                  <td align="center" valign="top">
					<form method=post name=wireless_basic action="/goform/wirelessBasic" >
					<!--<input type="hidden" id="rebootTag" name="rebootTag">-->
					<input type="hidden" id="GO" name="GO" value="wireless_basic.asp">
					<!--<table border="0" cellpadding="0" cellspacing="0" class="content2">
					<tr><td width="30%" valign="top"><input type="checkbox" name="enablewireless"  value="1" id="enablewireless"onClick="onenablewirelesschange()">�������߹���
					  <input type="hidden" name="enablewirelessEx" /></td><td valign="top">&nbsp;</td>
					</tr>
					</table>-->
					<!--<div id="divwieless" style="display:none; margin:0px; padding:0px;">-->
					<table class="content1" id="table1" cellpadding="0" cellspacing="0">
					  <!--<tr>
					    <td width="100" align="right">���߹���ģʽ</td>
					    <td>&nbsp;&nbsp;&nbsp;&nbsp;<INPUT name=WirelessT type=radio value=0 onClick="changeWds(0);">���߽����(AP)&nbsp;<INPUT name=WirelessT type=radio value=1 onClick="changeWds(1);" style="display:none">����(WDS)</td>
					    </tr>-->
					  <tr> 
						<td width="100" align="right">����ģʽ</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<select name="wirelessmode" id="wirelessmode" size="1" onChange="wirelessModeChange()">
                          <option value=0>11b/g���ģʽ</option>
                          <option value=1>11bģʽ</option>
                          <option value=4>11gģʽ</option>
                          <option value=9>11b/g/n���ģʽ</option>
                        </select></td>
					  </tr>
					  <input type="hidden" name="bssid_num" value="1">
					  <tr> 
						<td width="100" align="right" class="head">��SSID</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type=text name=ssid id=ssid size=20 maxlength=32 value=></td>
					  </tr>
					  <tr> 
						<td width="100" align="right" class="head">��SSID</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type=text name=mssid_1 id=mssid_1 size=20 maxlength=32 value=""></td>
					  </tr>
					  <tr> 
						<td width="100" align="right" class="head">�㲥(SSID)</td>
						<td>
						  &nbsp;&nbsp;&nbsp;&nbsp;<input type=radio name=broadcastssid value="0" checked>����&nbsp;
						  <input type=radio name=broadcastssid value="1">�ر�    </td>
					  </tr>

					  <tr> 
						<td width="100" align="right" class="head">AP ����</td>
						<td>
						  &nbsp;&nbsp;&nbsp;&nbsp;<input type=radio name=ap_isolate value="1" checked>����&nbsp;
						  <input type=radio name=ap_isolate value="0">�ر�    </td>
					  </tr>

					  <tr id="div_11b_channel" name="div_11b_channel" style="visibility:visible;">
					  <td width="100" align="right" class="head">�ŵ�</td>
					  <td>
					  <div id="11b">
					  </div></td>
					  </tr>
					  <tr id="div_11g_channel" name="div_11g_channel" style="visibility:visible;">
						<td width="100" align="right">�ŵ�</td>
						<td>
						<div id="11g"></div></td>
					  </tr>
					</table>
					<div id="div_11n">
					<hr style="width:75%;">
					<table name="div_11n" id="table2" cellspacing="0" cellpadding="0" class="content1" style="margin-top:0px;">
					  <tr>
						<td width="100" align="right" class="head">�ŵ�����</td>
						<td>
						  &nbsp;&nbsp;&nbsp;&nbsp;<input type=radio name=n_bandwidth value="0" onClick="Channel_BandWidth_onClick()" checked>20&nbsp;
						  <input type=radio name=n_bandwidth value="1" onClick="Channel_BandWidth_onClick()">20/40						</td>
					  </tr>
					  <tr>
						<td width="100" align="right" class="head">��չ�ŵ�</td>
						<td>
						  &nbsp;&nbsp;&nbsp;&nbsp;<select id="n_extcha" name="n_extcha" size="1">
						<option value=1 selected>2412MHz (Channel 1)</option>
						  </select>						</td>
					  </tr>
					</table>
					</div>
					<div id="div_wmm">
					<hr style="width:75%;">
					<table name="div_wmm" id="table4" cellspacing="0" cellpadding="0" class="content1" style="margin-top:0px;">
<!-- move to wireless_adv.html-->
					    <tr> 
						<td width="100" align="right">WMM Capable</td>
						<td>
						  &nbsp;&nbsp;&nbsp;&nbsp;<input type=radio name=wmm_capable value="on" onClick="wmm_capable_enable_switch()" checked>���� &nbsp;
						  <input type=radio name=wmm_capable value="off" onClick="wmm_capable_enable_switch()">�ر�
						</td>
					  </tr>
					  <tr id="div_apsd_capable" name="div_apsd_capable">
						<td width="100" align="right">APSD Capable</td>
						<td>
						  &nbsp;&nbsp;&nbsp;&nbsp;<input type=radio name=apsd_capable value="on">���� &nbsp;
						  <input type=radio name=apsd_capable value="off" checked>�ر�
						</td>
					  </tr>
<!---->
					</table>
					</div>
					<!--<div id="wdsMode">
					<hr style="width:75%;">
					<table class="content3" cellpadding="0" cellspacing="0">
					  <tr>
					    <td colspan=2 bgcolor="#9D9D9D"><b>����ģʽ: WDS��Repeaterģʽ��</b></td>
					  </tr>
					</table>
					<div style="width:75%; height:1px; background-color:#c0c7cd; overflow:hidden; padding:0px; margin-top:1px;"></div>	
					  <table cellpadding="0" cellspacing="0" class="content3" id="table3">
					  <tr id="wds_mac_list_1" name="wds_mac_list_1" style="visibility:visible;">
						<td width="100" align="right" class="head">AP MAC��ַ</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type=text name=wds_1 size=20 maxlength=17 value=""><input type="hidden" name="ssid_1"><input type="hidden" name="schannel_1"></td>
					  </tr>
					  <tr id="wds_mac_list_2" name="wds_mac_list_2" style="visibility:visible;">
						<td width="100" align="right" class="head">AP MAC��ַ</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type=text name=wds_2 size=20 maxlength=17 value=""><input type="hidden" name="ssid_2"><input type="hidden" name="schannel_2"></td>
					  </tr>
					  <tr id="wds_mac_list_3" name="wds_mac_list_3" style="visibility:hidden;">
						<td width="100" align="right" class="head">AP MAC��ַ</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type=text name=wds_3 size=20 maxlength=17 value=""><input type="hidden" name="ssid_3"><input type="hidden" name="schannel_3"></td>
					  </tr>
					  <tr id="wds_mac_list_4" name="wds_mac_list_4" style="visibility:hidden;">
						<td width="100" align="right" class="head">AP MAC��ַ</td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;<input type=text name=wds_4 size=20 maxlength=17 value=""><input type="hidden" name="ssid_4"><input type="hidden" name="schannel_4"></td>
					  </tr>
					  <tr><td colspan=2>
					<span class="style2"><b>ע�⣺</b>
					ѡ������(WDS)ģʽʱ����SSID���ŵ����Զ����óɺͶԶ�AP��һ����ͬʱ������ʹ��WEP���ܷ�ʽ��������ߺͶԶ�AP�ļ����ԡ�</span><br>
					</td></tr>
					  <input type="hidden" name="wds_list" value="1">
					</table>
						<input name="wlSurveyBtn" id="wlSurveyBtn" type="button" class="button" onClick="SurveyClose()" value="�ر�ɨ��" />
						  <table width="442" id="wdsScanTab" class="content3">
							<tr bgcolor="#CCCCCC">
							  <td width="10%"><div align="center">ѡ��</div></td>
							  <td width="20%"><div align="center">SSID</div></td>
							  <td width="30%"><p align="center">MAC��ַ</p></td>
							  <td width="10%"><div align="center">�ŵ�</div></td>
							  <td width="15%"><div align="center">��ȫ</div></td>
							  <td width="15%"><div align="center">�ź�ǿ��</div></td>
							</tr>
						  </table>
					</div>-->
					<table id="div_11n_plugfest" name="div_11n_plugfest" width=80% cellspacing="1" cellpadding="3">
					  <tr> 
						<td class="vline" colspan="2"></td>
					  </tr>
					</table> 
				<!--	</div> -->
						<SCRIPT>
							tbl_tail_save("document.wireless_basic");
						</SCRIPT> 
					</form> 
				  </td>
                </tr>
            </table></td>
          </tr>
        </table></td>
        <td align="center" valign="top" height="100%">
		<script>helpInfo("��ҳ��ֻҪ�����߻�����Ϣ�������ã�����ֻ����SSID���ŵ�������ѡ���Ĭ�ϡ�<br>\
			&nbsp;&nbsp;&nbsp;&nbsp;SSID�����������������豸������������ơ�<br>\
			&nbsp;&nbsp;&nbsp;&nbsp;SSID�㲥�������߿ͻ����ڱ����������Ҫ��������������ʱ�����ǽ�ͨ��·�������SSID�㲥�� ���ѡ�У�·�����������е����������㲥�Լ���SSID�š�<br>\
			&nbsp;&nbsp;&nbsp;&nbsp;�ŵ���������ѡ�������б����κ�һ���ŵ��������Զ�ģʽ��������ѡ��ǰ����ʹ�ñȽ��ٵ��ŵ��Ա�����š�<br>\
			&nbsp;&nbsp;&nbsp;&nbsp;��չ�ŵ��� ����ȷ��11nģʽʱ�����繤����Ƶ�ʶΡ�<br>\
			"
		);</script>		
		</td>
      </tr>
    </table>
	<script type="text/javascript">
	  table_onload('table1');
	  table_onload('table2');
	 // table_onload1('table3');
	 table_onload('table4');
    </script>
</BODY>
</HTML>





