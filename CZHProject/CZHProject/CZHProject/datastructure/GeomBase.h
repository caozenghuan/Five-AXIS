#pragma once


using namespace std;
#include <vector>
#include <map>
#include <set>
#include <math.h>
#include <Shlobj.h>
#include <QtGlobal>



#define CAD_ZERO		1.0E-6
#define NC_ZERO		1.0E-3
#define IS_ZERO(x)		(fabs(x)<=CAD_ZERO)
#define IS_NCZERO(x)		(fabs(x)<=NC_ZERO)
#define IS_BETWEEN(x,min,max) (x<=max && x>=min)
#define PI	3.1415926535
#define TOLERANCE  1.0E-4

struct _Matrix2D{double A[3][3];};
struct _Matrix3D{double A[4][4];};

typedef struct tagVector3D{
	double dx;
	double dy;
	double dz;

	tagVector3D()
	{
		dx = 0.0;
		dy = 0.0;
		dz = 0.0;
	};
	tagVector3D( const tagVector3D & ori)
	{
		dx = ori.dx;
		dy = ori.dy;
		dz = ori.dz;
	};
	tagVector3D( const double*pv)
	{
		dx = pv[0];
		dy = pv[1];
		dz = pv[2];
	};
	tagVector3D(double x, double y, double z)
	{
		dx = x;
		dy = y;
		dz = z;
	};

	//////////////////////////////////////////////////////////////////////////
public:
	//operator
	tagVector3D operator+(tagVector3D v) const{return tagVector3D(dx+v.dx,dy+v.dy,dz+v.dz);};
	void operator+=(tagVector3D v){	dx += v.dx;	dy += v.dy;	dz += v.dz;};
	tagVector3D operator-(tagVector3D v) const{return tagVector3D(dx-v.dx,dy-v.dy,dz-v.dz);};
	void operator-=(tagVector3D v){	dx -= v.dx;	dy -= v.dy;	dz -= v.dz;};

	tagVector3D operator*(double d) const{return tagVector3D(dx*d,dy*d,dz*d);};
	void operator*=(double d){	dx *= d;	dy *= d;	dz *= d;};
	tagVector3D operator/(double d) const{return tagVector3D(dx/d,dy/d,dz/d);	};
	void operator/=(double d)	{	dx /= d;dy /= d;dz /= d;	};

	//used in geometrical calculation
	//double operator*(POINT3D pnt){return (dx*pnt.x + dy*pnt.y + dz*pnt.z);	};
	// cross product
	tagVector3D operator*(tagVector3D v) const{	return tagVector3D(dy*v.dz-v.dy*dz,v.dx*dz-dx*v.dz,dx*v.dy-v.dx*dy);};
	// dot product
	double operator|(tagVector3D v) const{return dx*v.dx+dy*v.dy+dz*v.dz;};

	tagVector3D operator*(const _Matrix3D& matrix) const
	{
		double rx,ry,rz,sc;
		rx = dx * matrix.A[0][0] + dy * matrix.A[1][0] + dz * matrix.A[2][0] + matrix.A[3][0];
		ry = dx * matrix.A[0][1] + dy * matrix.A[1][1] + dz * matrix.A[2][1] + matrix.A[3][1];
		rz = dx * matrix.A[0][2] + dy * matrix.A[1][2] + dz * matrix.A[2][2] + matrix.A[3][2];
		sc = dx * matrix.A[0][3] + dy * matrix.A[1][3] + dz * matrix.A[2][3] + matrix.A[3][3];
		rx /= sc;
		ry /= sc;
		rz /= sc;
		return tagVector3D(rx,ry,rz);
	};
	void  operator*=(const _Matrix3D& matrix){(*this) = (*this)*matrix;	};

	//length
	double GetLength() const{return sqrt(dx*dx+dy*dy+dz*dz);};

	double GetLengthXY() const	{return sqrt(dx*dx+dy*dy);	};

	double GetLengthYZ() const	{return sqrt(dy*dy+dz*dz);	};

	double GetLengthZX() const	{return sqrt(dx*dx+dz*dz);	};

	tagVector3D GetNormal() const
	{
		double len = GetLength();
		return tagVector3D(dx/len,dy/len,dz/len);
	};

	void Normalize()
	{
		double len = GetLength();
		dx /= len;
		dy /= len;
		dz /= len;
	};

	BOOL IsZeroLength() const{return IS_ZERO(GetLength());	};

} VECTOR3D,*PVECTOR3D;

