// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       12/04/2015                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "aux_funcs.h"



// Synthkore  12-10-2014  Tolaemon
// Function: int32_t AUX_FUNCS_ceil(double value_to_round){
// Rounds up the received double into the nearest upper whole number and returns it into
// a long integer value.
// Receives:
//   value_to_round: with the double value we want to round to the nearest whole number.
// Returns:
int32_t AUX_FUNCS_ceil(double value_to_round){
	int32_t return_value=0;
	
	
	return_value=(uint32_t)(value_to_round);
	return_value=return_value+1;

	return return_value;
	
}//AUX_FUNCS_ceil



// Synthkore  12-10-2014  Tolaemon
// Function: void AUX_FUNCS_set_substring(uint8_t * destination_string,uint8_t * string_to_print,int16_t start_index,int16_t end_index)
// Prints the string_to_print inside the destination_string at the specified position start_index
// Receives:
//		pui8_dest: the string where the smaller string is going to be printed.
//		pui8_src: the string that is going to be placed inside the destination string.
//		i16_start_index: the position where the string is going to start.
//		i16_end_index: the last position where the string can be copied.
// Returns:
void AUX_FUNCS_set_substring(uint8_t * pui8_dest,uint8_t * pui8_src,int16_t i16_start_index,int16_t i16_end_index){
	
/* TBD-2022-03-03 Rehacer sin utilizar tamaños fijos constantes AUX_FUNCS_F_P_MAX_STR_LEN	
	if (i16_end_index>i16_start_index){
		
		sh_strlen=AUX_FUNCS_strlen(pui8_src, AUX_FUNCS_F_P_MAX_STR_LEN);// TBD-JBR 2022-03-04 This AUX_FUNCS_F_P_MAX_STR_LEN is wrong!
		if ((i16_start_index+sh_strlen-1)<i16_end_index){
			i16_end_index=i16_start_index+sh_strlen-1;
		}//if

		sh_aux2=0;
		for (sh_aux=i16_start_index;sh_aux<=i16_end_index;sh_aux++){
			pui8_dest[sh_aux]=pui8_src[sh_aux2];
			sh_aux2++;
		}//for
		
	}//if
*/

}//AUX_FUNCS_set_substring



// Synthkore  23-11-2014  Tolaemon
// Prints the string_to_print inside the destination_string centered between the space specified with start and end indexes.
// Receives:
//	 pui8_dest: the string where the string_to_center is going to be printed in.
//	 pui8_src_to_center: the string that is going to be placed centered inside the destination_string.
//	 i16_start_index: the start index of the space where the string is going to be printed.
//	 i16_end_index: the end index of the space where the string is going to be printed.
// Returns:
void AUX_FUNCS_center_into_string(uint8_t * pui8_dest, uint8_t * pui8_src_to_center,int16_t i16_start_index,int16_t i16_end_index){
	int16_t i16_aux,i16_aux2,i16_aux3,i16_aux4;
	int16_t i16_strlen=0;

	
	if (i16_end_index>i16_start_index){
		
		i16_strlen=AUX_FUNCS_strlen(pui8_src_to_center);
		if ((i16_start_index+i16_strlen-1)<i16_end_index){

			// get the position where the string_to_center will start to be copied
			i16_aux2=(i16_end_index-(i16_start_index+i16_strlen))/2;
			// get the position where the string_to_center will finish to be copied
			i16_aux3=i16_aux2+i16_strlen;
			i16_aux4=0;
			for ( i16_aux=i16_aux2;i16_aux<i16_aux3;i16_aux++){
				pui8_dest[i16_aux]=pui8_src_to_center[i16_aux4];
				i16_aux4++;
			}//for
			
		}else{
			
			// the string_to_center is too big and only the part that fits between
			// the specified start and end index will be copied.
			i16_aux2=0;
			for ( i16_aux=i16_start_index;i16_aux<i16_end_index;i16_aux++){
				pui8_dest[i16_aux]=pui8_src_to_center[i16_aux2];
				i16_aux2++;
			}//for
			
		}//if
		
	}//if

}//AUX_FUNCS_center_into_string



// Synthkore  27-11-2014  Tolaemon
// Function: uint8_t AUX_FUNCS_check_endstring(uint8_t * source_string,uint8_t * end_string,uint8_t remove)
// Checks if the specified end_string is contained at the end of source_string and removes it if
// if it is found amd remove is TRUE.
// Receives:
//	 pui8_src: the string where the end_string is going to be searched and removed from.
//	 pui8_end: the string that is going to be searched and removed from the end of the source_string
//   ui8_remove: when TRUE the end_string is removed from the source_string if it is contained there. If
//   FALSE then the routine only checks if the end_string is contianed into the source_string.
// Returns:
// 	 >=0: if end_string has been found and removed from the end of the source_string
//   <0 if end_string was not found or could not be removed from the end of the source_string
int16_t AUX_FUNCS_check_endstring(uint8_t * pui8_src, uint8_t * pui8_end, uint8_t ui8_remove){
	int16_t i16_strlen1;
	int16_t i16_strlen2;
	int16_t i16_ret_val=-1;
    uint8_t ui8_aux = 0;


	i16_strlen1=AUX_FUNCS_strlen(pui8_end);
	i16_strlen2=AUX_FUNCS_strlen(pui8_src);
	if ((i16_strlen1>0)&&(i16_strlen2>0)&&(i16_strlen1<=i16_strlen2)){
		
		do{
			i16_strlen1--;
			i16_strlen2--;
			
			// if processed character of the file name corresponds to a UPPPER case ( ASCII code from 
			// 65 = A and 90 = Z ) then convert it to LOWER case to compare it with received extension 
			// ( that should be in lower case ).
			ui8_aux = pui8_src[i16_strlen2];
			if ( (ui8_aux>=65) && (ui8_aux<=90) ){
				ui8_aux = ui8_aux + 32;
			}
			
		}while ((ui8_aux==pui8_end[i16_strlen1])&&(i16_strlen1>0));
		
		// check if the end_string has been found at the end of the source_string
		if (i16_strlen1==0){
			
			if (ui8_remove==TRUE){
				// the end_string is removed from the end of the source_string
				pui8_src[i16_strlen2]='\0';
			}//if
			i16_ret_val=0;
			
		}//if
		
	}//if

	return i16_ret_val;

}//AUX_FUNCS_check_endstring



