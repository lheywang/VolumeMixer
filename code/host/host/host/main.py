# ==============================================================================
#   host/main.py : Smooth Windows volume control (Win11) + Calibration
# ==============================================================================

import sys
import traceback
import time
import argparse
import zlib
import random
import statistics
from datetime import datetime

from .dev import (
    CmdASYNC,
    CmdSLPOS,
    CmdUICON,
    CmdDCONF,
    CmdRINIT,
    CmdSHUTD,
    MixerDevice,
)
from .audio import AudioController
from .logger import setup_logger


class Application:
    def __init__(self, debug: bool = False, timing: bool = False) -> None:
        self.logger = setup_logger(debug)
        self.logger.info("Host starting...")

        self.apps = AudioController(self.logger)
        self.device = MixerDevice(self.logger, timing=timing)

        if not getattr(self.device, "IsDeviceOpenned", False):
            self.logger.error("Device not opened. Exiting.")
            sys.exit(-2)

        # ---- smoothing state (5 sliders)
        self._ema = [0.0] * 5
        self._last_sent = [None] * 5
        self._ema_init = False

        # Tunables (feel)
        self.deadband = 0.01   # try 0.005 for more sensitivity
        self.alpha = 0.35      # 0.25 smoother, 0.45 more direct

    def reset(self, reason: str = "Unknown") -> None:
        self.logger.warning(f"Resetting the device. Reason: {reason}")
        try:
            self.device.SendCommand(CmdRINIT())
        except Exception:
            pass
        try:
            self.device.Close()
        except Exception:
            pass

    def stop(self, reason: str = "Unknown") -> None:
        self.logger.warning(f"Shutdown requested. Reason: {reason}")
        try:
            self.device.SendCommand(CmdSHUTD())
        except Exception:
            pass
        try:
            self.device.Close()
        except Exception:
            pass

    # ==============================================================================
    # Calibration (your function reintegrated)
    # ==============================================================================
    def calibrate(self) -> None:
        """
        Calibrate the device to account for the analog dispersion of the sliders.

        Steps:
          - Apply safe base settings
          - Ask user min/max
          - Compute adc_gain/offset + per-slider gain/offset
          - Apply DCONF
          - Reset + exit
        """

        # ---------------- utility: read slider values as 0..100 integers
        def get_values() -> list[int]:
            sliders: CmdSLPOS = CmdSLPOS()
            sliders, status = self.device.SendCommand(sliders)

            if status is True:
                # sliders.pos is 0..1 floats -> convert to 0..100 ints
                return [int(round(x * 100)) for x in sliders.pos]
            else:
                self.logger.warning("SLPOS returned an error during calibration.")
                return [0, 0, 0, 0, 0]

        # ---------------- utility: apply DCONF
        def apply_settings(
            adc_gain: float,
            adc_offset: float,
            slider_gain: list[float],
            slider_offset: list[float],
            dev_name: str,
        ) -> None:
            dconf = CmdDCONF()
            dconf.offset = float(adc_offset)
            dconf.gain = float(adc_gain)
            dconf.potOff = [float(x) for x in slider_offset]
            dconf.potGain = [float(x) for x in slider_gain]
            dconf.device = str(dev_name)

            _, status = self.device.SendCommand(dconf)
            if status is True:
                self.logger.info("Successfully updated device config (DCONF)!")
            else:
                self.logger.warning("DCONF returned an error during calibration.")

        self.logger.info("---------------------------------------")
        self.logger.info("  Welcome to the calibration menu.")
        self.logger.info("  You'll be guided over the steps")
        self.logger.info("---------------------------------------")

        # Configure some fixed settings
        MARGIN = 1  # 1% of the full range 
        GAIN_MARGIN = 1.05

        # First, create variables :
        adc_gain = 25.00
        adc_offset = 0.00

        slider_gain = [1.00, 1.00, 1.00, 1.00, 1.00]
        slider_offset = [0.00, 0.00, 0.00, 0.00, 0.00]

        # This will be a bit imprecise, but anyway, we still shall be unique
        t = datetime.now()
        name = f"{t.year % 100}{t.month % 10}{t.day % 10}HAD{(random.randint(0, 0xFFFFFFFF) % 10):1d}"
        self.logger.info(f"Generated device name : {name}")

        # Erase the previous settings, to ensure a correct base.
        apply_settings(adc_gain, adc_offset, slider_gain, slider_offset, name)

        # -------------------------------
        # STAGE 1 : Global calibration
        # -------------------------------
        input(
            "Place all the sliders on the LOWEST position (near touch pads). Then press ENTER: "
        )
        minimals = get_values()

        input(
            "Place all the sliders on the HIGHEST position (near screens). Then press ENTER: "
        )
        maximals = get_values()

        # Protect against division by zero (if a slider didn't move / faulty read)
        deltas = [maximals[i] - minimals[i] for i in range(5)]
        if any(d <= 0 for d in deltas):
            self.logger.error(f"Invalid deltas detected: {deltas}")
            self.logger.error("Calibration aborted. Check sliders movement and retry.")
            return

        # --- OFFSET ----
        adc_offset = round((statistics.mean([m + MARGIN for m in minimals]) / adc_gain), 2)

        minimals2 = [m + MARGIN - (adc_offset * adc_gain) for m in minimals]
        slider_offset = [round(m2 / adc_gain, 3) for m2 in minimals2]

        # --- GAINS ----
        gains = [adc_gain * (100 / delta) for delta in deltas]

        adc_gain = round(adc_gain * (100 / statistics.mean(deltas)) , 0)
        if adc_gain == 0:
            self.logger.error("adc_gain computed as 0. Calibration aborted.")
            return

        slider_gain = [round(g / adc_gain, 3) for g in gains]
        adc_gain = round(adc_gain * GAIN_MARGIN, 0)

        self.logger.info("Got these values:")
        self.logger.info(f"    - ADC_GAIN :       {adc_gain}")
        self.logger.info(f"    - SLIDER_GAINS :   {slider_gain}")
        self.logger.info(f"    - ADC_OFFSET :     {adc_offset}")
        self.logger.info(f"    - SLIDER_OFFSETS : {slider_offset}")
        self.logger.info("Now applying them to the device ...")

        apply_settings(adc_gain, adc_offset, slider_gain, slider_offset, name)

        # -------------------------------
        # STAGE 2 : End.
        # -------------------------------
        self.logger.info("---------------------------------------")
        self.logger.info("  FINISHED")
        self.logger.info("  Device will now reset, and program will exit.")
        self.logger.info("---------------------------------------")

        # Reset device and exit host cleanly
        self.reset("Done calibration. Exiting ...")
        raise SystemExit(0)

    # ==============================================================================
    # Runtime loop helpers
    # ==============================================================================
    def _smooth(self, raw: list[float], mute: list[bool]) -> list[float]:
        vals = raw[:]

        # Keep your project’s mute logic as-is
        for i in range(5):
            if mute[i] is False:
                vals[i] = 0.0

        if not self._ema_init:
            self._ema = vals[:]
            self._ema_init = True
        else:
            for i in range(5):
                self._ema[i] = (self.alpha * vals[i]) + ((1.0 - self.alpha) * self._ema[i])

        for i in range(5):
            self._ema[i] = max(0.0, min(1.0, self._ema[i]))

        return self._ema

    def read_sliders(self) -> tuple[bool, list[float], list[bool]]:
        sliders, ok = self.device.SendCommand(CmdSLPOS())
        if not ok:
            return False, [], []
        return True, sliders.pos, sliders.mute

    def apply_volumes_if_needed(self, smoothed: list[float]) -> None:
        to_send = smoothed[:]
        changed = False

        for i in range(5):
            last = self._last_sent[i]
            cur = to_send[i]
            if last is None or abs(cur - last) >= self.deadband:
                changed = True
            else:
                to_send[i] = last

        if not changed:
            return

        self.apps.SetSourcesVolumes(to_send)
        for i in range(5):
            self._last_sent[i] = to_send[i]

    def loop_slow(self) -> None:
        self.apps.Update()

        req_apps = CmdASYNC()
        req_apps.add_apps(self.apps.active_apps)

        req_apps, ok = self.device.SendCommand(req_apps)
        if not ok:
            return

        try:
            for index, sync in enumerate(req_apps.sync):
                if sync is False:
                    self.logger.info(
                        f"Slot {index + 1} not up-to-date (ID={req_apps.apps[index]}). Updating icon..."
                    )
                    icon_update = CmdUICON()
                    app = self.apps.active_apps[index]
                    icon_update.slider = app.position
                    icon_update.app = zlib.crc32(app.name.encode())
                    icon_update.icon = app.icon

                    _, ok_uicon = self.device.SendCommand(icon_update)
                    if not ok_uicon:
                        self.logger.warning("Failed to set an icon on the device.")
        except Exception as e:
            self.logger.debug(f"loop_slow exception: {e}")


