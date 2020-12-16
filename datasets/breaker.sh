#!/bin/bash
FILENAME=train.csv
HDR=$(head -1 $FILENAME)                # Pick up CSV header line to apply to each file
numOfLines=$(cat $FILENAME | wc -l)     # Get counts of lines
dataline=$(expr $numOfLines - 1)
divNumLine=$(expr $dataline / $1)
chuncksize=$(expr $divNumLine + 1)      # final size

split -l $chuncksize $FILENAME xyz      # Split the file into chunks of 20 lines each
n=0
for f in xyz*                           # Go through all newly created chunks
do
   if [ $n -gt 0 ]                      # because first file had header itself
   then
      echo $HDR > train_${n}.csv        # Write out header to new file called "Part(n)"
   fi             
   cat $f >> train_${n}.csv             # Add contents of each file from
   rm $f                                # Remove temporary file
   ((n++))                              # Increment name of output part
done