typedef struct tagVector2D{
	double dx;
	tagVector2D()
	{
		dx = 0.0;
		dy = 0.0;
	};
	double dy;

	tagVector2D(double x, double y)
	{
		dx = x;
		dy = y;
	};

	tagVector2D(const double* pv)
	{
		dx = pv[0];
		dy = pv[1];
	};

	tagVector2D(const tagVector2D& v)
	{
		dx = v.dx;
		dy = v.dy;
	};


	//////////////////////////////////////////////////////////////////////////
public:
	//operators declaration
	tagVector2D operator+(tagVector2D v) const { return tagVector2D(dx+v.dx,dy+v.dy);};
	tagVector2D operator-(tagVector2D v) const{ return tagVector2D(dx-v.dx,dy-v.dy);};
	void operator+=(tagVector2D v){dx += v.dx; dy += v.dy;	};
	void operator-=(tagVector2D v){		dx-=v.dx;		dy-=v.dy;	};
	tagVector2D operator*(double d) const{ return tagVector2D(dx*d,dy*d);};
	void operator*=(double d){	dx *= d;	dy *= d;};
	tagVector2D operator/(double d) const{return tagVector2D(dx/d,dy/d);};
	void operator/=(double d){	dx /= d; dy /= d;	};


	// cross product
	tagVector3D operator*(tagVector2D v) const{ return tagVector3D(0.0,0.0,dx*v.dy-v.dx*dy);};

	// dot product
	double operator|(tagVector2D v) const{return dx*v.dx+dy*v.dy;};

	//matrix transformation
	tagVector2D operator*(const _Matrix2D& matrix ) const
	{	
		double x,y,sc;
		x = dx*matrix.A[0][0]+dy*matrix.A[1][0] + matrix.A[2][0];
		y = dx*matrix.A[0][1]+dy*matrix.A[1][1] + matrix.A[2][1];
		sc= dx*matrix.A[0][2]+dy*matrix.A[1][2] + matrix.A[2][2];
		x /= sc;
		y /= sc;
		return tagVector2D(x,y);
	};
	void operator*=(const _Matrix2D& matrix ){(*this)=(*this)*matrix;};

	//methods declaration
	double GetLength()const{return sqrt(dx*dx+dy*dy);};
	tagVector2D GetNormal()const{	double len = GetLength();	return tagVector2D(dx/len,dy/len);};
	void Normalize()
	{
		double len = GetLength();
		dx = dx/len;
		dy = dy/len;
	};
	BOOL   IsZeroLength() const{return IS_ZERO(GetLength());};


} VECTOR2D,*PVECTOR2D;



/*typedef */struct tagMatrix2D:public _Matrix2D{
	//double A[3][3];

	tagMatrix2D()	{
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++){
				if(i==j)
					A[i][j] = 1.0;
				else
					A[i][j] = 0.0;
			}
	};

	tagMatrix2D(const tagMatrix2D& matrix)	{
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
				A[i][j]=matrix.A[i][j];
	};

	tagMatrix2D(const double* matrix)	{
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
				A[i][j]=matrix[i*3+j];
	};

	//operators
	tagMatrix2D operator*(const tagMatrix2D& matrix2) const	{
		tagMatrix2D matrix;
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++){
				matrix.A[i][j] = A[i][0]*matrix2.A[0][j]
				+ A[i][1]*matrix2.A[1][j]
				+ A[i][2]*matrix2.A[2][j];
			}
			return matrix;
	};

	void operator*=(const tagMatrix2D& matrix2)	{(*this)=(*this)*matrix2;};

	//methods
	void IdenticalMatrix()	{
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++){
				if(i==j)
					A[i][j] = 1.0;
				else
					A[i][j] = 0.0;
			}
	};

	double GetValue() const	{return A[0][0]*A[1][1]-A[0][1]*A[1][0];};

	// static member functions
	static double GetValue(double a00, double a01,	double a10, double a11)	{
		return a00*a11-a01*a10;
	};

	static tagMatrix2D CreateMirrorMatrix(VECTOR2D v)	{
		double len=v.GetLength();
		tagMatrix2D matrix;
		matrix.A[0][0]=(v.dx*v.dx - v.dy*v.dy)/len;
		matrix.A[0][1]=2.0*(v.dx*v.dy)/len;
		matrix.A[1][0]=2.0*(v.dx*v.dy)/len;
		matrix.A[1][1]= -matrix.A[0][0];
		return matrix;
	};

	static tagMatrix2D CreateRotateMatrix(double angle)	{
		tagMatrix2D m;
		m.A[0][0]=m.A[1][1]=cos(angle);
		m.A[0][1]=sin(angle);m.A[1][0]= -sin(angle);
		return m;
	};

	static tagMatrix2D CreateScaleMatrix(double d)	{
		tagMatrix2D m;
		m.A[0][0]=m.A[1][1]=d;
		return m;
	};

	static tagMatrix2D CreateTransfMatrix(VECTOR2D vec)	{
		tagMatrix2D m;
		m.A[2][0]=vec.dx;
		m.A[2][1]=vec.dy;
		return m;
	};
};//MATRIX2D, *PMATRIX2D;;
typedef  tagMatrix2D MATRIX2D, *PMATRIX2D;

