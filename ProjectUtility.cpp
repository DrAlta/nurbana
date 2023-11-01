#include "ProjectUtility.h"
#include <string.h>

/*
 * Transform a point (column vector) by a 4x4 matrix.  I.e.  out = m * in
 * Input:  m - the 4x4 matrix
 *         in - the 4x1 vector
 * Output:  out - the resulting 4x1 vector.
 */
static void transform_point(double out[4], const double m[16], const double in[4]) {
  #define M(row,col)  m[col*4+row]
  out[0]= M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
  out[1]= M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
  out[2]= M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
  out[3]= M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
  #undef M
} //eof transform_point()



/*
 * Perform a 4x4 matrix multiplication  (product = a x b).
 * Input:  a, b - matrices to multiply
 * Output:  product - product of a and b
 */
static void matmul(double *product, const double *a, const double *b) {
  /* This matmul was contributed by Thomas Malik */
  double temp[16];
  int i;

  #define A(row,col)  a[(col<<2)+row]
  #define B(row,col)  b[(col<<2)+row]
  #define T(row,col)  temp[(col<<2)+row]

  /* i-te Zeile */
  for (i = 0; i < 4; i++) {
    T(i,0)= A(i,0) * B(0,0) + A(i,1) * B(1,0) + A(i,2) * B(2,0) + A(i,3) * B(3,0);
    T(i,1)= A(i,0) * B(0,1) + A(i,1) * B(1,1) + A(i,2) * B(2,1) + A(i,3) * B(3,1);
    T(i,2)= A(i,0) * B(0,2) + A(i,1) * B(1,2) + A(i,2) * B(2,2) + A(i,3) * B(3,2);
    T(i,3)= A(i,0) * B(0,3) + A(i,1) * B(1,3) + A(i,2) * B(2,3) + A(i,3) * B(3,3);
  } //eof

  #undef A
  #undef B
  #undef T
  MEMCPY(product, temp, 16 * sizeof(double));
} //eof matmul()



/*
 * Compute inverse of 4x4 transformation matrix.
 * Code contributed by Jacques Leroy jle@star.be
 * Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
 */
static bool invert_matrix(const double *m, double *out) {
  /* NB. OpenGL Matrices are COLUMN major. */
  #define SWAP_ROWS(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
  #define MAT(m,r,c) (m)[(c)*4+(r)]

  double wtmp[4][8];
  double m0, m1, m2, m3, s;
  double *r0, *r1, *r2, *r3;

  r0= wtmp[0], r1= wtmp[1], r2= wtmp[2], r3= wtmp[3];

  r0[0] = MAT(m,0,0), r0[1]= MAT(m,0,1),
  r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
  r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
  r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
  r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
  r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
  r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
  r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
  r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
  r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
  r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
  r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

  /* choose pivot - or die */
  if (fabs(r3[0]) > fabs(r2[0]))
    SWAP_ROWS(r3, r2);
  if (fabs(r2[0]) > fabs(r1[0]))
    SWAP_ROWS(r2, r1);
  if (fabs(r1[0]) > fabs(r0[0]))
    SWAP_ROWS(r1, r0);
  if (0.0 == r0[0])
    return PU_FALSE;

  /* eliminate first variable     */
  m1= r1[0] / r0[0];
  m2= r2[0] / r0[0];
  m3= r3[0] / r0[0];
  s= r0[1];
  r1[1]-= m1 * s;
  r2[1]-= m2 * s;
  r3[1]-= m3 * s;
  s= r0[2];
  r1[2]-= m1 * s;
  r2[2]-= m2 * s;
  r3[2]-= m3 * s;
  s= r0[3];
  r1[3]-= m1 * s;
  r2[3]-= m2 * s;
  r3[3]-= m3 * s;
  s= r0[4];
  if (s != 0.0) {
    r1[4]-= m1 * s;
    r2[4]-= m2 * s;
    r3[4]-= m3 * s;
  } //fi
  s= r0[5];
  if (s != 0.0) {
    r1[5]-= m1 * s;
    r2[5]-= m2 * s;
    r3[5]-= m3 * s;
  } //fi
  s= r0[6];
  if (s != 0.0) {
    r1[6]-= m1 * s;
    r2[6]-= m2 * s;
    r3[6]-= m3 * s;
  } //fi
  s= r0[7];
  if (s != 0.0) {
    r1[7]-= m1 * s;
    r2[7]-= m2 * s;
    r3[7]-= m3 * s;
  } //fi

  /* choose pivot - or die */
  if (fabs(r3[1]) > fabs(r2[1]))
    SWAP_ROWS(r3, r2);
  if (fabs(r2[1]) > fabs(r1[1]))
    SWAP_ROWS(r2, r1);
  if (0.0 == r1[1])
    return PU_FALSE;

  /* eliminate second variable */
  m2= r2[1] / r1[1];
  m3= r3[1] / r1[1];
  r2[2]-= m2 * r1[2];
  r3[2]-= m3 * r1[2];
  r2[3]-= m2 * r1[3];
  r3[3]-= m3 * r1[3];
  s= r1[4];
  if (0.0 != s) {
    r2[4] -= m2 * s;
    r3[4] -= m3 * s;
  } //fi
  s= r1[5];
  if (0.0 != s) {
    r2[5] -= m2 * s;
    r3[5] -= m3 * s;
  } //fi
  s= r1[6];
  if (0.0 != s) {
     r2[6] -= m2 * s;
     r3[6] -= m3 * s;
  }
  s= r1[7];
  if (0.0 != s) {
    r2[7] -= m2 * s;
    r3[7] -= m3 * s;
  } //fi

  /* choose pivot - or die */
  if (fabs(r3[2]) > fabs(r2[2]))
    SWAP_ROWS(r3, r2);
  if (0.0 == r2[2])
    return PU_FALSE;

  /* eliminate third variable */
  m3= r3[2] / r2[2];
  r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
  r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

   /* last check */
  if (0.0 == r3[3])
    return PU_FALSE;

  s= 1.0 / r3[3];		/* now back substitute row 3 */
  r3[4] *= s;
  r3[5] *= s;
  r3[6] *= s;
  r3[7] *= s;

  m2= r2[3];			/* now back substitute row 2 */
  s= 1.0 / r2[2];
  r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2), r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
  m1= r1[3];
   
  r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1, r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
  m0= r0[3];

  r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0, r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

  m1= r1[2];			/* now back substitute row 1 */
  s= 1.0 / r1[1];
  r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1), r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);

  m0= r0[2];
  r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0, r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

  m0= r0[1];			/* now back substitute row 0 */
  s= 1.0 / r0[0];
  r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0), r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

  MAT(out, 0, 0) = r0[4];
  MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
  MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
  MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
  MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
  MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
  MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
  MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
  MAT(out, 3, 3) = r3[7];

  return PU_TRUE;

  #undef MAT
  #undef SWAP_ROWS
} //eof invert_matrix()



