# ==============================================================================
#   host/main.py : Implement the main logic of sound controller
#
#   08/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

from audio.audio import AudioController

if __name__ == "__main__":
    controller = AudioController()
    print(controller.sources)
