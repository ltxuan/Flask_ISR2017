from flask import Flask, render_template
from flask_socketio import SocketIO, emit
from threading import Thread
from background import background_task

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret'
socketio = SocketIO(app)

@app.route('/')
def index():
    return render_template('index.html')

@socketio.on('connect')
def handle_connect():
    print('Client connected')

@socketio.on('disconnect')
def handle_disconnect():
    print('Client disconnected')

if __name__ == '__main__':
    # Tạo một thread mới để chạy background_task()
    thread = Thread(target=background_task, args=(socketio,))
    thread.daemon = True
    thread.start()
    socketio.run(app, host="0.0.0.0", port=60)
