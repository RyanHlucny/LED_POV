import time
from machine import Pin, Timer, ADC
from rp2 import PIO, StateMachine, asm_pio, DMA
from time import ticks_us, ticks_ms

@asm_pio(out_init=PIO.OUT_LOW,sideset_init=(PIO.OUT_LOW,PIO.OUT_HIGH),out_shiftdir=PIO.SHIFT_LEFT,autopull=True,pull_thresh=32)
def display_4x7():
    wrap_target()
    out(pins,1)			.side((1 << 1) | 0)
    nop()				.side((1 << 1) | 1)
    wrap()  

ser_pin = Pin(0, Pin.OUT)
clock_pin = Pin(1, Pin.OUT)

sm = StateMachine(0,
                  display_4x7,
                  freq=4_000_000,
                  out_base=ser_pin,
                  sideset_base=clock_pin)

rpm_sample_timer = Timer()
adc = ADC(0)

# RPM measurement ISR
W = 500
num_blades = 7
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
def RPM_sample(timer):
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
        rpm = int(60*1e6/(now - t_last)/num_blades)
        t_last = now
    if (dc_shifted < lower_thresh and state == 1):
        state = 0
        
    filt[0] = filt[1]
    j += 1
    
rpm_sample_timer.init(freq=10e3,mode=Timer.PERIODIC,callback=RPM_sample)

#		  B A F G H C D E
digits = {
    '0': [1,1,1,0,0,1,1,1],
    '1': [1,0,0,0,0,1,0,0],
    '2': [1,1,0,1,0,0,1,1],
    '3': [1,1,0,1,0,1,1,0],
    '4': [1,0,1,1,0,1,0,0],
    '5': [0,1,1,1,0,1,1,0],
    '6': [0,1,1,1,0,1,1,1],
    '7': [1,1,0,0,0,1,0,0],
    '8': [1,1,1,1,0,1,1,1],
    '9': [1,1,1,1,0,1,1,0]
    }

def convert_num_to_word(num):
    # digits = '0123'
    if len(num) is not 4:
        raise ValueError("Invalid Input: Input must be exactly 4 digits")
    
    lst = []
    
    for i,c in enumerate(num):
        d = digits[c]
        if i == 0 or i == 2:
            temp1 = d[0:4]
            temp2 = d[4:8]
            temp1.reverse()
            temp2.reverse()
            d = temp1+temp2
        lst = d+lst
        
    d0 = int(''.join(str(i) for i in lst[0:8]), 2)
    d1 = int(''.join(str(i) for i in lst[8:16]), 2)
    d2 = int(''.join(str(i) for i in lst[16:24]), 2)
    d3 = int(''.join(str(i) for i in lst[24:32]), 2)

    dig0 = (d0>>4) | (d0 & 0x0F) << 16
    dig1 = (d1>>4) << 4 | (d1 & 0x0F) << 20
    dig2 = (d2>>4) << 8 | (d2 & 0x0F) << 24
    dig3 = (d3>>4) << 12 | (d3 & 0x0F) << 28
        
    return ~(dig0 | dig1 | dig2 | dig3)
    
sm.active(1)
_time = ticks_ms()
while True:
    if ticks_ms() - _time > 125:
        num_str = str(rpm)
        num_str = '0'*(4-len(num_str)) + num_str
        data = convert_num_to_word(num_str)
        print(f'Updating RPM as {num_str} at {ticks_ms()}')
        sm.put(data)
        _time = ticks_ms()
    
    



