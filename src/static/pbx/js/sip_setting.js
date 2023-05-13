/* data = {
  udp_port : "5060",
  en_tcp : false,
  tcp_port : "5060",
  en_tls : false,
  tls_verify_server : "no",
  tls_verify_client : "no",
  tls_client_method : "tslv2",
  rtp_port_start : '1000',
  rtp_port_end : "25000",
  dtmf_mode : "inband",
  max_regis : "3600",
  min_regis : "60",
  regis_time : "130",
  regis_attempts : "10",
  regis_timeout : "20",
  video_support : "no",
  dns_lookup : "no",
  user_agent : "xyz"
} */
socket.on("RES_GENERAL_SETTING", msg => {
    general_setting_status = false;
    document.querySelector("#UDP_Port").value                   = msg.udp_port            
    document.querySelector("#tcp_enable").checked               = msg.en_tcp            
    document.querySelector("#TCP_Port").value                   = msg.tcp_port
    document.querySelector("#TLS_Port").value                   = msg.tls_port          
    document.querySelector("#tls_enable").checked               = msg.en_tls            
    document.querySelector("#TLS_Verify_Server").value          = msg.tls_verify_server 
    document.querySelector("#TLS_Verify_Client").value          = msg.tls_verify_client 
    document.querySelector("#TLS_Client_Method").value     = msg.tls_client_method 
    document.querySelector("#RTP_Port_Start").value             = msg.rtp_port_start    
    document.querySelector("#RTP_Port_End") .value              = msg.rtp_port_end        
    document.querySelector("#DTMF_Mode").value                  = msg.dtmf_mode         
    document.querySelector("#Max_Registration").value           = msg.max_regis         
    document.querySelector("#Min_Registration").value           = msg.min_regis         
    document.querySelector("#Default_Incoming").value           = msg.regis_time        
    document.querySelector("#Register_Attempts").value          = msg.regis_attempts    
    document.querySelector("#Register_Timeout").value           = msg.regis_timeout     
    document.querySelector("#Video_Support").value              = msg.video_support     
    document.querySelector("#DNS_SRV_Look_Up").value            = msg.dns_lookup        
    document.querySelector("#User_Agent").value                 = msg.user_agent        
})
function save_general_setting(){
    data.general_setting.udp_port           = document.querySelector("#UDP_Port").value;
    data.general_setting.en_tcp             = document.querySelector("#tcp_enable").checked;
    data.general_setting.tcp_port           = document.querySelector("#TCP_Port").value;
    data.general_setting.en_tls             = document.querySelector("#tls_enable").checked;
    data.general_setting.tls_port           = document.querySelector("#TLS_Port").value;
    data.general_setting.tls_verify_server  = document.querySelector("#TLS_Verify_Server").value;
    data.general_setting.tls_verify_client  = document.querySelector("#TLS_Verify_Client").value;
    data.general_setting.tls_client_method  = document.querySelector("#TLS_Client_Method").value;
    data.general_setting.rtp_port_start     = document.querySelector("#RTP_Port_Start").value;
    data.general_setting.rtp_port_end       = document.querySelector("#RTP_Port_End").value;
    data.general_setting.dtmf_mode          = document.querySelector("#DTMF_Mode").value;
    data.general_setting.max_regis          = document.querySelector("#Max_Registration").value;
    data.general_setting.min_regis          = document.querySelector("#Min_Registration").value;
    data.general_setting.regis_time         = document.querySelector("#Default_Incoming").value;
    data.general_setting.regis_attempts     = document.querySelector("#Register_Attempts").value;
    data.general_setting.regis_timeout      = document.querySelector("#Register_Timeout").value;
    data.general_setting.video_support      = document.querySelector("#Video_Support").value;
    data.general_setting.dns_lookup         = document.querySelector("#DNS_SRV_Look_Up").value;
    data.general_setting.user_agent         = document.querySelector("#User_Agent").value;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}