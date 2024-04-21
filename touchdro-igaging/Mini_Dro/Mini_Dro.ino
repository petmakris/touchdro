
/*********************************************************************
Project Name    :   Mini Dro
Hard revision   :   V1.0
Soft revision   :   /
Description     :   Dro system for lathe or milling machine with 3 quadrature decoder, Oled SSD1306 display and 4 push buttons
Chip            :   STM32F103C8T6
freq uc         :   72Mhz 
Compiler        :   Arduino IDE 1.8.3
Author          :   G.Pailleret, 2017 

Remark          :  ok


Hardware config : 
                    * Oled display in I2C_1 (SCL = PB6 and SDA = PB7) use STM32 I2C function
                    * Quadrature X on Timer1 ( PA8 and PA9)
                    * Quadrature Y on Timer3 ( PA6 and PA7)
                    * Quadrature Z on Timer4 ( PA0 and PA1)
                    * Button X (X reset) on PB15
                    * Button Y (Y reset) on PB14
                    * Button Z (Z reset) on PB12
                    * Button M (M reset) on PB13

Revision        :

*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>
#include "HardwareTimer.h"
#include "QuadDecoder.h"
#include "Button.h"
#include <EEPROM.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//Hard timer 1 for X scale quadrature : T1C1 (PA8) and T1C2 (PA9)
HardwareTimer timer_X(1);
//Hard timer 3 for Y scale quadrature : T3C1 (PA6) and T3C2 (PA7)  
HardwareTimer timer_Y(3);
//Hard timer 2 for Z scale quadrature : T2C1 (PA0) and T2C2 (PA1)  
HardwareTimer timer_Z(2);

//Quad decoder Class
QuadDecoder Quad_X(512,false,false);
QuadDecoder Quad_Y(512,false,false);
QuadDecoder Quad_Z(512,false,false);

//Keyboard
Button MiniDroKeyboard;

typedef struct
{
  boolean Inverted_X;  
  boolean Inverted_Y;
  boolean Inverted_Z;
  boolean Diameter_Mode_Y;
  unsigned int  Reso_X;
  unsigned int  Reso_Y;
  unsigned int  Reso_Z;
} sConfigDro;

const sConfigDro csConfigDefault = {false,false,false,false,512,512,512};



enum eMS_Dro 
        {   
      State_Normal, 
      State_In_Config 
    };


struct MyObject{
  float field1;
  byte field2;
  char name[10];
};


// Variable
sConfigDro ConfigDro;
eMS_Dro MS_Dro;
unsigned long TestSys;

//******************  IT function
void IT_Overflow_X(){
if (timer_X.getDirection()){
  Quad_X.OverflowMinus();
} else{
  Quad_X.OverflowPlus();
}
}
void IT_Overflow_Y(){
if (timer_Y.getDirection()){
   Quad_Y.OverflowMinus();
} else{
  Quad_Y.OverflowPlus();
}
}
void IT_Overflow_Z(){
if (timer_Z.getDirection()){
   Quad_Z.OverflowMinus();
} else{
  Quad_Z.OverflowPlus();
}
}
void SysTick_Handler() 
{ 
  //TestSys++;
}



void setup()   {

             
  Serial.begin(115200);
  Serial.println("\nTest");
  ReadConfigInFlash(&ConfigDro);
  //SaveConfigInFlash();
  while(1)
  {
    Serial.print("-");    
    delay(100); 
  }
 
  
  MS_Dro = State_Normal; 

  //Test led PC13
  pinMode(PC13, OUTPUT);  
  digitalWrite(PC13, LOW);

  /* Systick used by I2C at 1Khz... */ 
  systick_attach_callback(SysTick_Handler);

  //Config general
  Restore_Config();

  
  //## configure timer_X as quadrature encoder ##
  pinMode(PA8, INPUT_PULLUP);  //channel A
  pinMode(PA9, INPUT_PULLUP);  //channel B
  timer_X.setMode(0, TIMER_ENCODER); //set mode, the channel is not used when in this mode. 
  timer_X.pause(); //stop... 
  timer_X.setPrescaleFactor(1); //normal for encoder to have the lowest or no prescaler. 
  timer_X.setOverflow(0xFFFF);    
  timer_X.setCount(0);          //reset the counter. 
  timer_X.setEdgeCounting(TIMER_SMCR_SMS_ENCODER3); //or TIMER_SMCR_SMS_ENCODER1 or TIMER_SMCR_SMS_ENCODER2. This uses both channels to count and ascertain direction. 
  timer_X.attachInterrupt(0, IT_Overflow_X); //Overflow interrupt  
  timer_X.resume();                 //start the encoder... 
  //timer_X.getCount();

  //## configure timer_Y as quadrature encoder ##
  pinMode(PA6, INPUT_PULLUP);  //channel A
  pinMode(PA7, INPUT_PULLUP);  //channel B
  timer_Y.setMode(0, TIMER_ENCODER); //set mode, the channel is not used when in this mode. 
  timer_Y.pause(); //stop... 
  timer_Y.setPrescaleFactor(1); //normal for encoder to have the lowest or no prescaler. 
  timer_Y.setOverflow(0xFFFF);    
  timer_Y.setCount(0);          //reset the counter. 
  timer_Y.setEdgeCounting(TIMER_SMCR_SMS_ENCODER3); //or TIMER_SMCR_SMS_ENCODER1 or TIMER_SMCR_SMS_ENCODER2. This uses both channels to count and ascertain direction. 
  timer_Y.attachInterrupt(0, IT_Overflow_Y); //Overflow interrupt  
  timer_Y.resume();                 //start the encoder... 
  //timer_Y.getCount();

  //## configure timer_Z as quadrature encoder ##
  pinMode(PA0, INPUT_PULLUP);  //channel A
  pinMode(PA1, INPUT_PULLUP);  //channel B
  timer_Z.setMode(0, TIMER_ENCODER); //set mode, the channel is not used when in this mode. 
  timer_Z.pause(); //stop... 
  timer_Z.setPrescaleFactor(1); //normal for encoder to have the lowest or no prescaler. 
  timer_Z.setOverflow(0xFFFF);    
  timer_Z.setCount(0);          //reset the counter. 
  timer_Z.setEdgeCounting(TIMER_SMCR_SMS_ENCODER3); //or TIMER_SMCR_SMS_ENCODER1 or TIMER_SMCR_SMS_ENCODER2. This uses both channels to count and ascertain direction. 
  timer_Z.attachInterrupt(0, IT_Overflow_Z); //Overflow interrupt  
  timer_Z.resume();                 //start the encoder... 
  //timer_Z.getCount();

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  delay(2000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  delay(1000);

  //Clear the counter Bug demarrage
  Quad_X.SetAbsolutZero();
  Quad_X.SetRelativeZero();
  Quad_Y.SetAbsolutZero();
  Quad_Y.SetRelativeZero();
  Quad_Z.SetAbsolutZero();
  Quad_Z.SetRelativeZero();  
}

