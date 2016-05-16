#ifndef _TESTERCASE_H_
#define _TESTERCASE_H_

#include "3dengine.h"
//#include "3drender.h"


#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#include <windows.h>  
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  
#endif  


class TestCase{
public:
	TestCase(): mEnginePtr(nullptr){}

	virtual void init(Engine* engine); 
	virtual void update(float dt);
	virtual void free();

	virtual void keyDown(int keyCode);
	virtual void keyUp(int keyCode);
	virtual void debug();


	//按键事件


protected:
	Engine *mEnginePtr;
	bool mNext;
};



class TestCase_2D:public TestCase{
public:
	virtual void init(Engine* engine) override;
	virtual void update(float dt) override;
};

class TestCase_Wire3D:public TestCase{
public:
	virtual void init(Engine* engine) override;
	virtual void update(float dt) override;
public:
	Matrix_4X4 mMatRotX;
	Matrix_4X4 mMatRotY;
	Matrix_4X4 mMatRotZ;

	Camera mCamera;


	Object4D*	mCube1;
	Object4D*	mCube2;
	RenderList4D mRenderList;

};

class TestCase_Camera:public TestCase_2D{
public:
	virtual void init(Engine* engine) override;
	virtual void update(float dt) override;
private:
	static const int NUM_OBJECTS		=16;
	static const int SPACING_OBJECT		=250;
private:
	Matrix_4X4		mMatRot;
	Camera			mCamera;
	RenderList4D	mRenderList;
	Object4D		mTank;
};

class TestCase_Light:public TestCase{
public:
	virtual void init(Engine* engine) override;
	virtual void update(float dt) override;

private:
	static const int AMBIENT_LIGHT_INDEX=0;
	static const int INFINITE_LIGHT_INDEX=1;//平行光
	static const int POINT_LIGHT_INDEX=2;//点光
	static const int SPOT2_LIGHT_INDEX=3;
	
	static const int UNIVERSE_RADIUS	=4000;
	static const int PLAYER_SPEED		=500;

	static const int NUM_TANKS			=32;
	static const int NUM_TOWERS			=64;

	static const int MARKER_SIZE		=800;
	static const int NUM_MARKERS_X		=2* UNIVERSE_RADIUS/MARKER_SIZE;
	static const int NUM_MARKERS_Z		=2* UNIVERSE_RADIUS/MARKER_SIZE;
	static const int NUM_MARKERS		=NUM_MARKERS_X*NUM_MARKERS_Z;

private:
	Camera				mCamera;
	LightList			mLightList;

	RenderList4D		mRenderList;



	Object4D			mPlayer;
	Object4D			mTank;
	Object4D			mTower;
	Object4D			mMarker;

	
	Point4D				mTankInfo[NUM_TANKS];
	Point4D				mTowerInfo[NUM_TOWERS];
	Point4D				mMarkerInfo[NUM_MARKERS];

	//坦克相对于相机的旋转角度
	float mTurning ;
	//点光源旋转角度
	float mPointLightAng;
	float mSpotLightAng;

	bool mKeyDownW;//上
	bool mKeyDownS;//下
	bool mKeyDownA;//左
	bool mKeyDownD;//右

	bool mWireFrame; // 线框模式
	bool mBackface;  // 背面剔除
	bool mLighting;  // 开启光照
	bool mSortZ;     // 形状排序(按Z值)
};


class TestCase_Gouraud:public TestCase{
public:
	virtual void init(Engine* engine) override;
	virtual void update(float dt) override;
	virtual void free() override;

	virtual void keyDown(int keyCode) override;
	virtual void debug() override;


private:
	//void LightBox(Engine*)

private:
	static const int AMBIENT_LIGHT_INDEX  = 0; // ambient light index
	static const int INFINITE_LIGHT_INDEX = 1; // infinite light index
	static const int POINT_LIGHT_INDEX    = 2; // point light index
	static const int SPOT2_LIGHT_INDEX    = 3; // spot2 light index

	//bool mNext;

private:
	Camera			mCamera;
	LightList		mLightList;
	RenderList4D	mRenderList;

	Object4D		mConstantWater;
	Object4D		mFlatWater;
	Object4D		mGouraudWater;

	Triangle		mTriangle2D;
	bool			mSortZ;
	bool			mBackface;
	bool			mLighting;
	bool			mWireFrame;


};

//////////////////////////////////////TestCase_Texture//////////////////////////////////////


class TestCase_Texture:public TestCase{
public:
	virtual void init(Engine* engine) override;
	virtual void update(float dt) override;
	virtual void free() override;
private:

	static const int AMBIENT_LIGHT_INDEX  = 0; // ambient light index
	static const int INFINITE_LIGHT_INDEX = 1; // infinite light index
	static const int POINT_LIGHT_INDEX    = 2; // point light index
	static const int SPOT2_LIGHT_INDEX    = 3; // spot2 light index

private:
	Camera mCamera;
	LightList mLightList;
	RenderList4D mRenderList;

	Object4D mSphereConstant;
	Object4D mSphereFlat;
	Object4D mSphereGouraud;

	bool mSortZ;     // 排序(按Z值)
	bool mBackface;  // 背面剔除
	bool mLighting;  // 开启光照
	bool mWireFrame; // 线框模式



};


 


#endif