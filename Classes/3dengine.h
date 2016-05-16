#ifndef _3D_ENGINE_H_
#define _3D_ENGINE_H_

//opengl的一些头文件
#include "cocos2d.h"
#include "3dmath.h"
#include "3dgeometry.h"
#include "3dbitmap.h"
#include "3dcamera.h"
USING_NS_CC;



class Engine{
public:
	Engine(int width,int height);
	~Engine();
public:
	CCSprite* getSprite();

	void beginDraw();
	void endDraw();

	void drawLine(int x1,int y1,int x2,int y2,const Color32 &color);
	void drawClipLine(int x1,int y1,int x2,int y2,const Color32 &color);

	void drawTopTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color32 &color);
	void drawBottomTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color32 &color);
	void drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3, const Color32 &color);
	
	void drawTopGouraudTriangle(Triangle& triangle);
	void drawBottomGouraudTriangle(Triangle& triangle);
	void drawGouraudTriangle(Triangle& triangle);

	void drawFlatTriangle(Triangle& triangle);

	void drawTopConstantTextureTriangle( Triangle& triangle);
	void drawBottomConstantTextureTriangle( Triangle& triangle);
	void drawConstantTextureTriangle( Triangle& triangle);

	void drawFlatTextureTriangle(const Triangle& triangle);
	void drawGouraudTextureTriangle(const Triangle& triangle);



	void drawQuad2D(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const Color32 &color);
	void drawBitmap(int x, int y, struct BitmapImage &bmp);


	void drawRenderListWire(const RenderList4D& rend_list);
	void drawRenderListSolid(const RenderList4D& render_list);
	void drawObjectWire(Object4D& object);
	



private:
	bool clipLine(int &x1, int &y1, int &x2, int &y2);

private:
	int mWidth;
	int mHeight;
	Color32* mPixelBuffer; // Cocos纹理(mSprite)像素数据
	int mPixelNum;
 	static const int mLogBase2_of_x[513];

	CCSprite* mSprite;

};


#endif