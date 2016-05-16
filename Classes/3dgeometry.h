#ifndef _3D_GEOMETRY_H_
#define _3D_GEOMETRY_H_



//opengl��һЩͷ�ļ�
#include "cocos2d.h"
#include "3dmath.h"
#include "3dcamera.h"
#include "fileutil.h"
#include <vector>
#include <stdlib.h>
#include <cstdio>
#include "3dmyplg.h"
#include "3dparser.h"
#include "3dbitmap.h"
//using namespace std;

USING_NS_CC;

struct Color32{
	Color32();
	Color32(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a); 
	union{
		int32_t  rgba;
		struct{
			GLubyte r;
			GLubyte g;
			GLubyte b;
			GLubyte a;
		};
		GLubyte rgba_M[4];
	};
	const static Color32 WHITE;
	const static Color32 YELLOW;
	const static Color32 BLUE;
	const static Color32 GREEN;
	const static Color32 RED;
	const static Color32 MAGENTA;
	const static Color32 BLACK;
	const static Color32 ORANGE;
	const static Color32 GRAY;
};

//����
struct Vertex2D{
	Vertex2D(){}
	Vertex2D(float _x, float _y);
	float x,y;
};

//�����
struct Polygon2D{
	int state;
	int x0,y0;
	int xv,yv;
	Color32 color; 
	std::vector<Vertex2D> vlist;

	void translate(int dx, int dy);
	void rotate(float theta);
	void scale(float sx, float sy);
};

#define VERTEX4D_ATTR_NULL             0x0000 // this vertex is empty
#define VERTEX4D_ATTR_POINT            0x0001 // ��������Ϣ��Ч
#define VERTEX4D_ATTR_NORMAL           0x0002 // ������Ϣ��Ч
#define VERTEX4D_ATTR_TEXTURE          0x0004 // ����������Ϣ��Ч

 

//����
struct Vertex4D{
	union{
		float M[12];
		struct
		{
			float x, y, z, w;     // point
			float nx, ny, nz, nw; // normal (vector or point)
			float u, v;           // texture coordinates 
			float i;    // final vertex intensity after lighting
			int   attr; // attributes / extra texture coordinates ��Щ��Ϣ��Ч
		};

		// high level types
		struct 
		{
			Point4D  point;  // the vertex point
			Vector4D normal; // the normal
			Point2D  coord;  // texture coordinates
		};
	};

};


///////////////////////////////////////////////////////Material////////////////////////////////////////////////////////

#define MATERIAL_ATTR_2SIDE							0x0001
#define MATERIAL_ATTR_TRANSPARENT					0x0002
#define MATERIAL_ATTR_RGB32							0x0010

#define MATERIAL_ATTR_SHADE_MODE_CONSTANT			0x0020 //�̶���ɫģʽ �޹���
#define MATERIAL_ATTR_SHADE_MODE_EMMISIVE			0x0020 // alias
#define MATERIAL_ATTR_SHADE_MODE_FLAT				0x0040 // �㶨��ɫģʽ �й���
#define MATERIAL_ATTR_SHADE_MODE_GOURAUD			0x0080
#define MATERIAL_ATTR_SHADE_MODE_FASTPHONG			0x0100
#define MATERIAL_ATTR_SHADE_MODE_TEXTURE			0x0200

#define MAX_MATERIALS								256
#define MATERIAL_STATE_ACTIVE                0x0001
#define MATERIAL_STATE_INACTIVE              0x0001

struct Material{
	int state;
	int id;
	char name[64];
	int attr;
	Color32 color; //������ɫ
	float ka ;//������ɢ��ϵ��
	float kd;//ɢ��ⷴ��ϵ��
	float ks;//����ⷴ��ϵ��
	float power;//���淴��ָ��
	Color32 ra,rd,rs;//Ԥ�ȼ������ɫ�뷴��ϵ���ĳ˻� 
	char texture_file[80];//texture ·��
};

