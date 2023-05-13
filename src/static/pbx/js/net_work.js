socket.on("RES_GE_SETTING", msg =>{
    console.log(msg);
    network_status = false;
    msg.forEach(item => {
        if (item.data.id != 5){
            document.getElementById("GE" + item.data.id).querySelector('input[name="ipv4"]').checked = item.data.ipv4;
            document.getElementById("GE" + item.data.id).querySelector('input[name="ipv6"]').checked = item.data.ipv6;
            document.getElementById("GE" + item.data.id).querySelector('input[name="dhcp"]').checked = item.data.dhcp;
            if (item.data.dhcp){
                document.getElementById("GE" + item.data.id).querySelector('input[name="ipaddress"]').readOnly = true;
                document.getElementById("GE" + item.data.id).querySelector('input[name="netmask"]').readOnly = true
                document.getElementById("GE" + item.data.id).querySelector('input[name="gateway"]').readOnly = true
                document.getElementById("GE" + item.data.id).querySelector('input[name="primaryDNS"]').readOnly = true
                document.getElementById("GE" + item.data.id).querySelector('input[name="secondaryDNS"]').readOnly = true
            }
            document.getElementById("GE" + item.data.id).querySelector('input[name="ipaddress"]').value = item.data.ipaddress;
            document.getElementById("GE" + item.data.id).querySelector('input[name="netmask"]').value = item.data.netmask;
            document.getElementById("GE" + item.data.id).querySelector('input[name="gateway"]').value = item.data.gateway;
            document.getElementById("GE" + item.data.id).querySelector('input[name="primaryDNS"]').value = item.data.primaryDNS;
            document.getElementById("GE" + item.data.id).querySelector('input[name="secondaryDNS"]').value = item.data.secondaryDNS;
        }
        else {
            document.getElementById("LOOP").querySelector('input[name="ipaddress"]').value = item.data.ipaddress;
            document.getElementById("LOOP").querySelector('input[name="netmask"]').value = item.data.netmask;
        }
                
    });
    if (type_user == "user"){
            $("input").prop("disabled", true);
            $("button").not('.tab_links_FXS_VoIP,.tablinks,.toolbar-item-3,#browse').prop("disabled", true);
            $(".contentInput").prop("disabled", true);
            $("select").prop("disabled", true);
            $(".saveBtn").prop("hidden", true);
            $(".cancelBtn").prop("hidden", true);
            $(".saveAndCancelBtn").prop("hidden", true);
            $(".input-group-addon").prop("hidden", true);
            $(".Btn1").prop("hidden", true);
            $(".Btn2").prop("hidden", true);
            $(".Btn3").prop("hidden", true);
            $(".Btn4").prop("hidden", true);
            $("textarea").prop("disabled", true);
            $(".Btn22").prop("hidden", true);
            $(".Btn11").prop("hidden", true);

        }
});

function Network_get_value(el){
    console.log(el);
    var index = parseInt(el.parentElement.querySelector('input[name="id"]').value) - 1;
    console.log(index);
    data.ge_setting[index].id = index + 1;
    try {
        data.ge_setting[index].ipv4 = el.parentElement.querySelector('input[name="ipv4"]').checked;
        data.ge_setting[index].ipv6 = el.parentElement.querySelector('input[name="ipv6"]').checked;
        data.ge_setting[index].dhcp = el.parentElement.querySelector('input[name="dhcp"]').checked;
    } catch (error) {
        
    }
    data.ge_setting[index].ipaddress = el.parentElement.querySelector('input[name="ipaddress"]').value;
    data.ge_setting[index].netmask = el.parentElement.querySelector('input[name="netmask"]').value;
    try {
        data.ge_setting[index].gateway = el.parentElement.querySelector('input[name="gateway"]').value;
        data.ge_setting[index].primaryDNS = el.parentElement.querySelector('input[name="primaryDNS"]').value;
        data.ge_setting[index].secondaryDNS = el.parentElement.querySelector('input[name="secondaryDNS"]').value; 
    } catch (error) {
        
    }
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}
function toggleCheckbox(el) {
    if(el.checked == true)
    {
        el.parentElement.parentElement.parentElement.querySelector('input[name="ipaddress"]').readOnly = true;
        el.parentElement.parentElement.parentElement.querySelector('input[name="netmask"]').readOnly = true;
        el.parentElement.parentElement.parentElement.querySelector('input[name="gateway"]').readOnly = true;
        el.parentElement.parentElement.parentElement.querySelector('input[name="primaryDNS"]').readOnly = true;
        el.parentElement.parentElement.parentElement.querySelector('input[name="secondaryDNS"]').readOnly = true;
    }
    else
    {
        el.parentElement.parentElement.parentElement.querySelector('input[name="ipaddress"]').readOnly = false;
        el.parentElement.parentElement.parentElement.querySelector('input[name="netmask"]').readOnly = false;
        el.parentElement.parentElement.parentElement.querySelector('input[name="gateway"]').readOnly = false;
        el.parentElement.parentElement.parentElement.querySelector('input[name="primaryDNS"]').readOnly = false;
        el.parentElement.parentElement.parentElement.querySelector('input[name="secondaryDNS"]').readOnly = false;
    }
}
toggleCheckbox(this);