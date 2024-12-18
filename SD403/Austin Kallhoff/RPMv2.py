import time
from machine import Pin
from rp2 import PIO, StateMachine, asm_pio, DMA

@asm_pio(out_init=(PIO.OUT_LOW,)*8,sideset_init=(PIO.OUT_LOW,)*4,out_shiftdir=PIO.SHIFT_RIGHT,autopull=True,pull_thresh=32)
def display_4x7():
    wrap_target()
#     irq(rel(0))					#[0]
    out(pins, 8)			.side(1<<0)	#[0]
    out(pins, 8)			.side(1<<1)	#[0]
    out(pins, 8)			.side(1<<2)	#[0]
    out(pins, 8)			.side(1<<3)	#[0]
    wrap()  

segment_pins = Pin(2, Pin.OUT)
digit_pins = Pin(10, Pin.OUT)

sm = StateMachine(0,
                  display_4x7,
                  freq=2000,
                  out_base=segment_pins,
                  sideset_base=digit_pins)

# dma = DMA()

# DREQ_PIO0_TX0 = 0

# c = dma.pack_ctrl(inc_write=False, inc_read=False, treq_sel=0)

# src_data = (0b11111100 << 24) | (0b11111000 << 16) | (0b11110000 << 8) | (0b11110000)
# src_data = bytearray(4)

# dma.config(
#     ctrl=c,
#     read=src_data,
#     write=sm,
#     trigger=True)


sm.irq(lambda p: print(time.ticks_ms()))

# segment_pins.value(1)
# digit_pins.value(1)
# time.sleep(3)
# sm.put(bytearray([255,255,255,255]))
sm.active(1)
# sm.put(bytearray([0b11111100, 0b11111000, 0b11110000, 0]))
# time.sleep(3)
start_time = time.ticks_ms()
a = 0
b = 60
c = 120
while(time.ticks_ms() - start_time < 3000):
    sm.put((255 << 24) | (255 << 16) | (255 << 8) | (255))
#     pass
#     a = (a+1)%255
#     b = (b+1)%255
#     c = (c+1)%255
#     sm.put((a << 16) | (b << 8) | (c) )
    
sm.active(0)
    
    