// Synthkore  30-05-2013  Tolaemon
// Fills the received string with he specified character. The parameter ui16_MAX_STR_ARRAY is used
// to take care of how many chars are filled into the destination array, and to avoid filling
// beyond the space of the destination string.
// Receives:
//   pui8_dest: pointer to the string that will be filled with the specified character
//   ui8_char: the character used to fill the received destination string
//   ui16_MAX_STR_ARRAY: the size of the array of the destination string, the last allowed 
//  character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
//  should contain a '\0'.
// Returns:
// By reference:
//   pui8_dest: pointer to a string with ui8_char copied ui16_dest_MAX_STR_ARRAY times
// By value:
void AUX_FUNCS_lstrfill( uint8_t * pui8_dest, uint8_t ui8_char, uint16_t ui16_dest_MAX_STR_ARRAY ){
	int16_t i16_aux;

	// copy the received character ui16_dest_MAX_STR_ARRAY times in the destination string
	i16_aux=0;
	while (i16_aux<(ui16_dest_MAX_STR_ARRAY-1)){
		pui8_dest[i16_aux]=ui8_char;
		i16_aux++;
	}//while
	pui8_dest[i16_aux]='\0';

}//AUX_FUNCS_lstrfill



// Synthkore  30-05-2013  Tolaemon
// Custom version of the string copy command: the difference with the standard strcpy instruction
// is that this function uses the ui16_MAX_STR_ARRAY parameter to take care of how many chars are copied into 
// the destination array, and avoid copying beyond the space reserved for the destination string.
// Receives: 
//   pui8_dest: pointer to the string where the source will be copied to
//   pui8_source: pointer with the string to copy into the destination string.
//   ui16_MAX_STR_ARRAY: the size of the array used to store the destination string, the last
//  allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
//  should contain a '\0'.
// Returns:
// By reference:
//   pui8_dest: pointer to a string with a copy the source string.
// By value:
void AUX_FUNCS_lstrcpy( uint8_t * pui8_dest, uint8_t * pui8_source, uint16_t ui16_dest_MAX_STR_ARRAY ){
	int16_t i16_aux;

	// copy the auxiliar trimmed string into the final string
	i16_aux=0;
	while ( (pui8_source[i16_aux]!='\0') && (i16_aux<(ui16_dest_MAX_STR_ARRAY-1)) ){
		pui8_dest[i16_aux]=pui8_source[i16_aux];
		i16_aux++;
	}//while
	pui8_dest[i16_aux]='\0';

}//AUX_FUNCS_lstrcpy



// Synthkore  21-05-2016  Tolaemon
// Custom version of the string compare function that uses ui16_MAX_STR_ARRAY parameter to check how many chars 
// are compared, and avoid array overrun copying beyond the space reserved for the string
// Receives:
//   pui8_string_A: first string to compare 
//   pui8_string_B: second string to compare 
//   ui16_MAX_STR_ARRAY: the size of the shortest array used to store the strings, the last
//  allowed character in the shortest string can be stored in (ui16_MAX_STR_ARRAY-2) and in that 
//  case (ui16_MAX_STR_ARRAY-1) should contain a '\0'. 
// Returns:
// By reference:
// By value:
//   <0	the first character that does not match has a lower value in source than in dest
//    0	the contents of both strings are equal
//   >0	the first character that does not match has a greater value in source than in dest
int16_t AUX_FUNCS_lstrcmp( uint8_t * pui8_string_A, uint8_t * pui8_string_B, uint16_t ui16_MAX_STR_ARRAY ){
	int16_t ret_val=0;
	int16_t i16_aux;
	
	
	i16_aux=0;
	while ( (i16_aux<(ui16_MAX_STR_ARRAY-1)) && (pui8_string_B[i16_aux]==pui8_string_A[i16_aux]) && (pui8_string_B[i16_aux]!='\0') && (pui8_string_A[i16_aux]!='\0') ){
		i16_aux++;
	}//while
	
	if (pui8_string_B[i16_aux]==pui8_string_A[i16_aux]){
		ret_val=0;
	}else{
		ret_val=(int16_t)pui8_string_B[i16_aux]-(int16_t)pui8_string_A[i16_aux];
	}//if
	
	return ret_val;

}//AUX_FUNCS_lstrcmp



