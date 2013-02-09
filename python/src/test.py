import serial
import time

ser = serial.Serial('/dev/rfcomm0', 19200, timeout=10)

def readSync():
    buffer = ""
    while ser.read() != "[": pass
    while True:
        next = ser.read()
        if next == "]": break
        else: buffer += next
    return buffer
    
def command(msg):
    ser.write(msg)
    return readSync()

print "connected"

for i in range(0,4):
    print command("[MV44F44F]")
    time.sleep(8)
    print command("[MV44F00F]")
    time.sleep(0.9)
    
ser.close()
