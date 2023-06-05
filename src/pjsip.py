from flask import Flask
import time
import subprocess
import threading

from tinydb import TinyDB, Query

def handle_message(str):
    if 'msg:account' in str:
        # socketio.emit('config_1_status', str)
        arr = str.split("\n")


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
