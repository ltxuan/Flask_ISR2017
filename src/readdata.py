from sip_process import index_near_key
from tinydb import TinyDB, Query

def get_key_value(src, key, seperate, index, end):
    start_index = index_near_key(src, src.index(key) + len(key) - 1, seperate) + index
    end_index = index_near_key(src, src.index(key) + len(key) - 1, end) - 1
    return src[start_index:end_index]



def read_system():
    # read_network()
    # read_pjsip_config()
    read_sip_conf()
def read_GE(PortGE):
    db = TinyDB('../NE_db/network_db')
    with open('/etc/network/interfaces','r', encoding='utf-8') as file:
        read_data = file.read()
        read_data = read_data.strip()
    data = {
    'id': 1,
    'ipv4': False,
    'ipv6': False,
    'dhcp': False,
    'ipaddress': '',
    'netmask': '',
    'gateway': '',
    'primaryDNS': '',
    'secondaryDNS': ''
    }
    if PortGE + '\n' in read_data:
        GE = read_data[read_data.index('#'+ PortGE +'\n') + len('#'+ PortGE +'\n'):read_data.index('#END' + PortGE)]
        if 'inet dhcp' in GE:
            data['ipv4'] = True
            data['dhcp'] = True
        elif 'inet6 dhcp' in GE:
            data['ipv6'] = True
            data['dhcp'] = True
        elif 'inet static' in GE:
            data['ipv4'] = True
            data['dhcp'] = False
            data['ipaddress'] = get_key_value(GE, 'address', ' ', 0, '\n')
            data['netmask'] = get_key_value(GE, 'netmask', ' ', 0, '\n')
            if 'gateway' in GE:
                data['gateway'] = get_key_value(GE, 'gateway', ' ', 0, '\n')
            if 'dns-nameservers' in GE:
                data['primaryDNS'] = GE[GE.index('dns-nameservers ') + len('dns-nameservers '):].split(' ')[0]
                data['secondaryDNS'] = get_key_value(GE, data['primaryDNS'], ' ', 0, '\n')
        elif 'inet6 static' in GE:
            data['ipv6'] = True
            data['dhcp'] = False
            data['ipaddress'] = get_key_value(GE, 'address', ' ', 0, '\n')
            data['netmask'] = get_key_value(GE, 'netmask', ' ', 0, '\n')
            if 'gateway' in GE:
                data['gateway'] = get_key_value(GE, 'gateway', ' ', 0, '\n')
            if 'dns-nameservers' in GE:
                data['primaryDNS'] = GE[GE.index('dns-nameservers ') + len('dns-nameservers '):].split(' ')[0]
                data['secondaryDNS'] = get_key_value(GE, data['primaryDNS'], ' ', 0, '\n')
    # Ghi dữ liệu vào cơ sở dữ liệu (database)
    db.update({'data':data}, Query().name == PortGE)
    db.close()

def read_Loopback():
    db = TinyDB('../NE_db/network_db')
    with open('/etc/network/interfaces','r', encoding='utf-8') as file:
        read_data = file.read()
        read_data = read_data.strip()
    data = {
    "id": 5,
    "ipaddress": "",
    "netmask": ""
    }
    LOOP = read_data[read_data.index("#LOOP_BACK"):read_data.index("#E_LOOP")]
    if "#LOOP_BACK" in read_data:
        data["ipaddress"] = get_key_value(LOOP, "address", " ", 0, "\n")
        data["netmask"] = get_key_value(LOOP, "netmask", " ", 0, "\n")
    # print('loop = \n', data)
    db.update({'data':data}, Query().name == "LOOP_BACK")
    db.close()

def read_network():
    read_GE('GE1')
    read_GE('GE2')
    read_GE('GE3')
    read_GE('GE4')
    read_Loopback()

