#!/bin/bash
#! RECIBIR POR PARAMETRO UN ARGUMENTO QUE SEA -c PARA COMP
if(($#==1))
then
    if [ $1 == "-d" ]
    then
    echo "Debuggeando..."
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device intel-hda -device hda-output -S -s
    fi
else
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device intel-hda -device hda-output
fi