// Synthkore  26-11-2016  Tolaemon
// Appends a copy of the source string to the end of the destination string. The terminating null character in destination is 
// overwritten by the first character of source string, and a null-character is included at the end of the new string formed 
// by the concatenation of both in destination.
// Receives:
//   pui8_dest: the destination string where the source string will be appended
//   pui8_source: the string to append at the end of the destination string
//   ui16_MAX_STR_ARRAY: the size of the array used to store the destination string, the last
//  allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
//  should contain a '\0'.
// Returns:
//		destination_string: with the concatenation of received destination_string and source_string
void AUX_FUNCS_strcat(uint8_t * pui8_dest, uint8_t * pui8_source, uint16_t ui16_MAX_STR_ARRAY){
	int16_t i16_aux,i16_aux2;


	// first get the index of the '\0' character indicating the end of the destination_string string
	i16_aux=0;
	while ( (i16_aux<(ui16_MAX_STR_ARRAY-1)) && (pui8_dest[i16_aux]!='\0') ){
		i16_aux++;
	}//while
	
	// append the content of the source string behind the destination string
	i16_aux2=0;
	while ( (i16_aux<(ui16_MAX_STR_ARRAY-1)) && (pui8_source[i16_aux2]!='\0') ){
		pui8_dest[i16_aux]=pui8_source[i16_aux2];
		i16_aux++;
		i16_aux2++;
	}//while	

	// set the end of string char behind the last entered char
	pui8_dest[i16_aux]='\0';

}//AUX_FUNCS_strcat



// Synthkore  26-11-2016  Tolaemon
// Appends the specified character to the end of the destination string. The terminating null character in 
// destination is overwritten by the source character, and a null-character is included at the end of the 
// new string formed by the concatenation of both in destination.
// Receives:
//   pui8_dest: the destination string where the received char will be appended
//   ui8_char: the char to append at the end of the destination string
//   ui16_MAX_STR_ARRAY: the size of the array used to store the destination string, the last
//  allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
//  should contain a '\0'.
// Returns:
//		destination_string: with the concatenation of received destination_string and source_string
void AUX_FUNCS_strcat_char(uint8_t * pui8_dest, uint8_t ui8_source_char, uint16_t ui16_MAX_STR_ARRAY){
	int16_t i16_aux;


	// first get the index of the '\0' character indicating the end of the destination_string string
	i16_aux=0;
	while ( (i16_aux<(ui16_MAX_STR_ARRAY-1)) && (pui8_dest[i16_aux]!='\0') ){
		i16_aux++;
	}//while
		
	// if the index of the '\0' is under the max size of the array then append the received char
	// to the end of the destination string
	if (i16_aux<(ui16_MAX_STR_ARRAY-1)){
		pui8_dest[i16_aux]=ui8_source_char;
		i16_aux++;
	}//if

	// set the end of string char behind the last entered char
	pui8_dest[i16_aux]='\0';
	
}//AUX_FUNCS_strcat_char



// Synthkore  01-04-2022  Tolaemon
// Removes all the characters between the end of the string and the first occurrence of the specified
// character, including the specified character.
// Receives:
//   pui8_string: with the string to remove the characters from the end to the specified character.
//   ui16_MAX_STR_ARRAY: the size of the array used to store the string, as is the array size the 
// string allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
// should contain a '\0'.
//   ui8_char: the char to remove all the char
// Returns:
//   pui8_string: the string without the characters from the end to the specifed char. If the specified 
// char is not found then all characters are removed.
void AUX_FUNCS_remove_from_end_to_char(uint8_t * pui8_string, uint8_t ui8_char){
	int16_t i16_aux;


	// start removing chars ( writting '\0' ) until the specified character is found
	i16_aux=AUX_FUNCS_strlen(pui8_string);
	while ((i16_aux>0)&&(pui8_string[i16_aux-1]!=ui8_char)){
		pui8_string[i16_aux-1]='\0';
		i16_aux--;
	}
	// if the specified character has been found then it is also removed
	if (i16_aux>0) { pui8_string[i16_aux-1]='\0'; }
	
}//AUX_FUNCS_remove_from_end_to_char



// Synthkore  19-03-2016  Tolaemon
// Copies the content of the two received strings on the destination string 
// Receives:
//   pui8_dest: the destination string where the two strings will be copied
//   pui8_first: the string that will be copied on first position
//   pui8_second: the string that will be copied on second position
//   ui16_MAX_STR_ARRAY: the size of the array used to store the destination string, the last
//  allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
//  should contain a '\0'.
// Returns:
void AUX_FUNCS_strjoin(uint8_t * pui8_dest,uint8_t * pui8_first,uint8_t * pui8_second,uint16_t ui16_MAX_STR_ARRAY){
	int16_t i16_aux,i16_aux2;
	
	
	i16_aux=0;
	
	// copy first string into destination
	i16_aux2=0;		
	while ( (i16_aux<(ui16_MAX_STR_ARRAY-1)) && (pui8_first[i16_aux2]!='\0') ){
		pui8_dest[i16_aux]=pui8_first[i16_aux2];
		i16_aux++;
		i16_aux2++;
	}//while

    // copy second string into destination
	i16_aux2=0;		
	while ( (i16_aux<(ui16_MAX_STR_ARRAY-1)) && (pui8_second[i16_aux2]!='\0') ){
		pui8_dest[i16_aux]=pui8_second[i16_aux2];
		i16_aux++;
		i16_aux2++;
	}//while

}//AUX_FUNCS_strjoin



