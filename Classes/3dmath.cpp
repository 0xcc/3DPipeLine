#include "3dmath.h"


///////////////////////////////////////////////Vector2D////////////////////////////////////////////////////
void Vector2D::init(float _x, float _y){
	this->x=_x;
	this->y=_y;
}

void Vector2D::init(const Vector2D &p0, const Vector2D &p1){
	this->x=p1.x-p0.x;
	this->y=p1.y-p0.y;
}

void Vector2D::zero(){
	this->x=0.0f;
	this->y=0.0f;
}

void Vector2D::normalize(){
	float length=sqrtf(this->x*this->x+this->y*this->y);

	if (length < EPSILON_E5) 
		return;

	float length_inv=1.0f/length;

	this->x=this->x*length_inv;
	this->y=this->y*length_inv;
}


void Vector2D::addTo(const Vector2D &other, Vector2D &result){
	result.x=this->x+other.x;
	result.y=this->y+other.y;
}

Vector2D Vector2D::add(const Vector2D &other){
	Vector2D result;
	result.init(this->x+other.x,this->y+other.y);
	
	return result;
}

void Vector2D::subTo(const Vector2D &other, Vector2D &result){
	result.x=this->x-other.x;
	result.y=this->y-other.y;
}


Vector2D Vector2D::sub(const Vector2D &other){
	Vector2D result;
	result.init(this->x-other.x,this->y-other.y);
	return result;
}


void Vector2D::scaleTo(float k, Vector2D &result){
	result.x=k*result.x;
	result.y=k*result.y;
}

void Vector2D::scale(float k)
{
	this->x *= k;
	this->y *= k;
}

float Vector2D::dot(const Vector2D &other)
{
	return this->x * other.x + this->y * other.y;
}

float Vector2D::length() const
{
	return sqrtf(this->x * this->x + this->y * this->y);
}


/*
a.b=|a||b|cos(theta)
cos(theta)=a.b/|a|*|b|
*/
float Vector2D::cosTh(const Vector2D &other)
{
	return this->dot(other) / (this->length() * other.length());
}

////////////////////////////////////Vector3D////////////////////////////////////////////////


void Vector3D::init(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void Vector3D::init(const Vector3D &p0, const Vector3D &p1)
{
	x = p1.x - p0.x;
	y = p1.y - p0.y;
	z = p1.z - p0.z;
}


void Vector3D::zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

void Vector3D::normalize()
{
	float length = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);

	if (length < EPSILON_E5) 
		return;

	float length_inv = 1.0f / length;

	this->x *= length_inv;
	this->y *= length_inv;
	this->z *= length_inv;
}


void Vector3D::normalizeTo(Vector3D &result) const
{
	result.zero();


	float length = this->length();

	if (length < EPSILON_E5) 
		return;

	float length_inv = 1.0f / length;

	result.x = this->x * length_inv;
	result.y = this->y * length_inv;
	result.z = this->z * length_inv;
}

void Vector3D::addTo(const Vector3D &other, Vector3D &result)
{
	result.x = this->x + other.x;
	result.y = this->y + other.y;
	result.z = this->z + other.z;
}

Vector3D Vector3D::add(const Vector3D &other)
{
	Vector3D result;

	result.init(this->x + other.x, this->y + other.y, this->z + other.z);

	return result;
}

void Vector3D::subTo(const Vector3D &other, Vector3D &result)
{
	result.x = this->x - other.x;
	result.y = this->y - other.y;
	result.z = this->z - other.z;
}

Vector3D Vector3D::sub(const Vector3D &other)
{
	Vector3D result;

	result.init(this->x - other.x, this->y - other.y, this->z - other.z);

	return result;
}

/*
			[00][01][02]
[0][1][2]	[10][11][12]
			[20][21][22]
*/
void Vector3D::mulMat3X3To(const Matrix_3X3 &other, Vector3D &result){
	for (int col=0;col<3;col++){
		float sum=0.0f;

		for (int idx=0;idx<3;idx++){
			sum+=this->M[idx]*other.M[idx][col];

		}
		result.M[col]=sum;
	}
}

