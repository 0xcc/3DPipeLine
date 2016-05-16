#include "3dgeometry.h"


/////////////////////////////////Color32//////////////////
Color32::Color32():r(0),g(0),b(0),a(0){

}

Color32::Color32(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a):r(_r),g(_g),b(_b),a(_a){

}
const Color32 Color32::WHITE  (255, 255, 255, 255);
const Color32 Color32::YELLOW (255, 255,   0, 255);
const Color32 Color32::GREEN  (  0, 255,   0, 255);
const Color32 Color32::BLUE   (  0,   0, 255, 255);
const Color32 Color32::RED    (255,   0,   0, 255);
const Color32 Color32::MAGENTA(255,   0, 255, 255);
const Color32 Color32::BLACK  (  0,   0,   0, 255);
const Color32 Color32::ORANGE (255, 127,   0, 255);
const Color32 Color32::GRAY   (166, 166, 166, 255);

//////////////////////////////Triangle////////////////////////////////////
TriangleRef::TriangleRef(){
	this->vert_list_local=static_cast<Vertex4D*> (malloc(3*sizeof(Vertex4D)));
	//this->attr=TRIANGLE_ATTR_SHARE_VERTEX;
	this->vert_idx[0]=0;
	this->vert_idx[1]=1;
	this->vert_idx[2]=2;
}

TriangleRef::~TriangleRef(){
	this->vert_list_local=nullptr;
}


bool TriangleRef::init(Vertex4D* vert_list_local,Vertex4D* vert_list_trans){
	this->vert_list_local=vert_list_local;
	this->vert_list_trans=vert_list_trans;
	return true;
}

/////////////////////////////////////MaterialList///////////////////////////////////////////////

Material MaterialList::materials[MAX_MATERIALS];
int MaterialList::num_materials = 0;
bool MaterialList::inited = false;

void MaterialList::reset()
{
	// this function resets all the materials

	// if this is the first time then zero EVERYTHING out
	if (!inited)
	{
		memset(materials, 0, MAX_MATERIALS * sizeof(Material));
		inited = true;
	}
	else
	{
		// scan thru materials and release all textures, if any?
		for (int curr_matt = 0; curr_matt < MAX_MATERIALS; curr_matt++)
		{

			memset(&materials[curr_matt], 0, sizeof(Material));
		}
 
	}
	 
	num_materials = 0;
}



////////////////////////////////Light///////////////////////////

void LightList::reset(){
	memset(lights,0,MAX_LIGHTS*sizeof(Light));
	lights_num=0;
}

bool LightList::initLight(int index,bool state, int attr, 
	Color32 c_ambient, Color32 c_diffuse, Color32 c_specular, 
	const Point4D* pos, const Vector4D* dir, 
	float kc, float kl, float kq, 
	float spot_inner, float spot_outer, int pf){

		if (index<0||index>=MAX_LIGHTS){
			return false;
		}

		lights[index].state=state;
		lights[index].id=index;
		lights[index].attr=attr;

		lights[index].c_ambient	=c_ambient;
		lights[index].c_diffuse=c_diffuse;
		lights[index].c_specular=c_specular;

		if (pos){
			lights[index].pos_world=*pos;
		}

		if (dir){
			lights[index].dir_world=*dir;
			lights[index].dir_world.normalize();
		}

		lights[index].kc=kc;
		lights[index].kl=kl;
		lights[index].kq=kq;

		lights[index].spot_inner=spot_inner;
		lights[index].spot_outer=spot_outer;
		lights[index].pf=pf;
		return true;
}



//////////////////////////////RenderList4D////////////////////////////////

void RenderList4D::reset(){
	
	this->num_triangles=0;
	//memset()
}


bool RenderList4D::insert(const TriangleRef& triangle_ref){

	if (this->num_triangles>=RENDERLIST4D_MAX_TRIANGLES){
		return false;
	}
	this->triangle_ptrs[this->num_triangles]=&this->triangle_data[this->num_triangles];
	if (this->num_triangles==0){
		this->triangle_data[0].next=this->triangle_data[0].prev=nullptr;
	}else{
		this->triangle_data[this->num_triangles].next=nullptr;
		this->triangle_ptrs[this->num_triangles]->prev=&this->triangle_data[this->num_triangles-1];

		this->triangle_data[this->num_triangles-1].next=&this->triangle_data[this->num_triangles];
	}
	
	Triangle* curr_triangle=&this->triangle_data[this->num_triangles];

	curr_triangle->attr=triangle_ref.attr;
	curr_triangle->state=triangle_ref.state;
	curr_triangle->color=triangle_ref.color;
	memcpy(curr_triangle->lit_color,triangle_ref.lit_color,sizeof(Color32)*3);
	//curr_triangle->next=nullptr;
	curr_triangle->nlength=triangle_ref.nlength;
	curr_triangle->normal=triangle_ref.normal;

	curr_triangle->material=triangle_ref.material;
	curr_triangle->texture=triangle_ref.texture;


	int index0=triangle_ref.vert_idx[0];
	int index1=triangle_ref.vert_idx[1];
	int index2=triangle_ref.vert_idx[2];

	Vertex4D vertex0=triangle_ref.vert_list_local[index0];
	Vertex4D vertex1=triangle_ref.vert_list_local[index1];
	Vertex4D vertex2=triangle_ref.vert_list_local[index2];

	curr_triangle->vert_list_local[0]=vertex0;

	int coordidx0=triangle_ref.coord_idx[0];
	int coordidx1=triangle_ref.coord_idx[1];
	int coordidx2=triangle_ref.coord_idx[2];

	curr_triangle->vert_list_local[0]=vertex0;
	curr_triangle->vert_list_local[0].coord=triangle_ref.coord_list[coordidx0 ];

	curr_triangle->vert_list_local[1]=vertex1;
	curr_triangle->vert_list_local[1].coord=triangle_ref.coord_list[coordidx1 ];

	curr_triangle->vert_list_local[2]=vertex2;
	curr_triangle->vert_list_local[2].coord=triangle_ref.coord_list[coordidx2];
	

	curr_triangle->vert_list_trans[0]=triangle_ref.vert_list_trans[index0];
	curr_triangle->vert_list_trans[0].coord= triangle_ref.coord_list[coordidx0];

	curr_triangle->vert_list_trans[1]=triangle_ref.vert_list_trans[index1];
	curr_triangle->vert_list_trans[1].coord=triangle_ref.coord_list[coordidx1];

	curr_triangle->vert_list_trans[2]=triangle_ref.vert_list_trans[index2];
	curr_triangle->vert_list_trans[2].coord=triangle_ref.coord_list[coordidx2];
	
	this->num_triangles++;

	switch(this->num_triangles){
	case 80:
	case 83:
	case 95:
	case 107:
		CCLOG("insert");

		break;
	}
	
	return true;
}

bool RenderList4D::insert(const Triangle& triangle){
	if (this->num_triangles>=RENDERLIST4D_MAX_TRIANGLES){
		return 0;
	}
	this->triangle_ptrs[this->num_triangles]=&this->triangle_data[this->num_triangles];
	memcpy(&this->triangle_data[num_triangles],&triangle,sizeof(Triangle));
	
	if (this->num_triangles==0){
		this->triangle_data[0].next=this->triangle_data[0].prev=nullptr;
	}else{
		this->triangle_data[this->num_triangles].next=nullptr;
		this->triangle_ptrs[this->num_triangles]->prev=&this->triangle_data[this->num_triangles-1];

		this->triangle_data[this->num_triangles-1].next=&this->triangle_data[this->num_triangles];
	}
	this->num_triangles++;
	return true;
}


//这个函数需要改正，会导致ptr指向不正确
bool RenderList4D::insert(const Object4D& object){
	if (this->num_triangles>=RENDERLIST4D_MAX_TRIANGLES){
		return false;
	}
 
	if (!(object.state&OBJECT4D_STATE_ACTIVE)||
		(object.state&OBJECT4D_STATE_CULLED)||
		!(object.state&OBJECT4D_STATE_VISIBLE)){
			return false;
	}

	for(int index=0;index<object.num_triangles;index++){
		TriangleRef& curr_triangle=object.triangle_ref_list[index];
		if (!(curr_triangle.state&TRIANGLE_STATE_ACTIVE)||
			(curr_triangle.state&TRIANGLE_STATE_CLIPPED)||
			(curr_triangle.state&TRIANGLE_STATE_BACKFACE)){
				continue;
		}
		//curr_triangle.vert_list_trans[0]=object.vert_list_local[curr_triangle.vert_idx[0]];
		//curr_triangle.vert_list_trans[1]=object.vert_list_local[curr_triangle.vert_idx[1]];
		//curr_triangle.vert_list_trans[2]=object.vert_list_local[curr_triangle.vert_idx[2]];
		
		 

		insert(curr_triangle);
		//this->triangle_ptrs[this->num_triangles]=&this->triangle_data[this->num_triangles];
	}
	return true;
}

//世界坐标系中 踢出背面
void RenderList4D::removeBackface(const Camera camera){
	for (int index=0;index<this->num_triangles;index++){
		Triangle* curr_triangle=this->triangle_ptrs[index];

		if (TRIANGLE_UNACTIVE_STATE(curr_triangle)||(curr_triangle->attr&TRIANGLE_ATTR_2SIDE)){
			continue;
		}

		Vector4D u,v,n;

		Vector4D::build(curr_triangle->vert_list_trans[0].point,curr_triangle->vert_list_trans[1].point,u);
		Vector4D::build(curr_triangle->vert_list_trans[0].point,curr_triangle->vert_list_trans[2].point,v);
		u.crossTo(v,n);

		Vector4D view;
		//Vector4D::build(curr_triangle->vert_list_trans[0].point,camera.pos,view);
		Vector4D::build(curr_triangle->vert_list_trans[0].point,camera.pos,view);


		float dp=n.dot(view);
		//
		if (dp<=0.0f){
			setBit(curr_triangle->state,TRIANGLE_STATE_BACKFACE);
			//curr_triangle->state|=TRIANGLE_STATE_BACKFACE;
		}
	}
}

int Compare_AvgZ_Triangle(const void* arg1, const void* arg2){
	float z1,z2;
	Triangle* triangle1=*((Triangle**)(arg1));
	Triangle* triangle2=*((Triangle**)(arg2));

	z1=(float)0.3333*(triangle1->vert_list_trans[0].z+triangle1->vert_list_trans[1].z+triangle1->vert_list_trans[2].z);
	z2=(float)0.3333*(triangle2->vert_list_trans[0].z+triangle2->vert_list_trans[1].z+triangle2->vert_list_trans[2].z);
	
	if (z1>z2){
		return -1;
	}else if (z1<z2){
		return 1;
	}else {
		return 0;
	}
}

