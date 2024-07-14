from machine import ADC, Pin, I2C
from utime import sleep
import _thread
from picozero import LED
import time
from ssd1306 import SSD1306_I2C



# 2 threads max

PumpPWM = PWM(Pin(20))


BatteryPercentage = 100 #temp, later ill get a way to get the battery voltage, the battery has a BMS so its fine for 

WIDTH = 128 
HEIGHT= 32
i2c=I2C(0,scl=Pin(1),sda=Pin(0),freq=200000)
oled = SSD1306_I2C(WIDTH,HEIGHT,i2c)
isCharging = Pin(14, Pin.IN)
SelectMode = Pin(15, Pin.IN)
Turbo = False
lines = ["no","no","no","no","no","no"]#no

def WriteToLine(StrToWrite,lineNum):#makes my life super easy, instead of pixels, i just set a line
    #6 lines total
    yValue = (lineNum * 5) + 2 # top two pixels are reserved for battery bar
    lines[lineNum] = StrToWrite



def ScreenFunctions():
    while True:
        for i in lines:
            WriteToLine(lines[x],x)#look how simple that is

def PWMpercentage(percentvalue):#converts percents to actual value
    first = percentvalue/100
    return 65535*first

#percentvalue = 50
#first = 50/100 = 0.5
#return 65535*0.5 = 32,767.5

def Charging():
    TimeLeft = 252
    Seconds = 1
    WriteToLine("Charging...",1)
    while True:
        WriteToLine("Time Left till full: {} min".format(TimeLeft),2)
        WriteToLine("Charging Wattage: 140 Watts",3)#lmao imaging having a variability

        if (isCharging.value() == 1):#if i press the button, then get out of charging mode
            break
        else:
            if (Seconds == 60):
                TimeLeft -= 1
                Seconds = 0
            else:
                sleep(1)
                Seconds += 1
        
        

    

def operationMode():
    if(SelectMode.value() == 1):
        Turbo = True
    else:
        Turbo = False
        

def ClearScreen():
    oled.fill(0)



def main():#main func
    print("initializing Screen")
    


if __name__ == "__main__":
    main()