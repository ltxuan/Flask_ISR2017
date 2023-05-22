socket.on("RES_FX_STATUS", msg =>{
  console.log(msg);
    let table = document.querySelector("#table_FXS_status");
    for (let i = 0; i < msg.length; i++){
        table.rows[i + 1].cells[1].innerText = msg[i].status;
        table.rows[i + 1].cells[2].innerText = msg[i].value;
    }
})
socket.on("fxs", str =>{
let index = str.slice(str.indexOf("$index:") + "$index:".length, str.indexOf("$status:"));
  let status = str.slice(str.indexOf("$status:") + "$status:".length, str.indexOf("$value:"));
  if (status == '0') status = 'YES';
  else status = "NO";
  let value =   str.slice(str.indexOf("$value:") + "$value:".length).trim() + ' V';
  table.rows[index + 1].cells[1].innerText = status;
  table.rows[index + 1].cells[2].innerText = value;
})
