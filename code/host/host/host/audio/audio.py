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
from . import AudioSource, AudioActiveApp, GetdB, GetVal, GetMatchingAudioSource


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
        # Set the channels numbers
        self.channel_number = 5

        # Open variables
        # List to hold audio sources
        self.sources: list[AudioSource] = []
        # List to hold active applications with audio sources
        self.active_apps: list[AudioActiveApp] = []

        # Fetch the audio sources available on the system and assign them to the sources list
        self.FetchSources()
        self.AssignShownApps()
        self.SetSourcesVolumes([1.0, 1.0, 1.0, 1.0, 1.0])

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
                                name=sink.name.lower(),
                                id=index + 1,
                                volume=sink.volume.value_flat,
                                muted=sink.mute,
                                handle=sink,  # This is the PulseAudio sink object
                            )
                        )

                    # Get all of the current outputs
                    outputs = pulse.sink_list()
                    actual = pulse.server_info().default_sink_name

                    # Fetch the actual member of the list
                    for index, out in enumerate(outputs):
                        if out.name == actual:
                            output = out

                    self.master.volume = output.volume.value_flat
                    self.master.muted = output.mute
                    self.master.handle = output

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
                                    / 2  # Medians of both channels
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
                self.master.volume = GetVal(volume.GetMasterVolumeLevel())
                self.master.muted = volume.GetMute()
                self.master.handle = volume

        return 0

    def SetSourcesVolumes(self, volumes: list[float]) -> int:
        """
        Sets the volumes for the audio sources.

        Args:
            volumes (list[float]): A list of volume levels to set for each audio source.

        Returns:
            int: 0 on success, -1 on failure.
        """
        if len(volumes) != self.channel_number:
            return -1

        # Ensure all volumes are on the same maximal level
        for index, vol in enumerate(volumes):
            if vol > 1.0:
                volumes[index] = 1.0

        # # Iterate over apps
        for index, app in enumerate(self.active_apps):

            # Then iterate over handles
            for sources in app.source:
                match (self.OS):
                    case "linux":
                        sources.volume = volumes[index]
                        with pulsectl.Pulse() as pulse:
                            sources.handle.volume.volume_flat = volumes[index]
                            pulse.volume_set(
                                sources.handle,
                                pulsectl.PulseVolumeInfo(
                                    [volumes[index], volumes[index]]
                                ),
                            )

                    case "windows":
                        # Master volume
                        if sources.id == 0:
                            sources.handle.SetMasterVolumeLevel(GetdB(volumes[0]), None)
                            sources.volume = volumes[0]

                        # App Volume
                        else:
                            sources.volume = volumes[index]

                            # iterate over the channels
                            chan = sources.handle.channelAudioVolume().GetChannelCount()
                            for k in range(chan):
                                sources.handle.channelAudioVolume().SetChannelVolume(
                                    k, volumes[index], None
                                )

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

        Note :
            The sort process of apps may be a bit complex due to Windows behavior. A registered
            app will tend to remains even if playback is stopped. Thus, the device can't really behave well if
            two concurrent apps are present (ex : Whatsapp AND discord.). In that case, close on (really close),
            wait for refresh and relaunch.
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
        self.active_apps.append(AudioActiveApp([self.master], "master", 0, data))

        # Adding audio sources
        for k in range(self.channel_number - 1):
            self.sources, tmp = GetMatchingAudioSource(self.sources, config, (k + 1))
            self.active_apps.append(tmp)