// Synthkore  29-05-2013  Tolaemon
// Removes the blank spaces and tab characters at the beginning and the end of the string
// Receives:
//	  pui8_string_to_trim: with the string we want to remove the blank chars or spaces
// Returns:
// By reference:
//    pui8_string_to_trim: the received string without the blank spaces and tab characters at beginning and the end.
// By value:
//   TRUE: if the string could be trimmed
//   FALSE: if was not possible to trim the string
void  AUX_FUNCS_trim_begining_end( uint8_t * pui8_string_to_trim){
	int ui_strlen;
	int ui_aux;


	ui_strlen = AUX_FUNCS_strlen(pui8_string_to_trim );
	if (ui_strlen>0){

		// remove the blank chars ( ' ' and '\t' ) from the end of the string
		ui_aux=ui_strlen;
		while ( (ui_aux>0) && ((pui8_string_to_trim[ui_aux-1]==' ') || (pui8_string_to_trim[ui_aux-1]=='\t')) ){
			ui_aux--;
			pui8_string_to_trim[ui_aux]='\0';
		}//while

		// remove the blank chars ( ' ' and '\t' ) from the beginning of the string
		while ( (pui8_string_to_trim[0]!='\0') && ((pui8_string_to_trim[ui_aux-1]==' ') || (pui8_string_to_trim[ui_aux-1]=='\t')) ){
			
			// if first char is a blank char then move all the chars one position to the beginning
			ui_aux=0;
			while (pui8_string_to_trim[ui_aux]!='\0'){
				pui8_string_to_trim[ui_aux]=pui8_string_to_trim[ui_aux+1];
				ui_aux++;
			}//while

		}//while

	}//if
	
}//AUX_FUNCS_trim_begining_end



// Synthkore  15-11-2016  Tolaemon
//   Procedure that removes the specified char from the beginning and the end of the
// received string.
// Receives:
//	  pui8_string_to_trim: with the string we want to remove the specified char
//	  ui8_char_to_remove: the char we want to remove from the beginning and the end
// Returns:
//	  by reference:
//		pui8_string_to_trim: pointing to the trimmed string y value:
void AUX_FUNCS_trim_string_char(uint8_t * pui8_string_to_trim,uint8_t ui8_char_to_remove ){
	int ui_strlen;
	int ui_aux;


	ui_strlen = AUX_FUNCS_strlen(pui8_string_to_trim);
	if (ui_strlen>0){

		// remove the specified char from the end of the string
		ui_aux=ui_strlen;
		while ((ui_aux>0)&&(pui8_string_to_trim[ui_aux-1]==ui8_char_to_remove)){
			ui_aux--;
			pui8_string_to_trim[ui_aux]='\0';
		}//while

		// remove the specified char from the begining of the string
		while ((pui8_string_to_trim[0]!='\0')&&(pui8_string_to_trim[0]==ui8_char_to_remove)){
			
			ui_aux=0;
			while (pui8_string_to_trim[ui_aux]!='\0'){
				pui8_string_to_trim[ui_aux]=pui8_string_to_trim[ui_aux+1];
				ui_aux++;
			}//while

		}//while

	}//if
	
}//AUX_FUNCS_trim_string_char



// Synthkore  15-11-2016  Tolaemon
// Returns the length of the received string. It also checks that the length of the received
// string is not beyond the MAX_LEN max allowed value. In that case it returns an error code.
// Receives:
//   pui8_string: with the string whose length in chars we want to get
// Returns:
//    >=0 with the length of the string
//    -1 the received string cannot be processed
int16_t AUX_FUNCS_strlen(uint8_t * pui8_string ){
	int16_t i16_ret_value=0;


	if (pui8_string==NULL){
		
		i16_ret_value=-1;
		
	}else{
		
		// search the end of string '\0' symbol
		while (pui8_string[i16_ret_value]!='\0'){
			i16_ret_value++;
		}//while
		
	}//if
	
	return i16_ret_value;
	
}//AUX_FUNCS_strlen


// Synthkore  15-11-2016  Tolaemon
// Returns the length of the received string. It also checks that the length of the received
// string is not beyond the MAX_LEN max allowed value. In that case it returns an error code.
// Receives:
//   pui8_string: with the string whose length in chars we want to get
//   ui16_MAX_STR_ARRAY: the size of the array that contains the string whose length we want to calculate. 
//  The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
//  should contain a '\0'.
// Returns:
//    >=0 with the length of the string
//    -1 the received string is greater than ush_MAX_LEN and cannot be processed
int16_t AUX_FUNCS_lstrlen(uint8_t * pui8_string, uint16_t ui16_MAX_STR_ARRAY ){
	int16_t i16_ret_value=0;


	if (pui8_string==NULL){
		
		i16_ret_value=-1;
	
	}else{
		
		// search the end of string '\0' symbol
		while ( (i16_ret_value<ui16_MAX_STR_ARRAY) && (pui8_string[i16_ret_value]!='\0') ){
			i16_ret_value++;
		}//while
	
		// if the end of the string has not been reached and the index is beyond the max allowed 
		// string length then return the corresponding error code
		if (i16_ret_value>=ui16_MAX_STR_ARRAY){
			i16_ret_value=-1;
		}//if
	
	}//if
	
	return i16_ret_value;
	
}//AUX_FUNCS_lstrlen