/*typedef*/ struct  tagMatrix3D:public _Matrix3D{
	//double A[4][4];

	//construction&&destruction
	tagMatrix3D()	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				A[i][j] = (i==j)?1.0:0.0;
			}
	};

	tagMatrix3D(const tagMatrix3D& matrix)	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				A[i][j] = matrix.A[i][j];
			}
	};

	tagMatrix3D(const double *matrix)	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				A[i][j] = matrix[i*4+j];
			}
	};
	//operators
	tagMatrix3D operator*(const tagMatrix3D& matrix2) const	{ 
		tagMatrix3D matrix;
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++){
				matrix.A[i][j] = A[i][0]*matrix2.A[0][j]
				+ A[i][1]*matrix2.A[1][j]
				+ A[i][2]*matrix2.A[2][j]
				+ A[i][3]*matrix2.A[3][j];
			}
			return matrix;
	};

	void operator*=(const tagMatrix3D& matrix){	(*this) = (*this)*matrix;	};

	//methods
	void IdenticalMatrix()	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				A[i][j] = (i==j)?1.0:0.0;
			}
	};

	double GetValue() const	{
		return  A[0][0]*A[1][1]*A[2][2] +
			A[0][1]*A[1][2]*A[2][0] +
			A[0][2]*A[1][0]*A[2][1] -
			A[0][2]*A[1][1]*A[2][0] -
			A[0][1]*A[1][0]*A[2][2] -
			A[0][0]*A[1][2]*A[2][1];
	};

	// static member functions
	static double GetValue(double a00, double a01, double a02,
		double a10, double a11, double a12,
		double a20, double a21, double a22)
	{
		return  a00*a11*a22 +
			a01*a12*a20 +
			a02*a10*a21 -
			a02*a11*a20 -
			a01*a10*a22 -
			a00*a12*a21;
	};

	static tagMatrix3D CreateMirrorMatrix(VECTOR3D v)
	{
		//double len=((CVector3D)v).GetLength();
		
		//matrix.A[0][0]= (v.dx*v.dx -1.0)*2.0/len/len;
		//matrix.A[1][1]= (v.dy*v.dy -1.0)*2.0/len/len;
		//matrix.A[2][2]= (v.dz*v.dz -1.0)*2.0/len/len;
		//matrix.A[0][1]=matrix.A[1][0]= v.dx*v.dy*2.0/len/len;
		//matrix.A[0][2]=matrix.A[2][0]= v.dx*v.dz*2.0/len/len;
		//matrix.A[1][2]=matrix.A[2][1]= v.dz*v.dy*2.0/len/len;
		//return matrix;

		//////////////////////////////////////////////////////////////////////////
		tagMatrix3D matrix;

		if (IS_ZERO(v.GetLengthXY()))
		{
			matrix.A[2][2] = -1;
			return matrix;
		}

		if (IS_ZERO(v.GetLengthYZ()))
		{
			matrix.A[0][0] = -1;
			return matrix;
		}

		if (IS_ZERO(v.GetLengthZX()))
		{
			matrix.A[1][1] = -1;
			return matrix;
		}

		matrix.A[2][2] = -1;

		//((CVector3D)v).Normalize();

		//double lenYZ=((CVector3D)v).GetLengthYZ();

		//CMatrix3D mTmpx;
		//mTmpx.A[1][1] = mTmpx.A[2][2] = v.dz / lenYZ;
		//mTmpx.A[1][2] = v.dy / lenYZ;
		//mTmpx.A[2][1] = -v.dy / lenYZ;

		//CMatrix3D mTmpy;
		//mTmpy.A[0][0] = mTmpy.A[2][2] = lenYZ;
		//mTmpy.A[0][2] = v.dx;
		//mTmpy.A[2][0] = -v.dx;

		//CMatrix3D mTmpyN;
		//mTmpyN.A[0][0] = mTmpyN.A[2][2] = lenYZ;
		//mTmpyN.A[0][2] = -v.dx;
		//mTmpyN.A[2][0] = v.dx;

		//CMatrix3D mTmpxN;
		//mTmpxN.A[1][1] = mTmpxN.A[2][2] = v.dz / lenYZ;
		//mTmpxN.A[1][2] = -v.dy / lenYZ;
		//mTmpxN.A[2][1] = v.dy / lenYZ;

		//matrix = mTmpx * mTmpy * matrix * mTmpyN * mTmpxN;

		double lxy = v.GetLengthXY();
		tagMatrix3D Rz;
		Rz.A[0][0]=Rz.A[1][1]=v.dy/lxy;
		Rz.A[0][1]=v.dx/lxy;Rz.A[1][0]= -v.dx/lxy;

		double len= v.GetLength();
		tagMatrix3D Rx;
		Rx.A[2][2]=Rx.A[1][1]=v.dz/len;
		Rx.A[1][2]=lxy/len;Rx.A[2][1]= -lxy/len;

		tagMatrix3D Rxn;
		Rxn.A[2][2]=Rxn.A[1][1]=v.dz/len;
		Rxn.A[2][1]=lxy/len;Rxn.A[1][2]= -lxy/len;

		tagMatrix3D Rzn;
		Rzn.A[0][0]=Rzn.A[1][1]=v.dy/lxy;
		Rzn.A[1][0]=v.dx/lxy;Rzn.A[0][1]= - v.dx/lxy;

		matrix = Rz * Rx * matrix * Rxn * Rzn;
		//////////////////////////////////////////////////////////////////////////

		return matrix;
	};

	static tagMatrix3D CreateRotateMatrix(double da,VECTOR3D v)
	{
		tagMatrix3D R;

		if(IS_ZERO(da))	return R;

		Q_ASSERT(!v.IsZeroLength());

		double lxy=v.GetLengthXY();
		if(IS_ZERO(lxy))
		{
			if(v.dz < 0.0) da *= -1.0;
			R.A[0][0]=R.A[1][1]=cos(da);
			R.A[0][1]=sin(da);R.A[1][0]=-sin(da);
			return R;
		}
		double lyz=v.GetLengthYZ();
		if(IS_ZERO(lyz))
		{
			if(v.dx < 0.0) da *= -1.0;
			R.A[2][2]=R.A[1][1]=cos(da);
			R.A[1][2]=sin(da);R.A[2][1]= -sin(da);
			return R;
		}
		double lxz=v.GetLengthZX();
		if(IS_ZERO(lxz))
		{
			if(v.dy < 0.0) da *= -1.0;
			R.A[0][0]=R.A[2][2]=cos(da);
			R.A[0][2]=-sin(da);R.A[2][0]=sin(da);
			return R;
		}

		tagMatrix3D Rz;
		Rz.A[0][0]=Rz.A[1][1]=v.dy/lxy;
		Rz.A[0][1]=v.dx/lxy;Rz.A[1][0]= -v.dx/lxy;

		double len=v.GetLength();
		tagMatrix3D Rx;
		Rx.A[2][2]=Rx.A[1][1]=v.dz/len;
		Rx.A[1][2]=lxy/len;Rx.A[2][1]= -lxy/len;

		R.A[0][0]=R.A[1][1]=cos(da);
		R.A[0][1]=sin(da);R.A[1][0]= -sin(da);

		tagMatrix3D Rxn;
		Rxn.A[2][2]=Rxn.A[1][1]=v.dz/len;
		Rxn.A[2][1]=lxy/len;Rxn.A[1][2]= -lxy/len;

		tagMatrix3D Rzn;
		Rzn.A[0][0]=Rzn.A[1][1]=v.dy/lxy;
		Rzn.A[1][0]=v.dx/lxy;Rzn.A[0][1]= - v.dx/lxy;

		return Rz*Rx*R*Rxn*Rzn;
	};

	static tagMatrix3D CreateScaleMatrix(double x,double y, double z)
	{
		Q_ASSERT(!IS_ZERO(x));
		Q_ASSERT(!IS_ZERO(y));
		Q_ASSERT(!IS_ZERO(z));

		tagMatrix3D m;
		m.A[0][0] = x;
		m.A[1][1] = y;
		m.A[2][2] = z;
		return m;
	};

	 void ScaleMatrix(double x,double y, double z)
	{
		 Q_ASSERT(!IS_ZERO(x));
		 Q_ASSERT(!IS_ZERO(y));
		 Q_ASSERT(!IS_ZERO(z));

		A[0][0] *= x;
		A[1][1] *= y;
		A[2][2] *= z;
	};

	static tagMatrix3D CreateTransfMatrix(VECTOR3D vec)
	{
		tagMatrix3D m;
		m.A[3][0]=vec.dx;
		m.A[3][1]=vec.dy;
		m.A[3][2]=vec.dz;
		return m;
	};

	void TransfMatrix(VECTOR3D vec)
	{
		A[3][0] +=vec.dx;
		A[3][1] +=vec.dy;
		A[3][2] +=vec.dz;
	};

	tagMatrix3D TranMatrix()
	{
		tagMatrix3D m;
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				m.A[j][i] = A[i][j];
			}
		return m;
	};

	 void MatrixToArray(double *m,bool bRow=true)
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				if (bRow=true)
				{				
					m[4*i+j] = A[i][j];
				}
				else
				{
					m[4*i+j] = A[j][i];
				}

			}

	};

} ;//MATRIX3D, *PMATRIX3D;
typedef tagMatrix3D MATRIX3D, *PMATRIX3D;

