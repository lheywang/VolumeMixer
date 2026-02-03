# ==============================================================================
#   host/main.py :          Implement the whole logic of the app.
#
#   03/02/2026
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# ------------------------------------------------------------------------------
# Importing modules
# ------------------------------------------------------------------------------
import logging

from dev import (
    IsDeviceAvailable,
    IsCommandValid,
    GetCommandStatus,
    CmdASYNC,
    CmdDCONF,
    CmdSLPOS,
    CmdUICON,
    CmdRINIT,
    CmdSHUTD,
    CmdCONNC,
    MixerDevice,
)
from audio import (
    AudioSource,
    AudioActiveApp,
    GetdB,
    GetVal,
    GetMatchingAudioSource,
    AudioController,
)

def init() -> None:
    pass

def loop() -> None:
    pass

def deinit() -> None:
    print("Performing a clean and exit ...")

if __name__ == "__main__":
    init()

    try:
        while True:
            loop()
    except KeyboardInterrupt:
        deinit()

    except SystemExit:
        deinit()

    except SystemError:
        deinit()

    except Exception as e:
        print(f"Unhandled exception : {e}")