/*
			[00][01][02][03]
[0][1][2]	[10][11][12][13]
			[20][21][22][23]
			[30][31][32][33]
*/
void Vector3D::mulMat4X4To(const Matrix_4X4 &other, Vector3D &result)
{
 

	for (int col = 0; col < 3; col++) {
		float sum = 0.0f; // used to hold result

		for (int idx = 0; idx < 3; idx++)
		{
			sum += this->M[idx] * other.M[idx][col];
		}

		//加上M 最后一行 对应的
		sum += other.M[3][col];    
 
		result.M[col] = sum;
	}
}


 
void Vector3D::scaleTo(float k, Vector3D &result)
{
	result.x = k * this->x;
	result.y = k * this->y;
	result.z = k * this->z;
}

void Vector3D::scale(float k)
{
	this->x *= k;
	this->y *= k;
	this->z *= k;
}

//点积
float Vector3D::dot(const Vector3D &other)
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}


//叉积
void Vector3D::crossTo(const Vector3D &other, Vector3D &result)
{

	result.x =  (this->y * other.z - this->z * other.y);
	result.y = -(this->x * other.z - this->z * other.x);
	result.z =  (this->x * other.y - this->y * other.x);
}

Vector3D Vector3D::cross(const Vector3D &other)
{

	Vector3D result;

	result.init(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);

	return result;
}

float Vector3D::length() const
{
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

//
float Vector3D::cosTh(const Vector3D &other)
{
	return this->dot(other) / (this->length() * other.length());
}

//////////////////////////////////////////////////////////////////////////////////////////////


void Vector4D::init(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Vector4D::init(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
	w = 1.0f;
}

void Vector4D::zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

void Vector4D::normalize()
{
	// normalizes the vector and returns the result

	// compute length
	float length = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);

	// test for zero length vector
	// if found return zero vector
	if (length < EPSILON_E5) 
		return;

	float length_inv = 1.0f / length;

	// compute normalized version of vector
	this->x *= length_inv;
	this->y *= length_inv;
	this->z *= length_inv;
	this->w = 1.0f;
}

void Vector4D::normalizeTo(Vector4D &result)
{
	// normalizes the sent vector and returns the result in vn
	result.zero();

	// compute length
	float length = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);

	// test for zero length vector
	// if found return zero vector
	if (length < EPSILON_E5) 
		return;

	float length_inv = 1.0f / length;

	// compute normalized version of vector
	result.x = this->x * length_inv;
	result.y = this->y * length_inv;
	result.z = this->z * length_inv;
	result.w = 1.0f;
}

void Vector4D::addTo(const Vector4D &other, Vector4D &result)
{
	// this function adds this+other and return it in result
	result.x = this->x + other.x;
	result.y = this->y + other.y;
	result.z = this->z + other.z;
	result.w = 1.0f;
}

Vector4D Vector4D::add(const Vector4D &other)
{
	// this function adds this+other and returns the result on the stack
	Vector4D result;

	result.init(this->x + other.x, this->y + other.y, this->z + other.z, 1.0f);

	// return result
	return result;
}

void Vector4D::subTo(const Vector4D &other, Vector4D &result)
{
	// this function subtracts this-other and return it in result
	result.x = this->x - other.x;
	result.y = this->y - other.y;
	result.z = this->z - other.z;
	result.w = 1.0f;
}

Vector4D Vector4D::sub(const Vector4D &other)
{
	// this function subtracts this-other and returns the result on the stack
	Vector4D result; 

	result.init(this->x - other.x, this->y - other.y, this->z - other.z, 1.0f);

	// return result
	return result;
}

