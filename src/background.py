# background.py
from flask_socketio import emit
from threading import Thread
import time
socketio = None

def set_socketio(sio):
    global socketio
    socketio = sio
def background_task():
    while True:
        data = "Hello from server at " + time.strftime('%H:%M:%S')
        # Gửi dữ liệu tới tất cả client đang kết nối
        if socketio is not None:
            socketio.emit('message', data)
        time.sleep(1)  # Gửi dữ liệu mỗi 5 giây
