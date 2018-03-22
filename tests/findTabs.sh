#!/bin/bash


if [ "$WORKSPACE" == "" ]
then 
  WORKSPACE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && cd ../ && pwd )"
fi

FOLDERS="$WORKSPACE/buildsupport $WORKSPACE/src $WORKSPACE/tests"

echo "Looking for tabs in folders:"

for folder in $FOLDERS;
do
  echo $folder
done


FOUND_TABS=$(grep -rP '\t' $FOLDERS)

if [ "$FOUND_TABS" == "" ]
then
  echo "No tabs found"
  exit 0;
else
  echo "Error: We found some tabs in the code. Please change them for two spaces. See list of tabs below:"
  echo "$FOUND_TABS"
  exit 1;
fi

