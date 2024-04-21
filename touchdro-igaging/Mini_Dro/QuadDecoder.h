#ifndef QuadDecoder_h
#define QuadDecoder_h
    
#include <Arduino.h>

#define Overflow_Size 65536    
class QuadDecoder
{
public:
QuadDecoder(unsigned int  Resolution , boolean Sens,boolean DiameterMode);

//To update value
void OverflowPlus();
void OverflowMinus();
void CounterValue(unsigned int Value);
void SetAbsolutZero();
void SetRelativeZero();
void SetZeroActiveMode();
void SwitchMode(); //Absolute or relative
boolean RelativeModeActived(); // true if actived

float GetValue(); //Absolut or reltive, depend of the mode

void SetResolution(unsigned int  Resolution);
void SetSens(boolean Sens);
void ToggleSens();
void SetDiameterMode (boolean DiameterMode);
void ToggleDiameterMode();


private:

void ComputeAbsoluteValue();

unsigned int _CountValue; //Encoder count value
long _AbsoluteCounter; //Absolute counter

long _AbsoluteCounterZero; //Zero Absolu
long _RelativeCounterZero; //Zero Relatif

unsigned int _Resolution; //Resolution par mm
boolean _Sens; //Sens de comptage
boolean _RelativeModeActived; // if true, relative mode else absolute mode
boolean _DiameterMode; // Mode Diameter true or false
int _Overflow; // Comptage overflow or underflow 16bits counter

};
    
#endif