/* projection du point (objx,objy,obz) sur l'ecran (winx,winy,winz) */
bool ProjectUtility::Project(double objx, double objy, double objz,
	   const double model[16], const double proj[16],
	   const int viewport[4],
	   double *winx, double *winy, double *winz) {
  /* matrice de transformation */
  double in[4], out[4];

  /* initilise la matrice et le vecteur a transformer */
  in[0]= objx;
  in[1]= objy;
  in[2]= objz;
  in[3]= 1.0;
  transform_point(out, model, in);
  transform_point(in, proj, out);

  /* d'ou le resultat normalise entre -1 et 1 */
  if (in[3] == 0.0)
    return PU_FALSE;

  in[0] /= in[3];
  in[1] /= in[3];
  in[2] /= in[3];

  /* en coordonnees ecran */
  *winx= viewport[0] + (1 + in[0]) * viewport[2] / 2;
  *winy= viewport[1] + (1 + in[1]) * viewport[3] / 2;
  /* entre 0 et 1 suivant z */
  *winz= (1 + in[2]) / 2;
  return PU_TRUE;
} //eof ProjectUtility::Project();



/* transformation du point ecran (winx,winy,winz) en point objet */
bool ProjectUtility::UnProject(double winx, double winy, double winz,
	     const double model[16], const double proj[16],
	     const int viewport[4],
	     double *objx, double *objy, double *objz) {
  /* matrice de transformation */
  double m[16], A[16];
  double in[4], out[4];

  /* transformation coordonnees normalisees entre -1 et 1 */
  in[0]= (winx - viewport[0]) * 2 / viewport[2] - 1.0;
  in[1]= (winy - viewport[1]) * 2 / viewport[3] - 1.0;
  in[2]= 2 * winz - 1.0;
  in[3]= 1.0;

  /* calcul transformation inverse */
  matmul(A, proj, model);
  invert_matrix(A, m);

  /* d'ou les coordonnees objets */
  transform_point(out, m, in);
  if (out[3] == 0.0)
    return PU_FALSE;
  *objx= out[0] / out[3];
  *objy= out[1] / out[3];
  *objz= out[2] / out[3];
  return PU_TRUE;
} //eof ProjectUtility::UnProject()


void ProjectUtility::PickMatrix(double x, double y, double width, double height, const int viewport[4]) {
   double	m[16];
   double	sx, sy;
   double	tx, ty;

   sx= viewport[2] / width;
   sy= viewport[3] / height;
   tx= (viewport[2] + 2.0 * (viewport[0] - x)) / width;
   ty= (viewport[3] + 2.0 * (viewport[1] - y)) / height;

#define M(row,col)  m[col*4+row]
   M(0, 0) = sx;
   M(0, 1) = 0.0;
   M(0, 2) = 0.0;
   M(0, 3) = tx;
   M(1, 0) = 0.0;
   M(1, 1) = sy;
   M(1, 2) = 0.0;
   M(1, 3) = ty;
   M(2, 0) = 0.0;
   M(2, 1) = 0.0;
   M(2, 2) = 1.0;
   M(2, 3) = 0.0;
   M(3, 0) = 0.0;
   M(3, 1) = 0.0;
   M(3, 2) = 0.0;
   M(3, 3) = 1.0;
#undef M

   glMultMatrixd(m);
} //eof PickMatrix()
