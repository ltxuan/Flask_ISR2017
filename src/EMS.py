from flask import Blueprint, request , jsonify
from tinydb import TinyDB, Query
import builtins
import datetime
import re


NE = Blueprint('routes', __name__)
@NE.route("/system/date/get", methods=['GET'])
def get_system_date():
	db_system_setting= TinyDB('../NE_db/system_setting')
	data = db_system_setting.get(Query().type == "time_setting")['data']
	db_system_setting.close()
	data['object'] = builtins.NODE_ID + "_ISR2017_SYS_DATE"
	d = datetime.datetime.now()
	day = d.strftime("%d")
	month = d.strftime("%m")
	year = d.strftime("%Y")
	hour = d.strftime("%H")
	minute = d.strftime("%M")
	second = d.strftime("%S")
	data['date'] = year + '-' + month + '-' + day + ' ' + hour + ':' + minute +':'+ second
	del data['time']
	print("object gui di")
	print(data)
	return jsonify(data)




@NE.route('/net/get', methods=['GET'])
def get_network():
	GE = request.args.get('key')
	print("gia tri nhan duoc")
	GE_key = GE[-3:]
	print(GE_key)
    
	db_network = TinyDB('../NE_db/network_db')
	network = db_network.get(Query().name == GE_key)['data']
	db_network.close()
	network['object'] = GE.replace('NodeID', builtins.NODE_ID)
	network['ipadd'] = network['ipaddress']
	del network['ipaddress']
	del network['id']
	print("object gui di")
	print(network)
	return jsonify(network)


@NE.route('/net/looback/get', methods=['GET'])
def get_looback():
	db_network = TinyDB('../NE_db/network_db')
	data = db_network.get(Query().name == 'LOOP_BACK')['data']
	db_network.close()
	data['ipadd'] = data['ipaddress']
	del data['ipaddress']
	del data['id']
	data['object'] = builtins.NODE_ID + '_ISR2017_NET_LOOPBACK'
	print('object gui di')
	print(data)
	return jsonify(data)

@NE.route('/ata/account/get', methods=['GET'])
def get_ata_account():
	# print (request.args.get('key'))
	if 'key' in request.args:
		print('gia tri nhan duoc')
		print(request.args.get('key'))
		key_arr = re.findall(r'\d+', request.args.get('key'))
		ID = key_arr[-1]
		db_sip = TinyDB('../NE_db/sip1_db')
		data = db_sip.get(Query().id == ID)
		db_sip.close()
		if data:
			del data['id']
			if data['status'] == "checked": data['status'] = True
			else: data['status'] = False
			if data['h_status'] == "checked": data['h_status'] = True
			else: data['h_status'] = False
			data['object'] = request.args.get('key').replace('NodeID', builtins.NODE_ID)
			print('object gui di')
			print(data)
			return jsonify(data)
		else:
			return jsonify({"result": -1})
	else:
		db_sip = TinyDB('../NE_db/sip1_db')
		data = sorted(db_sip.all(), key=lambda i: int(i['id']))
		for element in data:
			element['object'] = builtins.NODE_ID + "_ISR2017_ATA_ACC" + element['id']
			if element['status'] == "checked": element['status'] = True
			else: element['status'] = False
			if element['h_status'] == "checked": element['h_status'] = True
			else: element['h_status'] = False
			del element['id']
		return jsonify(data)

@NE.route('/ata/status/get', methods=['GET'])
def get_ata_status():
	if 'key' in request.args:
		print('gia tri nhan duoc')
		print(request.args.get('key'))
		key_arr = re.findall(r'\d+', request.args.get('key'))
		ID = key_arr[-1]



#     db.sip_status.findOne({ id: ID }, function (err, docs) {
#       if (docs) {
#         if (docs.status.includes('OK')) {
#           docs.register = 'yes';
#         }
#         else {
#           docs.register = 'no';
#         }
#         delete docs.status;
#         delete docs.id;
#         delete docs._id;
#         docs.state = state_text(sip1_arr_state[parseInt(ID)]);
#         docs.object = NODE_ID + '_ISR2017_ATA_STATUS_ACC' + ID;
#         console.log("object gui di");
#         console.log(docs);
#         res.json(docs);
#       }
#     });
#   }
