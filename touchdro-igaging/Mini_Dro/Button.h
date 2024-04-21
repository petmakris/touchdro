#ifndef Button_h
#define Button_h
    
#include <Arduino.h>
#include "RingBuffer.h"

#define IN_SW_X PB15
#define IN_SW_Y PB14
#define IN_SW_Z PB12
#define IN_SW_M PB13 

#define Key_X_Pressed !digitalRead(IN_SW_X)
#define Key_Y_Pressed !digitalRead(IN_SW_Y)
#define Key_Z_Pressed !digitalRead(IN_SW_Z)
#define Key_M_Pressed !digitalRead(IN_SW_M)

#define TimeDebounce 80		//Deebounce time in ms
#define TimeLongPress 2000	//Long press time in ms


//Enum Mastersate
enum eMS_Button 
        { 	
			All_Key_Released, 
			X_Key_In_Debounce, 
			X_Key_Debounced,
			X_Key_LongPressed,
			Y_Key_In_Debounce, 
			Y_Key_Debounced,
			Y_Key_LongPressed,
			Z_Key_In_Debounce, 
			Z_Key_Debounced,
			Z_Key_LongPressed,
			M_Key_In_Debounce, 
			M_Key_Debounced,
			M_Key_LongPressed
		};

enum eButtonState 
        {   
          AllKeyReleased = 0,  
          X_KeyShortPressed,
		  X_LongPressed,
		  Y_KeyShortPressed,
		  Y_LongPressed,
		  Z_KeyShortPressed,
		  Z_LongPressed,
		  M_KeyShortPressed,
		  M_LongPressed,
        };



 
class Button
{
public:
Button();
void Update();
eButtonState GiveMeTheButtonState();

private:
void Add_State (eButtonState State);
eMS_Button  MS_Button;
unsigned long TimeStartPress;
};
    
#endif
