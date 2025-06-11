# ==============================================================================
#   host/dev/commands.py :  Implement the commands classes, used to store
#                           commands arguments
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# import modules
import zlib
import json

from dataclasses import dataclass, field


# Constants
START = "START"
END = "END"


# Defining functions
def FormatCommand(cmd, buf):
    # Format string
    if buf == None:
        tmp = f"{START};{cmd};000;;".encode("utf-8")
    else:
        tmp = f"{START};{cmd};{"{:03d}".format(len(buf))};{buf};".encode("utf-8")

    # Get the CRC32 of the string
    crc = zlib.crc32(tmp)

    # Format the return value
    tmp = f"{tmp.decode("utf-8")}{crc:04X};{END}"

    # Exit
    return tmp


# Defining classes
@dataclass
class CmdASYNC:
    apps: list[str] = field(default_factory=lambda: ["", "", "", "", ""])
    sync: list[bool] = field(default_factory=lambda: [True, True, True, True, True])
    res: bool = True

    def cmd(self):
        # Create the dict for JSON encoding
        tmp = dict()
        tmp["apps"] = dict()
        for k in range(5):
            tmp["apps"][f"{k}"] = self.apps[k]

        # Format the command
        return FormatCommand("ASYNC", json.dumps(tmp, separators=(",", ":")))

    def parse(self, buf):
        # Load the JSON dict
        tmp = json.loads(buf)

        # Clearing lists
        self.apps.clear()
        self.sync.clear()

        # Copy values
        try:
            self.res = tmp["match"]
            for index, elem in enumerate(tmp["apps"]):
                self.apps.append(elem[f"{index + 1}"])
                self.sync.append(bool(elem["match"]))

        except Exception as e:
            print(f"Invalid dict detected ! : {e}")


@dataclass
class CmdUICON:
    slider: int = 0
    app: str = "Master"
    icon: bytes = b"x" * 128
    store: bool = True

    def cmd(self):
        # Create the dict for JSON encoding
        tmp = dict()
        tmp["icon"] = dict()
        tmp["icon"]["slider"] = self.slider
        tmp["icon"]["app"] = self.app
        tmp["icon"]["icon"] = self.icon.decode("utf-8")
        tmp["icon"]["store"] = self.store

        # Format the command
        return FormatCommand("UICON", json.dumps(tmp, separators=(",", ":")))


@dataclass
class CmdSLPOS:
    pos: list[float] = field(default_factory=lambda: [0.5, 0.5, 0.5, 0.5, 0.5])
    mute: list[bool] = field(
        default_factory=lambda: [False, False, False, False, False]
    )

    def cmd(self):
        # Format the command
        return FormatCommand("SLPOS", None)

    def parse(self, buf):
        # Load the JSON dict
        tmp = json.loads(buf)

        # Clearing lists
        self.pos.clear()
        self.mute.clear()

        # Copy values
        try:
            for index, elem in enumerate(tmp["sliders"]):
                self.pos.append((int(elem[f"{index + 1}"]) / 100))
                self.mute.append(bool(elem[f"{index + 1}M"]))

        except Exception as e:
            print(f"Invalid dict detected ! : {e}")


@dataclass
class CmdDCONF:
    potOff: list[float] = field(default_factory=lambda: [0.0, 0.0, 0.0, 0.0, 0.0])
    potGain: list[float] = field(default_factory=lambda: [1.0, 1.0, 1.0, 1.0, 1.0])
    offset: float = 0.0
    gain: float = 1.0
    device: str = "B2A1FH1D1"

    def cmd(self):
        # Create the dict for JSON encoding
        tmp = dict()
        tmp["cal"] = [dict() for _ in range(5)]
        for k in range(5):
            tmp["cal"][k][f"{k + 1}OFF"] = "{:+0.3f}".format(self.potOff[k])
            tmp["cal"][k][f"{k + 1}G"] = "{:0.3f}".format(self.potGain[k])
        tmp["gain"] = "{:0.3f}".format(self.gain)
        tmp["offset"] = "{:+0.3f}".format(self.offset)
        tmp["device"] = self.device

        # Format the command
        return FormatCommand("DCONF", json.dumps(tmp, separators=(",", ":")))


@dataclass
class CmdSHUTD:
    def cmd(self):
        # Format the command
        return FormatCommand("SHUTD", None)


@dataclass
class CmdRESET:
    def cmd(self):
        # Format the command
        return FormatCommand("RESET", None)


@dataclass
class CmdCONNC:
    def cmd(self):
        # Format the command
        return FormatCommand("CONNC", None)
