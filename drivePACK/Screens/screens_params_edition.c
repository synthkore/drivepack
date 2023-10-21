// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       10/11/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "screens_params_edition.h"

// references to external structures
extern struct ref_screens_control screens_control;

struct ref_screens_params_edition screens_params_edition;

// TBD - JBR 2022-01-26 Borrar!! Solo para test de la pantalla de edición de parametros
#define TEST_BORRAME_VALOR1      0
#define TEST_BORRAME_VALOR1_STR  "UNO"
#define TEST_BORRAME_VALOR2      1
#define TEST_BORRAME_VALOR2_STR  "DOS"
#define TEST_BORRAME_VALOR3      2
#define TEST_BORRAME_VALOR3_STR  "TRES"
#define TEST_BORRAME_VALOR4      3
#define TEST_BORRAME_VALOR4_STR  "CUAT"
#define TEST_BORRAME_VALOR5      4
#define TEST_BORRAME_VALOR5_STR  "CINC"

int16_t test_borrame[]={0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0}; 
// FIN TBD - JBR 2022-01-26 Borrar!! Solo para test de la pantalla de edición de parametros

// parameters literals in flash
// parameters edition default literals in flash
const uint8_t SCREEN_PARAM_EDIT_DEFAULT_TITLE[] = "Default title";
const uint8_t SCREEN_PARAM_EDIT_DEFAULT_TXT[]   = "Invalid option";
const uint8_t SCREEN_PARAM_EDIT_DEFAULT_HLP1[]  = "No defined help1";
const uint8_t SCREEN_PARAM_EDIT_DEFAULT_HLP2[]  = "No defined help2";

// parameters edition screen literals in flash
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_TITLE[]   = "Parameter A          ";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_01_TXT[]  = "Par1";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_01_HLP1[] = "Este parametro sirve p";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_01_HLP2[] = "ara ajustar el canal 1";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_02_TXT[]  = "Par2";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_02_HLP1[] = "El canal dos se puede ";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_02_HLP2[] = "configurar con este pa";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_03_TXT[]  = "Par3";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_03_HLP1[] = "Tres parametros hay en";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_03_HLP2[] = "tres valores tres tres";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_04_TXT[]  = "Par4";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_04_HLP1[] = "Ete esl parametro nume";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_04_HLP2[] = "ro cuatro que va tras ";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_05_TXT[]  = "Par5";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_05_HLP1[] = "Decia uno que cinco po";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_05_HLP2[] = "r el kulo te la hinco ";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_06_TXT[]  = "Par6";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_06_HLP1[] = "Seis mil seisciento eu";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_06_HLP2[] = "ros cuetsa un viaje a ";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_07_TXT[]  = "Par7";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_07_HLP1[] = "Siete fantasticos eran";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_07_HLP2[] = "los caballleros admira";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_08_TXT[]  = "Par8";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_08_HLP1[] = "Ocho rima con bizcocho";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_08_HLP2[] = "y tambien con xoxo    ";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_09_TXT[]  = "Par9";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_09_HLP1[] = "Nueve es un numero pre";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_09_HLP2[] = "vio a los 2 digitos   ";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_10_TXT[]  = "Par10";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_10_HLP1[] = "Diez decadas son 100 a";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_10_HLP2[] = "nios que es mucho";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_11_TXT[]  = "Par11";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_11_HLP1[] = "A los 11 yo estaba en";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_11_HLP2[] = "6o de EGB";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_12_TXT[]  = "Par12";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_12_HLP1[] = "12 meses tiene el anio";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_12_HLP2[] = "creo que eran 12 apost";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_13_TXT[]  = "Par13";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_13_HLP1[] = "Es un numero magico ro";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_13_HLP2[] = "deado de supersticion";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_14_TXT[]  = "Par14";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_14_HLP1[] = "14 es el que va antes";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_14_HLP2[] = "del 15               ";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_15_TXT[]  = "Par15";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_15_HLP1[] = "El duo dinamico canta";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_15_HLP2[] = "ba 15 anios tiene mi";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_16_TXT[]  = "Par16";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_16_HLP1[] = "16 es ya la adolescen";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_16_HLP2[] = "cia avanzadada       ";

