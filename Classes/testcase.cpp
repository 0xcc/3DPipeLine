#include "testcase.h"
#include "config.h"
#include <string>
 
void TestCase::init(Engine* engine){
	this->mEnginePtr=engine;
}

void TestCase::update(float dt){

}

void TestCase::free(){

}

void TestCase::keyDown(int keyCode){

}

void TestCase::keyUp(int keyCode){};

void TestCase::debug(){
	mNext=true;
}
 
//////////////////////////////////////////////////////////////////////
void TestCase_2D::init(Engine* engine){
	TestCase::init(engine);
	if (mEnginePtr){
		mEnginePtr->beginDraw();
		/*
		mEnginePtr->drawLine(6,60,66,70,Color32::RED);
		mEnginePtr->drawLine(100, 100, 500, 400, Color32::RED);
		mEnginePtr->drawLine(150, 100, 500, 100, Color32::GREEN);
		*/

		//mEnginePtr->drawClipLine(-100, -50, 2000, 3000, Color32::RED);
		mEnginePtr->drawTopTriangle(120, 50, 220, 50, 170, 100, Color32::RED);
		mEnginePtr->drawBottomTriangle(170, 0, 
										120, 50, 
										220, 50, Color32::ORANGE);
		/*
		mEnginePtr->drawTriangle2D(200,10,
									250,10,
									250,100,Color32::ORANGE);
									
		mEnginePtr->drawTriangle2D(200,10,
			300,100,
			250,100,
			Color32::ORANGE);
			*/
		mEnginePtr->drawTriangle2D(200,10,
			300,100,
			250,200,
			Color32::ORANGE);

		mEnginePtr->drawQuad2D(660, 60, 740, 100, 700, 150, 650, 120, Color32::YELLOW);


		struct BitmapImage bmp;
		if (bmp.loadFromFile("texture/liumazi.bmp"))
		{
			mEnginePtr->drawBitmap(80, 300, bmp);
		}

		mEnginePtr->endDraw();
	}

}
 

void TestCase_2D::update(float dt){

	if (mEnginePtr){
		//mEnginePtr->beginDraw();
		/*
		mEnginePtr->drawTopTriangle(120, 50, 220, 50, 170, 100, Color32::RED);
		mEnginePtr->drawBottomTriangle(170, 0, 
			120, 50, 
			220, 50, Color32::ORANGE);


		mEnginePtr->drawTriangle2D(200,10,
			300,100,
			250,200,
			Color32::ORANGE);

		mEnginePtr->drawQuad2D(660, 60, 740, 100, 700, 150, 650, 120, Color32::YELLOW);


		static struct BitmapImage bmp;
		if (!bmp.data){
			if (bmp.loadFromFile("texture/liumazi.bmp"))
			{
				mEnginePtr->drawBitmap(80, 300, bmp);
			}
		}


		 */

		//mEnginePtr->endDraw();
	
	}

}

///////////////////////////////////////////////TestCase_Wire3D///////////////////////////////////////////////////////

void TestCase_Wire3D::init(Engine* engine){
	TestCase::init(engine);

	//X转1度
	mMatRotX.buildEulerRotation(PI/180.0, 0, 0);
	//Y转1度
	mMatRotY.buildEulerRotation(0, PI/180.0, 0);
	//Z转1度
	mMatRotZ.buildEulerRotation(0, 0, PI / 180.0f);

	//相机位置 -200处
	Point4D cameraPos;
	cameraPos.init(0, 0, -200,1);

	this->mCamera.init(CAM_MODEL_EULER,
						cameraPos,
						Vector4D::ZERO, //欧拉相机旋转角度，0度
						nullptr,
						50.0f,
						500.0f,
						90.0f,
						RENDER_VIEWPORT_WIDTH,
						RENDER_VIEWPORT_HEIGHT);

	mCamera.buildCameraMatByEuler(CAM_ROT_SEQ_ZYX);
	
	 
	Triangle triangle;
	memset(&triangle,0,sizeof(Triangle));

	triangle.state=TRIANGLE_STATE_ACTIVE;
	triangle.attr=0;
	triangle.color=Color32::RED;
	
	triangle.vert_list_local[0].x=0;
	triangle.vert_list_local[0].y=50;
	triangle.vert_list_local[0].z=0;
	triangle.vert_list_local[0].w=1;

	triangle.vert_list_local[1].x=50;
	triangle.vert_list_local[1].y=-50;
	triangle.vert_list_local[1].z=0;
	triangle.vert_list_local[1].w=1;

	triangle.vert_list_local[2].x=-50;
	triangle.vert_list_local[2].y=-50;
	triangle.vert_list_local[2].z=0;
	triangle.vert_list_local[2].w=1;
	

	triangle.next=triangle.prev=nullptr;

	mRenderList.reset();
	mRenderList.insert(triangle);


	Vector4D scale_v;
	Point4D  pos_v;
	scale_v.init(8,8,8,1);
	pos_v.init(160,0,100,1);
	mCube1=new Object4D();
	mCube1->loadFromPLX1("model/cube1.plg",scale_v,pos_v,Vector4D::ZERO);

	mCube2=new Object4D();
	pos_v.init(-160, 0, 100, 1);
	mCube2->loadFromPLX1("model/cube2.plg",scale_v,pos_v,Vector4D::ZERO);


}

