socket.on("RES_DASHBOARD", msg =>{
    console.log(msg);
    document.querySelector("#arm_ready").innerText = msg.arm_ready;
    document.querySelector("#fpga_ready").innerText = msg.fpga_ready;
    document.querySelector("#v3\\.3").innerText = msg.ok_3v;
    document.querySelector("#v5").innerText = msg.ok_5v;
    document.querySelector("#temp").innerText = msg.temp + ' oC';
    document.querySelector("#hump").innerText = msg.humd + ' %';
    document.querySelector("#cpu_temp").innerText = msg.cpu_temp;
    var table = document.querySelector("#content-dashboard__left--item2");
    table.rows[2].cells[1].innerText = msg.arr[0];
    table.rows[3].cells[1].innerText = msg.arr[1];
    table.rows[4].cells[1].innerText = msg.arr[2];
    table.rows[5].cells[1].innerText = msg.arr[3];
    table.rows[8].cells[1].innerText = msg.arr[4];
    table.rows[9].cells[1].innerText = msg.arr[5];
    table.rows[10].cells[1].innerText = msg.arr[6];
    table.rows[11].cells[1].innerText = msg.arr[7];
    table.rows[14].cells[1].innerText = msg.arr[8];
    table.rows[15].cells[1].innerText = msg.arr[9];
    table.rows[16].cells[1].innerText = msg.arr[10];
    table.rows[17].cells[1].innerText = msg.arr[11];
    table.rows[20].cells[1].innerText = msg.arr[12];
    table.rows[21].cells[1].innerText = msg.arr[13];
    table.rows[22].cells[1].innerText = msg.arr[14];
    table.rows[23].cells[1].innerText = msg.arr[15];
    socket.emit('get_cpu_usage');
    setInterval(function(){
        socket.emit('get_cpu_usage');
    },10000);
});
socket.on("cpu_usage", str =>{
    console.log(str);
    let cpu_usage = str.slice(str.indexOf('ni,') + 'ni,'.length, str.indexOf(' id,'));
    console.log('==========' + cpu_usage);
    cpu_usage = 100 - parseInt(cpu_usage);
    document.querySelector("#cpu_usage").style.width = cpu_usage + '%';
    document.querySelector("#cpu_usage").innerText = cpu_usage + '%';
    let total = str.slice(str.indexOf("iB Mem :") + "iB Mem :".length, index_near_key(str, str.indexOf("iB Mem :") + "iB Mem :".length,'t') - 1).trim();
    let used = str.slice(str.indexOf('free,') + 'free,'.length, str.indexOf(' used,')).trim()
    total = parseFloat(total);
    used = parseFloat(used);
    let value = Math.round(used*100/total);
    if (!isNaN(value))
    document.querySelector("#memory_usage").style.width = value + '%';
    document.querySelector("#memory_usage").innerText = value + '%';
    let list_pid = str.slice(str.indexOf('avail Mem \n\n') + 'avail Mem \n\n'.length);
    console.log(str.indexOf('avail Mem \n\n'));
    document.querySelector("#list_process").innerText = list_pid;
});
socket.on("sensor", str =>{
    let temp = str.slice(str.indexOf("$") + 6, str.lastIndexOf("$"));
    let humd = str.slice(str.lastIndexOf("$") + 5).trim();
    document.querySelector("#temp").innerText = temp + ' oC';
    document.querySelector("#hump").innerText = humd + ' %';
});
function index_near_key(src, start, key){
    var i = 0;
    while(i < 1000){
        if (src.charAt(start + i) == key) return start + i + 1;
        else i++
    }
}