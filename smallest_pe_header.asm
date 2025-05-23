.intel_syntax noprefix

.set MEM_SECTION_ALIGNMENT, 0x1000
.set FILE_SECTIONALIGNMENT, 0x200

.set HEADER_SIZE, HEADER_END - HEADER_START
.set TEXT_SIZE, TEXT_END - TEXT_START

.set MEM_ALIGNED_HEADER_SIZE, HEADER_SIZE + MEM_SECTION_ALIGNMENT - 1 & -MEM_SECTION_ALIGNMENT
.set FILE_ALIGNED_HEADER_SIZE, HEADER_SIZE + FILE_SECTIONALIGNMENT - 1 & -FILE_SECTIONALIGNMENT
.set MEM_ALIGNED_TEXT_SIZE, TEXT_SIZE + MEM_SECTION_ALIGNMENT - 1 & -MEM_SECTION_ALIGNMENT
.set FILE_ALIGNED_TEXT_SIZE, TEXT_SIZE + FILE_SECTIONALIGNMENT - 1 & -FILE_SECTIONALIGNMENT

HEADER_START:
.ascii "MZ"
.zero 0x38 # You may reuse this space for whatever you want
.int 0x40
.ascii "PE"
.zero 2
.short 0x14C
.short 1
.zero 12
.int 0x60 + (DIRECTORIES_END - DIRECTORIES_START)
.int 0x103
.short 0x10B
.zero 2
.int FILE_ALIGNED_TEXT_SIZE
.int 0
.int 0
.int MEM_ALIGNED_HEADER_SIZE
.int MEM_ALIGNED_HEADER_SIZE
.int MEM_ALIGNED_HEADER_SIZE
.int 0x400000
.int MEM_SECTION_ALIGNMENT
.int FILE_SECTIONALIGNMENT
.short 5
.zero 6
.short 5
.zero 6
.int MEM_ALIGNED_HEADER_SIZE + MEM_ALIGNED_TEXT_SIZE
.int FILE_ALIGNED_HEADER_SIZE
.zero 4
.short 3
.short 0x8400
.int 0x100000
.int 0x1000
.int 0x100000
.int 0x1000
.zero 4
.int 0
.ascii ".text"
.zero 3
.int TEXT_SIZE
.int MEM_ALIGNED_HEADER_SIZE
.int FILE_ALIGNED_TEXT_SIZE
.int FILE_ALIGNED_HEADER_SIZE
.zero 12
.int 0xC0000020
DIRECTORIES_START:
DIRECTORIES_END:
HEADER_END:

TEXT_START:
    MOV EAX, 1
    RET
TEXT_END:
