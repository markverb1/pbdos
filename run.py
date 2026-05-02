
import os
import sys
import subprocess

PROJECT = os.getcwd()
print(PROJECT)
DOSBOX  = f"{os.getcwd()}\\dosbox-x\\dosbox-x.exe"


def build():
    src_dir = "src"
    build_dir = "build"
    sources = sorted(f for f in os.listdir(src_dir) if f.endswith(".c"))

    objs = " ".join(f"$(BUILDDIR)/{f.replace('.c', '.obj')}" for f in sources)

    rules = ""
    for src in sources:
        obj = src.replace(".c", ".obj")
        rules += f"$(BUILDDIR)/{obj} : src\\{src}\n"
        #rules += f"\t@if not exist \"$(BUILDDIR)\" mkdir \"$(BUILDDIR)\"\n"
        rules += f"\t$(CC) src\\{src} $(CFLAGS) $(INC) -fo=$(BUILDDIR)/{obj}\n\n"

    makefile = f"""
CC     = wcc
LINK   = wlink
CFLAGS = -0 -w4 -e25 -zq -od -d2 -bt=dos
INC    = -i="$(WATCOM)\\h"
BUILDDIR = {build_dir}
OBJS   = {objs}
TARGET = $(BUILDDIR)/pbdos.exe

$(TARGET) : $(OBJS)
\t@if not exist \"$(BUILDDIR)\" mkdir \"$(BUILDDIR)\"
\t$(LINK) name $(BUILDDIR)/pbdos sys dos d all op maxe=25,q,symf option map=$(BUILDDIR)/pbdos.map FIL {{$(OBJS)}}

{rules}
clean : .SYMBOLIC
\tdel /Q \"$(BUILDDIR)\"\\*.obj \"$(BUILDDIR)\"\\*.exe \"$(BUILDDIR)\"\\*.map \"$(BUILDDIR)\"\\*.sym
    """

    with open("makefile", "w") as f:
        f.write(makefile)

    print(f"Generated makefile with: {objs}")
    subprocess.run(["wmake"], cwd=PROJECT)

def run_dosbox(exe_args=""):
    subprocess.run("taskkill /im dosbox-x.exe /f".split(' '))
    args = [DOSBOX, "-c", f"mount c {PROJECT}", "-c", "c:", "-c", "cd build"]
    if exe_args:
        args += ["-c", exe_args]
    os.execv(DOSBOX, args)

if "-rn" in sys.argv:
    run_dosbox()
elif "-rd" in sys.argv:
    run_dosbox("pbdos.exe /d")
elif "-r" in sys.argv:
    run_dosbox("pbdos.exe")
elif "-c" in sys.argv:
    build()
else:
    build()
    run_dosbox("pbdos.exe" if "-d" not in sys.argv else "pbdos.exe /d")