typedef struct tagPoint2D{
	double x;
	double y;

	//bool m_bIsSelected;
	//bool m_bIsOperated;
	//bool m_bIsDeleted;

	tagPoint2D()	{
		x = 0.0;
		y = 0.0;

		//m_bIsSelected = false;
		//m_bIsOperated = false;
		//m_bIsDeleted = false;
	};
	tagPoint2D(double dx, double dy)	{
		x = dx;
		y = dy;

		//m_bIsSelected = false;
		//m_bIsOperated = false;
		//m_bIsDeleted = false;
	};

	tagPoint2D(const double*p)	{
		x=p[0];
		y=p[1];

		//m_bIsSelected = false;
		//m_bIsOperated = false;
		//m_bIsDeleted = false;
	};

	tagPoint2D(const tagPoint2D &p)	{
		x=p.x;
		y=p.y;

		//m_bIsSelected = false;
		//m_bIsOperated = false;
		//m_bIsDeleted = false;
	};


	// offsetting with vector
	tagPoint2D operator+(VECTOR2D v) const	{return tagPoint2D(x+v.dx,y+v.dy);};

	void operator+=(VECTOR2D v)	{x+=v.dx;	y+=v.dy;	};

	tagPoint2D operator-(VECTOR2D v) const	{return tagPoint2D(x-v.dx,y-v.dy);	};

	void operator-=(VECTOR2D v)	{x+=v.dx;		y+=v.dy;};


	// derive vector = this point - sp
	VECTOR2D operator-(tagPoint2D sp) const	{return VECTOR2D(x-sp.x,y-sp.y);};

	// transform the point
	tagPoint2D operator*(const MATRIX2D& matrix) const	{
		double xx,yy,sc;
		xx = x*matrix.A[0][0]+y*matrix.A[1][0] + matrix.A[2][0];
		yy = x*matrix.A[0][1]+y*matrix.A[1][1] + matrix.A[2][1];
		sc = x*matrix.A[0][2]+y*matrix.A[1][2] + matrix.A[2][2];
		xx /= sc;
		yy /= sc;
		return tagPoint2D(xx,yy);
	};

	void  operator*=(const MATRIX2D& matrix){(*this)=(*this)*matrix;};
} POINT2D, *PPOINT2D;