// Function:AUX_FUNCS_reverse_str(char str[], int length)
// Inverts the order of the characters in a string: first character becomes last, 
// last becomes first...
// Receives:
//   pui8_string: with the string to reverse
//   ui16_MAX_STR_ARRAY: the size of the array that contains the string to reverse. The last allowed 
// character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
// should contain a '\0'.
//  By reference:
//   pui8_string: the reversed string
void AUX_FUNCS_reverse_str(uint8_t * pui8_string, uint16_t ui16_MAX_STR_ARRAY ){
	uint8_t ui8_aux;
	int16_t i16_start = 0;
	int16_t i16_end = 0;
	
	
	i16_end = AUX_FUNCS_lstrlen(pui8_string, ui16_MAX_STR_ARRAY );
	if ( i16_end>0){
		
		while (i16_start < i16_end){
			// swap characters
			ui8_aux = pui8_string[i16_start];
			pui8_string[i16_start] = pui8_string[i16_end];
			pui8_string[i16_end] = ui8_aux;
			// move swapping indexes
			i16_start++;
			i16_end--;
		}//while
		
	}//if ( i16_end>0)
	
}//AUX_FUNCS_reverse_str



// Based on: https://www.geeksforgeeks.org/implement-itoa/
// Function: void AUX_FUNCS_itoa(int32_t i32_num, uint8_t * pui8_string, int16_t i16_base, uint16_t ui16_MAX_STR_ARRAY )
// Converts the received integer into a string.
// Receives:
//   ui32_num: the integer to convert to string
//   i16_base: base used to convert the integer to string
//   ui16_MAX_STR_ARRAY: the size of the array that contains the conversion of rceived value to
// string. The last allowed digit can be stored in (ui16_MAX_STR_ARRAY-2) and in that case 
// (ui16_MAX_STR_ARRAY-1) should contain a '\0'. To use that function the size of the destination
// array must be always specified
// Returns:
//  By reference:
//    pui8_string: text representation of the received value, with the result of converting 
//   the received integer to a string
//  By value:
void AUX_FUNCS_itoa(int32_t i32_num, uint8_t * pui8_string, int16_t i16_base, uint16_t ui16_MAX_STR_ARRAY ){
	int16_t i = 0;
	int16_t i16_start = 0;
	int16_t i16_end = 0;
	uint8_t ui8_aux = 0;
	uint8_t isNegative = FALSE;
	
	
	if ( (pui8_string!=NULL)&&(ui16_MAX_STR_ARRAY>1) ){
	
		// Handle 0 , otherwise empty string is printed for 0 
		if (i32_num == 0){
		
			i = 0;
			pui8_string[i] = '0';
			i++;
			pui8_string[i] = '\0';

		}else{

			 pui8_string[0]='\0';
	
			// In standard itoa(), negative numbers are handled only with
			// base 10. Otherwise numbers are considered unsigned.
			if (i32_num < 0 && i16_base == 10){
				isNegative = TRUE;
				i32_num = -i32_num;
			}
	
			// Process individual digits and add them into the string in reverse order
			i = 0;
			while ( (i32_num != 0) && (i< (ui16_MAX_STR_ARRAY-1)) ) {
				int16_t rem = i32_num % i16_base;
				pui8_string[i] = (rem > 9)? (rem-10) + 'a' : rem + '0';
				i++;
				i32_num = i32_num/i16_base;
			}//while
	        
			// If number is negative, append '-'
			if ( isNegative && (i< (ui16_MAX_STR_ARRAY-1)) ) pui8_string[i++] = '-';
			
			if (i<(ui16_MAX_STR_ARRAY-1)){
											
				pui8_string[i] = '\0'; // Append string terminator
							
				// Reverse the string
				i16_end = i-1;
				i16_start = 0;
				while (i16_start < i16_end){
					// swap characters
					ui8_aux = pui8_string[i16_start];
					pui8_string[i16_start] = pui8_string[i16_end];
					pui8_string[i16_end] = ui8_aux;
					// move swapping indexes
					i16_start++;
					i16_end--;
				}//while
				
			}//if

		}//if (ui32_num == 0)
			
	}//if  ((pui8_string!=NULL)&&(ui16_MAX_STR_ARRAY>1) )
	
}//AUX_FUNCS_itoa



