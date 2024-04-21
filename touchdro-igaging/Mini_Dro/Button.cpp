#include <Arduino.h>
#include "Button.h"
#include "RingBuffer.h"

RingBuffer MyRing(20);

Button::Button()
{
  pinMode(IN_SW_X, INPUT_PULLUP);
  pinMode(IN_SW_Y, INPUT_PULLUP);  
  pinMode(IN_SW_Z, INPUT_PULLUP);  
  pinMode(IN_SW_M, INPUT_PULLUP); 
  MS_Button = All_Key_Released;
  MyRing.clear();
}

void Button::Update()
{
  switch ( MS_Button )
  {
    case All_Key_Released:
      if ( Key_X_Pressed )
      {
        MS_Button = X_Key_In_Debounce; 
        TimeStartPress = millis();    
      }
	  if ( Key_Y_Pressed )
      {
        MS_Button = Y_Key_In_Debounce; 
        TimeStartPress = millis();    
      }
	  if ( Key_Z_Pressed )
      {
        MS_Button = Z_Key_In_Debounce; 
        TimeStartPress = millis();    
      }
	  if ( Key_M_Pressed )
      {
        MS_Button = M_Key_In_Debounce; 
        TimeStartPress = millis();    
      }
    break;
    case X_Key_In_Debounce:
       if ( Key_X_Pressed )
      {
        MS_Button = X_Key_In_Debounce;
        if( millis()> (TimeStartPress+TimeDebounce))
        {
          MS_Button = X_Key_Debounced;   
        }    
      }
      else
      {
         MS_Button = All_Key_Released;     
        
      }   
    break;
    case X_Key_Debounced:
      if ( Key_X_Pressed )
      {
		if( millis()> (TimeStartPress+TimeLongPress))
        {
			MS_Button = X_Key_LongPressed;
			Add_State(X_LongPressed);
        } 
      }
      else
      {
         MS_Button = All_Key_Released;
		 Add_State(X_KeyShortPressed);
         Add_State(AllKeyReleased);       
      }      
    break; 
	case X_Key_LongPressed:
      if ( Key_X_Pressed )
      {
      }
      else
      {
         MS_Button = All_Key_Released;
         Add_State(AllKeyReleased);       
      } 	
	break;
    case Y_Key_In_Debounce:
       if ( Key_Y_Pressed )
      {
        MS_Button = Y_Key_In_Debounce;
        if( millis()> (TimeStartPress+TimeDebounce))
        {
          MS_Button = Y_Key_Debounced;   
        }    
      }
      else
      {
         MS_Button = All_Key_Released;     
        
      }   
    break;
    case Y_Key_Debounced:
      if ( Key_Y_Pressed )
      {
		if( millis()> (TimeStartPress+TimeLongPress))
        {
			MS_Button = Y_Key_LongPressed;
			Add_State(Y_LongPressed);
        } 
      }
      else
      {
         MS_Button = All_Key_Released;
		 Add_State(Y_KeyShortPressed);
         Add_State(AllKeyReleased);       
      }      
    break; 
	case Y_Key_LongPressed:
      if ( Key_Y_Pressed )
      {
      }
      else
      {
         MS_Button = All_Key_Released;
         Add_State(AllKeyReleased);       
      } 	
	break;	
    case Z_Key_In_Debounce:
       if ( Key_Z_Pressed )
      {
        MS_Button = Z_Key_In_Debounce;
        if( millis()> (TimeStartPress+TimeDebounce))
        {
          MS_Button = Z_Key_Debounced;   
        }    
      }
      else
      {
         MS_Button = All_Key_Released;     
        
      }   
    break;
    case Z_Key_Debounced:
      if ( Key_Z_Pressed )
      {
		if( millis()> (TimeStartPress+TimeLongPress))
        {
			MS_Button = Z_Key_LongPressed;
			Add_State(Z_LongPressed);
        } 
      }
      else
      {
         MS_Button = All_Key_Released;
		 Add_State(Z_KeyShortPressed);
         Add_State(AllKeyReleased);       
      }      
    break; 
	case Z_Key_LongPressed:
      if ( Key_Z_Pressed )
      {
      }
      else
      {
         MS_Button = All_Key_Released;
         Add_State(AllKeyReleased);       
      } 	
	break;	
    case M_Key_In_Debounce:
       if ( Key_M_Pressed )
      {
        MS_Button = M_Key_In_Debounce;
        if( millis()> (TimeStartPress+TimeDebounce))
        {
          MS_Button = M_Key_Debounced;   
        }    
      }
      else
      {
         MS_Button = All_Key_Released;     
        
      }   
    break;
    case M_Key_Debounced:
      if ( Key_M_Pressed )
      {
		if( millis()> (TimeStartPress+TimeLongPress))
        {
			MS_Button = M_Key_LongPressed;
			Add_State(M_LongPressed);
        } 
      }
      else
      {
         MS_Button = All_Key_Released;
		 Add_State(M_KeyShortPressed);
         Add_State(AllKeyReleased);       
      }      
    break; 
	case M_Key_LongPressed:
      if ( Key_M_Pressed )
      {
      }
      else
      {
         MS_Button = All_Key_Released;
         Add_State(AllKeyReleased);       
      } 	
	break;		
  }
}

eButtonState Button::GiveMeTheButtonState()
{
  return (eButtonState)MyRing.pop();
}

void Button::Add_State (eButtonState State)
{
  MyRing.push((byte)State);    
}


