#include "3dcamera.h"

void Camera::init(int cam_attr, const Point4D& cam_pos, const Vector4D& cam_dir, const Point4D* cam_target, 
	float near_clip_z, float far_clip_z, float fov /*�Ƕ�*/, float viewport_width, float viewport_height ){

		//�������
		this->attr=cam_attr;

		this->pos=cam_pos;
		//������������ŷ���Ƕ�
		this->dir=cam_dir;

		this->u.init(1,0,0);
		this->v.init(0,1,0);
		this->n.init(0,0,1);


		if (cam_target!=nullptr){
			this->target=*cam_target;
		}else{
			this->target.zero();
		}

		this->near_clip_z=near_clip_z;
		this->far_clip_z=far_clip_z;

		this->viewport_width=viewport_width;
		this->viewport_height=viewport_height;

		this->viewport_center_x=(viewport_width-1)/2.0f;
		this->viewport_center_y=(viewport_height-1)/2.0f;

		this->aspect_ratio=viewport_width/viewport_height;//��߱�

		this->mat_cam.identify();
		this->mat_per.identify();
		this->mat_scr.identify();

		//����Ƕ�
		this->fov=fov;
		this->viewplane_width=2.0f;
		/*
					 viewport_width   viewplane_width
		aspect_ratio=---------------=----------------
					 viewport_height  viewplane_height
		*/
		this->viewplane_height=2.0f/aspect_ratio;
		
		float tan_fov_div2=tan(degreeToRadian(fov/2.0f));
		//�Ӿ�
		this->view_dist=this->viewplane_width*0.5f/tan_fov_div2;

		if (fov==90.0){
			//����ȷ��������ƽ��
			//ƽ���ϵ�һ��
			Point3D pt_origin;
			pt_origin.init(0.0f,0.0f,0.0f);

			Vector3D vn;

			vn.init(1.0f,0.0f,-1.0f);//������
			this->r_clip_plane.init(pt_origin,vn,true);
			
			vn.init(-1.0f,0.0f,-1.0f);
			this->l_clip_plane.init(pt_origin,vn,true);

			//��������ط�Ӧ���Ǵ����
			//top
			vn.init(0.0f,1.0f,-1.0f/this->aspect_ratio);
			this->t_clip_plane.init(pt_origin, vn, true);

			//bottom
			vn.init(0.0f, -1.0f, -1.0f / this->aspect_ratio);
			this->b_clip_plane.init(pt_origin, vn, true);
		}else{
			Point3D pt_origin;
			pt_origin.init(0.0f, 0.0f, 0.0f);

			Vector3D vn;
			
			vn.init(this->view_dist, 0.0f, -this->viewplane_width / 2.0f); 
			this->r_clip_plane.init(pt_origin, vn, true);

			vn.init(-this->view_dist, 0.0f, -this->viewplane_width / 2.0f); 
			this->l_clip_plane.init(pt_origin, vn, true);

			vn.init(0.0f, this->view_dist, -this->viewplane_height / 2.0f); // vn.Init(0.0f, this->view_dist, -this->viewplane_width / 2.0f); TODO: �д�ȷ�� 
			this->t_clip_plane.init(pt_origin, vn, true);

			vn.init(0.0f, -this->view_dist, -this->viewplane_height / 2.0f); // vn.Init(0.0f, -this->view_dist, -this->viewplane_width / 2.0f); TODO: �д�ȷ��
			this->b_clip_plane.init(pt_origin, vn, true);
		}
}

//ŷ�����
void Camera::buildCameraMatByEuler(CamRotSeq seq){
	
	Matrix_4X4 mt_inv; //���XYZƽ�������
	Matrix_4X4 mx_inv; //���X����ת�����
	Matrix_4X4 my_inv; //���Y����ת�����
	Matrix_4X4 mz_inv; //���Z����ת�����

	Matrix_4X4 mrot;   //��������ת����Ļ�
	Matrix_4X4 mtmp;   //��ʱ����


	mt_inv.init(1,		0,		0,		0,
				0,		1,		0,		0,
				0,		0,		1,		0,
				-pos.x, -pos.y, -pos.z, 1);

	//����Ϊ��λ
	float theta_x =this->dir.x;
	float theta_y=this->dir.y;
	float theta_z=this->dir.z;
	
	float cos_theta_x=cosf(theta_x);
	float sin_theta_x=-sinf(theta_x);
	mx_inv.init(
		1,    0,           0,           0,
		0,    cos_theta_x, sin_theta_x, 0,
		0,   -sin_theta_x, cos_theta_x, 0,
		0,    0,           0,           1);


	float cos_theta_y = cosf(theta_y);
	float sin_theta_y = -sinf(theta_y);
	my_inv.init(
		cos_theta_y, 0, -sin_theta_y, 0,
		0,           1,  0,           0,
		sin_theta_y, 0,  cos_theta_y, 0,
		0,           0,  0,           1);


	float cos_theta_z = cosf(theta_z);
	float sin_theta_z = -sinf(theta_z);
	mz_inv.init(
		cos_theta_z, sin_theta_z, 0, 0,
		-sin_theta_z, cos_theta_z, 0, 0,
		0,           0,           1, 0,
		0,           0,           0, 1);

	switch (seq)
	{
	case CAM_ROT_SEQ_XYZ:
		mx_inv.mulTo(my_inv, mtmp);
		mtmp.mulTo(mz_inv, mrot);
		break;

	case CAM_ROT_SEQ_YXZ:
		my_inv.mulTo(mx_inv, mtmp);
		mtmp.mulTo(mz_inv, mrot);
		break;

	case CAM_ROT_SEQ_XZY:
		mx_inv.mulTo(mz_inv, mtmp);
		mtmp.mulTo(my_inv, mrot);
		break;

	case CAM_ROT_SEQ_YZX:
		my_inv.mulTo(mz_inv, mtmp);
		mtmp.mulTo(mx_inv, mrot);
		break;

	case CAM_ROT_SEQ_ZYX:		
		mz_inv.mulTo(my_inv, mtmp);
		mtmp.mulTo(mx_inv, mrot);
		break;

	case CAM_ROT_SEQ_ZXY:		
		mz_inv.mulTo(mx_inv, mtmp);
		mtmp.mulTo(my_inv, mrot);
		break;

	default: 
		break;
	}

	mt_inv.mulTo(mrot, this->mat_cam);
}


void Camera::buildCameraMatByUVN(UVNMode mode){
	
	Matrix_4X4 mt_inv;
	Matrix_4X4 mt_uvn;

	mt_inv.init(1,		0,		0,		0,
				0,		1,		0,		0,
				0,		0,		1,		0,
			-pos.x,-pos.y,	-pos.z,		1);

	//n=target-pos
	Vector4D::build(this->pos,this->target,this->n);

	this->v.init(0,1,0);

	//ȷ��������
	this->v.crossTo(this->n,this->u);


	//ȷ��������
	this->n.crossTo(this->u,this->v);

	this->u.normalize();
	this->v.normalize();
	this->n.normalize();

	mt_uvn.init(
		this->u.x,   this->v.x,    this->n.x,    0,
		this->u.y,   this->v.y,    this->n.y,    0,
		this->u.z,   this->v.z,    this->n.z,    0,
		0,           0,            0,            1);

	mt_inv.mulTo(mt_uvn,this->mat_cam);
}





