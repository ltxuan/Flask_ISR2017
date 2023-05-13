
socket.on("RES_DIAL_PLAN", msg =>{
    dial_plan_status = false;
    data.dial_plan.dial_plan = document.querySelector("#LocialDialPlan").value = msg.dial_plan
    data.dial_plan.trunk_name = document.querySelector("#Trunkname").value = msg.trunk_name;
    data.dial_plan.ring_time =  document.querySelector("#ring_time").value = msg.ring_time;
    data.dial_plan.c = document.querySelector("#C_CallClass").checked = msg.c;
    data.dial_plan.ab = document.querySelector("#AB_CallClass").checked = msg.ab;
    document.querySelector("#local_dial_1").value = msg.digit1;
    document.querySelector("#local_dial_2").value = msg.digit2;
    document.querySelector("#local_dial_3").value = msg.digit3;
    document.querySelector("#local_dial_4").value = msg.digit4;
    document.querySelector("#local_dial_5").value = msg.digit5;
    document.querySelector("#local_dial_6").value = msg.digit6;
    document.querySelector("#local_dial_7").value = msg.digit7;
    document.querySelector("#local_dial_8").value = msg.digit8;
    document.querySelector("#local_dial_9").value = msg.digit9;
    document.querySelector("#local_dial_0").value = msg.digit0;
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

function save_dial_plan(){
    data.dial_plan.dial_plan = document.querySelector("#LocialDialPlan").value.split(" ").join('');
    data.dial_plan.trunk_name = document.querySelector("#Trunkname").value;
    data.dial_plan.ring_time =  document.querySelector("#ring_time").value;
    data.dial_plan.c = document.querySelector("#C_CallClass").checked;
    data.dial_plan.ab = document.querySelector("#AB_CallClass").checked;
    data.dial_plan.digit1 = document.querySelector("#local_dial_1").value;
    data.dial_plan.digit2 = document.querySelector("#local_dial_2").value;
    data.dial_plan.digit3 = document.querySelector("#local_dial_3").value;
    data.dial_plan.digit4 = document.querySelector("#local_dial_4").value;
    data.dial_plan.digit5 = document.querySelector("#local_dial_5").value;
    data.dial_plan.digit6 = document.querySelector("#local_dial_6").value;
    data.dial_plan.digit7 = document.querySelector("#local_dial_7").value;
    data.dial_plan.digit8 = document.querySelector("#local_dial_8").value;
    data.dial_plan.digit9 = document.querySelector("#local_dial_9").value;
    data.dial_plan.digit0 = document.querySelector("#local_dial_0").value;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}
