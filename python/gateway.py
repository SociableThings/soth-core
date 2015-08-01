#!/usr/bin/env python

import soth_pb2
import serial
import time

from scipy import arange, sin
from scipy import pi as mpi
from matplotlib import pylab as pl

ser = serial.Serial('/dev/cu.RNBT-75B9-RNI-SPP', 115200, timeout=10)
time.sleep(1)

MIN_POSITION = 150
MAX_POSITION = 900
CENTER_POSITION = 700

def make_sine(Freq, Amp, fs, sec = 2.):
  ret = Amp * sin(2. * mpi * Freq * arange(int(fs * sec)) / fs)
  return ret

def sendData(fr, fl, br, bl, y):
  for i in range(0, 3):
    servo_position = soth_pb2.ServoPosition()

    servo_position.servo_front_right = fr
    servo_position.servo_front_left = fl
    servo_position.servo_back_right = br
    servo_position.servo_back_left = bl
    servo_position.servo_yaw = y

    ser.write(servo_position.SerializeToString())
    ser.write("\0")

    #msg = servo_position.SerializeToString()
    #sp = servo_position.ParseFromString(msg)

  #ser.close()

F_default = 1
A_default = 200
WAIT_TIME = 50
fs_default = 200

def test():
  sine_wave = make_sine(F_default, A_default, fs_default)

  while True:
    for i in range(0, len(sine_wave)):
      d = int(sine_wave[i]) + CENTER_POSITION
      print d
      #sendData(d, d, d, d, -30)
      ser.write(str(d) + "\0")
      time.sleep(0.05)

if __name__ == "__main__":
  test()