// Based on: https://www.geeksforgeeks.org/implement-itoa/
// Function: void AUX_FUNCS_uitoa(uint32_t ui32_num, uint8_t * pui8_string, int16_t i16_base, uint16_t ui16_MAX_STR_ARRAY )
// Converts the received UNSIGNED integer into a string.
// Receives:
//   ui32_num: the unsigned integer to convert to string
//   i16_base: base used to convert the unsigned integer to string
//   ui16_MAX_STR_ARRAY: the size of the array that contains the conversion of rceived value to
// string. The last allowed digit can be stored in (ui16_MAX_STR_ARRAY-2) and in that case
// (ui16_MAX_STR_ARRAY-1) should contain a '\0'. To use that function the size of the destination
// array must be always specified
// Returns:
//  By reference:
//    pui8_string: text representation of the received value, with the result of converting
//   the received unsigned integer to a string
//  By value:
void AUX_FUNCS_uitoa(uint32_t ui32_num, uint8_t * pui8_string, int16_t i16_base, uint16_t ui16_MAX_STR_ARRAY ){
	int16_t i = 0;
	int16_t i16_start = 0;
	int16_t i16_end = 0;
	uint8_t ui8_aux = 0;
	
	
	if ( (pui8_string!=NULL)&&(ui16_MAX_STR_ARRAY>1) ){
		
		// Handle 0 , otherwise empty string is printed for 0
		if (ui32_num == 0){
			
			i = 0;
			pui8_string[i] = '0';
			i++;
			pui8_string[i] = '\0';

		}else{

			pui8_string[0]='\0';
			
			// Process individual digits and add them into the string in reverse order
			i = 0;
			while ( (ui32_num != 0) && (i< (ui16_MAX_STR_ARRAY-1)) ) {
				int16_t rem = ui32_num % i16_base;
				pui8_string[i] = (rem > 9)? (rem-10) + 'a' : rem + '0';
				i++;
				ui32_num = ui32_num/i16_base;
			}//while
			
			
			if (i<(ui16_MAX_STR_ARRAY-1)){
				
				pui8_string[i] = '\0'; // Append string terminator
				
				// Reverse the string
				i16_end = i-1;
				i16_start = 0;
				while (i16_start < i16_end){
					// swap characters
					ui8_aux = pui8_string[i16_start];
					pui8_string[i16_start] = pui8_string[i16_end];
					pui8_string[i16_end] = ui8_aux;
					// move swapping indexes
					i16_start++;
					i16_end--;
				}//while
				
			}//if

		}//if (ui32_num == 0)
		
	}//if  ((pui8_string!=NULL)&&(ui16_MAX_STR_ARRAY>1) )
	
}//AUX_FUNCS_uitoa



// Function: void AUX_FUNCS_atoi(uint8_t * string, int16_t sh_base, uint16_t ush_MAX_LEN, int32_t * sh_num)
// Converts the received string to an integer
// Receives:
//   pui8_string: the string to convert to an integer
//   ui8_base: base used to convert the string to integer
//   ui16_MAX_STR_ARRAY: the size of the array that contains the text representation of the value to 
// convert to number. The last allowed digit can be stored in (ui16_MAX_STR_ARRAY-2) and in that case
// (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
// Returns:
//  By reference:
//    i32_num: with the result of converting the received string to a integer
//  By value:
//    >=0 if the string could be converted
//    -1 if the received string could not be converted to an integer
int16_t AUX_FUNCS_atoi(uint8_t * pui8_string, uint8_t ui8_base, uint16_t ui16_MAX_STR_ARRAY, int32_t * i32_num){
	int32_t  i32_value;
	int16_t  i16_ret_val = 0;
	uint16_t ui16_length = 0;
	uint8_t  ui8_digit = 0;
	int8_t   i8_sign=1;
	uint16_t ui16_i;
	uint8_t  ui8_aux;

    
	// check that received string with value representation has characters to convert and is not empty
	ui16_length = AUX_FUNCS_lstrlen(pui8_string, ui16_MAX_STR_ARRAY );
	if (ui16_length<=0){
		
		i16_ret_val = -1;
		
	}else{
				
		// remove blank spaces from the beginning and the end of the string and then check the sign symbol
		AUX_FUNCS_trim_begining_end(pui8_string);
		if (pui8_string[0]=='-') i8_sign = -1; else i8_sign = 1;
		
		// process each digit of the received string
		ui16_length = AUX_FUNCS_lstrlen(pui8_string, ui16_MAX_STR_ARRAY);		
		i32_value=0;	
		ui16_i=0;
		while ( (ui16_i<ui16_length) && (i16_ret_val>=0) ){
			
			ui8_aux = pui8_string[ui16_i];
			if ( (ui8_aux>=0x30) && (ui8_aux<=0x39) ){
				// digit is between '0' and '9'
				ui8_digit = pui8_string[ui16_i]-0x30; 
			}else if ( (ui8_aux>=0x41) && (ui8_aux<=0x46) ){
				// digit is between 'A' and 'F'
				ui8_digit = 10 + pui8_string[ui16_i]-0x41;
			}else if ( (ui8_aux>=0x61) && (ui8_aux<=0x66) ){
				// digit is between 'a' and 'f'	
				ui8_digit = 10 + pui8_string[ui16_i]-0x61;			
			}else{
				// invalid digit character received
			    i16_ret_val = -1;
			}//if
			
			// add the current iteration digit value to the total value
			if ( (i16_ret_val>=0) && (ui8_digit<ui8_base) ){
				i32_value=(i32_value*ui8_base)+ui8_digit;
			}else{
				i16_ret_val = -1;
			}//if
			
			// process next digit			
			ui16_i++;
			
		}//while

		if (i16_ret_val>=0){
			(*i32_num) = i32_value * i8_sign ;
		}else{
			(*i32_num) = 0;	
		}
		
	}//if (ui16_length
	
	return i16_ret_val;
	
}//AUX_FUNCS_atoi



