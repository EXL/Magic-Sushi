#!/usr/bin/env bash

cd "${0%/*}"

rm -Rf magic-sushi
mkdir magic-sushi

cp -R ../Assets magic-sushi
cp ../Magic-Sushi-EZX magic-sushi

cp Magic-Sushi.desktop magic-sushi
cp ../Images/Icons/Magic-Sushi-EZX-IconBig.png magic-sushi
cp ../Images/Icons/Magic-Sushi-EZX-IconSmall.png magic-sushi

tar --numeric-owner --owner=2000 --group=233 -cvf magic-sushi.uncompressed magic-sushi
gzip -9f magic-sushi.uncompressed

rm -Rf magic-sushi
mv magic-sushi.uncompressed.gz Magic-Sushi-EZX_v1.0.0_17-Sep-2022.mpkg
