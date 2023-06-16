import re
import asyncio

def index_near_key(src, start, key):
    i = 0
    while i < 1000:
        if src[start + i] == key:
            return start + i + 1
        else:
            i += 1

def modify_key_custom(src, key, separate, index, value):
    tmp = src[src.index(key):index_near_key(src, src.index(key), '\n')]
    tmp1 = src[index_near_key(src, src.index(key), separate) + index:index_near_key(src, src.index(key), '\n')]
    tmp2 = tmp.replace(tmp1, value) + '\n'
    return src.replace(tmp, tmp2)

def add_sip(data):
    with open('/etc/asterisk/sip.conf','r', encoding='utf-8') as file:
        read_data = file.read()
    arr = re.split(r'\n;trunk\n', read_data)
    sip_config = "type = friend\n\
callerid = ATA <958200>\n\
secret = 958200\n\
host = dynamic\n\
canreinvite = no\n\
dtmfmode = rfc2833\n\
mailbox = 958200\n\
nat = yes\n\
qualify = yes\n\
"
    write_data = "\n;sip_account\n[" + data['name'] + "]\n" + sip_config
    write_data = modify_key_custom(write_data, "callerid", "=", 1, data['callerid'])
    write_data = modify_key_custom(write_data, "secret", "=", 1, data['secret'])
    write_data = modify_key_custom(write_data, "dtmfmode", "=", 1, data['dtmfmode'])
    if data['en_voice_mail']:
        write_data = modify_key_custom(write_data, "mailbox", "=", 1, data['mailbox'])
    else:
        write_data = write_data.replace('mailbox = 958200\n', '')
    write_data = modify_key_custom(write_data, "nat", "=", 1, data['nat'])
    write_data = modify_key_custom(write_data, "qualify", "=", 1, data['qualify'])
    if data['codec'] == "all" :
        write_data = write_data + "disallow = all\n\
allow = ulaw\n\
allow = alaw\n\
allow = g729\n\
allow = g722\n\
transport = udp\n\
encryption = yes\n\
media_encryption = sdes\n\
media_encryption = dtls\n\
context = myphones\n"
    else:
        write_data = write_data + "disallow = all\nallow = " + data['codec'] + "\ntransport = udp\nencryption = yes\nmedia_encryption = sdes\nmedia_encryption = dtls\ncontext = myphones\n"
    write_data = modify_key_custom(write_data, "transport", "=", 1, data['transport'])
    if data['srtp_enable']:
        write_data = write_data = modify_key_custom(write_data, "encryption", "=", 1, "yes")
        if data['srtp_mode'] == "sdes":
            write_data = write_data.replace("media_encryption = dtls", ";media_encryption = dtls")
        elif data['srtp_mode'] == "dtls": write_data = write_data.replace("media_encryption = sdes", ";media_encryption = sdes")
    else:
        write_data = modify_key_custom(write_data, "encryption", "=", 1, "no")
        write_data = write_data.replace("media_encryption = dtls", ";media_encryption = dtls")
        write_data = write_data.replace("media_encryption = sdes", ";media_encryption = sdes")
    print(write_data)
    write_data = arr[0] + write_data
    write_data = read_data.replace(arr[0], write_data)
    with open('/etc/asterisk/sip.conf','w', encoding='utf-8') as file:
        file.write(write_data)