int Compare_NearZ_Triangle(const void *arg1, const void *arg2){
	float z1,z2;
	Triangle* triangle1=*((Triangle**)(arg1));
	Triangle* triangle2=*((Triangle**)(arg2));

	z1=min(triangle1->vert_list_trans[0].z,triangle1->vert_list_trans[1].z);
	z1=min(triangle1->vert_list_trans[2].z,z1);

	z2=min(triangle2->vert_list_trans[0].z,triangle2->vert_list_trans[1].z);
	z2=min(triangle2->vert_list_trans[2].z,z2);

	if (z1>z2){
		return -1;
	}else if (z1<z2){
		return 1;
	}else {
		return 0;
	}
}

int Compare_FarZ_Triangle(const void *arg1, const void *arg2){
	float z1,z2;
	Triangle* triangle1=*((Triangle**)(arg1));
	Triangle* triangle2=*((Triangle**)(arg2));

	z1=max(triangle1->vert_list_trans[0].z,triangle1->vert_list_trans[1].z);
	z1=max(triangle1->vert_list_trans[2].z,z1);

	z2=max(triangle2->vert_list_trans[0].z,triangle2->vert_list_trans[1].z);
	z2=max(triangle2->vert_list_trans[2].z,z2);

	if (z1>z2){
		return -1;
	}else if (z1<z2){
		return 1;
	}else {
		return 0;
	}
}

void RenderList4D::sortZ(SortPolyMethod method){
	switch(method){
	case SORT_POLY_METHOD_AVGZ:
		 qsort(this->triangle_ptrs,this->num_triangles,sizeof(Triangle*),Compare_AvgZ_Triangle);
		break;
	case SORT_POLY_METHOD_NEARZ:
		 qsort(this->triangle_ptrs,this->num_triangles,sizeof(Triangle*),Compare_NearZ_Triangle);
		break;
	case SORT_POLY_METHOD_FARZ:
		qsort(this->triangle_ptrs,this->num_triangles,sizeof(Triangle*),Compare_FarZ_Triangle);
		break;
	}
}


void RenderList4D::LightInWorld(Camera& camera,LightList& light_list){
	float dp;
	float dist;
	float dists;
	float i;
	float nl;
	float atten;

	Vector4D u,v,n,l,s;
	for (int index=0;index<this->num_triangles;index++){

		Triangle* curr_triangle=this->triangle_ptrs[index];

		if (TRIANGLE_UNACTIVE_STATE(curr_triangle)||(curr_triangle->state&TRIANGLE_STATE_LIT)){
			continue;
		}

		setBit(curr_triangle->state,TRIANGLE_STATE_LIT);


		
		if (curr_triangle->attr&TRIANGLE_ATTR_SHADE_MODE_FLAT){
			//恒定着色，有光照


			

			unsigned int r_base = curr_triangle->color.r;
			unsigned int g_base = curr_triangle->color.g;
			unsigned int b_base = curr_triangle->color.b;

			unsigned int r_sum=0;
			unsigned int g_sum=0;
			unsigned int b_sum=0;
			n.z=FLT_MAX;//保证法向量只计算一遍

			for (int curr_light=0;curr_light<light_list.lights_num;curr_light++){
				if (!light_list.lights[curr_light].state){
					continue;
				}
				Light* light=&light_list.lights[curr_light];

				switch(light->attr&LIGHT_ATTR_TYPEMASK){
				case LIGHT_ATTR_AMBIENT:
					//环境光
					r_sum += (light->c_ambient.r * r_base) / 255;
					g_sum += (light->c_ambient.g * g_base) / 255;
					b_sum += (light->c_ambient.b * b_base) / 255;

					break;
				case LIGHT_ATTR_INFINITE:
					//平行光
					if (n.z==FLT_MAX){
						Vector4D::build(curr_triangle->vert_list_trans[0].point, curr_triangle->vert_list_trans[1].point, u); // 注意vert_list_trans中顶点(由cob加载而来)应该是按顺时针顺序存储的
						Vector4D::build(curr_triangle->vert_list_trans[0].point, curr_triangle->vert_list_trans[2].point, v); // 
						u.crossTo(v, n);
					}
					nl=curr_triangle->nlength;

					dp=n.dot(light->dir_world);
					if (dp>0){
						i = 128 * dp / nl; // 光照强度 cos(面法向量与光线向量夹角)
						r_sum += (light->c_diffuse.r * r_base * i) / (255 * 128);
						g_sum += (light->c_diffuse.g * g_base * i) / (255 * 128);
						b_sum += (light->c_diffuse.b * b_base * i) / (255 * 128);
					}

					break;
				case  LIGHT_ATTR_POINT:
					if (n.z==FLT_MAX){
						Vector4D::build(curr_triangle->vert_list_trans[0].point, curr_triangle->vert_list_trans[1].point, u); // 注意vert_list_trans中顶点(由cob加载而来)应该是按顺时针顺序存储的
						Vector4D::build(curr_triangle->vert_list_trans[0].point, curr_triangle->vert_list_trans[2].point, v); // 
						u.crossTo(v, n);
					}
					nl = curr_triangle->nlength;

					Vector4D::build(curr_triangle->vert_list_trans[0].point,light->pos_world,l);
					dist = l.lengthFast2();  
					
					dp=n.dot(l);
					if (dp>0){
						atten = light->kc + light->kl * dist + light->kq * dist * dist; // 分母

						i = 128 * dp / (nl * dist * atten); 

						r_sum += (light->c_diffuse.r * r_base * i) / (255*128);
						g_sum += (light->c_diffuse.g * g_base * i) / (255*128);
						b_sum += (light->c_diffuse.b * b_base * i) / (255*128);

					}
					break;
				case LIGHT_ATTR_SPOTLIGHT1:
					if (n.z==FLT_MAX){
						Vector4D::build(curr_triangle->vert_list_trans[0].point, curr_triangle->vert_list_trans[1].point, u); // 注意vert_list_trans中顶点(由cob加载而来)应该是按顺时针顺序存储的
						Vector4D::build(curr_triangle->vert_list_trans[0].point, curr_triangle->vert_list_trans[2].point, v); // 
						u.crossTo(v, n);
					}
					nl = curr_triangle->nlength;
					Vector4D::build(curr_triangle->vert_list_trans[0].point,light->pos_world,l);
					dist=l.lengthFast2();

					dp=n.dot(light->dir_world);
					if (dp > 0)
					{ 
						atten = light->kc + light->kl*dist + light->kq * dist * dist;

						i = 128 * dp / (nl * atten);

						r_sum += (light->c_diffuse.r * r_base * i) / (255*128);
						g_sum += (light->c_diffuse.g * g_base * i) / (255*128);
						b_sum += (light->c_diffuse.b * b_base * i) / (255*128);
					}

					break;
				case LIGHT_ATTR_SPOTLIGHT2:
					if (n.z==FLT_MAX){
						Vector4D::build(curr_triangle->vert_list_trans[0].point, curr_triangle->vert_list_trans[1].point, u); // 注意vert_list_trans中顶点(由cob加载而来)应该是按顺时针顺序存储的
						Vector4D::build(curr_triangle->vert_list_trans[0].point, curr_triangle->vert_list_trans[2].point, v); // 
						u.crossTo(v, n);
					}
					nl = curr_triangle->nlength;
					dp=n.dot(light->dir_world);
					if (dp > 0){
						Vector4D::build(curr_triangle->vert_list_trans[0].point, light->pos_world, s);
						dists = s.lengthFast2(); 
						float dpsl = s.dot(light->dir_world) / dists;
						if (dpsl > 0){
							atten = light->kc + light->kl * dists + light->kq * dists * dists; // 分母
							float dpsl_exp = dpsl; 

							for (int e_index = 1; e_index < light_list.lights[curr_light].pf; e_index++)
								dpsl_exp *= dpsl;  // n次方

							i = 128 * dp * dpsl_exp / (nl * atten); 

							r_sum += (light->c_diffuse.r * r_base * i) / (255*128);
							g_sum += (light->c_diffuse.g * g_base * i) / (255*128);
							b_sum += (light->c_diffuse.b * b_base * i) / (255*128);

						}
					}
					break;

				}
			}
			if (r_sum  > 255) r_sum = 255;
			if (g_sum  > 255) g_sum = 255;
			if (b_sum  > 255) b_sum = 255;
			curr_triangle->lit_color[0].r = r_sum;
			curr_triangle->lit_color[0].g = g_sum;
			curr_triangle->lit_color[0].b = b_sum;
			curr_triangle->lit_color[0].a = 255;

		}else if (curr_triangle->attr&TRIANGLE_ATTR_SHADE_MODE_GOURAUD){
			//平滑着色

			unsigned int r_base = curr_triangle->color.r;
			unsigned int g_base = curr_triangle->color.g;
			unsigned int b_base = curr_triangle->color.b;

			unsigned int r_sum0  = 0;
			unsigned int g_sum0  = 0;
			unsigned int b_sum0  = 0;

			unsigned int r_sum1  = 0;
			unsigned int g_sum1  = 0;
			unsigned int b_sum1  = 0;

			unsigned int r_sum2  = 0;
			unsigned int g_sum2  = 0;
			unsigned int b_sum2  = 0;

			for (int curr_light=0;curr_light<light_list.lights_num;curr_light++){
				Light& light=light_list.lights[curr_light];

				if (!light.state){
					continue;
				}

				switch(light.attr&LIGHT_ATTR_TYPEMASK){

				case LIGHT_ATTR_AMBIENT:
					{

					
					unsigned int ri=light.c_ambient.r*r_base/256;
					unsigned int gi=light.c_ambient.g*g_base/256;
					unsigned int bi=light.c_ambient.b*b_base/256;

					//每个顶点都加
					r_sum0 += ri;
					g_sum0 += gi;
					b_sum0 += bi;

					r_sum1 += ri;
					g_sum1 += gi;
					b_sum1 += bi;

					r_sum2 += ri;
					g_sum2 += gi;
					b_sum2 += bi;
					break;
					}
					
					
					 
				case LIGHT_ATTR_INFINITE:
					{

					
					dp=curr_triangle->vert_list_trans[0].normal.dot(light.dir_world);

					if (dp > 0)
					{
						i = 128 * dp; 
						r_sum0 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum0 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum0 += (light.c_diffuse.b * b_base * i) / (255*128);
					}
					dp=curr_triangle->vert_list_trans[1].normal.dot(light.dir_world);
					if (dp > 0)
					{
						i = 128 * dp; 
						r_sum1 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum1 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum1 += (light.c_diffuse.b * b_base * i) / (255*128);
					}

					dp=curr_triangle->vert_list_trans[2].normal.dot(light.dir_world);
					if (dp > 0)
					{
						i = 128 * dp; 
						r_sum2 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum2 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum2 += (light.c_diffuse.b * b_base * i) / (255*128);
					}
					break;
					}
					
				case LIGHT_ATTR_POINT:
					{

					
					Vector4D::build(curr_triangle->vert_list_trans[0].point,light.pos_world,l);
					dist=l.lengthFast2();

					dp=curr_triangle->vert_list_trans[0].normal.dot(l);
					if (dp > 0)
					{ 
						atten = light.kc + light.kl * dist + light.kq * dist * dist; // 分母, 可不重复计算

						i = 128 * dp / (dist * atten); 

						r_sum0 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum0 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum0 += (light.c_diffuse.b * b_base * i) / (255*128);
					}

					dp=curr_triangle->vert_list_trans[1].normal.dot(l);
					if (dp > 0)
					{ 
						atten = light.kc + light.kl * dist + light.kq * dist * dist; // 分母, 可不重复计算

						i = 128 * dp / (dist * atten); 

						r_sum1 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum1 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum1 += (light.c_diffuse.b * b_base * i) / (255*128);
					}

					dp=curr_triangle->vert_list_trans[2].normal.dot(l);
					if (dp > 0)
					{ 
						atten = light.kc + light.kl * dist + light.kq * dist * dist; // 分母, 可不重复计算

						i = 128 * dp / (dist * atten); 

						r_sum2 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum2 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum2 += (light.c_diffuse.b * b_base * i) / (255*128);
					}

					break;
					}
				case LIGHT_ATTR_SPOTLIGHT1:
					{

					
					Vector4D::build(curr_triangle->vert_list_trans[0].point,light.pos_world,l);
					dist=l.lengthFast2();

					dp=curr_triangle->vert_list_trans[0].normal.dot(light.dir_world);
					if (dp > 0)
					{
						atten = light.kc + light.kl * dist + light.kq * dist * dist; // 分母 用于随距离衰减

						i = 128 * dp / (atten);

						r_sum0 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum0 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum0 += (light.c_diffuse.b * b_base * i) / (255*128);
					}


					dp=curr_triangle->vert_list_trans[1].normal.dot(light.dir_world);
					if (dp > 0)
					{
						atten = light.kc + light.kl * dist + light.kq * dist * dist; // 分母 用于随距离衰减

						i = 128 * dp / (atten);

						r_sum1 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum1 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum1 += (light.c_diffuse.b * b_base * i) / (255*128);
					}

					dp=curr_triangle->vert_list_trans[2].normal.dot(light.dir_world);
					if (dp > 0)
					{
						atten = light.kc + light.kl * dist + light.kq * dist * dist; // 分母 用于随距离衰减

						i = 128 * dp / (atten);

						r_sum2 += (light.c_diffuse.r * r_base * i) / (255*128);
						g_sum2 += (light.c_diffuse.g * g_base * i) / (255*128);
						b_sum2 += (light.c_diffuse.b * b_base * i) / (255*128);
					}
					break;
					}
				case LIGHT_ATTR_SPOTLIGHT2:
					{

					
					dp=curr_triangle->vert_list_trans[0].normal.dot(light.dir_world);
					if (dp>0){
						Vector4D::build(curr_triangle->vert_list_trans[0].point,light.pos_world,s);
						dists=s.lengthFast2();
						float dpsl=s.dot(light.dir_world)/dists;

						if (dpsl>0){
							atten = light.kc + light.kl * dists + light.kq * dists * dists;    
							float dpsl_exp = dpsl;
							for (int e_index = 1; e_index < light_list.lights[curr_light].pf; e_index++)
								dpsl_exp *= dpsl;

							i = 128 * dp * dpsl_exp / (atten);

							r_sum0 += (light.c_diffuse.r * r_base * i) / (255*128);
							g_sum0 += (light.c_diffuse.g * g_base * i) / (255*128);
							b_sum0 += (light.c_diffuse.b * b_base * i) / (255*128);

						}
					}

					dp=curr_triangle->vert_list_trans[1].normal.dot(light.dir_world);
					if (dp>0){
						Vector4D::build(curr_triangle->vert_list_trans[0].point,light.pos_world,s);
						dists=s.lengthFast2();
						float dpsl=s.dot(light.dir_world)/dists;

						if (dpsl>0){
							atten = light.kc + light.kl * dists + light.kq * dists * dists;    
							float dpsl_exp = dpsl;
							for (int e_index = 1; e_index < light_list.lights[curr_light].pf; e_index++)
								dpsl_exp *= dpsl;

							i = 128 * dp * dpsl_exp / (atten);

							r_sum1 += (light.c_diffuse.r * r_base * i) / (255*128);
							g_sum1 += (light.c_diffuse.g * g_base * i) / (255*128);
							b_sum1 += (light.c_diffuse.b * b_base * i) / (255*128);

						}
					}


					dp=curr_triangle->vert_list_trans[2].normal.dot(light.dir_world);
					if (dp>0){
						Vector4D::build(curr_triangle->vert_list_trans[2].point,light.pos_world,s);
						dists=s.lengthFast2();
						float dpsl = s.dot(light.dir_world) / dists;

						if (dpsl>0){
							atten = light.kc + light.kl * dists + light.kq * dists * dists;    
							float dpsl_exp=dpsl;

							for (int e_index = 1; e_index < light_list.lights[curr_light].pf; e_index++)
								dpsl_exp *= dpsl;

							i = 128 * dp * dpsl_exp / (atten);

							r_sum2 += (light.c_diffuse.r * r_base * i) / (255*128);
							g_sum2 += (light.c_diffuse.g * g_base * i) / (255*128);
							b_sum2 += (light.c_diffuse.b * b_base * i) / (255*128);

						}
					}
					break;
					}
				}
			}
			if (r_sum0 > 255) r_sum0 = 255;
			if (g_sum0 > 255) g_sum0 =255;
			if (b_sum0 > 255) b_sum0 = 255;

			if (r_sum1 > 255) r_sum1 = 255;
			if (g_sum1 > 255) g_sum1 = 255;
			if (b_sum1 > 255) b_sum1 = 255;

			if (r_sum2 > 255) r_sum2 = 255;
			if (g_sum2 > 255) g_sum2 = 255;
			if (b_sum2 > 255) b_sum2 = 255;

			curr_triangle->lit_color[0].r = r_sum0;
			curr_triangle->lit_color[0].g = g_sum0;
			curr_triangle->lit_color[0].b = b_sum0;
			curr_triangle->lit_color[0].a = 255; // 可忽略 因为实际绘制时并未从这里读取 (由curr_poly->color.a读取)

			curr_triangle->lit_color[1].r = r_sum1;
			curr_triangle->lit_color[1].g = g_sum1;
			curr_triangle->lit_color[1].b = b_sum1;
			curr_triangle->lit_color[1].a = 255;

			curr_triangle->lit_color[2].r = r_sum2;
			curr_triangle->lit_color[2].g = g_sum2;
			curr_triangle->lit_color[2].b = b_sum2;
			curr_triangle->lit_color[2].a = 255;

		}else{
			curr_triangle->lit_color[0]=curr_triangle->color;
		}

	}

}