void Vector4D::mulMat4X4To(const Matrix_4X4 &other, Vector4D &result)
{
	// this function multiplies a VECTOR4D against a 4x4 matrix - this*other and stores the result
	// this function makes no assumptions

	for (int col = 0; col < 4; col++)
	{
		// compute dot product from row of this and column of other
		float sum = 0.0f; // used to hold result

		for (int idx = 0; idx < 4; idx++)
		{
			// add in next product pair
			sum += this->M[idx] * other.M[idx][col];
		}

		// insert resulting col element
		result.M[col] = sum;
	}
}

void Vector4D::mulMat4X3To(const Matrix_4X3 &other, Vector4D &result)
{
	// this function multiplies a VECTOR4D against a 4x3 matrix - this*other and stores the result
	// this function assumes that the last column of other is [0 0 0 1]t , thus w just gets replicated from the vector [x y z w]

	for (int col = 0; col < 3; col++)
	{
		// compute dot product from row of this and column of other
		float sum = 0.0f; // used to hold result

		for (int idx = 0; idx < 4; idx++)
		{
			// add in next product pair
			sum += this->M[idx] * other.M[idx][col];
		}

		// insert resulting col element
		result.M[col] = sum;
	}

	result.M[3] = this->M[3];
}

void Vector4D::divByW()
{
	x /= w;
	y /= w;
	z /= w;
	z = 1.0f;
}

void Vector4D::scaleTo(float k, Vector4D &result)
{
	// this function scales a vector by the constant k,
	// leaves the original unchanged, and returns the result
	// in vres as well as on the stack

	// multiply each component by scaling factor
	result.x = k * this->x;
	result.y = k * this->y;
	result.z = k * this->z;
	result.w = 1.0f;
}

void Vector4D::scale(float k)
{
	// this function scales a vector by the constant k,
	// in place , note w is left unchanged

	// multiply each component by scaling factor
	this->x *= k;
	this->y *= k;
	this->z *= k;
	this->w  = 1.0f;
}

float Vector4D::dot(const Vector4D &other)
{
	// computes the dot product between this and other
	return(this->x * other.x + this->y * other.y + this->z * other.z);
}

void Vector4D::crossTo(const Vector4D &other, Vector4D &result)
{
	// this function computes the cross product between this and other
	// and returns the vector that is perpendicular to each in result

	result.x =  (this->y * other.z - this->z * other.y);
	result.y = -(this->x * other.z - this->z * other.x);
	result.z =  (this->x * other.y - this->y * other.x);
	result.w = 1.0f;
}

Vector4D Vector4D::cross(const Vector4D &other)
{
	// this function computes the cross product between this and other
	// and returns the vector that is perpendicular to each
	Vector4D result;

	result.init(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x, 1.0f);

	// return result
	return result;
}