// Synthkore  02-05-2016  Tolaemon
// Function: uint8_t AUX_FUNCS_random_xorshift8(void)
// 8 bit random generator function, obtained from: www.arklyffe.com/main/2010/08/29/xorshift-pseudorandom-number-generator/:
// "...
// In 2003, George Marsaglia published a pseudorandom number generator based on repeated shift and XOR operations, a relative
// of the linear feedback shift register generators. With y seeded with any non-zero starting value. The generator will never
// produce zero, which is something to be careful of, but with the right values for a, b, and c, will cycle through all 
// non-zero values. If only a subset of the bits are used, a generator with the full period results where zeros do occur, 
// though not as frequently as the other values in the cycle: if you take 6 bits from an 8 bit generator, you will get each 
// non-zero 6-bit number 4 times per cycle, and only 3 zeros. Alternatively, you can simply subtract 1 from the output, and take
// the usual approaches to obtaining the desired range without unacceptable bias. The published generators I've found all use 
// 32 bit values or greater, but sometimes something simpler and smaller is needed. Here is the C code for the generators I have
// been testing. Note that they have been verified to be full cycle and to give reasonably random-looking results, but little more
//..."
// Receives:
// Returns:
// By reference:
// By value:
//    returns values from 1 to 255 inclusive, period is 255
uint8_t AUX_FUNCS_random_xorshift8(void){
	static uint8_t y8 = 1;

	y8 ^= (y8 << 7);
	y8 ^= (y8 >> 5);
	return y8 ^= (y8 << 3);
	
}//AUX_FUNCS_random_xorshift8



// Function: AUX_FUNCS_character_to_low_case(uint8_t upper_character)
// Converts the received character in uppercase to lowercase if possible.
// Receives:
//   ui8_uppercase_character: with the character to convert to uppercase
// Returns:
//   the character in uppercase if it has been possible to convert to 
// upper case
uint8_t AUX_FUNCS_character_to_low_case(uint8_t ui8_uppercase_character){
	
	
	// if received character corresponds to a CAPS ( ASCII code from
	// 65 = A and 90 = Z ) then 
	if ( (ui8_uppercase_character>=65) && (ui8_uppercase_character<=90) ){
		ui8_uppercase_character = ui8_uppercase_character + 32;
	}

	return ui8_uppercase_character;

}//AUX_FUNCS_character_to_low_case



// Synthkore  09-04-2021 Tolaemon
// Simply checks if the received string contains a valid number or it does contain 
// invalid numerical data.
// Receives:
//	  pui8_str_to_check: the string we want to check if contains a valid number
//    ui16_MAX_STR_ARRAY: the size of the array that contains the string to reverse. The last allowed
// character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
// should contain a '\0'.
// Returns:
//	By value:
//		 1 if the string contains a valid number 
//		-1 if does not contain a valid number
// Note: 
//  trim the string before calling this function.
int16_t AUX_FUNCS_check_if_valid_number(uint8_t * pui8_str_to_check, uint16_t ui16_MAX_STR_ARRAY ){
	int16_t i16_ret_val=1;
	uint16_t i=0;
	int16_t i16_dec_poin_cnt=0;
	int16_t i16_length=0;


	i16_length=AUX_FUNCS_lstrlen(pui8_str_to_check,ui16_MAX_STR_ARRAY);	
	if (i16_length<=0){

		// there are not characters on the string
		i16_ret_val=-1;

	}else{

		// checks if there is sign symbol at the beginning
		if ((pui8_str_to_check[0]=='-')||(pui8_str_to_check[0]=='+')){
			i=1;//start checking behind the symbol
		}else{
			i=0;
		}//if
		
		while ( (i<i16_length) && (i16_ret_val==1) ){

			if (((pui8_str_to_check[i]>='0')&&(pui8_str_to_check[i]<='9'))||((pui8_str_to_check[i]=='.')&&(i16_dec_poin_cnt<=1))){
				i++;
				if (pui8_str_to_check[i]=='.'){i16_dec_poin_cnt++;}
			}else{
				i16_ret_val=-1;
			}//if

		}//while

	}//if

	return i16_ret_val;

}//check_if_valid_number



// Synthkore  17-04-2021 Tolaemon
//    Adds the specified character at the beginning of the received string
// as many times as needed to get a string of the specified length. So sets
// the length of the received string to the specified length, filling empty
// spaces at the beginning with received character.
// Receives:
//	  pui8_dest_str: the string we want to add specified character at the beginning
//	  ui8_character: the character to add at the beginning of the string
//    i16_str_length: the final desired length for the received string
//    ui16_MAX_STR_ARRAY: the size of the array that contains the string to pad with the specified
// character. The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case 
// (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
// Returns:
//  By reference:
//    pui8_dest_str: with the padded string
//	By value:
//    >=0 if the string has been succesfully padded with the specified character
//    -1  an error has occurred while padding the string
int16_t AUX_FUNCS_pad_begin_with_char(uint8_t * pui8_dest_str, uint8_t ui8_character, int16_t i16_str_length,  uint16_t ui16_MAX_STR_ARRAY ){
	int16_t ret_val=0;
	int16_t i16_dest_str_length = 0;
	int16_t i16_aux = 0;
	int16_t i16_aux2 = 0;


	i16_dest_str_length=AUX_FUNCS_lstrlen(pui8_dest_str, ui16_MAX_STR_ARRAY);
	if  ( (i16_dest_str_length<0) || (i16_str_length>(ui16_MAX_STR_ARRAY-1)) || (i16_dest_str_length>=i16_str_length) ){

		// it is not possible to pad the string
		ret_val=-1;

	}else{

		// move characters of received string to the right to leave space for
		// the padding characters		
		i16_aux = i16_dest_str_length;
		i16_aux2 = i16_str_length;
		while (i16_aux>=0){
			pui8_dest_str[i16_aux2]=pui8_dest_str[i16_aux];
			i16_aux--;
			i16_aux2--;
		}//while

		// add padding characters until completing specified string length
		while (i16_aux2>=0){
			pui8_dest_str[i16_aux2]=ui8_character;
			i16_aux2--;
		}//while
	
	}//if

	return ret_val;

}//AUX_FUNCS_pad_begin_with_char



