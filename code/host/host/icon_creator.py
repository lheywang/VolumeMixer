# https://www.geeksforgeeks.org/python-pil-image-resize-method/
# https://holypython.com/python-pil-tutorial/how-to-convert-an-image-to-black-white-in-python-pil/#modes

from PIL import Image

im = Image.open(
    "/home/lheywang/Dev/VolumeMixer/code/host/host/icons_raw/Amd-Logo--Streamline-Logos.svg"
)
im.show()
