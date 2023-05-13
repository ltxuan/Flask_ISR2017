
/* var data = {
    remote_party_id : '',
    allow_guest : 'yes',
    pedantic : 'yes',
    alway_auth : 'yes',
    session_timers : 'default',
    session_expires : '',
    session_minse : '',
    session_refresher : 'default'    
  } */

socket.on("RES_ADVANCED_SETTING", msg => {
  if (msg.remote_party_id == 'send') {
    document.querySelector("#trust").checked = false;
    document.querySelector("#send").checked = true;
  }
  else if(msg.remote_party_id == 'trust') {
    document.querySelector("#trust").checked = true;
    document.querySelector("#send").checked = false;
  }
  document.querySelector("#Allow_Guest").value       = msg.allow_guest                              
  document.querySelector("#Pedantic").value          = msg.pedantic                              
  document.querySelector("#Alwaysauthreject").value  = msg.alway_auth                              
  document.querySelector("#Session-timers").value    = msg.session_timers                           
  document.querySelector("#Session-expires").value   = msg.session_expires                          
  document.querySelector("#Session-minse").value     = msg.session_minse                            
  document.querySelector("#Session-refresher").value = msg.session_refresher                        
})
function advanced_setting_save(){
    if (!document.querySelector("#send").checked && !document.querySelector("#trust").checked) data.advanced_setting.remote_party_id = '';
    else data.advanced_setting.remote_party_id   = document.querySelector("#send").checked?'send':'trust';                        
    data.advanced_setting.allow_guest       =  document.querySelector("#Allow_Guest").value;           
    data.advanced_setting.pedantic          =  document.querySelector("#Pedantic").value;      
    data.advanced_setting.alway_auth        =  document.querySelector("#Alwaysauthreject").value;
    data.advanced_setting.session_timers    =  document.querySelector("#Session-timers").value;               
    data.advanced_setting.session_expires   =  document.querySelector("#Session-expires").value;               
    data.advanced_setting.session_minse     =  document.querySelector("#Session-minse").value;               
    data.advanced_setting.session_refresher =  document.querySelector("#Session-refresher").value;  
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';                     
}
function togle_checkbox(el){
  if (el.checked){
    if (el.id == 'send') document.getElementById('trust').checked = false;
    if (el.id == 'trust') document.getElementById('send').checked = false;
  }
}