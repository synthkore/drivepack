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

#include "file_sys.h"


FATFS fatfs_fsys;
FILINFO filinfo_finfo;// File information structure
FIL fil_fobj;// File object structure
DIR dir_obj;// Directory object structure


int16_t FILE_SYS_Init(){

	memset(&fatfs_fsys,0,sizeof(FATFS));
	memset(&filinfo_finfo,0,sizeof(FILINFO));
	memset(&fil_fobj,0,sizeof(FIL));
	memset(&dir_obj,0,sizeof(DIR));
	
}//FILE_SYS_Init


int16_t FILE_SYS_dir_file_count(uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders){
	int16_t i16_ret_val=0;
	FRESULT funcResult;
	int16_t ui16_AccFiles, ui16_AccDirs;


    // rewind directory cursor to first directory element
	if ((funcResult = f_readdir(&dir_obj, NULL)) != FR_OK){			
		// an error has occurred
		i16_ret_val=-1;			
	}//if

   	if (i16_ret_val!=-1){
		   
		// count directories and/or files that match to received parameters
		ui16_AccFiles = 0;
		ui16_AccDirs = 0;
		while (((funcResult = f_readdir(&dir_obj, &filinfo_finfo)) == FR_OK) && filinfo_finfo.fname[0]) {

			// check if folders must be counted, and count them if necessary
			if ( ((ui8_consider_file_folders&FILE_SYS_FLAG_FOLDER)!=0) && (filinfo_finfo.fattrib & AM_DIR)  ){
				
				// current entry corresponds to a folder: Finfo.fname
				
				// check if folders must be counted, and count them if affirmative
				if ((ui8_consider_file_folders&FILE_SYS_FLAG_FOLDER)!=0){
					// current entry corresponds to a directory: (uint8_t*)Finfo.fname			
					ui16_AccDirs++;
				}//if

			}//if
			
			// check if files must be counted, and count them if necessary
			if ( ((ui8_consider_file_folders&FILE_SYS_FLAG_FILE)!=0) && (filinfo_finfo.fattrib & AM_ARC) ){
							
				// current entry corresponds to a file: Finfo.fname

				// check if extension must be considered
				if (pui8_file_extension!=NULL){
					
					// a file extension has been received, so only the files with that extension will be considered	
					if (AUX_FUNCS_check_endstring((uint8_t*) filinfo_finfo.fname, pui8_file_extension, FALSE)>=0){
						ui16_AccFiles++;
					}//if
					
				}else{
					ui16_AccFiles++;
				}//if
						
			}//if
			
		}//while

		i16_ret_val = (int16_t)(ui16_AccDirs + ui16_AccFiles);
	
	}//	if (i16_ret_val!=-1)
				
	return i16_ret_val;
	
}//FILE_SYS_dir_file_count


int16_t FILE_SYS_dir_file_goto_next(uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders){
	int16_t i16_ret_val=-1;
	FRESULT funcResult;


	// move the file cursor  to the next element that matches specified conditions ( file extension, file or 
	// folder ... ). The while with the f_readdir is executed until a file that matches the specified conditions 
	// is found
	i16_ret_val=-1;
	while ( (i16_ret_val==-1) && ((funcResult = f_readdir(&dir_obj, &filinfo_finfo)) == FR_OK) && filinfo_finfo.fname[0]) {

		// check if folders must be considered, and count them if necessary
		if ( ((ui8_consider_file_folders&FILE_SYS_FLAG_FOLDER)!=0) && (filinfo_finfo.fattrib & AM_DIR)  ){
			i16_ret_val=1;
		}//if
	
		// check if files must be counted, and count them if necessary
		if ( ( (ui8_consider_file_folders&FILE_SYS_FLAG_FILE)!=0) && (filinfo_finfo.fattrib & AM_ARC) ){
					
			// check if extension must be considered
			if (pui8_file_extension!=NULL){
									
				// a file extension has been received, so only the files with that extension will be considered
				// check NOTE *1
				if (AUX_FUNCS_check_endstring((uint8_t*) filinfo_finfo.fname, pui8_file_extension, FALSE)>=0){							
					i16_ret_val=1;
				}//if
									
			}else{
									
				// no file extension has been received, so all the files are considered as valid
				i16_ret_val=1;
									
			}//if					
					
		}//if ( ( (consider_file_folders&FILE_SYS_FLAG_CONSIDER_FILES)
							
	}//while
			

	return i16_ret_val;
	
}//FILE_SYS_dir_file_goto_next


