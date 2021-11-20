# Font Image Converter

This directory contains a script to convert images into 2D binary arrays for use as a font table for an LED matrix.

## Usage

Run the script from the command line. The script takes one argument, the directory containing images to convert into arrays:

```shell
python font_image_converter.py font_7x10
```

Images should be small (<32 pixels wide) and formatted as black and white. White pixels are read as 'off', black pixels are read as 'on'.

The generated table is interpreted from left to right, top to bottom. Each pixel is represented as a single bit. Use bitwise math to extract the values during assignment.

## Dependencies

This script requires [OpenCV](https://pypi.org/project/opencv-python/) to handle the image processing. Precompiled binaries of OpenCV can be installed via PyPI.

## License

This script is licensed under the terms of the [MIT license](https://opensource.org/licenses/MIT).
