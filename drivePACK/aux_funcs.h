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

/*********************************************************************************************
* @brief Rounds up the received double into the nearest upper whole number and returns it into
* a long integer value.
* @param[in] value_to_round With the double value we want to round to the following nearest
* whole number.
* @return the following nearest whole number to the received value.
* @note Tolaemon 12-10-2014
*********************************************************************************************/
int32_t AUX_FUNCS_ceil(double value_to_round);

/*********************************************************************************************
* @brief Prints the string_to_print inside the destination_string at the specified position
* start_index.
* @param[out] pui8_dest The string where the smaller string is going to be printed.
* @param[in] pui8_src The string that is going to be placed inside the destination string.
* @param[in] i16_start_index The position where the string is going to start.
* @param[in] i16_end_index The last position where the string can be copied.
* @note Tolaemon 12-10-2014
*********************************************************************************************/
void AUX_FUNCS_set_substring(uint8_t * pui8_dest,uint8_t * pui8_src,int16_t i16_start_index,int16_t i16_end_index);

/*********************************************************************************************
* @brief Prints the string_to_print inside the destination_string centered between the space
* specified with start and end indexes.
* @param[out] pui8_dest the string where the string_to_center is going to be printed in.
* @param[in] pui8_src_to_center the string that is going to be placed centered inside the
* destination_string.
* @param[in] i16_start_index the start index of the space where the string is going to be printed.
* @param[in] i16_end_index the end index of the space where the string is going to be printed.
* @note Tolaemon 23-11-2014
*********************************************************************************************/
void AUX_FUNCS_center_into_string(uint8_t * destination_string,uint8_t * string_to_center,int16_t start_index,int16_t end_index);

/*********************************************************************************************
* @brief Checks if the specified end_string is contained at the end of source_string and removes
* if it is found and remove is TRUE.
* @param[in] pui8_src: the string where the end_string is going to be searched and removed from.
* @param[in] pui8_end: the string that is going to be searched and removed from the end of the
* source_string
* @param[in] ui8_remove: when TRUE the end_string is removed from the source_string if it is
* contained there. If FALSE then the routine only checks if the end_string is contianed into the
* source_string.
* @return =0: if end_string has been found and removed from the end of the source_string
* <0 if end_string was not found or could not be removed from the end of the source_string
* @note Tolaemon 27-11-2014
*********************************************************************************************/
int16_t AUX_FUNCS_check_endstring(uint8_t * pui8_src, uint8_t * pui8_end, uint8_t ui8_remove);

