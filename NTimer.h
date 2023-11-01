#ifndef NTimer_H
#define NTimer_H

#include <stdio.h>
#include <time.h>


class NTimer {
  public:
    static	void	Start();
    static	void	Stop();
    static	float	Report(bool T);

}; //eof NTimer Class
#endif