def main() -> int:
    parser = argparse.ArgumentParser(description="VolumeMixer Host Application")
    parser.add_argument("--debug", action="store_true", help="Enable debug logs.")
    parser.add_argument("--timing", action="store_true", help="Print serial latency in ms.")
    parser.add_argument("--cal", action="store_true", help="Run calibration then exit.")
    args = parser.parse_args()

    app = Application(debug=args.debug, timing=args.timing)

    # If user requested calibration
    if args.cal:
        app.calibrate()
        return 0

    # --- Rates
    READ_HZ = 100
    APPLY_HZ = 60
    APPS_REFRESH_S = 3.0  # reduce CoreAudio rescans

    read_period = 1.0 / READ_HZ
    apply_period = 1.0 / APPLY_HZ

    last_apps = 0.0
    last_apply = 0.0

    latest_raw = [0.0] * 5
    latest_mute = [False] * 5
    ok_last = False

    try:
        while True:
            now = time.time()

            if (now - last_apps) >= APPS_REFRESH_S:
                app.loop_slow()
                last_apps = now

            ok, raw, mute = app.read_sliders()
            if ok:
                latest_raw = raw
                latest_mute = mute
                ok_last = True

            if ok_last and (now - last_apply) >= apply_period:
                smoothed = app._smooth(latest_raw, latest_mute)
                app.apply_volumes_if_needed(smoothed)
                last_apply = now

            time.sleep(read_period)

    except KeyboardInterrupt:
        app.reset("KeyboardInterrupt")
        return 0
    except SystemExit:
        app.stop("SystemExit")
        return 0
    except Exception as e:
        app.reset(f"Unhandled exception: {e}\n{traceback.format_exc()}")
        return -1


if __name__ == "__main__":
    raise SystemExit(main())