float Vector4D::length() const
{
	// computes the magnitude of a vector, slow
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

float Vector4D::lengthFast2()
{
	// this function computes the distance from the origin to x,y,z

	int t;  // used for swaping
	int x, y, z; // used for algorithm

	// make sure values are all positive
	x = fabs(this->x) * 1024;
	y = fabs(this->y) * 1024;
	z = fabs(this->z) * 1024;

	// sort values, x->y->z
	if (y < x)
	{
		t = x;
		x = y;
		y = t;
	}

	if (z < y) 
	{
		t = y;
		y = z;
		z = t;
	}

	if (y < x)
	{
		t = x;
		x = y;
		y = t;
	}

	int dist = (z + 11 * (y >> 5) + (x >> 2));

	// compute distance with 8% error
	return dist >> 10;
}

float Vector4D::cosTh(const Vector4D &other)
{
	// this function returns the cosine of the angle between
	// two vectors. Note, we could compute the actual angle,
	// many many times, in further calcs we will want ultimately
	// compute cos of the angle, so why not just leave it!
	return this->dot(other) / (this->length() * other.length());
}

void Vector4D::build(const Vector4D &init, const Vector4D &term, Vector4D &result)
{
	// build a 4d vector
	result.x = term.x - init.x;
	result.y = term.y - init.y;
	result.z = term.z - init.z;
	result.w = 1.0f;
}

const Vector4D Vector4D::ZERO = {0, 0, 0, 1}; 
const Vector4D Vector4D::ONE = {1, 1, 1, 1};


/////////////////////////////Plane3D//////////////////////////////////////////////

void Plane3D::init(const Point3D &_p0, const Vector3D &_normal, bool normalize){
	this->p0=_p0;
	if (normalize){
		_normal.normalizeTo(this->normal);
	}else{
		normal=_normal;
	}
}


float Plane3D::pointWhere(const Point3D &point){

	/*
		0 在平面上
		>0 正半面
		<0 负半面
	*/
	float hs=this->normal.x*(point.x-this->p0.x)+
				this->normal.y*(point.y-this->p0.y)+
				this->normal.z*(point.z-this->p0.z);

	return hs;
}

///////////////////////////////////////////////////////////////////////////

void Matrix_3X3::init(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22){
	this->M00 = m00; this->M01 = m01; this->M02= m02;
	this->M10 = m10; this->M11 = m11; this->M12= m12;
	this->M20 = m20; this->M21 = m21; this->M22 = m22;
}

void Matrix_3X3::zero(){
	memset(M,0,sizeof(M));
}

void Matrix_3X3::identify()
{
	memcpy(M, IDENTITY.M, sizeof(M));
}

//转置矩阵
void Matrix_3X3::transpose()
{
	Matrix_3X3 TempMat;

	TempMat.M00 = this->M00; TempMat.M01 = this->M10; TempMat.M02 = this->M20;
	TempMat.M10 = this->M01; TempMat.M11 = this->M11; TempMat.M12 = this->M21;
	TempMat.M20 = this->M02; TempMat.M21 = this->M12; TempMat.M22 = this->M22;

	memcpy(this->M, TempMat.M, sizeof(this->M));
}

void Matrix_3X3::transposeTo(Matrix_3X3 &mat)
{
	mat.M00 = this->M00; mat.M01 = this->M10; mat.M02 = this->M20;
	mat.M10 = this->M01; mat.M11 = this->M11; mat.M12 = this->M21;
	mat.M20 = this->M02; mat.M21 = this->M12; mat.M22 = this->M22;
}



void Matrix_3X3::addTo(const Matrix_3X3 &other, Matrix_3X3 &result)
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			result.M[row][col] = this->M[row][col] + other.M[row][col];
		}
	}
}

void Matrix_3X3::mulTo(const Matrix_3X3 &other, Matrix_3X3 &result)
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			float sum = 0.0f; // used to hold result

			for (int index = 0; index < 3; index++)
			{
				sum += this->M[row][index] * other.M[index][col];
			}
			result.M[row][col] = sum;
		}
	}
}

//秩
float Matrix_3X3::det() const
{
	// computes the determinate of a 3x3 matrix using co-factor expansion
	return this->M00 * (this->M11 * this->M22 - this->M21 * this->M12) - 
		this->M01 * (this->M10 * this->M22 - this->M20 * this->M12) + 
		this->M02 * (this->M10 * this->M21 - this->M20 * this->M11) ;
}

//逆矩阵
bool Matrix_3X3::inverseTo(Matrix_3X3 &result)
{
	// this function computes the inverse of a 3x3

	// first compute the determinate to see if there is an inverse
	float det = this->M00* (this->M11 * this->M22 - this->M21 * this->M12) - 
		this->M01* (this->M10 * this->M22 - this->M20 * this->M12) + 
		this->M02* (this->M10 * this->M21 - this->M20 * this->M11);

	if (fabs(det) < EPSILON_E5)
		return false;

	// compute inverse to save divides
	float det_inv = 1.0f / det;

	// compute inverse using m-1 = adjoint(m)/det(m) 
	result.M00 =  det_inv * (this->M11 * this->M22 - this->M21 * this->M12);
	result.M10 = -det_inv * (this->M10 * this->M22 - this->M20 * this->M12);
	result.M20 =  det_inv * (this->M10 * this->M21 - this->M20 * this->M11);

	result.M01 = -det_inv * (this->M01 * this->M22 - this->M21 * this->M02);
	result.M11 =  det_inv * (this->M00 * this->M22 - this->M20 * this->M02);
	result.M21 = -det_inv * (this->M00 * this->M21 - this->M20 * this->M01);

	result.M02 =  det_inv * (this->M01 * this->M12 - this->M11 * this->M02);
	result.M12 = -det_inv * (this->M00 * this->M12 - this->M10 * this->M02);
	result.M22 =  det_inv * (this->M00 * this->M11 - this->M10 * this->M01);

	// return success
	return true;
}