void RenderList4D::transformByMat(const Matrix_4X4& mat,TransformMode mode){
	Point4D presult;

	switch(mode){
	case TRANSFORMMODE_LOCAL_ONLY:
		for (int idx=0;idx<this->num_triangles;idx++){
			Triangle* curr_poly=this->triangle_ptrs[idx];

			if (TRIANGLE_UNACTIVE_STATE(curr_poly)){
					continue;
			}
			
			for (int vertexidx=0;vertexidx<3;vertexidx++){
				curr_poly->vert_list_local[vertexidx].point.mulMat4X4To(mat,presult);
				curr_poly->vert_list_local[vertexidx].point=presult;
			}
		}
		break;
	case TRANSFORMMODE_TRANS_ONLY:

		for (int idx=0;idx<this->num_triangles;idx++){
			Triangle* curr_poly=this->triangle_ptrs[idx];

			if (TRIANGLE_UNACTIVE_STATE(curr_poly)){
				continue;
			}

			for (int vertexidx=0;vertexidx<3;vertexidx++){
				curr_poly->vert_list_trans[vertexidx].point.mulMat4X4To(mat,presult);
				curr_poly->vert_list_trans[vertexidx].point=presult;
			}
		}
		break;
	case TRANSFORMMODE_LOCAL_TO_TRANS:

		for (int idx=0;idx<this->num_triangles;idx++){
			Triangle* curr_poly=this->triangle_ptrs[idx];

			if (TRIANGLE_UNACTIVE_STATE(curr_poly)){
				continue;
			}

			for (int vertexidx=0;vertexidx<3;vertexidx++){
				curr_poly->vert_list_local[vertexidx].point.mulMat4X4To(mat,curr_poly->vert_list_trans[vertexidx].point);
				////=presult;
			}
		}
		break;
	}
}


void RenderList4D::transformModelToWorld(const Point4D &world_pos, TransformMode mode /* = TRANSFORMMODE_LOCAL_TO_TRANS */){
	if (mode==TRANSFORMMODE_LOCAL_TO_TRANS){
		
		for (int polyidx=0;polyidx<this->num_triangles;polyidx++){
			
			Triangle* curr_poly=this->triangle_ptrs[polyidx];
			if (TRIANGLE_UNACTIVE_STATE(curr_poly)){
				continue;
			}

			for (int vertexidx=0;vertexidx<3;vertexidx++){
				curr_poly->vert_list_local[vertexidx].point.addTo(world_pos,curr_poly->vert_list_trans[vertexidx].point);

			}
		}
	}else{

		for (int polyidx=0;polyidx<this->num_triangles;polyidx++){

			Triangle* curr_poly=this->triangle_ptrs[polyidx];
			if (TRIANGLE_UNACTIVE_STATE(curr_poly)){
				continue;
			}

			for (int vertexidx=0;vertexidx<3;vertexidx++){
				curr_poly->vert_list_trans[vertexidx].point.addTo(world_pos,curr_poly->vert_list_trans[vertexidx].point);

			}
		}

	}
}


