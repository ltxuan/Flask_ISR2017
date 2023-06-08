
import json
from tinydb import TinyDB, Query


db = TinyDB('db')
User = Query()

'''luu du lieu tieng viet'''

def test():
    with open("./system_setting", encoding='utf-8') as system_setting:
        file_contents = system_setting.read()
        system_setting.close()
    tmp = file_contents.splitlines()

    db.truncate()  
    for i in range(len(tmp)):
        db.insert(json.loads(tmp[i]))
    with open('db', encoding='utf-8') as fp:
     model = json.load(fp)

    with open('db', 'w', encoding='utf-8') as fp:
     fp.write(json.dumps(model))

    users = db.search(User.type=="infor")
    print (type(users[0]))
    print(users[0]["data"])
def update():
    db = TinyDB('./sip_extension')
    User = Query()
    data = {'name': '958250', 'callerid': 'ATA <958250>', 'secret': '958250', 'dtmfmode': 'inband', 'en_voice_mail': False, 'mailbox': '958250', 'codec': 'all', 'transport': 'tcp', 'srtp_enable': True, 'srtp_mode': 'dtls', 'nat': 'no', 'qualify': 'no'}
    tmp = db.get(User.name == data['name'])
    print(tmp)
    print('id = \n', tmp.doc_id)
    db.update(data, doc_ids = [tmp.doc_id])
    print('done \n', db.search(User.name == data['name']))

def table():
    arr = ['item1', 'item2', 'item3', 'item4']  # Danh sách arr
    for i in reversed(range(len(arr))):
        print(arr[i])

def update_key():
    db = TinyDB('db')
    db.truncate()
    db.insert_multiple([{"type":"ATA","data":{"id":"21","sip_uri":"sip:958271@10.10.0.182","status":"OK"},"_id":"vqrGlnpyr1aFLj7O"},\
{"type":"dasboard","data":{"arm_ready":"NO","fpga_ready":"NO","ok_3v":"OK","ok_5v":"OK","arr":[],"cpu_temp":"0 oC","temp":"38.9","humd":"31.4"},"_id":"vrDmRmptXm2atjI4"},\
{"type":"ATA","data":{"id":"10","sip_uri":"sip:958260@10.10.0.182","status":"OK"},"_id":"vwfT8tpp5g7GVhZO"}])
    Status = Query()
    print(db.all())

    # Tìm tài liệu có 'type' là 'dashboard'
    dashboard_doc = db.get(Status.type == 'dasboard')

    # Kiểm tra xem tài liệu có tồn tại không
    if dashboard_doc:
        # Cập nhật trường 'arm_ready' trong trường 'data'
        dashboard_doc['data']['arm_ready'] = 'YES'
        db.update(dashboard_doc, doc_ids=[dashboard_doc.doc_id])

    print(db.all())



if __name__ == "__main__":
    update_key()
