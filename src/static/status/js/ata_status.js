socket.on("RES_SIP", msg =>{
    console.log(msg);
    sip_status_data = msg;
    var table = document.getElementById('sip_status_table');
    if (table.rows.length < 5){
        for (var i = 0; i < sip_status_data.length; i++){
            var row = table.insertRow(-1);
            var cell1 = row.insertCell(0);
            var cell2 = row.insertCell(1);
            var cell3 = row.insertCell(2);
            var cell4 = row.insertCell(3);
            var cell5 = row.insertCell(4);
            var cell6 = row.insertCell(5);
            cell1.innerHTML = i;
            cell2.innerHTML = sip_status_data[i].sip_uri;
            cell4.innerHTML = 'N\A';
            if (sip_status_data[i].status == "checked"){
                cell3.innerHTML = "YES";
                cell3.style="color:#34eb64";
            }
            else {
                cell3.innerHTML = "NO";
            }
        }
    }
    socket.emit("sip_status");
    socket.emit("GET_ATA_STATUS");
    
});
socket.on('config_1_status', msg =>{
    var arr = msg.split("\n");
    var table = document.querySelector("#sip_status_table");
    console.log(arr);
    for (var i = 0 ; i < arr.length - 1; i++){
      let sip_uri = arr[i].slice(arr[i].indexOf('$index:') + 7, arr[i].lastIndexOf('$'));
      let status = arr[i].slice(arr[i].lastIndexOf(':') + 1, arr[i].length);
        for (let j = 1, row; row = table.rows[j]; j++) {
            if (row.cells[1].innerText.includes(sip_uri)){
                row.cells[3].innerText = status;
                if (status.includes('OK')) row.cells[4].innerText = 'Yes';
                else row.cells[4].innerText = 'No';
            }
        }
    }
  });
  socket.on('sip_state', msg =>{
    var arr = [];
    arr = msg.split("$");
    var sip_table = document.getElementById('sip_status_table');
    for (var i = 0; i < 24; i++){
        sip_table.rows[i + 1].cells[5].innerHTML = state_text(arr[i + 1]);
    }
});
socket.on('RES_ATA_STATUS', msg =>{
    console.log(msg);
    var table = document.querySelector("#sip_status_table");
    for (let i = 0; i < msg.length; i++){
        for (let j = 1, row; row = table.rows[j]; j++) {
            if (row.cells[1].innerText.includes(msg[i].sip_uri)){
                row.cells[3].innerText = msg[i].status;
                if (msg[i].status.includes('OK')) row.cells[4].innerText = 'Yes';
                else row.cells[4].innerText = 'No';
            }
        }
    }
});
socket.on('database_change', msg =>{
    var sip_table = document.getElementById('sip_status_table');
    console.log(msg);
    if (msg.status != undefined){
      if (msg.status == "checked"){
          sip_table.rows[parseInt(msg.id) + 1].cells[2].innerHTML = "YES";
          sip_table.rows[parseInt(msg.id) + 1].cells[2].style="color:#34eb64";
      }
      else{
        sip_table.rows[parseInt(msg.id) + 1].cells[2].innerHTML = "NO";
        sip_table.rows[parseInt(msg.id) + 1].cells[2].style="";
      }
    }
    if (msg.sip_uri != undefined){
        sip_table.rows[parseInt(msg.id) + 1].cells[1].innerHTML = msg.sip_uri;
    }
  });
socket.on('call_state', msg =>{
    var sip_table = document.getElementById('sip_status_table');
    var index = msg.slice(msg.indexOf("$") + 1, msg.lastIndexOf("$"));
    var state = msg.slice(msg.lastIndexOf("$") + 1);
    sip_table.rows[parseInt(index) + 1].cells[5].innerHTML = state_text(state);
});
function state_text(state){
    switch(state) {
        case '0':
            return "IDLE";
        case '1':
            return "CALLING";
        case '2':
            return "IN COMING";
        case '3':
            return "EARLY";
        case '4':
            return "CONNECTING";
        case '5':
            return "CONFIRM";
        case '6':
            return "DISCONNECTED";
        default:
            return "IDLE";
    }
}
