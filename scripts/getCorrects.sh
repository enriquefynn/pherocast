#!/usr/bin/env zsh

allPercentage=0
files=0
for f in `ls *.txt`
{
    echo "Processing $f"
    percentage=`../../scripts/generateCorrects.py < $f`
    (( allPercentage = $allPercentage + $percentage ))
    (( files = $files + 1 ))
    echo "$f: $percentage" >> allFiles.txt
}
(( average = allPercentage / files ))
echo "Average: $average" >> allFiles.txt
