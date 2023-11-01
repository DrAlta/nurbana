// ----------------------
// --- OPERATOR STUFF ---
// ----------------------
template <class Type>
Matrix44<Type> Matrix44<Type>::operator * (Matrix44<Type> A) {
  Matrix44<Type>	NewM;

  M[0] = M[0]*A.M[0] + M[4]*A.M[1] + M[8]*A.M[2] + M[12]*A.M[3];
  M[1] = M[0]*A.M[4] + M[4]*A.M[5] + M[8]*A.M[6] + M[12]*A.M[7];
  M[2] = M[0]*A.M[8] + M[4]*A.M[9] + M[8]*A.M[10] + M[12]*A.M[11];
  M[3] = M[0]*A.M[12] + M[4]*A.M[13] + M[8]*A.M[14] + M[12]*A.M[15];
  M[4] = M[1]*A.M[0] + M[5]*A.M[1] + M[9]*A.M[2] + M[13]*A.M[3];
  M[5] = M[1]*A.M[4] + M[5]*A.M[5] + M[9]*A.M[6] + M[13]*A.M[7];
  M[6] = M[1]*A.M[8] + M[5]*A.M[9] + M[9]*A.M[10] + M[13]*A.M[11];
  M[7] = M[1]*A.M[12] + M[5]*A.M[13] + M[9]*A.M[14] + M[13]*A.M[15];
  M[8] = M[2]*A.M[0] + M[6]*A.M[1] + M[10]*A.M[2] + M[14]*A.M[3];
  M[9] = M[2]*A.M[4] + M[6]*A.M[5] + M[10]*A.M[6] + M[14]*A.M[7];
  M[10]= M[2]*A.M[8] + M[6]*A.M[9] + M[10]*A.M[10] + M[14]*A.M[11];
  M[11]= M[2]*A.M[12] + M[6]*A.M[13] + M[10]*A.M[14] + M[14]*A.M[15];
  M[12]= M[3]*A.M[0] + M[7]*A.M[1] + M[11]*A.M[2] + M[15]*A.M[3];
  M[13]= M[3]*A.M[4] + M[7]*A.M[5] + M[11]*A.M[6] + M[15]*A.M[7];
  M[14]= M[3]*A.M[8] + M[7]*A.M[9] + M[11]*A.M[10] + M[15]*A.M[11];
  M[15]= M[3]*A.M[12] + M[7]*A.M[13] + M[11]*A.M[14] + M[15]*A.M[15];

  return(NewM);
} //eof Matrix44<Type>::operator *



template <class Type>
Vector3<Type> Matrix44<Type>::operator * (Vector3<Type> V) {
  Type		W;
  Vector3<Type>	NewV;

  W= M[3]*V.X + M[7]*V.Y + M[11]*V.Z + M[15];
  NewV.Set( (M[0]*V.X + M[4]*V.Y + M[8]*V.Z  + M[12]) / W,
            (M[1]*V.X + M[5]*V.Y + M[9]*V.Z  + M[13]) / W,
            (M[2]*V.X + M[6]*V.Y + M[10]*V.Z + M[14]) / W
          );

  return(NewV);
} //eof Matrix44<Type>::operator *



// -----------------------
// --- MATRIX FUNCIONS ---
// -----------------------
template <class Type>
void Matrix44<Type>::Identity() {
  M[0]=  1;   M[1]= 0;   M[2]= 0;   M[3]= 0;
  M[4]=  0;   M[5]= 1;   M[6]= 0;   M[7]= 0;
  M[8]=  0;   M[9]= 0;  M[10]= 1;  M[11]= 0;
  M[12]= 0;  M[13]= 0;  M[14]= 0;  M[15]= 1;
} //eof Matrix44<Type>::Identity()


template <class Type>
void Matrix44<Type>::Transpose() {
  Swap(M[1],M[4]);
  Swap(M[2],M[8]);
  Swap(M[6],M[9]);
  Swap(M[3],M[12]);
  Swap(M[7],M[13]);
  Swap(M[11],M[14]);
} //eof Matrix44<Type>::Transpose()


