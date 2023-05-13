/* var data = {
    en_all_call         : false,
    en_all_call_value   : '*71*',
    dis_all_call        : true,
    dis_all_call_value  : '**71',
    en_busy             : false,
    en_busy_value       : '*81*',
    dis_busy            : true,
    dis_busy_value      : '**81'
  } */
socket.on("RES_CALL_FORWARD", msg =>{
    document.getElementById('en_all_call').checked       = msg.en_all_call              
    document.getElementById('en_all_call_value').value   = msg.en_all_call_value            
    document.getElementById('dis_all_call').checked      = msg.dis_all_call             
    document.getElementById('dis_all_call_value').value  = msg.dis_all_call_value           
    document.getElementById('en_busy').checked           = msg.en_busy              
    document.getElementById('en_busy_value').value       = msg.en_busy_value            
    document.getElementById('dis_busy').checked          = msg.dis_busy             
    document.getElementById('dis_busy_value').value      = msg.dis_busy_value           
})
function save_call_forward(){
    data.call_forward.en_all_call        = document.getElementById('en_all_call').checked;
    data.call_forward.en_all_call_value  = document.getElementById('en_all_call_value').value;
    data.call_forward.dis_all_call       = document.getElementById('dis_all_call').checked;
    data.call_forward.dis_all_call_value = document.getElementById('dis_all_call_value').value;
    data.call_forward.en_busy            = document.getElementById('en_busy').checked;
    data.call_forward.en_busy_value      = document.getElementById('en_busy_value').value;
    data.call_forward.dis_busy           = document.getElementById('dis_busy').checked;
    data.call_forward.dis_busy_value     = document.getElementById('dis_busy_value').value;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}