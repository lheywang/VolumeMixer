# ==============================================================================
#   host/dev/device.py :    Implement the device class, used to send and read
#                           commands.
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Importing modules
import zlib
import serial
import serial.serialutil
import serial.tools.list_ports

# Local imports
from . import IsDeviceAvailable
from . import CmdASYNC, CmdDCONF, CmdSLPOS, CmdUICON, CMDCONNC, CmdRESET, CmdSHUTD

# Defining some constants
BAUD = 921600
BYTE = serial.EIGHTBITS
PARITY = serial.PARITY_NONE
STOP = serial.STOPBITS_ONE


# Defining the base class
class MixerDevice:
    def __init__(self) -> None:
        # First, choose the COM port and set the launch guard to prevent any IO on unconfigured com port.rr
        if self._SelectCOMPort() != 0:
            print("No device available for now. Exiting...")
            self.IsDeviceOpenned = False
            return
        self.IsDeviceOpenned = True

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
                tmp = serial.Serial(
                    port.name,
                    baudrate=BAUD,
                    bytesize=BYTE,
                    parity=PARITY,
                    stopbits=STOP,
                    timeout=0.5,
                )
            # Unable to open com port
            except serial.serialutil.SerialException:
                continue
            except Exception as e:
                raise Exception(f"Unhandled error : {e}")

            # Then, if the port was openned, wait for a read operation :
            data = tmp.read()

            # Check if the message is typical from a mixer ready to accept a connection
            if IsDeviceAvailable(data):
                self.port = tmp
                return 0
            else:
                continue

        self.port = None
        return -1

    def SendCommand(self, cmd):
        pass
