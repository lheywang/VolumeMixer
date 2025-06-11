# ==============================================================================
#   host/audio/utls.py : Implement some utility functions
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Importing modules
import math

# Locals import
from . import AudioActiveApp, AudioSource

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


def GetMatchingAudioSource(executables: list[AudioSource], apps: dict, pos: int):
    ret = []
    print(executables)

    # First, identify the app
    for index, exe in enumerate(executables):
        for app in apps.keys():
            if app in exe.name:
                # Remove the key from the executables (prevent from doublons on different channels!)
                tmp = []
                for index2, exe2 in enumerate(executables):
                    if exe2.name == exe.name:
                        tmp.append(exe2)
                        if executables:
                            executables.pop(index2)
                tmp.append(exe)
                if executables:
                    executables.pop(index)

                # Build the temp storage values
                ret.append(tmp)
                ret.append(app)
                ret.append(apps[app])

                # Exit loop
                break

    # Check for the "any" tag
    if not ret:
        if "any" in apps.keys():
            if executables:
                ret.append(
                    executables
                )  # Get all executables names --> All affected by the volume
                ret.append("Games")
                ret.append("controller.bin")
            else:
                ret.append(
                    []
                )  # Get all executables names --> All affected by the volume
                ret.append("")
                ret.append("blank.bin")

    # Read the bytes
    with open(f"icons/{ret[2]}", "rb") as f:
        data = f.read()

    # Then build the return class
    return AudioActiveApp(ret[0], ret[1], pos, data)
