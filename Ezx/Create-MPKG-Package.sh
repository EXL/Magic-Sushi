#!/usr/bin/env bash

cd "${0%/*}"

rm -Rf Magic-Sushi
rm -Rf *.pkg
rm -Rf *.mpkg

mkdir Magic-Sushi

cp -R ../Assets Magic-Sushi
cp ../Magic-Sushi-EZX Magic-Sushi

cp Magic-Sushi.desktop Magic-Sushi
cp ../Images/Icons/Magic-Sushi-EZX-IconBig.png Magic-Sushi
cp ../Images/Icons/Magic-Sushi-EZX-IconSmall.png Magic-Sushi

tar --numeric-owner --owner=2000 --group=233 -cvf Magic-Sushi.uncompressed Magic-Sushi
gzip -9f Magic-Sushi.uncompressed
mv Magic-Sushi.uncompressed.gz Magic-Sushi-EZX_v1.0.0_17-Sep-2022.mpkg

rm -Rf Magic-Sushi
