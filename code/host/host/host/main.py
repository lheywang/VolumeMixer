# ==============================================================================
#   host/main.py : Implement the main logic of sound controller
#
#   08/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
# ==============================================================================

# Add selective import for Windows / Linux agnostic code
import pulsectl


def main():
    with pulsectl.Pulse() as pulse:
        for sink in pulse.sink_list():
            print(sink)

        volumes = [0.25, 0.75]

        for index, sink in enumerate(pulse.sink_input_list()):
            print(sink)
            print(sink.volume)
            pulse.volume_set_all_chans(sink, volumes[index])
            print(sink.volume)

    # This works fine for now !
    # To do : Add a small abstraction class !


if __name__ == "__main__":
    main()
