#
#  Project      Cooper River Tournament LED Board
#  @author      David Madison
#  @link        github.com/CobaltEcho/LEDMatrixTesting
#  @license     MIT - Copyright (c) 2021 David Madison
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

# This script automatically generates a C++ 2D array of on/off pixel data from
# a directory of images. The intention is to use this array as a font table
# with an LED matrix.

import cv2
import os
import argparse


def parse_image(image_path):
	img = cv2.imread(image_path, 0)
	if img is None:
		raise RuntimeError("File \"{}\" is not a supported image type".format(image_path))

	dimensions = img.shape  # [rows, columns]
	image_filename = os.path.basename(image_path)
	print("Processing image \"{}\" ({} x {})".format(image_filename, dimensions[1], dimensions[0]))

	# calculates the number of bytes required to store the image
	# representation as a binary bitmap
	width_bytes = int((dimensions[1] / 8)) + (dimensions[1] % 8 != 0)

	if width_bytes > 4:
		raise RuntimeError("Image row is too large to fit into uint32_t type ({} pixels, requires {} bytes)".format(dimensions[0], width_bytes))
	elif width_bytes == 3:
		width_bytes = 4  # no uint_24

	output = {}
	output['image_name'] = os.path.splitext(image_filename)[0]  # filename without extension
	output['width'] = dimensions[1]  # columns
	output['height'] = dimensions[0]  # rows
	output['num_bytes'] = width_bytes

	data = ""
	for row in img:
		row_byte = 0x00
		for index, pixel in enumerate(row):
			if pixel != 255:
				row_byte |= (1 << index)  # each filled pixel is a high bit
		data += "{:#04x}, ".format(row_byte)  # append data as hex

	output['data'] = data[:-2]  # copy and remove last comma

	return output


def main():
	parser = argparse.ArgumentParser(description="image to LED matrix font generator")
	parser.add_argument("directory", help="source directory for font files")

	args = parser.parse_args()

	if not os.path.isdir(args.directory) or os.path.isfile(args.directory):
		parser.error("Not a valid directory")

	# process image files into data arrays
	characters = []
	character_width = 0
	character_height = 0
	max_bytes = 1

	num_characters = 0

	for root, dirs, files in os.walk(args.directory):
		for file in files:
			path = os.path.abspath(os.path.join(args.directory, file))
			image_data = parse_image(path)
			characters.append(image_data)

			if num_characters == 0:
				character_width = image_data['width']
				character_height = image_data['height']
				max_bytes = image_data['num_bytes']
			else:
				if image_data['width'] != character_width \
					or image_data['height'] != character_height \
					or image_data['num_bytes'] != max_bytes:
						raise RuntimeError("Images are not of consistent size")

			num_characters += 1

	print("\nSuccessfully processed {} images".format(num_characters))
	print("------------------------\n")

	# assemble font array from processed data
	print("const uint8_t FontWidth = {};".format(character_width))
	print("const uint8_t FontHeight = {};".format(character_height))
	print("const uint8_t NumCharacters = {};".format(num_characters))
	print()

	output = "const uint{}_t FontTable[NumCharacters][FontHeight] = {{".format(max_bytes * 8)
	for character in characters:
		output += "\n\t{{ {} }},".format(character['data'])
	output = output +  "  //" + image_filename + "/n"
	print(output + '\n')

if __name__ == "__main__":
	main()
