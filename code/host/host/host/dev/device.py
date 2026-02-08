# ==============================================================================
#   host/dev/device.py :    Implement the device class, used to send and read
#                           commands.
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# ------------------------------------------------------------------------------
# Importing modules
# ------------------------------------------------------------------------------
import serial
import serial.serialutil
import serial.tools.list_ports
import time
import logging

# ------------------------------------------------------------------------------
# Local imports
# ------------------------------------------------------------------------------
from . import IsDeviceAvailable, IsCommandValid, GetCommandStatus
from . import CmdASYNC, CmdDCONF, CmdSLPOS, CmdUICON, CmdCONNC, CmdRINIT, CmdSHUTD

# ------------------------------------------------------------------------------
# Defining some constants
# ------------------------------------------------------------------------------
BAUD = 921600
BYTE = serial.EIGHTBITS
PARITY = serial.PARITY_NONE
STOP = serial.STOPBITS_ONE


# ------------------------------------------------------------------------------
# Defining the base class
# ------------------------------------------------------------------------------


class MixerDevice:

    def __init__(self, logger: logging.Logger) -> None:
        self.logger = logger

        # First, choose the COM port and set the launch guard to prevent any IO on unconfigured com port.
        # Todo : Uncomment this for real working !
        if self._SelectCOMPort() != 0:
            self.logger.error("No device available. Exiting...")
            self.IsDeviceOpenned = False
            return
        self.logger.info(f"Found a VolumeMixer device on port : {self.port.port}")
        self.IsDeviceOpenned = True

        # Then, we've got a device, so we try to connect to it
        cmd = CmdCONNC()
        self.IsDeviceConnected = self.SendCommand(cmd)

        if self.IsDeviceConnected:
            self.logger.info("Connected to device !")
        else:
            self.logger.error("Connected refused to device")

    def _SelectCOMPort(self):
        """
            Attempts to automatically select and open a serial COM port connected to a compatible mixer device.
            This method scans all available serial ports, tries to open each one, and reads initial data to determine
            if the connected device is a mixer ready to accept a connection. If a suitable device is found, the serial
            port is assigned to `self.port` and the method returns 0. If no compatible device is found, returns -1.

        Returns:
            int: 0 if a compatible device is found and the port is opened, -1 otherwise.

        Raises:
            Exception: If an unhandled error occurs while opening a port.
        """
        # First, list all of the available serial ports
        ports = list(serial.tools.list_ports.comports())

        # Then, try to open the ports and see if something is fetched ?:?

        for port in ports:
            try:
                self.logger.info(f"Trying port {port.name} ...")
                tmp = serial.Serial(
                    port.name,
                    baudrate=BAUD,
                    bytesize=BYTE,
                    parity=PARITY,
                    stopbits=STOP,
                    timeout=1.0,
                )
            # Unable to open com port
            except serial.serialutil.SerialException:
                continue
            except Exception as e:
                raise Exception(f"Unhandled error : {e}")

            # Then, if the port was openned, wait for a read operation : (message every 500 ms).
            time.sleep(0.6)

            try:
                data = tmp.read(tmp.in_waiting).decode()

                # Check if the message is typical from a mixer ready to accept a connection
                if IsDeviceAvailable(data):
                    self.port = tmp
                    self.logger.info(f"Found a device on : {self.port.port} !")
                    return 0
                else:
                    continue
            except UnicodeDecodeError as e:
                self.logger.info(f"Could not decode COM port {tmp.port} : {e}")
                continue

        return -1

    def SendCommand(
        self,
        cmd: CmdASYNC | CmdDCONF | CmdSLPOS | CmdUICON | CmdCONNC | CmdRINIT | CmdSHUTD,
    ):
        # First, get the command :
        buf = cmd.cmd()
        self.logger.debug(f"Sending buffer : {buf}")

        # Then, write the command on the serial port
        if self.IsDeviceOpenned:
            self.port.write(buf.encode())

            # Immediately return, True, because theses commands will
            # trigger the target in mode where it CAN'T respond...
            if type(cmd) == CmdRINIT or type(cmd) == CmdSHUTD:
                return cmd, True

            # Wait for an END tocken to be found to be seen.
            # The input buffer is flushed, to ensure no error will be fetched.
            self.port.reset_input_buffer()
            tmp = self.port.read_until(";END").decode().strip().strip("\0")
            self.logger.debug(f"Rode from the device : {tmp}")

            # Parse the output
            valid = IsCommandValid(tmp)
            if not valid:
                self.logger.warning(
                    f"Invalid command was received (False CRC). Ignoring it..."
                )
                return cmd, False

            # Check the return value of the command
            if type(cmd) == CmdASYNC or type(cmd) == CmdSLPOS:
                cmd.parse(tmp.split(";")[3])
                return cmd, True
            else:
                return cmd, GetCommandStatus(tmp)

        return cmd, False

    def ComputeCalibrationCoefficients(self, min_vals, max_vals) -> CmdDCONF:
        # Constants
        SAFE_GAIN = 25.0
        TARGET_OUTPUT = 100.0
        SAFETY_MARGIN = 0.02  # 2% of margin.
        GAIN_MARGIN = 1.02  # 2% of more gain (ensure we reach 100%)

        # --- PHASE 1: NORMALIZE MEASUREMENTS ---
        norm_mins = [m / SAFE_GAIN for m in min_vals]
        norm_maxs = [m / SAFE_GAIN for m in max_vals]

        # --- PHASE 2: CALCULATE TOTAL REQUIREMENTS ---
        req_total_gains = []
        req_total_offsets = []

        for i in range(5):
            n_range = norm_maxs[i] - norm_mins[i]

            t_gain = TARGET_OUTPUT / n_range
            req_total_gains.append(t_gain * GAIN_MARGIN)

            t_offset = norm_mins[i] + SAFETY_MARGIN
            req_total_offsets.append(t_offset)

        # --- PHASE 3: DISTRIBUTE GLOBAL VS LOCAL ---
        avg_gain = sum(req_total_gains) / len(req_total_gains)
        global_gain_int = round(avg_gain * GAIN_MARGIN, 1)

        avg_offset = min(req_total_offsets)
        global_offset_float = avg_offset

        # --- PHASE 4: CALCULATE FINE TUNING & BUILD COMMAND ---
        cmd = CmdDCONF()

        cmd.gain = round(global_gain_int, 1)
        cmd.offset = round(global_offset_float, 3)

        cmd.potGain = [
            round(req_total_gain / global_gain_int, 3)
            for req_total_gain in req_total_gains
        ]
        cmd.potOff = [
            round(req_total_offset - global_offset_float, 3)
            for req_total_offset in req_total_offsets
        ]

        # --- PHASE 5: SHOW THE USER NEW COEFFICIENTS ---
        self.logger.info(
            f"""
New coefficients are : 
    - ADC GAIN :        {cmd.gain}
    - ADC OFFSET :      {cmd.offset}
    - POTS GAIN :       {cmd.potGain}
    - POTS OFFSET :     {cmd.potOff}
"""
        )

        return cmd
