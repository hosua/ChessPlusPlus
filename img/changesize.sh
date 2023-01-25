for f in *.png; do
	# convert ./"$f" ./"${f%.png}.bmp"
	convert ./"$f" -resize 50x50\> ./"${f%.png}-50x50.png"
done
