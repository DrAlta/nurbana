#include "Endian.h"
/*
template<typename T>
T Endian::ReverseBytes(T v) {
  union type {
    T		var;
    char	byte[sizeof(T)];
  };

  type		data;
  char		swap;

  data.var= v;

  for (char i= 0; i < sizeof(T)/2; i++) {
    swap= data.byte[i];
    data.byte[i]= data.byte[sizeof(T)-i-1];
    data.byte[sizeof(T)-i-1]= swap;
  } //eof

  return(data.var);
} //eof Endian::ReverseBytes()
*/
