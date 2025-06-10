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


# Defining classes
@dataclass
class CmdASYNC:
    res: bool
    apps: dict


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
