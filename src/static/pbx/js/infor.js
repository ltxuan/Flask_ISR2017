socket.on("RES_INFOR", msg =>{
    infor_status = false;
    document.querySelector("#node_id").value       = msg.node_id     
    document.querySelector("#who_apply").value     = msg.who_apply     
    document.querySelector("#who_use").value       = msg.who_use    
    document.querySelector("#date_apply").value    = msg.date_apply       
    document.querySelector("#number_infor").value  = msg.number_infor         
    document.querySelector("#other_infor").value   = msg.other_infor         
});
function save_infor(){
    data.infor.node_id = document.querySelector("#node_id").value;
    data.infor.who_apply = document.querySelector("#who_apply").value;
    data.infor.who_use = document.querySelector("#who_use").value;
    data.infor.date_apply = document.querySelector("#date_apply").value;
    data.infor.number_infor = document.querySelector("#number_infor").value;
    data.infor.other_infor = document.querySelector("#other_infor").value;  
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}