#!/usr/bin/env python3
import serial
import time
import random

# List of function names
functions = ["rainbowFade", "colorWipe", "pulseWhite"]

ser = serial.Serial('/dev/ttyACM0',115200, timeout=1.0)
time.sleep(3)
ser.reset_input_buffer()
print("Serial OK")



try:
        while True:
            # Randomly select a function
            selected_function = random.choice(functions)

            # Generate three random values between 0 and 255
            var1 = random.randint(0, 255)
            var2 = random.randint(0, 255)
            var3 = random.randint(0, 255)
    
            time.sleep(1) #in seconds
            print("Send request to Arduino")
            ser.write(str(selected_function + " " + str(var1) + " " + str(var2) + " " + str(var3) + "\n").encode('utf-8'))

            # waiting to hear arduino is done, but really this does not matter IRL
            while ser.in_waiting <= 0:
                time.sleep(0.01)
            response = ser.readline().decode('utf-8').rstrip()
            print(response)
            
except KeyboardInterrupt:
    print("Close Serial Communication.")
    ser.close()


