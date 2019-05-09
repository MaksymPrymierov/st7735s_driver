#!/bin/bash
echo 0xf00f > /sys/class/st7735s/fill_screen

x=64
y=80

if [ $1 ]
then
	x=$1
fi

if [ $2 ]
then
	y=$2
fi

if [ $x -lt 30 ]
then
	x=30
elif [ $x -gt 98 ]
then
	x=98
fi

if [ $y -lt 20 ]
then
	y=20
elif [ $y -gt 130 ]
then
	y=130
fi

echo "0x0000 $(( $x - 10 )) $y 20 10" > /sys/class/st7735s/draw_rect
echo "0x0000 $(( $x - 20 )) $(( $y + 10 )) 40 10" > /sys/class/st7735s/draw_rect
echo "0x0000 $(( $x - 20 )) $(( $y + 20 )) 10 10" > /sys/class/st7735s/draw_rect
echo "0x0000 $(( $x + 10 )) $(( $y + 20 )) 10 10" > /sys/class/st7735s/draw_rect
echo "0x0000 $(( $x - 30 )) $(( $y - 20 )) 20 20" > /sys/class/st7735s/draw_rect
echo "0x0000 $(( $x + 10 )) $(( $y - 20 )) 20 20" > /sys/class/st7735s/draw_rect

