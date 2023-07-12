from flask import Flask, render_template, request, redirect, url_for, session
from tinydb import TinyDB, Query
import re
from flask_socketio import SocketIO, emit, disconnect
from flask_session import Session
from socket_process import socket_process
import threading
from sip_process import *
from pjsip import *
from readdata import *
import builtins
from EMS import NE

# creates a Flask application
app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

async_mode = None
builtins.socketio = SocketIO(app)

app.register_blueprint(NE)


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

def test():
	db_sip = TinyDB('../NE_db/sip1_db')
	data = sorted(db_sip.all(), key=lambda i: int(i['id']))
	print(data)


# run the application
if __name__ == "__main__":
	run_child_process()
	# read_system()
	# socket_process(socketio)
	# process_thread = threading.Thread(target=start_processes_pjsip)
	# process_thread.start()
	# process_thread.join()
	# process_thread2 = threading.Thread(target=start_processes_SNMP)
	# process_thread2.start()
	# process_thread2.join()

	# repeat_task()

	# socketio.run(app, host="0.0.0.0", port=80, debug=False)
	# test()




