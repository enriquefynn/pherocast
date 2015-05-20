#!/usr/bin/env zsh

allActualPercentage=0
allPotentialPercentage=0
files=0
for f in `ls *.txt`
{
    echo "Processing $f"
    percentages=`../../scripts/generateCorrects.py < $f`
    echo $percentages | cut -d' ' -f1 | read actualPercentage
    echo $percentages | cut -d' ' -f2 | read potentialPercentage
    (( allActualPercentage = $allActualPercentage + $actualPercentage ))
    (( allPotentialPercentage = $allPotentialPercentage + $potentialPercentage ))
    (( files = $files + 1 ))
    echo "$f: $actualPercentage $potentialPercentage" >> allFiles.txt
}

(( averageActual = allActualPercentage / files ))
(( averagePotential = allPotentialPercentage / files ))
echo "Average: $averageActual $averagePotential" >> allFiles.txt