/*********************************************************************************************
* @brief Fills the received string with he specified character. The parameter ui16_MAX_STR_ARRAY
* is used to take care of how many chars are filled into the destination array, and to avoid
* filling beyond the space of the destination string.
* @param[out] pui8_dest pointer to the string that will be filled with the specified character
* @param[in] ui8_char the character used to fill the received destination string
* @param[in] ui16_MAX_STR_ARRAY the size of the array of the destination string, the last allowed
* @param[in] character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
* should contain a '\0'.
* @note Tolaemon 30-05-2013
*********************************************************************************************/
void AUX_FUNCS_lstrfill( uint8_t * pui8_dest, uint8_t  ui8_char, uint16_t ui16_dest_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief Custom version of the string copy command: the difference with the standard strcpy
* instruction is that this function uses the ui16_MAX_STR_ARRAY parameter to take care of how
* many chars are copied into the destination array, and avoid copying beyond the space reserved
* for the destination string.
* @param[out] pui8_dest pointer to the string where the source will be copied to
* @param[in]  pui8_source pointer with the string to copy into the destination string.
* @param[in] ui16_MAX_STR_ARRAY: the size of the array used to store the destination string, the last
* allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
* should contain a '\0'.
* @note Tolaemon 30-05-2013
*********************************************************************************************/
void AUX_FUNCS_lstrcpy( uint8_t * pui8_dest, uint8_t * pui8_source, uint16_t ui16_dest_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief Custom version of the string compare function that uses ui16_MAX_STR_ARRAY parameter to
* check how many chars are compared, and avoid array overrun copying beyond the space reserved
for the string
* @param[in] pui8_string_A first string to compare
* @param[in] pui8_string_B second string to compare
* @param[in] ui16_MAX_STR_ARRAY the size of the shortest array used to store the strings, the
* last allowed character in the shortest string can be stored in (ui16_MAX_STR_ARRAY-2) and in
* that case (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
* @return <0	the first character that does not match has a lower value in source than in dest,
* 0	the contents of both strings are equal, >0 the first character that does not match has a
* greater value in source than in dest
* @note Tolaemon 21-05-2016
*********************************************************************************************/
int16_t AUX_FUNCS_lstrcmp( uint8_t * pui8_string_A, uint8_t * pui8_string_B, uint16_t ui16_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief Appends a copy of the source string to the end of the destination string. The
* terminating null character in destination is overwritten by the first character of source
* string, and a null-character is included at the end of the new string formed by the
* concatenation of both in destination.
* @param[out] pui8_dest the destination string where the source string will be appended
* @param[in] pui8_dest pui8_source the string to append at the end of the destination string
* @param[in] ui16_MAX_STR_ARRAY the size of the array used to store the destination string, the last
* allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
* should contain a '\0'.
* @note Tolaemon 26-11-2016
*********************************************************************************************/
void AUX_FUNCS_strcat(uint8_t * pui8_dest, uint8_t * pui8_source, uint16_t ui16_MAX_STR_ARRAY);

/*********************************************************************************************
* @brief Appends the specified character to the end of the destination string. The terminating
* null character in destination is overwritten by the source character, and a null-character is
* included at the end of the new string formed by the concatenation of both in destination.
* @param[out] pui8_dest the destination string where the received char will be appended
* @param[in] ui8_char the char to append at the end of the destination string
* @param[in] ui16_MAX_STR_ARRAY the size of the array used to store the destination string, the last
* allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
* should contain a '\0'.
* @note Synthkore  26-11-2016  Tolaemon
*********************************************************************************************/
void AUX_FUNCS_strcat_char(uint8_t * pui8_dest, uint8_t ui8_source_char, uint16_t ui16_MAX_STR_ARRAY);

/*********************************************************************************************
* @brief Removes all the characters between the end of the string and the first occurrence of
* the specified character, including the specified character.
* @param[out|in]  pui8_string with the string to remove the characters from the end to the
* specified character. Is the string without the characters from the end to the specifed char.
* If the specified char is not found then all characters are removed.
* @param[in] ui16_MAX_STR_ARRAY: the size of the array used to store the string, as is the array
* size the string allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case
* (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
* @param[in] ui8_char the char to remove from the received string
* @note Synthkore  01-04-2022  Tolaemon
*********************************************************************************************/
void AUX_FUNCS_remove_from_end_to_char(uint8_t * pui8_string, uint8_t ui8_char);

/*********************************************************************************************
* Copies the content of the two received strings on the destination string
* Receives:
* @param[out] pui8_dest the destination string where the two strings will be copied
* @param[in] pui8_first the string that will be copied on first position
* @param[in] pui8_second the string that will be copied on second position
* @param[in] ui16_MAX_STR_ARRAY the size of the array used to store the destination string, the last
* allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)
* should contain a '\0'.
* @note Tolaemon 19-03-2016
*********************************************************************************************/
void AUX_FUNCS_strjoin(uint8_t * pui8_dest,uint8_t * pui8_first,uint8_t * pui8_second,uint16_t ui16_MAX_STR_ARRAY);

/*********************************************************************************************
* @brief Removes the blank spaces and tab characters at the beginning and the end of the string
* Receives:
* @param[in|out] pui8_string_to_trim with the string we want to remove the blank chars or spaces
* It retunrs the received string without the blank spaces and tab characters at beginning and the end.
* @note Tolaemon 29-05-2013
*********************************************************************************************/
void  AUX_FUNCS_trim_begining_end( uint8_t * pui8_string_to_trim);

/*********************************************************************************************
* @brief procedure that removes the specified char from the beginning and the end of the
* received string.
* @param[in|out] pui8_string_to_trim: with the string we want to remove the specified char
* @param[in] ui8_char_to_remove the char we want to remove from the beginning and the end
* @note Tolaemon 15-11-2016
*********************************************************************************************/
void AUX_FUNCS_trim_string_char(uint8_t * pui8_string_to_trim,uint8_t ui8_char_to_remove );

/*********************************************************************************************
* @brief Returns the length of the received string. It also checks that the length of the
* received string is not beyond the MAX_LEN max allowed value. In that case it returns an error
* code.
* @param[in] pui8_string: with the string whose length in chars we want to get
* @return >=0 with the length of the string, -1 the received string cannot be processed
* @note Tolaemon  15-11-2016
*********************************************************************************************/
int16_t AUX_FUNCS_strlen(uint8_t * pui8_string);

/*********************************************************************************************
* @brief Returns the length of the received string. It also checks that the length of the received
* string is not beyond the MAX_LEN max allowed value. In that case it returns an error code.
* @param[in] pui8_string with the string whose length in chars we want to get
* @param[in] ui16_MAX_STR_ARRAY the size of the array that contains the string whose length we
* want to calculate.
* The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case
* (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
* @return >=0 with the length of the string, -1 the received string is greater than ush_MAX_LEN
* and cannot be processed
* @note Synthkore  15-11-2016  Tolaemon
*********************************************************************************************/
int16_t AUX_FUNCS_lstrlen(uint8_t * pui8_string, uint16_t ui16_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief Inverts the order of the characters in a string: first character becomes last,
* last becomes first...
* @param[in|out] pui8_string with the string to reverse. And the string revesed once finished.
* @param[in] ui16_MAX_STR_ARRAY the size of the array that contains the string to reverse. The
* last allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case
* (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
*********************************************************************************************/
void AUX_FUNCS_reverse_str(uint8_t * pui8_string, uint16_t ui16_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief  Converts the received integer into a string. Based on: https://www.geeksforgeeks.org/implement-itoa/
* @param[in] ui32_num: the integer to convert to string
* @param[out] pui8_string: text representation of the received value, with the result of
* converting the received integer to a string
* @param[in] i16_base base used to convert the integer to string
* @param[in] ui16_MAX_STR_ARRAY the size of the array that contains the conversion of received
* value to string. The last allowed digit can be stored in (ui16_MAX_STR_ARRAY-2) and in that case
* (ui16_MAX_STR_ARRAY-1) should contain a '\0'. To use that function the size of the destination
* array must be always specified
*********************************************************************************************/
void AUX_FUNCS_itoa(int32_t ui32_num, uint8_t * pui8_string, int16_t i16_base, uint16_t ui16_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief  Converts the received UNSIGNED integer into a string. Based on: https://www.geeksforgeeks.org/implement-itoa/
* @param[in] ui32_num the unsigned integer to convert to string
* @param[out] pui8_string text representation of the received value, with the result of
* converting the received integer to a string
* @param[in] i16_base base used to convert the unsigned integer to string
* @param[in] ui16_MAX_STR_ARRAY the size of the array that contains the conversion of rceived
* value to string. The last allowed digit can be stored in (ui16_MAX_STR_ARRAY-2) and in that
* case (ui16_MAX_STR_ARRAY-1) should contain a '\0'. To use that function the size of the
* destination array must be always specified.
*********************************************************************************************/
void AUX_FUNCS_uitoa(uint32_t ui32_num, uint8_t * pui8_string, int16_t i16_base, uint16_t ui16_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief Converts the received string to an integer
* @param[in] pui8_string the string to convert to an integer
* @param[in] ui8_base base used to convert the string to integer
* @param[in] ui16_MAX_STR_ARRAY: the size of the array that contains the text representation of
* the value to convert to number. The last allowed digit can be stored in (ui16_MAX_STR_ARRAY-2)
* and in that case(ui16_MAX_STR_ARRAY-1) should contain a '\0'.
* @param[out] i32_num: with the result of converting the received string to a integer
* @return >=0 if the string could be converted, -1 if the received string could not be converted
* to an integer.
*********************************************************************************************/
int16_t AUX_FUNCS_atoi(uint8_t * pui8_string, uint8_t ui8_base, uint16_t ui16_MAX_STR_ARRAY, int32_t * i32_num);

/*********************************************************************************************
* @brief8 bit random generator function, obtained from: www.arklyffe.com/main/2010/08/29/xorshift-pseudorandom-number-generator/:
* "...
* In 2003, George Marsaglia published a pseudorandom number generator based on repeated shift
* and XOR operations, a relative of the linear feedback shift register generators. With y seeded
* with any non-zero starting value. The generator will never produce zero, which is something to
* be careful of, but with the right values for a, b, and c, will cycle through all non-zero values.
* If only a subset of the bits are used, a generator with the full period results where zeros do
* occur, though not as frequently as the other values in the cycle: if you take 6 bits from an
* 8 bit generator, you will get each non-zero 6-bit number 4 times per cycle, and only 3 zeros.
* Alternatively, you can simply subtract 1 from the output, and take the usual approaches to
* obtaining the desired range without unacceptable bias. The published generators I've found all
* use 32 bit values or greater, but sometimes something simpler and smaller is needed. Here is
* the C code for the generators I have been testing. Note that they have been verified to be full
* cycle and to give reasonably random-looking results, but little more ..."
* @return "random" value from 1 to 255 inclusive, period is 255
* @note Synthkore  02-05-2016  Tolaemon
*********************************************************************************************/
uint8_t AUX_FUNCS_random_xorshift8(void);

/*********************************************************************************************
* @brief Converts the received character in uppercase to lowercase if possible.
* @param[in] ui8_uppercase_character with the character to convert to uppercase
* @return the character in uppercase if it has been possible to convert to upper case
*********************************************************************************************/
uint8_t AUX_FUNCS_character_to_low_case(uint8_t ui8_uppercase_character);

/*********************************************************************************************
* @brief Adds the specified character at the beginning of the received string as many times
* as needed to get a string of the specified length. So sets the length of the received string
* to the specified length, filling empty spaces at the beginning with received character.
* @param[out] pui8_dest_str the string we want to add specified character at the beginning. If
* no errors, it is returned padded.
* @param[in] ui8_character the character to add at the beginning of the string
* @param[in] i16_str_length: the final desired length for the received string
* @param[in] ui16_MAX_STR_ARRAY: the size of the array that contains the string to pad with
* the specified character. The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2)
* and in that case  (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
* @return >=0 if the string has been succesfully padded with the specified character, -1 an
* error has occurred while padding the string
* @note Tolaemon  17-04-2021
*********************************************************************************************/
int16_t AUX_FUNCS_check_if_valid_number(uint8_t * pui8_str_to_check, uint16_t ui16_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief Adds the specified character at the beginning of the received string as many times
* as needed to get a string of the specified length. So sets the length of the received string
* to the specified length, filling empty spaces at the beginning with received character.
* @param[out] pui8_dest_str the string we want to add specified character at the beginning. If
* no errors, it is returned padded.
* @param[in] ui8_character the character to add at the beginning of the string
* @param[in] i16_str_length: the final desired length for the received string
* @param[in] ui16_MAX_STR_ARRAY: the size of the array that contains the string to pad with
* the specified character. The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2)
* and in that case  (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
* @return >=0 if the string has been succesfully padded with the specified character, -1 an
* error has occurred while padding the string
* @note Tolaemon  17-04-2021
*********************************************************************************************/
int16_t AUX_FUNCS_pad_begin_with_char(uint8_t * pui8_dest_str, uint8_t ui8_character, int16_t i16_str_length,  uint16_t ui16_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief Adds the specified character at the end of the received string as many times as
* needed to get a string of the specified length. So sets the length of the received string to
* the specified length filling empty spaces at the end with received character.
* @param[out] pui8_dest_str: the string we want to add specified character at the beginning. If
* no errors, it is returned padded.
* @param[in] ui8_character: the character to add at the beginning of the string
* @param[in] i16_str_length: the final desired length for the received string
* @param[in] ui16_MAX_STR_ARRAY: the size of the array that contains the string to pad with the
* specified character. The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and
* in that case (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
* @return >=0 if the string has been succesfully padded with the specified character, -1 an
* error has occurred while padding the string
* @note Tolaemon  17-04-2021
*********************************************************************************************/
int16_t AUX_FUNCS_pad_end_with_char(uint8_t * pui8_dest_str, uint8_t ui8_character, int16_t i16_str_length,  uint16_t ui16_MAX_STR_ARRAY );

/*********************************************************************************************
* @brief Replaces in the received string all the occurrences of the specified old character
* with the specified new character.
* @param[in|out] pui8_str: the string where the specified old characters shall be replaced with the
* specified new character. If no errors occur it will contain the received string with the specified
* character replaced
* @param[in] ui8_old_char: the character to remove and replace with the new received string
* @param[in] ui8_new_char: the character to write in all occurrences of the old character
* @param[in] ui16_MAX_STR_ARRAY: the size of the array that contains the string
* character. The last allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in
* that case (ui16_MAX_STR_ARRAY-1) should contain a '\0'.
* @return >=0 if the string has been succesfully padded with the specified character -1
* an error has occurred while padding the string
* @note Tolaemon 17-04-2021
*********************************************************************************************/
int16_t AUX_FUNCS_replace_char(uint8_t * pui8_str, uint8_t ui8_old_char, uint8_t ui8_new_char, uint16_t ui16_MAX_STR_ARRAY );

#endif // _AUX_FUNCS_H_