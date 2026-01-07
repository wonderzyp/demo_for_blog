#!/bin/bash
set -e

clang --target=aarch64-pc-windows-msvc -ffreestanding -fshort-wchar -nostdlib -c main.c   -o main.obj
clang --target=aarch64-pc-windows-msvc -ffreestanding -fshort-wchar -nostdlib -c efi.c    -o efi.obj
clang --target=aarch64-pc-windows-msvc -ffreestanding -fshort-wchar -nostdlib -c common.c -o common.obj

lld-link /subsystem:efi_application /entry:efi_main /out:BOOTAA64.EFI main.obj efi.obj common.obj
mv BOOTAA64.EFI esp/EFI/BOOT/

qemu-system-aarch64 \
    -machine virt \
    -cpu cortex-a57 \
    -m 1024 \
    -nographic \
    -drive if=pflash,format=raw,readonly=on,file=AAVMF_CODE.fd \
    -drive format=raw,file=fat:rw:esp \
    -drive file=rootfs.ext4,format=raw,if=virtio \
    -net none
