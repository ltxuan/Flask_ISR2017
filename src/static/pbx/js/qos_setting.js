/* var data = {
    tos_sip : 'cs2',
    cos_sip : '3',
    tos_audio : 'cs2',
    cos_audio : '4',
    tos_video : 'cs2',
    cos_video : '3',
  } */
socket.on("RES_QOS_SETTING", msg => {    
    qos_setting_status =false;   
    document.querySelector("#Tos_SIP").value   = msg.tos_sip     
    document.querySelector("#Cos_SIP").value   = msg.cos_sip     
    document.querySelector("#Tos_Audio").value = msg.tos_audio   
    document.querySelector("#Cos_Audio").value = msg.cos_audio   
    document.querySelector("#Tos_Video").value = msg.tos_video   
    document.querySelector("#Cos_Video").value = msg.cos_video   
})
function qos_setting_save(){
    data.qos_setting.tos_sip    = document.querySelector("#Tos_SIP").value;                  
    data.qos_setting.cos_sip    = document.querySelector("#Cos_SIP").value;
    data.qos_setting.tos_audio  = document.querySelector("#Tos_Audio").value;
    data.qos_setting.cos_audio  = document.querySelector("#Cos_Audio").value;            
    data.qos_setting.tos_video  = document.querySelector("#Tos_Video").value;           
    data.qos_setting.cos_video  = document.querySelector("#Cos_Video").value;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';            
}               