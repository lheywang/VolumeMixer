# ==============================================================================
#   builder/main.py : Implement the main script to build images for the final
#                     application. This require some development tools to be
#                     installed on the host machine which are complex to install.
#   10/06/2025
#   l.heywang <leonard.heywang@proton.me>
#
#   requirements:
#     - cairosvg (depend on GTK runtime on Windows)
#
# ==============================================================================

# Adding path for cairosvg to work on Windows
import os

gtkbin = r"C:\\Program Files\\GTK3-Runtime Win64\\bin"
os.environ["PATH"] = os.pathsep.join((gtkbin, os.environ["PATH"]))

# Imports
import io
import tomllib
import cairosvg

from tqdm import tqdm
from PIL import Image, ImageOps


def load_svg_image(svg_path):
    """
    Load an SVG image and convert it to a PNG format.
    """
    with open(svg_path, "rb") as f:
        image_data = io.BytesIO(
            cairosvg.svg2png(f.read(), output_width=1000, output_height=1000)
        )
    return Image.open(image_data)


def edit_PIL_image(image):
    """
    Edit the PIL image as needed.
    """
    # First, remove a potential transparent background
    tmp = Image.new("RGBA", image.size, "WHITE")
    tmp.paste(image, (0, 0), image)

    # Convert to grayscale, then to black and white
    tmp = tmp.convert("L")
    tmp = tmp.convert("1")

    # Resize the image to 32x32 pixels
    tmp = tmp.resize((32, 32))

    # Invert the image colors
    tmp = ImageOps.invert(tmp)

    return tmp


def convert_PIL_to_bytes(image):
    """
    Convert a PIL image to bytes.
    """

    # Getting the raw bytes from the image
    # The first 76 bytes are the header, and the last 3 bytes are the footer (C formated string)
    raw_bytes = image.tobitmap()[76:-3]

    # Convert the raw bytes to a string, then to a list of integers
    # Each byte is represented as a hexadecimal string, separated by commas
    tmp = raw_bytes.decode("utf-8")
    tmp = tmp.replace("\n", "")
    tmp = tmp.split(",")

    data = []
    for byte in tmp:
        data.append(int(byte, 16))

    return data


def main():
    # First, read TOML config file :
    with open("icons_raw/attr.toml", "rb") as f:
        config = tomllib.load(f)["icons"]

    # Then iterate over the icons
    path = "icons_raw/"
    for index, icon in tqdm(enumerate(config)):
        # Load the SVG image
        img = load_svg_image(f"{path}{icon}.svg")

        # Tread the image
        img = edit_PIL_image(img)

        # Get the bytes from the image
        dat = convert_PIL_to_bytes(img)

        # Save image to file
        name = f"{config[icon]}".lower()
        with open(f"../host/icons/{name}.bin", "wb+") as f:
            f.write(bytes(dat))


if __name__ == "__main__":
    main()
