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
def heatmap_blues(value,
    value_min = 0, value_max = 255,
    color_span = 64):
  scale = (256.0 + float(color_span * 2)) / (value_max - value_min)
  value = scale * (value - value_min)
  return (
    int_clamp(value - color_span * 2, 0, 255),
    int_clamp(value - color_span, 0, 255),
    int_clamp(value, 0, 255)
  )

## Apply heatmap on values dict and returns a transform dict
def apply_heatmap(values, heatmap_fn):
  heatmap = {}
  j = 0
  for i in values:
    heatmap[i] = heatmap_fn(j, value_max = len(values))
    j += 1
  return heatmap

## Input buffer (in our case: stdin)
buf = sys.stdin.buffer

## Supported bits per pixel
im_supported_bpp = [8, 16, 32];

## Read image header
print('reading header...');
im_width  = read_packed_uint(buf, 32)
im_height = read_packed_uint(buf, 32)
im_bpp    = read_packed_uint(buf, 32)

if im_bpp not in im_supported_bpp:
  print('bpp of', im_bpp, 'is not supported!')
  exit(2)

## Create image object
image = Image.new('RGB', (im_width, im_height), 'black')
print('image:', im_width, im_height, im_bpp)

## Initialize buffers and other stuff
im_pixbuf = image.load()
im_buf = []
im_histogram = {}

## Read from stdin to im_buf and fill the histogram
print('reading values...');
for y in range(im_height):
  for x in range(im_width):
    value = read_packed_uint(buf, im_bpp)
    im_buf.append(value)
    if value not in im_histogram:
      im_histogram[value] = 1

## A hack to show min/max of the histogram
print('min:', list(im_histogram.keys())[0],
    ', max:', list(im_histogram.keys())[len(im_histogram) - 1])

print('generating heatmap...');
im_heatmap = apply_heatmap(im_histogram, heatmap_blues)

## Put everything into an image
print('creating image...');
for y in range(im_height):
  for x in range(im_width):
    im_pixbuf[x, y] = im_heatmap[im_buf[y * im_width + x]];

image.show()
