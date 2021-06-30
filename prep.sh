#!/bin/env bash


# .gitignore
# Makefile
# inc/bitmap_image.h
# inc/color.h
# inc/light.h
# inc/object.h
# inc/vector.h
# scene.txt
# scene_guide.txt
# specs.pdf
# src/color.cpp
# src/light.cpp
# src/main.cpp
# src/object.cpp
# src/vector.cpp

rm -rf 1605018/
mkdir 1605018
cp inc/bitmap_image.h 1605018/
cp scene.txt 1605018/

FILE="1605018/1605018_header.h"
rm -f "$FILE"
touch "$FILE"

FN="inc/vector.h"
LN=$(wc -l "$FN" | awk '{print $1}')
LN=$((LN - 2))
sed -n -e "3,$LN p" -e "$LN q" "$FN" >> "$FILE"

FN="inc/color.h"
LN=$(wc -l "$FN" | awk '{print $1}')
LN=$((LN - 2))
sed -n -e "3,$LN p" -e "$LN q" "$FN" >> "$FILE"

FN="inc/light.h"
LN=$(wc -l "$FN" | awk '{print $1}')
LN=$((LN - 2))
sed -n -e "3,$LN p" -e "$LN q" "$FN" >> "$FILE"

FN="inc/object.h"
LN=$(wc -l "$FN" | awk '{print $1}')
LN=$((LN - 2))
sed -n -e "3,$LN p" -e "$LN q" "$FN" >> "$FILE"

sed -i '/#include ".*\.h"/d' "$FILE"


FILE="1605018/1605018_main.cpp"
rm -f "$FILE"
touch "$FILE"


FN="src/vector.cpp"
LN=$(wc -l "$FN" | awk '{print $1}')
sed -n -e "1,$LN p" -e "$LN q" "$FN" >> "$FILE"

FN="src/color.cpp"
LN=$(wc -l "$FN" | awk '{print $1}')
sed -n -e "1,$LN p" -e "$LN q" "$FN" >> "$FILE"

FN="src/light.cpp"
LN=$(wc -l "$FN" | awk '{print $1}')
sed -n -e "1,$LN p" -e "$LN q" "$FN" >> "$FILE"

FN="src/object.cpp"
LN=$(wc -l "$FN" | awk '{print $1}')
sed -n -e "1,$LN p" -e "$LN q" "$FN" >> "$FILE"

FN="src/main.cpp"
LN=$(wc -l "$FN" | awk '{print $1}')
sed -n -e "1,$LN p" -e "$LN q" "$FN" >> "$FILE"

sed -i '/#include ".*\.h"/d' "$FILE"

sed -i '1s/^/#include "1605018_header.h"\n/' "$FILE"
sed -i '1s/^/#include "bitmap_image.h"\n/' "$FILE"

zip -r 1605018.zip 1605018/
mv 1605018.zip /home/enan/Downloads/
