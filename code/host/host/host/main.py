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
import statistics
import sys
import time
import argparse
from datetime import datetime
import random

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

    def calibrate(self) -> None:
        """
        Calibrate the device to account for the analog dispersion of the sliders.
        This procedure is automatic, but need to be started by the user.

        The method used :
            - Set gain and parameters to a safe value. (Mostly 0.000 and 1.000), except the global gain to 25.
            - Ask the user to place slider in different positions, to fetch data.
            - Use maths to compute the different elements.
            - Apply
            
        """

        # Define a utility function to read the values more easily :

        # -----------------------------------------------------------------------------------------------------------------
        def get_values() -> list[int]:
            """
            Small utility function, used to fetch the slider positions without needing to define functions, and so on...
            """
            # Sending command
            sliders: CmdSLPOS = CmdSLPOS()
            sliders, status = self.device.SendCommand(sliders)

            # Fetching values
            if status == True:
                # Updating volumes
                return list(map(lambda x: x * 100, sliders.pos))
            else:
                self.logger.warning("Previous command returned an error.")
                return [0, 0, 0, 0, 0]

        # -----------------------------------------------------------------------------------------------------------------
        def apply_settings(
            adc_gain: float,
            adc_offset: float,
            slider_gain: list[float],
            slider_offset: list[float],
            dev_name: str,
        ) -> None:
            """
            Small utility function to apply settings to the device, before getting the next calibration steps.
            """
            # Creating command
            dconf = CmdDCONF()

            dconf.offset = adc_offset
            dconf.gain = adc_gain

            dconf.potOff = slider_offset
            dconf.potGain = slider_gain

            dconf.device = dev_name

            # Send command
            dconf, status = self.device.SendCommand(dconf)

            # Fetching values
            if status == True:
                # Updating volumes
                self.logger.info("Sucessfully updated device config !")
                return
            else:
                self.logger.warning("Previous command returned an error.")
                return

        # -----------------------------------------------------------------------------------------------------------------

        self.logger.info("---------------------------------------")
        self.logger.info("  Welcome to the calibration menu.")
        self.logger.info("  You'll be guided over the steps")
        self.logger.info("---------------------------------------")

        # Configure some fixed settings
        MARGIN = 1 # 1% of the full range

        # First, create variables :
        adc_gain = 25.00
        adc_offset = 0.00

        slider_gain = [1.00, 1.00, 1.00, 1.00, 1.00]
        slider_offset = [0.00, 0.00, 0.00, 0.00, 0.00]

        time = datetime.now()
        name = f"{time.year % 100}{time.month}{time.day}HAD{(random.randint(0, 0xFFFFFFFF) % 10):1d}"
        self.logger.info(f"Generated device name : {name}")

        # Erase the previous settings, to ensure a correct base.
        apply_settings(adc_gain, adc_offset, slider_gain, slider_offset, name)

        # -------------------------------
        # STAGE 1 :  Global calibration
        # -------------------------------
        # Ask user to place the sliders on the lowest position :
        input(
            "Place all the sliders on the lowest position (near the touch sensing pads). Then, hit enter : "
        )

        # Compute the ADC offset :
        minimals = get_values()

        # Ask the user to place the slider on the middle position.
        # This ensure we won't overflow to 100 (otherwise gain = perfect !!).
        #
        input(
            "Place all the sliders on the highest position (near the screens). Then, hit enter : "
        )

        maximals = get_values()

        """
        This second part of the code is quite heavy on the math, but enable a smooth user experience ...

        First, we compute the gains, an integer : 
        We're using the smallest range for all of them, to remove errors, and remove stress on the final stages.
        Then, we compute the individual gains, by dividing their gain by the target one.
        
        Finally, we compute the offsets, in a very similar method
        """

        # --- OFFSET ----
        adc_offset = round((statistics.mean([minimal + MARGIN for minimal in minimals]) / adc_gain), 2)

        # Get the remaining minimals
        minimals2 = [minimal + MARGIN - (adc_offset * adc_gain) for minimal in minimals]
        slider_offset = [round(minimal2 / adc_gain, 3) for minimal2 in minimals2]

        # --- GAINS ----
        # First, compute the deltas, and the ideal gains.
        deltas = [maximals[i] - minimals[i] for i in range(len(maximals))]
        gains = [adc_gain * (100 / delta) for delta in deltas]

        # Then, compute the gains values
        adc_gain = round(adc_gain * (100 / statistics.mean(deltas)), 0)
        slider_gain = [round(gain / adc_gain, 3) for gain in gains]

        # Debug prints
        self.logger.info(f"Got theses values : ")
        self.logger.info(f"    - ADC_GAIN :       {adc_gain}")
        self.logger.info(f"    - SLIDER_GAINS :   {slider_gain}")
        self.logger.info(f"    - ADC_OFFSET :     {adc_offset}")
        self.logger.info(f"    - SLIDER_OFFSETS : {slider_offset}")
        self.logger.info("     ")
        self.logger.info("Now applying them to the device ...")

        # Then, apply theses values :
        apply_settings(adc_gain, adc_offset, slider_gain, slider_offset, name)

        # -------------------------------
        # STAGE 2 : End.
        # -------------------------------
        self.logger.info("---------------------------------------")
        self.logger.info("  FINISHED")
        self.logger.info("  Device will now reset, and program will exit.")
        self.logger.info("---------------------------------------")

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
        app.calibrate()
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