void TestCase_Wire3D::update(float dt){
	if (mEnginePtr){
		Point4D pos_v;
		pos_v.init(0,0,100,1);
		mEnginePtr->beginDraw();
		
		mRenderList.transformByMat(mMatRotX,TRANSFORMMODE_LOCAL_ONLY);
		mRenderList.transformModelToWorld(pos_v);
		mRenderList.transformWorldToCamera(mCamera);
		mRenderList.transformCameraToPerspective(mCamera);
		mRenderList.transformPerspectiveToScreen(mCamera);

		mEnginePtr->drawRenderListWire(mRenderList);
		
		
		mCube1->transformByMat(mMatRotY,TRANSFORMMODE_LOCAL_ONLY,true,true);
		mCube1->transformModelToWorld(TRANSFORMMODE_LOCAL_TO_TRANS);
		mCube1->transformWorldToCamera(mCamera);
		mCube1->transformCameraToPerspective(mCamera);
		mCube1->transformPerspectiveToScreen(mCamera);
		mEnginePtr->drawObjectWire(*mCube1);

		mCube2->transformByMat(mMatRotZ,TRANSFORMMODE_LOCAL_ONLY,true,true);
		mCube2->transformModelToWorld(TRANSFORMMODE_LOCAL_TO_TRANS);
		mCube2->transformWorldToCamera(mCamera);
		mCube2->transformCameraToPerspective(mCamera);
		mCube2->transformPerspectiveToScreen(mCamera);
		mEnginePtr->drawObjectWire(*mCube2);
		
		mEnginePtr->endDraw();
	}
}

//////////////////////////////////TestCase_Camera/////////////////////////////////////////
void TestCase_Camera::init(Engine* engine){
	TestCase::init(engine);
	mMatRot.buildEulerRotation(0,PI_DIV_180,0);


	mCamera.init(CAM_MODEL_UVN,
				Point4D::ZERO,//相机位置
				Vector4D::ZERO,//相机旋转向量
				&Point4D::ZERO,//相机注视目标点
				50.0f,//近平面
				5000.0f,//远平面
				90.0f,	//fov角度
				RENDER_VIEWPORT_WIDTH,
				RENDER_VIEWPORT_HEIGHT
				);

	mTank.loadFromPLX1("model/tank1.plg",Vector4D::ONE,Point4D::ZERO,Vector4D::ZERO);

}

void TestCase_Camera::update(float dt){
	
	if (mEnginePtr){
		

		static float view_angle=0.0f;
		static float camera_distance=1750.0f;
		mCamera.pos.x=camera_distance*cosf(view_angle);
		mCamera.pos.y=camera_distance*sinf(view_angle);
		mCamera.pos.z=2*camera_distance*sinf(view_angle);

		view_angle+=PI_DIV_180;
		if (view_angle>=PI2){
			view_angle=0.0f;
		}

		// 以相机位置和注视点(0,0,0,1)计算相机UVN矩阵
		mCamera.buildCameraMatByUVN(UVN_MODE_SIMPLE);

		mTank.transformByMat(mMatRot,TRANSFORMMODE_LOCAL_ONLY,true,true);

		mRenderList.reset();
		int x = -NUM_OBJECTS / 2;
		int z =- NUM_OBJECTS / 2;
		for (int x = -NUM_OBJECTS / 2; x < NUM_OBJECTS / 2; x++){
			for (int z =- NUM_OBJECTS / 2; z < NUM_OBJECTS / 2; z++){
				mTank.resetFlag();
				mTank.world_pos.x = x * SPACING_OBJECT + SPACING_OBJECT / 2;
				mTank.world_pos.y = 0;
				mTank.world_pos.z = z * SPACING_OBJECT + SPACING_OBJECT / 2;
				if (!mTank.cull(mCamera, CULL_OBJECT_XYZ_PLANES))
				{
					mTank.transformModelToWorld(TRANSFORMMODE_LOCAL_TO_TRANS);
					mRenderList.insert(mTank);
				}
			}
		}

		
		mRenderList.transformWorldToCamera(mCamera); // apply world to camera transform
		mRenderList.transformCameraToPerspective(mCamera); // apply camera to perspective transformation
		mRenderList.transformPerspectiveToScreen(mCamera); // apply screen transform

		mEnginePtr->beginDraw();
		mEnginePtr->drawRenderListWire(mRenderList);
		mEnginePtr->endDraw();
	}
	
}


