#ifndef _3D_CAMERA_H_
#define _3D_CAMERA_H_
#include "3dmath.h"

//�������
#define CAM_MODEL_EULER            0x0008
#define CAM_MODEL_UVN              0x0010

enum CamRotSeq{
	CAM_ROT_SEQ_XYZ=0,
	CAM_ROT_SEQ_YXZ=1,
	CAM_ROT_SEQ_XZY=2,
	CAM_ROT_SEQ_YZX=3,
	CAM_ROT_SEQ_ZYX=4,
	CAM_ROT_SEQ_ZXY=5,
};

//UVN���ģʽ
enum UVNMode{
	UVN_MODE_SIMPLE		=0,//��ģʽ,ʹ��Ŀ��λ�ú͹۲�ο���(�����λ��)
	//���������ʱ��ʵ��
	UVN_MODE_SPHERICAL = 1, // ����ģʽ,�۲�ο���P=(p, y����, z����)
};

struct Camera {
	int state;
	int attr;

	//λ��
	Point4D pos;

	//UVN�����
	//Euler���:
	Vector4D dir;


	//����4������ uvn�����
	Vector4D u;
	Vector4D v;
	Vector4D n;
	Vector4D target;

	float view_dist;

	//����Ƕ� ������Ƕ�
	float fov;


	//��ƽ�桢�ü������Ϣ
	float near_clip_z;
	float far_clip_z;
	
	Plane3D r_clip_plane;
	Plane3D l_clip_plane;
	Plane3D t_clip_plane;
	Plane3D b_clip_plane;
	
	//��ƽ��� �߶�
	float viewplane_width;
	float viewplane_height;

	//�ӿ���Ϣ
	float viewport_width;
	float viewport_height;
	float viewport_center_x;
	float viewport_center_y;

	//�ӿڿ�߱�
	float aspect_ratio;

	//��������->�������
	Matrix_4X4 mat_cam;
	//�������->͸������(����ƽ���ϵ��߼�����)
	Matrix_4X4 mat_per;
	//͸������->��Ļ����(����Ļ�ϵ���������)
	Matrix_4X4 mat_scr;

	void init(int cam_attr, //�������,CAM_MODEL_EULER��CAM_MODEL_UVN
				const Point4D& cam_pos,//���λ��
				const Vector4D& cam_dir,//�������,����������ŷ���Ƕ�(������Ϊ�Ƹ�����ת�Ƕ�)
				const Point4D* cam_target,//UVN��� �۲췽��

				float near_clip_z,//
				float far_clip_z,
				float fov,//����fovֵ���������Ҫ���ݿ�߱ȼ���
				
				float viewport_width,
				float viewport_height
				);

	void buildCameraMatByEuler(CamRotSeq seq);
	void buildCameraMatByUVN(UVNMode mode=UVN_MODE_SIMPLE);
	void buildPerspectiveMat();
	void buildScreenMat4D();
	void buildScreenMat();


};

#endif