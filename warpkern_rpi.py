from warpkern import WarpkernPhy

import wiringpi
from typing import List

import random
import math

from time import time

from threading import Thread

import numpy as np

from cffi import FFI


def writeData(data):
    wiringpi.wiringPiSPIDataRW(0, data)  # write the last chunk


def floatToByte(val: float) -> int:
    # Convert float to byte + temporal dithering
    return max(0, min(int(val * 255), 255))


class PiPhy(WarpkernPhy):

    def __init__(self):
        # wiringpi.wiringPiSetup()

        #wiringpi.wiringPiSPISetup(0, 4800000)

        self.thread = None

        self.ffi = FFI()
        self.ffi.cdef("""
int wiringPiSetup (void) ;
int wiringPiSPIGetFd     (int channel) ;
int wiringPiSPIDataRW    (int channel, unsigned char *data, int len) ;
int wiringPiSPISetupMode (int channel, int speed, int mode) ;
int wiringPiSPISetup     (int channel, int speed) ;
""")
        self._wiringpi = self.ffi.dlopen("/usr/lib/libwiringPi.so")

        self.chan = self.ffi.cast("int", 0)
        self.dlen = None
        self.dataptr = None

        self._wiringpi.wiringPiSetup()
        self._wiringpi.wiringPiSPISetup(
            self.chan, self.ffi.cast("int", 4500000))

    def pushData(self, data: np.array):
        if self.dlen is None:
            self.dlen = self.ffi.cast("int", len(data))
        if self.dataptr is None:
            self.dataptr = self.ffi.cast("unsigned char *", data.ctypes.data)

        self._wiringpi.wiringPiSPIDataRW(self.chan, self.dataptr, self.dlen)