const uint8_t SCREEN_PARAM_EDIT_A_PARAM_17_TXT[]  = "Par17";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_17_HLP1[] = "Diez y siete son 10 +";
const uint8_t SCREEN_PARAM_EDIT_A_PARAM_17_HLP2[] = "7 ( siete)           ";



void SCREENS_params_edition_clear(){
	int16_t i16_aux;


	// clear the structure used to store the current parameters edition screen
	for (i16_aux=0;i16_aux<SCREENS_PARAM_EDIT_MAX_PARAMS;i16_aux++){
		memset(&screens_params_edition.parameter[i16_aux],0,sizeof(t_param_edit_option));		
		screens_params_edition.parameter[i16_aux].pui8_description = SCREEN_PARAM_EDIT_DEFAULT_TXT;
		screens_params_edition.parameter[i16_aux].pui8_help[0] = SCREEN_PARAM_EDIT_DEFAULT_HLP1;
		screens_params_edition.parameter[i16_aux].pui8_help[1] = SCREEN_PARAM_EDIT_DEFAULT_HLP2;
		
	}//for
	screens_params_edition.pui8_title = SCREEN_PARAM_EDIT_DEFAULT_TITLE;
	screens_params_edition.i16_num_parameters=0;
	screens_params_edition.i16_params_cols_numb=0;
	screens_params_edition.i16_params_cols_width=0;
	screens_params_edition.i16_params_rows_numb=0;
	screens_params_edition.i16_selected_index=0;
	screens_params_edition.i16_top_index=0;
	screens_params_edition.i16_bottom_index=0;

	screens_params_edition.ev_process_cur_function = NULL;
	screens_params_edition.load_cur_function = NULL;
	screens_params_edition.show_cur_function = NULL;

}//SCREENS_params_edition_clear



void SCREENS_params_edition_init(){
	
	SCREENS_params_edition_clear();
	
}//SCREENS_params_edition_init



int8_t SCREENS_params_edition_load(int16_t i16_params_edition_id){
	int8_t i8_ret_val = 1;


	switch (i16_params_edition_id){
		
		case SCREEN_PARAM_EDIT_ID_EDIT_A:
			SCREENS_params_edition_clear();
			// initialize the strings used to print the title of the current screen
			screens_params_edition.pui8_title = SCREEN_PARAM_EDIT_A_PARAM_TITLE;
			// initialize the pointers to the functions that manage specific parameters of that screen
			screens_params_edition.load_cur_function = SCREENS_params_edition_load_PARAMS_EDIT_A;
			screens_params_edition.show_cur_function = SCREENS_params_edition_show_PARAMS_EDIT_A;
			screens_params_edition.ev_process_cur_function = SCREENS_params_edition_ev_manager_PARAMS_EDIT_A;	
			break;

		default:
			i8_ret_val = -1;
			break;
			
	}//switch

	// call to the function that loads current dialog screen specific data
	if (i8_ret_val>=0){
		
		// update screens_params_edition.i8_state before calling current dialog specific load function
		if (screens_params_edition.load_cur_function!=NULL){		
			i8_ret_val = screens_params_edition.load_cur_function();
		}//if
        		
		// set the SCREENS control in PARAM EDITION SCREEN mode
		screens_control.i8_current_type=SCREENS_PARAMS_EDITION_SCREEN;
		screens_control.i16_current_ID=i16_params_edition_id;
		// set the OnLoad event to show the screen content
		screens_control.ui8_on_load_event=TRUE;
		
	}//if

	return i8_ret_val;

}//SCREENS_params_edition_load



