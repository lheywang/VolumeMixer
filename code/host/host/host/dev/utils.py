# ==============================================================================
#   host/dev/utils.py : Implement some utils functions to build commands
#
#   11/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Import modules
from . import (
    CmdASYNC,
    CmdDCONF,
    CmdSLPOS,
    CmdUICON,
    CmdRESET,
    CmdSHUTD,
    CMDCONNC,
)


# Define functions
def IsDeviceAvailable(buf: bytes):
    return True
    return False


def FormatCommand(buf):
    # Todo :  Add START and END token
    # Todo :  Compute CRC32
    # Todo :  Return formatted string
    pass
