from flask import Blueprint
from tinydb import TinyDB, Query



NE = Blueprint('routes', __name__)
@NE.route("/system/date/get", methods=['GET'])
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