void loop() 
{
  eButtonState eState;
  unsigned int CurrentSelection=1;
  while (1) 
  {
    Serial.print("-");
    Quad_X.CounterValue(timer_X.getCount());
    Quad_Y.CounterValue(timer_Y.getCount());
    Quad_Z.CounterValue(timer_Z.getCount());

    //Update Keyboard
    MiniDroKeyboard.Update();
    eState = MiniDroKeyboard.GiveMeTheButtonState(); 
    //Masterstate
    switch ( MS_Dro )
    {
      case State_Normal:
        //Normal mode 
        Display_Test();
        if(eState == X_KeyShortPressed) Quad_X.SetZeroActiveMode(); 
        if(eState == Y_KeyShortPressed) Quad_Y.SetZeroActiveMode();
        if(eState == Z_KeyShortPressed) Quad_Z.SetZeroActiveMode();
        if(eState == X_LongPressed) Quad_X.SwitchMode(); 
        if(eState == Y_LongPressed) Quad_Y.SwitchMode();
        if(eState == Z_LongPressed) Quad_Z.SwitchMode();
        if(eState == M_LongPressed) MS_Dro = State_In_Config ; 
      break;
      case State_In_Config:
        //Config mode
        if(eState == X_KeyShortPressed)
        {
          if(CurrentSelection==1)CurrentSelection=8;
          else CurrentSelection--;     
        }
        if(eState == Z_KeyShortPressed)
        {
          if(CurrentSelection==8)CurrentSelection=1;
          else CurrentSelection++;     
        }
        if(eState == Y_KeyShortPressed)
        {
          switch(CurrentSelection)
          {
            case 1:
              ConfigDro.Inverted_X = !ConfigDro.Inverted_X;  
            break;  
            case 2:
              ConfigDro.Inverted_Y = !ConfigDro.Inverted_Y;
            break;
            case 3:
              ConfigDro.Inverted_Z = !ConfigDro.Inverted_Z;
            break;
            case 4:
              ConfigDro.Diameter_Mode_Y = !ConfigDro.Diameter_Mode_Y;
            break; 
            case 5:
              if(ConfigDro.Reso_X != 1)ConfigDro.Reso_X--;            
            break;
            break; 
            case 6:
              if(ConfigDro.Reso_Y != 1)ConfigDro.Reso_Y--;      
            break;
            case 7:
              if(ConfigDro.Reso_Z != 1)ConfigDro.Reso_Z--;  
            break;
            case 8:
               MS_Dro = State_Normal ;
              //Store config in memort
              SaveConfigInFlash(&ConfigDro);
              //Dispatch config to function
              Dispatch_Config(&ConfigDro);
            break;
              
          }    
        }
        if(eState == M_KeyShortPressed)
        {
          switch(CurrentSelection)
          {
            case 1:
             
            break;  
            case 2:
  
            break;
            case 3:
 
            break;
            case 4:
            break; 
            case 5:
              if(ConfigDro.Reso_X <= 1000)ConfigDro.Reso_X++;            
            break;
            break; 
            case 6:
              if(ConfigDro.Reso_Y <= 1000)ConfigDro.Reso_Y++;      
            break;
            case 7:
              if(ConfigDro.Reso_Z <= 1000)ConfigDro.Reso_Z++;  
            break;
            case 8:
            break;
              
          }    
        }  
        Display_Config(CurrentSelection);
      break;   
    }  
  }  
}

