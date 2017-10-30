# Track line position
# import packages and modules
import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches 

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

def get_nearest_peak(arr, threshold, old_pos):
    nearestPos = old_pos
    min_width = 5
    max_width = 40
    lhs = 0
    rhs = 0
    min_dist_from_old_pos = int(len(arr))/2 - 1
    peak_count = 0
    
    for i in range(int(len(arr))):
        if arr[i] >= threshold and i > rhs:
            lhs = i
            for i in range(lhs, (int(len(arr)))):
                if (arr[i] < threshold) or (i >= int(len(arr))-1):
                    rhs = i
                    peak_count = peak_count + 1
                    break
            if (rhs - lhs >= min_width) and (rhs - lhs <= max_width) and abs(lhs + (rhs - lhs)/2 - old_pos) < abs(min_dist_from_old_pos):
                nearestPos = lhs + (rhs - lhs)/2
                min_dist_from_old_pos = abs(lhs + (rhs - lhs)/2 - old_pos)
                
    print '%d peaks detected'%(peak_count)
    return nearestPos
        

# open the COM5 port at 921600 baud (Nucleo)
# open the COM4 port at 9600 (HC-05)
ser = serial.Serial('COM5', 9600)
count = 0
nearest_peak = 63.5

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
        if count >= 32:
            
            # Median filter
            # array128 = median_filter(array128)
            
            # Mean filter
            array128 = mean_filter(array128)
            array128 = mean_filter(array128)

            # Calculate threshold
            threshold = min(array128) + 0.5*(max(array128) - min(array128))
            
            # Update line position
            max_index = get_max_index(array128)
            
            # Detect nearest peak
            old_pos = nearest_peak
            nearest_peak = get_nearest_peak(array128, threshold, old_pos)
            
            # Plot
            plt.plot(array128, linewidth=2, ls='steps')
            plt.ylim(0, 1000)
            plt.xlim(0, 127)
            plt.grid(True)
            red_patch = mpatches.Patch(color='red', alpha=0.5, label='Nearest Peak')
            green_patch = mpatches.Patch(color='green', alpha=0.5, label='Max Pixel')
            plt.legend(handles=[red_patch, green_patch])
            
            # Plot threshold
            plt.axhline(y=threshold, linewidth=2, alpha=0.5, color='red')
           
            # Plot center line
            plt.axvline(x=63.5, linewidth=1, ls='dashed', alpha=0.5, color='grey')
            
            # Plot max index
            plt.axvline(x=max_index, linewidth=15, alpha=0.5, color='green')
            
            # Plot nearest peak
            plt.axvline(x=nearest_peak, linewidth=15, alpha=0.5, color='brown')
            
            
            plt.pause(0.010)  # pause in seconds
            count = 0
        
except:
    # close the serial port
    ser.close()
