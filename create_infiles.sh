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

# if [[ -d "$input_dir" ]] #Check if directory exists. If it doesn't, create it
#     then
#     echo Directory already exist
#     exit
# else
#     mkdir -p "./$input_dir"
# fi

# Give the input file to an array.
# Each array "object" is one word. Therefore, we can calculate where the countries are.
mapfile -d " " array < "$inputfile"
arrayMax=${#array[@]}

while (($i<$arrayMax))
    do
    echo while
    i=$arrayMax
    done
# if [[ ( $lines > 10000) && ($duplicate == 0) ]] # Check if numLines are within the limit for the ids.
#     then
#     echo With duplication disabled, there can be 10000 Ids. If you want to proceed duplication must be enabled.
#     read -p "If you want to proceed press yes: " yes
#     if [[ ("$yes"=="yes") ]] # If user seems to want to continue, enable duplication and make idNum=9999.
#         then
#         idNum=9999
#         duplicate=1
#     else
#         exit
#     fi
# elif [[ ( $lines == 10000) && ($duplicate == 0) ]]
#     then
#     idNum=9999
#     lines=9999
# else
#     idNum=$[$lines]
# fi

# # Name of file that the script is writing to
# input=inputFile.txt

# virus=($(cat "$1")) # Array with viruses names
# countries=($(cat "$2")) # Array with countries
# viruslength=$[${#virus[@]}] # Length of virus array
# countrieslength=$[${#countries[@]}] # Length of countries array

# id=($(shuf -i 0-$idNum -n $lines)) # Array with rundom id's that are going to be used.

# # Empty the new file from the previous things that it contained and insert the first entry
# if [ $(( $RANDOM % 2 )) == 1 ]
#     then
#     vac="YES"
#     Date="$(( $RANDOM % 30 + 1 ))-$(( $RANDOM % 12 +1 ))-$(( $RANDOM % 50 + 2000 ))"
# else
#     vac="NO"
#     Date=""
# fi 
# var="${id[0]} name${id[0]} surname${id[0]} ${countries[$(( $RANDOM % $countrieslength ))]} $(( $RANDOM % 120 + 1 )) ${virus[$(( $RANDOM % $viruslength ))]} $vac $Date"
# echo $var > $input

# # Make of input file
# flag=0
# for ((i=1; i<$lines; i++))
# do
#     if [[ ( $duplicate != 0 ) && ( $(( $RANDOM % 2 )) == 1 ) && ( $flag == 0 ) ]] # Checks if duplication is allowed
#         then 
#         flag=1
#         echo $var >> $input
#         continue
#     fi

#     if [ $(( $RANDOM % 2 )) == 1 ] # Figures out if the citizen is vaccinated or not
#         then
#         vac="YES"
#         Date="$(( $RANDOM % 30 + 1 ))-$(( $RANDOM % 12 +1 ))-$(( $RANDOM % 50 + 2000 ))"
#     else
#         vac="NO"
#         Date=""
#     fi 

#     flag=0
#     var="${id[$i]} name${id[$i]} surname${id[$i]} ${countries[$(( $RANDOM % $countrieslength ))]} $(( $RANDOM % 120 + 1 )) ${virus[$(( $RANDOM % $viruslength ))]} $vac $Date"
#     echo $var >> $input

# done
