// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       18/10/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************
#ifndef _FILE_SYS_H_
#define _FILE_SYS_H_

#include "sam.h"
#include "gpio.h"
#include "sys_defs.h"
#include "aux_funcs.h"
#include "timing.h"
#include "dpack_ctrl.h"
#include "./FatFsSrc/ff.h"

#define FILE_SYS_FLAG_FILE		        0x01
#define FILE_SYS_FLAG_FOLDER		    0x02

#define FILE_SYS_FILE_NAME_MAX_LEN        25

#define FILE_SYS_CURRENT_PATH_ARRAY_MAX   50 // the max allowed folders depth on the file explorer

#define FILE_SYS_MOUNT_RETRIES             2 // the max number of attempts to mount the unit



// Synthkore  19-02-2022  Tolaemon
// Description: initializes the module structures
// Receives:
// Returns:
// NOTE:
int16_t FILE_SYS_Init();

// Synthkore  19-02-2022  Tolaemon
// Description: counts the number of elements in the current directory. Depending on the specified 
// parameter "consider_file_folders", the function counts only the files, the folders or both type
// of elements present in the current directory. If file extension parameter is != NULL it will only
// consider files with the specified extension.
// Receives:
//   pui8_file_extension: if different than NULL the routine only counts the files that have
// that extension.
//   ui8_consider_file_folders: the navigation mode specifies if only the files, the folders or both are
// considered when counting the items.
// Returns:
//   >=0 The number of files present on the current folder
//   -1 In case of error
int16_t FILE_SYS_dir_file_count(uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders);

// Synthkore  19-02-2022  Tolaemon
// Description: places the file system navigation index pointing to the next element that matches 
// specified conditions.
// Receives:
//   pui8_file_extension: the file extension that must have the next file where we want to place
//  the navigation index.
//   ui8_consider_file_folders: the navigation mode specifies if only the files, the folders or both are
// considered when selecting the next element.
// Returns:
//   By reference:
//    pFinfo:
//   By value:
//    >=0 with the position where the navigation index as been placed. It should be the same
//  than te value received with the position parameter plus 1.
//    -1 was not possible to place the navigation index at the next position.
// NOTE:
int16_t FILE_SYS_dir_file_goto_next(uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders);

// Synthkore  19-02-2022  Tolaemon
// Description: places the file navigation index pointing to the element at the specified position.
// It counts files, folders or both depending on the parameter "consider_file_folders". In case 
// file_extension is !=NULL it will only consider the files with the specified extension.
// Receives:
//   i16_index: the position in the directory where we want to place the file navigation index
//   pui8_file_extension: if different than NULL the routine only counts the files that have
// that extension.
//   max_file_name_len: the max length allowed of the file name.
//   consider_file_folders: the navigation mode specifies if only the files, the folders or both are
// considered when selecting the next element.
// Returns:
//   By value:
//    >=0 with the position where the navigation index as been placed. It should be the same
//  than the value received with the position parameter.
//    -1 was not possible to place the navigation index at the specified position.
int16_t FILE_SYS_dir_file_goto_index(int16_t i16_index, uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders);

// Synthkore  19-02-2022  Tolaemon
// Description:
//   Returns most important information of the element ( file or folder ) where the directory access pointer is
// currently open. That includes: name, size, and type ( file or folder ).
// Receives:
// Returns:
//  By referene:
//   pui8_file_folder_name: with the name of the file or folder where the the directory access pointer is
// currently pointing to.
//   pui64_size: size of the file where the the directory access pointer is currently pointing to ( in case
// currently pointed element it is a file ).
//   pui8_flag_folder_files: flags with the kind of element the directory access pointer is currently pointing
// to: file or folder.
//  By value:
int16_t FILE_SYS_get_current_info(uint8_t * pui8_file_folder_name, uint16_t ui16_max_name_len, uint64_t * pui64_size, uint8_t * pui8_flag_folder_files);

// Synthkore  19-02-2022  Tolaemon
// Description:
//   Simply checks the state ( OK or NOT OK) of the element ( file or folder ) where the directory access pointer
//  is currently open. It may be usefull to check if the folder or unit is available.
// Receives:
// Returns:
//  By referene:
//    >=0 directory access pointer is OK
//    -1 directory access pointer is NOT OK ( not initialized ? disk unit not mounted ? ... )
int16_t FILE_SYS_check_current_state();

// Synthkore  19-02-2022  Tolaemon
// Description:
//  Writes the specified number of bytes of the received buffer in the currently open file. 
// Receives:
//   p_data_to_write: pointing to the buffer with the data to write into the currently open file.
//   ui32_n_bytes_to_write: number of bytes of the buffer to write into the currently open file.
// Returns:
//  By referene:
//   pui32_n_written_bytes: the number of bytes of the received buffer that could finally be written
//  into the file.
//  By value:
int16_t FILE_SYS_file_write( const void * p_data_to_write, uint32_t ui32_n_bytes_to_write, uint32_t * pui32_n_written_bytes);