void SCREENS_params_edition_show(){
	int16_t i16_lcdx;// x position where the character is going to be print
	int8_t i8_param_row;// current parameters row
	int16_t i16_param_rows_to_show;// the number of rows to show
	int8_t i8_param_cols_to_show;// the number of columns to show
	uint8_t ui8_str_aux[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
	int8_t i8_aux2;
	int16_t i16_aux2;
	double db_aux;


	i16_aux2=screens_params_edition.i16_bottom_index;

	// calculate the number of rows of parameters that are going to be shown in the screen. A row is something like this:
	//  Param1    Param2   Param3
	//   25       [On]       200
	if ( ( (screens_params_edition.i16_top_index - screens_params_edition.i16_bottom_index)+1 )>=(screens_params_edition.i16_params_rows_numb*screens_params_edition.i16_params_cols_numb)){
		i16_param_rows_to_show=screens_params_edition.i16_params_rows_numb;
	}else{
		db_aux=((double)(screens_params_edition.i16_top_index - screens_params_edition.i16_bottom_index)+1)/(double)screens_params_edition.i16_params_cols_numb ;
		i16_param_rows_to_show=(int8_t)AUX_FUNCS_ceil(db_aux);
	}//if

	// print each parameter into the LCD
	for (i8_param_row=0;i8_param_row<i16_param_rows_to_show;i8_param_row++){
		
		// calculate the number of columns that are going to be shown in the current row
		if ( ( (screens_params_edition.i16_top_index - i16_aux2) + 1 )>screens_params_edition.i16_params_cols_numb){
			i8_param_cols_to_show=screens_params_edition.i16_params_cols_numb;
		}else{
			i8_param_cols_to_show=(screens_params_edition.i16_top_index - i16_aux2) + 1 ;
		}//if
			
		// print parameter descriptions on each column of that row
		i16_lcdx=0;
		memset(ui8_str_aux,SCREENS_PARAM_EDIT_BACKGROUND_CHAR,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
		for (i8_aux2=0;i8_aux2<i8_param_cols_to_show;i8_aux2++){		
			// print the parameter edition description on the text buffer
			GRAPHIX_text_buffer_set_string(i16_lcdx,(i8_param_row*SCREENS_PARAM_EDIT_ROWS),(uint8_t*)screens_params_edition.parameter[i16_aux2+i8_aux2].pui8_description,ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_NEUTRAL, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
			i16_lcdx=i16_lcdx+screens_params_edition.i16_params_cols_width+1;
		}//for

		// print parameter values
		i16_lcdx=0;
		memset(ui8_str_aux,SCREENS_PARAM_EDIT_BACKGROUND_CHAR,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
		for (i8_aux2=0;i8_aux2<i8_param_cols_to_show;i8_aux2++){
			
			// check if the current parameter is the selected
			if (i16_aux2+i8_aux2==screens_params_edition.i16_selected_index){				
   			    // add the brackets [ ] to mark the selected parameter
				sprintf (ui8_str_aux,"[%s]",screens_params_edition.parameter[i16_aux2+i8_aux2].value);
				// print the parameter edition description on the text buffer
				GRAPHIX_text_buffer_set_string(i16_lcdx,(i8_param_row*SCREENS_PARAM_EDIT_ROWS+1),(uint8_t*)ui8_str_aux,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_NEUTRAL, GRAPHIX_TEXT_COL_IDX_BLACK);			
			}else{
				// do not add the brackets because the parameter is not selected
				sprintf (ui8_str_aux," %s  ",screens_params_edition.parameter[i16_aux2+i8_aux2].value);
				// print the parameter edition description on the text buffer
				GRAPHIX_text_buffer_set_string(i16_lcdx,(i8_param_row*SCREENS_PARAM_EDIT_ROWS+1),(uint8_t*)ui8_str_aux,ATTR_SPACE_BACKSYMBOL,GRAPHIX_TEXT_COL_NEUTRAL, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
				
			}
			i16_lcdx=i16_lcdx+screens_params_edition.i16_params_cols_width+1;
			
		}//for

		// print the parameter edition value on the buffer
		// GRAPHIX_text_buffer_set_string(0,(i8_param_row*SCREENS_PARAM_EDIT_ROWS+1),(uint8_t*)ui8_aux_string,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_NEUTRAL,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);

		i16_aux2=i16_aux2+screens_params_edition.i16_params_cols_numb;
		
	}//for

	// call to the function that shows current dialog screen specific data
	if (screens_params_edition.show_cur_function!=NULL){
		screens_params_edition.show_cur_function(SCREEN_EMPTY_MESSAGE);
	}

	// prints the 'up', 'down' or 'score' indicating to the user if there are more elements available
	SCREENS_print_up_down_symbols(screens_params_edition.i16_bottom_index, screens_params_edition.i16_top_index, screens_params_edition.i16_num_parameters );

	// refresh the content of the buffer to screen
	GRAPHIX_text_buffer_refresh();

}//SCREENS_params_edition_show



void SCREENS_params_edition_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){


	// processes the common events in the current param edition screen

	// ON LOAD event, a new parameter screen has been loaded
	if (screens_control.ui8_on_load_event==TRUE){
	
		// reset the OnLoad flag
		screens_control.ui8_on_load_event = FALSE;

		// clear the screen buffer content by filling it with the SCREENS_MENU_BACKGROUND_CHAR char
		GRAPHIX_text_buffer_fill(SCREENS_PARAM_EDIT_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
				
		// print the title of new loaded parameter edition screen
		SCREENS_print_title(screens_params_edition.pui8_title,0);

		// update the help content lines in screen according to new loaded parameter edition screen
		SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[0] ,0);
		SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[1] ,1);
	
		// call the function that shows the current param edition screen
		SCREENS_params_edition_show();	
	
	}//if
	
	// RIGHT button has been pressed
	if (pui8_pushbutton_values[USER_IFACE_PUSHBT_RIGHT]==USER_IFACE_PRESSED){

		if ( (((screens_params_edition.i16_selected_index+1)%screens_params_edition.i16_params_cols_numb)!=0) && ((screens_params_edition.i16_selected_index+1)<screens_params_edition.i16_num_parameters) ){
			
			screens_params_edition.i16_selected_index++;
			
			// reset the selection encoder after selecting the new parameter
			pi16_encoders_var_value[USER_IFACE_ENC_1]=0;
								
			// if selected index has changed then also update the help text
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[0],0);
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[1],1);
			
		}//if

		// call the function that shows the current param edition screen
		SCREENS_params_edition_show();

	}//if

	// LEFT button has been pressed
	if (pui8_pushbutton_values[USER_IFACE_PUSHBT_LEFT]==USER_IFACE_PRESSED) {
		
		if (((screens_params_edition.i16_selected_index)%screens_params_edition.i16_params_cols_numb)==0){
			
			// reset the pointers to the functions that process current option
			screens_params_edition.load_cur_function=NULL;
			screens_params_edition.ev_process_cur_function=NULL;
			screens_params_edition.show_cur_function=NULL;
			
			// the user is in the top left side of the param edition screen and has pressed the back button,
			// so we have to jump back to the menu. Load the structure of the menu pointed by menu_to_jump
			SCREENS_menus_load(SCREEN_MENU_CURRENT);
			
		}else if ( (((screens_params_edition.i16_selected_index+1)%screens_params_edition.i16_params_cols_numb)!=1) && (screens_params_edition.i16_selected_index>0) ){
			
			screens_params_edition.i16_selected_index--;
			
			// reset the selection encoder after selecting the new parameter
			pi16_encoders_var_value[USER_IFACE_ENC_1]=0;
			
			// if selected index has changed then also update the help text
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[0],0);
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[1],1);			
			
			// call the function that shows the current param edition screen
			SCREENS_params_edition_show();
		
		}//if
		
	}//if

	// UP push button has been pressed
	if (pui8_pushbutton_values[USER_IFACE_PUSHBT_UP]==USER_IFACE_PRESSED){
		
		// update the index of the selection cursor
		if ( (screens_params_edition.i16_selected_index - screens_params_edition.i16_params_cols_numb) >= 0 ){
			
			screens_params_edition.i16_selected_index=screens_params_edition.i16_selected_index-screens_params_edition.i16_params_cols_numb;
			
			// reset the selection encoder after selecting the new parameter
			pi16_encoders_var_value[USER_IFACE_ENC_1]=0;
			
			// if selected index has changed then also update the help text
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[0],0);
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[1],1);
						
		}//if
		
		// update the top and the top and bottom indexes used to control the part of the param edition screen that is being shown
		if (screens_params_edition.i16_selected_index<screens_params_edition.i16_bottom_index){
			
			// update bottom index
			if (screens_params_edition.i16_bottom_index - screens_params_edition.i16_params_cols_numb < 0){
				screens_params_edition.i16_bottom_index = 0;
			}else{
				screens_params_edition.i16_bottom_index = screens_params_edition.i16_bottom_index - screens_params_edition.i16_params_cols_numb;
			}//if
			// update top index
			if (( screens_params_edition.i16_bottom_index+(screens_params_edition.i16_params_cols_numb*screens_params_edition.i16_params_rows_numb) )>screens_params_edition.i16_num_parameters){
				screens_params_edition.i16_top_index=screens_params_edition.i16_num_parameters-1;
			}else{
				screens_params_edition.i16_top_index=screens_params_edition.i16_bottom_index+(screens_params_edition.i16_params_cols_numb*screens_params_edition.i16_params_rows_numb)-1;
			}//if
			
			// as the navigation index has moved out of the screen then clear all the characters to write them all
			GRAPHIX_text_buffer_fill(SCREENS_PARAM_EDIT_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
			
		}//if
				
		// call the function that shows the current param edition screen
		SCREENS_params_edition_show();

	}//if
	
	// DOWN push button has been pressed
	if (pui8_pushbutton_values[USER_IFACE_PUSHBT_DOWN]==USER_IFACE_PRESSED){

		// update the index of the selection cursor
		if ( (screens_params_edition.i16_selected_index + screens_params_edition.i16_params_cols_numb) < screens_params_edition.i16_num_parameters ){

			// if the selection index can be moved to the next line by adding SCREENS_PARAM_EDIT_COLS we move it to the next line by adding SCREENS_PARAM_EDIT_COLS
			screens_params_edition.i16_selected_index=screens_params_edition.i16_selected_index+screens_params_edition.i16_params_cols_numb;
			
			// reset the selection encoder after selecting the new parameter
			pi16_encoders_var_value[USER_IFACE_ENC_1]=0;
			
			// if selected index has changed then also update the help text
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[0],0);
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[1],1);
						
		}else if( (screens_params_edition.i16_selected_index/screens_params_edition.i16_params_cols_numb) < ((screens_params_edition.i16_num_parameters-1)/screens_params_edition.i16_params_cols_numb)  ){
			
			// if can not move down one position the index but the cursor can still be moved to the next line, then we moved it to las position:
			//    ParA  ParB  ParC    >   ParA  ParB  ParC
			//     10    20   [30]	  >    10    20    30
			//    ParD  ParC          >   ParD  ParC      
			//     40    50	          >    40   [50]
			screens_params_edition.i16_selected_index = screens_params_edition.i16_num_parameters - 1;
			
			// reset the selection encoder after selecting the new parameter
			pi16_encoders_var_value[USER_IFACE_ENC_1]=0;
		
			// if selected index has changed then also update the help text
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[0],0);
			SCREENS_print_help(screens_params_edition.parameter[screens_params_edition.i16_selected_index].pui8_help[1],1);
			
		}//if
		
		// update the top and the top and bottom indexes used to control the part of the param edition screen that is being shown
		if (screens_params_edition.i16_selected_index>screens_params_edition.i16_top_index){
			
			// update bottom index to next line
			screens_params_edition.i16_bottom_index = screens_params_edition.i16_bottom_index + screens_params_edition.i16_params_cols_numb;
			
			// update top index
			if (( screens_params_edition.i16_bottom_index+(screens_params_edition.i16_params_cols_numb*screens_params_edition.i16_params_rows_numb) )>screens_params_edition.i16_num_parameters){
				screens_params_edition.i16_top_index=screens_params_edition.i16_num_parameters-1;
			}else{
				screens_params_edition.i16_top_index=screens_params_edition.i16_bottom_index+(screens_params_edition.i16_params_cols_numb*screens_params_edition.i16_params_rows_numb)-1;
			}//if
			
			// as the navigation index has moved out of the screen then clear all the characters to write them all
			GRAPHIX_text_buffer_fill(SCREENS_PARAM_EDIT_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
						
		}//if

		// call the function that shows the current param edition screen
		SCREENS_params_edition_show();

	}//if

	// call to the specific function that processes the KEYS used to modify selected parameter value
	if (screens_params_edition.ev_process_cur_function!=NULL){
	
		if (screens_params_edition.ev_process_cur_function(pi16_encoders_var_value, pui8_pushbutton_values)>0){
		
			// if the value has changed then call the function that shows the current param edition screen
			SCREENS_params_edition_show();
		
		}//if
	
	}//if
	
}//SCREENS_params_edition_ev_manager