struct MaterialList{
	static Material materials[MAX_MATERIALS];
	static int num_materials;
	static bool inited;
	static void reset();
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����������
enum TriType
{
	TRI_TYPE_NONE        = 0,
	TRI_TYPE_FLAT_TOP    = 1, // ƽ��
	TRI_TYPE_FLAT_BOTTOM = 2, // ƽ��
	TRI_TYPE_GENERAL     = 3, // ��ͨ
};

// �������߽��в�ֵ
enum InterpolationSide
{
	INTERP_LHS  = 0, // ���
	INTERP_RHS  = 1, // �ұ�
};


#define VERTEX_FLAGS_OVERRIDE_MASK          0xf000 // this masks these bits to extract them
#define VERTEX_FLAGS_OVERRIDE_CONSTANT      0x1000
#define VERTEX_FLAGS_OVERRIDE_EMISSIVE      0x1000 // (alias)
#define VERTEX_FLAGS_OVERRIDE_PURE          0x1000
#define VERTEX_FLAGS_OVERRIDE_FLAT          0x2000
#define VERTEX_FLAGS_OVERRIDE_GOURAUD       0x4000
#define VERTEX_FLAGS_OVERRIDE_TEXTURE       0x8000

#define VERTEX_FLAGS_INVERT_TEXTURE_U       0x0080  // invert u texture coordinate 
#define VERTEX_FLAGS_INVERT_TEXTURE_V       0x0100  // invert v texture coordinate
#define VERTEX_FLAGS_INVERT_SWAP_UV         0x0800  // swap u and v texture coordinates


//������װ��
#define TRIANGLE_STATE_NULL               0x0000
#define TRIANGLE_STATE_ACTIVE			0x0001//�Ƿ�ɼ�
#define TRIANGLE_STATE_CLIPPED		0x0002//�Ƿ񱻲ü�
#define TRIANGLE_STATE_BACKFACE		0x0004//�Ƿ���
#define TRIANGLE_STATE_LIT			0x0008 //������������


#define TRIANGLE_UNACTIVE_STATE(index)	((index==nullptr)||(!index->state&TRIANGLE_STATE_ACTIVE)||(index->state&(TRIANGLE_STATE_CLIPPED|TRIANGLE_STATE_BACKFACE)))

#define TRIANGLE_ATTR_NULL				0x0000
#define TRIANGLE_ATTR_2SIDE				0x0001
#define TRIANGLE_ATTR_TRANSPARENT		0x0002
#define TRIANGLE_ATTR_RGB32				0x0010
//#define TRIANGLE_ATTR_SHARE_VERTEX		0x0004 //�Ƿ�ͱ�������ι���������vert_list_local

#define TRIANGLE_ATTR_SHADE_MODE_PURE		0x0020 //�̶���ɫģʽ �޹���
#define TRIANGLE_ATTR_SHADE_MODE_CONSTANT   0x0020 // (alias)
#define TRIANGLE_ATTR_SHADE_MODE_FLAT       0x0040 // �㶨��ɫģʽ һ�����������е����ض�ʹ��һ�����ռ�����
#define TRIANGLE_ATTR_SHADE_MODE_GOURAUD    0x0080
#define TRIANGLE_ATTR_SHADE_MODE_PHONG      0x0100
#define TRIANGLE_ATTR_SHADE_MODE_FASTPHONG  0x0100
#define TRIANGLE_ATTR_SHADE_MODE_TEXTURE    0x0200 


#define TRIANGLE_ATTR_ENABLE_MATERIAL       0x0800 // use a real material for lighting
#define TRIANGLE_ATTR_DISABLE_MATERIAL      0x1000 // use basic color only for lighting (emulate version 1.0)


//�԰����������ݵ�������
struct Triangle{
	int state; //4
	int attr; //4
	Color32 color;//4 ԭʼ��ɫ
	/*
	������ɫ
	lit_color[0] Ϊflat ����
	lit_color[0~2] 
	*/
	Color32 lit_color[3]; //12

	BitmapImage* texture;
	int material;


	//�淨��
	Vector4D normal;//16
	//���߳���
	float	nlength; //4

	//ԭ�������� ���ܲ�ֻ������
	Vertex4D vert_list_local[3];//4
	//�任��Ķ���
	Vertex4D vert_list_trans[3];//48*3
	
	Triangle* next;
	Triangle* prev;
	
 
	bool init();

};

//�ͱ�������ι�����
struct TriangleRef{
	int state; //4
	int attr; //4
	Color32 color;//4 ԭʼ��ɫ

	/*
	������ɫ
	lit_color[0] Ϊflat ����
	lit_color[0~2] 
	*/
	Color32 lit_color[3]; //12

	//�淨��
	Vector4D normal;//16
	//���߳���
	float	nlength; //4

	struct BitmapImage* texture;
	int material;

	//ԭ�������� ���ܲ�ֻ������
	Vertex4D* vert_list_local;//4
	//�任��Ķ���
	Vertex4D* vert_list_trans;//48*3

	//����list
	Point2D* coord_list;
	
	//��������,ָ��𴦵�vert_list_local �� vert_list_trans������
	int vert_idx[3];//12
	int coord_idx[3];//12

