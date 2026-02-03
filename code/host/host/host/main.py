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
import time
import argparse

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


class Application:
    """
    Define a new class, that contain the base application logic.
    Enable the internalization of some variables, to retains states from
    one execution to the next.
    """

    def __init__(self, LogLevel: bool = False) -> None:
        """
        Open a new instance of the app.

        Will trigger the openning of the COM port, and all the subsequent
        device initialization and software fetching.
        """
        # Creating the logger device
        self.logger = setup_logger(LogLevel)
        self.logger.info("Created the logger, will now defer them.")

        # Openning the devices
        self.apps = AudioController(self.logger)
        self.device = MixerDevice(self.logger)

        # If device isn't openned, exit without returning.
        # Logs where already defered within the class init.
        if self.device.IsDeviceOpenned == False:
            sys.exit(-2)

        return

    def loop(self) -> None:
        """
        Contain the base application logic, executed repetely.

        Each call will try to :

        - Fetch the slider position
            -> Update theses into the OS agnostic module

        - Fetch the applications displayed
            -> Compare them to the actual apps on the host OS.
                -> If an unmatch exist, update it (read blob, send it).
        """

        # -----------------------------------------------
        # First, update our apps bank :
        # -----------------------------------------------
        self.apps.Update()

        # -----------------------------------------------
        # Then, we need to fetch the slider position :
        # -----------------------------------------------
        # Sending command
        sliders: CmdSLPOS = CmdSLPOS()
        sliders, status = self.device.SendCommand(sliders)

        # Handling mute :
        if status == True:
            for index, _ in enumerate(sliders.pos):
                if sliders.mute[index] == True:
                    sliders.pos[index] = 0

                self.logger.debug(
                    f"Slider {index + 1} : {sliders.pos[index]:3.2f}. Muted : {sliders.mute[index]}."
                )

            # Updating volumes
            self.apps.SetSourcesVolumes(sliders.pos)
        else:
            self.logger.warning("Previous command returned an error.")

        # -----------------------------------------------
        # Third, fetch the displayed apps, and ensure
        # they're matching the needed ones. For the ones
        # that do not match, we update them :
        # -----------------------------------------------
        # Sending command
        req_apps: CmdASYNC = CmdASYNC()
        req_apps.add_apps(self.apps.active_apps)
        req_apps, status = self.device.SendCommand(req_apps)

        # Handling asynchronized apps :
        if status == True:
            for index, sync in enumerate(req_apps.sync):
                if sync == False:
                    self.logger.info(
                        f"Apps {index + 1} (ID = {req_apps.apps[index]}) not up to date. Updating it..."
                    )
                else:
                    self.logger.debug(
                        f"Apps {index + 1} (ID = {req_apps.apps[index]}) is up to date."
                    )
        else:
            self.logger.warning("Previous command returned an error.")

        # Function exit
        return

    def calibrate() -> None:
        """
        Calibrate the device to account for the analog dispersion of the sliders.
        This procedure is automatic, but need to be started by the user.

        The method used : 
        STAGE I : Global calibration
            1) Reset the conf to factory default (OFFSET(s) = 0, GAIN(s) = 1.00). This ensure a standard starting point.
            2) Ask the user to place all sliders to the highest position (near the screens) -> Read the values, compute mean.
            3) Ask the user to place all sliders to the lowest position (near touch sensing pads) -> Read the values, compute a mean.
            4) Compute the median offset of the device (median lowest).
            5) Compute the median gain of the device (1 - median highest) + 1.
            5) Apply theses settings.

        STAGE II : Per slider calibration
            For each slider : 
                1) Ask the user to place it on the highest position (near the screen).
                2) Read it's value (must be arround 1.00). Compute it's own gain (1 -  value) + 1.
                3) Ask the user to place it on the lowest position (near touch sensing pads).
                4) Read it's value (must be arround 0.00). Compute it's own offset (- value).
            
            Finally, apply the settings.

        STAGE III : Validation.
            1) Ask the user to play with the slider, and verify that they correctly range from 0 to 100 for all of them.
        """

        # First, create variables : 
        adc_gain = 1.00
        adc_offset = 0.00

        slider_gain = [1.00, 1.00, 1.00, 1.00, 1.00]
        slider_offset = [0.00, 0.00, 0.00, 0.00, 0.00]

        # -------------------------------
        # STAGE 1 :  Global calibration
        # -------------------------------

        
        # -------------------------------
        # STAGE 2 : Per slider calibration
        # -------------------------------

        
        # -------------------------------
        # STAGE 3 : Validation.
        # -------------------------------

        return

    def stop(self, reason: str) -> None:
        """
        Will perform a device stop (SHUTD) --> Won't respond after. Need to reset (manually).

        Typically called after a SIGTERM signal (since app is launched as a deamon / service)
        --> SIGTERM = shutdown.
        """
        self.logger.info("Performing a clean and exit ...")
        self.logger.info(f"    Reason is : {reason}")

        shutdown = CmdSHUTD()
        shutdown = self.device.SendCommand(shutdown)

    def reset(self, reason: str) -> None:
        """
        Will perform a device reset (RINIT). Usefull to recover from errors...

        Typically called after an error, to leave the device into a open state,
        waiting for new connections.
        """
        self.logger.error("Performing a reset of the device ...")
        self.logger.error(f"    Reason is : {reason}")

        rst = CmdRINIT()
        rst = self.device.SendCommand(rst)


if __name__ == "__main__":

    # -----------------------------------------------
    # First, look in which mode we're launched ?
    # -----------------------------------------------
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-c",
        "--calibration",
        help="Place the code in a calibration mode",
        dest="cal",
        action="store_true",
    )
    parser.add_argument(
        "-d",
        "--debug",
        help="Configure the logs to be more verbose",
        dest="debug",
        action="store_true",
    )
    args = parser.parse_args()

    # Defining some parameters
    REFRESH_FREQ = 10

    # Initializing the device
    app = Application(args.debug)

    # Giving some infos
    app.logger.info(f"Getting calibration : {args.cal}")
    app.logger.info(f"Getting debug : {args.debug}")

    if args.cal:
        try:
            app.calibrate()

        except:
            app.reset(f"Done calibration. Exiting ...")
            sys.exit(0)

    else:
        # Running the loop, until somehow something exit
        try:
            while True:
                app.loop()
                time.sleep(1 / REFRESH_FREQ)

        except KeyboardInterrupt:
            app.reset("KeyboardInterrupt")
            sys.exit(0)

        except SystemExit:
            app.stop("SystemExit")
            sys.exit(0)

        except SystemError:
            app.stop("SystemError")
            sys.exit(0)

        except Exception as e:
            app.reset(f"Unhandled exception : {e}")
            sys.exit(-1)
