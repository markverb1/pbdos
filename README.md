# PB-DOS

## Playing
run `pbdos.exe` on a machine with DOS support, anything will do  
Command line args:  
`/d` run with debugging enabled (removes some fake loading)

## Requirements
- Open Watcom installed with 16-bit DOS option and set environment variables for it
- DOSBox-X installed at `dosbox-x/` directory (not strictly needed)
- Windows machine (I am not on Linux right now to test)
- Python 3

## Compiling
`python3 ./run.py`  
Command line args:
- `-d` run with debugging enabled (`/d` switch)
- `-r` run without compiling
- `-rn` run DOSBox-X without compiling or running the program
- `-rd` run without compiling with debugging enabled (`/d` switch)
- `-c` compile only without running

To clean up write `wmake clean`.