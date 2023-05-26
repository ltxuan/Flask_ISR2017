import re

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
            # with open('/etc/asterisk/sip.conf','w', encoding='utf-8') as file:
            #     file.write(write_data)

