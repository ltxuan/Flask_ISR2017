
socket.on("RES_QUAGGA_SETTING", msg =>{
    console.log(msg)
    quagga_status = false;
    //rip
    document.querySelector("#IPv4_RIP").checked = msg.RIP.ipv4;
    document.querySelector("#IPv6_RIP").checked = msg.RIP.ipv6;
    if (msg.RIP.net_work_1 != "") {
        document.querySelector("#RIP_netw1").value = msg.RIP.net_work_1.slice(0, msg.RIP.net_work_1.indexOf("/"));
        document.querySelector("#RIP_nets1").value = msg.RIP.net_work_1.slice(msg.RIP.net_work_1.indexOf("/"));
    }
    if (msg.RIP.net_work_2 != ""){
        document.querySelector("#RIP_netw2").value = msg.RIP.net_work_2.slice(0, msg.RIP.net_work_2.indexOf("/"));
        document.querySelector("#RIP_nets2").value = msg.RIP.net_work_2.slice(msg.RIP.net_work_2.indexOf("/"))
    }
    if (msg.RIP.net_work_3 != ""){
        document.querySelector("#RIP_netw3").value = msg.RIP.net_work_3.slice(0, msg.RIP.net_work_3.indexOf("/"));
        document.querySelector("#RIP_nets3").value = msg.RIP.net_work_3.slice(msg.RIP.net_work_3.indexOf("/"));
    }
    if (msg.RIP.net_work_4 != ""){
        document.querySelector("#RIP_netw4").value = msg.RIP.net_work_4.slice(0, msg.RIP.net_work_4.indexOf("/"));
        document.querySelector("#RIP_nets4").value = msg.RIP.net_work_4.slice(msg.RIP.net_work_4.indexOf("/"));
    }
    if (msg.RIP.net_work_5 != ""){
        document.querySelector("#RIP_netw5").value = msg.RIP.net_work_5.slice(0, msg.RIP.net_work_5.indexOf("/"));
        document.querySelector("#RIP_nets5").value = msg.RIP.net_work_5.slice(msg.RIP.net_work_5.indexOf("/"));
    }
    document.querySelector("#OSPF_RIP").checked = msg.RIP.ospf;
    document.querySelector("#Static_RIP").checked = msg.RIP.static;
    document.querySelector("#Kernel_RIP").checked = msg.RIP.kernel;
    document.querySelector("#Connected_RIP").checked = msg.RIP.connected;

    //ospf
    document.querySelector("#IPv4_OSPF").checked = msg.OSPF.ipv4;
    document.querySelector("#IPv6_OSPF").checked = msg.OSPF.ipv6;
    if (msg.OSPF.net_work_1 != " area ") {
        document.querySelector("#OSPF_netw1").value = msg.OSPF.net_work_1.slice(0, msg.OSPF.net_work_1.indexOf("/"));
        document.querySelector("#OSPF_nets1").value = msg.OSPF.net_work_1.slice(msg.OSPF.net_work_1.indexOf("/"), msg.OSPF.net_work_1.indexOf(" area "));
        document.querySelector("#OSPF_area1").value = msg.OSPF.net_work_1.slice(msg.OSPF.net_work_1.indexOf("area") + 5);
    }
    if (msg.OSPF.net_work_2 != " area "){
        document.querySelector("#OSPF_netw2").value = msg.OSPF.net_work_2.slice(0, msg.OSPF.net_work_2.indexOf("/"));
        document.querySelector("#OSPF_nets2").value = msg.OSPF.net_work_2.slice(msg.OSPF.net_work_2.indexOf("/"), msg.OSPF.net_work_2.indexOf(" area "));
        document.querySelector("#OSPF_area2").value = msg.OSPF.net_work_2.slice(msg.OSPF.net_work_2.indexOf("area") + 5);
    }
    if (msg.OSPF.net_work_3 != " area "){
        document.querySelector("#OSPF_netw3").value = msg.OSPF.net_work_3.slice(0, msg.OSPF.net_work_3.indexOf("/"));
        document.querySelector("#OSPF_nets3").value = msg.OSPF.net_work_3.slice(msg.OSPF.net_work_3.indexOf("/"), msg.OSPF.net_work_3.indexOf(" area "));
        document.querySelector("#OSPF_area3").value = msg.OSPF.net_work_3.slice(msg.OSPF.net_work_3.indexOf("area") + 5);
    }
    if (msg.OSPF.net_work_4 != " area "){
        document.querySelector("#OSPF_netw4").value = msg.OSPF.net_work_4.slice(0, msg.OSPF.net_work_4.indexOf("/"));
        document.querySelector("#OSPF_nets4").value = msg.OSPF.net_work_4.slice(msg.OSPF.net_work_4.indexOf("/"), msg.OSPF.net_work_4.indexOf(" area "));
        document.querySelector("#OSPF_area4").value = msg.OSPF.net_work_4.slice(msg.OSPF.net_work_4.indexOf("area") + 5);
    }
    if (msg.OSPF.net_work_5 != " area "){
        document.querySelector("#OSPF_netw5").value = msg.OSPF.net_work_5.slice(0, msg.OSPF.net_work_5.indexOf("/"));
        document.querySelector("#OSPF_nets5").value = msg.OSPF.net_work_5.slice(msg.OSPF.net_work_5.indexOf("/"), msg.OSPF.net_work_5.indexOf(" area "));
        document.querySelector("#OSPF_area5").value = msg.OSPF.net_work_5.slice(msg.OSPF.net_work_5.indexOf("area") + 5);
    }
    document.querySelector("#RIP_OSPF").checked = msg.OSPF.rip;
    document.querySelector("#Static_OSPF").checked = msg.OSPF.static;
    document.querySelector("#Kernel_OSPF").checked = msg.OSPF.kernel;
    document.querySelector("#Connected_OSPF").checked = msg.OSPF.connected;
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
})

