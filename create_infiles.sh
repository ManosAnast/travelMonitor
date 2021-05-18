#!/bin/bash

total=$#
Check=3
#test number of parametrs
if (( $total<$Check ))
    then
    echo Bash Format is ./create_infiles.sh inputFile input_dir numFilesPerDirectory 
    exit
fi

inputfile="./$1"  #number of lines that the file will have
input_dir=$2 #duplicateallowed flag
numfiles=$3

if [[ -d "$input_dir" ]] #Check if directory exists. If it doesn't, create it
    then
    echo Directory already exist
    exit
else
    mkdir -p "./$input_dir"
fi

# Give the input file to an array.
# Each array "object" is one word. Therefore, we can calculate where the countries are.
mapfile -t arrayLine < "$inputfile"
IFS=' ' read -r -a arrayData <<< ${arrayLine[@]}
arrayMax=${#arrayData[@]}

# Create subdirectories and files for every country
let i=3
let k=0
while [[ $i -le $arrayMax ]]
    do

    if [[ ! -d "./$input_dir${arrayData[$i]}" ]] # If the subdirectory doesn't exist, create it and it's files.
        then
        mkdir -p "./$input_dir${arrayData[$i]}"
        for ((j=1; j<=$numfiles; j++))
            do
            touch "./$input_dir${arrayData[$i]}/${arrayData[$i]}-$j.txt"
        done
    fi
    
    # See readme.
    i=$(($i+3))
    if [[ ("${arrayData[$i]}" == "YES") ]]
        then
        i=$(($i+5))
    else
        i=$(($i+4))
    fi
    k=$(($k+1))
done

# Traverse the subdirectories that you made at the previous loop.
# Based on the country of every entry put it at the correct subdirectory.
dirlist=$(find $"./$input_dir" -mindepth 1 -maxdepth 1 -type d) # List of the subdirectories
for dir in $dirlist
do
    cd $dir # Enter the subdirectory
    files=$(find -not -name "*1.txt" -type f) # Find all the files that are not the first one
    firstfile=$(find -name "*1.txt" -type f) # Find the first file
    IFS=' ' read -r -a filelist <<< ${files[@]} # Make an array with the files

    let i=3
    let k=0
    let j=1
    while [[ $i -lt $arrayMax ]] # The same loop as before, but now we insert to the appropriate file the entries
    do
        if [[ ("./$input_dir${arrayData[$i]}" == "$dir") ]]
        then
            if [[ $j == 1 ]] || [[ $numfiles == 1 ]]
            then
                echo ${arrayLine[$k]} >> "${firstfile}"
                j=0
            else
                echo ${arrayLine[$k]} >> "${filelist[$j]}"
                j=$(($j+1))
            fi
        fi

        i=$(($i+3))
        if [[ ("${arrayData[$i]}" == "YES") ]]
            then
            i=$(($i+5))
        else
            i=$(($i+4))
        fi
        k=$(($k+1))
    done
    cd ../..
done
