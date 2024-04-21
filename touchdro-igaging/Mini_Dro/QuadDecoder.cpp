#include <Arduino.h>
#include "QuadDecoder.h"

QuadDecoder::QuadDecoder(unsigned int  Resolution , boolean Sens, boolean DiameterMode)
{
  _Resolution = Resolution;
  _Overflow = 0; 
  _AbsoluteCounter = 0;
  _Sens = Sens;
  _AbsoluteCounterZero = 0;
  _RelativeCounterZero = 0;
  _DiameterMode = DiameterMode; 
  _RelativeModeActived = false;
}

void QuadDecoder::OverflowPlus()
{
  _Overflow++;
}

void QuadDecoder::OverflowMinus()
{
  _Overflow--;
}
void QuadDecoder::CounterValue(unsigned int Value)
{
  _CountValue = Value;   
  
  
}

float QuadDecoder::GetValue()
{
  long temp;
  ComputeAbsoluteValue();
  if ( _RelativeModeActived == false)
  {
    //Absolute
    temp = _AbsoluteCounter-_AbsoluteCounterZero;   
  }
  else
  {
    //relative  
    temp = _AbsoluteCounter-_RelativeCounterZero;
  } 
  
  return  (float)temp/_Resolution; 
}
void QuadDecoder::SwitchMode()
{
  _RelativeModeActived = !_RelativeModeActived;  
}
boolean QuadDecoder::RelativeModeActived()
{
  return _RelativeModeActived;   
}

void QuadDecoder::SetAbsolutZero()
{
  ComputeAbsoluteValue();
  _AbsoluteCounterZero = _AbsoluteCounter;  
  
}
void QuadDecoder::SetRelativeZero()
{
  ComputeAbsoluteValue();   
  _RelativeCounterZero = _AbsoluteCounter; 
}
void QuadDecoder::SetZeroActiveMode()
{
  if(_RelativeModeActived == false)SetAbsolutZero();
  else SetRelativeZero();     
}

void QuadDecoder::ComputeAbsoluteValue()
{
   _AbsoluteCounter = _CountValue + _Overflow * Overflow_Size;
  if(_Sens == true)
  {
    _AbsoluteCounter = -_AbsoluteCounter;      
  }
  if(_DiameterMode == true)
  {
    _AbsoluteCounter = _AbsoluteCounter*2;    
  }  
}

void QuadDecoder::SetResolution(unsigned int  Resolution)
{
  _Resolution = Resolution;   
}
void QuadDecoder::SetSens(boolean Sens)
{
  _Sens = Sens;    
}
void QuadDecoder::SetDiameterMode (boolean DiameterMode)
{
  _DiameterMode = DiameterMode;   
}

void QuadDecoder::ToggleSens()
{
  _Sens = !_Sens;  
}
void QuadDecoder::ToggleDiameterMode()
{
  _DiameterMode = !_DiameterMode;  
}

