# ==============================================================================
#   host/audio/utls.py : Implement some utility functions
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Importing modules
import math

# Defining constants
SCALE = 34


# Defining utils functions
def GetdB(value: float):
    if value >= 0.01:
        return SCALE * math.log(value)
    return -96.0


def GetVal(dB: float):
    if dB <= -96.0:
        return 0.00
    return math.pow(10, (dB / SCALE))
