from flask import Flask, render_template, request, redirect, url_for, session, jsonify
from tinydb import TinyDB, Query
import re
from flask_socketio import SocketIO, emit, disconnect
import json
import datetime
from flask_session import Session
# creates a Flask application
app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

async_mode = None
socketio = SocketIO(app, async_mode=async_mode)


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

#Thong bao connect socketio thanh cong
@socketio.event
def connect():
    emit('my_response', {'data': 'Connected'})
    print('add 1 client connected')
#Xu ly du lieu trong file system_setting

@socketio.on('GET_INFOR')
def infor():
    db = TinyDB('../NE_db/system_setting')
    User = Query()
    emit("RES_INFOR", db.search(User.type=="infor")[0]['data'])

@socketio.on('GET_BACKUP')
def backup():
    db = TinyDB('../NE_db/system_setting')
    User = Query()
    emit("RES_BACKUP", db.search(User.type=="backup")[0]['data'])
    
@socketio.on('GET_QUAGGA_SETTING')
def quangga():
    db = TinyDB('../NE_db/system_setting')
    User = Query()
    emit("RES_QUAGGA_SETTING", db.search(User.type=="quagga_setting")[0])

@socketio.on('GET_DATE_TIME')
def date_time():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	doc = db.search(User.type=="time_setting")
	days = 	["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
	d = datetime.datetime.now()
	day = d.strftime("%d")
	month = d.strftime("%m")
	year = d.strftime("%Y")
	hour = d.strftime("%H")
	minute = d.strftime("%M")
	second = d.strftime("%S")
	thu = days[int(d.strftime("%w"))]
	emit("RES_DATE_TIME",{ 'data': doc[0]['data'], 'time': { 'day': day, 'month': month, 'year': year, 'hour': hour, 'minute': minute, 'second': second, 'thu': thu}})

@socketio.on('GET_MAKE_CER')
def MakeCer():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	emit("RES_MAKE_CER", db.search(User.type=="make_cer")[0]['data'])

@socketio.on('GET_DIAL_PLAN')
def DialPlan():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	emit("RES_DIAL_PLAN", db.search(User.type=="dial_plan")[0]['data'])

@socketio.on('GET_CALL_FORWARD')
def CallForward():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	emit("RES_CALL_FORWARD", db.search(User.type=="call_forward")[0]['data'])

@socketio.on('GET_GE_SETTING')
def GE():
    db = TinyDB('../NE_db/network_db')
    User = Query()
    emit("RES_GE_SETTING", db.all())

@socketio.on('GET_TRUNK')
def trunk_state():
	db = TinyDB('../NE_db/trunk')
	User = Query()
	emit("RES_TRUNK", db.all())

@socketio.on('GET_AUDIO_SETTING')
def Audio_Setting():
	db = TinyDB('../NE_db/audio_setting')
	User = Query()
	emit("RES_AUDIO_SETTING", db.all()[0]['data'])

@socketio.on('GET_GENERAL_SETTING')
def General_Setting():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	emit("RES_GENERAL_SETTING", db.search(User.type=="general_setting")[0]['data'])

@socketio.on('GET_NAT_SETTING')
def Nat_Setting():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	emit("RES_NAT_SETTING", db.search(User.type=="nat_setting")[0]['data'])

@socketio.on('GET_CODEC_SETTING')
def Codec_Setting():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	emit("RES_CODEC_SETTING", db.search(User.type=="codec_setting")[0]['data'])

@socketio.on('GET_QOS_SETTING')
def Qos_Setting():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	emit("RES_QOS_SETTING", db.search(User.type=="qos_setting")[0]['data'])

@socketio.on('GET_T38_SETTING')
def Qos_Setting():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	emit("RES_T38_SETTING", db.search(User.type=="t38_setting")[0]['data'])

@socketio.on('GET_ADVANCED_SETTING')
def Qos_Setting():
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	print (db.all())
	emit("RES_ADVANCED_SETTING", db.search(User.type=="advanced_setting")[0]['data'])


@socketio.on('GET_PJSIP_CONFIGURE')
def Pjsip_Config():
	db = TinyDB('../NE_db/sip1_db')
	User = Query()
	emit("GET_PJSIP_CONFIGURE", sorted(db.all(), key=lambda i: int(i['id'])))

@socketio.on('GET_FXS')
def sip_extension():
	db = TinyDB('../NE_db/sip_extension')
	User = Query()
	emit("RES_FSX", sorted(db.all(), key=lambda i: i['name']))

@socketio.on('GET_SIP')
def Get_sip():
	db = TinyDB('../NE_db/sip1_db')
	User = Query()
	emit("RES_SIP", sorted(db.all(), key=lambda i: int(i['id'])))

@socketio.on('GET_ATA_STATUS')
def ATA_Status():
	db = TinyDB('../NE_db/sip_status')
	User = Query()
	emit("RES_ATA_STATUS", sorted(db.all(), key=lambda i: int(i['id'])))
 
@socketio.on('GET_FXS_STATUS')
def FXS_Status():
	db = TinyDB('../NE_db/fxs_status')
	User = Query()
	emit("RES_FX_STATUS", sorted(db.all(), key=lambda i: int(i['id'])))

def get_eth_address(eth):
	with open('/etc/network/interfaces', encoding='utf-8') as fs:
		read_data = fs.read()
		fs.close()
	list_network = read_data.split('#GE')
	for i in range(len(list_network)):
		if eth in list_network[i]:
			return(list_network[i].split('address')[1].splitlines()[0])

@socketio.on('GET_ROUTER_STATUS')
def checkAdress():
	ip0 = get_eth_address('eth5')
	ip1 = get_eth_address('eth4')
	ip2 = get_eth_address('eth3')
	ip3 = get_eth_address('eth2')
	ip0 = ip0[0:ip0.rfind('.')]
	ip1 = ip1[0:ip1.rfind('.')]
	ip2 = ip2[0:ip2.rfind('.')]
	ip3 = ip3[0:ip3.rfind('.')]
	data = {'GE1': "STATIC",
      'GE2': "STATIC",
      'GE3': "STATIC",
      'GE4': "STATIC"}
	
	print('\n', ip0)
	print('\n', ip1)
	print('\n', ip2)
	print('\n', ip3)
	db = TinyDB('../NE_db/system_setting')
	User = Query()
	Key_OSPF = list(db.search(User.type=="quagga_setting")[0]['OSPF'].values())
	for i in range(len(Key_OSPF)):
		if isinstance(Key_OSPF[i], str):
			if ip0.strip() in Key_OSPF[i]: data['GE1'] = 'OSPF'
			if ip1.strip() in Key_OSPF[i]: data['GE2'] = 'OSPF'
			if ip2.strip() in Key_OSPF[i]: data['GE3'] = 'OSPF'
			if ip3.strip() in Key_OSPF[i]: data['GE4'] = 'OSPF'
	Key_RIP = list(db.search(User.type=="quagga_setting")[0]['RIP'].values())
	for i in range(len(Key_RIP)):
		if isinstance(Key_RIP[i], str):
			if ip0.strip() in Key_RIP[i]: data['GE1'] = 'RIP'
			if ip1.strip() in Key_RIP[i]: data['GE2'] = 'RIP'
			if ip2.strip() in Key_RIP[i]: data['GE3'] = 'RIP'
			if ip3.strip() in Key_RIP[i]: data['GE4'] = 'RIP'
	emit("RES_ROUTER_STATUS", data)

@app.route('/status')
def status():
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



# run the application
if __name__ == "__main__":
	socketio.run(app, host="0.0.0.0", port=60)
	# checkAdress()