typedef struct tagPoint3D{
	double x;
	double y;
	double z;
	bool m_bIsSelected;
	bool m_bIsOperated;
	int m_classfiy;
	int facenumber;
	//bool m_bIsSelected;
	//bool m_bIsOperated;
	//bool m_bIsDeleted;
	

	tagPoint3D()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		facenumber = -1;
		m_bIsSelected = false;
		m_bIsOperated = false;
		//m_bIsDeleted = false;

	};
	tagPoint3D(double dx, double dy, double dz)
	{
		x = dx;
		y = dy;
		z = dz;

		//m_bIsSelected = false;
		//m_bIsOperated = false;
		//m_bIsDeleted = false;
	};

	tagPoint3D(const double*p)	{
		x=p[0];
		y=p[1];
		z=p[2];

		//m_bIsSelected = false;
		//m_bIsOperated = false;
		//m_bIsDeleted = false;
	};


	tagPoint3D(const tagPoint3D &p)	{
		x=p.x;
		y=p.y;
		z=p.z;

		//m_bIsSelected = false;
		//m_bIsOperated = false;
		//m_bIsDeleted = false;
	};

	bool operator < (const tagPoint3D &p) const	{
		double dTolerance = TOLERANCE / 2.0;
		//if ((z < ( p.z - dTolerance) ) || (y < ( p.y - dTolerance)) || (x < ( p.x - dTolerance)) )
		//{
		//	return true;
		//}
		//else
		//	return false;

		//return ((z < ( p.z - dTolerance) ) || (y < ( p.y - dTolerance)) || (x < ( p.x - dTolerance)) );

		if(z < ( p.z - dTolerance)) return true;
		if(z > ( p.z + dTolerance)) return false;
		if(y < ( p.y - dTolerance)) return true;
		if(y > ( p.y + dTolerance)) return false;
		if(x < ( p.x - dTolerance)) return true;
		if(x > ( p.x + dTolerance)) return false;
		return false;
	};

	//operators
	tagPoint3D operator*(const MATRIX3D& matrix) const	{
		double rx,ry,rz,sc;
		rx = x * matrix.A[0][0] + y * matrix.A[1][0] + z * matrix.A[2][0] + matrix.A[3][0];
		ry = x * matrix.A[0][1] + y * matrix.A[1][1] + z * matrix.A[2][1] + matrix.A[3][1];
		rz = x * matrix.A[0][2] + y * matrix.A[1][2] + z * matrix.A[2][2] + matrix.A[3][2];
		sc = x * matrix.A[0][3] + y * matrix.A[1][3] + z * matrix.A[2][3] + matrix.A[3][3];
		rx /= sc;
		ry /= sc;
		rz /= sc;
		return tagPoint3D(rx,ry,rz);
	};

	void  operator*=(const MATRIX3D& matrix){(*this) = (*this)*matrix;	};

	tagPoint3D  operator*(const double d){return tagPoint3D(x*d,y*d,z*d);	};
	void  operator*=(const double d){(*this) = (*this)*d;	};

	tagPoint3D  operator/(const double d){/*ASSERT(abs(d) < CAD_ZERO);*/	return tagPoint3D(x/d,y/d,z/d);	};
	void  operator/=(const double d){(*this) = (*this)/d;	};

	// offsetting with vector
	tagPoint3D operator+(VECTOR3D v) const{	return tagPoint3D(x+v.dx,y+v.dy,z+v.dz);};

	void operator+=(VECTOR3D v)	{
		x+=v.dx;
		y+=v.dy;
		z+=v.dz;
	};

	tagPoint3D operator+(tagPoint3D p) const{	return tagPoint3D(x+p.x,y+p.y,z+p.z);};
	void operator+=(tagPoint3D p)	{
		x+=p.x;
		y+=p.y;
		z+=p.z;
	};

//	tagPoint3D operator-(tagPoint3D p) const{	return tagPoint3D(x-p.x,y-p.y,z-p.z);};
	void operator-=(tagPoint3D p)	{
		x-=p.x;
		y-=p.y;
		z-=p.z;
	};

	tagPoint3D operator-(VECTOR3D v) const{	return tagPoint3D(x-v.dx,y-v.dy,z-v.dz);};

	void operator-=(VECTOR3D v)	{
		x-=v.dx;
		y-=v.dy;
		z-=v.dz;
	};


	// derive vector = this point - sp
	VECTOR3D operator-(tagPoint3D sp) const	{return VECTOR3D(x-sp.x,y-sp.y,z-sp.z);	};

	BOOL operator==(tagPoint3D &pos) const	{
		VECTOR3D  vect(x-pos.x,y-pos.y,z-pos.z);

		if( IS_ZERO( vect.GetLength( ) ) ) return TRUE;
		else return FALSE;

	};

	BOOL operator!=(tagPoint3D pos) const	{
		VECTOR3D  vect(x-pos.x,y-pos.y,z-pos.z);

		if( IS_ZERO( vect.GetLength( ) ) ) return FALSE;
		else return TRUE;
	};

	//used in geometrical calculation
	double  operator*(VECTOR3D v)	{return (x*v.dx + y*v.dy + z*v.dz);	};

} POINT3D, *PPOINT3D;