const Matrix_3X3 Matrix_3X3::IDENTITY =
{
	1,0,0,
	0,1,0,
	0,0,1,
};
////////////////////////////////////////////////Matrix_4X3////////////////////////////////////////////////////////////////



void Matrix_4X3::zero()
{
	memset(M, 0, sizeof(M));
}

void Matrix_4X3::identify()
{
	memcpy(M, IDENTITY.M, sizeof(M));
}

void Matrix_4X3::setColumn(int col, const Matrix_1X4 &mat)
{
	// TODO:
	this->M[0][col] = mat.M[0];
	this->M[1][col] = mat.M[1];
	this->M[2][col] = mat.M[2];
	this->M[3][col] = mat.M[3];
}

const Matrix_4X3 Matrix_4X3::IDENTITY =
{
	1,0,0, 
	0,1,0, 
	0,0,1, 
	0,0,0,
};

///////////////////////////////////////////////Matrix_4X4/////////////////////////////////////////////////////////////////////////////////


void Matrix_4X4::init(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	// this function fills a 4x4 matrix with the sent data in row major form
	this->M00 = m00; this->M01 = m01; this->M02 = m02; this->M03 = m03;
	this->M10 = m10; this->M11 = m11; this->M12 = m12; this->M13 = m13;
	this->M20 = m20; this->M21 = m21; this->M22 = m22; this->M23 = m23;
	this->M30 = m30; this->M31 = m31; this->M32 = m32; this->M33 = m33;
}

void Matrix_4X4::zero()
{
	memset(M, 0, sizeof(M));
}

void Matrix_4X4::identify()
{
	memcpy(M, IDENTITY.M, sizeof(M));
}

void Matrix_4X4::transpose()
{
	Matrix_4X4 TempMat;

	TempMat.M00 = this->M00; TempMat.M01 = this->M10; TempMat.M02 = this->M20; TempMat.M03 = this->M30; 
	TempMat.M10 = this->M01; TempMat.M11 = this->M11; TempMat.M12 = this->M21; TempMat.M13 = this->M31; 
	TempMat.M20 = this->M02; TempMat.M21 = this->M12; TempMat.M22 = this->M22; TempMat.M23 = this->M32; 
	TempMat.M30 = this->M03; TempMat.M31 = this->M13; TempMat.M32 = this->M22; TempMat.M33 = this->M33;

	memcpy(this->M, TempMat.M, sizeof(this->M));
}

void Matrix_4X4::transposeTo(Matrix_4X4 &mat)
{
	mat.M00 = this->M00; mat.M01 = this->M10; mat.M02 = this->M20; mat.M03 = this->M30; 
	mat.M10 = this->M01; mat.M11 = this->M11; mat.M12 = this->M21; mat.M13 = this->M31; 
	mat.M20 = this->M02; mat.M21 = this->M12; mat.M22 = this->M22; mat.M23 = this->M32; 
	mat.M30 = this->M03; mat.M31 = this->M13; mat.M32 = this->M22; mat.M33 = this->M33;
}

void Matrix_4X4::setColumn(int col, const Matrix_1X4 &mat)
{
	this->M[0][col] = mat.M[0];
	this->M[1][col] = mat.M[1];
	this->M[2][col] = mat.M[2];
	this->M[3][col] = mat.M[3];
}

