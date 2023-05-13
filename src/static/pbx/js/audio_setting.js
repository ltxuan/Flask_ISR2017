socket.on("RES_AUDIO_SETTING", msg => {
    console.log(msg);
    audio_setting_status = false;
    document.querySelector("#Preferred_Vocoder1").value = msg.priotiy[0];
    document.querySelector("#Preferred_Vocoder2").value = msg.priotiy[1];
    document.querySelector("#Preferred_Vocoder3").value = msg.priotiy[2];
    document.querySelector("#Preferred_Vocoder4").value = msg.priotiy[3];
    document.querySelector("#Preferred_Vocoder5").value = msg.priotiy[4];
    document.querySelector("#Preferred_Vocoder6").value = msg.priotiy[5];
    document.querySelector("#Preferred_Vocoder7").value = msg.priotiy[6];
    document.querySelector("#in_audio").checked = msg.in_band;
    document.querySelector("#via_RTP").checked = msg.rc;
    document.querySelector("#via_SIP").checked = msg.info;
    document.querySelector("#Echo_Cancellation_Yes").checked = msg.echo;
    document.querySelector("#Echo_Cancellation_No").checked = !msg.echo;
    document.querySelector("#SRTP_Mode").value = msg.srtp_mode;
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
function check_dtmf(el){
    if (el.checked = true){
        data.audio_setting.dtmf_change = 1;
        let arr = el.parentElement.getElementsByTagName("input");
        for (let i = 0; i < 3; i++){
            if (arr[i].id != el.id) arr[i].checked = false;
        }
    }
}
function save_audio_setting(){
    audio_setting_status = false;
    data.audio_setting.priotiy[0] = (document.querySelector("#Preferred_Vocoder1").value);
    data.audio_setting.priotiy[1] = (document.querySelector("#Preferred_Vocoder2").value);
    data.audio_setting.priotiy[2] = (document.querySelector("#Preferred_Vocoder3").value);
    data.audio_setting.priotiy[3] = (document.querySelector("#Preferred_Vocoder4").value);
    data.audio_setting.priotiy[4] = (document.querySelector("#Preferred_Vocoder5").value);
    data.audio_setting.priotiy[5] = (document.querySelector("#Preferred_Vocoder6").value);
    data.audio_setting.priotiy[6] = (document.querySelector("#Preferred_Vocoder7").value); 
    for (let i = 0; i < 6; i++){
        for (let j = i + 1; j < 7; j++ ){
            if (data.audio_setting.priotiy[i] == data.audio_setting.priotiy[j] && data.audio_setting.priotiy[i] != ""){
                toastr.error('Preferred Vocoder - choice ' + (i + 1) + ' is same as Preferred Vocoder - choice ' + (j + 1) , 'ERROR');
                return;
            }
        }
    }
    data.audio_setting.srtp_mode = document.querySelector("#SRTP_Mode").value;
    data.audio_setting.in_band   = document.querySelector("#in_audio").checked;
    data.audio_setting.rc        = document.querySelector("#via_RTP").checked;
    data.audio_setting.info      =  document.querySelector("#via_SIP").checked;
    data.audio_setting.echo      = document.querySelector("#Echo_Cancellation_Yes").checked;
    var buttonChange             = document.getElementById('buttonChange');
    buttonChange.style.display   = 'block';
}

function change_priority(){
    data.audio_setting.priotiy_change = 1;
}
function change_echo(){
    data.audio_setting.echo_change = 1;
}
function change_srtp(){
    data.audio_setting.srtp_change = 1;
}