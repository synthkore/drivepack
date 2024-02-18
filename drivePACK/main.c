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

#include "sam.h"
#include "./FatFsSrc/ff.h"
#include "gpio.h"
#include "sys_defs.h"
#include "aux_funcs.h"
#include "timing.h"
#include "sdmmc.h"
#include "console.h"
#include "graphix.h"
#include "user_iface.h"
#include "./Screens/screens.h"
#include "./Commands/commands.h"


// TODO:
// Dar soporte a nombres largos
// Reducir tiempo de mensaje al Recivir cancion desde el PC o tras hace el LOAD etc
// Cuando se está reproduciendo una ROM debería aparecer el titulo de la ROM, y no el "Run fil in RAM".
// La opcion "Save as" es una chusta... no se entiende nada como guardar el fichero, debería ser mucho más sencillo.
// Revisar la carga de los ficheros .DRP, a ver si los campos son opcionales o no, y ¿que hace el software si el fichero no tien datos binarios de la ROM, o de titulo o de las canciones ?
// En las pantallas de carga o de escritura de un fichero indicar el nombre del fichero cargado o guardado al mostrar el mensaje de confirmación.
// Las funciones load_ ( p.ej. SCREENS_dialog_load_SAVE_RAM_FILE ) se podrían quitar y utilizar el evento LOAD==TRUE para todas las inicializaciones en cada subestado ¿ O quizas mejor quitar el LOAD==TRUE y dejar las funciones load para que quede to más organziado ?
// Funcion para limpiar ( memset 0 ) todas las estructuras correspondientes a una ROM y llamarla siempre antes de recibir o cargar un fichero
// Falta reordenar las pantallas de File Explorer, terminar las funciones load, show, event manager... comprobar que las secuencias son correctas...
// Falta actualizar los títulos de las pantallas de File Explorer
// Revisar asunto del SCREEN_DIALOG_INITIAL_STATE de todos los Dialog: se va a usar realmente ? se está usando bien ?
// Al hacer SCREEN_DIALOG_DEFAULT_STATE desde screens_menus ( SCREENS_dialog_load(screens_menus.option[i16_aux].i16_screen_item_id,SCREEN_DIALOG_DEFAULT_STATE); ) siempre se pone SCREEN_DIALOG_DEFAULT_STATE ¿ no habria que indicar también el subestado ? o quizas en lugar de ser SCREEN_DIALOG_DEFAULT_STATE podriamos poner SCREEN_DIALOG_INITIAL_STATE ??????
// Revisar Title y 2 lineas Help de Dialog, porque parece que hay muchas que no tienen pero deberían tenerlos/las.
// Corregir traza JBR 2022-09-19 Corregir la napa y poner orden en el file explorer y actualizar gestión de las teclas, habría que anadir otro pulsador ....
// ¿¿ Mover el timer TIMING_TICKS timer; de las estructuras screen_dialog, screens_f_explorer, screens_param_edition etc. a la estructura screens_control ?? ... no se si vale la pena
// Unificar la nomenclatura / nombre de los diferentes elementos en la interfaz y en los comentarios: ¿ usamos memory buffer, RAM buffer, nibbles buffer ?... siempre lo mismo para no confundir
// ¿ Falta un commands_init() ??
// Al entrar en un fichero desde el navegador y salir por un error o pq ha terminado, en lugar de regresar el navegador, nos regresa al menu.
// Mirar lo de eliminar la funcion de SCREEN_MENUS_Init y meterlo todo SCREEN_MENUS_clear, tal como s ehace en SCREEN_DIALOG, SCREEN_PARAMETER_EDITION_et
// Mirar tlo de hacer los arrays como CONST
// !!! Se propone revisar las funciones que reciben como parámetro la longitud del ¿ Buffer ? ¿ cadena ? para que consideren este parámetro siempre como el tamaño del buffer y no como el tamaño máximo de la cadena. En las AUX_FUNCS ya se está hacindeo bien pero p.ej. en FILE_SYS no. En AUX_FUNC dice: " ui16_MAX_STR_ARRAY: the size of the array used to store the destination string, the last  allowed character can be stored in (ui16_MAX_STR_ARRAY-2) and in that case (ui16_MAX_STR_ARRAY-1)  should contain a '\0'."
// !!! Hay un monton de llamadas a funciones que hacen uso de los buffers auxiliares AUX_FUNCS_aux_string1 screens_str_aux2 etc ( p.ej AUX_FUNCS_strcat(screens_str_aux2,"[",SCREEN_MAX_CHARS_PER_ROW) que tienen mal puestas las dimensiones del array.
// Sería interesante que al llamar a las funciones AUXFUNCS_ con cadenas, en lugar de pasarles la cadena, y el tamaño del array, le pasásemos un tipo en el que por un lado estuviera la cadena y por el otro el tamaño del array, así quedaría todo más compacto.
// En el componente FYLE_SYS deberia guardarse una cadena con el path actual del sistema de ficheros: ui8_current_path. Este path se debería inicializar al hacer Mount, asignar al hacer OpenDir o ir actualizando al hacer Chdir
// Mirar como organizo la secuencia Load y Show y la inicializacion de los indices en Menus, la idea es que al hacer un Load los indices se inicialicen en el load, pero esto no sirve en: SCREENS_menus_load...  SCREENS_menus_show
// Organizar la funcion de Init de las Screens porque se está haciendo el Init de los menus en screen.h pero en el de las otras estructuras en sus respectivos ficheros. El Init de los menus deberia estar en screen_menus y en screens.h solo la estructura screen global.
// Hacer que la funcion LOAD the admita un paráemtro con el subestado al que queremos saltar. Modificar la estructura t_menu_option de modo que esta indique:
// - El tipo de elemento al que apunta ( MENU, DIALOGO o F EXPLORER )
// - Añadir un parametro ID al (*load_function) para que al hacer Load inicialice la estructura de un modo u otro
// - Añadir el valor del parametro con que se llamará a la funcion (*load_function).
// typedef struct{
//  	int16_t i16_menu_to_jump; // stores the ID of the menu where the interface has to jump. If it is set to SCREENS_MENU_NULL it means it does not have to jump to any menu option.
// 	    uint8_t ui8_description[SCREENS_MENU_OPTION_MAX_LEN]; // the string with the option text
// 	    void (*key_process_function)(int16_t * , uint8_t * ); // pointer to the function that processes the keys pressed by the user on that param edition screen
// 	    void (*load_function)(); // pointer to the function that loads the information corresponding to that function screen
// 	    void (*show_function)(); // pointer to the function that shows the information corresponding to that function screen
// }t_menu_option;
// Detectar cuando se ha introducido el número máximo de caracteres, no dejar seguir metiendo ni almacenando.
// Revisar las llamadas a las funciones AUX_FUNCS_strlen(xxxx, AUX_FUNCS_F_P_MAX_LEN_STR); ya que no se si AUX_FUNCS_F_P_MAX_LEN_STR está correctametne o convenientemente dimenisonado. Quizas habría que implementar la opción que si se pone -1 en lugar de la dimension máxima, no se cheque el límite de tamaño.
// En ScreensEdition incluir la barra de situación vertical.
// En ScreensEdition incluir las flechas indicativas si ha hay contenio a visualizar hacia arriba o hacia abajo
// En las pantallas añadir la posibilidad de incluir ayuda y título.
// En las pantallas añadir la posibilidad de incluir cooredenada X e Y

