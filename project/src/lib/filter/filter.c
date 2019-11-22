#include "filter.h"


// #define   coefHP    2

// float iirFilterHP (uint16_t data)
// {
//   uint16_t i;
//   static float x[coefHP] = {0};
//   static float y[coefHP] = {0};
  
//   const float b [] = {
//         0.9329321560713876, -1.8658643121427751,  0.9329321560713876};
       
//   const float a [] = {
//         1.                , -1.8613611468290827,  0.8703674774564693};
  
//   float filtered;

//   filtered =  b[0] * (float)data + b[1] * x[0] + b[2] * x[1]
//                                  - a[1] * y[0] - a[2] * y[1];
  
//   for(i = coefHP-1; i!=0; i--)
//   {
//     x[i] = x[i-1];
//   }
//   x[0]=(float)data;

//   for(i = coefHP-1; i!=0; i--)
//   {
//     y[i] = y[i-1];
//   }
//   y[0]=filtered;

//   return filtered;
// }

#define orderFilterLP 2

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

