@echo off
echo "----------------------------------------------------------------------------"


set forte_bin_dir="bin\win32"

::rm old folder
rm -r "%forte_bin_dir%"

::call setup_windows.sh
sh.exe ./setup_windows.sh

::goto new Directory
cd "%forte_bin_dir%"

::make
make


echo "----------------------------------------------------------------------------"
echo " DONE. to re-make just type:"
echo "cd %forte_bin_dir%"
echo "make -j" 
echo "----------------------------------------------------------------------------"

