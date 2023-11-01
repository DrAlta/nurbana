#include "NURBS.h"

Object_List	*NURBS::OBJList;

int NURBS::Perform(unsigned char Action,int N,Object_NURBS **objs,int Args,...) {
  int		*AUXi= NULL,i,int_cnt,flt_cnt,fpt_cnt;
  float		**AUXfpt= NULL;
  va_list	List;


  // Retrieve Arguments
  va_start(List,Args);
  int_cnt= 0;
  flt_cnt= 0;
  fpt_cnt= 0;

  for (i= 0; i < Args; i++) {
    switch( va_arg(List,int) ) {
      case NURBS_TYPE_INT:
        if (int_cnt) {
          AUXi= (int*)realloc(AUXi,sizeof(int)*(int_cnt+1));
        } else {
          AUXi= (int*)malloc(sizeof(int)*2);
        } //fi
        AUXi[int_cnt]= va_arg(List,int);
        int_cnt++;
        break;

      case NURBS_TYPE_FLOAT_PTR:
        if (fpt_cnt) {
          AUXfpt= (float**)realloc(AUXi,sizeof(float)*(fpt_cnt+1));
        } else {
          AUXfpt= (float**)malloc(sizeof(float)*2);
        } //fi
        AUXfpt[fpt_cnt]= va_arg(List,float*);
        fpt_cnt++;
        break;

      default:
        break;
    } //eos
  } //eof
  va_end(List);

  switch( Action ) {
    case NURBS_Generate_Surface:
      NURBS_Generate::Surface(objs[0]);
      break;

    case NURBS_Generate_KnotVector:
      NURBS_Generate::KnotVector(objs[0]);
      break;

    case NURBS_Generate_Cyclic:
      NURBS_Generate::Cyclic(objs[0]);
      break;

    case NURBS_Generate_TrimSurface:
      NURBS_Generate::TrimSurface(objs[0]);
      break;

    case NURBS_Knot_Insert:
      NURBS_Knot::Insert(objs[0],AUXi[0],AUXi[1]);
      break;

    case NURBS_Knot_Remove:
      NURBS_Knot::Remove(objs[0]);
      break;

    case NURBS_Knot_Subdivide:
      NURBS_Knot::Subdivide(objs[0]);
      break;

    case NURBS_Degree_Elevate:
      NURBS_Degree::Elevate(objs[0],AUXi[0],AUXi[1]);
      break;

    case NURBS_Degree_Reduce:
      NURBS_Degree::Reduce(objs[0],AUXi[0],AUXi[1]);
      break;

    case NURBS_Construct_Loft:
      NURBS_Construct::Loft(objs,N,AUXi[0]);
      break;

    case NURBS_Construct_Blend:
      NURBS_Construct::Blend(objs[0],objs[1],objs[2],AUXi[0]);
      break;

    case NURBS_Fitting_GlobalInterp:
      NURBS_Fitting::GlobalInterp(objs[0],AUXi[0],AUXi[1],AUXi[2]);
      break;

    default:
      break;
  } //eos

  // Free Memory
  if (int_cnt) free(AUXi);
  if (fpt_cnt) free(AUXfpt);
  return(0);
} //eof NURBS::Perform()
