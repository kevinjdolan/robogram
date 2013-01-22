import serial


ser = serial.Serial('/dev/rfcomm0', 9600, timeout=10)

def readSync():
    buffer = ""
    while ser.read() != "[": pass
    while True:
        next = ser.read()
        if next == "]": break
        else: buffer += next
    return buffer
    
def writeVerify(msg):
    for char in msg:
        ser.write(char)
        while ser.read() != char: pass

def command(msg):
    writeVerify(msg)
    return readSync()

print "connected"

import time

start = time.time();
print command("[GS]")
print time.time() - start

start = time.time();
print command("[GS]")
print time.time() - start

start = time.time();
print command("[GS]")
print time.time() - start

start = time.time();
print command("[GS]")
print time.time() - start

#while command("[GS]")[0] != 'F': pass
#print command("[MV00F00F]")
ser.close()
