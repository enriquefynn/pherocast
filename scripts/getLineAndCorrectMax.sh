#!/usr/bin/env zsh

allPercentage=0
files=0
while read line
do
    file=$(echo $line | awk '{print $1}')
    p1=$(echo $line | awk '{print $2}')
    p2=$(echo $line | awk '{print $3}')
    l=`wc -l /home/fynn/GIT/individual/BUS_SE/$file | awk '{print $1}'`
    ((real =  (p1 / p2) * 100 ))
    echo $l $real
done
