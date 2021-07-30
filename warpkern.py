from typing import List, Type

from time import time
import random

import numpy as np

def floatToByte(val: float) -> int:
    # Convert float to byte + temporal dithering
    return max(0, min(int(val * 255), 255))


class WarpkernPhy():
    def pushData(self, data: List[List[float]]):
        pass

class Warpkern():

    def __init__(self, ringcount: int, ledcount: int, anims: List[Type], phy: WarpkernPhy, verbose: bool=False, debug: bool=False):
        self.ringcount = ringcount
        self.ledcount = ledcount
        self.animclasses = anims
        self.phy = phy
        self.verbose = verbose
        self.debug = debug

        self.anims = [x(self.ringcount, self.ledcount)
                      for x in self.animclasses]

        self.currentAnim = random.choice(self.anims)    # type: Anim

        self.time = time()

        self.nextAnimAt = self.time + 360

        self.totalLedCount = self.ringcount * self.ledcount
        self.totalByteCount = self.totalLedCount * 4 + 1 + self.totalLedCount // 8
        self.pixdata = np.ones((self.totalLedCount, 4))
        self.outdata = np.ones((self.totalLedCount + 1, 4))
        self.outdata[0][0] = 0
        self.outdata[0][1] = 0
        self.outdata[0][2] = 0
        self.outdata[0][3] = 0

        self.powdata = np.ones((self.totalLedCount, 4)) * 8.

        self.dithervec = (np.random.random_sample(
            (self.totalLedCount, 4)) * 0.95 - 0.5) / 255
        self.dithervec[:, 0] = np.zeros(self.totalLedCount)

    def dither(self):
        self.dithervec[:, 1:4] = (np.random.random_sample(
            (self.totalLedCount, 3)) * 0.95 - 0.5) / 255

    def tick(self):
        self.dt = time() - self.time
        self.time = time()

        if self.time > self.nextAnimAt:
            self.currentAnim = random.choice(self.anims)

        self.dither()

        self.outdata[1:self.totalLedCount +
                     1] = np.power(self.pixdata, self.powdata) + self.dithervec

        self.phy.pushData(np.array((self.outdata).clip(0, 1)
                                   * 255, np.uint8).flatten())
