#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H

#include "globals.h"

long  My_Power(int Base, int Power)
{
  {
    // Method 1 (Using Nested Loops)
    // We can calculate power by using repeated addition.
    // For example to calculate 5^6.
    // 1) First 5 times add 5, we get 25. (5^2)
    // 2) Then 5 times add 25, we get 125. (5^3)
    // 3) Then 5 time add 125, we get 625 (5^4)
    // 4) Then 5 times add 625, we get 3125 (5^5)
    // 5) Then 5 times add 3125, we get 15625 (5^6)
    
    if (Power == 0)
    return 1; 
    
    long answer = Base;
    long increment = Base;
    int i, j;
    for(i = 1; i < Power; i++)
    {
      for(j = 1; j < Base; j++)
      {
        answer += increment;
      }
      increment = answer;
    }
    return answer;
  } 
}//=============================================

long  Buff2Decimal()
{
  long  result = 0;
  
  
  for ( int i=0; i<=INDEX_DIGITS; i++ )
  {
    result = result + Buffer[i] * (long)My_Power(2,i);
  }
  
  if ( Buffer[20] ) {Znak = -1;}
  else        {Znak = +1;}

  return Znak * result;
}//=============================================


void CaliperSetup( int clkPin, int dataPin){
  dialClkPin = clkPin;
  dialDataPin = dataPin;
  pinMode(clkPin,   INPUT); 
  pinMode(dataPin,  INPUT); 
  
  CurrentMillis   = millis();
  Timer       = millis();

}



long CaliperLoop(){
  long CaliperValue = -1;
  b_CLK_IS  = digitalRead(dialClkPin);   // read the CLK pin
  
  if (b_CLK_WAS==LOW  &&   b_CLK_IS==HIGH)// there is clk rising edge
  {
    b_DATA_IS = digitalRead(dialDataPin);   // read the DATA pin
    
    CurrentMillis   = millis();// we read sys time here
    
    if (CurrentMillis - Timer >= WAIT_TIME)// time out check  
    {//time passed is longer than time_out  >>  It is FIRST pulse  in the new package
      // to reset the buffer
      Index_Now = 0;      // to Set  buff index N = 0
    }
    
    Buffer[Index_Now]= b_DATA_IS;

    if (Index_Now >= INDEX_MAX)
    {// the buffer is full - time to process the data
      //###############################
      //Serial.println("Save");
      CaliperValue = Buff2Decimal();//to proceed the data in the buffer
      Serial.println(CaliperValue);
      //###############################
      
      // to reset the buffer
      Index_Now = 0;// to Set  buff index N = 0
      //Serial.println("  ");//Serial.println("last pulse");
    }
    
    Index_Now++;// preps for next pulse

    Timer = CurrentMillis ;// to restart timer
  }

  
  b_CLK_WAS = b_CLK_IS;// preps for next cycle
  return CaliperValue;
}

#endif
