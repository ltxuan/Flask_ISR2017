from sip_process import index_near_key
from tinydb import TinyDB, Query

def get_key_value(src, key, separate, index, end):
    start_index = index_near_key(src, src.index(key) + len(key) - 1, separate) + index
    end_index = index_near_key(src, src.index(key) + len(key) - 1, end) - 1
    return src[start_index:end_index]

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
    print(data)
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
            read_data = read_data.replace('\n', '')
    arr = read_data.split('#account')
    db = TinyDB("../NE_db/sip1_db")
    db.truncate()
