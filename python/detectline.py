'''
# Draw a thick red hline at y=1 that spans the xrange
plt.axhline(y=threshold, linewidth=2, color='#d62728')

# Draw a default vline at x=1 that spans the yrange
plt.axvline(x=1)

plt.axis([0, 2, 0, 3])
'''

# Track line position
# import packages and modules
import serial
import numpy as np
import matplotlib.pyplot as plt

def mean_filter(arr):
    new_arr = np.zeros(len(arr))
    for i in range(1,len(arr)-1):
        new_arr[i] = (arr[i-1] + arr[i] + arr[i+1])/3.0
    new_arr[0] = new_arr[1]
    new_arr[-1] = new_arr[-2]
    return new_arr
    
def median_filter(arr):
    new_arr = np.zeros(len(arr))
    for i in range(1,len(arr)-1):
        new_arr[i] = get_median(arr[i-1:i+2])
    new_arr[0] = new_arr[1]
    new_arr[-1] = new_arr[-2]
    return new_arr
    
def get_median(arr):
    tmp_arr = arr;
    # bubble sort ascending
    for i in range(len(tmp_arr)):
        for j in range(len(tmp_arr)-1):
            if tmp_arr[j+1] < tmp_arr[j]:
                tmp = tmp_arr[j]
                tmp_arr[j] = tmp_arr[j+1]
                tmp_arr[j] = tmp
    return tmp_arr[int(len(tmp_arr)/2)]

def get_max_index(arr):
    # default to center
    max_index = 0
    max_val = 0
    
    for i in range(len(arr)):
        if arr[i] > max_val:
            max_index = i
            max_val = arr[i]
    return max_index

# open the COM5 port at 921600 baud
ser = serial.Serial('COM5', 921600)
count = 0;
line_pos = 63.5

# preallocate array
array128 = np.zeros(128)
plt.hold(False)

try:
    while(1):
        # Read values
        line = ser.readline()
        vec = line.split()
        if len(vec) == 2 and 0 <= int(vec[0]) <= 127 and 10 <= int(vec[1]) <= 4000:
            array128[int(vec[0])] = vec[1]
            count = count + 1;
            
        # Refresh plot
        if count >= 128:
            
            # Calculate threshold
            threshold = min(array128) + (max(array128) - min(array128))/2.0
            
            # Median filter
            array128 = median_filter(array128)
            
            # Mean filter
            array128 = mean_filter(array128)
            array128 = mean_filter(array128)
                
            # Update line position
            max_index = get_max_index(array128)
            if max_index > line_pos:
                line_pos = line_pos + 1
            elif max_index < line_pos:
                line_pos = line_pos - 1

            # Plot
            plt.plot(array128, linewidth=2)
            plt.ylim(0, 500)
            plt.xlim(0, 127)
            
            # Plot threshold
            plt.axhline(y=threshold, linewidth=2, alpha=0.5, color='red')
           
            # Plot center line
            plt.axvline(x=63.5, linewidth=1, ls='dashed', alpha=0.2, color='grey')
            
            # Plot max index
            plt.axvline(x=max_index, linewidth=2, alpha=0.5, color='purple')
            
            # Plot line position
            plt.axvline(x=line_pos, linewidth=10, alpha=0.2, color='green')
            
            plt.pause(0.040)  # pause in seconds
            count = 0
        
except:
    # close the serial port
    ser.close()
