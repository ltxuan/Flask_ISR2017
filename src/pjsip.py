from flask import Flask
import time
import subprocess
import threading
from tinydb import TinyDB, Query
import builtins
from threading import Timer
import datetime

sip1_arr_state = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
mutex = threading.Lock()

def handle_message(str):
    if 'msg:account' in str:
        db_sip_db = TinyDB('../NE_db/sip1_db')
        db_status = TinyDB('../NE_db/sip_status')
        socketio.emit('config_1_status', str)
        arr = str.split("\n")
        list_acc = db_sip_db.all()
        for i in range(len(arr) - 1):
            sip_uri = arr[i][arr[i].index('$index:') + 7:arr[i].rindex('$')]
            status = arr[i][arr[i].rindex(':') + 1:]
            for element in list_acc:
                if sip_uri in element['sip_uri']:
                    id = element['id']
                    break
                print('\n', id , sip_uri, status)
                if db_status.get(Query().id==id) is not None:
                    db_status.update({'id': id, 'sip_uri': sip_uri, 'status': status}, Query().id == id)
                else:
                    db_status.insert([{'id': id, 'sip_uri': sip_uri, 'status': status}])
        db_status.close()
        db_sip_db.close()
    elif 'msg:status' in str:
        if socketio is not None:
            socketio.emit('sip_state', str)
        arr = str.split("$")
        for i in range(len(arr)):
            sip1_arr_state[i - 1] = arr[i]
        print(sip1_arr_state)
    elif 'msg:call_state' in str:
        if socketio is not None:
            socketio.emit('sip_state', str)
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
        if socketio is not None:
            socketio.emit('arm_ready', str)
        if mutex.acquire(timeout=3):
            try:
                db = TinyDB('../NE_db/status')
                Status = Query()
                dashboard_doc = db.get(Status.type == 'dasboard')
                if dashboard_doc:
                    # Cập nhật trường 'arm_ready' trong trường 'data'
                    dashboard_doc['data']['arm_ready'] = 'YES'
                    db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
            finally:
                mutex.release()
        else:
            pass
    elif 'msg:fpga_ready' in str:
        if socketio is not None:
            socketio.emit('fpga_ready', str)
        if mutex.acquire(timeout=3):
            try:
                db = TinyDB('../NE_db/status')
                Status = Query()
                dashboard_doc = db.get(Status.type == 'dasboard')
                if dashboard_doc:
                    # Cập nhật trường 'arm_ready' trong trường 'data'
                    dashboard_doc['data']['fpga_ready'] = 'YES'
                    db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
            finally:
                mutex.release()
        else:
            pass
    elif 'msg:3v_ok' in str:
        if socketio is not None:
            socketio.emit('3v_ok', str)
        if mutex.acquire(timeout=3):
            try:
                db = TinyDB('../NE_db/status')
                Status = Query()
                dashboard_doc = db.get(Status.type == 'dasboard')
                if dashboard_doc:
                    # Cập nhật trường 'arm_ready' trong trường 'data'
                    dashboard_doc['data']['ok_3v'] = 'OK'
                    db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
            finally:
                mutex.release()
        else:
            pass
    elif 'msg:5v_ok' in str:
        if socketio is not None:
            socketio.emit('5v_ok', str)
        if mutex.acquire(timeout=3):
            try:
                db = TinyDB('../NE_db/status')
                Status = Query()
                dashboard_doc = db.get(Status.type == 'dasboard')
                if dashboard_doc:
                    # Cập nhật trường 'arm_ready' trong trường 'data'
                    dashboard_doc['data']['ok_5v'] = 'OK'
                    db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
            finally:
                mutex.release()
        else:
            pass
    elif 'msg:sensor' in str:
        if socketio is not None:
            socketio.emit('sensor', str)
        temp = str[str.index("$") + 6:str.rindex("$")]
        humd = str[str.rindex("$") + 5:].strip()
        if mutex.acquire(timeout=3):
            try:
                db = TinyDB('../NE_db/status')
                Status = Query()
                dashboard_doc = db.get(Status.type == 'dasboard')
                if dashboard_doc:
                    # Cập nhật trường 'arm_ready' trong trường 'data'
                    dashboard_doc['data']['temp'] = temp
                    dashboard_doc['data']['humd'] = humd
                    db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])
            finally:
                mutex.release()
        else:
            pass
    elif 'msg:fxs' in str:
        db_fxs = TinyDB('../NE_db/fxs_status')
        Fxs_status = Query()
        if socketio is not None:
            socketio.emit('fxs', str)
        index = str[str.index("$index:") + len("$index:"):str.index("$status:")]
        status = str[str.index("$status:") + len("$status:"):str.index("$value:")]
        if status == '0':
            status = 'YES'
        else:
            status = 'NO'
        value = str[str.index("$value:") + len("$value:"):].strip() + ' V'
        if db_fxs.get(Fxs_status.id==index) is not None:
            db_fxs.update({'id': index, 'status': status, 'value': value}, Status.id == index)
        else:
            db_fxs.insert([{'id': index, 'status': status, 'value': value}])

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


def start_child_process(command, directory, stdout_callback, stderr_callback, close_callback, delay):
    time.sleep(delay)  # Đợi khoảng thời gian
    child = subprocess.Popen(command, cwd=directory, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    def read_stdout():
        for line in iter(child.stdout.readline, b''):
            stdout_callback(line.decode().strip())
    
    def read_stderr():
        for line in iter(child.stderr.readline, b''):
            stderr_callback(line.decode().strip())
    
    # Tạo và khởi chạy luồng đọc đầu ra stdout
    stdout_thread = threading.Thread(target=read_stdout)
    stdout_thread.start()
    
    # Tạo và khởi chạy luồng đọc đầu ra stderr
    stderr_thread = threading.Thread(target=read_stderr)
    stderr_thread.start()
    
    # Đợi quá trình con kết thúc và gọi hàm callback
    child.wait()
    close_callback(child.returncode)

def start_processes_pjsip():
    # Quá trình con thứ nhất, chạy sau 10 giây
    start_child_process(
        ['../RL_uart/app', ''],
        None,  # Thư mục hiện tại
        lambda line: print(line),  # Callback cho stdout
        lambda line: handle_message(line),  # Callback cho stderr
        lambda code: restart_pjsip(),  # Callback khi quá trình con kết thúc
        10  # Khoảng thời gian chờ (10 giây)
    )
def start_processes_SNMP():    
    # Quá trình con thứ hai, chạy sau 15 giây
    start_child_process(
        ['./example-demon', ''],
        '../SNMP',  # Thư mục của quá trình con
        lambda line: print("SNMP stdout: " + line),  # Callback cho stdout
        lambda line: print("SNMP stderr: " + line),  # Callback cho stderr
        lambda code: None,  # Callback khi quá trình con kết thúc
        15  # Khoảng thời gian chờ (15 giây)
    )

def enable_restart_callback():
    global enable_restart
    enable_restart = True 

def repeat_task():
    global enable_restart
    if enable_restart:
            current_hour = datetime.datetime.now().hour
            if current_hour == 0 or current_hour == 1:
                all_off = all(element == 0 for element in sip1_arr_state)
                if all_off:
                    enable_restart = False
                    Timer(7200, enable_restart_callback).start()
                    builtins.child.kill()

    # Lập lịch thực hiện lại sau một khoảng thời gian (ví dụ: 30 phút)
    repeat_time = Timer(1800, repeat_task).start()

   