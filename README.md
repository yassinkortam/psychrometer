# Psychrometer

## Introduction

The goal of this project was to create a device that can measure ambient temperature and humidity and compute psychrometric calculations. The resulting data can then be viewed on Adafruit IO, a mobile device, or an OLED display.

## config.h

Sets up Adafruit.IO (can't be used concurrently with bluetooth due to storage limitations).

## psychrometry.h

Contains methods to compute psychrometric calculations.

## main.cpp

Driver program. Takes user inputs and executes functions from the above header files.

