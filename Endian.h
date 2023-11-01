#ifndef Endian_H
#define Endian_H

//template<class T>
class Endian {
  public:

    union EV {
      unsigned	int	i;
      unsigned	char	c;
    };


    // Reverse Bytes Function
    template<typename T>
    static void RB(T &v) {
      union type {
        T	var;
        char	byte[sizeof(T)];
      };

      type	data;
      char	swap;

      data.var= v;

      for (char i= 0; i < sizeof(T)/2; i++) {
        swap= data.byte[i];
        data.byte[i]= data.byte[sizeof(T)-i-1];
        data.byte[sizeof(T)-i-1]= swap;
      } //eof

      v= data.var;
//    return(data.var);
    } //eof Endian::RB()

}; //eoc Endian
#endif