int16_t FILE_SYS_dir_file_goto_index(int16_t i16_index, uint8_t * pui8_file_extension, uint8_t ui8_consider_file_folders){
	int16_t i16_ret_val=0;
	FRESULT funcResult;
	int16_t i16_aux;

		
    // rewind directory cursor to first directory element
    if ((funcResult = f_readdir(&dir_obj, NULL)) != FR_OK){
	    // an error has occurred
	    i16_ret_val=-1;
    }//if

    if (i16_ret_val!=-1){	
			
		// move the file navigation index to the element "position" that matches specified conditions ( file extension, 
		// file or folder ... ). The while and the f_readdir is executed until a file that matches the specified conditions
		// at the specified "position" is found			
		i16_aux=0;// start counting from 1 because of the parent "\.." folder  // <<< TBD-JBR 2022-Revisar este '0' y este comentario...
		i16_ret_val=-1;
	    while ( (i16_ret_val==-1) && (i16_aux<=i16_index) && ((funcResult = f_readdir(&dir_obj, &filinfo_finfo)) == FR_OK) && filinfo_finfo.fname[0]) {
				
			// consider folders?: check if folders must be counted, and count them if affirmative
			if ( ((ui8_consider_file_folders&FILE_SYS_FLAG_FOLDER)!=0) && (filinfo_finfo.fattrib & AM_DIR) ){
						
				// check if index is placed at the specified position, and increase it if not
				if (i16_aux==i16_index){
					i16_ret_val=i16_index;
				}else{
					i16_aux++;
				}//if
						
			}//if
	
			// consider files?: check if files must be counted, and count them if affirmative
			if ( ( (ui8_consider_file_folders&FILE_SYS_FLAG_FILE)!=0) && (filinfo_finfo.fattrib & AM_ARC) ){
					
				// check if extension must be considered
				if (pui8_file_extension!=NULL){
									
					// a file extension has been received, so only the files with that extension will be considered
					// check NOTE *1
					if (AUX_FUNCS_check_endstring((uint8_t*) filinfo_finfo.fname, pui8_file_extension, FALSE)>=0){
										
						// check if index is placed at the specified position, and increase it if not
						if (i16_aux==i16_index){
							i16_ret_val=(int16_t)i16_index;
						}else{
							i16_aux++;
						}//if
										
					}//if
									
				}else{
									
					// no file extension has been received, so all the files are considered as valid
					// check if index is placed at the specified position, and increase it if not
					if (i16_aux==i16_index){
						i16_ret_val=(int16_t)i16_index;
					}else{
						i16_aux++;
					}//if
									
				}//if					
					
			}//if
					
		}//while
			
	}//if (i16_ret_val!=-1)

	return i16_ret_val;
	
}//FILE_SYS_dir_file_goto_index


