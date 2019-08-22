import serial
import time

ser = serial.Serial()
ser.port = "COM5"
ser.baudrate = 57600

ser.open()
time.sleep(3)

#flush serial buffer
ser.reset_input_buffer()
ser.reset_output_buffer()

with open("tniasm.out", "rb") as f:
    byte = f.read(1)
    while byte != b'':
        print(byte.hex() + ' ')
        ser.write(byte)
        time.sleep(0.05)
        byte = f.read(1)

output = ""

while ser.in_waiting > 0:
    output = output + ser.read().hex() + " "

print("\n")
print(output)
ser.close()
