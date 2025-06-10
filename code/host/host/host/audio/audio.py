# ==============================================================================
#   host/audio/audio.py : Implement the audio controller for the host
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Import necessary libraries
import sys
from dataclasses import dataclass


# Import the appropriate audio libraries based on the operating system
match (sys.platform):
    case "linux":
        # Linux
        import pulsectl

        OS = "linux"

    case "win32":
        # Windows
        from pycaw.pycaw import AudioUtilities

        OS = "windows"

    case _:
        raise NotImplementedError(f"Platform {sys.platform} is not supported.")


# Define a dataclass for audio sources
@dataclass
class AudioSource:
    name: str
    id: int
    volume: float
    muted: bool
    handle: None  # Depending on the OS, this could be a PulseAudio source or a Windows audio session handle


# Define a dataclass for apps
@dataclass
class AudioActiveApp:
    source: AudioSource
    name: str
    position: int
    icon: bytes | None = None


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
                                name=session.Process.name(),
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
                                handle=session,
                            )
                        )

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
            - Firefox volume
            - VoIP volume
            - Music app
            - Game app --> In reality, any unknown app will be assigned to the game app

        Linux :
            - Master volume
            - Firefox volume
            - Firefox volume (twice, since linux set one sink per active tab)
            - Music app
            - VoIP volume
        """
        # Todo : Handle speaker openning (include file copy ? )
        # with open() as f:
        #     data = f.read()

        # self.active_apps.append(AudioActiveApp(self.master, "master", 0, data))

        # Todo : Handle parsing of apps name, and then assign the right icons data
        # Todo : Assign app place
