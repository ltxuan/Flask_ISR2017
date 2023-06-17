from flask import Flask, render_template, request, redirect, url_for, session, jsonify
from tinydb import TinyDB, Query
import re
from flask_socketio import SocketIO, emit, disconnect
from flask_session import Session
from socket_process import socket_process
import threading
from sip_process import *
from pjsip import *
from readdata import *



# creates a Flask application
app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

async_mode = None
socketio = SocketIO(app)
set_socketio(socketio)



@app.before_first_request
def before_first_request():
    session.clear()  # Xoa het cac phien lam viec cu
    
@app.route("/")
def home():
	return redirect(url_for('login'))

@app.route('/login', methods =['GET', 'POST'])
def login():
	db = TinyDB('../NE_db/user_db')
	User = Query()
	msg = ''
	if request.method == 'POST' and 'username' in request.form and 'password' in request.form:
		username = request.form['username']
		password = request.form['password']
		if db.search(User.username == username) and db.search(User.password == password):
			msg = 'Logged in successfully !'
			session['loggedin'] = True
			session["username"] = request.form.get("username")
			return redirect(url_for('pbx'))
		else:
			msg = 'Incorrect username or password !'
	return render_template('login.html', msg = msg)

@app.route('/pbx')
def pbx():
	if not session.get("username"):
		return redirect(url_for('login'))
	return render_template('pbx.html')



@app.route('/status')
def status():
	if not session.get("username"):
		return redirect(url_for('login'))
	return render_template('status.html')

@app.route('/logout')
def logout():
	session["username"] = None
	return redirect(url_for('login'))

@app.route('/register', methods =['GET', 'POST'])
def register():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	msg = ''
	if request.method == 'POST' and 'username' in request.form and 'password' in request.form :
		username = request.form['username']
		password = request.form['password']
		
		if db.search(User.name == 'username'):
			msg = 'Account already exists !'
		elif not re.match(r'[A-Za-z0-9]+', username):
			msg = 'Username must contain only characters and numbers !'
		elif not username or not password:
			msg = 'Please fill out the form !'
		else:
			db.insert({'username': username, 'password': password})
			msg = 'You have successfully registered !'
	elif request.method == 'POST':
		msg = 'Please fill out the form !'
	return render_template('register.html', msg = msg)


#---------------Rest API ----------------#
@app.route("/system/date/get", methods=['GET'])
def get_system_date():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	data = db.search(User.type == "time_setting")[0]['data']
	data['object'] = '015' + "_ISR2017_SYS_DATE"
	data['date'] = data['date'] + " " + data['time']
	del data['time']
	print("object gui di")
	print(data)
	return jsonify(data)


# run the application
if __name__ == "__main__":
	# socket_process(socketio)
	# thread = threading.Thread(target=schedule_Init_pjsip)
	# thread.start()
	read_network()
	# socketio.run(app, host="0.0.0.0", port=60, debug=False)

	# thread2 = threading.Thread(target=delayed_action2)

    # # Khởi chạy các thread
    # thread1.start()
    # thread2.start()



