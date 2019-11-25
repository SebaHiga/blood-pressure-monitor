#include "filter.h"

/*
    Fs = 500
    Order = 2
    fc = 10
*/
uint16_t iirFilterLP (uint16_t data)
{
  uint16_t i;
  static uint16_t x[orderFilterLP] = {0};
  static uint16_t y[orderFilterLP] = {0};
  
  const float b [] = {
       0.003621681515,  0.00724336303 ,  0.003621681515};
       
  const float a [] = {
        1.            , -1.822694925196,  0.837181651256};
  
  uint16_t filtered;

  filtered =  b[0] * (float)data + b[1] * x[0] + b[2] * x[1]//
                                 - a[1] * y[0] - a[2] * y[1];//

  if(filtered > 32767) filtered = 450;  

  for(i = orderFilterLP-1; i!=0; i--)
  {
    x[i] = x[i-1];
  }
  x[0]=(float)data;

  for(i = orderFilterLP-1; i!=0; i--)
  {
    y[i] = y[i-1];
  }
  y[0]=filtered;

  return filtered;
}

/*
    Fs = 500
    Order = 2
    fc = 10
*/
int iirFilterHP (uint16_t data)
{
  uint16_t i;
  static int x[orderFilterLP] = {0};
  static int y[orderFilterLP] = {0};
  
  const float b [] = {
       0.056391241094,  0.            , -0.056391241094};
       
  const float a [] = {
        1.            , -1.886471637182,  0.887217517812};
  
  int filtered;

  filtered =  b[0] * (float)data + b[1] * x[0] + b[2] * x[1]//
                                 - a[1] * y[0] - a[2] * y[1];//

  if(filtered > 32767) filtered = 450;  

  for(i = orderFilterLP-1; i!=0; i--)
  {
    x[i] = x[i-1];
  }
  x[0]=(float)data;

  for(i = orderFilterLP-1; i!=0; i--)
  {
    y[i] = y[i-1];
  }
  y[0]=filtered;

  return filtered;
}
