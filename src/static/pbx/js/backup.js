
socket.on("RES_BACKUP", msg =>{
    console.log(msg);
    msg.sort(function(a, b){
        let x = a.file_name.toLowerCase();
        let y = b.file_name.toLowerCase();
        if (x < y) {return -1;}
        if (x > y) {return 1;}
        return 0;
      });
    let table = document.querySelector("#table-Backup_and_Restore");
    msg.forEach(element => {
            let row = table.insertRow(-1);
            var cell1 = row.insertCell(0);
            var cell2 = row.insertCell(1);
            var cell3 = row.insertCell(2);
            var cell4 = row.insertCell(3);
            cell1.innerHTML = 'ALL';
            cell2.innerHTML = element.file_name;
            cell3.innerHTML = element.date;
            cell4.innerHTML = '<i class="fa fa-download" aria-hidden="true" onclick="download_backup(this)"></i>';
            getPagination('table-Backup_and_Restore','#table_mother_Backup_and_Restore ',10);
    });
});
function createNewBackUp() {
    var newBackUp = document.getElementById('newBackUp');
    newBackUp.style.display = "block";
    const d = new Date();
    document.querySelector("#backup_file").value = "backup_" + d.getFullYear() + '_' + (d.getMonth() + 1) + '_' + d.getDate() + '_' + d.getHours() + '_' + d.getMinutes();

}
function uploadBackUp() {
    var uploadBackUp = document.getElementById('uploadBackUp');
    uploadBackUp.style.display = "block";
}
function closeTable(tableId) {
    var tableId1= document.getElementById(tableId);
    tableId1.style.display = 'none';
}
function saveBackupRestore(){
    window.location.href = "mbackup/" +  document.querySelector("#backup_file").value;
    closeTable("newBackUp");
}
function download_backup(el){
    let name = el.parentElement.parentElement.cells[1].innerText;
    window.location.href = "dbackup/" +  name;
}
function confirm_submit(){
    let r = confirm('Are you sure you wish to do this');
    if (r){
        sessionStorage.setItem("tabName", "Reset_and_Reboot_Options");
        sessionStorage.setItem("reboot", true);
    }
    return r;
}
function reset_default(){
    let r = confirm('Bạn có chắc muốn khôi phục cấu hình mặc định của nhà sản xuất hay không?');
    if (r){
        let r1 = confirm('Thiết bị sẽ khôi phục cấu hình mặc định nếu chọn OK!');
        if (r1){
            socket.emit("reset_default");
        }
    }
}