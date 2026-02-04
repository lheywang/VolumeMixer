# ==============================================================================
#   host/logger/logger.py :          Implement some logger higher level
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
from logging.handlers import RotatingFileHandler

from . import ColorFormatter


def setup_logger(LogLevel: bool, log_file="VolumeMixer.log"):
    logger = logging.getLogger("VolumeMixer")

    level = logging.DEBUG if LogLevel else logging.INFO
    logger.setLevel(level)

    # Standard format for the file (No colors here!)
    file_format = logging.Formatter(
        "[%(asctime)s] [%(levelname)8s] --- (%(filename)12s:%(lineno)3d) : %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )

    # 1. Rotating File Handler
    # maxBytes: 5MB per file | backupCount: Keep 3 old log files before overwriting
    file_handler = RotatingFileHandler(log_file, maxBytes=524288, backupCount=3)
    file_handler.setFormatter(file_format)
    logger.addHandler(file_handler)

    # 2. Color Console Handler
    console_handler = logging.StreamHandler(sys.stdout)
    console_handler.setFormatter(ColorFormatter())
    logger.addHandler(console_handler)

    return logger
