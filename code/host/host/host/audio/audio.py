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

        # Fetch the audio sources available on the system
        self.FetchSources()

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
                                volume=session.channelAudioVolume(),
                                muted=session.SimpleAudioVolume.GetMute(),
                                handle=session,
                            )
                        )