# Modify Sip acount function
def modify_sip(data):
    with open('/etc/asterisk/sip.conf','r', encoding='utf-8') as file:
        read_data = file.read()
    arr = read_data.replace(";sip_account\n", ', ').replace(';trunk\n', ', ').split(', ')
    sip_config = "type = friend\n\
callerid = ATA <958200>\n\
secret = 958200\n\
host = dynamic\n\
canreinvite = no\n\
dtmfmode = rfc2833\n\
mailbox = 958200\n\
nat = yes\n\
qualify = yes\n\
"
    for i in range(len(arr)):
        if "[" + data['name'] + "]" in arr[i]:
            write_data = "[" + data['name'] + "]\n" + sip_config
            write_data = modify_key_custom(write_data, "callerid", "=", 1, data['callerid'])
            write_data = modify_key_custom(write_data, "secret", "=", 1, data['secret'])
            write_data = modify_key_custom(write_data, "dtmfmode", "=", 1, data['dtmfmode'])
            if data['en_voice_mail']: write_data = modify_key_custom(write_data, "mailbox", "=", 1, data['mailbox']);
            else: 
                write_data = write_data.replace('mailbox = 958200\n', '')
            write_data = modify_key_custom(write_data, "nat", "=", 1, data['nat'])
            write_data = modify_key_custom(write_data, "qualify", "=", 1, data['qualify'])
            if data['codec'] == "all":
                write_data = write_data + "disallow = all\n\
allow = ulaw\n\
allow = alaw\n\
allow = g729\n\
allow = g722\n\
transport = udp\n\
encryption = yes\n\
media_encryption = sdes\n\
media_encryption = dtls\n\
context = myphones\n\
"
            else:
                write_data = write_data + "disallow = all\nallow = " + data['codec'] + "\ntransport = udp\nencryption = yes\nmedia_encryption = sdes\nmedia_encryption = dtls\ncontext = myphones\n\n"
        
            write_data = modify_key_custom(write_data, "transport", "=", 1, data['transport'])
            if data['srtp_enable']:
                write_data = modify_key_custom(write_data, "encryption", "=", 1, "yes")
                if data['srtp_mode'] == "sdes":
                    write_data = write_data.replace("media_encryption = dtls", ";media_encryption = dtls")
                elif data['srtp_mode'] == "dtls":
                    write_data = write_data.replace("media_encryption = sdes", ";media_encryption = sdes")
            else:
                write_data = modify_key_custom(write_data, "encryption", "=", 1, "no")
                write_data = write_data.replace("media_encryption = dtls", ";media_encryption = dtls")
                write_data = write_data.replace("media_encryption = sdes", ";media_encryption = sdes")
            write_data = read_data.replace(arr[i], write_data)
            with open('/etc/asterisk/sip.conf','w', encoding='utf-8') as file:
                file.write(write_data)


def delete_sip(data):
    with open('/etc/asterisk/sip.conf','r', encoding='utf-8') as file:
        read_data = file.read()
    arr = read_data.replace(";sip_account\n", ', ').replace(';trunk\n', ', ').split(', ')
    for i in range(len(arr)):
        if "[" + data['name'] + "]" in arr[i]:
            print(data['name'])
            write_data = read_data.replace(";sip_account\n[" + data['name'] + "]", "[" + data['name'] + "]")
            write_data = write_data.replace(arr[i], "")
            with open('/etc/asterisk/sip.conf','w', encoding='utf-8') as file:
                file.write(write_data)
# Cac ham xu ly trunk
def add_trunk(data):
    with open('/etc/asterisk/sip.conf','r', encoding='utf-8') as file:
        read_data = file.read()
    trunk_config = 'username = htc-isr2017\n' +\
    'type = peer\n' +\
    'secret = 123456\n' +\
    'host = 192.168.2.1\n' +\
    'fromdomain = 192.168.2.1\n' +\
    'dtmfmode = rfc2833\n'
    write_data = "\n;trunk\n[" + data['trunk_name'] + "]\n" + trunk_config
    if data['codec'] == 'all':
        if 'disallow = all' in write_data:
            write_data = write_data + 'disallow = all\n' +\
            'allow = ulaw\n' +\
            'allow = alaw\n' +\
            'allow = g729\n' +\
            'allow = g722\n'
    else:
        if "disallow = all" in write_data:
            write_data = write_data + "allow = " + data['codec'] + "\n"            
    write_data = write_data + 'transport = udp\n' +\
    'nat=yes\n' +\
    'qualify = yes\n' +\
    'encryption = yes\n'
    if data['username'] != "":
        write_data = modify_key_custom(write_data, "username", "=", 1, data['username'])
    else:
        write_data = write_data.replace("username", ";username")
    if data['type'] != "":
        write_data = modify_key_custom(write_data, "type", "=", 1, data['type'])
    else:
        write_data = write_data.replace("type", ";type")
    if data['secret'] != "":
        write_data = modify_key_custom(write_data, "secret", "=", 1, data['secret'])
    else:
        write_data = write_data.replace("secret", ";secret")
    if data['dtmfmode'] != "":
        write_data = modify_key_custom(write_data, "dtmfmode", "=", 1, data['dtmfmode'])
    else:
        write_data = write_data.replace("dtmfmode", ";dtmfmode")
    if data['host'] != "":
        write_data = modify_key_custom(write_data, "host", "=", 1, data['host'])
    else:
         write_data = write_data.replace("host", ";host")
    if data['fromdomain'] != "":
        write_data = modify_key_custom(write_data, "fromdomain", "=", 1, data['fromdomain'])
    else:
        write_data = write_data.replace("fromdomain", ";fromdomain")
    if data['nat'] is None:
        write_data = write_data.replace('nat', ';nat')
    if data['quality'] is None:
        write_data = write_data.replace('qualify', ';qualify')
    if data['en_srtp']:
        write_data = write_data + 'media_encryption = ' + data['srtp_mode'] + '\n' 
    else:
        write_data = modify_key_custom(write_data, "encryption", "=", 1, 'no')
    write_data = modify_key_custom(write_data, "transport", "=", 1, data['transport'])
    write_data = write_data + 'context = public\n'
    print(write_data)
    write_data = read_data + write_data
    with open('/etc/asterisk/sip.conf','w', encoding='utf-8') as file:
        file.write(write_data)

