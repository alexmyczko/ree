[![Codacy Badge](https://api.codacy.com/project/badge/Grade/b5bebc8b8aed438da6bf0b2a63161d7a)](https://www.codacy.com/app/alexmyczko/ree?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=alexmyczko/ree&amp;utm_campaign=Badge_Grade)

# ree
Extract BIOS ROM extensions

includes the tool `fontdump` (to extract the fonts from your video bios roms)

make sure you can read /dev/mem (be root)
chmod +x ree
./ree

or use the c version (much faster, and does checksum on the rom extension)
make # or gcc ree -o ree.bin
./ree.bin

will scan your system memory, address c0000 - f0000 in 512 byte steps for
identification of rom extensions (55,aa), if found it will calculate
its size (byte after id, multiplied by 512) and saves that to a .rom file.

ported to shell from the pascal version of 1997
original (dos/win9x) version is ree.exe and ree.pas in ree.zip

note
as most system bios are bigger than 64k nowadays (128k or 256k) and them
are only 64k memory window reserved, they are not dumped completely, if
anyone knows how to dump the completely, please contact me

links
flashrom, https://github.com/flashrom/flashrom
seabios
coreboot
