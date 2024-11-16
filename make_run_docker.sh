#!/bin/bash
# Agrega un parametro para que se ejecute en modo debug
# Build parameters for debug mode:
# -d: Builds the kernel in debug mode and runs it in QEMU
# -w: Builds the kernel for windows and runs it in QEMU
args=("$@")
isWindows=false
debug_params=""
withBuddy=false

## Reviso si algún argumento es -d o -w
for arg in "${args[@]}"; do
    if [ "$arg" = "-d" ]; then
        debug_params="-s -S"
    elif [ "$arg" = "-w" ]; then
        isWindows=true
    fi
    if [ "$arg" = "-b" ]; then
        withBuddy=true
    fi
done

if [ "$withBuddy" = true ]; then
    docker run --rm -v $(PWD):/app -it agodio/itba-so-multi-platform:3.0 bash -c "cd /app/Toolchain && make all" 
    docker run --rm -v $(PWD):/app -it agodio/itba-so-multi-platform:3.0 bash -c "cd /app/ && make buddy"
else
    docker run --rm -v $(PWD):/app -it agodio/itba-so-multi-platform:3.0 bash -c "cd /app/Toolchain && make all" 
    docker run --rm -v $(PWD):/app -it agodio/itba-so-multi-platform:3.0 bash -c "cd /app/ && make all"
fi


if [ isWindows = true ]; then
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device intel-hda -device hda-output $debug_params
else
    qemu-system-x86_64 -M pc -drive file=Image/x64BareBonesImage.qcow2,format=qcow2 -m 512 -audiodev coreaudio,id=audio0 -machine pcspk-audiodev=audio0 $debug_params
fi