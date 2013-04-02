import serial
import time

ser = serial.Serial('/dev/rfcomm0', 115200, timeout=10)

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

sum = 0
for i in range(0, 100):
    start = time.time()
    print command("[GS]")
    duration = time.time() - start
    sum += duration
    
print "AVERAGE:"
print sum / 100
    
ser.close()
