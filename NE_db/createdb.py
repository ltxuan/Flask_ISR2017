from tinydb import TinyDB, Query
import json


system_setting = TinyDB("./system_setting")
users = TinyDB("./user_db")
sip1_config = TinyDB("./sip1_db")
network = TinyDB("./network_db")
sip_extension = TinyDB("./sip_extension")
audio_setting = TinyDB("./audio_setting")
trunk = TinyDB("./trunk")
status = TinyDB("./status")
fxs_status = TinyDB("./fxs_status")
sip_status = TinyDB("./sip_status")

# clean database because tiny insert will write next and repeat
system_setting.truncate()
users.truncate()
sip1_config.truncate()
network.truncate()
sip_extension.truncate()
audio_setting.truncate()
trunk.truncate()
status.truncate()
fxs_status.truncate()
sip_status.truncate()

# system_setting has untf -8
with open("../my_db/system_setting", encoding='utf-8') as system:
    file_contents = system.read()
    system.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    system_setting.insert(json.loads(tmp[i]))
with open('system_setting', encoding='utf-8') as fp:
     model = json.load(fp)
with open('system_setting', 'w', encoding='utf-8') as fp:
     fp.write(json.dumps(model))
# user
with open("../my_db/user_db") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    users.insert(json.loads(tmp[i]))
# sip1_config
with open("../my_db/sip1_db") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    sip1_config.insert(json.loads(tmp[i]))
# network
with open("../my_db/network_db") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    network.insert(json.loads(tmp[i]))
# sip_extension
with open("../my_db/sip_extension") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    sip_extension.insert(json.loads(tmp[i]))
# audio_setting
with open("../my_db/audio_setting") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    audio_setting.insert(json.loads(tmp[i]))
# trunk
with open("../my_db/trunk") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    trunk.insert(json.loads(tmp[i]))
# status
with open("../my_db/status") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    status.insert(json.loads(tmp[i]))
# fxs_status
with open("../my_db/fxs_status") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    fxs_status.insert(json.loads(tmp[i]))
# sip_status
with open("../my_db/sip_status") as system_setting:
    file_contents = system_setting.read()
    system_setting.close()
tmp = file_contents.splitlines()
for i in range(len(tmp)):
    sip_status.insert(json.loads(tmp[i]))