def modify_trunk(data):
    with open('/etc/asterisk/sip.conf','r', encoding='utf-8') as file:
        read_data = file.read()
    arr = read_data.replace(";sip_account\n", ', ').replace(';trunk\n', ', ').split(', ')
    str_check = "[" + data['trunk_name'] + "]"
    for i in reversed(range(len(arr))):
        if str_check in arr[i]:
            trunk_config = 'username = htc-isr2017\n' +\
            'type = peer\n' +\
            'secret = 123456\n' +\
            'host = 192.168.2.1\n' +\
            'fromdomain = 192.168.2.1\n' +\
            'dtmfmode = rfc2833\n'
            write_data = "[" + data['trunk_name'] + "]\n" + trunk_config   
            if data['codec'] == 'all':
                if 'disallow = all' not in write_data:
                    write_data = write_data + 'disallow = all\n' +\
                    'allow = ulaw\n' +\
                    'allow = alaw\n' +\
                    'allow = g729\n' +\
                    'allow = g722\n'
            else:
                if "disallow = all" not in write_data:
                    write_data = write_data + "allow = " + data['codec'] + "\n"            
            write_data = write_data + 'transport = udp\n' +\
            'nat=yes\n' +\
            'qualify = yes\n' +\
            'encryption = yes\n'
            if data['username'] != "":
                write_data = modify_key_custom(write_data, "username", "=", 1, data['username'])
            else:
                write_data = write_data.replace("username", ";username")
            if data['type'] != "":
                write_data = modify_key_custom(write_data, "type", "=", 1, data['type'])
            else:
                write_data = write_data.replace("type", ";type")
            if data['secret'] != "":
                write_data = modify_key_custom(write_data, "secret", "=", 1, data['secret'])
            else:
                write_data = write_data.replace("secret", ";secret")
            if data['dtmfmode'] != "":
                write_data = modify_key_custom(write_data, "dtmfmode", "=", 1, data['dtmfmode'])
            else:
                write_data = write_data.replace("dtmfmode", ";dtmfmode")
            if data['host'] != "":
                write_data = modify_key_custom(write_data, "host", "=", 1, data['host'])
            else:
                write_data = write_data.replace("host", ";host")
            if data['fromdomain'] != "":
                write_data = modify_key_custom(write_data, "fromdomain", "=", 1, data['fromdomain'])
            else:
                write_data = write_data.replace("fromdomain", ";fromdomain")
            if data['nat'] is None:
                write_data = write_data.replace('nat', ';nat')
            if data['quality'] is None:
                write_data = write_data.replace('qualify', ';qualify')
            if data['en_srtp']:
                write_data = write_data + 'media_encryption = ' + data['srtp_mode'] + '\n' 
            else:
                write_data = modify_key_custom(write_data, "encryption", "=", 1, 'no')
            write_data = modify_key_custom(write_data, "transport", "=", 1, data['transport'])
            write_data = write_data + 'context = public\n'
            write_data = read_data.replace(arr[i], write_data)
            with open('/etc/asterisk/sip.conf','w', encoding='utf-8') as file:
                file.write(write_data) 

def delete_trunk(data):
    with open('/etc/asterisk/sip.conf','r', encoding='utf-8') as file:
        read_data = file.read()
    arr = read_data.replace(";sip_account\n", ', ').replace(';trunk\n', ', ').split(', ')
    str_check = "[" + data['trunk_name'] + "]"
    for i in reversed(range(len(arr))):
        if str_check in arr[i]:
            write_data = read_data.replace(";trunk\n[" + data['trunk_name'] + "]", "[" + data['trunk_name'] + "]")
            write_data = write_data.replace(arr[i], "")
            with open('/etc/asterisk/sip.conf','w', encoding='utf-8') as file:
                file.write(write_data) 


