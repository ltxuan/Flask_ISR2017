from sip_process import index_near_key
from tinydb import TinyDB, Query
from socket_process import get_eth_address
import os
import re
import builtins
import subprocess




def get_key_value(src, key, seperate, index, end):
    if key not in src:
        start_index = 0
        end_index = 0
    else:
        start_index = index_near_key(src, src.index(key) + len(key) - 1, seperate) + index
        end_index = index_near_key(src, src.index(key) + len(key) - 1, end) - 1
    return src[start_index:end_index]



def read_system():
    Init_system()
    read_network()
    read_pjsip_config()
    read_sip_conf()
    read_sip_setting()
    read_extension_conf()
    read_quagga()
    read_infor()

def Init_system():
    db_status = TinyDB('../NE_db/status')
    if db_status.get(Query().type== "dasboard") is not None:
        data = {'arm_ready': 'NO',
        'fpga_ready': 'NO',
        'ok_3v': 'OK',
        'ok_5v': 'OK',
        'arr': [],
        'cpu_temp': '0 oC',
        'temp': '0',
        'humd': '0'}
        db_status.update({'data': data}, Query().type== 'dasboard')
    else:
        data = {'arm_ready': 'NO',
        'fpga_ready': 'NO',
        'ok_3v': 'OK',
        'ok_5v': 'OK',
        'arr': [],
        'cpu_temp': '0 oC',
        'temp': '0',
        'humd': '0'}
        db_status.insert({'type': "dasboard",  'data': data})
    db_status.close()
    folder_path = '/etc/backup'
    list_backup = os.listdir(folder_path)

    db_system_setting = TinyDB('../NE_db/system_setting')
    if db_system_setting.get(Query().type== 'backup') is not None:
        send_arr = []
        for element in list_backup:
            name = element.replace('.zip', '')
            date = re.search(r'_(\d+_\d+_\d+)', name).group(1)
            date = date.replace('_', '/')
            send_arr.append({'file_name': name, 'date': date})
        db_system_setting.update({'data': send_arr}, Query().type== 'backup')
    else:
        db_system_setting.insert({'type': "backup",  'data': []})

    if db_system_setting.get(Query().type== 'infor') is not None:
        builtins.NODE_ID = db_system_setting.search(Query().type == 'infor')[0]['data']['node_id']
    else:
        builtins.NODE_ID = '999'
    # print(builtins.NODE_ID)

    subprocess.run(['/etc/init.d/snmpd', 'stop'], shell=True)

