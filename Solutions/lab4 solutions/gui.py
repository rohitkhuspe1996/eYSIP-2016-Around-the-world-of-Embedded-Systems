# LAB 4
#GUI to show the movement of Joystick using ADC and UART on TIVA Launchpad.

import serial
import numpy
import matplotlib.pyplot as plt
from drawnow import *

plt.ion()
Data=serial.Serial('com5',timeout=None,baudrate=9600) # read serial data from com5 with 9600 Baudrate


def makefig():
    plt.ylim(0,6)                       # Set the Y limit of axis in graph
    plt.xlim(0,6)                       # Set the X limit of axis in graph.
    plt.title('Joystick movement')      
    plt.grid(True)                      # Enable the Grid
    plt.plot(x,y,'ro')                  # Plot the received point with a "dot" marker and red color.
   
while True:
    Datastring = Data.readline()        # read the serial data.
    dataArray = Datastring.split(',')   # Split the data at comma.
    x = float( dataArray[0])            # Convert the X co-ordinates into float. 
    y = float( dataArray[1])            # Convert the Y co-ordinates into float.                
    drawnow(makefig)                    # use drawnow to continuously update the graph.

    
    
