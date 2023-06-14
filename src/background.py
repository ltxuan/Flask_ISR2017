# background.py
from flask_socketio import emit
from threading import Thread
import time

def background_task(socketio):
    while True:
        data = "Hello from server at " + time.strftime('%H:%M:%S')

        # Gửi dữ liệu tới tất cả client đang kết nối
        socketio.emit('message', data)

        time.sleep(1)  # Gửi dữ liệu mỗi 5 giây
