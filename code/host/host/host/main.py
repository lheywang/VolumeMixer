# ==============================================================================
#   host/main.py : Implement the main logic of sound controller
#
#   08/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

import logging
from logging.handlers import TimedRotatingFileHandler

from audio import AudioController
from dev import MixerDevice


if __name__ == "__main__":
    # Open, and configure a logger for the whole project
    logger = logging.getLogger("VolumeMixer")
    logger.setLevel(logging.DEBUG)
    logger.handlers.clear()

    handler = TimedRotatingFileHandler(
        filename=f"VolumeMixer.log",
        when="D",
        interval=1,
        backupCount=3,
        encoding="utf-8",
        delay=False,
    )
    formatter = logging.Formatter(
        "[%(asctime)s] [%(levelname)8s] --- (%(filename)12s:%(lineno)03s) : %(message)s ",
        "%Y-%m-%d %H:%M:%S",
    )
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    # First, initialize the pure software part
    controller = AudioController(logger)

    # Then, seek for the device and connect to it
    device = MixerDevice(logger)

    # Todo : Add global algorithm !