uint8_t SCREENS_params_edition_ev_manager_update_param_val(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values, int16_t * pi16_value, int16_t pui16_step_variation, int16_t min_value, int16_t max_value){
	static TIMING_TICKS tticks_time_last_change=0;
	static TIMING_TICKS tticks_variation=0;
	uint8_t ret_val=FALSE;
	int16_t i16_aux1;
	TIMING_TICKS tticks_aux;
	uint8_t ch_factor;
	
	
	// to apply the variable increment or decrement on the parameter depending on the time the pushbutton or the
	// encoder has been pressed or turned, first check that the time elapsed since the last time the parameter was
	// modified is smaller than SCREEN_EDITION_CONT_PRESSED_MS. If the elapsed time since the last time it was
	// pressed it is greater than SCREEN_EDITION_CONT_PRESSED_MS it means that the pushbuttons may have been
	// released for a while or a short time ( it has not been continuously pressed ).
	tticks_aux=TIMING_GetTicksSinceStart();
	if ((tticks_aux-tticks_time_last_change)<SCREEN_EDITION_CONT_PRESSED_MS){
		// the pushbutton or the encoder has been pressed continuously
		tticks_variation=tticks_variation+(tticks_aux-tticks_time_last_change);
	}else{
		// it is more than
		tticks_variation=0;
	}//if
	tticks_time_last_change=tticks_aux;
	
	// calculate the factor used on the variable increase/decrease variation speed depending
	// on the time the pushbuttons or the encoder has been continuously pressed or turned.
	if (tticks_variation<SCREEN_EDITION_CONT_PR_INT2_MS){
		ch_factor=1; // first continuously pressed interval factor
	}else if (tticks_variation<SCREEN_EDITION_CONT_PR_INT3_MS){
		ch_factor=SCREEN_EDITION_CONT_PR_INT2_FACT; // second continuously pressed interval factor
	}else if (tticks_variation<SCREEN_EDITION_CONT_PR_INT4_MS){
		ch_factor=SCREEN_EDITION_CONT_PR_INT3_FACT; // third continuously pressed interval factor
	}else if (tticks_variation<SCREEN_EDITION_CONT_PR_INT5_MS){
		ch_factor=SCREEN_EDITION_CONT_PR_INT4_FACT; // fourth continuously pressed interval factor
	}else{
		ch_factor=SCREEN_EDITION_CONT_PR_INT5_FACT; // fifth continuously pressed interval factor
	}//if
	
	// INCREASE button has been pressed
	//if (pushbutton_values[USER_IFACE_PUSHBT_INCR_ACCEPT]==USER_IFACE_PRESSED){
	//	
	//	if ((*value+step_variation)>max_value){
	//		// if the highest value is exceeded continue form the lowest value
	//		*value=min_value;
	//	}else{
	//		*value=*value+(step_variation*ch_factor);
	//	}//if
	//	
	//	// return TRUE to indicate the value has been modified
	//	ret_val=TRUE;
    //		
	//}//if
	
	//  DECREASE button has been pressed
	//if (pushbutton_values[USER_IFACE_PUSHBT_DECR_CANCEL]==USER_IFACE_PRESSED){
	// 
	//  if ((*value-step_variation)<min_value){
	//	   // if the lowest value is exceeded continue form the highest value
	//     *value=max_value;
	//  }else{
	//     *value=*value-(step_variation*ch_factor);
	//  } //if
	//
	//  // return TRUE to indicate the value has been modified
	//  ret_val=TRUE;
	//
    //}//if

	// check the encoder state and modify the parameter if it has changed
	if (pi16_encoders_var_value[USER_IFACE_ENC_1]!=0){
		
		// in the case of the encoders the value variation is the step variation
		// multiplied by the number of the detected encoder steps
		i16_aux1 = (pui16_step_variation * ch_factor) * pi16_encoders_var_value[USER_IFACE_ENC_1];
		
		// modify the received value
		if ((*pi16_value+i16_aux1)>max_value){
			// if the highest value is exceeded continue form the lowest value
			*pi16_value=min_value;
		}else if ((*pi16_value+i16_aux1)<min_value){
			// if the lowest value is exceeded continue form the highest value
			*pi16_value=max_value;
		}else{
			*pi16_value=*pi16_value+i16_aux1;
		}//if
       
		// reset the encoder var value
		pi16_encoders_var_value[USER_IFACE_ENC_1]=0;
		
		// return TRUE to indicate the value has been modified
		ret_val=TRUE;
		
	}//if (encoders_var_value

	return ret_val;
	
}//SCREENS_params_edition_ev_manager_update_param_val