def read_pjsip_config():
    with open('/etc/pjsip/config', 'r', encoding='utf-8') as file:
        read_data = file.read()
    arr = read_data.split('#account')
    db = TinyDB("../NE_db/sip1_db")
    db.truncate()
    for i in range(1, len(arr)):
        data = {}
        data['id'] = str(i - 1)
        if get_key_value(arr[i], "status", '=', 0, '\n') == '1':
            data['status'] = 'checked'
        else:
            data['status'] = ''
        if get_key_value(arr[i], "h_status", '=', 0, '\n') == '1':
            data['h_status'] = 'checked'
        else:
            data['h_status'] = ''
        data['caller_id'] = get_key_value(arr[i], "username", '=', 0, '\n')
        data['domain'] = get_key_value(arr[i], "domain", '=', 0, '\n')
        data['port'] = get_key_value(arr[i], "sip_port", '=', 0, '\n')
        data['hotline'] = get_key_value(arr[i], "hotline", '=', 0, '\n')
        data['transport'] = get_key_value(arr[i], "transport", '=', 0, '\n')
        data['password'] = get_key_value(arr[i], "secret", '=', 0, '\n')
        data['sip_uri'] = "sip:" + data['caller_id'] + "@" + data['domain'] + ":" + data['port']
        db.insert(data)
    db.close()
    data = {'priority': []}
    start_index = read_data.find('#codec_set')
    end_index = read_data.find('#end_codec_set')
    codec = read_data[start_index + len('#codec_set') + 1:end_index - 1]
    list_codec = codec.split('\n')
    
    for item in list_codec:
        equal_index = item.find('=')
        
        if equal_index != -1:
            value = item[equal_index + 1:]
            
            if value.isdigit() and value != '0':
                key = item[:equal_index]
                data["priority"].insert(100 - int(value), key)
    data['priotiy_change'] = False
    data['dtmf_change'] = False
    val = get_key_value(read_data, 'dtmf', '=', 0, '\n')
    if val == 'inband':
        data['in_band'] = True
        data['rc'] = False
        data['info'] = False
    elif val == 'rfc2833':
        data['in_band'] = False
        data['rc'] = True
        data['info'] = False
    elif val == 'info':
        data['in_band'] = False
        data['rc'] = False
        data['info'] = True
    data['srtp_change'] = False
    data['srtp_mode'] = get_key_value(read_data, "use_srtp", '=', 0, '\n')
    data['echo_change'] = False
    if get_key_value(read_data, "echo_enable", '=', 0, '\n') == '1':
        data['echo'] = True
    else: data['echo'] = False
    db_audio = TinyDB("../NE_db/audio_setting")
    db_audio.update({'data':data}, Query().type == "audio_setting")


def read_sip_conf():
    with open('/etc/asterisk/sip.conf', 'r', encoding='utf-8') as file:
        read_data = file.read()
    arr = read_data.replace(";sip_account\n", ', ').replace(";trunk\n", ', ').split(', ')
    db_sip_extension = TinyDB('../NE_db/sip_extension ')
    db_trunk = TinyDB('../NE_db/trunk')
    db_sip_extension.truncate()
    db_trunk.truncate()
    sample_all_codec = 'disallow = all\n' +\
    'allow = ulaw\n' +\
    'allow = alaw\n' +\
    'allow = g729\n' +\
    'allow = g722\n'
    for i in range(len(arr)):
        if "context = myphones" in arr[i]:
            data = {}
            data['name'] = arr[i][arr[i].index('[') +1 : arr[i].index(']')]
            data['callerid'] = get_key_value(arr[i], "callerid", '=', 1, '\n')
            data['secret'] = get_key_value(arr[i], "secret", '=', 1, '\n')
            data['dtmfmode'] = get_key_value(arr[i], "dtmfmode", '=', 1, '\n')
            data['mailbox'] = get_key_value(arr[i], "mailbox", '=', 1, '\n')

            data['nat'] = get_key_value(arr[i], "nat", '=', 1, '\n')
            data['qualify'] = get_key_value(arr[i], "qualify", '=', 1, '\n')
            data['transport'] = get_key_value(arr[i], "transport", '=', 1, '\n')
            data['srtp_mode'] = 'sdes'
            if get_key_value(arr[i], "\nencryption", '=', 1, '\n') == 'yes':
                data['srtp_enable'] = True
                data['srtp_mode'] = get_key_value(arr[i], "media_encryption", '=', 1, '\n')
            else:
                data['srtp_enable'] = False
            if sample_all_codec in arr[i]: data['codec'] = 'all'
            else: data['codec'] = get_key_value(arr[i], "\nallow", '=', 1, '\n')
            print('sip \n', data)
    #         db.sip_extension.insert([data], function (err) {
    #   });
      
      