void Display_Test()
{
  char buffer_x[16];
  char buffer_y[16];
  char buffer_z[16];
  sprintf(buffer_x,"%8.3f",Quad_X.GetValue());
  sprintf(buffer_y,"%8.3f",Quad_Y.GetValue());
  sprintf(buffer_z,"%8.3f",Quad_Z.GetValue());
  display.clearDisplay();
  //Constant text
  display.setTextColor(BLACK,WHITE);
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("  Mini Dro V1.000   ");
  display.setTextSize(1);
  display.setTextColor(WHITE); 
  display.println("  ");

  display.setTextSize(2);
  if(Quad_X.RelativeModeActived()) display.setTextColor(BLACK,WHITE);
  else display.setTextColor(WHITE);  
  display.print("X");
  display.setTextColor(WHITE);
  display.print(":");  
  display.println(buffer_x);
  if(Quad_Y.RelativeModeActived()) display.setTextColor(BLACK,WHITE);
  else display.setTextColor(WHITE);  
  display.print("Y");
  display.setTextColor(WHITE);
  display.print(":");  
  display.println(buffer_y);
  if(Quad_Z.RelativeModeActived()) display.setTextColor(BLACK,WHITE);
  else display.setTextColor(WHITE);  
  display.print("Z");
  display.setTextColor(WHITE);
  display.print(":");  
  display.println(buffer_z);
  display.display();
}


