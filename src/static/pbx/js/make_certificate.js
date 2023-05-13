socket.on("RES_MAKE_CER", msg => {
    make_cer_status = false;
    document.querySelector("#Make_Certificate_Device_ID").value     = msg.device_ip                   
    document.querySelector("#Make_Certificate_Device_Name").value   = msg.device_name                   
    document.querySelector("#make_all").checked                    = msg.make_all      
});
function save_make_cer(){
    data.make_cer.device_ip   = document.querySelector("#Make_Certificate_Device_ID").value;
    data.make_cer.device_name = document.querySelector("#Make_Certificate_Device_Name").value;
    data.make_cer.make_all    = document.querySelector("#make_all").checked;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}