// Synthkore  17-04-2021 Tolaemon
//    Adds the specified character at the end of the received string
// as many times as needed to get a string of the specified length. So sets
// the length of the received string to the specified length, filling empty
// spaces at the end with received character.
// Receives:
//	  pui8_dest_str: the string we want to add specified character at the beginning
//	  ui8_character: the character to add at the beginning of the string
//    i16_str_length: the final desired length for the received string
//    ui16_MAX_STR_ARRAY: the size of the array that contains the string to pad with the specified
// character. The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case
// (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
// Returns:
//  By reference:
//    pui8_dest_str: with the padded string
//	By value:
//    >=0 if the string has been succesfully padded with the specified character
//    -1  an error has occurred while padding the string
int16_t AUX_FUNCS_pad_end_with_char(uint8_t * pui8_dest_str, uint8_t ui8_character, int16_t i16_str_length,  uint16_t ui16_MAX_STR_ARRAY ){
	int16_t ret_val=0;
	int16_t i16_dest_str_length = 0;
	int16_t i16_aux = 0;


	i16_dest_str_length=AUX_FUNCS_lstrlen(pui8_dest_str, ui16_MAX_STR_ARRAY);
	if  ( (i16_dest_str_length<0) || (i16_str_length>(ui16_MAX_STR_ARRAY-1)) || (i16_dest_str_length>=i16_str_length) ){

		// it is not possible to pad the string
		ret_val=-1;

	}else{
		
		// add padding characters until completing specified string length
		i16_aux = i16_dest_str_length;
   		while (i16_aux<i16_str_length){
			pui8_dest_str[i16_aux]=ui8_character;
			i16_aux++;
		}//while
        pui8_dest_str[i16_aux]='\0';

	}//if

	return ret_val;

}//AUX_FUNCS_pad_end_with_char



// Synthkore  17-04-2021 Tolaemon
//    Replaces in the received string all the occurrences of the specified old character 
// with the specified new character.
// Receives:
//	  pui8_str: the string where the specified old characters shall be replaced with the
//  specified new character.
//	  ui8_old_char: the character to remove and replace with the new received string
//	  ui8_new_char: the character to write in all occurrences of the old character
//    ui16_MAX_STR_ARRAY: the size of the array that contains the string 
//  character. The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in 
//  that case (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
// Returns:
//  By reference:
//    pui8_str: the received string after having replaced all the occurrences of the specified
//  old character with the new character.
// By value:
//   >=0 if the string has been succesfully padded with the specified character
//   -1  an error has occurred while padding the string
int16_t AUX_FUNCS_replace_char(uint8_t * pui8_str, uint8_t ui8_old_char, uint8_t ui8_new_char, uint16_t ui16_MAX_STR_ARRAY ){
	int16_t ret_val=0;
	int16_t i16_aux = 0;


    i16_aux = 0;
    while ( (pui8_str[i16_aux]!='\0') && (i16_aux<(ui16_MAX_STR_ARRAY-1))) {
		// if the character corresponds to the received ui8_old_char then it is replaced by the ui8_new_char
		if (pui8_str[i16_aux]==ui8_old_char){
			pui8_str[i16_aux]=ui8_new_char;
		}
		i16_aux++;
	}//while
    pui8_str[i16_aux]='\0';

	return ret_val;

}//AUX_FUNCS_replace_char



// Synthkore  28-04-2023 Tolaemon
//   Converts the received chars with the hexadecimal ASCII representation of an 8 bits value
//  to the corresponding int8_t value. So it receives a 2 chars array and converts it to an 
//  uint8_ value. 
// I.e:
//   [0]0 [1]A -> 10
//   [0]2 [1]B -> 43
// Receives:
//    pui8_str: pointer to the 2 uint8_t elements with the chars wit the uint8 hexadecimal 
// representation that must be converted.
// Returns:
//  By reference:
//    pui8_byte_value: pointing to the 8 bits value with the result of converting the 2 received
//  chars with the hexadecimal representation of an 8 bits value.
// By value:
//   >=0 with the conversion 
//   <0  if something failed and was not possible to do the conversion
int16_t AUX_FUNCS_hexchars_to_ui8(uint8_t * pui8_str, uint8_t * pui8_byte_value ){
	int16_t ret_val=0;
	
	
	return ret_val;

}//AUX_FUNCS_hexchars_to_ui8