int8_t SCREENS_params_edition_load_PARAMS_EDIT_A(){
    int8_t i8_ret_val=0;	

			
	// initialize the parameters edition structure with the data of the selected channel
	// of the selected MOS6581 chip				
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_01].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_01_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_01].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_01_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_01].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_01_HLP2;
	sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_01].value,"%04d",test_borrame[0]);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_02_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_02_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_02_HLP2;
	switch (test_borrame[1]){
		case TEST_BORRAME_VALOR1:
			sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR1_STR);
			break;
		case TEST_BORRAME_VALOR2:
			sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR2_STR);
			break;
		case TEST_BORRAME_VALOR3:
			sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR3_STR);
			break;
		case TEST_BORRAME_VALOR4:
			sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR4_STR);
			break;
		case TEST_BORRAME_VALOR5:
			sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR5_STR);
			break;
	}//switch
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_03].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_03_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_03].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_03_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_03].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_03_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_03].value,"val3",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_04].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_04_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_04].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_04_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_04].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_04_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_04].value,"val4",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_05].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_05_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_05].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_05_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_05].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_05_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_05].value,"val5",SCREENS_PARAM_EDIT_MAX_LEN);

	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_06].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_06_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_06].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_06_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_06].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_06_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_06].value,"val6",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_07].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_07_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_07].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_07_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_07].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_07_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_07].value,"val7",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_08].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_08_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_08].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_08_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_08].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_08_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_08].value,"val8",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_09].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_09_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_09].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_09_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_09].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_09_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_09].value,"val9",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_10].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_10_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_10].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_10_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_10].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_10_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_10].value,"val10",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_11].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_11_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_11].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_11_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_11].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_11_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_11].value,"val11",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_12].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_12_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_12].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_12_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_12].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_12_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_12].value,"val12",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_13].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_13_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_13].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_13_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_13].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_13_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_13].value,"val13",SCREENS_PARAM_EDIT_MAX_LEN);
		
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_14].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_14_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_14].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_14_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_14].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_14_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_14].value,"val14",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_15].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_15_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_15].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_15_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_15].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_15_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_15].value,"val15",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_16].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_16_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_16].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_16_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_16].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_16_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_16].value,"val16",SCREENS_PARAM_EDIT_MAX_LEN);
		
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_17].pui8_description = SCREEN_PARAM_EDIT_A_PARAM_17_TXT;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_17].pui8_help[0] = SCREEN_PARAM_EDIT_A_PARAM_17_HLP1;
	screens_params_edition.parameter[PARAM_EDIT_A_PARAM_17].pui8_help[1] = SCREEN_PARAM_EDIT_A_PARAM_17_HLP2;
	AUX_FUNCS_lstrcpy(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_17].value,"val17",SCREENS_PARAM_EDIT_MAX_LEN);
	
	screens_params_edition.i16_num_parameters=PARAM_EDIT_A_PARAM_TOTAL;

	// configures the columns and rows how the parameters are going to be shown:
	screens_params_edition.i16_params_cols_numb=SCREENS_PARAM_EDIT_3_COLS;
	screens_params_edition.i16_params_rows_numb=SCREENS_PARAM_EDIT_6_ROWS; // set the number of para edition ROWs in one screen: 2 x 8 = 16 lines. Param description + Param value
	screens_params_edition.i16_params_cols_width=SCREENS_PARAM_EDIT_3_COLS_WIDTH;

	// initialize the index information
	screens_params_edition.i16_bottom_index=0;
	screens_params_edition.i16_selected_index=0;
	if (screens_params_edition.i16_num_parameters<(screens_params_edition.i16_params_cols_numb*screens_params_edition.i16_params_rows_numb )){
		screens_params_edition.i16_top_index=screens_params_edition.i16_num_parameters-1;
	}else{
		screens_params_edition.i16_top_index=screens_params_edition.i16_bottom_index+(screens_params_edition.i16_params_cols_numb*screens_params_edition.i16_params_rows_numb)-1;
	}//if
   
   return i8_ret_val;

}//SCREENS_params_edition_load_PARAMS_EDIT_A



