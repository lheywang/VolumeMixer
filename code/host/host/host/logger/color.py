# ==============================================================================
#   host/logger/color.py :          Implement some logging in console with
#                                   colors !
#
#   03/02/2026
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# ------------------------------------------------------------------------------
# Importing modules
# ------------------------------------------------------------------------------
import logging


class ColorFormatter(logging.Formatter):
    """Custom formatter to add colors to the console output only."""

    # ANSI escape sequences for colors
    grey = "\x1b[38;20m"
    blue = "\x1b[34;20m"
    yellow = "\x1b[33;20m"
    red = "\x1b[31;20m"
    bold_red = "\x1b[31;1m"
    reset = "\x1b[0m"

    FORMAT = (
        "[%(asctime)s] [%(levelname)8s] --- (%(filename)12s:%(lineno)3d) : %(message)s"
    )

    LEVEL_COLORS = {
        logging.DEBUG: grey,
        logging.INFO: blue,
        logging.WARNING: yellow,
        logging.ERROR: red,
        logging.CRITICAL: bold_red,
    }

    def format(self, record):
        log_fmt = self.LEVEL_COLORS.get(record.levelno) + self.FORMAT + self.reset
        formatter = logging.Formatter(log_fmt, datefmt="%Y-%m-%d %H:%M:%S")
        return formatter.format(record)