void RenderList4D::transformWorldToCamera(const Camera& cam){

	for (int poly = 0; poly < this->num_triangles; poly++)
	{
		Triangle *curr_poly = this->triangle_ptrs[poly];


		if (TRIANGLE_UNACTIVE_STATE(curr_poly))
		{
			continue; // move onto next poly
		}

		for (int vertex = 0; vertex < 3; vertex++)
		{

			Point4D presult; // hold result of each transformation

			curr_poly->vert_list_trans[vertex].point.mulMat4X4To(cam.mat_cam, presult);

			// store result back
			curr_poly->vert_list_trans[vertex].point = presult;
		}
	}
}

void RenderList4D::transformCameraToPerspective(const Camera& cam){
	
	for(int poly=0;poly<this->num_triangles;poly++){
		Triangle* curr_poly=this->triangle_ptrs[poly];
		if (TRIANGLE_UNACTIVE_STATE(curr_poly)){
			continue;
		}

		for (int vertex=0;vertex<3;vertex++){
			float z=curr_poly->vert_list_trans[vertex].z;

			curr_poly->vert_list_trans[vertex].x=cam.view_dist*curr_poly->vert_list_trans[vertex].x/z;
			curr_poly->vert_list_trans[vertex].y=cam.view_dist*curr_poly->vert_list_trans[vertex].y/z;
		}

	}
}


void RenderList4D::transformPerspectiveToScreen(const Camera &cam){

	float alpha = 0.5f * cam.viewport_width - 0.5f;
	float beta  = 0.5f * cam.viewport_height - 0.5f;

	for (int poly=0;poly<this->num_triangles;poly++){
		Triangle* curr_poly=this->triangle_ptrs[poly];
		if (TRIANGLE_UNACTIVE_STATE(curr_poly)){
			continue;
		}
		for (int vertex=0;vertex<3;vertex++){
			curr_poly->vert_list_trans[vertex].x=alpha+alpha*curr_poly->vert_list_trans[vertex].x;
			curr_poly->vert_list_trans[vertex].y=beta-beta*curr_poly->vert_list_trans[vertex].y;


		}

	}

}



//////////////////////////////////////////////Object4D/////////////////////////////////////////////////////

Object4D::Object4D(){
	this->head_vert_list_local=nullptr;
	this->head_vert_list_trans=nullptr;
	this->triangle_ref_list=nullptr;
	this->vert_list_local=nullptr;
	this->vert_list_trans=nullptr;
	this->triangle_ref_list=nullptr;
}
Object4D::~Object4D(){
	this->destroy();
}

void Object4D::destroy(){

	if (this->head_vert_list_local){
		free(this->head_vert_list_local);
		this->head_vert_list_local=nullptr;
	}
	if (this->head_vert_list_trans){
		free(this->head_vert_list_trans);
		this->head_vert_list_trans=nullptr;
	}

	if (this->triangle_ref_list){
		free(this->triangle_ref_list);
	}



	//memset(this,0,sizeof(Object4D));
}

void Object4D::initBasis(){
	this->ux.init(1,0,0,1);
	this->uy.init(0,1,0,1);
	this->uz.init(0,0,1,1);

}

std::string* Object4D::plxLine(){
	if (plxLineIdx>=plxLines.size()){
		return nullptr;
	}else{
		return &plxLines[plxLineIdx++];
	}
}


bool Object4D::init(int _num_vertices, int _num_triangles, int _num_frames, bool destroy){
	if (destroy){
		//this->destroy();
	}

	
	int size=sizeof(TriangleRef);
	this->triangle_ref_list=new TriangleRef[_num_triangles]; //(Triangle*)(malloc(size*_num_triangles));
	if (!this->triangle_ref_list){
		return false;
	}
	memset(this->triangle_ref_list,0,size*_num_triangles);

	
	this->head_vert_list_local=static_cast<Vertex4D*>(malloc(sizeof(Vertex4D)*_num_vertices*_num_frames));
	if (!this->head_vert_list_local){
		return false;
	}
	memset(this->head_vert_list_local,0,sizeof(Vertex4D)*_num_vertices*_num_frames);

	this->head_vert_list_trans=static_cast<Vertex4D*>(malloc(sizeof(Vertex4D)*_num_vertices*_num_frames));
	if (!this->head_vert_list_trans){
		return false;
	}
	memset(this->head_vert_list_trans,0,sizeof(Vertex4D)*_num_vertices*_num_frames);

	this->vert_list_local=&this->head_vert_list_local[this->curr_frame];
	this->vert_list_trans=&this->head_vert_list_trans[this->curr_frame];

	this->avg_radius=new float[num_frames];
	this->max_radius=new float[num_frames];
	this->texture=nullptr;

	return true;
}

bool Object4D::loadFromPLX1(const std::string &filename, const Vector4D &scale, const Point4D &pos,const Vector4D &rot){
 

	
	this->destroy();
	this->initBasis();

	this->state=OBJECT4D_STATE_ACTIVE|OBJECT4D_STATE_VISIBLE;

	this->world_pos.x=pos.x;
	this->world_pos.y=pos.y;
	this->world_pos.z=pos.z;
	this->world_pos.w=pos.w;

	this->attr=OBJECT4D_ATTR_SINGLE_FRAME;
	this->num_frames=1;
	this->curr_frame=0;
 
	std::string filetext=FileUtils::getInstance()->getStringFromFile(const_cast<char*>(filename.c_str()));

	std::vector<std::string> filelines;
	split(filetext,"\n",filelines);

	plxLines.clear();
	
	for (std::vector<std::string>::size_type i=0;i<filelines.size();i++){
		std::string line=trim(filelines[i]);
		if (!line.empty()&&line[0]!='#'){
			plxLines.push_back(line);
		}
	}
	
	plxLineIdx=0;

	std::string* token_string;

	if (!(token_string=plxLine())){
		CCLOG("PLG file error with file(ther is no data in the file) %s (object descriptor invalid).", filename.c_str());
		return false;
	}

	CCLOG("Object Descriptor: %s", token_string->c_str());
	
	sscanf(token_string->c_str(),"%s %d %d",this->name,&this->total_vertices,&this->num_triangles);
	
	this->num_vertices=this->total_vertices;
	
	if (this->init(this->num_vertices,this->num_triangles,this->num_frames,false)==false){
		CCLOG("PLG file error with file %s (can't allocate memory).", filename.c_str());
	}
	
	
	for (int vertex=0;vertex<this->num_vertices;vertex++){

		if (!(token_string=plxLine())){
			CCLOG("PLG file error with file %s (vertex list invalid).", filename.c_str());
			return false;
		}

		sscanf(token_string->c_str(),"%f %f %f",&this->vert_list_local[vertex].x,
												&this->vert_list_local[vertex].y,
												&this->vert_list_local[vertex].z);
		this->vert_list_local[vertex].w=1.0f;

		this->vert_list_local[vertex].x*=scale.x;
		this->vert_list_local[vertex].y*=scale.y;
		this->vert_list_local[vertex].z*=scale.z;

		//this->vert_list_local[vertex].attr|=VERTEX4DV1_ATTR_POINT;
	}

	//顶点加载结束，可以计算最长半径了。
	this->avg_radius=new float[this->num_frames];
	this->max_radius=new float[this->num_frames];
	this->computeRadius();
	
		
	int poly_surface_desc=0;
	int poly_num_verts=0;
	char tmp_string[8];
	for (int index=0;index<this->num_triangles;index++){
		if (!(token_string = plxLine()))
		{
			CCLOG("PLG file error with file %s (polygon descriptor invalid).", filename.c_str());
			return false;
		}

		CCLOG("Polygon %d:", index);
		//this->poly_list
		//读取多边形对应的索引，对应到this->vert_list_local中
		sscanf(token_string->c_str(),"%s %d %d %d %d",tmp_string,&poly_num_verts,
																&this->triangle_ref_list[index].vert_idx[0],
																&this->triangle_ref_list[index].vert_idx[1],
																&this->triangle_ref_list[index].vert_idx[2]);
	
		if (tmp_string[0] == '0' && (toupper(tmp_string[1]) == 'x'||toupper(tmp_string[1]) == 'X'))
			sscanf(tmp_string, "%x", &poly_surface_desc);
		else
			poly_surface_desc = atoi(tmp_string);

		
		this->triangle_ref_list[index].attr=TRIANGLE_ATTR_NULL;
		this->triangle_ref_list[index].state=TRIANGLE_STATE_ACTIVE;


		this->triangle_ref_list[index].init(this->vert_list_local,vert_list_trans);

		if (poly_surface_desc&PLX_2SIDED_FLAG){
			//双面
			this->triangle_ref_list[index].attr|=TRIANGLE_ATTR_2SIDE;
		}else {
			CCLOG("1 sided.");
		}

		if (poly_surface_desc&PLX_COLOR_MODE_RGB_FLAG){
			this->triangle_ref_list[index].attr |=TRIANGLE_ATTR_RGB32;

			int red=(poly_surface_desc & 0x0f00) >> 8;
			int green=(poly_surface_desc & 0x00f0) >> 4;
			int blue  = (poly_surface_desc & 0x000f);

			this->triangle_ref_list[index].color.r=red*17;
			this->triangle_ref_list[index].color.g=green*17;
			this->triangle_ref_list[index].color.b=blue*17;
			this->triangle_ref_list[index].color.a=255;

		}


		int shade_mode=poly_surface_desc&PLX_SHADE_MODE_MASK;

		switch(shade_mode){
		case PLX_SHADE_MODE_PURE_FLAG:
			this->triangle_ref_list[index].attr |=TRIANGLE_ATTR_SHADE_MODE_FLAT;
			break;
		case PLX_SHADE_MODE_FLAT_FLAG:
			this->triangle_ref_list[index].attr |=TRIANGLE_ATTR_SHADE_MODE_FLAT;
			break;
		case PLX_SHADE_MODE_GOURAUD_FLAG:
			this->triangle_ref_list[index].attr |=TRIANGLE_ATTR_SHADE_MODE_GOURAUD;
			break;
		case PLX_SHADE_MODE_PHONG_FLAG:
			this->triangle_ref_list[index].attr |=TRIANGLE_ATTR_SHADE_MODE_PHONG;
			break;
		default:
			break;
		}
	}

	return true;
}

