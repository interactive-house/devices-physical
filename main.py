import serial
import firebase_admin
import time
import datetime
import ast
from firebase_admin import db
from firebase_admin import credentials

url = "https://soft-d20bd-default-rtdb.firebaseio.com/";
cred = credentials.Certificate('soft-d20bd-firebase-adminsdk-ho640-8bafc4b748.json')
default_app = firebase_admin.initialize_app(cred, {'databaseURL': url})
ref = db.reference('/heads')
print("Hello")
arduino = serial.Serial('COM5', 9600, timeout=.1)
print("Hello")

def toFirebase(soilInfo):
    print(soilInfo)
    timeNow = datetime.datetime.now().time()
    ref.child('0').push({ "soil":soilInfo})
    ref.child('0').update({'soils':{"soil":soilInfo}})

while True:
    data = arduino.readline()[:-2].decode("utf-8")
    if data!="":
        state = str(ast.literal_eval(data)['soil'])
        toFirebase()