//////////////////////////////////////////////////////////////////////////
//  (x0,y0): the left and bottom corner
//  (x1,y1): the right and top corner
//////////////////////////////////////////////////////////////////////////
typedef struct tagBox2D{
	double x0;
	double y0;
	double x1;
	double y1;

	tagBox2D(){	x0=y0=x1=y1=0;	};

	tagBox2D(double ix0,double iy0,double ix1,double iy1)	{
		x0 = ix0; y0=iy0; x1=ix1; y1=iy1;
		normalize();
	};

	tagBox2D(POINT2D pt0,POINT2D pt1)	{
		x0 = pt0.x; y0=pt0.y; x1=pt1.x; y1=pt1.y;
		normalize();
	};

	tagBox2D(tagBox2D &b)	{
		x0=b.x0; y0=b.y0;
		x1=b.x1; y1=b.y1;
	};

	tagBox2D(POINT2D p,VECTOR2D v)	{
		x0=p.x; y0=p.y;
		x1=p.x+v.dx;
		y1=p.x+v.dy;
		normalize();
	};


	// operator
	//// get the union box of this and box b.
	tagBox2D operator+(tagBox2D b) const	{
		tagBox2D box;
		box.x0 = (x0<b.x0)?x0:b.x0;
		box.y0 = (y0<b.y0)?y0:b.y0;
		box.x1 = (x1>b.x1)?x1:b.x1;
		box.y1 = (y1>b.y1)?y1:b.y1;
		return box;
	};

	void operator+=(tagBox2D b)	{
		tagBox2D box = (*this)+b;
		(*this) = box;
	};

	//// get the intersect box of this and box b.
	tagBox2D operator&(tagBox2D b) const	{
		tagBox2D box;
		double xx0,yy0,xx1,yy1;
		xx0 = (x0>b.x0)?x0:b.x0;
		yy0 = (y0>b.y0)?y0:b.y0;
		xx1 = (x1<b.x1)?x1:b.x1;
		yy1 = (y1<b.y1)?y1:b.y1;
		if((xx1>xx0) && (yy1>yy0)){
			box = tagBox2D(xx0,yy0,xx1,yy1);
		}
		return box;
	};

	void operator&=(tagBox2D b)	{
		tagBox2D box = (*this)&b;
		*this = box;
	};

	// get attribs
	BOOL IsZero() const
	{
		return IS_ZERO(Length() * Width());
	};

	// Length of X direction
	double Length() const 	{return x1-x0;	};

	// Length of Y direction
	double Width() const 	{	return y1-y0;	};

	// relationship culation
	// there may be four cases:
	// <enumSeparated,enumIntersected>
	enum {enumSeparated,enumIntersected};
	UINT GetRelationWith(tagBox2D b) const	{
		tagBox2D box;
		box = (*this)&b;
		if(box.IsZero())
			return enumSeparated;
		else
			return enumIntersected;
	};

	void normalize()	{
		double xx0,yy0,xx1,yy1;
		xx0 = (x0<x1)?x0:x1;
		xx1 = (x0>x1)?x0:x1;
		yy0 = (y0<y1)?y0:y1;
		yy1 = (y0>y1)?y0:y1;
		x0 = xx0; y0 = yy0; x1 = xx1; y1 = yy1;
	};

	void Clear(){	x0=y0=x1=y1=0;	};
} BOX2D , *PBOX2D;

