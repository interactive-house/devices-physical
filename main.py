import serial
import firebase_admin
from firebase_admin import db
from firebase_admin import credentials

url = "https://soft-d20bd-default-rtdb.firebaseio.com/";

# Fetch the service account key JSON file contents
cred = credentials.Certificate('soft-d20bd-firebase-adminsdk-ho640-c6075710e1.json')

# Initialize the app with a service account, granting admin privileges
default_app = firebase_admin.initialize_app(cred, {'databaseURL': url})

# As an admin, the app has access to read and write all data, regradless of Security Rules
ref = db.reference("Data")

windows = ref.child("window")
door = ref.child("door")
lamp = ref.child("light")
soil = ref.child("soil")

arduino = serial.Serial('COM3', 9600, timeout=.1)

isWindowOpen = False
isDoorOpen = False
isLampOn = False
while True:
    
    windowFromFireBase = windows.get()
    doorFromFireBase = door.get()
    lampFromFireBase = lamp.get()

    if(isWindowOpen != windowFromFireBase):
        arduino.write(b'w')
        isWindowOpen= not isWindowOpen
    

    if(isDoorOpen != doorFromFireBase):
        arduino.write(b'd')
        isDoorOpen= not isDoorOpen
    

    if(isLampOn != lampFromFireBase):
        arduino.write(b'l')
        isLampOn= not isLampOn

    dataFromArduino = arduino.readline().decode().strip()
    if('Soil' in dataFromArduino):
        split = dataFromArduino.split(': ')
        soil.set(split[1])

    