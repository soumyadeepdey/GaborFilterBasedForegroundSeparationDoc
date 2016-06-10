#!/bin/bash
name='file.txt'
make
ls *.jpg > $name
cat $name | while read LINE
do
echo $LINE
./gabor $LINE
done
rm $name
