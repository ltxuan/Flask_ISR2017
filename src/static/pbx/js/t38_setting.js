/* var data = {
    re_invite : 'yes',
    err_correct : 'fec',
    max_data : '400'
  } */
socket.on("RES_T38_SETTING", msg => {
    document.querySelector("#Re-invite_SDP").value    = msg.re_invite   
    document.querySelector("#Error_Correction").value = msg.err_correct     
    document.querySelector("#T38_Max_BitRate").value  = msg.max_data        
});
function t38_setting_save(){
    data.t38_setting.re_invite   = document.querySelector("#Re-invite_SDP").value;
    data.t38_setting.err_correct = document.querySelector("#Error_Correction").value;
    data.t38_setting.max_data    = document.querySelector("#T38_Max_BitRate").value;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}