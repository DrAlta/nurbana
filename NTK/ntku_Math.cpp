#include "ntku_Math.h"



int ntku_Math::NumMag(float Num) {
  //Calculates the magnitude of any number
  int digits=0;
          
  while (Num >= 1) {
    digits++;
    Num/=10.0;
  } // eof
 
  return(digits);
} //eof NumMag
