# https://www.geeksforgeeks.org/python-pil-image-resize-method/
# https://holypython.com/python-pil-tutorial/how-to-convert-an-image-to-black-white-in-python-pil/#modes

from PIL import Image
import cairosvg
import io

with open("D:\\Dev\\VolumeMixer\\code\\host\\host\\icons_raw\\90.svg", "rb") as f:
    image_data = io.BytesIO(cairosvg.svg2png(f.read()))
img = Image.open(image_data)
img.show()