// REVISAR:
// Añadir como parámetro los atributos tras la cadena / caracter
// Revisar estas asignaciones en el Init del text buffer
//		text_buffer.row[ui8_row_aux].ui8_coord_X = 1;
//		text_buffer.row[ui8_row_aux].ui8_coord_Y = 1;
//		text_buffer.row[ui8_row_aux].ui8_pixel_size_X = 1;
//		text_buffer.row[ui8_row_aux].ui8_pixel_size_Y = 1;
// Cambiar los índices utilizados para el acceso al text buffer por uint16, y tambien cambiar los ui8_y_origin etc? de ui8 a ui16
// Creo que no, pero como es confuso tener 2 grupos de defines tan similares, ¿no se podría usar solo un grupo?
//     Grupo1: SCREEN_JUMP_TO_DIALOG, SCREEN_JUMP_TO_PARAMS_EDITION, ....
//     Grupo2: SCREENS_MENU_SCREEN, SCREENS_PARAMS_EDITION_SCREEN, SCREENS_DIALOG_SCREEN
// Analizar como vamos a retornar de las pantallas tipo Dialog, asignaremos directamente las variables a las estructuras de control de screen ?

// HECHO:
// !!!!!! Revisar las longitudes de los arrays de cadenas, y los MAX_LEN y meter el parámetro MAX_LEN en los que no tengan.
// Revisar asunto de las cadenas de texto y el campo MAX_LEN, y aclrar si el MAX_LEN es el tamaño del array, o la longitud del la cadena ( sin el '\0' ).
// Añadir campo LENGTH a las funciones que trabajan con cadenas y aun no tienen ese campo.
// Implementar la barra lateral en los menus.
// Impelmentar el poder mover por linea de comandos los GPIOs de los LEDs
// Actualizar la funcion de write ROM to disk ( grabar ficheros ) para funcionar con el nuevo formato de DATA BLOCKS
// Limpiar fondo tras el texto de los títulos y la ayuda
// Añadir el '.' y el '..' al navegador File Explorer.
// Se estan metiendo muchos buffers y variables de los modelos de los LCDs que no se estan utilizando ( mirar en el .map ). Habría que deshabilitarlos con algun define.
// Mover el comando "coms rx1k" a "rom rx1k" y quitar todas las rutinas COMS
// Hacer que en la opcion Load and Run, al cargar la ROM y hacer RUN pulsando back se pueda regresar directamente a la carpeta en que estabamos, o si pulsamos a la derecha volver al menu.
// Al navegar en el file explorer hacia adelante y hacia atrás en ocasiones no deja entrar dentro de determinadas carpetas. Quizás el contador de profunidad este gestionandose mal.
// Reemplazar en el código todas las referencias a romulus_ por dpack_

