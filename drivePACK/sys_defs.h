// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       01/04/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************
#ifndef SYS_DEFS_H
#define SYS_DEFS_H

#define TRUE   1
#define FALSE  0
#define NULL   0

#define SW_VERSION "0.6"
#define SW_NAME "dpack RomPack emulator"

// rom file access defines
#define ROMPACK_FILE_EMPTY_CHAR            0xA5
#define ROMPACK_FILE_TAG_N_VERSION         "ROMPACKv02"
#define ROMPACK_FILE_TAG_N_VERSION_SIZE    11
#define ROMPACK_HEADER_NIBBLES             {0x5A} // 0x00, 0x00, 0x0D, 0xF2, 0x00}
#define ROMPACK_HEADER_NUM_BYTES           1
#define ROMPACK_FILE_RW_DEFAULT_N_BYTES    16
#define ROMPACK_FILE_MAX_XML_TAG_SIZE      16

#endif /* SYS_DEFS_H */