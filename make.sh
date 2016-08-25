#!/bin/sh
SOURCE=dcf
gcc ${SOURCE}.c -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast $@ -o ${SOURCE} && ./${SOURCE} > out/dcf_code.s

dcf_toolchain/bin/dcf-unknown-elf-as out/dcf_code.s -o out/dcf_code.elf
dcf_toolchain/bin/dcf-unknown-elf-objcopy -R .data -j .text -O binary out/dcf_code.elf out/dcf_code.bin
