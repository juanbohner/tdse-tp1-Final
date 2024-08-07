#ifndef INC_MENU_H_
#define INC_MENU_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include <display.h>
/********************** macros ***********************************************/

/********************** typedef **********************************************/
//typedef enum s_menu {STATE_MENU_0, STATE_MENU_1, STATE_MENU_2,STATE_MENU_3, STATE_MENU_4};
//
///********************** external data declaration ****************************/
//
///********************** external functions declaration ***********************/
//void reset_display();
void imprimir_menu_0(int next_hop);
void imprimir_menu_1();
void imprimir_menu_2(int next_hop);
void imprimir_menu_3(int next_hop, float temp);
/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* APP_INC_APP_H_ */

/********************** end of file ******************************************/
