import serial
ser = serial.Serial('COM5')
ser.write(b'w')
ser.close()    