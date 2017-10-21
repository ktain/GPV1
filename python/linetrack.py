# Track line

# import packages and modules
import serial
import numpy as np
import matplotlib.pyplot as plt

# preallocate array
array128 = np.zeros(129)
plt.title('Line tracking')
plt.hold(False)
    
# open the COM5 port at 921600 baud
ser = serial.Serial('COM5', 921600)
count = 0;

try:
    while(1):
        line = ser.readline()
        vec = line.split()
        if len(vec) == 2 and 0 < int(vec[0]) <= 128 and 10 < int(vec[1]) <= 4000:
            array128[int(vec[0])] = vec[1]
            count = count + 1;
        
        if count > 128:
            count = 0
            plt.plot(array128)
            plt.ylim(0, 4000)
            plt.xlim(1, 128)
            plt.pause(0.000001)  # pause in seconds
        
except:
    # close the serial port
    ser.close()
