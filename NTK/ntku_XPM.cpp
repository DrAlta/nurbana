#include "ntku_XPM.h"

void ntku_XPM::getXPMheader(unsigned char **temp, int data[4]) {
 int i,k,j,m;   //Iteration Variables
 char buffer[BUF_SIZE]; //Up to 8 digits long

 /*** OBTAIN XPM/XBM FORMAT INFO FROM HEADER ***/
 k= 0; m= 0;
 for (i= 0; i < int(strlen((char*)temp[0])); i++) {
   if (temp[0][i] != 32) {
     buffer[k]= temp[0][i];
     k++;
   } else {
     k= 0;
     data[m]= atoi(buffer); m++;
     for (j=0; j < BUF_SIZE; j++) buffer[j]= 32; //PURGE BUFFER
   } //fi
 } //eof
 data[m]= atoi(buffer);
 /*********************************/
} //eof getXPMheader



void ntku_XPM::getXPMColor(unsigned char **temp, int data[4], char ColString[8], char rgb[3]) {
  int				i,n,k,match;
  char				Buffer[8],test;
  unsigned	char		tempB,value;


    /*** CYCLE THROUGH EACH LINE OF COLOR TABLE ***/
    for (i= 2; i <= data[2]; i++) { //start at line 2 cause NONE is wierd
      /*** EXTRACT COLOR STRING ***/
      for (n= 0; n < data[3]; n++) Buffer[n]= temp[i][n];
      /*** CHECK TO SEE IF THERE IS A MATCH ***/
      match= 1;
      for (n= 0; n < data[3]; n++) {
        if (!(Buffer[n] == ColString[n])) {
          match= 0;
	} //fi
      } //eof
      /*** IF THERE IS A MATCH THEN GET COLOR _CODE_ AND CONVERT TO HEX ***/
      if (match) {
        if (i == -1) { // Set to i == 2 to enable
          // This is used for quasi-transparency
//  	  rgb[0]= Color[0];
// 	  rgb[1]= Color[1];
//	  rgb[2]= Color[2];
	} else { 
          // GET COLOR CODE by 2 and convert
          for (k= 0; k < 3; k++) { // R,G,B
          tempB= 0;
            for (n= 0; n < 2; n++) {
              test= temp[i][data[3]+4+(k*2)+n]; // data[3]+<TAB>+c #
              if (test==48) {
                value= 0x0;
	      } else if (test==49) {
                value= 0x1;
	      } else if (test==50) {
                value= 0x2;
	      } else if (test==51) {
                value= 0x3;
	      } else if (test==52) {
                value= 0x4;
	      } else if (test==53) {
                value= 0x5;
	      } else if (test==54) {
                value= 0x6;
	      } else if (test==55) {
                value= 0x7;
	      } else if (test==56) {
                value= 0x8;
	      } else if (test==57) {
                value= 0x9;
	      } else if (test==65) {
                value= 0xa;
	      } else if (test==66) {
                value= 0xb;
	      } else if (test==67) {
                value= 0xc;
	      } else if (test==68) {
                value= 0xd;
	      } else if (test==69) {
                value= 0xe;
	      } else if (test==70) {
                value= 0xf;
	      } //fi
              if (!n) {
                tempB= 0x10*value;
	      } else {
                tempB+= value;
	      } //fi
	    } //eof

            rgb[k]= tempB;
	  } //eof
	} //fi transparency
      } //fi (match)
    } //eof
} //eof getXPMColor()
