# 3D viewer

## Overview

This project was made to render object(.obj) files into 3D-models. Rendering parameters are:

* XYZ scaling
* XYZ rotatingThe
* XYZ movement

They all could be specified by certain fields on graphical interface. The interface itself was mada with GTK+ 3.0
library

## Main functions

Project contains functions to parse *.obj files, write object data to files, async calls for gnuplot and callbacks for 
interface forms

## Testing

All tests are present in `unit_tests/` folder and are made with <check.h> library

## Samples

In this project there are also some object samples, they all are present in `obj_files/` directory
