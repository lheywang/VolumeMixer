# ==============================================================================
#   host/audio/classes.py : Implement the audio storage classes
#
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# ------------------------------------------------------------------------------
# Import modules
# ------------------------------------------------------------------------------
from dataclasses import dataclass


# ------------------------------------------------------------------------------
# Define a dataclass for audio sources
# ------------------------------------------------------------------------------
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
    source: AudioSource | list[AudioSource]
    name: str
    position: int
    icon: bytes | None = None
