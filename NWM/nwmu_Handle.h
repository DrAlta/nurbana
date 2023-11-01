#ifndef nwmu_Handle_H
#define nwmu_Handle_H

//#include "../GLUT/glut.h"
#include "../InvertBuffer.h"

class nwmu_Handle {
  public:
    static	void	Initialize(int VP1, int VP2, int VP3, int VP4, int Pos, int Siz, bool _VH);
    static	void	Update(int x, int y);

  private:
    static	void	SetView();
}; //eof nwmu_Handle Class
#endif