bool Object4D::loadFormCOB(const std::string &filename, const Vector4D &scale, const Point4D &pos, const Vector4D &rot, int vertex_flags){
	Parser_V1 parser;

	int num_texture_vertices=0;
	Matrix_4X4 mat_local;
	Matrix_4X4 mat_world;

	mat_local.identify();
	mat_world.identify();
	this->destroy();

	this->state=OBJECT4D_STATE_ACTIVE|OBJECT4D_STATE_VISIBLE;

	this->num_frames=1;
	this->curr_frame=0;
	this->attr=OBJECT4D_ATTR_SINGLE_FRAME;

	this->world_pos.x = pos.x;
	this->world_pos.y = pos.y;
	this->world_pos.z = pos.z;
	this->world_pos.w = pos.w;

	if(!parser.loadFile(const_cast<char*>(filename.c_str()))){
		CCLOG("Couldn't open .COB file %s.", filename.c_str());
		return false;
	}

	while(1){
		if (!parser.getLine(PARSER_STRIP_EMPTY_LINES| PARSER_STRIP_WS_ENDS)){
			CCLOG("Image 'name' not found in .COB file %s.", filename.c_str());
			return false;
		}
		//读取Object 名字
		if (parser.match(parser.cur_line.c_str(),"['Name'] [s>0]")){
			strcpy(this->name,parser.pstrings[1]);
			CCLOG("COB Reader Object Name: %s", this->name);
			break;
		}

	}

	// 坐标系中心位置和各坐标轴(向量)
	// center 0 0 0
	// x axis 1 0 0
	// y axis 0 1 0
	// z axis 0 0 1

	while(1){
		if (!parser.getLine(PARSER_STRIP_EMPTY_LINES| PARSER_STRIP_WS_ENDS)){
			CCLOG("Center not found in .COB file %s.", filename.c_str());
			return false;
		}
		if (parser.match(parser.cur_line.c_str(), "['center'] [f] [f] [f]")){
			// 位移逆矩阵
			mat_local.M[3][0]=-parser.pfloats[0]; //center x
			mat_local.M[3][1]=-parser.pfloats[1]; //center y
			mat_local.M[3][2]=-parser.pfloats[2]; //center z
		
			// 旋转逆矩阵
			// 因为相当于定义了局部坐标系各坐标轴轴向量 与UVN相机模型类似

			// "x axis" 
			parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
			parser.match(parser.cur_line.c_str(), "['x'] ['axis'] [f] [f] [f]");

			// place row in x column of transform matrix
			mat_local.M[0][0] = parser.pfloats[0]; // rxx
			mat_local.M[1][0] = parser.pfloats[1]; // rxy
			mat_local.M[2][0] = parser.pfloats[2]; // rxz

			// "y axis" 
			parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
			parser.match(parser.cur_line.c_str(), "['y'] ['axis'] [f] [f] [f]");

			// place row in y column of transform matrix
			mat_local.M[0][1] = parser.pfloats[0]; // ryx
			mat_local.M[1][1] = parser.pfloats[1]; // ryy
			mat_local.M[2][1] = parser.pfloats[2]; // ryz

			// "z axis" 
			parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
			parser.match(parser.cur_line.c_str(), "['z'] ['axis'] [f] [f] [f]");

			// place row in z column of transform matrix
			mat_local.M[0][2] = parser.pfloats[0]; // rzx
			mat_local.M[1][2] = parser.pfloats[1]; // rzy
			mat_local.M[2][2] = parser.pfloats[2]; // rzz

			break;
		}
	}

	// now "Transform" 另一个变换矩阵?? (据说是从局部坐标变为世界坐标的)

	while (1)
	{
		// get the next line, we are looking for "Transform"
		if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
		{
			CCLOG("Transform not found in .COB file %s.", filename.c_str());
			return false;
		}

		// check for pattern?  
		if (parser.match(parser.cur_line.c_str(), "['Transform']"))
		{
			// TODO: 为何没有读最后一列			

			// "x axis" 
			parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
			parser.match(parser.cur_line.c_str(), "[f] [f] [f]");

			// place row in x column of transform matrix
			mat_world.M[0][0] = parser.pfloats[0]; // rxx
			mat_world.M[1][0] = parser.pfloats[1]; // rxy
			mat_world.M[2][0] = parser.pfloats[2]; // rxz

			// "y axis" 
			parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
			parser.match(parser.cur_line.c_str(), "[f] [f] [f]");

			// place row in y column of transform matrix
			mat_world.M[0][1] = parser.pfloats[0]; // ryx
			mat_world.M[1][1] = parser.pfloats[1]; // ryy
			mat_world.M[2][1] = parser.pfloats[2]; // ryz

			// "z axis" 
			parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
			parser.match(parser.cur_line.c_str(), "[f] [f] [f]");

			// place row in z column of transform matrix
			mat_world.M[0][2] = parser.pfloats[0]; // rzx
			mat_world.M[1][2] = parser.pfloats[1]; // rzy
			mat_world.M[2][2] = parser.pfloats[2]; // rzz

			// Print_Mat_4X4(&mat_world, "World COB Matrix:");

			// no need to read in last row, since it's always 0,0,0,1 and we don't use it anyway
			break;
		}
	}


	// step 6: get number of vertices and polys in object 顶点数量
	while (1)
	{
		// get the next line, we are looking for "World Vertices" 
		if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
		{
			CCLOG("'World Vertices' line not found in .COB file %s.", filename.c_str());
			return false;
		}

		// check for pattern?  
		if (parser.match(parser.cur_line.c_str(), "['World'] ['Vertices'] [i]"))
		{
			// simply extract the number of vertices from the pattern matching 
			// output arrays
			this->num_vertices = parser.pints[0];
			 
			CCLOG("COB Reader Num Vertices: %d", this->num_vertices);
			break;
		}
	}

	if (!this->init(this->num_vertices, this->num_vertices * 3, this->num_frames, false)) // TODO: num_vertices * 3 是否足够 ??
	{
		CCLOG("ASC file error with file %s (can't allocate memory).", filename.c_str());
	}

	//读取顶点列表
	for (int vertex=0;vertex<this->num_vertices;vertex++){
		while(1){
			if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				CCLOG("Vertex list ended abruptly! in .COB file %s.", filename.c_str());
				return false;
			}

			if (parser.match(parser.cur_line.c_str(), "[f] [f] [f]")){
				this->vert_list_local[vertex].x=parser.pfloats[0];
				this->vert_list_local[vertex].y=parser.pfloats[1];
				this->vert_list_local[vertex].z=parser.pfloats[2];
				this->vert_list_local[vertex].w=1;

				Point4D temp_point;
				if (vertex_flags&VERTEX_FLAGS_TRANSFORM_LOCAL){
					this->vert_list_local[vertex].point.mulMat4X4To(mat_local,temp_point);
					this->vert_list_local[vertex].point=temp_point;
				}

				if (vertex_flags & VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD) // 执行世界变换
				{
					this->vert_list_local[vertex].point.mulMat4X4To(mat_world, temp_point);
					this->vert_list_local[vertex].point = temp_point;
				}

				if (vertex_flags&VERTEX_FLAGS_INVERT_X){
					this->head_vert_list_local[vertex].x=-this->head_vert_list_local[vertex].x;
				}

				if (vertex_flags&VERTEX_FLAGS_INVERT_Y){
					this->head_vert_list_local[vertex].y=-this->head_vert_list_local[vertex].y;
				}

				if (vertex_flags&VERTEX_FLAGS_INVERT_Z){
					this->head_vert_list_local[vertex].z=-this->head_vert_list_local[vertex].z;
				}

				if (vertex_flags & VERTEX_FLAGS_SWAP_YZ)
					std::swap(this->vert_list_local[vertex].y, this->vert_list_local[vertex].z);

				if (vertex_flags & VERTEX_FLAGS_SWAP_XZ)
					std::swap(this->vert_list_local[vertex].x, this->vert_list_local[vertex].z);

				if (vertex_flags & VERTEX_FLAGS_SWAP_XY)
					std::swap(this->vert_list_local[vertex].x, this->vert_list_local[vertex].y);

				this->vert_list_local[vertex].x *= scale.x;
				this->vert_list_local[vertex].y *= scale.y;
				this->vert_list_local[vertex].z *= scale.z;

				setBit(this->vert_list_local[vertex].attr,VERTEX4D_ATTR_POINT);
				//setBit(this->vert_list_local[vertex].attr,VERTE)
				break;
			}
		}
	}

	this->computeRadius();

	CCLOG("Object average radius = %f, max radius = %f", this->avg_radius[0], this->max_radius[0]);

	//纹理坐标 个数
	while(1){
		if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
		{
			CCLOG("'Texture Vertices' line not found in .COB file %s.", filename.c_str());
			return false;
		}

		if (parser.match(parser.cur_line.c_str(), "['Texture'] ['Vertices'] [i]"))
		{
			// simply extract the number of texture vertices from the pattern matching 
			// output arrays
			num_texture_vertices = parser.pints[0];
			
			CCLOG("COB Reader Texture Vertices: %d", num_texture_vertices);
			break;
		}
	}

	this->coord_list=static_cast<Point2D*>(malloc(sizeof(Point2D)*num_texture_vertices));
	//读取纹理数据
	for (int tvertex = 0; tvertex < num_texture_vertices; tvertex++){
		while(1){
			if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				CCLOG("Texture Vertex list ended abruptly! in .COB file %s.", filename.c_str());
				return false;
			}

			if (parser.match(parser.cur_line.c_str(), "[f] [f]")){
				
				this->coord_list[tvertex].x = parser.pfloats[0]; 
				this->coord_list[tvertex].y = parser.pfloats[1];
				break;
			}		
		}
	}

	int poly_material[OBJECT4D_MAX_POLYS];
	int material_index_referenced[MAX_MATERIALS];

	memset(material_index_referenced, 0, sizeof(material_index_referenced));

	//加载三角形信息
	/*
	Faces 704
	Face verts 3 flags 0 mat 0
	<0,0> <3,31> <2,23> 
	Face verts 3 flags 0 mat 0
	<0,1> <4,37> <3,31> 
	*/

	while(1){
		if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
		{
			CCLOG("'Faces' line not found in .COB file %s.", filename.c_str());
			return false;
		}

		if (parser.match(parser.cur_line.c_str(), "['Faces'] [i]"))
		{
			CCLOG("COB Reader found face list in .COB file %s.", filename.c_str());

			// 读取三角形数量
			this->num_triangles = parser.pints[0];

			break;
		}
	}

	int poly_surface_desc=0;
	int poly_num_verts=0;
	int num_material_object=0;

	
	/////到此正常
	for (int poly=0;poly<this->num_triangles;poly++){

		while(1){

			// get the next polygon face
			if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				CCLOG("face list ended abruptly! in .COB file %s.", filename.c_str());
				return false;
			}

			if (parser.match(parser.cur_line.c_str(), "['Face'] ['verts'] [i] ['flags'] [i] ['mat'] [i]"))
			{//材质编号
				poly_material[poly] = parser.pints[2];
			}

			if (material_index_referenced[poly_material[poly]] == 0)
			{
				material_index_referenced[poly_material[poly]]=1;
				num_material_object++;
			}
			//必须是三角形
			if (parser.pints[0] != 3)
			{
				CCLOG("face not a triangle! in .COB file %s.", filename.c_str());
				return false;
			}

			//读取三角形顶点 纹理 索引
			if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				CCLOG("face list ended abruptly! in .COB file %s.", filename.c_str());
				return false;
			}

			parser.cur_line = replace(parser.cur_line, ",<>", ' ');
			parser.match(parser.cur_line.c_str(), "[i] [i] [i] [i] [i] [i]");


			if (vertex_flags & VERTEX_FLAGS_INVERT_WINDING_ORDER){

				this->triangle_ref_list[poly].vert_idx[0]=parser.pints[4];
				this->triangle_ref_list[poly].vert_idx[1]=parser.pints[2];
				this->triangle_ref_list[poly].vert_idx[2]=parser.pints[0];

				this->triangle_ref_list[poly].coord_idx[0]=parser.pints[5];
				this->triangle_ref_list[poly].coord_idx[1]=parser.pints[3];
				this->triangle_ref_list[poly].coord_idx[2]=parser.pints[1];
			}else{
				this->triangle_ref_list[poly].vert_idx[0]=parser.pints[0];
				this->triangle_ref_list[poly].vert_idx[1]=parser.pints[2];
				this->triangle_ref_list[poly].vert_idx[2]=parser.pints[4];

				this->triangle_ref_list[poly].coord_idx[0]=parser.pints[1];
				this->triangle_ref_list[poly].coord_idx[1]=parser.pints[3];
				this->triangle_ref_list[poly].coord_idx[2]=parser.pints[5];
			}
			this->triangle_ref_list[poly].vert_list_local=this->vert_list_local;
			this->triangle_ref_list[poly].coord_list=this->coord_list;
			this->triangle_ref_list[poly].state=TRIANGLE_STATE_ACTIVE;
			this->triangle_ref_list[poly].vert_list_trans=this->vert_list_trans;
			//this->triangle_ref_list[poly].attr=TRIANGLE_ATTR_SHADE_MODE_GOURAUD;
			break;
		}
	}


	//到此正常
	for (int curr_material=0;curr_material<num_material_object;curr_material++){
		while(1){
			if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS)){
				CCLOG("material list ended abruptly! in .COB file %s.", filename.c_str());
				return false;
			}
			if (parser.match(parser.cur_line.c_str(), "['mat#'] [i]") ){
				int material_index = parser.pints[0]; // 材质编号

				while(1){
					if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
					{
						CCLOG("RGB color ended abruptly! in .COB file %s.", filename.c_str());
						return false;
					}

					parser.cur_line = replace(parser.cur_line, ",", ' ');
					
					if (parser.match(parser.cur_line.c_str(), "['rgb'] [f] [f] [f]") )
					{
						// extract data and store color in material libary 添加材质至全局材质表
						// pfloats[] 0,1,2,3, has data
						MaterialList::materials[material_index + MaterialList::num_materials].color.r = (parser.pfloats[0] * 255 + 0.5f);
						MaterialList::materials[material_index + MaterialList::num_materials].color.g = (parser.pfloats[1] * 255 + 0.5f);
						MaterialList::materials[material_index + MaterialList::num_materials].color.b = (parser.pfloats[2] * 255 + 0.5f);

						break; // while looking for rgb
					}
				}

				while (1){
					if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS)){
						CCLOG("material properties ended abruptly! in .COB file %s.", filename.c_str());
						return false;
					}
				
					if (parser.match(parser.cur_line.c_str(), "['alpha'] [f] ['ka'] [f] ['ks'] [f] ['exp'] [f]") ){

						MaterialList::materials[material_index + MaterialList::num_materials].color.a  = parser.pfloats[0] * 255 + 0.5f; // TODO: 后面会被修正的
						MaterialList::materials[material_index + MaterialList::num_materials].ka       = parser.pfloats[1];
						MaterialList::materials[material_index + MaterialList::num_materials].kd       = 1; // hard code for now
						MaterialList::materials[material_index + MaterialList::num_materials].ks       = parser.pfloats[2];
						MaterialList::materials[material_index + MaterialList::num_materials].power    = parser.pfloats[3];

						for (int rgb_index = 0; rgb_index < 3; rgb_index++){
							// ambient reflectivity
							MaterialList::materials[material_index + MaterialList::num_materials].ra.rgba_M[rgb_index] = 
								MaterialList::materials[material_index + MaterialList::num_materials].ka * 
								MaterialList::materials[material_index + MaterialList::num_materials].color.rgba_M[rgb_index] + 
								0.5f;

							// diffuse reflectivity
							MaterialList::materials[material_index + MaterialList::num_materials].rd.rgba_M[rgb_index] = 
								MaterialList::materials[material_index + MaterialList::num_materials].kd * 
								MaterialList::materials[material_index + MaterialList::num_materials].color.rgba_M[rgb_index] + 
								0.5f;

							// specular reflectivity
							MaterialList::materials[material_index + MaterialList::num_materials].rs.rgba_M[rgb_index] = 
								MaterialList::materials[material_index + MaterialList::num_materials].ks * 
								MaterialList::materials[material_index + MaterialList::num_materials].color.rgba_M[rgb_index] + 
								0.5f;
						}
						break;
					}
				}


				//  look for the "shader class: color" 定位颜色属性
				while (1)
				{
					// get the next line
					if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
					{
						CCLOG("shader class ended abruptly! in .COB file %s.", filename.c_str());
						return false;
					}

					if (parser.match(parser.cur_line.c_str(), "['Shader'] ['class:'] ['color']") )
					{
						break;
					}
				}

				while (1)
				{
					// get the next line
					if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
					{
						CCLOG("shader name ended abruptly! in .COB file %s.", filename.c_str());
						return false;
					}

					// replace the " with spaces
					parser.cur_line = replace(parser.cur_line, "\"", ' ');

					// is this a "plain color" poly?
					if (parser.match(parser.cur_line.c_str(), "['Shader'] ['name:'] ['plain'] ['color']")) // 无纹理
					{
						// not much to do this is default, we need to wait for the reflectance type
						// to tell us the shading mode

						break;
					}

					// is this a "texture map" poly?
					if (parser.match(parser.cur_line.c_str(), "['Shader'] ['name:'] ['texture'] ['map']")) // 有纹理
					{
						// set the texture mapping flag in material
						setBit(MaterialList::materials[material_index + MaterialList::num_materials].attr, MATERIAL_ATTR_SHADE_MODE_TEXTURE);

						// almost done, we need the file name of the darn texture map, its in this format:
						// file name: string "D:\Source\..\models\textures\wall01.bmp"

						// of course the filename in the quotes will change
						// so lets hunt until we find it...
						while (1)
						{
							// get the next line
							if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
							{
								CCLOG("couldnt find texture name! in .COB file %s.", filename.c_str());
								return false;
							}

							// replace the " with spaces
							parser.cur_line = replace(parser.cur_line, "\"", ' ');

							// is this the file name?
							if (parser.match(parser.cur_line.c_str(), "['file'] ['name:'] ['string']") )
							{
								// and save the FULL filename (useless though since its the path from the 
								// machine that created it, but later we might want some of the info).
								// filename and path starts at char position 19, 0 indexed
								std::string texture_filename = trim(parser.cur_line.substr(18));
								memcpy(MaterialList::materials[material_index + MaterialList::num_materials].texture_file, texture_filename.c_str(), texture_filename.length() + 1);

								// the OBJECT4DV2 is only allowed a single texture, although we are loading in all
								// the materials, if this is the first texture map, load it, and set a flag disallowing
								// any more texture loads for the object
								if (!this->texture)
								{
									// step 1: allocate memory for bitmap
									this->texture = new BitmapImage;

									// load the texture, just use the final file name and the absolute global texture path
									this->texture->loadFromFile(const_cast<char*>(("texture/" + extractFilename(texture_filename)).c_str()));

									// 仅支持24位位图 (load失败的话bpp==0)
									if (this->texture->bpp != 24)
									{
										this->texture->reset(); // 阻止后期绘制
									}

									// flag object as having textures
									setBit(this->attr, OBJECT4D_ATTR_TEXTURE);
								}

								// TODO: 未设置MaterialList_V1::materials[material_index + MaterialList_V1::num_materials].texture ??
								break;
							}
						}

						break;
					}
				}

				while (1)
				{
					// get the next line
					if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
					{
						CCLOG("shader transparency class not found in .COB file %s.", filename.c_str());
						return false;
					}

					// look for "Shader class: transparency"
					if (parser.match(parser.cur_line.c_str(), "['Shader'] ['class:'] ['transparency']") )
					{
						// now we know the next "shader name" is what we are looking for so, break
						break;
					}
				}

				while (1)
				{
					// get the next line
					if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
					{
						CCLOG("shader name ended abruptly! in .COB file %s.", filename.c_str());
						return false;
					}

					// get rid of those quotes					
					parser.cur_line = replace(parser.cur_line, "\"", ' ');

					// did we find the name?
					if (parser.match(parser.cur_line.c_str(), "['Shader'] ['name:'] [s>0]"))
					{
						// figure out if transparency is enabled
						if (strcmp(parser.pstrings[2], "none") == 0) // 不透明
						{
							// disable the alpha bit and write 0 alpha
							resetBit(MaterialList::materials[material_index + MaterialList::num_materials].attr, MATERIAL_ATTR_TRANSPARENT);

							// set alpha to 255 完全不透明
							MaterialList::materials[material_index + MaterialList::num_materials].color.a = 255;
						}
						else						
							if (strcmp(parser.pstrings[2], "filter") == 0) // 透明
							{
								// enable the alpha bit and write the alpha level
								setBit(MaterialList::materials[material_index + MaterialList::num_materials].attr, MATERIAL_ATTR_TRANSPARENT);

								// now search for color line to extract alpha level
								//  look for the "colour: color (x, x, x)"
								while (1)
								{
									// get the next line
									if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
									{
										CCLOG("transparency color not found in .COB file %s.", filename.c_str());
										return false;
									}

									// get rid of extraneous characters
									parser.cur_line = replace(parser.cur_line, ":(,)", ' ');									

									// look for colour: color (146, 146, 146)
									if (parser.match(parser.cur_line.c_str(), "['colour'] ['color'] [i] [i] [i]"))
									{
										// set the alpha level to the highest value
										int max_alpha = max(max(parser.pints[0], parser.pints[1]), parser.pints[2]);

										// clamp
										if (max_alpha < 0)
										{
											max_alpha = 0;
										}
										else
											if (max_alpha > 255)
											{
												max_alpha = 255;
											}

											// set alpha value 透明度
											MaterialList::materials[material_index + MaterialList::num_materials].color.a = max_alpha;

											break;
									}
								}
							}

							break;
					}
				}

				//定位(光照)反射属性
				while (1)
				{
					// get the next line
					if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
					{
						CCLOG("shader reflectance class not found in .COB file %s.", filename.c_str());
						return false;
					}

					// look for "Shader class: reflectance"
					if (parser.match(parser.cur_line.c_str(), "['Shader'] ['class:'] ['reflectance']"))
					{
						// now we know the next "shader name" is what we are looking for so, break
						break;
					}
				}

				while (1)
				{
					// get the next line
					if (!parser.getLine(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
					{
						CCLOG("shader name ended abruptly! in .COB file %s.", filename.c_str());
						return false;
					}

					// get rid of those quotes
					parser.cur_line = replace(parser.cur_line.c_str(), "\"", ' ');

					// did we find the name?
					if (parser.match(parser.cur_line.c_str(), "['Shader'] ['name:'] [s>0]"))
					{
						// figure out which shader to use
						if (strcmp(parser.pstrings[2], "constant") == 0)
						{
							// set the shading mode flag in material
							setBit(MaterialList::materials[material_index + MaterialList::num_materials].attr, MATERIAL_ATTR_SHADE_MODE_CONSTANT);
						}
						else
							if (strcmp(parser.pstrings[2], "matte") == 0)
							{
								// set the shading mode flag in material
								setBit(MaterialList::materials[material_index + MaterialList::num_materials].attr, MATERIAL_ATTR_SHADE_MODE_FLAT);
							}
							else
								if (strcmp(parser.pstrings[2], "plastic") == 0)
								{
									// set the shading mode flag in material
									setBit(MaterialList::materials[curr_material + MaterialList::num_materials].attr, MATERIAL_ATTR_SHADE_MODE_GOURAUD);
								}
								else
									if (strcmp(parser.pstrings[2], "phong") == 0)
									{
										// set the shading mode flag in material
										setBit(MaterialList::materials[material_index + MaterialList::num_materials].attr, MATERIAL_ATTR_SHADE_MODE_FASTPHONG);
									}
									else
									{
										// set the shading mode flag in material
										setBit(MaterialList::materials[material_index + MaterialList::num_materials].attr, MATERIAL_ATTR_SHADE_MODE_FLAT);
									}

									break;
					}
				}

				break;
				
			}
		}
	
	}


	///

	for (int curr_poly = 0; curr_poly < this->num_triangles; curr_poly++)
	{
		/*
		CCLOG("fixing poly material %d from index %d to index %d", 
			curr_poly, 
			poly_material[curr_poly],
			poly_material[curr_poly] + MaterialList_V1::num_materials);
		*/
		// fix up offset
		poly_material[curr_poly] = poly_material[curr_poly] + MaterialList::num_materials;

		// we need to know what color depth we are dealing with, so check
		// the bits per pixel, this assumes that the system has already
		// made the call to DDraw_Init() or set the bit depth

		// cool, 32 bit mode
		setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_RGB32);

		// test if this is a textured poly, if so override the color to WHITE,
		// so we get maximum reflection in lighting stage
		if (MaterialList::materials[poly_material[curr_poly]].attr & MATERIAL_ATTR_SHADE_MODE_TEXTURE)
		{
			this->triangle_ref_list[curr_poly].color = Color32::WHITE; // RGB16Bit(255,255,255); 有纹理的三角形 一律将颜色设置为白色 以反射所有的光线
			this->triangle_ref_list[curr_poly].color.a = MaterialList::materials[poly_material[curr_poly]].color.a; // 保留正确的a分量
		}
		else
		{
			this->triangle_ref_list[curr_poly].color = MaterialList::materials[poly_material[curr_poly]].color; // 已包含正确的a分量			
// 			this->poly_list[curr_poly].color = Color32(
// 				MaterialList_V1::materials[poly_material[curr_poly]].color.r,
// 				MaterialList_V1::materials[poly_material[curr_poly]].color.g,
// 				MaterialList_V1::materials[poly_material[curr_poly]].color.b,
// 				255);
		}

		// now set all the shading flags
		// figure out which shader to use
		if (MaterialList::materials[poly_material[curr_poly]].attr & MATERIAL_ATTR_SHADE_MODE_CONSTANT)
		{
			// set shading mode
			setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_SHADE_MODE_CONSTANT);
			this->triangle_ref_list[curr_poly].lit_color[0] = this->triangle_ref_list[curr_poly].color; // fixed by mazi, for no lights, 已包含正确的a分量 (可忽略,因为不从lit_color[]取.a)
		}
		else
			if (MaterialList::materials[poly_material[curr_poly]].attr & MATERIAL_ATTR_SHADE_MODE_FLAT)
			{
				// set shading mode
				setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_SHADE_MODE_FLAT);
				this->triangle_ref_list[curr_poly].lit_color[0] = Color32::BLACK; // fixed by mazi, for no lights
			}
			else
				if (MaterialList::materials[poly_material[curr_poly]].attr & MATERIAL_ATTR_SHADE_MODE_GOURAUD)
				{
					// set shading mode
					setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_SHADE_MODE_GOURAUD);

					// going to need vertex normals!
					setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[0]].attr, VERTEX4D_ATTR_NORMAL); 
					setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[1]].attr, VERTEX4D_ATTR_NORMAL); 
					setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[2]].attr, VERTEX4D_ATTR_NORMAL);

					this->triangle_ref_list[curr_poly].lit_color[0] = Color32::BLACK; // fixed by mazi, for no lights
					this->triangle_ref_list[curr_poly].lit_color[1] = Color32::BLACK;
					this->triangle_ref_list[curr_poly].lit_color[2] = Color32::BLACK;
				}
				else
					if (MaterialList::materials[poly_material[curr_poly]].attr & MATERIAL_ATTR_SHADE_MODE_FASTPHONG)
					{
						// set shading mode
						setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_SHADE_MODE_FASTPHONG);

						// going to need vertex normals!
						setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[0]].attr, VERTEX4D_ATTR_NORMAL); 
						setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[1]].attr, VERTEX4D_ATTR_NORMAL); 
						setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[2]].attr, VERTEX4D_ATTR_NORMAL);

						this->triangle_ref_list[curr_poly].lit_color[0] = Color32::BLACK; // fixed by mazi, for no lights
						this->triangle_ref_list[curr_poly].lit_color[1] = Color32::BLACK;
						this->triangle_ref_list[curr_poly].lit_color[2] = Color32::BLACK;
					}
					else
					{
						// set shading mode to default flat
						setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_SHADE_MODE_FLAT);
						this->triangle_ref_list[curr_poly].lit_color[0] = Color32::BLACK; // fixed by mazi, for no lights
					}

		if (MaterialList::materials[poly_material[curr_poly]].attr & MATERIAL_ATTR_SHADE_MODE_TEXTURE)
		{
			// set shading mode
			setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_SHADE_MODE_TEXTURE);

			// apply texture to this polygon
			this->triangle_ref_list[curr_poly].texture = this->texture; // 三角形纹理 = 物体纹理

			// set texture coordinate attributes
			setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[0]].attr, VERTEX4D_ATTR_TEXTURE); // 纹理坐标信息有效
			setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[1]].attr, VERTEX4D_ATTR_TEXTURE);
			setBit(this->vert_list_local[this->triangle_ref_list[curr_poly].vert_idx[2]].attr, VERTEX4D_ATTR_TEXTURE);
		}

		// now test for alpha channel
		if (MaterialList::materials[poly_material[curr_poly]].attr & MATERIAL_ATTR_TRANSPARENT)
		{
			// set the alpha flag in polygon 透明
			setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_TRANSPARENT);
		}

		// set the material mode to ver. 1.0 emulation (for now only!!!)
		setBit(this->triangle_ref_list[curr_poly].attr, TRIANGLE_ATTR_DISABLE_MATERIAL);
	} // end for curr_poly


	///待续
	MaterialList::num_materials+=num_material_object;

	if (this->texture){
		for (int tvertex = 0; tvertex < num_texture_vertices; tvertex++){
			// step 1: scale the texture coordinates by the texture size
			int texture_size = this->texture->width; 

			// scale 0..1 to 0..texture_size-1 纹理坐标浮点数转整数
			this->coord_list[tvertex].x *= (texture_size - 1); 
			this->coord_list[tvertex].y *= (texture_size - 1);

			// now test for vertex transformation flags
			if (vertex_flags & VERTEX_FLAGS_INVERT_TEXTURE_U)  
			{
				this->coord_list[tvertex].x = (texture_size-1) - this->coord_list[tvertex].x;
			}

			if (vertex_flags & VERTEX_FLAGS_INVERT_TEXTURE_V)  
			{
				this->coord_list[tvertex].y = (texture_size-1) - this->coord_list[tvertex].y;
			}

			if (vertex_flags & VERTEX_FLAGS_INVERT_SWAP_UV)  
			{
				std::swap(this->coord_list[tvertex].x, this->coord_list[tvertex].y);
			}
		}
	}



	//noooooooooooo
