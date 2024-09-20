#!/bin/bash

qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device intel-hda -device hda-output