	TriangleRef* next;//4
	TriangleRef* prev;//4

	TriangleRef();
	~TriangleRef();

	bool init(Vertex4D* vert_list_local,Vertex4D* vert_list_trans);
};


enum TransformMode{
	TRANSFORMMODE_LOCAL_ONLY			=0,
	TRANSFORMMODE_TRANS_ONLY			=1,
	TRANSFORMMODE_LOCAL_TO_TRANS		=2,
	TRANSFORMMODE_COPY_LOCAL_TRANS		=3
};



// defines for objects version 1
#define OBJECT4D_MAX_VERTICES           1024  // 64
#define OBJECT4D_MAX_POLYS              1024  // 128

// states for objects
#define OBJECT4D_STATE_NULL				0x0000
#define OBJECT4D_STATE_ACTIVE           0x0001
#define OBJECT4D_STATE_VISIBLE          0x0002 
#define OBJECT4D_STATE_CULLED           0x0004

//object4d attr��
#define OBJECT4D_ATTR_SINGLE_FRAME		0x0001
#define OBJECT4D_ATTR_MULTI_FRAME		0x0002
#define OBJECT4D_ATTR_TEXTURE			0x0004


// general culling flags
#define CULL_OBJECT_X_PLANE           0x0001 // cull on the x clipping planes
#define CULL_OBJECT_Y_PLANE           0x0002 // cull on the y clipping planes
#define CULL_OBJECT_Z_PLANE           0x0004 // cull on the z clipping planes
#define CULL_OBJECT_XYZ_PLANES        (CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE | CULL_OBJECT_Z_PLANE)


//COB ģ���ļ� ��
#define VERTEX_FLAGS_INVERT_X               0x0001  // inverts the Z-coordinates
#define VERTEX_FLAGS_INVERT_Y               0x0002  // inverts the Z-coordinates
#define VERTEX_FLAGS_INVERT_Z               0x0004  // inverts the Z-coordinates
#define VERTEX_FLAGS_SWAP_YZ                0x0008  // transforms a RHS model to a LHS model
#define VERTEX_FLAGS_SWAP_XZ                0x0010   
#define VERTEX_FLAGS_SWAP_XY                0x0020
#define VERTEX_FLAGS_INVERT_WINDING_ORDER   0x0040  // invert winding order from cw to ccw or ccw to cc


//����Objectģ�͵�ʱ�򣬽��б任�ñ��
#define VERTEX_FLAGS_TRANSFORM_LOCAL        0x0200  // if file format has local transform then do it!
#define VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD  0x0400  // if file format has local to world then do it!


struct Object4D{
	int id;
	char name[64];
	int state;
	int attr;//�Ƿ��涯��

	Point4D world_pos;
	Vector4D ux,uy,uz;

	//ÿ֡��������
	int num_vertices;
	int num_frames;
	//������Ϣ
	//��ǰ֡��Ϣ
	int curr_frame;

	//����frame ��ƽ���뾶
	float* avg_radius;
	//����frame�����뾶
	float* max_radius;

	Vertex4D* vert_list_local;
	Vertex4D* vert_list_trans;  

	//���ж���
	int total_vertices;
	Vertex4D* head_vert_list_local; // ����֡���������׵�ַ(ԭʼ)
	Vertex4D* head_vert_list_trans; // ����֡���������׵�ַ(�任)

	//����������
	int num_triangles; 
	//�������б�
	TriangleRef* triangle_ref_list;

	//����
	Point2D* coord_list;
	BitmapImage* texture;

	std::string* plxLine();
	std::vector<std::string> plxLines;
	std::vector<std::string>::size_type plxLineIdx;

	Object4D();
	~Object4D();

	bool init(int _num_vertices, int _num_triangles, int _num_frames, bool destroy);

	void destroy();
	void initBasis();
	void resetFlag();
	//�������Object ���� frame �İ뾶��Ϣ
	void computeRadius();
	void computeVertexNormals();
	void computePolyNormals();

	bool Object4D::cull(const Camera& camer,int cull_flag);

	// ������任(ֻ��������ת) TODO: ���任�������ƽ��ʱ ux,uy,uzҲ���ű任�Ļ���������
	void transformByMat(const Matrix_4X4 &mat, TransformMode mode, bool transform_basis, bool all_frames); 
	//TRANSFORMMODE_LOCAL_TO_TRANS
	void transformModelToWorld(TransformMode mode=TRANSFORMMODE_LOCAL_TO_TRANS, bool all_frames=true); 
	void transformWorldToCamera(const Camera& cam);
	void transformCameraToPerspective(const Camera& cam); 
	void transformPerspectiveToScreen(const Camera& cam);

