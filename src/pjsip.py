from flask import Flask
import time
import subprocess
import threading
from flask_socketio import SocketIO, emit, disconnect

sip1_arr_state = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

from tinydb import TinyDB, Query

mutex = threading.Lock()
mutex_status = mutex.acquire(timeout=3)

def handle_message(str):
    if 'msg:account' in str:
        db = TinyDB('../NE_db/sip1_db')
        User = Query()
        db_status = TinyDB('../NE_db/status')
        Status = Query()
        # emit('config_1_status', str)
        arr = str.split("\n")
        list_acc = db.all()
        for i in range(len(arr) - 1):
            sip_uri = arr[i][arr[i].index('$index:') + 7:arr[i].rindex('$')]
            status = arr[i][arr[i].rindex(':') + 1:]
            for element in list_acc:
                if sip_uri in element['sip_uri']:
                    id = element['id']
                    break
                if db_status.get(Status.id==id) is not None:
                    db_status.update({'id': id, 'sip_uri': sip_uri, 'status': status}, Status.id == id)
                else:
                    db_status.insert([{'id': id, 'sip_uri': sip_uri, 'status': status}])
        db_status.close()
        db.close()
    elif 'msg:status' in str:
        # emit('sip_state', str)
        arr = str.split("$")
        for i in range(len(arr)):
            sip1_arr_state[i - 1] = arr[i]
        print(sip1_arr_state)
    elif 'msg:call_state' in str:
        # emit('sip_state', str)
        index = str[str.index("$") + 1 : str.rindex("$")]
        state = str[str.rindex("$") + 1 :]
        sip1_arr_state[int(index)] = state
        print(sip1_arr_state)
    elif 'msg:reset_network' in str:
        write_data = "# interfaces(5) file used by ifup(8) and ifdown(8)\n" +\
      "auto lo lo:10\n" +\
      "iface lo inet loopback\n" +\
      "#LOOP_BACK\n" +\
      "iface lo:10 inet static\n" +\
      "     address 192.168.0.1\n" +\
      "     netmask 255.255.255.255\n" +\
      "#E_LOOP\n" +\
      "#GE1\n" +\
      "auto eth0\n" +\
      "iface eth0 inet static\n" +\
      "     address 192.168.1.10\n" +\
      "     netmask 255.255.255.0\n" +\
      "     gateway 192.168.1.1\n" +\
      "#ENDGE1\n" +\
      "#GE2\n" +\
      "auto eth1\n" +\
      "iface eth1 inet static\n" +\
      "     address 192.168.2.20\n" +\
      "     netmask 255.255.255.0\n" +\
      "     gateway 192.168.2.1\n" +\
      "#ENDGE2\n" +\
      "#GE3\n" +\
      "auto eth2\n" +\
      "iface eth2 inet static\n" +\
      "     address 192.168.3.30\n" +\
      "     netmask 255.255.255.0\n" +\
      "     gateway 192.168.3.1\n" +\
      "#ENDGE3\n" +\
      "#GE4\n" +\
      "auto eth3\n" +\
      "iface eth3 inet static\n" +\
      "     address 192.168.4.40\n" +\
      "     netmask 255.255.255.0\n" +\
      "     gateway 192.168.4.1\n" +\
      "#ENDGE4\n"
        with open('/etc/network/interfaces','w', encoding='utf-8') as file:
            file.write(write_data)
        db = TinyDB('../NE_db/user_db')
        User = Query()
        if db.get(User.username == 'admin') is None:
            db.insert({'username': 'admin','password': '123456','type': 'admin'})
        else:
            db.update({'password': '123456'}, User.username == 'admin')
        db.close()
        child = subprocess.Popen(["reboot"], stdin=subprocess.PIPE)
        child.stdin.write(b"reboot\n")
        print('reboot')
        child.stdin.close()
    elif 'msg:arm_ready' in str:
        # emit('arm_ready', str)
        if mutex_status:
            db = TinyDB('../NE_db/status')
            Status = Query()
            dashboard_doc = db.get(Status.type == 'dasboard')
            if dashboard_doc:
                # Cập nhật trường 'arm_ready' trong trường 'data'
                dashboard_doc['data']['arm_ready'] = 'YES'
                db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
        mutex.release()
    elif 'msg:fpga_ready' in str:
        # emit('fpga_ready', str)
        if mutex_status:
            db = TinyDB('../NE_db/status')
            Status = Query()
            dashboard_doc = db.get(Status.type == 'dasboard')
            if dashboard_doc:
                # Cập nhật trường 'arm_ready' trong trường 'data'
                dashboard_doc['data']['fpga_ready'] = 'YES'
                db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
        mutex.release()
    elif 'msg:3v_ok' in str:
        emit('3v_ok', str)
        if mutex_status:
            db = TinyDB('../NE_db/status')
            Status = Query()
            dashboard_doc = db.get(Status.type == 'dasboard')
            if dashboard_doc:
                # Cập nhật trường 'arm_ready' trong trường 'data'
                dashboard_doc['data']['ok_3v'] = 'OK'
                db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
        mutex.release()
    elif 'msg:5v_ok' in str:
        # emit('5v_ok', str)
        if mutex_status:
            db = TinyDB('../NE_db/status')
            Status = Query()
            dashboard_doc = db.get(Status.type == 'dasboard')
            if dashboard_doc:
                # Cập nhật trường 'arm_ready' trong trường 'data'
                dashboard_doc['data']['ok_5v'] = 'OK'
                db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
        mutex.release()
    




    


        


def restart_pjsip():
    command = ['../RL_uart/app']
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    def handle_output(data):
        str = data.decode().strip()
        print(str)
    
    def handle_error(data):
        str = data.decode().strip()
        print("========")
        print(str)
        handle_message(str)
    
    while True:
        output = process.stdout.readline()
        if not output and process.poll() is not None:
            break
        if output:
            handle_output(output)
        
        error_data = process.stderr.readline()
        if not error_data and process.poll() is not None:
            break
        if error_data:
            handle_error(error_data)
    
    exit_code = process.returncode
    print(f"Child process exited with code {exit_code}")
    time.sleep(5)
    restart_pjsip()

def Init_pjsip():
    command = ['../RL_uart/app']
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    # Xử lý đầu ra của tiến trình con
    def handle_output(stream):
        for line in stream:
            data = line.decode().strip()
            print(data)
    
    # Xử lý lỗi của tiến trình con
    def handle_error(stream):
        for line in stream:
            error_data = line.decode().strip()
            print("========")
            print(error_data)
            handle_message(error_data)  # Gọi handle_message để xử lý lỗi
    
    # Tạo các thread để xử lý đầu ra và lỗi của tiến trình con
    output_thread = threading.Thread(target=handle_output, args=(process.stdout,))
    output_thread.start()
    
    error_thread = threading.Thread(target=handle_error, args=(process.stderr,))
    error_thread.start()
    
    # Chờ tiến trình con kết thúc
    try:
        process.wait()
    except KeyboardInterrupt:
        process.terminate()
    
    # Xử lý sự kiện kết thúc của tiến trình con
    exit_code = process.returncode
    print(f"Child process exited with code {exit_code}")
    time.sleep(5)  # Đợi 5 giây
    restart_pjsip()
    

def schedule_Init_pjsip():
    # Len lich thuc hien khoi dong pjsip sau 10s
    time.sleep(10)
    Init_pjsip()
