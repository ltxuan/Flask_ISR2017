$(document).ready(function () {
    socket.on("GET_PJSIP_CONFIGURE", msg => {
        console.log(msg)
        sip_configure_status = false;
        for (var i = 0; i < msg.length; i++){
            if (msg[i].status == "checked"){
                document.querySelector("#sipaccountenable" + i).checked = true
            }
            document.querySelector("#sipaccount" + i).value = msg[i].sip_uri;
            document.querySelector("#user" + i).value = msg[i].caller_id;
            document.querySelector("#password-field" + i).value = msg[i].password;
            document.querySelector("[name='tran" + i + "']").value = msg[i].transport;
            document.querySelector("#port" + i).value = msg[i].port;
            document.querySelector("[name='Hotline" + i + "']").value = msg[i].hotline;
            if (msg[i].h_status == "checked"){
                document.querySelector("#Hlineenable" + i).checked = true;
            }
        }
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
    /* socket.on('config_1_status', msg =>{
        var arr = msg.split("\n");
        console.log(arr);
        for (var i = 0 ; i < arr.length - 1; i++){
          var index = arr[i].slice(arr[i].indexOf('$index:') + 7, arr[i].lastIndexOf('$'));
          if (arr[i].includes('OK')){
            toastr.success('CONFIG TO ACCOUNT ' + index, 'SUCCESS');
          }
          else{
            var status = arr[i].slice(arr[i].indexOf('$status:') + 7);
            toastr.error('CONFIG TO ACCOUNT ' + index + ' GET EROR : ' + status, 'ERROR');
          }
        }
      }); */
});
toastr.options.closeButton = true;
toastr.options.timeOut = 60000;
function summit_all_sip(){
  for (let i = 0; i < 24; i++){
    let tmp_obj = {};
    tmp_obj.id = i.toString();
    if (document.querySelector("#sipaccountenable" + i).checked){
      tmp_obj.status = "checked";
    }
    else tmp_obj.status = "";
    if (document.querySelector("#Hlineenable" + i).checked){
      tmp_obj.h_status = "checked";
    }
    else tmp_obj.h_status = "";
    tmp_obj.sip_uri = document.querySelector("#sipaccount"+i).value;
    tmp_obj.domain = document.querySelector("#sipaccount"+i).value.slice(document.querySelector("#sipaccount"+i).value.indexOf('@') + 1,document.querySelector("#sipaccount"+i).value.lastIndexOf(':'));
    tmp_obj.caller_id = document.querySelector("#user" + i).value;
    tmp_obj.password = document.querySelector("#password-field" + i).value;
    tmp_obj.transport = document.querySelector("[name='tran" + i + "']").value;
    tmp_obj.port = document.querySelector("#port" + i).value;
    tmp_obj.hotline = document.querySelector("[name='Hotline" + i + "']").value;
    if (tmp_obj.sip_uri != ("sip:" + tmp_obj.caller_id + "@" + tmp_obj.domain + ":" + tmp_obj.port)){
      toastr.error('SIP URI DONT MATCH AT ACCOUNT: ' + tmp_obj.id, 'ERROR');
      data.sip_configure = [];
      return;
    }
    data.sip_configure.push(tmp_obj);
  }
  var buttonChange = document.getElementById('buttonChange');
  buttonChange.style.display = 'block';
}
function submit_sip_configure_function(){
    var flag = false;
    message.forEach((element) => {
      var sip_uri = document.getElementById("sipaccount"+element.id).value;
      var domain = sip_uri.slice(sip_uri.indexOf('@') + 1,sip_uri.lastIndexOf(':'));
      var caller_id = document.getElementById("user"+element.id).value;
      var port = document.getElementById('port'+element.id).value;
      console.log(sip_uri);
      console.log(caller_id);
      console.log(port);
      if (sip_uri != ("sip:" + caller_id + "@" + domain + ":" + port)){
        flag = true;
        toastr.error('SIP URI DONT MATCH AT ACCOUNT: ' + element.id, 'ERROR');
      }
    });
    if (flag){
      return;
    }
    else{
        data.sip_configure = message;
        var buttonChange = document.getElementById('buttonChange');
        buttonChange.style.display = 'block';
    }
}
function show_pass(element){
  var pass_el = element.parentElement.parentElement.parentElement.childNodes[1];
  if (pass_el.type == "password"){
    pass_el.type = "text";
    element.classList.add( "fa-eye");
    element.classList.remove( "fa-eye-slash" );
  }
  else{
    pass_el.type = "password";
    element.classList.remove("fa-eye" );
    element.classList.add( "fa-eye-slash" );
  }
}
function sip_conf_change(element){
    if (element.name.includes("sipaccount")){
        var id = element.name.slice(10);
        for (var i = 0; i < message.length; i++){
          if (message[i].id == id){
            message[i].sip_uri = element.value;
            message[i].domain = element.value.slice(element.value.indexOf('@') + 1,element.value.lastIndexOf(':'));
            console.log(message[i].domain)
            return;
          }
        }
        var tmp = new Object;
        tmp.id = id;
        tmp.sip_uri = element.value;
        tmp.domain = element.value.slice(element.value.indexOf('@') + 1,element.value.lastIndexOf(':'));
        message.push(tmp);
      }
      else if (element.name.includes("port")){
        var id = element.name.slice(4);
        for (var i = 0; i < message.length; i++){
          if (message[i].id == id){
            message[i].port = element.value;
            return;
          }
        }
        var tmp = new Object;
        tmp.id = id;
        tmp.port = element.value;
        message.push(tmp);
      }
      else if (element.name.includes("user")){
        var id = element.name.slice(4);
        for (var i = 0; i < message.length; i++){
          if (message[i].id == id){
            message[i].caller_id = element.value;
            return;
          }
        }
        var tmp = new Object;
        tmp.id = id;
        tmp.caller_id = element.value;
        message.push(tmp);
      }
      else if (element.name.includes("pass")){
        var id = element.name.slice(4);
        for (var i = 0; i < message.length; i++){
          if (message[i].id == id){
            message[i].password = element.value;
            return;
          }
        }
        var tmp = new Object;
        tmp.id = id;
        tmp.password = element.value;
        message.push(tmp);
      }
      else if (element.name.includes("tran")){
        var id = element.name.slice(4);
        for (var i = 0; i < message.length; i++){
          if (message[i].id == id){
            message[i].transport = element.value.toUpperCase();
            return;
          }
        }
        var tmp = new Object;
        tmp.id = id;
        tmp.transport = element.value;
        message.push(tmp);
      }
      else if (element.name.includes("Hotline")){
        var id = element.name.slice(7);
        for (var i = 0; i < message.length; i++){
          if (message[i].id == id){
            message[i].hotline = element.value;
            return;
          }
        }
        var tmp = new Object;
        tmp.id = id;
        tmp.hotline = element.value;
        message.push(tmp);
      }
}
function onchange_func(element){
    if (element.name.includes("sipaccountenable")){
      var id = element.name.slice(16);
      for (var i = 0; i < message.length; i++){
        if (message[i].id == id){
          if (element.checked){
            message[i].status = "checked";
          }
          else message[i].status = "";
          return;
        }
      }
      var tmp = new Object;
      tmp.id = id;
      if (element.checked){
        tmp.status = "checked";
      }
      else tmp.status = "";
      message.push(tmp);
    }
    if (element.name.includes("Hlenable")){
      var id = element.name.slice(8);
      for (var i = 0; i < message.length; i++){
        if (message[i].id == id){
          if (element.checked){
            message[i].h_status = "checked";
          }
          else message[i].h_status = "";
          return;
        }
      }
      var tmp = new Object;
      tmp.id = id;
      if (element.checked){
        tmp.h_status = "checked";
      }
      else tmp.h_status = "";
      message.push(tmp);
    }
  }