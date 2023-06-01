import subprocess
import threading
import time
import sys

def handle_message(message):
    # Xử lý thông điệp
    pass

def restart_pjsip():
    # Khởi động lại pjsip
    pass

def child_stdout_handler(child):
    while True:
        data = child.stdout.readline()
        if not data:
            break
        str_data = data.decode('utf-8')
        sys.stdout.write(str_data)
        sys.stdout.flush()

def child_stderr_handler(child):
    while True:
        data = child.stderr.readline()
        if not data:
            break
        str_data = data.decode('utf-8')
        sys.stdout.write("========\n")
        sys.stdout.write(str_data)
        sys.stdout.flush()
        handle_message(str_data)

def abc():
    time.sleep(10)  # Đợi 10 giây

    child = subprocess.Popen(['../RL_uart/app'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    stdout_thread = threading.Thread(target=child_stdout_handler, args=(child,))
    stderr_thread = threading.Thread(target=child_stderr_handler, args=(child,))

    stdout_thread.start()
    stderr_thread.start()

    code = child.wait()
    sys.stdout.write(f"child process exited with code {code}\n")
    sys.stdout.flush()

    time.sleep(5)  # Đợi 5 giây
    restart_pjsip()