def read_GE(PortGE):
    db = TinyDB('../NE_db/network_db')
    with open('/etc/network/interfaces','r', encoding='utf-8') as file:
        read_data = file.read()
        read_data = read_data.strip()
        file.close()
    data = {
    'id': PortGE[2],
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
    if db.get(Query().name == PortGE) is not None:
        db.update({'data':data}, Query().name == PortGE)
    else:
        db.insert({'name': PortGE,  'data': data})
    db.close()

def read_Loopback():
    db = TinyDB('../NE_db/network_db')
    with open('/etc/network/interfaces','r', encoding='utf-8') as file:
        read_data = file.read()
        read_data = read_data.strip()
        file.close()
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
    if db.get(Query().name == 'LOOP_BACK') is not None:
        db.update({'data':data}, Query().name == 'LOOP_BACK')
    else:
        db.insert({'name': 'LOOP_BACK',  'data': data})
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
        file.close
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
    db_audio.close()


def read_sip_conf():
    with open('/etc/asterisk/sip.conf', 'r', encoding='utf-8') as file:
        read_data = file.read()
        file.close()
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
            # print('sip \n', data)
            db_sip_extension.insert(data)
        elif "context = public" in arr[i]:
            data = {}
            data['trunk_name'] = arr[i][arr[i].index('[') + 1: arr[i].index(']')]
            if sample_all_codec in arr[i]: data['codec'] = "all"
            else: data['codec'] = get_key_value(arr[i], "\nallow", '=', 1, '\n')
            if ';username' in  arr[i]: data['username'] = ""
            else: data['username'] = get_key_value(arr[i], "callerid", '=', 1, '\n')
            if ';type' in arr[i]: data['type'] = ""
            else: data['type'] = get_key_value(arr[i], "type", '=', 1, '\n')
            if ';secret' in arr[i]: data['secret'] = ""
            else: data['secret'] = get_key_value(arr[i], "secret", '=', 1, '\n')
            if ';dtmfmode' in arr[i]: data['dtmfmode'] = ""
            else: data['dtmfmode'] = get_key_value(arr[i], "dtmfmode", '=', 1, '\n')
            if ';host' in arr[i]: data['host'] = ""
            else: data['host'] = get_key_value(arr[i], "host", '=', 1, '\n')
            if ';fromdomain' in arr[i]: data['fromdomain'] = ""
            else: data['fromdomain'] = get_key_value(arr[i], "fromdomain", '=', 1, '\n')
            if ';nat' in arr[i]: data['nat'] = False
            else: data['nat'] = True
            if ';qualify' in arr[i]: data['quality'] = False
            else: data['quality'] = True
            data['transport'] = get_key_value(arr[i], "transport", '=', 1, '\n')
            if get_key_value(arr[i], "\nencryption", '=', 1, '\n') == 'yes':
                data['en_srtp'] = True
                data['srtp_mode'] = get_key_value(arr[i], "media_encryption", '=', 1, '\n')
            else: data['en_srtp'] = False
            # print('trunk =\n', data)
            db_trunk.insert(data)
    db_sip_extension.close()
    db_trunk.close()

def read_sip_setting():
    with open('/etc/asterisk/sip.conf', 'r', encoding='utf-8') as file:
        read_data = file.read()
        file.close()
    # general setting
    data = {}
    if "\nudpbindaddr=0.0.0.0:" in read_data: 
        data['udp_port'] = get_key_value(read_data, "\nudpbindaddr=0.0.0.0", ':', 0, ' ')
    else: data['udp_port'] = ""
    if '\n;tcpenable=' in read_data: data['en_tcp'] = False
    else: data['en_tcp'] = True
    data['tcp_port'] = get_key_value(read_data, "tcpbindaddr=0.0.0.0", ':', 0, ' ')
    if '\n;tlsenable=' in read_data: data['en_tls'] = False
    else: data['en_tls'] = True
    data['tls_port'] = get_key_value(read_data, "tlsbindaddr=0.0.0.0", ':', 0, ' ')
    data['tls_verify_server'] = get_key_value(read_data, "\ntlsdontverifyserver", '=', 0, '\n')
    data['tls_verify_client'] = get_key_value(read_data, "\ntlsdontverifyclient", '=', 0, '\n')
    data['tls_client_method'] = get_key_value(read_data, "\ntlsclientmethod", '=', 0, ' ')
    data['dtmf_mode'] = get_key_value(read_data, "\ndtmfmode=", '=', 0, ' ')
    if '\n;maxexpiry=' in read_data: data['max_regis'] = ""
    else: data['max_regis'] = get_key_value(read_data, "\nmaxexpiry", '=', 0, ' ')
    if '\n;minexpiry=' in read_data: data['min_regis'] = ""
    else: data['min_regis'] = get_key_value(read_data, "\minexpiry", '=', 0, ' ')
    if '\n;defaultexpiry=' in read_data: data['regis_time'] = ""
    else: data['regis_time'] = get_key_value(read_data, "\ndefaultexpiry", '=', 0, ' ')
    if '\n;registerattempts=' in read_data: data['regis_attempts'] = ""
    else: data['regis_attempts'] = get_key_value(read_data, "\nregisterattempts", '=', 0, ' ')
    if '\n;registertimeout=' in read_data: data['regis_timeout'] = ""
    else: data['regis_timeout'] = get_key_value(read_data, "\nregistertimeout", '=', 0, ' ')
    data['video_support'] = get_key_value(read_data, "\nvideosupport", '=', 0, ' ')
    data['dns_lookup'] = get_key_value(read_data, "\nsrvlookup", '=', 0, ' ')
    if '\n;useragent=' in read_data: data['user_agent'] = ""
    else: data['user_agent'] = get_key_value(read_data, "\nuseragent", '=', 0, ' ')
    with open('/etc/asterisk/rtp.conf', 'r', encoding='utf-8') as file:
        read_data_tmp = file.read()
        file.close()
    data['rtp_port_start'] = get_key_value(read_data_tmp, "\nrtpstart", '=', 0, '\n')
    data['rtp_port_end'] = get_key_value(read_data_tmp, "\nrtpend", '=', 0, '\n')
    db_system_setting = TinyDB('../NE_db/system_setting')
    db_system_setting.update({'data': data}, Query().type == "general_setting")
    # nat_setting
    data = {}
    if '\n;icesupport=' in read_data: data['en_stun'] = False
    else: data['en_stun'] = True
    if '\n;stunaddr=' in read_data: data['stun_address'] = ""
    else: data['stun_address'] = get_key_value(read_data, "\nstunaddr", '=', 0, '\n')
    if '\n;turnport=' in read_data: data['stun_port'] = ""
    else: data['stun_port'] = get_key_value(read_data, "\nturnport", '=', 0, '\n')
    if '\n;externaddr=' in read_data: data['ex_ip_address'] = ""
    else: data['ex_ip_address'] = get_key_value(read_data, "\nexternaddr", '=', 0, '\n')
    if '\n;externhost=' in read_data: data['ex_ip_host'] = ""
    else: data['ex_ip_host'] = get_key_value(read_data, "\nexternhost", '=', 0, '\n')
    if '\n;externrefresh=' in read_data: data['ex_refresh_interval'] = ""
    else: data['ex_refresh_interval'] = get_key_value(read_data, "\nexternrefresh", '=', 0, '\n')
    if '\n;localnet=' in read_data: data['local_net_id'] = ""
    else: data['local_net_id'] = get_key_value(read_data, "\nlocalnet", '=', 0, '\n')
    if '\n;nat=no\n' in read_data: data['nat_mode'] = 'no'
    else: data['nat_mode'] = 'yes'
    data['allow_rtp_re_invite'] = get_key_value(read_data, "\ncanreinvite", '=', 1, '\n').strip()
    db_system_setting.update({'data': data}, Query().type =="nat_setting")
    # codec_seting
    data = { 'allow_codec': [], 'avail_codec': [] }
    codec_list = read_data[read_data.index('start_codec\n') + 12 : read_data.index(';end_codec')]
    arr_codec = [
      'alaw',
      'ulaw',
      'g719',
      'g722',
      'g726',
      'g729',
      'gsm',
      'ilbc',
      'lpc10',
      'speex',
      'slin',
      'vp9',
      'vp8',
      'jpeg',
      'h264',
      'h263p',
      'h263',
      'h261'
    ]
    for element in arr_codec:
        if element in codec_list:
            data['allow_codec'].append(element)
        else:
            data['avail_codec'].append(element)
    db_system_setting.update({'data': data}, Query().type =="codec_setting")
    # qos_setting 
    data = {}
    if "\n;tos_sip=" in read_data: data['tos_sip'] = 'no'
    else: data['tos_sip'] = get_key_value(read_data, "\ntos_sip=", '=', 0, ' ')
    if "\n;cos_sip=" in read_data: data['cos_sip'] = 'no'
    else: data['cos_sip'] = get_key_value(read_data, "\ncos_sip=", '=', 0, ' ')
    if "\n;tos_audio=" in read_data: data['tos_audio'] = 'no'
    else: data['tos_audio'] = get_key_value(read_data, "\ntos_audio=", '=', 0, ' ')
    if "\n;cos_audio=" in read_data: data['cos_audio'] = 'no'
    else: data['cos_audio'] = get_key_value(read_data, "\ncos_audio=", '=', 0, ' ')
    if "\n;tos_video=" in read_data: data['tos_video'] = 'no'
    else: data['tos_video'] = get_key_value(read_data, "\ntos_video=", '=', 0, ' ')
    if "\n;cos_video=" in read_data: data['cos_video'] = 'no'
    else: data['cos_video'] = get_key_value(read_data, "\ncos_video=", '=', 0, ' ')
    db_system_setting.update({'data': data}, Query().type =="qos_setting")
    # t38_setting
    data = {}
    if "\n;t38pt_udptl=" in read_data:
        data['re_invite'] = 'no'
        data['max_data'] = ''
    else: data['re_invite'] = 'yes'
    if ',maxdatagram=' in read_data:
        data['max_data'] = get_key_value(read_data, ",maxdatagram=", '=', 0, ' ')
        data['err_correct'] = read_data[read_data.index("t38pt_udptl=yes,") + len("t38pt_udptl=yes,"): read_data.index(',maxdatagram=')]
    else:
        data['err_correct'] = get_key_value(read_data, 't38pt_udptl=yes,', ',', 0, ' ')
        data['max_data'] = ''
    db_system_setting.update({'data': data}, Query().type =="t38_setting")

    data = {}
    if ';trustrpid=' in read_data: 
        if ";sendrpid=" in read_data: data['remote_party_id'] = ""
        else: data['remote_party_id'] = 'send'
    else:
        data['remote_party_id'] = 'trust'
    data['allow_guest'] = get_key_value(read_data, "\nallowguest=", '=', 0, ' ')
    data['pedantic'] = get_key_value(read_data, "\npedantic=", '=', 0, ' ')
    data['alway_auth'] = get_key_value(read_data, "\nalwaysauthreject=", '=', 0, ' ')
    if "\n;session-timers=" in read_data: data['session_timers'] = 'default'
    else: data['session_timers'] = get_key_value(read_data, "\nsession-timers=", '=', 0, '\n')
    if "\n;session-expires=" in read_data: data['session_expires'] = ''
    else: data['session_expires'] = get_key_value(read_data, "\nsession-expires=", '=', 0, '\n')
    if "\n;session-minse=" in read_data: data['session_minse'] = ''
    else: data['session_minse'] = get_key_value(read_data, "\nsession-minse=", '=', 0, '\n')
    if "\n;session-refresher=" in read_data: data['session_refresher'] = 'default'
    else: data['session_refresher'] = get_key_value(read_data, "\nsession-refresher=", '=', 0, '\n')
    db_system_setting.update({'data': data}, Query().type =="advanced_setting")
    db_system_setting.close() 


def read_extension_conf():
    with open('/etc/asterisk/extensions.conf', 'r', encoding='utf-8') as file:
        read_data = file.read()
        file.close()
    public = read_data[read_data.index("[public]"): read_data.index("[default]")]
    # dial_plan
    data = {}
    arr_public = public.split('\n')
    data['dial_plan'] = ""
    for line in arr_public:
        if 'exten => _' in line:
            data['dial_plan'] += ';' + get_key_value(line, "\nexten => _", '_', 0, ',')
    data['dial_plan'] = data['dial_plan'].replace(';', '')
    temp = read_data[read_data.index(";my_dial_plan") : read_data.index(";end_my_dial_plan") + 18]
    data['trunk_name'] = get_key_value(temp, "Dial(SIP/${EXTEN}@", '@', 0, ',')
    data['ring_time'] = get_key_value(temp, "Dial(SIP/${EXTEN},", ',', 0, ')')
    if ";exten => _[0-4]XXXXX.,1,Ringing\n" in read_data:
        data['c'] = True
        data['ab'] = False
    else:
        data['c'] = False
        data['ab'] = True
    with open('/etc/pjsip/config', 'r', encoding='utf-8') as file:
        sip_config = file.read()
        file.close()
    data['digit1'] = get_key_value(sip_config, "digit1", "=", 0, "\n")
    data['digit2'] = get_key_value(sip_config, "digit2", "=", 0, "\n")
    data['digit3'] = get_key_value(sip_config, "digit3", "=", 0, "\n")
    data['digit4'] = get_key_value(sip_config, "digit4", "=", 0, "\n")
    data['digit5'] = get_key_value(sip_config, "digit5", "=", 0, "\n")
    data['digit6'] = get_key_value(sip_config, "digit6", "=", 0, "\n")
    data['digit7'] = get_key_value(sip_config, "digit7", "=", 0, "\n")
    data['digit8'] = get_key_value(sip_config, "digit8", "=", 0, "\n")
    data['digit9'] = get_key_value(sip_config, "digit9", "=", 0, "\n")
    data['digit0'] = get_key_value(sip_config, "digit0", "=", 0, "\n")
    db_system_setting = TinyDB('../NE_db/system_setting')
    db_system_setting.update({'data': data}, Query().type =="dial_plan")
    # call forward
    data = {}
    call_forward = read_data[read_data.index(';call_forwarding\n') : read_data.index(';end_call_forwarding\n')]
    arr_forward = call_forward.split('\n')
    data['en_all_call'] = False
    data['dis_all_call'] = False
    data['en_busy'] = False
    data['dis_busy'] = False
    data['en_all_call_value'] = ""
    data['dis_all_call_value'] = ""
    data['en_busy_value'] = ""
    data['dis_busy_value'] = ""
    for element in arr_forward:
        if 'Set(DB(CFIM/${CALLERID(num)})=${REPLACE(EXTEN:4,*#-)})' in element:
            data['en_all_call'] = True
            data['en_all_call_value'] = get_key_value(element, "exten = _", '_', 0, 'X')
        if ',1,DBdel(CFIM/${CALLERID(num)})' in element:
            data['dis_all_call'] = True
            data['dis_all_call_value'] = get_key_value(element, "exten =", '=', 1, ',')
        if 'X.,1,Set(DB(CFBS/${CALLERID(num)})=${REPLACE(EXTEN:4,*#-)})' in element:
            data['en_busy'] = True
            data['en_busy_value'] = get_key_value(element, "exten = _", '_', 0, 'X')
        if ',1,DBdel(CFBS/${CALLERID(num)})' in element:
            data['dis_busy'] = True
            data['dis_busy_value'] = get_key_value(element, "exten =", '=', 1, ',')
    db_system_setting.update({'data': data}, Query().type =="call_forward")
    db_system_setting.close()

def read_quagga():
    data = { 'OSPF': {}, 'RIP': {} }
    with open('/etc/quagga/ospfd.conf', 'r', encoding='utf-8') as file:
        read_data = file.read()
        file.close()
    with open('/etc/sysctl.conf', 'r', encoding='utf-8') as file:
        file_sysctr = file.read()
        file.close()
    if '#net.ipv4.ip_forward=1' not in file_sysctr:
        tmp = read_data[read_data.index("router ospf"): read_data.index("log stdout")]
        arr_network = tmp.split('\n')
        arr_network = list(filter(lambda a: 'network' in a, arr_network))
        for element in arr_network:
            element = element.replace('network', '').strip()
        data['OSPF']['ipv4'] = True
        data['OSPF']['ipv6'] = False
        try: data['OSPF']['net_work_1'] = arr_network[0]
        except IndexError: data['OSPF']['net_work_1'] = " area "
        try: data['OSPF']['net_work_2'] = arr_network[1]
        except IndexError: data['OSPF']['net_work_2'] = " area "
        try: data['OSPF']['net_work_3'] = arr_network[2]
        except IndexError: data['OSPF']['net_work_3'] = " area "
        try: data['OSPF']['net_work_4'] = arr_network[3]
        except IndexError: data['OSPF']['net_work_4'] = " area "
        try: data['OSPF']['net_work_5'] = arr_network[4]
        except IndexError: data['OSPF']['net_work_5'] = " area "
        
        if '!redistribute rip' in tmp: data['OSPF']['rip'] = False
        else: data['OSPF']['rip'] = True
        if '!redistribute static' in tmp: data['OSPF']['static'] = False
        else: data['OSPF']['static'] = True
        if'!redistribute kernel' in tmp: data['OSPF']['kernel'] = False
        else: data['OSPF']['kernel'] = True
        if '!redistribute connected' in tmp: data['OSPF']['connected'] = False
        else: data['OSPF']['connected ']= True
    else:
        with open('/etc/quagga/ospf6d.conf', 'r', encoding='utf-8') as file:
            read_data = file.read()
            file.close()
        data['OSPF']['ipv4'] = False
        data['OSPF']['ipv6'] = True
        tmp = read_data[read_data.index("!start_net_work"): read_data.index("!end_net_work")]
        list_ipv6 = tmp.split('interface ')
        try: 
            eth = list_ipv6[1][0: 4].strip()
            ip = get_eth_address(eth)
            area = list_ipv6[1][list_ipv6[1].index('0.0.0.') + len('0.0.0.'): index_near_key(list_ipv6[1], list_ipv6[1].index('0.0.0.'), '\n') - 1]
            data['OSPF']['net_work_1'] = ip + '/' + ' area ' + area
        except IndexError: data['OSPF']['net_work_1'] = ' area '
        try: 
            eth = list_ipv6[2][0: 4].strip()
            ip = get_eth_address(eth)
            area = list_ipv6[2][list_ipv6[2].index('0.0.0.') + len('0.0.0.'): index_near_key(list_ipv6[2], list_ipv6[2].index('0.0.0.'), '\n') - 1]
            data['OSPF']['net_work_2'] = ip + '/' + ' area ' + area
        except IndexError: data['OSPF']['net_work_2'] = ' area '
        try: 
            eth = list_ipv6[3][0: 4].strip()
            ip = get_eth_address(eth)
            area = list_ipv6[3][list_ipv6[3].index('0.0.0.') + len('0.0.0.'): index_near_key(list_ipv6[3], list_ipv6[3].index('0.0.0.'), '\n') - 1]
            data['OSPF']['net_work_3'] = ip + '/' + ' area ' + area
        except IndexError: data['OSPF']['net_work_3'] = ' area '
        try: 
            eth = list_ipv6[4][0: 4].strip()
            ip = get_eth_address(eth)
            area = list_ipv6[4][list_ipv6[4].index('0.0.0.') + len('0.0.0.'): index_near_key(list_ipv6[4], list_ipv6[4].index('0.0.0.'), '\n') - 1]
            data['OSPF']['net_work_4'] = ip + '/' + ' area ' + area
        except IndexError: data['OSPF']['net_work_4'] = ' area '
        try: 
            eth = list_ipv6[5][0: 4].strip()
            ip = get_eth_address(eth)
            area = list_ipv6[5][list_ipv6[5].index('0.0.0.') + len('0.0.0.'): index_near_key(list_ipv6[5], list_ipv6[5].index('0.0.0.'), '\n') - 1]
            data['OSPF']['net_work_5'] = ip + '/' + ' area ' + area
        except IndexError: data['OSPF']['net_work_5'] = ' area '
        if '!redistribute rip' in tmp: data['OSPF']['rip'] = False
        else: data['OSPF']['rip'] = True
        if '!redistribute static' in tmp: data['OSPF']['static'] = False
        else: data['OSPF']['static'] = True
        if'!redistribute kernel' in tmp: data['OSPF']['kernel'] = False
        else: data['OSPF']['kernel'] = True
        if '!redistribute connected' in tmp: data['OSPF']['connected'] = False
        else: data['OSPF']['connected ']= True
    db_system_setting = TinyDB('../NE_db/system_setting')
    if db_system_setting.get(Query().type== 'quagga_setting') is not None:
        db_system_setting.update({'OSPF': data['OSPF']}, Query().type== 'quagga_setting')
    else:
        db_system_setting.insert({'type': "quagga_setting",  'OSPF': data['OSPF']})
    
    with open('/etc/quagga/ripd.conf', 'r', encoding='utf-8') as file:
        read_data = file.read()
        file.close()
    if '#net.ipv4.ip_forward=1' not in file_sysctr:
        tmp = read_data[read_data.index("router rip"): read_data.index("log stdout")]
        arr_network = tmp.split('\n')
        arr_network = list(filter(lambda a: 'network' in a, arr_network))
        for element in arr_network:
            element = element.replace('network', '').strip()
        data['RIP']['ipv4'] = True
        data['RIP']['ipv6'] = False
        try: data['RIP']['net_work_1'] = arr_network[0]
        except IndexError: data['RIP']['net_work_1'] = ""
        try: data['RIP']['net_work_2'] = arr_network[1]
        except IndexError: data['RIP']['net_work_2'] = ""
        try: data['RIP']['net_work_3'] = arr_network[2]
        except IndexError: data['RIP']['net_work_3'] = ""
        try: data['RIP']['net_work_4'] = arr_network[3]
        except IndexError: data['RIP']['net_work_4'] = ""
        try: data['RIP']['net_work_5'] = arr_network[4]
        except IndexError: data['RIP']['net_work_5'] = ""

        if '!redistribute ospf' in tmp: data['RIP']['ospf'] = False
        else: data['RIP']['ospf'] = True
        if '!redistribute static' in tmp: data['RIP']['static'] = False
        else: data['RIP']['static'] = True
        if'!redistribute kernel' in tmp: data['RIP']['kernel'] = False
        else: data['RIP']['kernel'] = True
        if '!redistribute connected' in tmp: data['RIP']['connected'] = False
        else: data['RIP']['connected ']= True
    else:
        with open('/etc/quagga/ripngd.conf', 'r', encoding='utf-8') as file:
            read_data = file.read()
            file.close()
        tmp = read_data[read_data.index("router ripng"): read_data.index("log stdout")]
        list_tmp = tmp.split('\n')
        data['RIP']['ipv4'] = False
        data['RIP']['ipv6'] = True
        list_tmp = list(filter(lambda a: 'network' in a, list_tmp))

        print(list_tmp)
        try: 
            eth = list_tmp[0][list_tmp[0].index('eth'):].strip()
            ip = get_eth_address(eth)
            data['RIP']['net_work_1'] = ip + '/'
        except IndexError:data['RIP']['net_work_1'] = ''
        try: 
            eth = list_tmp[1][list_tmp[1].index('eth'):].strip()
            ip = get_eth_address(eth)
            data['RIP']['net_work_2'] = ip + '/'
        except IndexError:data['RIP']['net_work_2'] = ''
        try: 
            eth = list_tmp[2][list_tmp[2].index('eth'):].strip()
            ip = get_eth_address(eth)
            data['RIP']['net_work_3'] = ip + '/'
        except IndexError:data['RIP']['net_work_3'] = ''
        try: 
            eth = list_tmp[3][list_tmp[3].index('eth'):].strip()
            ip = get_eth_address(eth)
            data['RIP']['net_work_4'] = ip + '/'
        except IndexError:data['RIP']['net_work_4'] = ''
        try: 
            eth = list_tmp[4][list_tmp[4].index('eth'):].strip()
            ip = get_eth_address(eth)
            data['RIP']['net_work_5'] = ip + '/'
        except IndexError:data['RIP']['net_work_5'] = ''
        if '!redistribute ospf' in tmp: data['RIP']['ospf'] = False
        else: data['RIP']['ospf'] = True
        if '!redistribute static' in tmp: data['RIP']['static'] = False
        else: data['RIP']['static'] = True
        if'!redistribute kernel' in tmp: data['RIP']['kernel'] = False
        else: data['RIP']['kernel'] = True
        if '!redistribute connected' in tmp: data['RIP']['connected'] = False
        else: data['RIP']['connected ']= True
    # print(data)
    if db_system_setting.get(Query().type== 'quagga_setting') is not None:
        db_system_setting.update({'RIP': data['RIP']}, Query().type== 'quagga_setting')
    else:
        db_system_setting.insert({'type': "quagga_setting",  'RIP': data['RIP']})
    db_system_setting.close()


def read_infor():
    with open('/etc/informations.txt', 'r', encoding='utf-8') as file:
        read_data = file.read()
        file.close()
    data = {}
    data['node_id'] = get_key_value(read_data, "Node id ", ":", 1, "\n").strip()
    data['who_apply'] = get_key_value(read_data, "Don vi/ Luc luong trien khai", ":", 1, "\n").strip()
    data['who_use'] = get_key_value(read_data, "Don vi su dung", ":", 1, "\n").strip()
    data['date_apply'] = get_key_value(read_data, "Ngay trien khai", ":", 1, "\n").strip()
    data['number_infor'] = read_data[read_data.index("So thue bao su dung va thong tin cac thue bao : ") + len("So thue bao su dung va thong tin cac thue bao : "): read_data.index("Thong tin khac :") - 1]
    data['other_infor'] = read_data[read_data.index("Thong tin khac :") + len("Thong tin khac :"):].strip()
    # print(data)
    db_system_setting = TinyDB('../NE_db/system_setting')
    if db_system_setting.get(Query().type== 'infor') is not None:
        db_system_setting.update({'data': data}, Query().type== 'infor')
    else:
        db_system_setting.insert({'type': "infor",  'data': data})
    db_system_setting.close()
    




    
    
    

    

    
      