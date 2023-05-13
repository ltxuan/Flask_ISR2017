
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

if __name__ == "__main__":
    test()