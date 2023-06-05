from flask import Flask, render_template
from flask_socketio import SocketIO
from axuan import *

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret_key'
socketio = SocketIO(app)


# @socketio.on('my_event')
# def handle_my_event(data):
#     message = data['message']
#     socketio.emit('my_response', {'message': 'Server says: ' + message})

@app.route('/')
def index():
    return render_template('index.html')


if __name__ == '__main__':
    socket_process(socketio)
    socketio.run(app, host="0.0.0.0", port=60)
    
    