// Synthkore  19-02-2022  Tolaemon
// Description:
//   Returns the requested number of bytes from the currently open file. 
// Receives:
//   ui32_n_bytes_to_read: the number of bytes to read from the currently open file.
// Returns:
//  By reference:
//   p_read_data: buffer where the function stores data read from currently open file.
//   pui32_n_read_bytes: number of bytes of the requested number of bytes that the function could finally read.
//  By value:
int16_t FILE_SYS_file_read(const void * p_read_data, uint32_t ui32_n_bytes_to_read, uint32_t * pui32_n_read_bytes);

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Opens the file specified in pui8_path in the mode specified on ui8_mode
// Receives:
//   pui8_path: name of the file to open.
//   ui8_mode: file access mode 
// Returns:
//  By value:
int16_t FILE_SYS_file_open(const uint8_t* pui8_path, uint8_t ui8_mode);

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Closes currently opened file ( if open )
// Receives:
// Returns:
//  By value:
int16_t FILE_SYS_file_close();

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Places the file access pointer pointing to the specified offset inside the currently
//  open file.
// Receives:
//   ui64_ofs: offset in bytes relative to the file begining where to place the file
//  access pointer.
// Returns:
//  By value:
int16_t FILE_SYS_file_lseek(uint64_t ui64_ofs);

// Synthkore  26-02-2022  Tolaemon
// Description:
//  Mounts (intializes) the file system control structures with the data of the specified logical drive.
// Receives:
//   pui8_path: logical drive to be mounted.
//   ui8_mode: 0:Do not mount (delayed mount), 1:Mount immediately
// Returns:
//  By value:
int16_t FILE_SYS_mount(const uint8_t* pui8_path, uint8_t ui8_mode);

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Places the directory access pointer to the specified directory 
// Receives:
//   pui8_path: with the name of the path to place the directory access read pointer on.
// Returns:
//  By value:
int16_t FILE_SYS_dir_open(const uint8_t* pui8_path);

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Places the directory access pointer pointing to the first directory element.
// Receives:
// Returns:
//  By value:
int16_t FILE_SYS_dir_rewind();

// Synthkore  24-07-2022  Tolaemon
// Description:
//   Returns if the file access pointer has reached the end of the file
// Receives:
// Returns:
//  By value:
//   >=0 the end of the file has NOT been reached yet
//   <0 the end of the file has been reached
int16_t FILE_SYS_file_eof();

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Returns the offset of the file access pointer inside the current opened file .
// Receives:
// Returns:
//  By value:
uint64_t FILE_SYS_file_tell();

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Change current directory or current drive
// Receives:
//   pui8_path: with the name of the directory to move to
// Returns:
//  By value:
int16_t FILE_SYS_dir_ch(const uint8_t* pui8_path);

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Creates the specified directory with the specified name.
// Receives:
//   pui8_path: with the name of the directory to create.
// Returns:
//  By value:
int16_t FILE_SYS_dir_mk(const uint8_t* pui8_path);

// Synthkore  26-02-2022  Tolaemon
// Description:
//   Removes the specified element in the path ( file or folder ).
// Receives:
//   pui8_path: with the name of the element ( file or folder ) to remove.
// Returns:
//  By value:
int16_t FILE_SYS_dir_rm(const uint8_t* pui8_path);

// Synthkore  02-10-2022  Tolaemon
// Returns a string with the current path
// Receives:
//	ui16_max_fir_name_len: maximum number of characters that can be stored in the
// string used to return the current path.
// Returns:
//  By reference:
//   pui8_cur_dir: string with the current path the FS is pointing to
//	By value:
//   >=0 the command has been succesfully executed
//	 <0 an error occurred while trying to get the current path
int16_t FILE_SYS_get_cur_dir(uint8_t * pui8_cur_dir,uint16_t ui16_max_fir_name_len);

// Synthkore  02-15-2023  Tolaemon
// Returns the label and the serial number of the current mounted SD card unit.
// Receives:
//	  pui8_label: pointer to the string where the label of current unit will be saved.
//	  pui32_serial_n: pointer to an uint32 where the serial number of the unit
//   will be stored.
// Returns:
//  By reference:
//	  pui8_label: pointer to the string updated with the current mounted unit label name
//	  pui32_serial_n: pointer to an uint32 that contains the current unit serial number.
//	By value:
//    >=0 the command has been succesfully executed
//	  <0 an error occurred while trying to get the current unit label
int16_t FILE_SYS_get_label(uint8_t * pui8_label, uint32_t * pui32_serial_n);

// Synthkore  02-15-2023  Tolaemon
// Returns the size in MegaBytes of the current mounted unit.
// Receives:
//	  pui32_size_in_MB: variable where the size of the current mounted SD unit will
//  be stored.
// Returns:
//  By reference:
//	  pui32_size_in_MB: pointer to the variable that stores the size of the unit in MBytes
//	By value:
//    >=0 the command has been succesfully executed
//	  <0 an error occurred while trying to get the size of the current unit
int16_t FILE_SYS_get_unit_size_MBs(uint32_t * pui32_size_in_MB);

#endif /* _FILE_SYS_H_ */