//hjh

	this->computePolyNormals();
	this->computeVertexNormals();

	return true;

}



void Object4D::transformByMat(const Matrix_4X4 &mat, TransformMode mode, bool transform_basis, bool all_frames){

	Vector4D vresult;

	int endIndex=num_vertices;

	if (all_frames){
		endIndex=total_vertices;
	}

	Vertex4D* curr_local_vertex=nullptr;
	Vertex4D* curr_trans_vertex=nullptr;
	for (int vertex=0;vertex<endIndex;vertex++){
		
		curr_local_vertex=&this->vert_list_local[vertex];
		curr_trans_vertex=&this->vert_list_trans[vertex];

		switch(mode){
		case TRANSFORMMODE_LOCAL_ONLY:
			
			curr_local_vertex->point.mulMat4X4To(mat,vresult);
			curr_local_vertex->point=vresult;
			if (curr_local_vertex->attr&VERTEX4D_ATTR_NORMAL){
				curr_local_vertex->normal.mulMat4X4To(mat,vresult);
				curr_local_vertex->normal=vresult;
			}
			//curr_local_vertex=&this->vert_list_local[vertex].point;
			
			break;
		case TRANSFORMMODE_TRANS_ONLY:
			curr_trans_vertex->point.mulMat4X4To(mat,vresult);
			curr_trans_vertex->point=vresult;
			if (curr_trans_vertex->attr&VERTEX4D_ATTR_NORMAL){
				curr_trans_vertex->normal.mulMat4X4To(mat,vresult);
				curr_trans_vertex->normal=vresult;
			}

			break;
		case TRANSFORMMODE_LOCAL_TO_TRANS:
			//this->vert_list_local[vertex].point.mulMat4X4To(mat,this->head_vert_list_trans[vertex].point);
			curr_local_vertex->point.mulMat4X4To(mat,curr_trans_vertex->point);
			if (curr_local_vertex->attr&VERTEX4D_ATTR_NORMAL){
				curr_local_vertex->normal.mulMat4X4To(mat,curr_trans_vertex->normal);
			}

			break;
		default:
			break;
		} 
	}

	if (transform_basis){
		this->ux.mulMat4X4To(mat,vresult);
		this->ux=vresult;

		this->uy.mulMat4X4To(mat,vresult);
		this->uy=vresult;

		this->uz.mulMat4X4To(mat,vresult);
		this->uz=vresult;
	}
}

