# ==============================================================================
#   host/audio/audio.py : Implement the audio controller for the host
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Import necessary libraries
import sys
import os
import tomllib

# Custom imports
from . import AudioSource, AudioActiveApp, GetdB, GetMatchingAudioSource


# Import the appropriate audio libraries based on the operating system
match (sys.platform):
    case "linux":
        # Linux
        import pulsectl

        OS = "linux"

    case "win32":
        # Windows
        from comtypes import CLSCTX_ALL
        from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume

        OS = "windows"

    case _:
        raise NotImplementedError(f"Platform {sys.platform} is not supported.")


class AudioController:

    def __init__(self):
        self.OS = OS
        self.master = AudioSource(
            name="Master",
            id=0,
            volume=1.0,
            muted=False,
            handle=None,  # This will be set based on the OS
        )
        self.sources = []  # List to hold audio sources
        self.active_apps = []  # List to hold active applications with audio sources

        # Fetch the audio sources available on the system and assign them to the sources list
        self.FetchSources()
        self.AssignShownApps()

    def FetchSources(self):
        """
        Fetch the audio sources available on the system
        """
        match (self.OS):
            case "linux":
                with pulsectl.Pulse() as pulse:
                    for index, sink in enumerate(pulse.sink_input_list()):
                        self.sources.append(
                            AudioSource(
                                name=sink.name,
                                id=index + 1,
                                volume=sink.volume.value_flat,
                                muted=sink.mute,
                                handle=sink,  # This is the PulseAudio sink object
                            )
                        )

            case "windows":
                sessions = AudioUtilities.GetAllSessions()
                for index, session in enumerate(sessions):
                    if session.Process is not None:
                        self.sources.append(
                            AudioSource(
                                name=session.Process.name().lower(),
                                id=index + 1,
                                volume=(
                                    (
                                        session.channelAudioVolume().GetChannelVolume(0)
                                        + session.channelAudioVolume().GetChannelVolume(
                                            1
                                        )
                                    )
                                    / 2
                                ),
                                muted=session.SimpleAudioVolume.GetMute(),
                                handle=session,  # This is the handle to the session
                            )
                        )

                master = AudioUtilities.GetSpeakers()
                interface = master.Activate(
                    IAudioEndpointVolume._iid_, CLSCTX_ALL, None
                )
                volume = interface.QueryInterface(IAudioEndpointVolume)
                self.master.volume = volume.GetMasterVolumeLevel()
                self.master.muted = volume.GetMute()
                volume.SetMasterVolumeLevel(-15, None)

        return 0

    def SetSourcesVolumes(self, volumes: list[float]) -> int:
        """
        Sets the volumes for the audio sources.

        Args:
            volumes (list[float]): A list of volume levels to set for each audio source.

        Returns:
            int: 0 on success, -1 on failure.
        """
        if len(volumes) != len(self.sources):
            return -1

        return 0

    def AssignShownApps(self):
        """
        Assigns the shown applications to the audio sources.
        This method is a placeholder for future implementation.

        Windows :
            - Master volume
            - VoIP volume
            - Firefox volume
            - Music app
            - Game app --> In reality, any unknown app will be assigned to the game app

        Linux :
            - Master volume
            - Firefox volume
            - Firefox volume (twice, since linux set one sink per active tab)
            - Music app
            - VoIP volume
        """
        # First, load config files
        match (self.OS):
            case "linux":
                with open("config/linux.toml", "rb") as f:
                    config = tomllib.load(f)

            case "windows":
                with open("config/win.toml", "rb") as f:
                    config = tomllib.load(f)

        # Reading base icon data
        with open("icons/speaker.bin", "rb") as f:
            data = f.read()

        # ensure the list is clean
        self.active_apps.clear()

        # Append the base handle, for the master volume
        self.active_apps.append(AudioActiveApp(self.master, "master", 0, data))

        # Adding audio sources
        self.active_apps.append(GetMatchingAudioSource(self.sources, config["1"], 1))
        self.active_apps.append(GetMatchingAudioSource(self.sources, config["2"], 2))
        self.active_apps.append(GetMatchingAudioSource(self.sources, config["3"], 3))
        self.active_apps.append(GetMatchingAudioSource(self.sources, config["4"], 4))

        # Todo : Handle parsing of apps name, and then assign the right icons data
        # Todo : Assign app place
        # Todo : Handle doublons (discord !) --> For volume change, we check all of the names and then change all of the volumes