void Matrix_4X4::addTo(const Matrix_4X4 &other, Matrix_4X4 &result)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			result.M[row][col] = this->M[row][col] + other.M[row][col];
		}
	}
}

void Matrix_4X4::mulTo(const Matrix_4X4 &other, Matrix_4X4 &result)
{

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			float sum = 0.0f; // used to hold result

			for (int index = 0; index < 4; index++)
			{
				// add in next product pair
				sum += this->M[row][index] * other.M[index][col];
			}

			// insert resulting row,col element
			result.M[row][col] = sum;
		}
	}
}

bool Matrix_4X4::inverseTo(Matrix_4X4 &result)
{

	float det = this->M00 * (this->M11 * this->M22 - this->M12 * this->M21) -
		this->M01 * (this->M10 * this->M22 - this->M12 * this->M20) +
		this->M02 * (this->M10 * this->M21 - this->M11 * this->M20);

	// test determinate to see if it's 0
	if (fabs(det) < EPSILON_E5)
		return false;

	float det_inv  = 1.0f / det;

	result.M00 =  det_inv * (this->M11 * this->M22 - this->M12 * this->M21);
	result.M01 = -det_inv * (this->M01 * this->M22 - this->M02 * this->M21);
	result.M02 =  det_inv * (this->M01 * this->M12 - this->M02 * this->M11);
	result.M03 =  0.0f; // always 0

	result.M10 = -det_inv * (this->M10 * this->M22 - this->M12 * this->M20);
	result.M11 =  det_inv * (this->M00 * this->M22 - this->M02 * this->M20);
	result.M12 = -det_inv * (this->M00 * this->M12 - this->M02 * this->M10);
	result.M13 =  0.0f; // always 0

	result.M20 =  det_inv * (this->M10 * this->M21 - this->M11 * this->M20);
	result.M21 = -det_inv * (this->M00 * this->M21 - this->M01 * this->M20);
	result.M22 =  det_inv * (this->M00 * this->M11 - this->M01 * this->M10);
	result.M23 =  0.0f; // always 0

	result.M30 = -(this->M30 * result.M00 + this->M31 * result.M10 + this->M32 * result.M20);
	result.M31 = -(this->M30 * result.M01 + this->M31 * result.M11 + this->M32 * result.M21);
	result.M32 = -(this->M30 * result.M02 + this->M31 * result.M12 + this->M32 * result.M22);
	result.M33 =  1.0f; // always 0

	// return success 
	return true;
}


void Matrix_4X4::buildModelToWorld(const Vector4D &world_pos){
	init(
		1,		0,		0,		0,
		0,		1,		0,		0,
		0,		0,		1,		0,
		world_pos.x,world_pos.y,world_pos.z,1
		);

}