void Object4D::transformModelToWorld(TransformMode mode, bool all_frames){
	int endIndex=num_vertices;

	if (all_frames){
		endIndex=total_vertices;
	}

	for (int vertex=0;vertex<this->num_vertices;vertex++){
		switch(mode){
		case TRANSFORMMODE_LOCAL_TO_TRANS:
			//for (int index=0;index<endIndex;index++){
				this->vert_list_local[vertex].point.addTo(this->world_pos,this->vert_list_trans[vertex].point);
				this->vert_list_trans[vertex].normal=this->vert_list_local[vertex].normal;
			//}
			break;
		default:
			//一律按 TRANSFORMMODE_TRANS_ONLY 处理
			//for (int index=0;index<endIndex;index++){
				this->vert_list_trans[vertex].point.addTo(this->world_pos,this->vert_list_trans[vertex].point);
			//}
			break;
		}
	}

}


void Object4D::transformWorldToCamera(const Camera& cam){

	for (int index=0;index<this->num_vertices;index++){
		Point4D presult;
		this->vert_list_trans[index].point.mulMat4X4To(cam.mat_cam,presult);
		this->vert_list_trans[index].point=presult;

		//法线没有处理
		
	}
}

//转到视景体坐标
void Object4D::transformCameraToPerspective(const Camera& cam){

	for (int index=0;index<this->num_vertices;index++){
		float z=this->vert_list_trans[index].z;
		this->vert_list_trans[index].x=cam.view_dist*this->vert_list_trans[index].x/z;
		this->vert_list_trans[index].y = cam.view_dist * this->vert_list_trans[index].y * cam.aspect_ratio / z;
	}
}

