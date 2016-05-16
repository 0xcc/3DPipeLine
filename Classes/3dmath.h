#ifndef _3D_MATH_H_
#define _3D_MATH_H_

#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdint.h>

//����
const float PI         = 3.141592654f; // 180��
const float PI2        = 6.283185307f; // 360��
const float PI_DIV_2   = 1.570796327f; // 90��
const float PI_DIV_3   = 1.0471975511965977461542144610932f; // 60��
const float PI_DIV_4   = 0.785398163f; // 45��
const float PI_DIV_6   = 0.52359877559829887307710723054658f; // 30��
const float PI_DIV_12  = 0.261799387f; // 15��
const float PI_DIV_30  = 0.10471975511965977461542144610932f; // 6��
const float PI_DIV_180 = 0.017453292f; // 1��
const float PI_DIV_360 = 0.008726646f; // 0.5��
const float PI_INV     = 0.318309886f; // 1/PI

// defines for small numbers
const float EPSILON_E3 = 1E-3f;
const float EPSILON_E4 = 1E-4f;
const float EPSILON_E5 = 1E-5f;
const float EPSILON_E6 = 1E-6f;


//�߶��ཻ����
enum ParmLineIntersect{
	PARM_LINE_INTERSECT_NO          = 0, // û���ཻ
	PARM_LINE_INTERSECT_IN_SEGMENT  = 1, // �������߶���
	PARM_LINE_INTERSECT_OUT_SEGMENT = 2, // �������߶���
	PARM_LINE_INTERSECT_EVERYWHERE  = 3, // �����ཻ(���߶���ͬһ��ֱ����)		
};

// fixed point
#define FIXP16_SHIFT     0          // λ����
#define FIXP16_MAG       65536      // �൱��1
#define FIXP16_DP_MASK   0x0000ffff // С����������
#define FIXP16_WP_MASK   0xffff0000 // ������������
#define FIXP16_ROUND_UP  0x00008000 // �൱��0.5


//2D ����
struct Vector2D{
	void init(float _x, float _y);
	void init(const Vector2D &p0, const Vector2D &p1);

	union
	{
		//������ʽ
		float M[2]; 
		//x,y��ʽ
		struct  
		{
			float x, y;
		};

		//������ʽ
		struct 
		{
			float u, v;
		};
	};

	void zero();
	void normalize();//��һ�� length(vector2d)=1

	//this+other-->result
	void addTo(const Vector2D &other, Vector2D &result);
	Vector2D add(const Vector2D &other);

	void subTo(const Vector2D &other, Vector2D &result);
	Vector2D sub(const Vector2D &other);

	void scaleTo(float k, Vector2D &result);
	void scale(float k);

	float dot(const Vector2D &other); // ����
	float length() const;// �󳤶�

	//���� �����������ļн�
	float cosTh(const Vector2D &other);
};

typedef Vector2D Point2D;


struct Matrix_3X3;
struct Matrix_4X4;
struct Matrix_4X3;
struct Vector3D{
	void init(float _x, float _y, float _z);
	void init(const Vector3D &p0, const Vector3D &p1);

	union
	{
		float M[3]; 
		struct 
		{
			float x, y, z;
		};
	};

	void zero();
	void normalize(); // ��һ��(ʹ����Ϊ1)
	void normalizeTo(Vector3D &result) const;

	void addTo(const Vector3D &other, Vector3D &result);
	Vector3D add(const Vector3D &other);

	void subTo(const Vector3D &other, Vector3D &result);
	Vector3D sub(const Vector3D &other);

	// ��Vector3D��Matrix_3X3���
	void mulMat3X3To(const Matrix_3X3 &other, Vector3D &result); 
	// ��Vector3D����Vector4D��Matrix_4X4���, ���Ҽ���this->w=1
	void mulMat4X4To(const Matrix_4X4 &other, Vector3D &result); 

	void scaleTo(float k, Vector3D &result);
	void scale(float k);

	// ���
	float dot(const Vector3D &other); 
	//���
	void crossTo(const Vector3D &ohter, Vector3D &result);
	Vector3D cross(const Vector3D &other);
	//����
	float length() const; 
	// ��н����� 
	float cosTh(const Vector3D &other); 

};

typedef Vector3D Point3D;

struct Vector4D
{
	void init(float _x, float _y, float _z, float _w);
	void init(float _x, float _y, float _z);

	union
	{
		float M[4]; // array indexed storage

		struct // explicit names
		{
			float x, y, z, w;
		};
	};

	void zero();
	// ��һ��
	void normalize(); 
	void normalizeTo(Vector4D &result);

	void addTo(const Vector4D &other, Vector4D &result);
	Vector4D add(const Vector4D &other);

	void subTo(const Vector4D &other, Vector4D &result);
	Vector4D sub(const Vector4D &other);

	// ��Matrix_4X4���
	void mulMat4X4To(const Matrix_4X4 &other, Vector4D &result);
	// ��Matrix_4X3���, ����otherȱ�ٵ�һ��Ϊ[0,0,0,1]
	void mulMat4X3To(const Matrix_4X3 &other, Vector4D &result); 

