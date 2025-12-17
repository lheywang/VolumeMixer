# ==============================================================================
#   host/dev/utils.py : Implement some utils functions to build commands
#
#   11/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================
# ------------------------------------------------------------------------------
# Imports
# ------------------------------------------------------------------------------
import zlib

# ------------------------------------------------------------------------------
# Constants
# ------------------------------------------------------------------------------
TOKEN = "VolumeMixer-Waiting_for_connection..."


# ------------------------------------------------------------------------------
# Define functions
# ------------------------------------------------------------------------------
def IsDeviceAvailable(buf: str):
    if TOKEN in buf:
        return True
    return False


def IsCommandValid(buf: str):
    # First, get the crc32 of the string to ensure it's valid
    tmp = buf[:-12:]

    # Extract the CRCs
    try:
        ccrc = zlib.crc32(tmp.encode())
        rcrc = int(buf[-12:-4], 16)

        # Check them
        if rcrc == ccrc:
            return True
        return False
    except:
        return False


def GetCommandStatus(buf: str):
    # Split the buffer in multiple parts
    tmp = buf.split(";")[3]

    if tmp == "ACK":
        return True
    return False
