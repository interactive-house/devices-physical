import serial
import firebase_admin
from firebase_admin import db
from firebase_admin import credentials

url = "https://smarthome-3bb7b-default-rtdb.firebaseio.com/"

# Fetch the service account key JSON file contents
cred = credentials.Certificate(
    "smarthome-3bb7b-firebase-adminsdk-bv9hc-17639baa20.json"
)

# Initialize the app with a service account, granting admin privileges
default_app = firebase_admin.initialize_app(cred, {"databaseURL": url})

# As an admin, the app has access to read and write all data, regradless of Security Rules
ref = db.reference("")

windows = ref.child("SmartHomeValueWindow").child("StatusOfWindow")
door = ref.child("SmartHomeValueDoor").child("StatusOfDoor")
lamp = ref.child("SmartHomeValueLight").child("StatusOflight")
soil = ref.child("SmartHomeValueSoil").child("StatusOfSoil")
arduino = serial.Serial("COM3", 9600, timeout=0.1)

windowStatus = "closed"
doorStatus = "closed"
lampStatus = "off"
while True:

    windowFromFireBase = windows.get()
    doorFromFireBase = door.get()
    lampFromFireBase = lamp.get()
    if windowStatus != windowFromFireBase:
        arduino.write(b"w")
        windowStatus = windowFromFireBase

    if doorStatus != doorFromFireBase:
        arduino.write(b"d")
        doorStatus = doorFromFireBase

    if lampStatus != lampFromFireBase:
        arduino.write(b"l")
        lampStatus = lampFromFireBase

    dataFromArduino = arduino.readline().decode().strip()
    if "Soil" in dataFromArduino:
        split = dataFromArduino.split(": ")
        soil.set(split[1])