int main(void)
{
	int16_t pi16_encoders_var_value[USER_IFACE_MAX_ENCODERS];
	uint8_t pui8_buttons_state[USER_IFACE_MAX_PUSH_BUTTONS];	 
	uint8_t pui8_console_in_buffer[MAX_COMMAND_LEN];
	t_command command_aux;
	int16_t i16_aux = 0;

	SystemInit();
	GPIO_InitClock();	
	GPIO_Init();
	TIMING_Init();
	SDMMC_Init();
	USER_IFACE_Init();
	CONSOLE_Init();
	GRAPHIX_Init();
	SCREENS_Init();
    FILE_SYS_Init();
    DPACK_CTRL_Init();

    USER_IFACE_encoders_clear(pi16_encoders_var_value);
    USER_IFACE_pusbuttons_clear(pui8_buttons_state);

/*
	LCD_ST7735_fast_fill ( LCD_COLOR_BLUE, 0,0, 159, 127);
	tticks_timer = TIMING_SetTicksDelay(50000);while (TIMING_CheckTicksDelayReached(tticks_timer)==FALSE){1;};

	// LCD_ST7735_fast_fill ( LCD_COLOR_RED, 10,10, 100, 100);
	// tticks_timer = TIMING_SetTicksDelay(50000);while (TIMING_CheckTicksDelayReached(tticks_timer)==FALSE){1;};

	// LCD_ST7735_fast_fill ( LCD_COLOR_YELLOW, 30,30, 80, 80);
	// tticks_timer = TIMING_SetTicksDelay(50000);while (TIMING_CheckTicksDelayReached(tticks_timer)==FALSE){1;};
	
	GRAPHIX_print_string(12, 20, 2, 2, LCD_COLOR_BLACK, LCD_COLOR_TRANSPARENT,(uint8_t*)"DRIVE pack",11);
	GRAPHIX_print_string(11, 19, 2, 2, LCD_COLOR_WHITE, LCD_COLOR_TRANSPARENT,(uint8_t*)"DRIVE pack",11);

    GRAPHIX_print_string(35, 50, 1, 1, LCD_COLOR_WHITE, LCD_COLOR_TRANSPARENT,(uint8_t*)"K4SIO ROM pack",15);
	GRAPHIX_print_string(35, 60, 1, 1, LCD_COLOR_WHITE, LCD_COLOR_TRANSPARENT,(uint8_t*)"  emulator",15);

    GRAPHIX_print_string(15, 80, 1, 1, LCD_COLOR_WHITE, LCD_COLOR_TRANSPARENT,(uint8_t*)"by synthkore - 2022",20);
	GRAPHIX_print_string(0, 90, 1, 1, LCD_COLOR_WHITE, LCD_COLOR_TRANSPARENT,(uint8_t*)"www.synthkore.com/dpack",24);

	// initialize the buffers used to store the state of the encoders and pushbuttons
	USER_IFACE_samples_clear(i16_encoders_var_value,ui8_buttons_state);    
		
	GRAPHIX_text_buffer_set_string(0, 0,  "00cdefghijklmnopqrstxyz", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_WHITE, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 1,  "01234567890123456789012", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_WHITE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 2,  "02234567891011213141516", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_RED, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 3,  "03cdefghijklmnopqrstxyz", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_GREEN, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 4,  "04CDEFGHIJKLMNOPQRSTXYZ", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 5,  "05234567891011213141516", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_RED, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 6,  "06cdefghijklmnopqrstxyz", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_ORANGE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 7,  "07CDEFGHIJKLMNOPQRSTXYZ", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_YELLOW, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 8,  "08234567891011213141516", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_GREEN, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 9,  "09cdefghijklmnopqrstxyz", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_DARK_GREEN, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 10, "10CDEFGHIJKLMNOPQRSTXYZ", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 11, "11234567891011213141516", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_BRIGHT_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 12, "12cdefghijklmnopqrstxyz", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_DARK_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 13, "13CDEFGHIJKLMNOPQRSTXYZ", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 14, "14234567891011213141516", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_BRIGHT_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 15, "15cdefghijklmnopqrstxyz", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_DARK_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 16, "16CDEFGHIJKLMNOPQRSTXYZ", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_ORANGE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 17, "17234567891011213141516", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_ORANGE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 18, "18cdefghijklmnopqrstxyz", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_ORANGE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 19, "19CDEFGHIJKLMNOPQRSTXYZ", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_ORANGE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	GRAPHIX_text_buffer_set_string(0, 20, "20234567891011213141516", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_ORANGE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
		
	GRAPHIX_text_buffer_refresh();

	GRAPHIX_text_buffer_set_string(3, 3, "YES YOU CAN!", ATTR_SHADOW, GRAPHIX_TEXT_COL_IDX_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_YELLOW);
	
	GRAPHIX_text_buffer_refresh();
	
	GRAPHIX_text_buffer_set_string(3, 3, "YES YOU CAN!", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_BLUE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	
	GRAPHIX_text_buffer_refresh();
	
	GRAPHIX_text_buffer_set_string(3, 5, "Y S Y U C N!", ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_RED, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	
	GRAPHIX_text_buffer_refresh();

    GRAPHIX_text_buffer_fill(129, ATTR_SHADOW, GRAPHIX_TEXT_COL_IDX_RED, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_YELLOW);

	GRAPHIX_text_buffer_refresh();

    GRAPHIX_text_buffer_fill(129, ATTR_SHADOW, GRAPHIX_TEXT_COL_IDX_RED, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_YELLOW);
	
	GRAPHIX_text_buffer_refresh();
*/

	while (1){
						
		// get all the encoders value
		USER_IFACE_sample_encoders(pi16_encoders_var_value);
			
		// get all the pushbuttons value
		USER_IFACE_sample_pushbuttons(pui8_buttons_state);
	        
		// update the corresponding screen if necessary
		SCREENS_ev_manager(pi16_encoders_var_value, pui8_buttons_state);
			
		// once the pushbuttons and the encoders have been processed all their values are cleared
        USER_IFACE_encoders_clear(pi16_encoders_var_value);
        USER_IFACE_pusbuttons_clear(pui8_buttons_state);
		
		// ####  COMMAND LINE INPUT PARSE: take input characters and decode the command if possible
		
		// when new characters are received then they are stored into the input buffer, if the 
		// final command character SYMBOL is received (CONSOLE_ev_manager(_CTRL_RETURN) then call the
		// function that parses the string with all the received chars to get the command tokens
		if (CONSOLE_check_input(pui8_console_in_buffer)==1){
			
		    // call the function that parses the string to get the command tokens
			if (COMMANDS_parse_command_string(pui8_console_in_buffer, MAX_COMMAND_LEN, &command_aux)<0){
				
				// if no valid command has been parsed then reset the console input buffer to  
				// start receiving characters of a new command
				COMMANDS_clear_command( &command_aux);
				// reset the console input buffer to start receiving characters of a new command
				CONSOLE_reset();
				
			}//if

		}//if

		// #### CURRENT COMMANDS EXECUTION: execute the command if there is any active

		// if a complete command has been received then it is executed. 
		//  '2' means a command cycle has been executed but not completed yet
		//  '0' means there is nothing to execute
		i16_aux = COMMANDS_execute(&command_aux);
		if ( (i16_aux!=2) && (i16_aux!=0) ){		
			
			// if command has been executed or if it has not been possible to execute 
			// then clear the structure and prepare it for a new command
			COMMANDS_clear_command( &command_aux);
			// reset the console input buffer to start receiving characters of a new command
			CONSOLE_reset();				
				
		}//if

	 }//while

}//main
