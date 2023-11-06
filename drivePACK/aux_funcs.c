// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       12/04/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "aux_funcs.h"

int32_t AUX_FUNCS_ceil(double value_to_round){
	int32_t return_value=0;
	
	
	return_value=(uint32_t)(value_to_round);
	return_value=return_value+1;

	return return_value;
	
}//AUX_FUNCS_ceil


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


uint8_t AUX_FUNCS_random_xorshift8(void){
	static uint8_t y8 = 1;

	y8 ^= (y8 << 7);
	y8 ^= (y8 >> 5);
	return y8 ^= (y8 << 3);
	
}//AUX_FUNCS_random_xorshift8


uint8_t AUX_FUNCS_character_to_low_case(uint8_t ui8_uppercase_character){
	
	
	// if received character corresponds to a CAPS ( ASCII code from
	// 65 = A and 90 = Z ) then 
	if ( (ui8_uppercase_character>=65) && (ui8_uppercase_character<=90) ){
		ui8_uppercase_character = ui8_uppercase_character + 32;
	}

	return ui8_uppercase_character;

}//AUX_FUNCS_character_to_low_case


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
