# ==============================================================================
#   host/dev/commands.py :  Implement the commands classes, used to store
#                           commands arguments
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Importing modules
from dataclasses import dataclass

# Local import
from . import FormatCommand

# Constants
START = "START"
END = "END"


# Defining classes
@dataclass
class CmdASYNC:
    res: bool
    apps: dict

    def cmd(self):
        return f"{START};ASYNC;"


@dataclass
class CmdUICON:
    slider: int
    app: str
    icon: bytes
    store: bool


@dataclass
class CmdSLPOS:
    pos: list
    mute: list


@dataclass
class CmdDCONF:
    potOff: list
    potGain: list
    offset: float
    gain: float
    device: str


@dataclass
class CmdSHUTD:
    def cmd(self):
        return FormatCommand(f"SHUTD;000;;")


@dataclass
class CmdRESET:
    def cmd(self):
        return f"{START};ASYNC;"


@dataclass
class CMDCONNC:
    def cmd(self):
        return f"{START};ASYNC;"
