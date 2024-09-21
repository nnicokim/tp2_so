#!/bin/bash
docker run --rm -v $(PWD):/app -it agodio/itba-so-multi-platform:3.0 bash -c "cd /app/Toolchain && make all" 
docker run --rm -v $(PWD):/app -it agodio/itba-so-multi-platform:3.0 bash -c "cd /app/ && make all"

# Agrega un parametro para que se ejecute en modo debug
if [ "$1" = "-w" ]; then
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device intel-hda -device hda-output
    # qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspkelseqemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspkfi
else
    qemu-system-x86_64 -M pc -drive file=Image/x64BareBonesImage.qcow2,format=qcow2 -m 512 -audiodev coreaudio,id=audio0 -machine pcspk-audiodev=audio0 
fi