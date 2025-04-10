as.exe --32 tiny_pe.asm
ld -s -m i386pe --exclude-all-symbols --no-seh --disable-auto-import -o tiny_pe2.exe -b pe-i386 a.out