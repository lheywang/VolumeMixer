# ==============================================================================
#   host/audio/utls.py : Implement some utility functions
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Importing modules
import math

# Locals import
from . import AudioActiveApp, AudioSource

# Defining constants
SCALE = 34


# Defining utils functions
def GetdB(value: float) -> float:
    """
    Converts a linear amplitude value to decibels (dB).

    Parameters:
        value (float): The linear amplitude value to convert. Should be >= 0.

    Returns:
        float: The value in decibels (dB). If the input value is less than 0.01, returns -96.0 dB as a floor value.

    Notes:
        - Uses a logarithmic scale for conversion.
        - Assumes the existence of a constant SCALE for scaling the logarithmic result.
    """
    if value >= 0.01:
        return SCALE * math.log(value)
    return -96.0


def GetVal(dB: float) -> float:
    """
    Converts a decibel (dB) value to a linear scale value.
    Parameters:
        dB (float): The value in decibels to convert.
    Returns:
        float: The corresponding linear scale value. Returns 0.00 if dB is less than or equal to -96.0.
    Note:
        The conversion uses the formula: 10^(dB / SCALE).
        The constant SCALE should be defined elsewhere in the code.
    """

    if dB <= -96.0:
        return 0.00
    return math.pow(10, (dB / SCALE))


def GetMatchingAudioSource(
    executables: list[AudioSource], apps: dict, pos: int
) -> tuple:
    """
    Matches audio source executables to application definitions and retrieves associated icon data.

    Args:
        executables (list[AudioSource]): List of AudioSource objects representing running executables.
        apps (dict): Dictionary mapping positions to application name keys and their associated icon filenames.
        pos (int): The position index to select the relevant set of applications from the apps dictionary.

    Returns:
        tuple:
            - executables (list[AudioSource]): The updated list of executables with matched items removed.
            - AudioActiveApp: An instance of AudioActiveApp initialized with matched executables, app name, position, and icon data.
    Notes:
        - If no specific app matches, falls back to an "any" tag if present.
        - Reads icon data from the filesystem based on the matched application's icon filename.
        - Removes matched executables from the input list to prevent duplicate assignments.
    """
    # Initialize var
    ret = []

    # Fetch target apps
    apps = apps[f"{pos}"]

    # First, identify the app
    for index, exe in enumerate(executables):
        for app in apps.keys():
            if app in exe.name:

                # Remove the key from the executables (prevent from doublons on different channels!)
                tmp = []
                tmp.append(exe)
                if executables:
                    executables.pop(index)

                # Check for doublons
                for index2, exe2 in enumerate(executables):
                    if exe2.name == exe.name:
                        tmp.append(exe2)
                        if executables:
                            executables.pop(index2)

                # Build the temp storage values
                ret.append(tmp)
                ret.append(app)
                ret.append(apps[app])

                # Exit loop
                break

    # Removing already assigned elements
    for index, exe in enumerate(executables):
        for app in apps.keys():
            if app in exe.name:
                if executables:
                    executables.remove(exe)

    # Check for the "any" tag
    if not ret:

        # If the keyword "any" is present :
        if "any" in apps.keys():
            if executables:
                ret.append(
                    executables
                )  # Get all executables names --> All affected by the volume
                ret.append("Games")
                ret.append("controller.bin")
            else:
                ret.append(
                    []
                )  # Get all executables names --> All affected by the volume
                ret.append("")
                ret.append("blank.bin")

        else:
            ret.append([])  # Get all executables names --> All affected by the volume
            ret.append("")
            ret.append("blank.bin")

    # Read the bytes
    with open(f"icons/{ret[2]}", "rb") as f:
        data = f.read()

    # Then build the return class
    return executables, AudioActiveApp(ret[0], ret[1], pos, data)
