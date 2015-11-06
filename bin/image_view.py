#!/usr/bin/python3

import sys
import time
import PIL
import struct
import math

from PIL import Image

## Reads binary int from buffer
def read_packed_uint(buf, bpp = 32):
  if bpp == 8:
    return struct.unpack('B', buf.read(1))[0];
  if bpp == 16:
    return struct.unpack('H', buf.read(2))[0];
  if bpp == 32:
    return struct.unpack('I', buf.read(4))[0];
  return None

def int_clamp(x, x_min, x_max):
    return int(max(x_min, min(x, x_max)))

## Map value to a color space of blues
def heatmap_blues_from_value(value,
    value_min = 0, value_max = 255,
    color_span = 128):
  scale = (256.0 + float(color_span * 2)) / (value_max - value_min)
  value = scale * (value - value_min)
  return (
    int_clamp(value - color_span * 2, 0, 255),
    int_clamp(value - color_span, 0, 255),
    int_clamp(value, 0, 255)
  )

## Input buffer (in our case: stdin)
buf = sys.stdin.buffer

## Supported bits per pixel
image_supported_bpp = [8, 16, 32];

## Read image header
print('reading header...');
image_width  = read_packed_uint(buf, 32)
image_height = read_packed_uint(buf, 32)
image_bpp    = read_packed_uint(buf, 32)

if image_bpp not in image_supported_bpp:
  print('bpp of', image_bpp, 'is not supported!')
  exit(2)

## Create image object
image = Image.new('RGB', (image_width, image_height), 'black')
print('image:', image_width, image_height, image_bpp)

## Initialize buffers and other stuff
image_pixbuf = image.load()
image_valbuf = [];
image_valmin = math.inf;
image_valmax = -math.inf;

## Read from stdin to image_valbuf and determine min/max value
print('reading values...');
for y in range(image_height):
  for x in range(image_width):
    value = read_packed_uint(buf, image_bpp);
    image_valbuf.append(value);
    if value < image_valmin:
      image_valmin = value
    if value > image_valmax:
      image_valmax = value

print('min:', image_valmin, ', max:', image_valmax)

## Put everything into an image
print('creating image...');
for y in range(image_height):
  for x in range(image_width):
    image_pixbuf[x, y] = heatmap_blues_from_value(
      image_valbuf[y * image_width + x],
      value_min = image_valmin,
      value_max = image_valmax,
      color_span = 64
    )

image.show()
