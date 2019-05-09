#!/bin/bash
read -p "background color: " bg_color

read -p "swaston color: " color

echo $bg_color > /sys/class/st7735s/fill_screen



echo "$color 4 20 17 51" > /sys/class/st7735s/draw_rect
echo "$color 4 71 119 17" > /sys/class/st7735s/draw_rect
echo "$color 55 20 17 119" > /sys/class/st7735s/draw_rect
echo "$color 72 20 51 17" > /sys/class/st7735s/draw_rect
echo "$color 4 122 51 17" > /sys/class/st7735s/draw_rect
echo "$color 106 88 17 51" > /sys/class/st7735s/draw_rect
