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
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
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

/*********************************************************************************************
* @brief initializes the module structures
* @return
* @note Tolaemon 19-02-2022
*********************************************************************************************/
int16_t FILE_SYS_Init();

/*********************************************************************************************
* @brief counts the number of elements in the current directory. Depending on the specified 
* parameter "consider_file_folders", the function counts only the files, the folders or both type
* of elements present in the current directory. If file extension parameter is != NULL it will only
* consider files with the specified extension.
* @param[in]  pui8_file_extension if different than NULL the routine only counts the files that have
* that extension.
* @param[in] ui8_consider_file_folders the navigation mode specifies if only the files, the 
* folders or both are considered when counting the items.
* @return >=0 The number of files present on the current folder, -1 In case of error
* @note Tolaemon 19-02-2022
*********************************************************************************************/
int16_t FILE_SYS_dir_file_count(uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders);

/*********************************************************************************************
* @brief places the file system navigation index pointing to the next element that matches 
* specified conditions.
* @param[in] pui8_file_extension the file extension that must have the next file where we want 
* to place the navigation index.
* @param[in] ui8_consider_file_folders the navigation mode specifies if only the files, the 
* folders or both are considered when selecting the next element.
* @param[out]  pFinfo
* @return >=0 with the position where the navigation index as been placed. It should be the 
* same than the value received with the position parameter plus 1, -1 was not possible to place 
* the navigation index at the next position.
* @note Tolaemon 19-02-2022
*********************************************************************************************/
int16_t FILE_SYS_dir_file_goto_next(uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders);

/*********************************************************************************************
* @brief places the file navigation index pointing to the element at the specified position.
* It counts files, folders or both depending on the parameter "consider_file_folders". In case 
* file_extension is !=NULL it will only consider the files with the specified extension.
* @param[in] i16_index the position in the directory where we want to place the file navigation
* index.
* @param[in] pui8_file_extension if different than NULL the routine only counts the files that 
* have that extension.
* @param[in] max_file_name_len: the max length allowed of the file name.
* @param[in] consider_file_folders: the navigation mode specifies if only the files, the folders
* or both are considered when selecting the next element.
* @return >=0 with the position where the navigation index as been placed. It should be the same
* than the value received with the position parameter, -1 was not possible to place the navigation
* index at the specified position.
* @note Tolaemon 19-02-2022
*********************************************************************************************/
int16_t FILE_SYS_dir_file_goto_index(int16_t i16_index, uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders);

/*********************************************************************************************
* @brief Returns most important information of the element ( file or folder ) where the directory 
* access pointer is currently open. That includes: name, size, and type ( file or folder ).
* @param[out] pui8_file_folder_name with the name of the file or folder where the the directory 
* access pointer is currently pointing to.
* @param[in] ui16_max_name_len the size of the buffer used to store and get the name of the current 
* folder or file. It must be properly set to avoid writting file name characters outside the space
* reserved for them.
* @param[out] pui64_size size of the file where the the directory access pointer is currently 
* pointing to ( in case currently pointed element it is a file ).
* @param[out] pui8_flag_folder_files: flags with the kind of element the directory access pointer 
* is currently pointing to: file or folder.
* @return >=0 no errors occurred, <0 some error occurred.
* @note Tolaemon 19-02-2022
*********************************************************************************************/
int16_t FILE_SYS_get_current_info(uint8_t * pui8_file_folder_name, uint16_t ui16_max_name_len, uint64_t * pui64_size, uint8_t * pui8_flag_folder_files);

/*********************************************************************************************
* @brief Simply checks the state ( OK or NOT OK) of the element ( file or folder ) where the 
* directory access pointer is currently open. It may be usefull to check if the folder or unit
* is available.
* @return >=0 directory access pointer is OK, -1 directory access pointer is NOT OK ( not 
* initialized ? disk unit not mounted ? ... )
* @note Tolaemon 19-02-2022
*********************************************************************************************/
int16_t FILE_SYS_check_current_state();

/*********************************************************************************************
* @brief Writes the specified number of bytes of the received buffer in the currently open file. 
* @param[in] p_data_to_write: pointing to the buffer with the data to write into the currently open file.
* @param[in] ui32_n_bytes_to_write: number of bytes of the buffer to write into the currently open file.
* @param[out] pui32_n_written_bytes the number of bytes of the received buffer that could finally be written
* into the file.
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
* FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM,
* FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 19-02-2022
*********************************************************************************************/
int16_t FILE_SYS_file_write( const void * p_data_to_write, uint32_t ui32_n_bytes_to_write, uint32_t * pui32_n_written_bytes);

/*********************************************************************************************
* @brief Returns the requested number of bytes from the currently open file. 
* @param[in] ui32_n_bytes_to_read: the number of bytes to read from the currently open file.
* @param[out] p_read_data: buffer where the function stores data read from currently open file.
* @param[out] pui32_n_read_bytes: number of bytes of the requested number of bytes that the function could finally read.
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
* FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM,
* FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 19-02-2022
*********************************************************************************************/
int16_t FILE_SYS_file_read(const void * p_read_data, uint32_t ui32_n_bytes_to_read, uint32_t * pui32_n_read_bytes);

/*********************************************************************************************
* @brief Opens the file specified in pui8_path in the mode specified on ui8_mode
* @param[in] pui8_path name of the file to open.
* @param[in] ui8_mode: file access mode 
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
* FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM,
* FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 26-02-2022
*********************************************************************************************/
int16_t FILE_SYS_file_open(const uint8_t* pui8_path, uint8_t ui8_mode);

