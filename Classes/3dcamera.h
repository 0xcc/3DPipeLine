#ifndef _3D_CAMERA_H_
#define _3D_CAMERA_H_
#include "3dmath.h"

//相机类型
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

//UVN相机模式
enum UVNMode{
	UVN_MODE_SIMPLE		=0,//简单模式,使用目标位置和观察参考点(即相机位置)
	//下面这个暂时不实现
	UVN_MODE_SPHERICAL = 1, // 球面模式,观察参考点P=(p, y分量, z分量)
};

struct Camera {
	int state;
	int attr;

	//位置
	Point4D pos;

	//UVN相机：
	//Euler相机:
	Vector4D dir;


	//下面4个变量 uvn相机用
	Vector4D u;
	Vector4D v;
	Vector4D n;
	Vector4D target;

	float view_dist;

	//横向角度 和纵向角度
	float fov;


	//视平面、裁剪面等信息
	float near_clip_z;
	float far_clip_z;
	
	Plane3D r_clip_plane;
	Plane3D l_clip_plane;
	Plane3D t_clip_plane;
	Plane3D b_clip_plane;
	
	//视平面宽 高度
	float viewplane_width;
	float viewplane_height;

	//视口信息
	float viewport_width;
	float viewport_height;
	float viewport_center_x;
	float viewport_center_y;

	//视口宽高比
	float aspect_ratio;

	//世界坐标->相机坐标
	Matrix_4X4 mat_cam;
	//相机坐标->透视坐标(在视平面上的逻辑坐标)
	Matrix_4X4 mat_per;
	//透视坐标->屏幕坐标(在屏幕上的物理坐标)
	Matrix_4X4 mat_scr;

	void init(int cam_attr, //相机类型,CAM_MODEL_EULER或CAM_MODEL_UVN
				const Point4D& cam_pos,//相机位置
				const Vector4D& cam_dir,//相机朝向,方向向量或欧拉角度(各分量为绕各轴旋转角度)
				const Point4D* cam_target,//UVN相机 观察方向

				float near_clip_z,//
				float far_clip_z,
				float fov,//横向fov值，纵向的需要根据宽高比计算
				
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