	// ���ļ�����ģ��, TODO: ��δ��תrot   
	bool loadFromPLX1(const std::string &filename, const Vector4D &scale, const Point4D &pos,const Vector4D &rot); 

	// ����ASCģ�� ��֡ ������ �޲��� 
	// TODO: rot������δ�õ�
	bool loadFromASC(const std::string &filename, const Vector4D &scale, const Point4D &pos, const Vector4D &rot, int vertex_flags); 

	// ����COBģ�� ��֡ ��������
	// TODO: rot������δ�õ�
	bool loadFormCOB(const std::string &filename, const Vector4D &scale, const Point4D &pos, const Vector4D &rot, int vertex_flags); 


};




// defines for light types ��Դ����
#define LIGHT_ATTR_AMBIENT      0x0001    // basic ambient light ������Դ
#define LIGHT_ATTR_INFINITE     0x0002    // infinite light source ����Զ��Դ(�����Դ��ƽ�й�Դ) ����ĳ����ƽ�еش�����Զ������ ǿ�Ȳ������ž���˥�� ������̫�� P487
#define LIGHT_ATTR_DIRECTIONAL  0x0002    // infinite light source (alias)
#define LIGHT_ATTR_POINT        0x0004    // point light source ���Դ
#define LIGHT_ATTR_SPOTLIGHT1   0x0008    // spotlight type 1 (simple)  �۹�ƹ�Դ(�ǳ���)
#define LIGHT_ATTR_SPOTLIGHT2   0x0010    // spotlight type 2 (complex) �۹�ƹ�Դ(��)
#define LIGHT_ATTR_TYPEMASK     0x001F    // ��Դ��������

struct Light {
	bool state;
	int id;
	int attr;

	//������ǿ��
	Color32 c_ambient;
	//ɢ��⣨ƽ�й�Դ�����Դ��
	Color32 c_diffuse;
	//���淴���ǿ��
	Color32 c_specular;

	//��������ϵ�е�λ�úͷ���
	Point4D		pos_world;
	Vector4D	dir_world;

	//�������ϵ�е�λ�úͷ���
	Point4D		pos_camera;
	Vector4D	dir_camera;

	//˥������ ���ڵ��Դ���۹��
	float kc,kl,kq;

	float spot_inner;
	float spot_outer;

	int pf;
};

#define MAX_LIGHTS			8

struct LightList{
	Light lights[MAX_LIGHTS];
	int lights_num;
	LightList():lights_num(0){}
	void reset();
	bool initLight(int index,bool state, int attr,
		Color32 c_ambient,
		Color32 c_diffuse,
		Color32 c_specular,
		const Point4D* pos,
		const Vector4D* dir,
		float kc,
		float kl,
		float kq,
		float spot_inner,
		float spot_outer,
		int pf);
};



enum SortPolyMethod
{
	SORT_POLY_METHOD_AVGZ  = 0, // sorts on average of all vertices
	SORT_POLY_METHOD_NEARZ = 1, // sorts on closest z vertex of each poly
	SORT_POLY_METHOD_FARZ  = 2, // sorts on farthest z vertex of each poly
};


//RenderList�������ε�������
#define RENDERLIST4D_MAX_TRIANGLES 32768

//struct Object4D;
struct LightList;
struct RenderList4D{
	int state;
	int attr;

	//��������ָ��ָ�� triangle_data
	Triangle* triangle_ptrs[RENDERLIST4D_MAX_TRIANGLES];

	Triangle triangle_data[RENDERLIST4D_MAX_TRIANGLES];
	//��ǰ����ε�����
	int num_triangles;

	//void RenderList4D();

	//�������������
	void reset();
	bool insert(const Triangle& triangle);
	bool insert(const TriangleRef& triangle);
	bool insert(const Object4D& object);

	void removeBackface(const Camera camera);
	void sortZ(SortPolyMethod method);
	void LightInWorld(Camera& camera,LightList& light_list);

	void transformByMat(const Matrix_4X4& mat,TransformMode mode);
	void transformModelToWorld(const Point4D &world_pos, TransformMode mode = TRANSFORMMODE_LOCAL_TO_TRANS);
	void transformWorldToCamera(const Camera& camera);
	void transformCameraToPerspective(const Camera& cam);
	void transformPerspectiveToScreen(const Camera &cam);
	
};



#endif