void Matrix_4X4::buildEulerRotation(float theta_x, float theta_y, float theta_z){

	Matrix_4X4 mx,my,mz,mtmp;
	float sin_theta,cos_theta;
	int rot_seq=0;

	//X轴转
	if (fabs(theta_x)>EPSILON_E5){
		rot_seq=rot_seq|1;
	}

	//Y轴转
	if (fabs(theta_y)>EPSILON_E5){
		rot_seq=rot_seq|2;
	}

	//Z轴转
	if (fabs(theta_z)>EPSILON_E5){
		rot_seq=rot_seq|4;
	}

	switch(rot_seq){
	case 0:
		this->identify();
		break;
	case 1:
		cos_theta=cosf(theta_x);
		sin_theta=sinf(theta_x);
		this->init(1,	0,			0,			0,
				   0,	cos_theta,	sin_theta,	0,
				   0,  -sin_theta,	cos_theta,	0,
				   0,	0,			0,			1);
		break;

	case 2:
		cos_theta	= cosf(theta_y);
		sin_theta	= sinf(theta_y);
		this->init(
					cos_theta,	0,-sin_theta,0,
					0,			1,0			,0,
					sin_theta,	0,cos_theta ,0,
					0,			0,0			,1);
		break;
	case 3:

		//X轴
		cos_theta = cosf(theta_x);
		sin_theta = sinf(theta_x);
		mx.init(
			1,    0,          0,         0,
			0,    cos_theta,  sin_theta, 0,
			0,   -sin_theta,  cos_theta, 0,
			0,    0,          0,         1);

		cos_theta = cosf(theta_y);
		sin_theta = sinf(theta_y);

		//Y轴
		my.init(
			cos_theta, 0, -sin_theta, 0,
			0,         1,  0,         0,
			sin_theta, 0,  cos_theta, 0,
			0,         0,  0,         1);
		mx.mulTo(my,*this);
		break;
	case 4:
		cos_theta = cosf(theta_z);
		sin_theta = sinf(theta_z);

		this->init(
			cos_theta, sin_theta, 0, 0,  
			-sin_theta, cos_theta, 0, 0,
			0,         0,         1, 0,
			0,         0,         0, 1);
		break;
	case 5:
		cos_theta = cosf(theta_x);
		sin_theta = sinf(theta_x);

		mx.init(
			1,    0,          0,         0,
			0,    cos_theta,  sin_theta, 0,
			0,   -sin_theta,  cos_theta, 0,
			0,    0,          0,         1);

		cos_theta = cosf(theta_z);
		sin_theta = sinf(theta_z);

		mz.init(
			cos_theta, sin_theta, 0, 0,  
			-sin_theta, cos_theta, 0, 0,
			0,         0,         1, 0,
			0,         0,         0, 1);
		mx.mulTo(mz, *this);
		break;
	case 6:
		cos_theta = cosf(theta_y);
		sin_theta = sinf(theta_y);

		my.init(
			cos_theta, 0, -sin_theta, 0,  
			0,         1,  0,         0,
			sin_theta, 0,  cos_theta, 0,
			0,         0,  0,         1);

		cos_theta = cosf(theta_z);
		sin_theta = sinf(theta_z);

		mz.init(
			cos_theta, sin_theta, 0, 0,  
			-sin_theta, cos_theta, 0, 0,
			0,         0,         1, 0,
			0,         0,         0, 1);
		my.mulTo(mz, *this);
		break;
	case 7:
		cos_theta = cosf(theta_x);
		sin_theta = sinf(theta_x);

		mx.init( 
			1,    0,         0,         0,
			0,    cos_theta, sin_theta, 0,
			0,   -sin_theta, cos_theta, 0,
			0,    0,         0,         1);

		cos_theta = cosf(theta_y);
		sin_theta = sinf(theta_y);
		my.init(
			cos_theta, 0, -sin_theta,  0,  
			0,         1,  0,          0,
			sin_theta, 0,  cos_theta,  0,
			0,         0,  0,          1);

		cos_theta = cosf(theta_z);
		sin_theta = sinf(theta_z);
		mz.init( 
			cos_theta, sin_theta, 0, 0,  
			-sin_theta, cos_theta, 0, 0,
			0,         0,         1, 0,
			0,         0,         0, 1);

		mx.mulTo(my, mtmp);
		mtmp.mulTo(mz, *this);

		break;
	default:
		break;
	}


}


const Matrix_4X4 Matrix_4X4::IDENTITY =
{
	1,0,0,0, 
	0,1,0,0, 
	0,0,1,0, 
	0,0,0,1,
};

//////////////////////////////////////////////////////////////////////////
void split(const std::string &str, const std::string &sep, std::vector<std::string>& ret){

	ret.clear();
	std::string tmps=str;

	while(!tmps.empty()){
		std::string::size_type pos=tmps.find(sep);
		if (std::string::npos==pos){
			//结束
			ret.push_back(tmps);
			break;
		}else{
			//找到
			ret.push_back(tmps.substr(0,pos));
			//删除已经找到的部分
			tmps.erase(0,pos+sep.length());
		}
	}
}




