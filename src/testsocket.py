from flask import Flask, render_template,request
from flask_socketio import SocketIO, emit, disconnect

# Set this variable to "threading", "eventlet" or "gevent" to test the
# different async modes, or leave it set to None for the application to choose
# the best option based on installed packages.
async_mode = None

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

@app.route('/')
def index():
    return render_template('index.html')


@socketio.event
def my_event(message):
    emit('my_response',
         {'data': message['data']})

@socketio.event
def disconnect_request():
    emit('my_response',
         {'data': 'Disconnected!'})

@socketio.event
def connect():
    emit('my_response', {'data': 'abcfdg'})


@socketio.on('disconnect')
def test_disconnect():
    print('Client disconnected', request.sid)


if __name__ == '__main__':
    socketio.run(app)