	void divByW();

	void scaleTo(float k, Vector4D &result);
	void scale(float k);

	float dot(const Vector4D &other);
	void crossTo(const Vector4D &other, Vector4D &result);
	Vector4D cross(const Vector4D &other);
	float length() const;
	float lengthFast2();
	float cosTh(const Vector4D &other);

	static void build(const Vector4D &init, const Vector4D &term, Vector4D &result);

	static const Vector4D ZERO;
	static const Vector4D ONE;
};
typedef Vector4D Point4D;


struct Plane3D{
	Point3D p0; //ƽ���ϵĵ�
	Vector3D normal; //������

	void init(const Point3D &_p0, const Vector3D &_normal, bool normalize);

	/*
		��ƽ���Ϸ���	=0
		����ռ�		>0
		����ռ�		<0
	*/
	float pointWhere(const Point3D &point);


};


struct Matrix_1X4
{
	union
	{
		float M[4]; // array indexed data storage

		struct // storage in row major form with explicit names
		{
			float M00, M01, M02, M03;
		};
	};

	//void mulMat4X4To(const Matrix_4X4 &other, Matrix_1X4 &result);
};


// 3x3 matrix
struct Matrix_3X3
{
	void init(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);

	union
	{
		float M[3][3]; // array indexed data storage

		struct // storage in row major form with explicit names
		{
			float M00, M01, M02;
			float M10, M11, M12;
			float M20, M21, M22;
		};
	};

	void zero();
	void identify();
	void transpose(); // ת��
	void transposeTo(Matrix_3X3 &mat);
	//void setColumn(int col, const Matrix_1X3 &mat);
	//void setColumn(int col, const Matrix_3X1 &mat);

	void addTo(const Matrix_3X3 &other, Matrix_3X3 &result);
	void mulTo(const Matrix_3X3 &other, Matrix_3X3 &result);
	float det() const; // ����ʽ
	bool inverseTo(Matrix_3X3 &result); // �����

	static const Matrix_3X3 IDENTITY; 
};


// 4x3 matrix
struct Matrix_4X3
{
	union
	{
		float M[4][3]; // array indexed data storage

		struct // storage in row major form with explicit names
		{
			float M00, M01, M02;
			float M10, M11, M12;
			float M20, M21, M22;
			float M30, M31, M32;
		};
	};

	void zero();
	void identify();
	void setColumn(int col, const Matrix_1X4 &mat);

	static const Matrix_4X3 IDENTITY;
};

// 4x4 matrix
struct Matrix_4X4
{
	void init(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

	union
	{
		float M[4][4]; // array indexed data storage

		struct // storage in row major form with explicit names
		{
			float M00, M01, M02, M03;
			float M10, M11, M12, M13;
			float M20, M21, M22, M23;
			float M30, M31, M32, M33;
		};
	};

	void zero();
	void identify();
	void transpose(); // ת��
	void transposeTo(Matrix_4X4 &mat);
	void setColumn(int col, const Matrix_1X4 &mat);

	void addTo(const Matrix_4X4 &other, Matrix_4X4 &result);
	void mulTo(const Matrix_4X4 &other, Matrix_4X4 &result);
	//����� A X inv(A) ==id����
	bool inverseTo(Matrix_4X4 &result); // �������,�ú��������������һ��Ϊ[0,0,0,1]�ľ���

	// λ������
	void buildModelToWorld(const Vector4D &world_pos);

	// ŷ����ת����
	void buildEulerRotation(float theta_x, float theta_y, float theta_z);

	static const Matrix_4X4 IDENTITY;
};


//�Ƕ�-->����
inline float degreeToRadian(float degree){
	return degree*(PI/180.0f);
}

//����->�Ƕ�
inline float radianToDegree(float radian){
	return radian * 180.0f / PI;
}

//[a,b)�����
inline int randRange(int a,int b){
	return a + rand() % (b - a + 1);
}

// floating point comparison ���������
inline bool floatEqual(float a, float b)
{
	return fabs(a - b) < EPSILON_E3;
}

// ��λ
inline void setBit(int &flag, int bit)
{
	flag |= bit;
}

// ����λ
inline void resetBit(int &flag, int bit)
{
	flag &= ~bit;
}

// ��str��sep�ָ�str��ret
void split(const std::string &str, const std::string &sep, std::vector<std::string>& ret);

// ȥ���հ��ַ�
std::string trim(const std::string &str);
std::string trim_left(const std::string &str);
std::string trim_right(const std::string &str);

// ��str��ȥ��chars���������ַ�
std::string strip(const std::string &str, const std::string &chars);

// ��str������chars���������ַ��滻Ϊrep
std::string replace(const std::string &str, const std::string &chars, char rep);

// ������·������ȡ�ļ���
std::string extractFilename(const std::string& filename);

// ��������
int toInt(char *istring);
float toFloat(char *fstring);


// ȡBuff�е�С������
uint32_t fromLittleEndian32(const unsigned char* buff);
uint16_t fromLittleEndian16(const unsigned char* buff);


template <typename T> void swap(T& a,T& b){
	T c(a);
	a=b;
	b=c;
}


#endif