/*********************************************************************************************
* @brief Closes currently opened file ( if open )
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
* FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM,
* FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 26-02-2022
*********************************************************************************************/
int16_t FILE_SYS_file_close();

/*********************************************************************************************
* @brief Places the file access pointer pointing to the specified offset inside the currently
* open file.
* @param[in] ui64_ofs offset in bytes relative to the file begining where to place the file
* access pointer.
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
* FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM,
* FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 26-02-2022
*********************************************************************************************/
int16_t FILE_SYS_file_lseek(uint64_t ui64_ofs);

/*********************************************************************************************
* @brief Mounts (intializes) the file system control structures with the data of the specified
* logical drive.
* @param[in] pui8_path: logical drive to be mounted.
* @param[in] ui8_mode: 0:Do not mount (delayed mount), 1:Mount immediately
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
* FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM,
* FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 26-02-2022
*********************************************************************************************/
int16_t FILE_SYS_mount(const uint8_t* pui8_path, uint8_t ui8_mode);

/*********************************************************************************************
* @brief Places the directory access pointer to the specified directory 
* @param[in] pui8_path: with the name of the path to place the directory access read pointer on.
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
* FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM,
* FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 26-02-2022
*********************************************************************************************/
int16_t FILE_SYS_dir_open(const uint8_t* pui8_path);

/*********************************************************************************************
* @brief  Places the directory access pointer pointing to the first directory element.
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
* FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM,
* FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 26-02-2022
*********************************************************************************************/
int16_t FILE_SYS_dir_rewind();

/*********************************************************************************************
* @brief Returns if the file access pointer has reached the end of the file
* @return >=0 the end of the file has NOT been reached yet, <0 the end of the file has been reached
* @note Tolaemon 24-07-2022
*********************************************************************************************/
int16_t FILE_SYS_file_eof();

/*********************************************************************************************
* @brief Returns the offset of the file access pointer inside the current opened file .
* @return the offset of the file access pointer inside the current opened file .
* @note Tolaemon 26-02-2022
*********************************************************************************************/
uint64_t FILE_SYS_file_tell();

/*********************************************************************************************
* @brief Change current directory or current drive
* @param[in] pui8_path with the name of the directory to move to
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, 
* FR_INVALID_NAME,FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, 
* FR_NOT_ENABLED, FR_NO_FILESYSTEM, FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE,
*  FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 26-02-2022
*********************************************************************************************/
int16_t FILE_SYS_dir_ch(const uint8_t* pui8_path);

/*********************************************************************************************
* @brief Creates the specified directory with the specified name.
* @param[in] pui8_path with the name of the directory to create.
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH,
* FR_INVALID_NAME,FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE,
* FR_NOT_ENABLED, FR_NO_FILESYSTEM, FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE,
*  FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon 26-02-2022 
*********************************************************************************************/
int16_t FILE_SYS_dir_mk(const uint8_t* pui8_path);

/*********************************************************************************************
* @brief Removes the specified element in the path ( file or folder ).
* @param[in] pui8_path with the name of the element ( file or folder ) to remove.
* @return FRESULT (FR_OK = 0,FR_DISK_ERR, FR_INT_ERR,FR_NOT_READY, FR_NO_FILE, FR_NO_PATH,
* FR_INVALID_NAME,FR_DENIED,FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE,
* FR_NOT_ENABLED, FR_NO_FILESYSTEM, FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE,
*  FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER )
* @note Tolaemon  26-02-2022  Tolaemon
*********************************************************************************************/
int16_t FILE_SYS_dir_rm(const uint8_t* pui8_path);

/*********************************************************************************************
* @brief  Returns a string with the current path
* @param[in] ui16_max_fir_name_len: maximum number of characters that can be stored in the
* string used to return the current path.
* @param[out] pui8_cur_dir: string with the current path the FS is pointing to
* @return >=0 the command has been succesfully executed, <0 an error occurred while trying to 
* get the current path
* @note Tolaemon 02-10-2022 
*********************************************************************************************/
int16_t FILE_SYS_get_cur_dir(uint8_t * pui8_cur_dir,uint16_t ui16_max_fir_name_len);

/*********************************************************************************************
* @brief Returns the label and the serial number of the current mounted SD card unit.
* @param[in] pui8_label: pointer to the string where the label of current unit will be saved.
* @param[in] pui32_serial_n: pointer to an uint32 where the serial number of the unit
* will be stored.
* @param[out] pui8_label pointer to the string updated with the current mounted unit label name
* @param[out] pui32_serial_n: pointer to an uint32 that contains the current unit serial number.
* @return >=0 the command has been succesfully executed, <0 an error occurred while trying to get
* the current unit label
* @note Tolaemon 02-15-2023 
*********************************************************************************************/
int16_t FILE_SYS_get_label(uint8_t * pui8_label, uint32_t * pui32_serial_n);

/*********************************************************************************************
* @brief Returns the size in MegaBytes of the current mounted unit.
* @param[in] pui32_size_in_MB: variable where the size of the current mounted SD unit will
* be stored.
* @param[out] pui32_size_in_MB: pointer to the variable that stores the size of the unit in MBytes
* @return >=0 the command has been succesfully executed, <0 an error occurred while trying to get 
* the size of the current unit
* @note Tolaemon 02-15-2023 
*********************************************************************************************/
int16_t FILE_SYS_get_unit_size_MBs(uint32_t * pui32_size_in_MB);

#endif /* _FILE_SYS_H_ */