int16_t FILE_SYS_dir_rewind(){
	int16_t i16_ret_val=0;
	FRESULT funcResult;
	
	
	funcResult = f_readdir(&dir_obj,NULL);
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_dir_rewind


int16_t FILE_SYS_get_current_info(uint8_t * pui8_file_folder_name, uint16_t ui16_max_name_len, uint64_t * pui64_size, uint8_t * pui8_flag_folder_files){
	int16_t i16_ret_val=-1;


	// before getting the type,name and size of the element ( file or folder ) check if Finfo is pointing to a valid file
	if (filinfo_finfo.fname!=NULL){
		
		// get the attributes of the element
		(*pui8_flag_folder_files)=0;			
		if (filinfo_finfo.fattrib & AM_DIR) {
			(*pui8_flag_folder_files) = (*pui8_flag_folder_files) | FILE_SYS_FLAG_FOLDER;
		}
		if (filinfo_finfo.fattrib & AM_ARC) {
			(*pui8_flag_folder_files) = (*pui8_flag_folder_files) | FILE_SYS_FLAG_FILE;
		}
		
		// get the size of the element		
		(*pui64_size)=(uint64_t)filinfo_finfo.fsize;
    
        // get the name of the file or folder
		AUX_FUNCS_lstrcpy(pui8_file_folder_name,(uint8_t*)filinfo_finfo.fname, ui16_max_name_len);
		
		i16_ret_val = 0;
		
	}// if (Finfo.fname!=NULL)

	return i16_ret_val;
	
}//FILE_SYS_get_current_info


int16_t FILE_SYS_check_current_state(){
	int16_t i16_ret_val=0;
	FRESULT funcResult;
	
	if ((funcResult = f_readdir(&dir_obj, &filinfo_finfo)) != FR_OK){
		i16_ret_val=-1;
	}//if

	return i16_ret_val;
	
}//FILE_SYS_check_current_state


int16_t FILE_SYS_file_write( const void * p_data_to_write, uint32_t ui32_n_bytes_to_write, uint32_t * pui32_n_written_bytes){
   int16_t i16_ret_val=0; 
   FRESULT funcResult;  

   
   funcResult = f_write(&fil_fobj, p_data_to_write, ui32_n_bytes_to_write, pui32_n_written_bytes);// Write it to the destination file
   if (funcResult!=FR_OK){
	   i16_ret_val=(int16_t)-funcResult;
   }else{
	   i16_ret_val= (int16_t)(*pui32_n_written_bytes);
   }//if

	return i16_ret_val;

}//FILE_SYS_file_write


int16_t FILE_SYS_file_read(const void * p_read_data, uint32_t ui32_n_bytes_to_read, uint32_t * pui32_n_read_bytes){
	int16_t i16_ret_val=0;
	FRESULT funcResult;


	funcResult = f_read(&fil_fobj, p_read_data, ui32_n_bytes_to_read, pui32_n_read_bytes);// Read File  
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val= (int16_t)(*pui32_n_read_bytes);
	}//if

	return i16_ret_val;

}//FILE_SYS_file_read