template <class Type>
void Matrix44<Type>::Invert() {
  int			i, j, k;
  Matrix44<Type>	Out;
  Out.Identity();  

  for (i= 0; i < 4; i++) {
    // Divide each column by it's diagonal element if not equal
    // to one.  If Diagonal element 0 then divide each element by one.
    if (M[i + i*4] != 1.0) {
      Type divby= M[i + i*4];
        if (!divby) divby= 1;
      for (j= 0; j < 4; j++) {
        Out.M[i + j*4]/= divby;
        M[i + j*4]/= divby;
      } //eof
    } //fi


    // For non-diagonal components, for the i-th current row
    // if the element is not equal to zero then take each column
    // component along that row and subtract...
    for (j= 0; j < 4; j++) {
      if (j != i) {
        if (M[j + i*4] != 0.0) {
          Type mulby= M[j + i*4];
          for (k= 0; k < 4; k++) {
            M[j + k*4]-= mulby * M[i + k*4];
            Out.M[j + k*4]-= mulby * Out.M[i + k*4];
          } //eof
        } //fi
      } //fi
    } //eof
  } //eof

  for (i= 0; i < 16; i++) M[i]= Out.M[i];
} //eof Matrix44<Type::Invert()


template <class Type>
void Matrix44<Type>::Rotate(Type Angle, Vector3<Type> Axis) {
  Type	RadAng,ca,sa;

  RadAng= Angle*Matrix44ToRads;
  ca= (Type)cos(RadAng);
  sa= (Type)sin(RadAng);

  if (Axis.X == 1 && Axis.Y == 0 && Axis.Z == 0) { // ABOUT X-AXIS
    M[0]= 1; M[4]= 0;  M[8]= 0;   M[12]= 0;
    M[1]= 0; M[5]= ca; M[9]= -sa; M[13]= 0;
    M[2]= 0; M[6]= sa; M[10]= ca; M[14]= 0;
    M[3]= 0; M[7]= 0;  M[11]= 0;  M[15]= 1;
  } //fi

  if (Axis.X == 0 && Axis.Y == 1 && Axis.Z == 0) { // ABOUT Y-AXIS
    M[0]= ca;  M[4]= 0;  M[8]= sa;  M[12]= 0;
    M[1]= 0;   M[5]= 1;  M[9]= 0;   M[13]= 0;
    M[2]= -sa; M[6]= 0;  M[10]= ca; M[14]= 0;
    M[3]= 0;   M[7]= 0;  M[11]= 0;  M[15]= 1;
  } //fi

  if (Axis.X == 0 && Axis.Y == 0 && Axis.Z == 1) { // ABOUT Z-AXIS
    M[0]= ca; M[4]= -sa; M[8]= 0;  M[12]= 0;
    M[1]= sa; M[5]= ca;  M[9]= 0;  M[13]= 0;
    M[2]= 0;  M[6]= 0;  M[10]= 1;  M[14]= 0;
    M[3]= 0;  M[7]= 0;  M[11]= 0;  M[15]= 1;

  } else {  // ARBITRARY AXIS
    Type	l= Axis.LengthSqr();
    Type	x,y,z;

    x= Axis.X;
    y= Axis.Y;
    z= Axis.Z;

    if (l > Type(1.0001) || l < Type(0.9999) && l!= 0) {
      // needs normalization
      l= Type(1.0)/sqrt(l);
      x*= l;
      y*= l;
      z*= l;
    } //fi

    Type	x2,y2,z2;
    x2= x*x;
    y2= y*y;
    z2= z*z;

    M[0]= x2+ca*(1-x2);
    M[4]= (x*y)+ca*(-x*y)+sa*(-z);
    M[8]= (x*z)+ca*(-x*z)+sa*y;
    M[1]= (x*y)+ca*(-x*y)+sa*z;
    M[5]= y2+ca*(1-y2);
    M[9]= (y*z)+ca*(-y*z)+sa*(-x);
    M[2]= (x*z)+ca*(-x*z)+sa*(-y);
    M[6]= (y*z)+ca*(-y*z)+sa*x;
    M[10]= z2+ca*(1-z2);
    M[12]= M[13]= M[14]= M[3]= M[7]= M[11]= 0;
    M[15]= 1;
  } //fi


} //eof Matrix44<Type::Rotate()