std::string trim(const std::string &str)
{
	std::string ret = str;

	ret.erase(ret.begin(), std::find_if(ret.begin(), ret.end(), std::not1(std::ptr_fun<int, int>(std::isspace)))); // trim_left
	ret.erase(std::find_if(ret.rbegin(), ret.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), ret.end()); // trim_right

	return ret;
}

std::string trim_left(const std::string &str)
{
	std::string ret = str;

	ret.erase(ret.begin(), std::find_if(ret.begin(), ret.end(), std::not1(std::ptr_fun<int, int>(std::isspace)))); // trim_left

	return ret;
}

std::string trim_right(const std::string &str)
{
	std::string ret = str;

	ret.erase(std::find_if(ret.rbegin(), ret.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), ret.end()); // trim_right

	return ret;
}

std::string strip(const std::string &str, const std::string &chars)
{
	if (chars.empty())
	{
		return str;
	}
	else
	{
		std::string ret = str;

		std::string::size_type idx = ret.find_last_of(chars);
		while (idx != std::string::npos)
		{
			ret.erase(idx, 1);
			idx = ret.find_last_of(chars);
		}

		return ret;
	}
}

std::string replace(const std::string &str, const std::string &chars, char rep)
{
	std::string ret = str;

	for (std::string::size_type i = 0; i < ret.size(); i++)
	{
		for (std::string::size_type j = 0; j < chars.size(); j++)
		{
			if (ret[i] == chars[j])
			{
				ret[i] = rep;
				break;
			}
		}
	}

	return ret;
}

std::string extractFilename(const std::string& filename)
{
	std::string::size_type idx = filename.find_last_of("/\\");

	if (idx != std::string::npos)
	{
		return filename.substr(idx + 1, std::string::npos);
	}
	else
	{
		return filename;
	}
}



int toInt(char *istring)
{
	// validates the sent string as a int and converts it, if it's not valid
	// the function sends back INT_MIN, the chances of this being the number
	// validated is slim
	// [whitespace] [sign]digits

	char *string = istring;

	// must be of the form
	// [whitespace] 
	while(isspace(*string)) string++;

	// [sign] 
	if (*string=='+' || *string=='-') string++;

	// [digits] 
	while(isdigit(*string)) string++;

	// the string better be the same size as the other one
	if (strlen(istring) == (int)(string - istring))
		return(atoi(istring));
	else
		return(INT_MIN);
} // end IsInt

float toFloat(char *fstring)
{
	// validates the sent string as a float and converts it, if it's not valid
	// the function sends back FLT_MIN, the chances of this being the number
	// validated is slim
	// [whitespace] [sign] [digits] [.digits] [ {d | D | e | E }[sign]digits]

	char *string = fstring;

	// must be of the form
	// [whitespace] 
	while(isspace(*string)) string++;

	// [sign] 
	if (*string=='+' || *string=='-') string++;

	// [digits] 
	while(isdigit(*string)) string++;

	// [.digits] 
	if (*string =='.') 
	{
		string++;
		while(isdigit(*string)) string++;
	}

	// [ {d | D | e | E }[sign]digits]
	if (*string =='e' || *string == 'E' || *string =='d' || *string == 'D')
	{
		string++;

		// [sign] 
		if (*string=='+' || *string=='-') string++;

		// [digits] 
		while(isdigit(*string)) string++;
	} 

	// the string better be the same size as the other one
	if (strlen(fstring) == (int)(string - fstring))
		return(atof(fstring));
	else
		return(FLT_MIN);
}



uint32_t fromLittleEndian32(const unsigned char* buff)
{
	uint32_t Ret0 = buff[0];
	uint32_t Ret1 = buff[1];
	uint32_t Ret2 = buff[2];
	uint32_t Ret3 = buff[3];

	return (Ret3 << 24) | (Ret2 << 16) | (Ret1 << 8) | (Ret0);
}

uint16_t fromLittleEndian16(const unsigned char* buff)
{
	uint16_t Ret0 = buff[0];
	uint16_t Ret1 = buff[1];

	return (Ret1 << 8) | (Ret0);
}





