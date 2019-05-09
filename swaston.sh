#!/bin/bash

if [ $1 ]
then
	color_mode=$1
else
	echo "Choose color mode: "
	echo "1 - classic swaston"
	echo "2 - inverted swaston"
	echo "3 - custom colorset"

	read color_mode
fi

if [ $color_mode == 1 ]
then
	bg_color=0xffff
	color=0x0000
elif [ $color_mode == 2 ]
then
	bg_color=0x0000
	color=0xffff
else
	read -p "background color: 0x" bg_color
	bg_color="0x$bg_color"

	read -p "swaston color: 0x" color
	color="0x$color"
fi

echo $bg_color > /sys/class/st7735s/fill_screen

echo "$color 4 20 17 51" > /sys/class/st7735s/draw_rect
echo "$color 4 71 119 17" > /sys/class/st7735s/draw_rect
echo "$color 55 20 17 119" > /sys/class/st7735s/draw_rect
echo "$color 72 20 51 17" > /sys/class/st7735s/draw_rect
echo "$color 4 122 51 17" > /sys/class/st7735s/draw_rect
echo "$color 106 88 17 51" > /sys/class/st7735s/draw_rect
