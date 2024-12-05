from machine import Pin, Timer, ADC
from time import sleep_us, ticks_us


display_timer = Timer()
digit_timer = Timer()
rpm_timer = Timer()
adc = ADC(0)

i = 0
num_str = '1392'
num = 1392;

def display(timer):
#     print('display start')
    rpm_timer.deinit()
    global i, num_str
    i = 0
    num_str = '0'*(4-len(str(rpm))) + str(rpm)
#     print(num_str)
    clear_digits()
    digit_timer.init(period=3, mode=Timer.ONE_SHOT, callback=display_dig)

def display_dig(timer):
    global i
#     print(type(num_str))
#     try:
        
    display_digit(num_str[i],i)
        
#     except:
#         print(type(num_str))
#         print(len(num_str))
#         print(i)
        
#     print(i)
    if i < 3:
        digit_timer.init(period=3, mode=Timer.ONE_SHOT, callback=display_dig)
        i += 1
    else:
        rpm_timer.init(freq=5e3,mode=Timer.PERIODIC,callback=RPM)
    
display_timer.init(period=20, mode=Timer.PERIODIC, callback=display)

# RPM measurement ISR
W = 500
# samps = [0]*W
filt = [0]*2
avg = [0]*W
t_last = ticks_us()

upper_thresh = 0.005*65535
lower_thresh = -0.005*65535
state = 0
j = 0
rpm = 0

a = 0.75
b = 1-a
def RPM(timer):
    global j, rpm, state, filt, avg, t_last
    
    data_in = adc.read_u16()
    now = ticks_us()
    
    if j < 1:
        avg.append(data_in)
        avg.pop(0)
    elif j < W+1:
        filt[1] = a*filt[0] + b*data_in
        avg.append((avg[W-1]*(j-1)+data_in)/j)
        avg.pop(0)
    else:
        filt[1] = a*filt[0] + b*data_in
        avg.append(avg[W-1]+(data_in-avg[0])/W)
        avg.pop(0)
    
    dc_shifted = filt[1] - avg[W-1]
    
    if (dc_shifted > upper_thresh and state == 0):
        state = 1
        rpm = int(60*1e6/(now - t_last)/2)
        t_last = now
    if (dc_shifted < lower_thresh and state == 1):
        state = 0
        
    filt[0] = filt[1]

#     print(f'{data_in}\t{filt[1]}\t{avg[W-1]}\t{rpm}')
#     if j % 126 == 0:
#         display_timer.init(period=0, mode=Timer.ONE_SHOT, callback=display)
    j += 1
    
rpm_timer.init(freq=6e3,mode=Timer.PERIODIC,callback=RPM)

# Define pins for segments 
segment_pins = {
    'A': Pin(2, Pin.OUT),#A
    'B': Pin(3, Pin.OUT),#B
    'C': Pin(4, Pin.OUT),#C
    'D': Pin(5, Pin.OUT),#D
    'E': Pin(6, Pin.OUT),#E
    'F': Pin(7, Pin.OUT),#F
    'G': Pin(8, Pin.OUT),#G
    'DP': Pin(9, Pin.OUT)#H
}

# Defining pins for digits
digit_pins = [
    Pin(10, Pin.OUT),  # D1
    Pin(11, Pin.OUT),  # D2
    Pin(12, Pin.OUT),  # D3
    Pin(13, Pin.OUT)   # D4
]

# Define segment states for numbers 0-9
digits = {
    '0': [1, 1, 1, 1, 1, 1, 0, 0],
    '1': [0, 1, 1, 0, 0, 0, 0, 0],
    '2': [1, 1, 0, 1, 1, 0, 1, 0],
    '3': [1, 1, 1, 1, 0, 0, 1, 0],
    '4': [0, 1, 1, 0, 0, 1, 1, 0],
    '5': [1, 0, 1, 1, 0, 1, 1, 0],
    '6': [1, 0, 1, 1, 1, 1, 1, 0],
    '7': [1, 1, 1, 0, 0, 0, 0, 0],
    '8': [1, 1, 1, 1, 1, 1, 1, 0],
    '9': [1, 1, 1, 1, 0, 1, 1, 0]
}

# clear digits at start
def clear_digits():
    for digit in digit_pins:
        digit.value(0)

# display a single digit at a specific position
def display_digit(number, position):
    
    for i,b in enumerate([val==position for val in range(4)]):
        digit_pins[i].value(1 if b is True else 0)
    

    segment_values = digits.get(str(number), [0, 0, 0, 0, 0, 0, 0, 0])
    for idx, seg in enumerate(['A', 'B', 'C', 'D', 'E', 'F', 'G', 'DP']):
        segment_pins[seg].value(segment_values[idx])

#display 4-digit number
def display_number(number):
    clear_digits()
    num_str = str(number)
    if len(num_str) < 4:
        num_str = '0' * (4 - len(num_str)) + num_str
    
    # Display each digit one at a time with multiplexing
    for position, digit in enumerate(num_str):
        display_digit(digit, position)


while True:
    pass
#     num = (num + 1) % 10000
#     num_str = '0'*(4-len(str(num))) + str(num)
    
    
    #
