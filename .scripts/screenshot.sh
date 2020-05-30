#! /bin/sh

fname="screenshot_$(date +%s).png"
ffmpeg -f x11grab \
		-video_size 1920x1080 \
		-i $DISPLAY \
		-vframes 1 \
		~/Pictures/$fname
wait
