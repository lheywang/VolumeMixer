# ==============================================================================
#   host/main.py : Implement the main logic of sound controller
#
#   08/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

from audio import AudioController
from dev import MixerDevice

if __name__ == "__main__":
    # First, initialize the pure software part
    controller = AudioController()

    # Then, seek for the device and connect to it
    device = MixerDevice()
    print(device.SendCommand())

    # Todo : Add global algorithm !