void Display_Config(unsigned int CurrentSelection)
{
 
  
  display.clearDisplay();
  //Constant text
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(1);
  if(CurrentSelection==1)display.setTextColor(BLACK,WHITE); 
  display.print("X inverted : ");
  if(ConfigDro.Inverted_X)display.println("true");
  else display.println("false");
  if(CurrentSelection==1)display.setTextColor(WHITE);
  if (CurrentSelection==2)display.setTextColor(BLACK,WHITE); 
  display.print("Y inverted : ");
  if(ConfigDro.Inverted_Y)display.println("true");
  else display.println("false");
  if(CurrentSelection==2)display.setTextColor(WHITE);
  if (CurrentSelection==3)display.setTextColor(BLACK,WHITE); 
  display.print("Z inverted : ");
  if(ConfigDro.Inverted_Z)display.println("true");
  else display.println("false");
  if(CurrentSelection==3)display.setTextColor(WHITE);
  if (CurrentSelection==4)display.setTextColor(BLACK,WHITE);
  display.print("Diameter mode : ");
  if(ConfigDro.Diameter_Mode_Y)display.println("true");
  else display.println("false");
  if(CurrentSelection==4)display.setTextColor(WHITE);
  if (CurrentSelection==5)display.setTextColor(BLACK,WHITE);
  display.print("Reso X: ");
  display.println(ConfigDro.Reso_X,DEC);
  if(CurrentSelection==5)display.setTextColor(WHITE);
  if (CurrentSelection==6)display.setTextColor(BLACK,WHITE);
  display.print("Reso Y: ");
  display.println(ConfigDro.Reso_Y,DEC);
  if(CurrentSelection==6)display.setTextColor(WHITE);
  if (CurrentSelection==7)display.setTextColor(BLACK,WHITE);
  display.print("Reso Z: ");
  display.println(ConfigDro.Reso_Z,DEC);
  if(CurrentSelection==7)display.setTextColor(WHITE);
  if (CurrentSelection==8)display.setTextColor(BLACK,WHITE);
  display.println("--> Save & Exit");
  display.display();
}

void Restore_Config()
{
  //Read Config in Memory
  ReadConfigInFlash(&ConfigDro);
  //Dispatch the config
  Dispatch_Config(&ConfigDro);
}
void SaveConfigInFlash(sConfigDro *pConf)
{
  unsigned int uiCount;
  char *pt;
  EEPROM.format();
  pt = (char*)pConf; 
  for(uiCount=0;uiCount<sizeof(sConfigDro);uiCount++)
  {
    EEPROM.write(uiCount,*pt);
    pt++;  
  } 
}
void ReadConfigInFlash(sConfigDro *pConf)
{
  unsigned int uiCount;
  uint16 uiState;
  uint16 value;
  char *pt;
  uiState = EEPROM_OK;
  pt = (char*)pConf; 
  for(uiCount=0;uiCount<sizeof(sConfigDro);uiCount++)
  {
    uiState |= EEPROM.read(uiCount,&value);
    *pt = (char) value;
    pt++;  
  }
  if(uiState != EEPROM_OK)
  {
    //Problem, restore default  
    *pConf = csConfigDefault;
  } 
}
void Dispatch_Config(sConfigDro *pConf)
{
  Quad_X.SetSens( pConf->Inverted_X );  
  Quad_Y.SetSens( pConf->Inverted_Y );
  Quad_Z.SetSens( pConf->Inverted_Z );
  Quad_Y.SetDiameterMode(pConf->Diameter_Mode_Y);
  Quad_X.SetResolution(pConf->Reso_X);
  Quad_Y.SetResolution(pConf->Reso_Y);
  Quad_Z.SetResolution(pConf->Reso_Z);
}