//////////////////////////////////////////////////////////////////////////
//  (x0,y0): the left and bottom corner
//  (x1,y1): the right and top corner
//////////////////////////////////////////////////////////////////////////
typedef struct tagBox3D{
	double x0;
	double y0;
	double z0;
	double x1;
	double y1;
	double z1;

	tagBox3D()	{		x0=y0=z0=x1=y1=z1=0;	};

	tagBox3D(double ix0,double iy0,double iz0,double ix1,double iy1, double iz1){
		x0 = ix0; y0=iy0; z0=iz0;x1=ix1; y1=iy1;z1=iz1;
		normalize();
	};

	tagBox3D(POINT3D pt0,POINT3D pt1)	{
		x0 = pt0.x; y0=pt0.y; z0=pt0.z;x1=pt1.x; y1=pt1.y;z1=pt1.z;
		normalize();
	};

	tagBox3D(tagBox3D &b)	{
		x0=b.x0; y0=b.y0;z0=b.z0;
		x1=b.x1; y1=b.y1;z1=b.z1;
	};

	tagBox3D(POINT3D p,VECTOR3D v)	{
		x0=p.x; y0=p.y; z0=p.z;
		x1=p.x+v.dx;
		y1=p.x+v.dy;
		z1=p.x+v.dz;
		normalize();
	};

	// operator
	//// get the union box of this and box b.
	tagBox3D operator+(tagBox3D b) const	{
		tagBox3D box;// = (*this);

	//	if( b.IsEmpty( ) ) return  box;
		if( IsEmpty( ) ) return b;

		box.x0 = (x0<b.x0)?x0:b.x0;
		box.y0 = (y0<b.y0)?y0:b.y0;
		box.z0 = (z0<b.z0)?z0:b.z0;
		box.x1 = (x1>b.x1)?x1:b.x1;
		box.y1 = (y1>b.y1)?y1:b.y1;
		box.z1 = (z1>b.z1)?z1:b.z1;

		return box;
	};

	void operator*= ( double sc )	{
		x0 = x0*sc;
		y0 = y0*sc;
		z0 = z0*sc;
		x1 = x1*sc;
		y1 = y1*sc;
		z1 = z1*sc;
	};

	tagBox3D operator*( const MATRIX3D& matrix ) const	{
		POINT3D p0(x0,y0,z0);
		POINT3D p1(x1,y1,z1);

		p0 *= matrix;
		p1 *= matrix;

		return 	tagBox3D(p0,p1);
	};

	void operator*=(const MATRIX3D& matrix ){(*this) = (*this)*matrix;	};

	tagBox3D operator*( double sc ) const	{
		tagBox3D box;
		box.x0 = x0*sc;
		box.y0 = y0*sc;
		box.z0 = z0*sc;
		box.x1 = x1*sc;
		box.y1 = y1*sc;
		box.z1 = z1*sc;
		return box;
	};

	void operator+=(tagBox3D b)	{
		tagBox3D box = (*this)+b;
		(*this) = box;
	};

	//// get the intersect box of this and box b.
	tagBox3D operator&(tagBox3D b) const	{
		tagBox3D box;

		if( b.IsEmpty( ) || ((tagBox3D *)this)->IsEmpty( ) ) return box;

		double xx0,yy0,zz0,xx1,yy1,zz1;
		xx0 = (x0>b.x0)?x0:b.x0;
		yy0 = (y0>b.y0)?y0:b.y0;
		zz0 = (z0>b.z0)?z0:b.z0;
		xx1 = (x1<b.x1)?x1:b.x1;
		yy1 = (y1<b.y1)?y1:b.y1;
		zz1 = (z1<b.z1)?z1:b.z1;
		if((xx1>xx0) && (yy1>yy0) && (zz1>zz0)){
			box = tagBox3D(xx0,yy0,zz0,xx1,yy1,zz1);
		}
		return box;
	};

	void operator&=(tagBox3D b)	{
		tagBox3D box = (*this)&b;
		*this = box;
	};

	BOOL operator<<(tagBox3D  b) const	{
		if( IsEmpty( ) ) return TRUE;

		if( (x0 >= b.x0) && (y0 >= b.y0) && (z0 >= b.z0) &&
			(x1 <= b.x1) && (y1 <= b.y1) && (z1 <= b.z1)  ) return TRUE;
		else
			return FALSE;
	};

	BOOL operator>>(tagBox3D b) const	{
		if( b.IsEmpty( ) ) return TRUE;

		if( (x0 >= b.x0) && (y0 >= b.y0) && (z0 >= b.z0) &&
			(x1 <= b.x1) && (y1 <= b.y1) && (z1 <= b.z1)  ) return FALSE;
		else
			return TRUE;
	};

	BOOL operator>>(POINT3D p) const	{
		if( (x0 <= p.x) && (y0 <= p.y) && (z0 <= p.z) &&
			(x1 >= p.x) && (y1 >= p.y) && (z1 >= p.z)  ) return TRUE;
		else
			return FALSE;
	};

	//// get the intersect box of this and box b.
	tagBox3D operator|(tagBox3D b) const	{
		tagBox3D box;

		if( ((tagBox3D *)this)->IsEmpty( ) ) {
			box = b;
			return box;
		}
		if( b.IsEmpty( ) ) {
			box = *this;
			return box;
		}

		double xx0,yy0,zz0,xx1,yy1,zz1;
		xx0 = (x0<b.x0)?x0:b.x0;
		yy0 = (y0<b.y0)?y0:b.y0;
		zz0 = (z0<b.z0)?z0:b.z0;
		xx1 = (x1>b.x1)?x1:b.x1;
		yy1 = (y1>b.y1)?y1:b.y1;
		zz1 = (z1>b.z1)?z1:b.z1;
		if((xx1>=xx0) && (yy1>=yy0) && (zz1>=zz0)){
			box = tagBox3D(xx0,yy0,zz0,xx1,yy1,zz1);
		}
		return box;
	};

	void operator|=(tagBox3D b)	{
		tagBox3D box = (*this)|b;
		*this = box;
	};

	// get attribs
	BOOL IsEmpty() const	{
		return	IS_ZERO(Width()) &&
			IS_ZERO(Height()) &&
			IS_ZERO(Length());
	};

	// Length of X direction
	double Length() const 	{	return x1-x0;	};

	// Length of Y direction
	double Width() const {	return y1-y0;	};
	// Length of Z direction
	double Height() const {	return z1-z0;	};


	// relationship culation
	// there may be four cases:
	// <enumSeparated,enumIntersected>
	enum {enumSeparated,enumIntersected};
	UINT GetRelationWith(tagBox3D b) const	{
		tagBox3D box;
		box = (*this)&b;
		if(box.IsEmpty())
			return enumSeparated;
		else
			return enumIntersected;
	};

	void normalize()	{
		double xx0,yy0,xx1,yy1,zz0,zz1;
		xx0 = (x0<x1)?x0:x1;
		xx1 = (x0>x1)?x0:x1;
		yy0 = (y0<y1)?y0:y1;
		yy1 = (y0>y1)?y0:y1;
		zz0 = (z0<z1)?z0:z1;
		zz1 = (z0>z1)?z0:z1;
		x0 = xx0; y0 = yy0; x1 = xx1; y1 = yy1;
		z0 = zz0; z1 = zz1;
	};

	void Clear()	{		x0=y0=z0=x1=y1=z1=0;	};

	tagBox3D operator+(VECTOR3D vect) const	{
		tagBox3D box;

		box.x0 = x0+vect.dx;
		box.y0 = y0+vect.dy;
		box.z0 = z0+vect.dz;
		box.x1 = x1+vect.dx;
		box.y1 = y1+vect.dy;
		box.z1 = z1+vect.dz;

		return box;
	};

	void operator+=(VECTOR3D vect)	{
		x0 = x0+vect.dx;
		y0 = y0+vect.dy;
		z0 = z0+vect.dz;
		x1 = x1+vect.dx;
		y1 = y1+vect.dy;
		z1 = z1+vect.dz;	
	};

	tagBox3D operator-(VECTOR3D vect) const	{
		tagBox3D box;

		box.x0 = x0-vect.dx;
		box.y0 = y0-vect.dy;
		box.z0 = z0-vect.dz;
		box.x1 = x1-vect.dx;
		box.y1 = y1-vect.dy;
		box.z1 = z1-vect.dz;

		return box;
	};

	void operator-=(VECTOR3D vect)	{
		x0 = x0-vect.dx;
		y0 = y0-vect.dy;
		z0 = z0-vect.dz;
		x1 = x1-vect.dx;
		y1 = y1-vect.dy;
		z1 = z1-vect.dz;
	};

} BOX3D , *PBOX3D;