void Object4D::transformPerspectiveToScreen(const Camera &cam){
	float alpha = 0.5f * cam.viewport_width - 0.5f;
	float beta  = 0.5f * cam.viewport_height - 0.5f;
	for (int vertex = 0; vertex < this->num_vertices; vertex++)
	{
		// 此处透视坐标x、y都已经归一化 即取值范围为-1～1
		this->vert_list_trans[vertex].x = alpha + alpha * this->vert_list_trans[vertex].x;
		this->vert_list_trans[vertex].y = beta  - beta  * this->vert_list_trans[vertex].y;
	}

}

void Object4D::computeRadius(){
	
	this->avg_radius[this->curr_frame]=0;
	this->max_radius[this->curr_frame]=0;
	int max=0;
	for (int index=0;index<this->num_vertices;index++){
		int x=this->vert_list_local[index].x;
		x=x*x;

		int y=this->vert_list_local[index].y;
		y=y*y;

		int z=this->vert_list_local[index].z;
		z=z*z;

		float dist_to_vertex=sqrtf(x+y+z);

		this->avg_radius[this->curr_frame]+=dist_to_vertex;
		if (dist_to_vertex>max){
			max=dist_to_vertex;
		}
	}
	this->avg_radius[this->curr_frame]=this->avg_radius[curr_frame]/num_vertices;
	this->max_radius[this->curr_frame]=max;
}

void Object4D::computePolyNormals(){
	for (int poly = 0; poly < this->num_triangles; poly++)
	{
		// extract vertex indices into master list, rember the polygons are 
		// NOT self contained, but based on the vertex list stored in the object
		// itself
		int vindex_0 = this->triangle_ref_list[poly].vert_idx[0];
		int vindex_1 = this->triangle_ref_list[poly].vert_idx[1];
		int vindex_2 = this->triangle_ref_list[poly].vert_idx[2];

		// we need to compute the normal of this polygon face, and recall
		// that the vertices are in cw order, u=p0->p1, v=p0->p2, n=uxv
		Vector4D u, v, n;

		// build u, v
		Vector4D::build(this->vert_list_local[vindex_0].point, this->vert_list_local[vindex_1].point, u);
		Vector4D::build(this->vert_list_local[vindex_0].point, this->vert_list_local[vindex_2].point, v);

		// compute cross product
		u.crossTo(v, n);

		// compute length of normal accurately and store in poly nlength
		// +- epsilon later to fix over/underflows
		this->triangle_ref_list[poly].nlength = n.length();

		this->triangle_ref_list[poly].normal=n;
	}
}


//计算顶点法向量
void Object4D::computeVertexNormals(){

	//每个顶点所涉及的三角数量
	int polys_touch_vertex[OBJECT4D_MAX_VERTICES];

	memset(polys_touch_vertex, 0, sizeof(int) * OBJECT4D_MAX_VERTICES);


	for (int poly = 0; poly < this->num_triangles; poly++)
	{
		//CCLOG("processing poly %d", poly);

		// test if this polygon needs vertex normals
		if (this->triangle_ref_list[poly].attr & TRIANGLE_ATTR_SHADE_MODE_GOURAUD) // 仅当三角形为Gouraud着色模式时计算
		{
			int vindex_0 = this->triangle_ref_list[poly].vert_idx[0];
			int vindex_1 = this->triangle_ref_list[poly].vert_idx[1];
			int vindex_2 = this->triangle_ref_list[poly].vert_idx[2];

	 		Vector4D u, v, n;

			// build u, v
			Vector4D::build(this->vert_list_local[vindex_0].point, this->vert_list_local[vindex_1].point, u);
			Vector4D::build(this->vert_list_local[vindex_0].point, this->vert_list_local[vindex_2].point, v);
 
			u.crossTo(v, n);  
			// update vertex array to flag this polygon as a contributor 顶点被共用计数加1
			polys_touch_vertex[vindex_0]++;
			polys_touch_vertex[vindex_1]++;
			polys_touch_vertex[vindex_2]++;

			this->vert_list_local[vindex_0].normal.addTo(n, this->vert_list_local[vindex_0].normal); // 顶点法向量累加上三角形(片)法向量 这里没有对三角形法线归一化 是因为法线长度对应了三角形的面积(加权平均的权重)
			this->vert_list_local[vindex_1].normal.addTo(n, this->vert_list_local[vindex_1].normal);
			this->vert_list_local[vindex_2].normal.addTo(n, this->vert_list_local[vindex_2].normal);
		}
	}

	// now we are almost done, we have accumulated all the vertex normals, but need to average them
	for (int vertex = 0; vertex < this->num_vertices; vertex++)
	{

		// test if this vertex has a normal and needs averaging
		if (polys_touch_vertex[vertex] >= 1)
		{
			this->vert_list_local[vertex].nx /= polys_touch_vertex[vertex]; // TODO: 一定要除吗 ?? 直接归一化不就行了 ??
			this->vert_list_local[vertex].ny /= polys_touch_vertex[vertex];
			this->vert_list_local[vertex].nz /= polys_touch_vertex[vertex];
			
			// now normalize the normal
			this->vert_list_local[vertex].normal.normalize(); // 归一化 顶点法向量

		}
	}

}


void Object4D::resetFlag(){
	//this->state &= ~OBJECT4D_STATE_CULLED;
	this->state=OBJECT4D_STATE_ACTIVE|OBJECT4D_STATE_VISIBLE;
	for(int index=0;index<this->num_triangles;index++){
		TriangleRef* curr_triangle=&this->triangle_ref_list[index];
		if (curr_triangle->state&TRIANGLE_STATE_ACTIVE){
			continue;
		}
		curr_triangle->state &= ~TRIANGLE_STATE_CLIPPED;
		curr_triangle->state &= ~TRIANGLE_STATE_BACKFACE;
		curr_triangle->state &= ~TRIANGLE_STATE_LIT;
	}
}

bool Object4D::cull(const Camera& camer,int cull_flag){
	Point4D sphere_pos;

	//得到在相机坐标系中的位置
	this->world_pos.mulMat4X4To(camer.mat_cam,sphere_pos);
	if (cull_flag&CULL_OBJECT_Z_PLANE){
		if((sphere_pos.z-this->max_radius[curr_frame]) > camer.far_clip_z ||
			(sphere_pos.z+this->max_radius[curr_frame]<camer.near_clip_z)){
				this->state |=OBJECT4D_STATE_CULLED;
				return true;
		}
	}
	if (cull_flag&CULL_OBJECT_X_PLANE){
		float z_test=0.5f*camer.viewplane_width*sphere_pos.z/camer.view_dist;
		if ((sphere_pos.x - this->max_radius[this->curr_frame]) > z_test || (sphere_pos.x + this->max_radius[this->curr_frame]) < -z_test)
		{ 
			this->state |= OBJECT4D_STATE_CULLED;
			return true;
		}
	}

	if (cull_flag & CULL_OBJECT_Y_PLANE){
		float z_test = 0.5f * camer.viewplane_height * sphere_pos.z / camer.view_dist;

		if ((sphere_pos.y - this->max_radius[this->curr_frame]) > z_test || (sphere_pos.y + this->max_radius[this->curr_frame]) < -z_test)
		{
			this->state |= OBJECT4D_STATE_CULLED;
			return true;
		}
	}
	return false;

}