///////////////////////////////////////////////////////////////////////////////////
void TestCase_Light::init(Engine* engine){
	
	TestCase::init(engine);

	Point4D cameraPos;
	cameraPos.init(0,40,0,1);

	mCamera.init(CAM_MODEL_EULER,
		cameraPos,
		Vector4D::ZERO,
		nullptr,
		200.0, //near z plane
		12000.0,//far z plane
		120,//fov
		RENDER_VIEWPORT_WIDTH,
		RENDER_VIEWPORT_HEIGHT
		);
	
	mLightList.reset();

	mLightList.lights_num=4;

	//环境光
	mLightList.initLight(AMBIENT_LIGHT_INDEX,true,LIGHT_ATTR_AMBIENT,
						Color32(99,99,99,255),
						Color32::BLACK,
						Color32::BLACK,
						nullptr,
						nullptr,
						0,0,0,
						0,0,0);


	Vector4D dlight_dir;
	dlight_dir.init(-1,0,-1,1);
	//平行光
	mLightList.initLight(INFINITE_LIGHT_INDEX,true,LIGHT_ATTR_INFINITE,
						Color32::BLACK,
						Color32(99, 99, 99, 255),
						Color32::BLACK,
						nullptr,&dlight_dir,
						0,0,0,
						0,0,0);


	//点光源
	Vector4D plight_pos;
	plight_pos.init(0,200,0,1);
	mLightList.initLight(POINT_LIGHT_INDEX,true,LIGHT_ATTR_POINT,
						Color32::BLACK,
						Color32::GREEN,
						Color32::BLACK,
						&plight_pos,nullptr,
						0,0.0001f,0,
						0,0,1);

	//聚光灯

	Vector4D slight_pos;
	slight_pos.init(0,200,0,1);
	Vector4D slight_dir;
	slight_dir.init(1,0,0,1);
	mLightList.initLight(SPOT2_LIGHT_INDEX,true,LIGHT_ATTR_SPOTLIGHT2,
						Color32::BLACK,
						Color32::RED,
						Color32::BLACK,
						&slight_pos,&slight_dir,
						0,0.0001f,0,
						0,0,1);

	Vector4D scale_v;
	scale_v.init(0.75f, 0.75f, 0.75f, 1.0f);
	mPlayer.loadFromPLX1("model/tank2.plg",scale_v, Point4D::ZERO, Vector4D::ZERO);

	scale_v.init(0.75f, 0.75f, 0.75f, 1.0f);
	mTank.loadFromPLX1("model/tank3.plg", scale_v, Point4D::ZERO, Vector4D::ZERO);

	scale_v.init(1.0f, 2.0f, 1.0f, 1.0f);
	mTower.loadFromPLX1("model/tower1.plg", scale_v, Point4D::ZERO, Vector4D::ZERO);


	scale_v.init(5.0f, 5.0f, 5.0f, 1.0f);
	mMarker.loadFromPLX1("model/marker1.plg", scale_v, Point4D::ZERO, Vector4D::ZERO);
	

	//tank的位置
	for (int index = 0; index < NUM_TANKS; index++)
	{
		// randomly position the tanks
		mTankInfo[index].x = randRange(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
		mTankInfo[index].y = 0; // obj_tank.max_radius;
		mTankInfo[index].z = randRange(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
		mTankInfo[index].w = randRange(0, 360) / 360.0f * PI2; // 注意 w分量是坦克模型局部坐标y轴旋转角度
	}


	// position the towers
	for (int index = 0; index < NUM_TOWERS; index++)
	{
		// randomly position the tower
		mTowerInfo[index].x = randRange(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
		mTowerInfo[index].y = 0; // obj_tower.max_radius;
		mTowerInfo[index].z = randRange(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
	}


	// position the markers

	for (int index_x = 0, index = 0; index_x < NUM_MARKERS_X; index_x++)
	{
		for (int index_z = 0; index_z < NUM_MARKERS_Z; index_z++, index++)
		{
			mMarkerInfo[index].x = randRange(-100, 100) - UNIVERSE_RADIUS + index_x * MARKER_SIZE;
			mMarkerInfo[index].y = mMarker.max_radius[0];
			mMarkerInfo[index].z = randRange(-100, 100) - UNIVERSE_RADIUS + index_z * MARKER_SIZE;
		}
	}

	mTurning = 0;

	mPointLightAng = 0;
	mSpotLightAng = PI2; // angles for light motion

	mKeyDownW = false;
	mKeyDownS = false;
	mKeyDownA = false;
	mKeyDownD = false;

	mWireFrame = false;
	mBackface = true;
	mLighting = true;
	mSortZ = true;

}


void TestCase_Light::update(float dt){

	if (mEnginePtr){


		float tmp=0;
		tmp=PLAYER_SPEED*dt;
		if (KEY_DOWN('W')){
			//keyDown('W');
			mCamera.pos.x+=tmp*sinf(mCamera.dir.y);
			mCamera.pos.z+=tmp*cosf(mCamera.dir.y);

		}
		if (KEY_DOWN('S')){
			//keyDown('S');
			mCamera.pos.x-=tmp*sinf(mCamera.dir.y);
			mCamera.pos.z-=tmp*cosf(mCamera.dir.y);
		}
		if (KEY_DOWN('A')){
			mKeyDownA=true;
			mCamera.dir.y-=PI_DIV_360*4;
			mTurning-=PI_DIV_360*3;
			if (mTurning<-PI_DIV_12){
				mTurning=-PI_DIV_12;
			}
		}
		if (KEY_DOWN('D')){
			mKeyDownD=true;
			mCamera.dir.y += PI_DIV_360 * 4;
			mTurning += PI_DIV_360 * 3;
			if (mTurning > PI_DIV_12)
				mTurning = PI_DIV_12;
		}

		if (!mKeyDownA&& !mKeyDownD){
			if (mTurning > 0)
			{
				mTurning -= PI_DIV_360;
				if (mTurning < 0)
				{
					mTurning = 0;
				}
			}
			else if (mTurning < 0)
				{
					mTurning += PI_DIV_360;
					if (mTurning > 0)
					{
						mTurning = 0;
					}
				}
		}


		mRenderList.reset();
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.x=4000*cosf(mPointLightAng);
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.y=200;
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.z=4000*sinf(mPointLightAng);

		mPointLightAng+=PI_DIV_2*dt;

		if (mPointLightAng>PI2){
			mPointLightAng=0;
		}

		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.x=2000*cosf(mSpotLightAng);
		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.y=200;
		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.z=2000*sinf(mSpotLightAng);
		mSpotLightAng-=PI_DIV_3*dt;

		if (mSpotLightAng<0){
			mSpotLightAng=PI2;
		}

		mCamera.buildCameraMatByEuler(CAM_ROT_SEQ_ZYX);

		mPlayer.resetFlag();

		mPlayer.world_pos.x=mCamera.pos.x+300*sinf(mCamera.dir.y);
		mPlayer.world_pos.y=mCamera.pos.y-70;
		mPlayer.world_pos.z=mCamera.pos.z+300 * cosf(mCamera.dir.y);

		Matrix_4X4 matRot;

		//绕Y轴旋转
		matRot.buildEulerRotation(0,mCamera.dir.y+mTurning,0);
		mPlayer.transformByMat(matRot,TRANSFORMMODE_LOCAL_TO_TRANS,true,true);
		mPlayer.transformModelToWorld(TRANSFORMMODE_TRANS_ONLY);
		mRenderList.insert(mPlayer);
		
		
		for (int index=0;index<NUM_TANKS;index++){
			mTank.resetFlag();

			//这个合理？
			matRot.buildEulerRotation(0,mTankInfo[index].w,0);
			mTank.transformByMat(matRot,TRANSFORMMODE_LOCAL_TO_TRANS,true,true);

			mTank.world_pos.x=mTankInfo[index].x;
			mTank.world_pos.y=mTankInfo[index].y;
			mTank.world_pos.z=mTankInfo[index].z;
			if (!mTank.cull(mCamera,CULL_OBJECT_XYZ_PLANES)){
				mTank.transformModelToWorld(TRANSFORMMODE_TRANS_ONLY);
				mRenderList.insert(mTank);
			}
		}
		
		
		
		for (int index = 0; index < NUM_TOWERS; index++)
		{
			// reset the object (this only matters for backface and object removal)
			mTower.resetFlag();

			mTower.world_pos.x = mTowerInfo[index].x;
			mTower.world_pos.y = mTowerInfo[index].y;
			mTower.world_pos.z = mTowerInfo[index].z;

			if (!mTower.cull(mCamera, CULL_OBJECT_XYZ_PLANES))
			{
				mTower.transformModelToWorld(); 

				mRenderList.insert(mTower); 
			}
		}
		
		for (int index = 0; index < NUM_MARKERS; index++)
		{
			mMarker.resetFlag();

			mMarker.world_pos.x = mMarkerInfo[index].x;
			mMarker.world_pos.y = mMarkerInfo[index].y;
			mMarker.world_pos.z = mMarkerInfo[index].z;

			if (!mMarker.cull(mCamera, CULL_OBJECT_XYZ_PLANES))
			{
				mMarker.transformModelToWorld(TRANSFORMMODE_LOCAL_TO_TRANS); 

				mRenderList.insert(mMarker);
			}		

		}
		/**/
		if (mBackface){
			mRenderList.removeBackface(mCamera);
		}

		//光照函数
		if (mLighting){
			///
			mRenderList.LightInWorld(mCamera,mLightList);
		}

		mRenderList.transformWorldToCamera(mCamera);

		if(mSortZ){
			mRenderList.sortZ(SORT_POLY_METHOD_AVGZ);
		}
		
		mRenderList.transformCameraToPerspective(mCamera);
		mRenderList.transformPerspectiveToScreen(mCamera);

		mEnginePtr->beginDraw();
			mEnginePtr->drawQuad2D(0,0,
									RENDER_VIEWPORT_WIDTH-1,0,
									RENDER_VIEWPORT_WIDTH-1, RENDER_VIEWPORT_HEIGHT/2,
									0,RENDER_VIEWPORT_HEIGHT/2,
									Color32(0,140,192,255)
									);

			mEnginePtr->drawQuad2D(
				0, RENDER_VIEWPORT_HEIGHT / 2,
				RENDER_VIEWPORT_WIDTH-1, RENDER_VIEWPORT_HEIGHT / 2,
				RENDER_VIEWPORT_WIDTH-1, RENDER_VIEWPORT_HEIGHT-1,
				0, RENDER_VIEWPORT_HEIGHT-1,
				Color32(103, 62, 3, 255)
				);
			if (mWireFrame){
				mEnginePtr->drawRenderListWire(mRenderList);
			}else{
				mEnginePtr->drawRenderListSolid(mRenderList);
			}
		mEnginePtr->endDraw();
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////

void TestCase_Gouraud::init(Engine* engine){
	TestCase::init(engine);
	mNext=false;

	mCamera.init(CAM_MODEL_EULER,
		Point4D::ZERO,
		Vector4D::ZERO,
		nullptr,
		200,
		12000,
		90,
		RENDER_VIEWPORT_WIDTH,
		RENDER_VIEWPORT_HEIGHT);

	mCamera.buildCameraMatByEuler(CAM_ROT_SEQ_ZYX);
	mLightList.reset();
	mLightList.lights_num=0;
	
	
	mLightList.initLight(AMBIENT_LIGHT_INDEX,
		true,
		LIGHT_ATTR_AMBIENT,
		Color32(50, 50, 50, 255),
		Color32::BLACK,
		Color32::BLACK,
		nullptr,nullptr,
		0,0,0,
		0,0,0);
	mLightList.lights_num++;
	
	 
	Vector4D dlight_dir = {-1, 0, -1, 0};
	mLightList.initLight(INFINITE_LIGHT_INDEX,  
		true,                  // turn the light on
		LIGHT_ATTR_INFINITE,   // infinite light type
		Color32::BLACK, 
		Color32::GRAY,         // color for diffuse term only
		Color32::BLACK,
		nullptr, &dlight_dir,  // need direction only
		0, 0, 0,               // no need for attenuation
		0, 0, 0);
	mLightList.lights_num++;
	 
	
	//点光源
	mLightList.initLight(POINT_LIGHT_INDEX,
		true,                  // turn the light on
		LIGHT_ATTR_POINT,      // pointlight type
		Color32::BLACK, 
		Color32::GREEN,        // color for diffuse term only
		Color32::BLACK,   
		nullptr, nullptr,      // need pos only 
		0, 0.001f, 0,          // linear attenuation only
		0, 0, 1);
	mLightList.lights_num++;
	
	
	// spot light2 聚光灯
	Vector4D slight2_dir = {-1, 0, -1, 0}; // 聚光灯方向 这里仍然是反着表示的
	mLightList.initLight(SPOT2_LIGHT_INDEX,
		true,                     // turn the light on
		LIGHT_ATTR_SPOTLIGHT2,    // spot light type 2
		Color32::BLACK,
		Color32::RED,             // color for diffuse term only
		Color32::BLACK,
		nullptr, &slight2_dir,    // need direction only
		0, 0.001f, 0,             // linear attenuation only
		0, 0, 1);
	mLightList.lights_num++;
	
	Vector4D vscale;
	vscale.init(20,20,20);

	// load constant shaded water
	mConstantWater.loadFormCOB("model/water_constant_01.cob", vscale, Point4D::ZERO, Vector4D::ZERO, VERTEX_FLAGS_SWAP_YZ | VERTEX_FLAGS_TRANSFORM_LOCAL | VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD);
	//mConstantWater.loadFormCOB("model/cube_gouraud_01.cob",vscale,Point4D::ZERO, Vector4D::ZERO, VERTEX_FLAGS_SWAP_YZ | VERTEX_FLAGS_TRANSFORM_LOCAL | VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD);
	mConstantWater.world_pos.x=-72;
	mConstantWater.world_pos.y=0;
	mConstantWater.world_pos.z=120;


	// load flat shaded water
	mFlatWater.loadFormCOB("model/water_flat_01.cob", vscale, Point4D::ZERO, Vector4D::ZERO, VERTEX_FLAGS_SWAP_YZ | VERTEX_FLAGS_TRANSFORM_LOCAL | VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD);
	mFlatWater.world_pos.x = 0;
	mFlatWater.world_pos.y = 0;
	mFlatWater.world_pos.z = 120;

	// load gouraud shaded water
	mGouraudWater.loadFormCOB("model/water_gouraud_01.cob", vscale, Point4D::ZERO, Vector4D::ZERO, VERTEX_FLAGS_SWAP_YZ | VERTEX_FLAGS_TRANSFORM_LOCAL | VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD);
	mGouraudWater.world_pos.x = 72;
	mGouraudWater.world_pos.y = 0;
	mGouraudWater.world_pos.z = 120;
	
	// 额外的2d三角形
	/*
	//平顶
	mTriangle2D.vert_list_trans[0].x = 220;
	mTriangle2D.vert_list_trans[0].y = 100;
	mTriangle2D.lit_color[0] = Color32::RED;

	mTriangle2D.vert_list_trans[1].x = 480;
	mTriangle2D.vert_list_trans[1].y = 100;
	mTriangle2D.lit_color[1] = Color32::GREEN;

	mTriangle2D.vert_list_trans[2].x = 350;
	mTriangle2D.vert_list_trans[2].y = 500;
	mTriangle2D.lit_color[2] = Color32::BLUE;	
	*/
	
	//平底
	mTriangle2D.vert_list_trans[0].x = 400;
	mTriangle2D.vert_list_trans[0].y = 100;
	mTriangle2D.lit_color[0] = Color32::RED;

	mTriangle2D.vert_list_trans[1].x = 300;
	mTriangle2D.vert_list_trans[1].y = 200;
	mTriangle2D.lit_color[1] = Color32::GREEN;

	mTriangle2D.vert_list_trans[2].x = 500;
	mTriangle2D.vert_list_trans[2].y = 300;
	mTriangle2D.lit_color[2] = Color32::BLUE;	
	
	// 开关变量
	mSortZ = true;
	mBackface = true;
	mLighting = true;
	mWireFrame = false;

}


void TestCase_Gouraud::debug(){
	mNext=true;
}

void TestCase_Gouraud::update(float dt){
	if (mEnginePtr ){
		this->mNext=false;
		static float plight_ang=0,slight_ang=PI2;

		
		// move point light source in ellipse around game world
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.x = 1000 * cosf(plight_ang);
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.y = 100;
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.z = 1000 * cosf(plight_ang);
		
		plight_ang += PI_DIV_6 * dt; // 每秒30度
		if (plight_ang > PI2)
			plight_ang = 0;
		
		
		// move spot light source in ellipse around game world
		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.x = 1000 * cosf(slight_ang);
		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.y = 200;
		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.z = 1000 * sinf(slight_ang);
		
		slight_ang -= PI_DIV_4 * dt; // 每秒45度
		if (slight_ang < 0)
			slight_ang = PI2;

		// use these to rotate objects
		static float x_ang = 0, y_ang = 0, z_ang = 0;


		mLightList.lights[AMBIENT_LIGHT_INDEX].state=0;
		mLightList.lights[INFINITE_LIGHT_INDEX].state=0;
		mLightList.lights[POINT_LIGHT_INDEX].state=0;
		mLightList.lights[SPOT2_LIGHT_INDEX].state=0;


		mLightList.lights[AMBIENT_LIGHT_INDEX].state=1;
		mLightList.lights[INFINITE_LIGHT_INDEX].state=1;
		mLightList.lights[POINT_LIGHT_INDEX].state=1;
		mLightList.lights[SPOT2_LIGHT_INDEX].state=1;

		// 构造局部旋转矩阵
		Matrix_4X4 mat;
		mat.buildEulerRotation(x_ang, y_ang, z_ang);

		// update rotation angles
		
		x_ang += PI_DIV_4 * dt;
		while (x_ang >= PI2)
			x_ang -= PI2;
		
		
		y_ang += PI_DIV_4 * dt * 2;
		while (y_ang >= PI2) 
			y_ang -= PI2;
		
		z_ang += PI_DIV_4 * dt * 3;
		while (z_ang >= PI2) 
			z_ang -= PI2;
		

		mRenderList.reset();
		mConstantWater.resetFlag();

		mConstantWater.transformByMat(mat, TRANSFORMMODE_LOCAL_TO_TRANS, true, false);
		mConstantWater.transformModelToWorld(TRANSFORMMODE_TRANS_ONLY, false);
		Vertex4D* v= mConstantWater.vert_list_trans;

		mRenderList.insert(mConstantWater);
		


		
		mFlatWater.resetFlag();
		mFlatWater.transformByMat(mat, TRANSFORMMODE_LOCAL_TO_TRANS, true, false);
		mFlatWater.transformModelToWorld(TRANSFORMMODE_TRANS_ONLY, false);
		mRenderList.insert(mFlatWater);

		
		mGouraudWater.resetFlag();
		mGouraudWater.transformByMat(mat, TRANSFORMMODE_LOCAL_TO_TRANS, true, false);
		mGouraudWater.transformModelToWorld(TRANSFORMMODE_TRANS_ONLY, false);
		mRenderList.insert(mGouraudWater);
		
		
		if (mBackface){
			mRenderList.removeBackface(mCamera);
		}
		
		if (mLighting)
			mRenderList.LightInWorld(mCamera, mLightList);

		for (int i=0;i<mLightList.lights_num;i++){
			Light& light=mLightList.lights[i];
			if (light.state){
				//CCLOG("pos[%i] %i,%i,%i",i,light.pos_world.x,light.pos_world.y,light.pos_world.z);

			}
		}

		mRenderList.transformWorldToCamera(mCamera);

		if (mSortZ)
			mRenderList.sortZ(SORT_POLY_METHOD_AVGZ);

		mRenderList.transformCameraToPerspective(mCamera);
		mRenderList.transformPerspectiveToScreen(mCamera);

		mEnginePtr->beginDraw();

		//mEnginePtr->drawGouraudTriangle(mTriangle2D);
		if (mWireFrame){
			mEnginePtr->drawRenderListWire(mRenderList);
		}else{
			mEnginePtr->drawRenderListSolid(mRenderList);
		}

		mEnginePtr->endDraw();

	}
}

void TestCase_Gouraud::keyDown(int code){

}

void TestCase_Gouraud::free(){

}




///////////////////////////////////TestCase_Texture/////////////////////////////////////////////////////////////


void TestCase_Texture::init(Engine* engine){
	TestCase::init(engine);

	mCamera.init(
		CAM_MODEL_EULER, // the euler model 欧拉相机
		Point4D::ZERO,   // initial camera position 相机在原点
		Vector4D::ZERO,  // initial camera angles 相机无旋转
		nullptr,         // no target 不需要
		200,             // near and far clipping planes 本例中此参数无用 因为没有对物体做剔除(cull)
		12000,
		60,              // field of view in degrees 视角
		RENDER_VIEWPORT_WIDTH,
		RENDER_VIEWPORT_HEIGHT);

	mCamera.buildCameraMatByEuler(CAM_ROT_SEQ_ZYX);


	mLightList.reset();
	mLightList.lights_num = 4;

	// ambient light 环境光
	mLightList.initLight(AMBIENT_LIGHT_INDEX,   
		true,                     // turn the light on
		LIGHT_ATTR_AMBIENT,       // ambient light type
		Color32(50, 50, 50, 255), // color for ambient term only
		Color32::BLACK,
		Color32::BLACK,
		nullptr, nullptr,         // no need for pos or dir
		0, 0, 0,                  // no need for attenuation
		0, 0, 0);                 // spotlight info NA

	// directional light 平行光
	Vector4D dlight_dir = {-1, 0, -1, 0}; // 平行光方向, 注意是反着表示的, 实际方向为从{-1, 0, -1, 0}指向原点{0, 0, 0, 0}
	mLightList.initLight(INFINITE_LIGHT_INDEX,  
		true,                  // turn the light on
		LIGHT_ATTR_INFINITE,   // infinite light type
		Color32::BLACK, 
		Color32::GRAY,         // color for diffuse term only
		Color32::BLACK,
		nullptr, &dlight_dir,  // need direction only
		0, 0, 0,               // no need for attenuation
		0, 0, 0);              // spotlight info NA

	// point light 点光源
	// Vector4D plight_pos = {0, 200, 0, 0}; // 点光源初始位置 此处可不设置 update里一直在更新的
	mLightList.initLight(POINT_LIGHT_INDEX,
		true,                  // turn the light on
		LIGHT_ATTR_POINT,      // pointlight type
		Color32::BLACK, 
		Color32::GREEN,        // color for diffuse term only
		Color32::BLACK,   
		nullptr, nullptr,      // need pos only 
		0, 0.001f, 0,          // linear attenuation only
		0, 0, 1);              // spotlight info NA

	// spot light2 聚光灯
	// Vector4D slight2_pos = {0, 200, 0, 0}; // 聚光灯初始位置 此处可不设置 update里一直在更新的
	Vector4D slight2_dir = {-1, 0, -1, 0}; // 聚光灯方向 这里仍然是反着表示的
	mLightList.initLight(SPOT2_LIGHT_INDEX,
		true,                     // turn the light on
		LIGHT_ATTR_SPOTLIGHT2,    // spot light type 2
		Color32::BLACK,
		Color32::RED,             // color for diffuse term only
		Color32::BLACK,
		nullptr, &slight2_dir,    // need direction only
		0, 0.001f, 0,             // linear attenuation only
		0, 0, 1);


	// 模型缩放向量
	Vector4D vscale = {20, 20, 20, 0};

	// load flat shaded cube
	mSphereConstant.loadFormCOB("model/sphere_constant_textured_02.cob", vscale, Point4D::ZERO, Vector4D::ZERO, VERTEX_FLAGS_INVERT_WINDING_ORDER | VERTEX_FLAGS_TRANSFORM_LOCAL | VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD);
	mSphereConstant.world_pos.x = -60;
	mSphereConstant.world_pos.y = 0;
	mSphereConstant.world_pos.z = 150;

	/*
	mSphereFlat.loadFormCOB("model/sphere_flat_textured_02.cob", vscale, Point4D::ZERO, Vector4D::ZERO, VERTEX_FLAGS_INVERT_WINDING_ORDER | VERTEX_FLAGS_TRANSFORM_LOCAL | VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD);
	mSphereFlat.world_pos.x = 0;
	mSphereFlat.world_pos.y = 0;
	mSphereFlat.world_pos.z = 150;

	mSphereGouraud.loadFormCOB("model/sphere_gouraud_textured_02.cob", vscale, Point4D::ZERO, Vector4D::ZERO, VERTEX_FLAGS_INVERT_WINDING_ORDER | VERTEX_FLAGS_TRANSFORM_LOCAL | VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD);
	mSphereGouraud.world_pos.x = 60;
	mSphereGouraud.world_pos.y = 0;
	mSphereGouraud.world_pos.z = 150;
	*/
	// 开关变量
	mSortZ = true;
	mBackface = true;
	mLighting = true;
	mWireFrame = false;


}

void TestCase_Texture::update(float dt){

	if (mEnginePtr&&this->mNext){
		this->mNext=true;
		// angles for light motion
		static float plight_ang = 0, slight_ang = PI2;

		// move point light source in ellipse around game world
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.x = 1000 * cosf(plight_ang);
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.y = 100;
		mLightList.lights[POINT_LIGHT_INDEX].pos_world.z = 1000 * cosf(plight_ang);

		plight_ang += PI_DIV_6 * dt; // 每秒30度
		if (plight_ang > PI2)
			plight_ang = 0;
		
		// move spot light source in ellipse around game world
		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.x = 300 * cosf(slight_ang);
		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.y = 0;
		mLightList.lights[SPOT2_LIGHT_INDEX].pos_world.z = 300 * sinf(slight_ang);

		slight_ang -= PI_DIV_4 * dt; // 每秒45度
		if (slight_ang < 0)
			slight_ang = PI2;

		// use these to rotate objects
		static float x_ang = 0, y_ang = 0, z_ang = 0;

		// 构造局部旋转矩阵
		Matrix_4X4 mat;
		mat.buildEulerRotation(x_ang, y_ang, z_ang);
		/*
		// update rotation angles
		x_ang += PI_DIV_12 * dt * 1;
		while (x_ang >= PI2)
			x_ang -= PI2;

		y_ang += PI_DIV_12 * dt * 2;
		while (y_ang >= PI2) 
			y_ang -= PI2;

		z_ang += PI_DIV_12 * dt * 3;
		while (z_ang >= PI2) 
			z_ang -= PI2;
		*/
		// reset the render list 清空渲染列表
		mRenderList.reset();

		// 
		mSphereConstant.resetFlag();
		mSphereConstant.transformByMat(mat, TRANSFORMMODE_LOCAL_TO_TRANS, true, false); // TODO: 注意顶点法线也跟着变了哦 所以只能做旋转 不可移动 另外单位向量旋转后仍然是单位向量 所以顶点法线不需要再做归一化		
		mSphereConstant.transformModelToWorld(TRANSFORMMODE_TRANS_ONLY, false);		
		mRenderList.insert(mSphereConstant);

		/*
		for (int i=0;i<mRenderList.num_triangles;i++){

			Triangle *poly= mRenderList.triangle_ptrs[i];

			if (poly->vert_list_local[0].u>0||poly->vert_list_local[0].v>0||
				poly->vert_list_local[1].u>0||poly->vert_list_local[1].v>0||
				poly->vert_list_local[2].u>0||poly->vert_list_local[2].v>0){
					CCLOG("asdasd");
			}

		}
		*/
		CCLOG("asdasd");
		// 
		mSphereFlat.resetFlag();		
		mSphereFlat.transformByMat(mat, TRANSFORMMODE_LOCAL_TO_TRANS, true, false);
		mSphereFlat.transformModelToWorld(TRANSFORMMODE_TRANS_ONLY, false);
		//mRenderList.insert(mSphereFlat);

		//
		mSphereGouraud.resetFlag();
		mSphereGouraud.transformByMat(mat, TRANSFORMMODE_LOCAL_TO_TRANS, true, false);
		mSphereGouraud.transformModelToWorld(TRANSFORMMODE_TRANS_ONLY, false);
		//mRenderList.insert(mSphereGouraud);

		// remove backfaces
		if (mBackface)
			mRenderList.removeBackface(mCamera);

		// light scene all at once 
		if (mLighting)
			mRenderList.LightInWorld(mCamera, mLightList);

		// apply world to camera transform
		mRenderList.transformWorldToCamera(mCamera);

		// sort the polygon list (hurry up!) Z排序在相机坐标下进行比较好 因为此时只需要比较Z坐标即可
		if (mSortZ)
			mRenderList.sortZ(SORT_POLY_METHOD_AVGZ); // Z坐标较大的三角形被认为"较小" 所以排在前面 所以先被绘制

		// apply camera to perspective transformation
		mRenderList.transformCameraToPerspective(mCamera);

		// apply screen transform
		mRenderList.transformPerspectiveToScreen(mCamera);

		// render the object
		mEnginePtr->beginDraw();

		if (mWireFrame)
			mEnginePtr->drawRenderListWire(mRenderList);
		else
			mEnginePtr->drawRenderListSolid(mRenderList);		

		mEnginePtr->endDraw();

	}

}

void TestCase_Texture::free(){

}

 