int16_t FILE_SYS_file_open(const uint8_t* pui8_path, uint8_t ui8_mode){
	int16_t i16_ret_val=0;
	FRESULT funcResult;


	funcResult = f_open(&fil_fobj, (TCHAR*)pui8_path, (BYTE)ui8_mode);
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_file_open


int16_t FILE_SYS_file_close(){
	int16_t i16_ret_val=0;
	FRESULT funcResult;


	funcResult = f_close(&fil_fobj);
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_file_close


int16_t FILE_SYS_file_lseek(uint64_t ui64_ofs){
	int16_t i16_ret_val=0;
	FRESULT funcResult;


	funcResult = f_lseek(&fil_fobj,(FSIZE_t)ui64_ofs);
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val=0;
	}//if

	return i16_ret_val;
	
}//FILE_SYS_file_lseek


int16_t FILE_SYS_mount(const uint8_t* pui8_path, uint8_t ui8_mode){
	int16_t i16_ret_val=0;
	int16_t i16_mount_retries=0;
	FRESULT funcResult;
	
	
	// try to mount the unit
	i16_mount_retries=0;
    while ( (i16_mount_retries<FILE_SYS_MOUNT_RETRIES) && (funcResult = f_mount(&fatfs_fsys, (const TCHAR*) pui8_path,(BYTE)ui8_mode))!=FR_OK ){
		// increase the retries counter and wait to try again
		i16_mount_retries++;
		TIMMING_BlockingDelay(TIMMING_TICKS_500_MS);
	}//while
	
	if ( (funcResult!=FR_OK)||(i16_mount_retries==FILE_SYS_MOUNT_RETRIES) ){
		// ERROR!
		i16_ret_val=(int16_t)-funcResult;
	}else{
		// OK!
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_mount


int16_t FILE_SYS_dir_open(const uint8_t* pui8_path){
	int16_t i16_ret_val=0;
	FRESULT funcResult;


	// try to open the received path
	funcResult = f_opendir(&dir_obj, (const TCHAR*) pui8_path);
	if (funcResult!=FR_OK){
		// ERROR!
		i16_ret_val=(int16_t)-funcResult;
	}else{
        // OK!
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_dir_open


int16_t FILE_SYS_file_eof(){

	return  ( (int16_t)((f_eof(&fil_fobj)==1)?-1:0) );

}//FILE_SYS_file_eof


uint64_t FILE_SYS_file_tell(){

    return (uint64_t)f_tell(&fil_fobj);

}//FILE_SYS_file_tell


int16_t FILE_SYS_dir_ch(const uint8_t* pui8_path){
	int16_t i16_ret_val=0;
	FRESULT funcResult;
	

	funcResult = f_chdir((const TCHAR*) pui8_path);
	if (funcResult!=FR_OK){
		// ERROR!
		i16_ret_val=(int16_t)-funcResult;
	}else{		
		// OK!
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_dir_ch


int16_t FILE_SYS_dir_mk(const uint8_t* pui8_path){
	int16_t i16_ret_val=0;
	FRESULT funcResult;
	

	funcResult = f_mkdir((const TCHAR*) pui8_path);
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_dir_mk


int16_t FILE_SYS_dir_rm(const uint8_t* pui8_path){
	int16_t i16_ret_val=0;
	FRESULT funcResult;
	

	funcResult = f_unlink((const TCHAR*) pui8_path);
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_dir_rm


int16_t FILE_SYS_get_cur_dir(uint8_t * pui8_cur_dir,uint16_t ui16_max_fir_name_len){
	int16_t i16_ret_val = 0;
	FRESULT funcResult;

	funcResult = f_getcwd(pui8_cur_dir,ui16_max_fir_name_len);
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_get_cur_dir


int16_t FILE_SYS_get_label(uint8_t * pui8_label, uint32_t * pui32_serial_n){
	int16_t i16_ret_val = 0;
	FRESULT funcResult;

	funcResult = f_getlabel((const TCHAR*)"",(TCHAR*)pui8_label,(DWORD*)pui32_serial_n);
	if (funcResult!=FR_OK){
		i16_ret_val=(int16_t)-funcResult;
	}else{
		i16_ret_val=0;
	}//if

	return i16_ret_val;

}//FILE_SYS_get_label


int16_t FILE_SYS_get_unit_size_MBs(uint32_t * pui32_size_in_MB){
	int16_t i16_ret_val = 0;
    uint16_t ui16_sectors_clus = 0;
	uint16_t ui16_sector_size = 0;
	uint32_t ui32_num_clusters = 0;
	uint64_t ui64_aux = 0;


	// get the data needed to calculate the unit size.
#if FF_MAX_SS == FF_MIN_SS
     ui16_sector_size = ((UINT)FF_MAX_SS); //// #define SS(fs)	((UINT)FF_MAX_SS)	/* Fixed sector size */
#else
     ui16_sector_size = fatfs_fsys.ssize;// #define SS(fs)	((fs)->ssize)	/* Variable sector size */
#endif	
	 ui16_sectors_clus = fatfs_fsys.csize; //Cluster size [sectors] 
     ui32_num_clusters = fatfs_fsys.n_fatent - 2; // Number of FAT entries (number of clusters + 2)
     
	 // calculate the number of bytes of the unit:
	 ui64_aux = ui32_num_clusters * ui16_sectors_clus * ui16_sector_size;
     ui64_aux = (ui64_aux>>20);// shit 10 bits to right to divide bye 1.048.576 to conver from bytes to mega-bytes.
    
     (*pui32_size_in_MB) = (uint32_t)ui64_aux;

	return i16_ret_val;

}//FILE_SYS_get_unit_size_MBs