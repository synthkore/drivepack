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
#ifndef _AUX_FUNCS_H_
#define _AUX_FUNCS_H_

#include "sam.h"
#include "sys_defs.h"

#define AUX_FUNCS_F_P_MAX_STR_SIZE_64 64 // TBD-JBR 2022-04-10 mirar el uso que se hace de este tamaño de cadena, pq me parece que está mal
#define AUX_FUNCS_F_P_MAX_STR_SIZE_32 32 // TBD-JBR 2022-04-10 mirar el uso que se hace de este tamaño de cadena, pq me parece que está mal
#define AUX_FUNCS_F_P_MAX_STR_SIZE_16 16 // TBD-JBR 2022-04-10 mirar el uso que se hace de este tamaño de cadena, pq me parece que está mal

#define AUX_FUNCS_NO_ARRAY_SIZE     0 // to specifiy to not to consider the size of the array used to store the string

int32_t AUX_FUNCS_ceil(double value_to_round);
void AUX_FUNCS_set_substring(uint8_t * pui8_dest,uint8_t * pui8_src,int16_t i16_start_index,int16_t i16_end_index);
int16_t AUX_FUNCS_check_endstring(uint8_t * pui8_src, uint8_t * pui8_end, uint8_t ui8_remove);
void AUX_FUNCS_center_into_string(uint8_t * destination_string,uint8_t * string_to_center,int16_t start_index,int16_t end_index);
void AUX_FUNCS_lstrfill( uint8_t * pui8_dest, uint8_t  ui8_char, uint16_t ui16_dest_MAX_STR_ARRAY );
void AUX_FUNCS_lstrcpy( uint8_t * pui8_dest, uint8_t * pui8_source, uint16_t ui16_dest_MAX_STR_ARRAY );
int16_t AUX_FUNCS_lstrcmp( uint8_t * pui8_string_A, uint8_t * pui8_string_B, uint16_t ui16_MAX_STR_ARRAY );
void AUX_FUNCS_strcat(uint8_t * pui8_dest, uint8_t * pui8_source, uint16_t ui16_MAX_STR_ARRAY);
void AUX_FUNCS_strcat_char(uint8_t * pui8_dest, uint8_t ui8_source_char, uint16_t ui16_MAX_STR_ARRAY);
void AUX_FUNCS_remove_from_end_to_char(uint8_t * pui8_string, uint8_t ui8_char);
void AUX_FUNCS_strjoin(uint8_t * pui8_dest,uint8_t * pui8_first,uint8_t * pui8_second,uint16_t ui16_MAX_STR_ARRAY);
void  AUX_FUNCS_trim_begining_end( uint8_t * pui8_string_to_trim);
void AUX_FUNCS_trim_string_char(uint8_t * pui8_string_to_trim,uint8_t ui8_char_to_remove );
int16_t AUX_FUNCS_strlen(uint8_t * pui8_string);
int16_t AUX_FUNCS_lstrlen(uint8_t * pui8_string, uint16_t ui16_MAX_STR_ARRAY );
void AUX_FUNCS_reverse_str(uint8_t * pui8_string, uint16_t ui16_MAX_STR_ARRAY );
void AUX_FUNCS_itoa(int32_t ui32_num, uint8_t * pui8_string, int16_t i16_base, uint16_t ui16_MAX_STR_ARRAY );
void AUX_FUNCS_uitoa(uint32_t ui32_num, uint8_t * pui8_string, int16_t i16_base, uint16_t ui16_MAX_STR_ARRAY );
int16_t AUX_FUNCS_atoi(uint8_t * pui8_string, uint8_t ui8_base, uint16_t ui16_MAX_STR_ARRAY, int32_t * i32_num);
uint8_t AUX_FUNCS_random_xorshift8(void);
uint8_t AUX_FUNCS_character_to_low_case(uint8_t ui8_uppercase_character);
int16_t AUX_FUNCS_check_if_valid_number(uint8_t * pui8_str_to_check, uint16_t ui16_MAX_STR_ARRAY );
int16_t AUX_FUNCS_pad_begin_with_char(uint8_t * pui8_dest_str, uint8_t ui8_character, int16_t i16_str_length,  uint16_t ui16_MAX_STR_ARRAY );
int16_t AUX_FUNCS_pad_end_with_char(uint8_t * pui8_dest_str, uint8_t ui8_character, int16_t i16_str_length,  uint16_t ui16_MAX_STR_ARRAY );

#endif // _AUX_FUNCS_H_