//typedef vector<int>		vec_int;

typedef vector<long>		vec_long;
typedef vector<vec_long>	vec_vec_long;

typedef vector <double>     vec_double;
typedef vector <vec_double>     vec_vec_double;

typedef	map<long, long>		map_long_long;

typedef vector<POINT2D>		vec_POINT2D;
typedef vector<PPOINT2D>	vec_PPOINT2D;
typedef vector<POINT3D>		vec_POINT3D;
typedef vector<PPOINT3D>	vec_PPOINT3D;

typedef map<POINT3D, long>	map_POINT3D_INT;
typedef map_POINT3D_INT::iterator	it_map_POINT3D_INT;
typedef pair<POINT3D, long>	pair_POINT3D_INT;
typedef	pair<it_map_POINT3D_INT,bool>	pair_ItMapPntInt_bool;

typedef vector<VECTOR2D>	vec_VECTOR2D;
typedef vector<PVECTOR2D>	vec_PVECTOR2D;
typedef vector<VECTOR3D>	vec_VECTOR3D;
typedef vector<PVECTOR3D>	vec_PVECTOR3D;

typedef vector<MATRIX2D>	vec_MATRIX2D;
typedef vector<PMATRIX2D>	vec_PMATRIX2D;
typedef vector<MATRIX3D>	vec_MATRIX3D;
typedef vector<PMATRIX3D>	vec_PMATRIX3D;



static double _AngleBetween3D(VECTOR3D v1,VECTOR3D v2,bool bDirected = false)
{
	double dArc;
	VECTOR3D vecCross;
	v1.Normalize();
	v2.Normalize();
	dArc = acos(v1|v2);

	if (!bDirected)
	{	
		return dArc;
	}

	//if (( abs(dArc) <= CAD_ZERO) ||(abs(dArc - PI) <= CAD_ZERO))
	//{
	//	return dArc;
	//}

	vecCross = v1 * v2;
	if (vecCross.dz > 0.0)
	{
		return dArc;
	}	
	if (vecCross.dz < 0.0)
	{
		return (-dArc);
	}

	if (vecCross.dy > 0.0)
	{
		return dArc;
	}
	if (vecCross.dy < 0.0)
	{
		return (-dArc);
	}

	if (vecCross.dx > 0.0)
	{
		return dArc;
	}
	if (vecCross.dx < 0.0)
	{
		return (-dArc);
	}

	return dArc;
};

// exported API functions
static double _AngleBetween2D(VECTOR2D v1,VECTOR2D v2)
{
	//if(_IsParallel(v1,v2))	return 0;
	//CVector2D cv1(v1),cv2(v2);
	//return acos((cv1.GetNormal())|(cv2.GetNormal()));

	VECTOR3D cv1(v1.dx, v1.dy,0.0);
	VECTOR3D cv2(v2.dx, v2.dy, 0.0);
	return  _AngleBetween3D(cv1,cv2);


};

static double _DistOf(POINT2D pt0,POINT2D pt1)
{
	VECTOR2D vec(pt1.x-pt0.x,pt1.y-pt0.y);
	return vec.GetLength();
};

static double _DistOf(POINT3D pt0,POINT3D pt1)
{
	VECTOR3D vec(pt1.x-pt0.x,pt1.y-pt0.y, pt1.z-pt0.z);
	return vec.GetLength();
};

static double _SquareDistOf(POINT2D pt0,POINT2D pt1)
{
	VECTOR2D vec(pt1.x-pt0.x,pt1.y-pt0.y);
	return vec|vec;
};

static double _SquareDistOf(POINT3D pt0,POINT3D pt1)
{
	VECTOR3D vec(pt1.x-pt0.x,pt1.y-pt0.y, pt1.z-pt0.z);
	return vec|vec;
};

static BOOL _IsParallel(VECTOR2D v0,VECTOR2D v1)
{
	return IS_ZERO((v0*v1).dz);
};

static BOOL _IsParallel(VECTOR3D v0,VECTOR3D v1)
{
	return IS_ZERO((v0*v1).GetLength());
};

static BOOL _IsOrthogonal(VECTOR3D v0,VECTOR3D v1)
{
	return IS_ZERO(v0|v1);
};

