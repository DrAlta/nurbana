/****************************************************
 * Material List Object
 * Start Date: Mar 25, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Stores attributes for Material List object and
 * associated ID for each
 ****************************************************/


#ifndef MaterialList_H
#define MaterialList_H

#include <stdio.h>
#include <string.h>

class MaterialList {
  public:
    struct MaterialNode {
      int		ID;
      char		Name[256];
      unsigned	char	diff[3];
      unsigned	char	spec[3];
      MaterialNode	*Next;
    }; //eos

    MaterialList::MaterialList();
    MaterialList::~MaterialList();

    int		Create(char *Name);
    void	Create(int ID, char *Name);
    void	Remove(int ID);
    int		GetFreeID();

    unsigned	char*	Diff(int ID);
    unsigned	char*	Spec(int ID);

    char*	Name(int ID);
    void	GetName(int ID, char *Text);
    void	SetName(int ID, char *Text);

    void	ActiveMaterial(int ID) { _ActiveMaterial= ID; }
    int		ActiveMaterial() { return( _ActiveMaterial ); }

    bool	GetHead(unsigned int &ID);
    bool	GetNext(unsigned int &ID);


  private:
    MaterialNode	*_Head;
    unsigned	char	_default_diffuse[3];
    unsigned	char	_default_specularity[3];
    int			_ActiveMaterial;
}; //eof class MaterialList
#endif
