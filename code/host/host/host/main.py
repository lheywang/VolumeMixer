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
import sys

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

from logger import setup_logger

def init() -> None:
    # Creating the logger device
    logger = setup_logger()
    logger.info("Created the logger, will now defer them.")

    # Openning the devices
    apps = AudioController(logger)
    device = MixerDevice(logger)

    return logger, device, apps

def loop(logger : logging.Logger, device: MixerDevice, apps: AudioController) -> None:
    pass

def deinit(logger : logging.Logger, reason : str) -> None:
    logger.info("Performing a clean and exit ...")
    logger.info(f"    Reason is : {reason}")

if __name__ == "__main__":

    # Initializing the device
    logger, dev, apps = init()

    # Running the loop, until somehow something exit
    try:
        while True:
            loop(logger, dev, apps)

    except KeyboardInterrupt:
        deinit(logger, "KeyboardInterrupt")
        sys.exit(0)

    except SystemExit:
        deinit(logger, "SystemExit")
        sys.exit(0)

    except SystemError:
        deinit(logger, "SystemError")
        sys.exit(0)

    except Exception as e:
        logger.error(f"Unhandled exception : {e}")
        sys.exit(-1)


