/* 
data = {
  en_stun : false,
  stun_address : '10.10.100.12',
  stun_port : '5060',
  ex_ip_address : '10.207.218.78',
  ex_ip_host : '10.10.200.11',
  ex_refresh_interval : '250',
  local_net_id : '10.10.200.7/255.255.0.0',
  nat_mode : "yes",
  allow_rtp_re_invite : "no"
} */

socket.on("RES_NAT_SETTING", msg => {
    nat_setting_status = false;
    document.querySelector("#Enable_STUN").checked                  = msg.en_stun                      
    document.querySelector("#STUN_Address").value                   = msg.stun_address                 
    document.querySelector("#STUN_Port").value                      = msg.stun_port                
    document.querySelector("#External_IP_Address").value            = msg.ex_ip_address                   
    document.querySelector("#External_IP_Host").value               = msg.ex_ip_host                      
    document.querySelector("#External_Refresh_Interval").value      = msg.ex_refresh_interval             
    document.querySelector("#Local_Network_Identification").value   = msg.local_net_id                    
    document.querySelector("#NAT_Mode").value                       = msg.nat_mode                 
    document.querySelector("#Allow_RTP").value                      = msg.allow_rtp_re_invite             
})
function nat_setting_save(){
    data.nat_setting.en_stun                    = document.querySelector("#Enable_STUN").checked;                               
    data.nat_setting.stun_address               = document.querySelector("#STUN_Address").value;                                
    data.nat_setting.stun_port                  = document.querySelector("#STUN_Port").value;                               
    data.nat_setting.ex_ip_address              = document.querySelector("#External_IP_Address").value;                                 
    data.nat_setting.ex_ip_host                 = document.querySelector("#External_IP_Host").value;                                
    data.nat_setting.ex_refresh_interval        = document.querySelector("#External_Refresh_Interval").value;                               
    data.nat_setting.local_net_id               = document.querySelector("#Local_Network_Identification").value;                                
    data.nat_setting.nat_mode                   = document.querySelector("#NAT_Mode").value;                                
    data.nat_setting.allow_rtp_re_invite        = document.querySelector("#Allow_RTP").value;                                                       
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block'
}