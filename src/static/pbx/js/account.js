socket.on("user_exist", msg => {
    document.querySelector("#notificationNewPass_AddUser").innerText = "User has existed";
    document.querySelector("#notificationNewPass_AddUser").style = 'display:block';
});
socket.on("user_not_exist", msg => {
    document.querySelector("#notificationNewPass_ChangePassword").innerText = "User not existed";
    document.querySelector("#notificationNewPass_ChangePassword").style = 'display:block';
});
socket.on("wrong_pass", msg => {
    document.querySelector("#notificationNewPass_ChangePassword").innerText = "Wrong password";
    document.querySelector("#notificationNewPass_ChangePassword").style = 'display:block';
});
function change_user(){
    document.querySelector("#notificationNewPass_ChangePassword").style = 'display:none';
    data.account.change_user_name =  document.querySelector("#selectUser_ChangePassword").value;
    data.account.old_pass_word =  document.querySelector("#Old_Password_ChangePassword").value;
    data.account.new_pass_word =  document.querySelector("#New_Password_ChangePassword").value;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}
function add_user(){
    document.querySelector("#notificationNewPass_AddUser").style = 'display:none';
    data.account.user_name =  document.querySelector("#selectUser").value;
    data.account.pass_word =  document.querySelector("#New_Password_AddUser").value;
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}
function checkPassLoop_AddUser(){
    let pass = document.querySelector("#New_Password_AddUser").value;
    let re_pass = document.querySelector("#Retype_New_Password__AddUser").value;
    if (pass != re_pass){
        document.querySelector("#notificationNewPass_AddUser").innerText = "The entered passwords do not match. Try again!";
        document.querySelector("#notificationNewPass_AddUser").style = 'display:block';
    }
    else {
        document.querySelector("#notificationNewPass_AddUser").style = 'display:none';
    }
}
function checkPassLoop_ChangePass(){
    let pass = document.querySelector("#New_Password_ChangePassword").value;
    let re_pass = document.querySelector("#Retype_New_Password_ChangePassword").value;
    if (pass != re_pass){
        document.querySelector("#notificationNewPass_ChangePassword").innerText = "The entered passwords do not match. Try again!";
        document.querySelector("#notificationNewPass_ChangePassword").style = 'display:block';
    }
    else document.querySelector("#notificationNewPass_ChangePassword").style = 'display:none';
}
function checkStrongPass(){
    var pass =  $('#New_Password_AddUser').val();
    var this1= $('#New_Password_AddUser').val().length;
    var format = /[ `!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?~]/;
     if(this1<=6 && this1>0){
        document.getElementById('verybad_AddUser').style.display='block';
         document.getElementById('medium_AddUser').style.display='none';
         document.getElementById('strong_AddUser').style.display='none';
 
        $('#infoPass_AddUser').html('very bad');
        $('#infoPass_AddUser').css({'color':'var(--danger)'});
     }
     if(this1>6 && ((/[A-Z]/.test(pass)) && format.test(pass))){
         document.getElementById('verybad_AddUser').style.display='none';
         document.getElementById('medium_AddUser').style.display='none';
         document.getElementById('strong_AddUser').style.display='block';
 
         $('#infoPass_AddUser').html('strong');
         $('#infoPass_AddUser').css({'color':'var(--success)'});
     }
      else if(this1>6 && this1<=8 && ((/[A-Z]/.test(pass)) || format.test(pass))){
         document.getElementById('verybad_AddUser').style.display='none';
         document.getElementById('medium_AddUser').style.display='block';
         document.getElementById('strong_AddUser').style.display='none';
          $('#infoPass_AddUser').html('medium');
          $('#infoPass_AddUser').css({'color':'var(--warning)'});
     }
     else if(this1>8){
         document.getElementById('verybad_AddUser').style.display='none';
         document.getElementById('medium_AddUser').style.display='block';
         document.getElementById('strong_AddUser').style.display='none';
          $('#infoPass_AddUser').html('medium');
          $('#infoPass_AddUser').css({'color':'var(--warning)'});
     }
 }
function checkStrongPass1(){
    var pass =$('#New_Password_ChangePassword').val();
    var this1= $('#New_Password_ChangePassword').val().length;
    var format = /[ `!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?~]/;
    if(this1<=6 && this1>0){
        document.getElementById('verybad_ChangePassword').style.display='block';
        document.getElementById('medium_ChangePassword').style.display='none';
        document.getElementById('strong_ChangePassword').style.display='none';

        $('#infoPass_ChangePassword').html('very bad');
        $('#infoPass_ChangePassword').css({'color':'var(--danger)'});
    }
    if(this1>6 && ((/[A-Z]/.test(pass)) && format.test(pass))){
        console.log('ok');
        document.getElementById('verybad_ChangePassword').style.display='none';
        document.getElementById('medium_ChangePassword').style.display='none';
        document.getElementById('strong_ChangePassword').style.display='block';

        $('#infoPass_ChangePassword').html('strong');
        $('#infoPass_ChangePassword').css({'color':'var(--success)'});
    }
    else if(this1>6 && this1<=8 && ((/[A-Z]/.test(pass)) || format.test(pass))){
        document.getElementById('verybad_ChangePassword').style.display='none';
        document.getElementById('medium_ChangePassword').style.display='block';
        document.getElementById('strong_ChangePassword').style.display='none';
        $('#infoPass_ChangePassword').html('medium');
        $('#infoPass_ChangePassword').css({'color':'var(--warning)'});
    }
    else if(this1>8){
        document.getElementById('verybad_ChangePassword').style.display='none';
        document.getElementById('medium_ChangePassword').style.display='block';
        document.getElementById('strong_ChangePassword').style.display='none';
        $('#infoPass_ChangePassword').html('medium');
        $('#infoPass_ChangePassword').css({'color':'var(--warning)'});
    }
}