function Save_OSPF(){
    console.log("save opds");
    data.quagga_setting[1].ipv4 = document.querySelector("#IPv4_OSPF").checked;
    data.quagga_setting[1].ipv6 = document.querySelector("#IPv6_OSPF").checked;
    data.quagga_setting[1].net_work_1 = document.querySelector("#OSPF_netw1").value + document.querySelector("#OSPF_nets1").value + " area " + document.querySelector("#OSPF_area1").value;
    data.quagga_setting[1].net_work_2 = document.querySelector("#OSPF_netw2").value + document.querySelector("#OSPF_nets2").value + " area " + document.querySelector("#OSPF_area2").value;
    data.quagga_setting[1].net_work_3 = document.querySelector("#OSPF_netw3").value + document.querySelector("#OSPF_nets3").value + " area " + document.querySelector("#OSPF_area3").value;
    data.quagga_setting[1].net_work_4 = document.querySelector("#OSPF_netw4").value + document.querySelector("#OSPF_nets4").value + " area " + document.querySelector("#OSPF_area4").value;
    data.quagga_setting[1].net_work_5 = document.querySelector("#OSPF_netw5").value + document.querySelector("#OSPF_nets5").value + " area " + document.querySelector("#OSPF_area5").value;
    data.quagga_setting[1].rip = document.querySelector("#RIP_OSPF").checked;
    data.quagga_setting[1].static = document.querySelector("#Static_OSPF").checked;
    data.quagga_setting[1].kernel = document.querySelector("#Kernel_OSPF").checked;
    data.quagga_setting[1].connected = document.querySelector("#Connected_OSPF").checked;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}

function Save_RIP(){
    data.quagga_setting[2].ipv4 = document.querySelector("#IPv4_RIP").checked;
    data.quagga_setting[2].ipv6 = document.querySelector("#IPv6_RIP").checked;
    data.quagga_setting[2].net_work_1 = document.querySelector("#RIP_netw1").value + document.querySelector("#RIP_nets1").value
    data.quagga_setting[2].net_work_2 = document.querySelector("#RIP_netw2").value + document.querySelector("#RIP_nets2").value
    data.quagga_setting[2].net_work_3 = document.querySelector("#RIP_netw3").value + document.querySelector("#RIP_nets3").value
    data.quagga_setting[2].net_work_4 = document.querySelector("#RIP_netw4").value + document.querySelector("#RIP_nets4").value
    data.quagga_setting[2].net_work_5 = document.querySelector("#RIP_netw5").value + document.querySelector("#RIP_nets5").value
    data.quagga_setting[2].ospf = document.querySelector("#OSPF_RIP").checked;
    data.quagga_setting[2].static = document.querySelector("#Static_RIP").checked;
    data.quagga_setting[2].kernel = document.querySelector("#Kernel_RIP").checked;
    data.quagga_setting[2].connected = document.querySelector("#Connected_RIP").checked;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}