int8_t SCREENS_params_edition_show_PARAMS_EDIT_A(uint8_t * ui8_message){
   int8_t i8_ret_val=0;
   
   
   return i8_ret_val;

}//SCREENS_params_edition_show_PARAMS_EDIT_A



int8_t SCREENS_params_edition_ev_manager_PARAMS_EDIT_A(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){
	int8_t i8_ret_value=-1;
	int16_t i16_aux1;


	// set the flag that indicates that some of the interface controls has changed its state and probably the value of the selected parameter has to updated
	if (pi16_encoders_var_value[USER_IFACE_ENC_1]!=0){
				
		i8_ret_value=1;
				
	}//if
		
	// check which of the parameters is being edited
	switch (screens_params_edition.i16_selected_index){
			
		case PARAM_EDIT_A_PARAM_01:
			
			if (i8_ret_value==1){
  
  				// call the function that manages the encoder and buttons and updates the value of the parameter if necessary
  				i16_aux1=(int16_t)test_borrame[0];
  				SCREENS_params_edition_ev_manager_update_param_val(pi16_encoders_var_value, pui8_pushbutton_values, &i16_aux1, 1, -1000, +1000 );
  				test_borrame[0] = i16_aux1;
				  
                // set the new value into parameter value
				sprintf(screens_params_edition.parameter[0].value,"%04d",test_borrame[0]);
				
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_01
			
		case PARAM_EDIT_A_PARAM_02:

			if (i8_ret_value==1){
					  
				// call the function that manages the encoder and buttons and updates the value of the parameter if necessary
				i16_aux1=(int16_t)test_borrame[1];
				SCREENS_params_edition_ev_manager_update_param_val(pi16_encoders_var_value, pui8_pushbutton_values, &i16_aux1, 1, TEST_BORRAME_VALOR1, TEST_BORRAME_VALOR5);
				test_borrame[1] = i16_aux1;
  
				// update the data to show on the screen with the new value
				switch (test_borrame[1]){
					case TEST_BORRAME_VALOR1:
						sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR1_STR);
						break;
					case TEST_BORRAME_VALOR2:
						sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR2_STR);
						break;
					case TEST_BORRAME_VALOR3:
						sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR3_STR);
						break;
					case TEST_BORRAME_VALOR4:
						sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR4_STR);
						break;
					case TEST_BORRAME_VALOR5:
						sprintf(screens_params_edition.parameter[PARAM_EDIT_A_PARAM_02].value,TEST_BORRAME_VALOR5_STR);
						break;
				}//switch
  						
			}//if (ch_update ...
					
			break;//PARAM_EDIT_A_PARAM_02

		case PARAM_EDIT_A_PARAM_03:

			if (i8_ret_value==1){
	
			}//if (ch_update ...

			break;//PARAM_EDIT_A_PARAM_03
			
		case PARAM_EDIT_A_PARAM_04:

			if (i8_ret_value==1){
						
			}//if (ch_update ...

			break;//PARAM_EDIT_A_PARAM_04
			
		case PARAM_EDIT_A_PARAM_05:

			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_05

		case PARAM_EDIT_A_PARAM_06:

			if (i8_ret_value==1){
						
			}//if (ch_update ...

			break;//PARAM_EDIT_A_PARAM_06

		case PARAM_EDIT_A_PARAM_07:

			if (i8_ret_value==1){
						
			}//if (ch_update ...

			break;//PARAM_EDIT_A_PARAM_07

		case PARAM_EDIT_A_PARAM_08:

			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_08

		case PARAM_EDIT_A_PARAM_09:

			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_09

		case PARAM_EDIT_A_PARAM_10:

			if (i8_ret_value==1){
						
			}//if (ch_update ...
					
			break;//PARAM_EDIT_A_PARAM_10

		case PARAM_EDIT_A_PARAM_11:
			
			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_11

		case PARAM_EDIT_A_PARAM_12:

			if (i8_ret_value==1){
						
			}//if (ch_update ...

		break;//PARAM_EDIT_A_PARAM_12
			
		case PARAM_EDIT_A_PARAM_13:
			
			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_13

		case PARAM_EDIT_A_PARAM_14:
			
			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_14

		case PARAM_EDIT_A_PARAM_15:
			
			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_15

		case PARAM_EDIT_A_PARAM_16:
			
			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_16

		case PARAM_EDIT_A_PARAM_17:

			if (i8_ret_value==1){
						
			}//if (ch_update ...
			
			break;//PARAM_EDIT_A_PARAM_17
			
        default:
			i8_ret_value=-1;
			break;
			
  	}//switch (screens_edition.i16_selected_index)

	return i8_ret_value;

}//SCREENS_params_edition_ev_manager_PARAMS_EDIT_A
