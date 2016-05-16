#include "3dengine.h"




//////////////////////////////////////////////////////////////////


const int Engine::mLogBase2_of_x[513] =
{
	0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
}; 


Engine::Engine(int width,int height){
	mWidth=width;
	mHeight=height;
	
	mPixelNum=width*height;
	mPixelBuffer=new Color32[mPixelNum];

	CCTexture2D* texture=new CCTexture2D();
	texture->initWithData(mPixelBuffer,kCCTexture2DPixelFormat_RGBA8888,mWidth,mHeight,CCSize(mWidth,mHeight));
	texture->setAliasTexParameters();

	mSprite=CCSprite::createWithTexture(texture);
	if (mSprite){
		mSprite->retain();
	}	
}

Engine::~Engine(){
	delete mPixelBuffer;
	if (mSprite){
		mSprite->release();
	}
}

CCSprite* Engine::getSprite(){
	return mSprite;
}

inline void mymemset(void *dest,int32_t data,int count){
	_asm{
		mov edi,dest;
		mov eax,data;
		mov ecx,count;
		rep stosd
	}

}

//这个函数很慢啊,必须优化
void Engine::beginDraw(){
	mymemset(mPixelBuffer,(int)Color32::GRAY.rgba,mPixelNum);
	//for (int i=0;i<mPixelNum;i++){
	//	mPixelBuffer[i]=Color32::GRAY;
	//}
}

void Engine::endDraw(){
	//将像素数据关联到相应的texture
	if (mSprite){
	
		CCTexture2D* texture=mSprite->getTexture();
		texture->release();
		texture=new CCTexture2D();
		texture->initWithData(mPixelBuffer,kCCTexture2DPixelFormat_RGBA8888,mWidth,mHeight,CCSize(mWidth,mHeight));
		 //mSprite->setTexture()
		//mSprite->getTexture()->release();
		mSprite->setTexture(texture);
		 
	}
}
 
void Engine::drawLine(int x1,int y1,int x2,int y2,const Color32 &color){
	int dx,
		dy,
		dx2,
		dy2,
		x_inc,
		y_inc,
		error;

	//线条起始地址
	Color32 *pixelAddr=mPixelBuffer+y1*mWidth+x1;

	dx=x2-x1;
	dy=y2-y1;

	if (dx>=0){
		x_inc=1;
	}else{

		x_inc=-1;
		dx=-dx;
	}

	if (dy>=0){

		y_inc=mWidth;
	}else{

		y_inc=-mWidth;
		dy=-dy;
	}

	dx2=dx<<1;//dx*2
	dy2=dy<<1;//dy*2

	if (dx>dy){
		error=dy2-dx;
		for (int index = 0; index <= dx; index++)
		{
			*pixelAddr=color;
			if (error>=0){
				error-=dx2;
				pixelAddr+=y_inc;
			}
			error+=dy2;
			pixelAddr+=x_inc;
		}
	}else{
		error = dx2 - dy; 
		for (int index = 0; index <= dy; index++)
		{
			*pixelAddr=color;
			if (error >= 0){
				error-=dy2;
				pixelAddr+=x_inc;
			}
			error+=dx2;
			pixelAddr+=y_inc;
		}
	}
}


void Engine::drawClipLine(int x1,int y1,int x2,int y2,const Color32 &color){
	if (clipLine(x1,y1,x2,y2)){
		drawLine(x1,y1,x2,y2,color);

	}
}


bool Engine::clipLine(int &x1, int &y1, int &x2, int &y2){
	// this function clips the sent line using the globally defined clipping region

	// internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008 // 上(北)
#define CLIP_CODE_S  0x0004 // 下(南)
#define CLIP_CODE_E  0x0002 // 右(东)
#define CLIP_CODE_W  0x0001 // 左(西)

#define CLIP_CODE_NE 0x000A // 右上
#define CLIP_CODE_SE 0x0006 // 右下
#define CLIP_CODE_NW 0x0009 // 左上
#define CLIP_CODE_SW 0x0005 // 左下

	// 上下左右大小限制
	int min_clip_y = 0,
		max_clip_y = mHeight - 1,
		min_clip_x = 0,
		max_clip_x = mWidth - 1;

	int xc1 = x1, 
		yc1 = y1, 
		xc2 = x2, 
		yc2 = y2;

	int p1_code = 0, 
		p2_code = 0;

	// determine codes for p1 and p2
	if (y1 < min_clip_y)
		p1_code |= CLIP_CODE_N;
	else
		if (y1 > max_clip_y)
			p1_code |= CLIP_CODE_S;

	if (x1 < min_clip_x)
		p1_code |= CLIP_CODE_W;
	else
		if (x1 > max_clip_x)
			p1_code |= CLIP_CODE_E;

	if (y2 < min_clip_y)
		p2_code |= CLIP_CODE_N;
	else
		if (y2 > max_clip_y)
			p2_code |= CLIP_CODE_S;

	if (x2 < min_clip_x)
		p2_code |= CLIP_CODE_W;
	else
		if (x2 > max_clip_x)
			p2_code |= CLIP_CODE_E;

	// try and trivially reject
	if (p1_code & p2_code)
		return false;

	// test for totally visible, if so leave points untouched
	if (p1_code == 0 && p2_code == 0)
		return true;

	// determine end clip point for p1
	switch (p1_code)
	{
	case CLIP_CODE_C:
		break;

	case CLIP_CODE_N:
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1); // x = x1 + (y - y1) * 1/k, k为斜率, 下同
		break;

	case CLIP_CODE_S:
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);
		break;

	case CLIP_CODE_W:
		xc1 = min_clip_x;
		yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1); // y = y + (x - x1) * k, k为斜率, 下同
		break;

	case CLIP_CODE_E:
		xc1 = max_clip_x;
		yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
		break;

		// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
		// north hline intersection (与上面的横线的交点,下略)
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next (在横向区域外)
		if (xc1 < min_clip_x || xc1 > max_clip_x) // TODO: 有可能 xc1 < min_clip_x ?? 下同,略
		{
			// east vline intersection (与右边的竖线的焦点,下略)
			xc1 = max_clip_x;
			yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
		} // end if

		break;

	case CLIP_CODE_SE:
		// south hline intersection
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x)
		{
			// east vline intersection
			xc1 = max_clip_x;
			yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
		} // end if

		break;

	case CLIP_CODE_NW:
		// north hline intersection
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x)
		{
			xc1 = min_clip_x;
			yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);	
		} // end if

		break;

	case CLIP_CODE_SW:		
		// south hline intersection
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);	

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x)
		{
			xc1 = min_clip_x;
			yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
		} // end if

		break;

	default:
		break;
	} // end switch

	// determine clip point for p2
	switch (p2_code)
	{
	case CLIP_CODE_C: 
		break;

	case CLIP_CODE_N:		   
		yc2 = min_clip_y;
		xc2 = x2 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);
		break;

	case CLIP_CODE_S:		   
		yc2 = max_clip_y;
		xc2 = x2 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);
		break;

	case CLIP_CODE_W:		   
		xc2 = min_clip_x;
		yc2 = y2 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
		break;

	case CLIP_CODE_E:		   
		xc2 = max_clip_x;
		yc2 = y2 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
		break;

		// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:		   
		// north hline intersection
		yc2 = min_clip_y;
		xc2 = x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			// east vline intersection
			xc2 = max_clip_x;
			yc2 = y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
		} // end if

		break;

	case CLIP_CODE_SE:      	   
		// south hline intersection
		yc2 = max_clip_y;
		xc2 = x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);	

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			// east vline intersection
			xc2 = max_clip_x;
			yc2 = y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
		} // end if

		break;

	case CLIP_CODE_NW:      	   
		// north hline intersection
		yc2 = min_clip_y;
		xc2 = x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			xc2 = min_clip_x;
			yc2 = y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);	
		} // end if

		break;

	case CLIP_CODE_SW:		   
		// south hline intersection
		yc2 = max_clip_y;
		xc2 = x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);	

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			xc2 = min_clip_x;
			yc2 = y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);	
		} // end if

		break;

	default:
		break;
	} // end switch

	// do bounds check 整条线在剪切区域外,两端点不处于同一区块(上下左右)
	if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
		(yc1 < min_clip_y) || (yc1 > max_clip_y) ||
		(xc2 < min_clip_x) || (xc2 > max_clip_x) ||
		(yc2 < min_clip_y) || (yc2 > max_clip_y) )
	{
		return false;
	} // end if

	// store vars back
	x1 = xc1;
	y1 = yc1;
	x2 = xc2;
	y2 = yc2;

	return true;
}


//平顶三角形
void Engine::drawTopTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color32 &color){

	float dx_right;
	float dx_left;
	float xs,xe;
	float height;

 

	int min_clip_y=0,
		max_clip_y=mHeight-1,
		min_clip_x=0,
		max_clip_x=mWidth-1;

	//调整次序
	if (x2<x1){
		//保证y1 y2相等 不做调整
		int temp_x=x2;
		x2=x1;
		x1=temp_x;
	}
	

	height=y3-y1;

	dx_left=(x3-x1)/height;
	dx_right=(x3-x2)/height;

	xs=x1;
	xe=x2+0.5;

	if (y1<min_clip_y){
		//跳过多出的y部分
		xs=xs+dx_left*(-y1);
		xe=xe+dx_right*(-y1);
		y1=y2=min_clip_y;
		//xs=xs+dx_left*(-y1+min_clip_y);
	}

	if (y3>max_clip_y)
	{//剔除下面多余的部分
		y3=max_clip_y;
	}


	//扫描线所在列开头位置
	Color32* pixelAddr=mPixelBuffer+y1*mWidth;

 
	for (int y=y1;y<y3;y++,pixelAddr+=mWidth){
		for (int x=xs;x<xe;x++){
			pixelAddr[x]=color;
		}
		//重新设置下一行xs ,xe的位置
		xs+=dx_left;
		xe+=dx_right;
	}
}

void Engine::drawBottomTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color32 &color){

	int min_clip_y=0,
		max_clip_y=mHeight-1,
		min_clip_x=0,
		max_clip_x=mWidth-1;

	float	dx_left,
			dx_right,
			xs,xe,
			height;

	if (x3<x2){
		int tempx=x3;
		x3=x2;
		x2=tempx;
	}

	height=y3-y1;// 确保>0

	dx_left=(x2-x1)/height;	//向左步长
	dx_right=(x3-x1)/height;//向右步长
	xs=xe=x1;

	if (y1<0){
		xs=xs+(-y1)*dx_left;
		xe=xe+(-y1)*dx_right;
		y1=min_clip_y;
		
	}

	if (y3>max_clip_y){
		y3=max_clip_y;
	}

	Color32* pixelAddr=mPixelBuffer+y1*mWidth;

	for (int y=y1;y<y3;y++,pixelAddr+=mWidth){
		for (int x=xs;x<xe;x++){
			pixelAddr[x]=color;
		}
		xs+=dx_left;
		xe+=dx_right;
	}
}


void Engine::drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3, const Color32 &color){


	if ((floatEqual(x1,x2)&&floatEqual(x2,x3))||(floatEqual(y1,y2)&&floatEqual(y2,y3))){
		//跳过线条
		return;
	}
	int temp=0;
	//根据y从高到低 排次序
	label:
	if (y1>y2){
		swap(y1,y2);
		swap(x1,x2);
		//y1<y2
	}

	if (y2>y3){
		swap(y2,y3);
		swap(x2,x3);
		goto label;
	}
	if (y1>y3){
		swap(y1,y3);
		swap(x1,x3);
		goto label;
	}
 
	//排序结束

	int min_clip_y = 0;
	int max_clip_y = mHeight - 1;
	int min_clip_x = 0;
	int max_clip_x = mWidth - 1;

	if ( y3 < min_clip_y || y1 > max_clip_y ||
		(x1 < min_clip_x && x2 < min_clip_x && x3 < min_clip_x) ||
		(x1 > max_clip_x && x2 > max_clip_x && x3 > max_clip_x))
	{
		return;
	}


	if (y1==y2){
		//平顶三角形
		drawTopTriangle(x1,y1,x2,y2,x3,y3,color);
	}else if (y2==y3){
		drawBottomTriangle(x1,y1,x2,y2,x3,y3,color);
	} else if(y1==y3){


	}else{
		//分割三角形为上下两半
		//float k=((float)(y3-y1))/(x3-x1);
		//int x22=(y2-y1)/k+x1;
		float x22 = x1 + (y2 - y1) * (x3 - x1) / (y3 - y1);
		drawBottomTriangle(x1,y1,x2,y2,x22,y2,color);
		drawTopTriangle(x2,y2,x22,y2,x3,y3,color);

		//int new_x = x1 + (0.5f + (y2 - y1) * (x3 - x1) / static_cast<float>(y3 - y1));
		//drawBottomTriangle(x1, y1, new_x, y2, x2, y2, color);
		//drawTopTriangle(x2, y2, new_x, y2, x3, y3, color);

	}
}


void Engine::drawQuad2D(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const Color32 &color){
	drawTriangle2D(x1,y1,x2,y2,x3,y3,color);
	drawTriangle2D(x1,y1,x3,y3,x4,y4,color);
}



void Engine::drawBitmap(int x, int y, struct BitmapImage &bmp){
	if (bmp.width <= 0 || bmp.height <= 0 || bmp.bpp != 24 || bmp.data == nullptr || x >= mWidth || y >= mHeight){
		return;
	}

	using namespace std;

	int y1=max(y,0);
	int x1=max(x,0);

	int x2=min(x+bmp.width-1,mWidth-1);
	int y2=min(y+bmp.height-1,mHeight-1);

	Color32* pixelAddr=mPixelBuffer+y1*mWidth+x1;

	BitmapPixel_24* texture_data=(BitmapPixel_24*)bmp.data;

	if (y<0){
		texture_data+=(-y)*bmp.width;
	}
	if (x<0){
		texture_data+=-x;
	}

	for (int yi=y1;yi<y2;yi++){
		for (int xi=x1,i=0;xi<=x2;xi++,i++){
			pixelAddr[i].r=texture_data[i].r;
			pixelAddr[i].g=texture_data[i].g;
			pixelAddr[i].b=texture_data[i].b;
			pixelAddr[i].a=255;//不透明
		}
		pixelAddr+=mWidth;
		texture_data+=bmp.width;
	}


}


void Engine::drawRenderListWire(const RenderList4D& rend_list){

	for (int poly=0;poly<rend_list.num_triangles;poly++){
		Triangle* curr_poly=rend_list.triangle_ptrs[poly];
		if (TRIANGLE_UNACTIVE_STATE(curr_poly)){
			continue;
		}

		drawClipLine(curr_poly->vert_list_trans[0].x,curr_poly->vert_list_trans[0].y,
					curr_poly->vert_list_trans[1].x,curr_poly->vert_list_trans[1].y,
					curr_poly->color);

		drawClipLine(curr_poly->vert_list_trans[1].x,curr_poly->vert_list_trans[1].y,
			curr_poly->vert_list_trans[2].x,curr_poly->vert_list_trans[2].y,
			curr_poly->color);

		drawClipLine(curr_poly->vert_list_trans[2].x,curr_poly->vert_list_trans[2].y,
			curr_poly->vert_list_trans[0].x,curr_poly->vert_list_trans[0].y,
			curr_poly->color);
	}
}

void Engine::drawObjectWire(Object4D& object){

	for (int index=0;index<object.num_triangles;index++){
		int state=object.triangle_ref_list[index].state;
		if ((!state&TRIANGLE_STATE_ACTIVE)||
			(state&(TRIANGLE_STATE_CLIPPED|TRIANGLE_STATE_BACKFACE))){
				continue;
		}
		
		int idx0=object.triangle_ref_list[index].vert_idx[0];
		int idx1=object.triangle_ref_list[index].vert_idx[1];
		int idx2=object.triangle_ref_list[index].vert_idx[2];

		TriangleRef triangle=object.triangle_ref_list[index];
		//int vindex_0=object.triangle_list.vert_idx[0];
		//int vindex_1=object
		triangle.color=Color32::YELLOW;
		drawClipLine(object.vert_list_trans[idx0].x,object.vert_list_trans[idx0].y,
					object.vert_list_trans[idx1].x,object.vert_list_trans[idx1].y,
					triangle.color);

		drawClipLine(object.vert_list_trans[idx1].x,object.vert_list_trans[idx1].y,
			object.vert_list_trans[idx2].x,object.vert_list_trans[idx2].y,
			triangle.color);

		drawClipLine(object.vert_list_trans[idx2].x,object.vert_list_trans[idx2].y,
			object.vert_list_trans[idx0].x,object.vert_list_trans[idx0].y,
			triangle.color);
	}

}


void Engine::drawRenderListSolid(const RenderList4D& render_list){

	
	for (int index=0;index<render_list.num_triangles;index++){
		Triangle* curr_triangle=render_list.triangle_ptrs[index];

		if (TRIANGLE_UNACTIVE_STATE(curr_triangle)){
			continue;
		}
		/*
		if (curr_triangle->vert_list_local[0].u>0||curr_triangle->vert_list_local[0].v>0||
			curr_triangle->vert_list_trans[1].u>0||curr_triangle->vert_list_trans[1].v>0||
			curr_triangle->vert_list_trans[2].u>0||curr_triangle->vert_list_trans[2].v>0){
				CCLOG("adfas");
		}
		*/
		if (curr_triangle->attr&TRIANGLE_ATTR_SHADE_MODE_TEXTURE)
		{
			if (curr_triangle->attr&&TRIANGLE_ATTR_SHADE_MODE_CONSTANT)
			{
				//drawFlatTriangle(*curr_triangle);
				drawConstantTextureTriangle(*curr_triangle);
			}
			else if (curr_triangle->attr&&TRIANGLE_ATTR_SHADE_MODE_FLAT)
			{
			
			}
			else if (curr_triangle->attr&&TRIANGLE_ATTR_SHADE_MODE_GOURAUD)
			{

			}

			CCLOG("asdf");

		}
		else{
				if (curr_triangle->attr&TRIANGLE_ATTR_SHADE_MODE_CONSTANT )
				{
					drawTriangle2D(curr_triangle->vert_list_trans[0].x,curr_triangle->vert_list_trans[0].y,
						curr_triangle->vert_list_trans[1].x,curr_triangle->vert_list_trans[1].y,
						curr_triangle->vert_list_trans[2].x,curr_triangle->vert_list_trans[2].y,
						curr_triangle->color);
				}else if (curr_triangle->attr& TRIANGLE_ATTR_SHADE_MODE_FLAT)
				{
						drawFlatTriangle(*curr_triangle);
				}else if(curr_triangle->attr&TRIANGLE_ATTR_SHADE_MODE_GOURAUD) 
				{

					if (curr_triangle->attr&TRIANGLE_ATTR_TRANSPARENT){
				
					}else{
					drawGouraudTriangle(*curr_triangle);
							/*
						Color32 *litcolor=curr_triangle->lit_color;

						CCLOG("color[0] %i %i %i %i",litcolor[0].r,litcolor[0].g,litcolor[0].b,litcolor[0].a);
						CCLOG("color[1] %i %i %i %i",litcolor[1].r,litcolor[1].g,litcolor[1].b,litcolor[1].a);
						CCLOG("color[2] %i %i %i %i",litcolor[2].r,litcolor[2].g,litcolor[2].b,litcolor[2].a);
						*/
					}

				}
		 }
	}
}

 
void Engine::drawTopGouraudTriangle(Triangle& triangle){
	//int v0=0,v1=1,v2=2;
	Vertex4D v0= triangle.vert_list_trans[0];
	Color32  color0=triangle.lit_color[0];

	Vertex4D v1= triangle.vert_list_trans[1];
	Color32 color1=triangle.lit_color[1];

	Vertex4D v2= triangle.vert_list_trans[2];
	Color32 color2=triangle.lit_color[2];


	if ((floatEqual(v0.x,v1.x)&& floatEqual(v1.x,v2.x))||(floatEqual(v0.y,v1.y)&&floatEqual(v1.y,v2.y))){
		//直线不画
		return ;
	}

	if ( (!floatEqual(v0.y,v1.y))&& (!floatEqual(v0.y,v2.y)) &&(!floatEqual(v1.y,v2.y))){
		//没有两个点再同一水平上
		return;
	}

	Vertex4D tempVertex;
	Color32 tempColor;
	if (v0.y>v1.y && v0.y>v2.y){
		tempVertex=v0;
		v0=v2;
		v2=tempVertex;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
	}else if (v1.y>v0.y && v1.y>v2.y){
		tempVertex=v1;
		v1=v2;
		v2=tempVertex;

		tempColor=color1;
		color1=color2;
		color2=tempColor;
	}

	if (v0.x>v1.x){
		tempVertex=v0;
		v0=v1;
		v1=tempVertex;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}

 
	float dx_left=0.0f,dx_right=0.0f,dy=0.0f;
	int xs=0,xe=0;
	float k_left=0.0f,k_right=0.0f;

	if (floatEqual(v0.y,v1.y)){
		//平顶
		dx_left=v2.x-v0.x;
		dx_right=v2.x-v1.x;

		if (floatEqual(dx_left,0.0f)||floatEqual(dx_right,0.0f)){
			return;
		}

		dy=v2.y-v0.y;

		//斜率
		dx_left=dx_left/dy;
		dx_right=dx_right/dy;


		Color32* pixelAddr=(Color32*)mPixelBuffer;
		pixelAddr=pixelAddr+mWidth*(int)(v0.y+0.5);

		float v0v2_dr=(color2.r-color0.r)/dy;
		float v0v2_dg=(color2.g-color0.g)/dy;
		float v0v2_db=(color2.b-color0.b)/dy;

		float v1v2_dr=(color2.r-color1.r)/dy;
		float v1v2_dg=(color2.g-color1.g)/dy;
		float v1v2_db=(color2.b-color1.b)/dy;


		for (int y=v0.y;y<v2.y;y++){

			xs=v0.x+(y-v0.y)*dx_left;//x轴起始点
			xe=v1.x+(y-v0.y)*dx_right;//x轴终点
			int dy=y-v0.y;

			float rs=color0.r+dy*v0v2_dr;
			float re=color1.r+dy*v1v2_dr;

			float gs=color0.g+dy*v0v2_dg;
			float ge=color1.g+dy*v1v2_dg;

			float bs=color0.b+dy*v0v2_db;
			float be=color1.b+dy*v1v2_db;

			float pitch=float(xe-xs);


			float dr=(re-rs)/pitch;
			float dg=(ge-gs)/pitch;
			float db=(be-bs)/pitch;

			int step=0;
			for (int x=xs;x<xe;x++){
				GLubyte r=rs+dr*step;
				GLubyte g=gs+dg*step;
				GLubyte b=bs+db*step;
				*(pixelAddr+x)=Color32(r,g,b,255);
				step++;
			}

			pixelAddr=pixelAddr+mWidth;

		}
	}
}

void Engine::drawBottomGouraudTriangle(Triangle& triangle){
	
	//int v0=0,v1=1,v2=2;
	Vertex4D v0= triangle.vert_list_trans[0];
	Color32  color0=triangle.lit_color[0];

	Vertex4D v1= triangle.vert_list_trans[1];
	Color32 color1=triangle.lit_color[1];

	Vertex4D v2= triangle.vert_list_trans[2];
	Color32 color2=triangle.lit_color[2];


	if ((floatEqual(v0.x,v1.x)&& floatEqual(v1.x,v2.x))||(floatEqual(v0.y,v1.y)&&floatEqual(v1.y,v2.y))){
		//直线不画
		return ;
	}
	if ( (!floatEqual(v0.y,v1.y))&& (!floatEqual(v0.y,v2.y)) &&(!floatEqual(v1.y,v2.y))){
		//没有两个点再同一水平上
		return;
	}
	 
	Vertex4D tempVertex;
	Color32 tempColor;
	if (v1.y<v0.y && v1.y<v2.y){
		//顶点v1.y最小
		tempVertex=v0;
		v0=v1;
		v1=tempVertex;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}else if (v2.y<v0.y&& v2.y<v1.y){
		tempVertex=v0;
		v0=v2;
		v2=tempVertex;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
	}

	if (v1.x>v2.x){
		tempVertex=v1;
		v1=v2;
		v2=tempVertex;

		tempColor=color1;
		color1=color2;
		color2=tempColor;
	}

	

	float dx_left=0.0f,dx_right=0.0f,dy=0.0f;
	int xs=0,xe=0;
	float k_left=0.0f,k_right=0.0f;

	if (floatEqual(v1.y,v2.y)){
		//平底
		float dx_left=	v1.x-v0.x;
		float dx_right=v2.x-v0.x;

		float dy=v1.y-v0.y;

		int bottom=v1.y;
		int step=0;

		float v1v0_dr=(color1.r-color0.r)/dy;
		float v1v0_dg=(color1.g-color0.g)/dy;
		float v1v0_db=(color1.b-color0.b)/dy;

		float v2v0_dr=(color2.r-color0.r)/dy;
		float v2v0_dg=(color2.g-color0.g)/dy;
		float v2v0_db=(color2.b-color0.b)/dy;

		float dxlstep=dx_left/dy;
		float dxrstep=dx_right/dy;
		Color32* pixelAddr=(Color32*)mPixelBuffer;
		pixelAddr=pixelAddr+(mWidth*int(v0.y+0.5));

		for (int y=v0.y;y<bottom;y++){
			step++;
			int xs=v0.x+dxlstep*(y-v0.y);
			int xe=v0.x+dxrstep*(y-v0.y);

			float rs=color0.r+v1v0_dr*step;
			float gs=color0.g+v1v0_dg*step;
			float bs=color0.b+v1v0_db*step;


			float re=color0.r+v2v0_dr*step;
			float ge=color0.g+v2v0_dg*step;
			float be=color0.b+v2v0_db*step;

			float dr=(re-rs)/(xe-xs);
			float dg=(ge-gs)/(xe-xs);
			float db=(be-bs)/(xe-xs);
			//z++;
			for (int x=xs;x<xe;x++){

				GLubyte r=rs+(x-xs)*dr;
				GLubyte g=gs+(x-xs)*dg;
				GLubyte b=bs+(x-xs)*db;

				*(pixelAddr+x)=Color32(r,g,b,255);

			}
			pixelAddr=pixelAddr+mWidth;
		}

	}

}

void Engine::drawGouraudTriangle(Triangle& triangle){
	//int v0=0,v1=1,v2=2;
	Vertex4D v0= triangle.vert_list_trans[0];
	Color32  color0=triangle.lit_color[0];

	Vertex4D v1= triangle.vert_list_trans[1];
	Color32 color1=triangle.lit_color[1];

	Vertex4D v2= triangle.vert_list_trans[2];
	Color32 color2=triangle.lit_color[2];
 

	if ((floatEqual(v0.x,v1.x)&& floatEqual(v1.x,v2.x))||(floatEqual(v0.y,v1.y)&&floatEqual(v1.y,v2.y))){
		//直线不画
		return ;
	}

	//照y排序
	Vertex4D tempV;
	Color32 tempColor;
Label:
	if (v0.y>v1.y){
		tempV=v0;
		v0=v1;
		v1=tempV;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}

	if (v1.y>v2.y){
		tempV=v1;
		v1=v2;
		v2=tempV;

		tempColor=color1;
		color1=color2;
		color2=color1;
		goto Label;
	}

	if (v0.y>v2.y){
		tempV=v0;
		v0=v2;
		v2=tempV;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
		goto Label;
	}
	
	//照x排序
Label2:
	 
	 if (floatEqual(v0.y,v1.y)&& v0.x>v1.x ){
		 tempV=v0;
		 v0=v1;
		 v1=tempV;

		 tempColor=color0;
		 color0=color1;
		 color1=tempColor;
	 }

	 if (floatEqual(v1.y,v2.y)&& v1.x>v2.x){
		
		 tempV=v1;
		 v1=v2;
		 v2=tempV;

		 tempColor=color1;
		 color1=color2;
		 color2=color1;
		goto Label2;
	 }

	 if (floatEqual(v0.y,v2.y)&&v0.x>v2.x){
		 tempV=v0;
		 v0=v2;
		 v2=tempV;

		 tempColor=color0;
		 color0=color2;
		 color2=tempColor;
		 goto Label2;
	 }
	 //上面的这段排序代码要重写


	 float dx_left=0.0f,dx_right=0.0f,dy=0.0f;
	 int xs=0,xe=0;
	 float k_left=0.0f,k_right=0.0f;

	 if (floatEqual(v0.y,v1.y)){
		 drawTopGouraudTriangle(triangle);
		  
	 }else if (floatEqual(v1.y,v2.y)){
		 
		 drawBottomGouraudTriangle(triangle);
	 }else {
		 Vertex4D v;
		 //v.x=triangle.vert_list_trans[1].x;
		 
		 float k=(v2.y-v0.y)/(v2.x-v0.x);
		 float xt=((v1.y-v0.y)/k)+v0.x;
		 float yt=v1.y;


		 int r=color0.r*( (v1.y-v0.y)/(v2.y-v0.y) );
		 int g=color0.g*( (v1.y-v0.y)/(v2.y-v0.y) );
		 int b=color0.b*( (v1.y-v0.y)/(v2.y-v0.y) );


		 Triangle t1;
		 t1.attr=triangle.attr;
		 t1.state=triangle.state;
		 t1.color=triangle.color;
		 
		 t1.vert_list_trans[0]=v0; //triangle.vert_list_trans[0];
		 t1.lit_color[0]=color0; 
		 
		 t1.vert_list_trans[1]=v1; //triangle.vert_list_trans[1];
		 t1.lit_color[1]=color1;

		 t1.vert_list_trans[2].x=xt;
		 t1.vert_list_trans[2].y=yt;
		 t1.lit_color[2]=Color32(r,g,b,255);
		 
		 if ( t1.vert_list_trans[0].x<300 ||t1.vert_list_trans[1].x<300 ||t1.vert_list_trans[2].x<300){
			 CCLOG("ASDF");
		 }
		 drawBottomGouraudTriangle(t1);

		 
		 
		 t1.vert_list_trans[0]=v1; //t1.vert_list_trans[1];
		 t1.lit_color[0]=color1; //t1.lit_color[1];

		 t1.vert_list_trans[1].x =xt;//t1.vert_list_trans[2];
		 t1.vert_list_trans[1].y=yt;
		 t1.lit_color[1]=Color32(r,g,b,255);//color2; //t1.lit_color[2];

		 t1.vert_list_trans[2]=v2; //triangle.vert_list_trans[2];
		 t1.lit_color[2]=color2; //triangle.lit_color[2];
		 drawTopGouraudTriangle(t1);
		
		 
		 if ( t1.vert_list_trans[0].x<300 ||t1.vert_list_trans[1].x<300 ||t1.vert_list_trans[2].x<300){
			 CCLOG("ASDF");
		 }
		 
	 }
}


void Engine::drawFlatTriangle(Triangle& triangle){

	Vertex4D v0= triangle.vert_list_trans[0];
	Color32  color0=triangle.lit_color[0];

	Vertex4D v1= triangle.vert_list_trans[1];
	Color32 color1=triangle.lit_color[1];

	Vertex4D v2= triangle.vert_list_trans[2];
	Color32 color2=triangle.lit_color[2];


	if ((floatEqual(v0.x,v1.x)&& floatEqual(v1.x,v2.x))||(floatEqual(v0.y,v1.y)&&floatEqual(v1.y,v2.y))){
		//直线不画
		return ;
	}

	//照y排序
	Vertex4D tempV;
	Color32 tempColor;
Label:
	if (v0.y>v1.y){
		tempV=v0;
		v0=v1;
		v1=tempV;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}

	if (v1.y>v2.y){
		tempV=v1;
		v1=v2;
		v2=tempV;

		tempColor=color1;
		color1=color2;
		color2=color1;
		goto Label;
	}

	if (v0.y>v2.y){
		tempV=v0;
		v0=v2;
		v2=tempV;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
		goto Label;
	}

	//照x排序
Label2:

	if (floatEqual(v0.y,v1.y)&& v0.x>v1.x ){
		tempV=v0;
		v0=v1;
		v1=tempV;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}

	if (floatEqual(v1.y,v2.y)&& v1.x>v2.x){

		tempV=v1;
		v1=v2;
		v2=tempV;

		tempColor=color1;
		color1=color2;
		color2=color1;
		goto Label2;
	}

	if (floatEqual(v0.y,v2.y)&&v0.x>v2.x){
		tempV=v0;
		v0=v2;
		v2=tempV;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
		goto Label2;
	}

	drawTriangle2D(v0.x,v0.y,
					v1.x,v1.y,
					v2.x,v2.y,
					triangle.lit_color[0]);


}





void Engine::drawTopConstantTextureTriangle( Triangle& triangle){
	//int v0=0,v1=1,v2=2;
	Vertex4D v0= triangle.vert_list_trans[0];
	Color32  color0=triangle.lit_color[0];

	Vertex4D v1= triangle.vert_list_trans[1];
	Color32 color1=triangle.lit_color[1];

	Vertex4D v2= triangle.vert_list_trans[2];
	Color32 color2=triangle.lit_color[2];


	if ((floatEqual(v0.x,v1.x)&& floatEqual(v1.x,v2.x))||(floatEqual(v0.y,v1.y)&&floatEqual(v1.y,v2.y))){
		//直线不画
		return ;
	}

	if ( (!floatEqual(v0.y,v1.y))&& (!floatEqual(v0.y,v2.y)) &&(!floatEqual(v1.y,v2.y))){
		//没有两个点再同一水平上
		return;
	}

	Vertex4D tempVertex;
	Color32 tempColor;
	if (v0.y>v1.y && v0.y>v2.y){
		tempVertex=v0;
		v0=v2;
		v2=tempVertex;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
	}else if (v1.y>v0.y && v1.y>v2.y){
		tempVertex=v1;
		v1=v2;
		v2=tempVertex;

		tempColor=color1;
		color1=color2;
		color2=tempColor;
	}

	if (v0.x>v1.x){
		tempVertex=v0;
		v0=v1;
		v1=tempVertex;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}


	float dx_left=0.0f,dx_right=0.0f,dy=0.0f;
	int xs=0,xe=0;
	float k_left=0.0f,k_right=0.0f;

	if (floatEqual(v0.y,v1.y)){
		//平顶
		dx_left=v2.x-v0.x;
		dx_right=v2.x-v1.x;

		if (floatEqual(dx_left,0.0f)||floatEqual(dx_right,0.0f)){
			return;
		}

		dy=v2.y-v0.y;

		//斜率
		dx_left=dx_left/dy;
		dx_right=dx_right/dy;


		Color32* pixelAddr=(Color32*)mPixelBuffer;
		pixelAddr=pixelAddr+mWidth*(int)(v0.y+0.5);

		BitmapImage* texture=triangle.texture;		

		BitmapPixel_24 *texture_data = (BitmapPixel_24*)(triangle.texture->data); 

		float dxdyl=(v1.x-v0.x)/(v2.y-v0.y);
		float dudyl=(v1.u-v0.u)/(v2.y-v0.y);
		float dvdyl=(v1.v-v0.v)/(v2.y-v0.y);
		float dudx =(v1.u-v0.u)/(v1.x-v0.x);
				

		int idxystart=(v0.y+0.5);
		int idyend=(v2.y+0.5);
		int stepy=0;
		for (int y=idxystart;y<idyend;y++){

			xs=v0.x+(y-idxystart)*dx_left;//x轴起始点
			xe=v1.x+(y-idxystart)*dx_right;//x轴终点

			//v=v+vstep;
			//float us=v0.u+y*((v2.y-v0.y)/(v2.x-v0.x));
			//int ty=v0.v+stepy*vstep;
			//int tx=v0.u+kv2v0*y;
			int v=v0.v+(y-idxystart)*dvdyl;
			for (int x=xs;x<xe;x++){
				int u=v0.u+(x-xs)*dudx;

				BitmapPixel_24* text=((BitmapPixel_24*)texture_data)+texture->width*v+u; 

				//(int)(texture->width*(v))+us+ustep*(x-xs);

				pixelAddr[x].r=text->r;
				pixelAddr[x].g=text->g;
				pixelAddr[x].b=text->b;
				pixelAddr[x].a=255;
			}

			stepy++;
			pixelAddr=pixelAddr+mWidth;
			
			/*
			if (stepy==5){
				break;
			}
			*/
		}
		
	}
}

void Engine::drawBottomConstantTextureTriangle(Triangle& triangle){
 
	//int v0=0,v1=1,v2=2;
	Vertex4D v0= triangle.vert_list_trans[0];
	Color32  color0=triangle.lit_color[0];

	Vertex4D v1= triangle.vert_list_trans[1];
	Color32 color1=triangle.lit_color[1];

	Vertex4D v2= triangle.vert_list_trans[2];
	Color32 color2=triangle.lit_color[2];


	if ((floatEqual(v0.x,v1.x)&& floatEqual(v1.x,v2.x))||(floatEqual(v0.y,v1.y)&&floatEqual(v1.y,v2.y))){
		//直线不画
		return ;
	}
	if ( (!floatEqual(v0.y,v1.y))&& (!floatEqual(v0.y,v2.y)) &&(!floatEqual(v1.y,v2.y))){
		//没有两个点再同一水平上
		return;
	}

	Vertex4D tempVertex;
	Color32 tempColor;
	if (v1.y<v0.y && v1.y<v2.y){
		//顶点v1.y最小
		tempVertex=v0;
		v0=v1;
		v1=tempVertex;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}else if (v2.y<v0.y&& v2.y<v1.y){
		tempVertex=v0;
		v0=v2;
		v2=tempVertex;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
	}

	if (v1.x>v2.x){
		tempVertex=v1;
		v1=v2;
		v2=tempVertex;

		tempColor=color1;
		color1=color2;
		color2=tempColor;
	}



	float dx_left=0.0f,dx_right=0.0f,dy=0.0f;
	int xs=0,xe=0;
	float k_left=0.0f,k_right=0.0f;

	if (floatEqual(v1.y,v2.y)){
		//平底
		float dx_left=	v1.x-v0.x;
		float dx_right=v2.x-v0.x;

		float dy=v1.y-v0.y;

		float dxlstep=dx_left/dy;
		float dxrstep=dx_right/dy;
		Color32* pixelAddr=(Color32*)mPixelBuffer;
		pixelAddr=pixelAddr+(mWidth*int(v0.y+0.5));

		int bottom=v1.y;
		int step=0;
		for (int y=v0.y;y<bottom;y++){
			step++;
			int xs=v0.x+dxlstep*(y-v0.y);
			int xe=v0.x+dxrstep*(y-v0.y);

			 
			//z++;
			for (int x=xs;x<xe;x++){

 

			}
			pixelAddr=pixelAddr+mWidth;
		}

	}


 
}


void Engine::drawConstantTextureTriangle( Triangle& triangle){
 
	//int v0=0,v1=1,v2=2;
	Vertex4D v0= triangle.vert_list_trans[0];
	Color32  color0=triangle.lit_color[0];

	Vertex4D v1= triangle.vert_list_trans[1];
	Color32 color1=triangle.lit_color[1];

	Vertex4D v2= triangle.vert_list_trans[2];
	Color32 color2=triangle.lit_color[2];


	if ((floatEqual(v0.x,v1.x)&& floatEqual(v1.x,v2.x))||(floatEqual(v0.y,v1.y)&&floatEqual(v1.y,v2.y))){
		//直线不画
		return ;
	}

	//照y排序
	Vertex4D tempV;
	Color32 tempColor;
Label:
	if (v0.y>v1.y){
		tempV=v0;
		v0=v1;
		v1=tempV;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}

	if (v1.y>v2.y){
		tempV=v1;
		v1=v2;
		v2=tempV;

		tempColor=color1;
		color1=color2;
		color2=color1;
		goto Label;
	}

	if (v0.y>v2.y){
		tempV=v0;
		v0=v2;
		v2=tempV;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
		goto Label;
	}

	//照x排序
Label2:

	if (floatEqual(v0.y,v1.y)&& v0.x>v1.x ){
		tempV=v0;
		v0=v1;
		v1=tempV;

		tempColor=color0;
		color0=color1;
		color1=tempColor;
	}

	if (floatEqual(v1.y,v2.y)&& v1.x>v2.x){

		tempV=v1;
		v1=v2;
		v2=tempV;

		tempColor=color1;
		color1=color2;
		color2=color1;
		goto Label2;
	}

	if (floatEqual(v0.y,v2.y)&&v0.x>v2.x){
		tempV=v0;
		v0=v2;
		v2=tempV;

		tempColor=color0;
		color0=color2;
		color2=tempColor;
		goto Label2;
	}
	//上面的这段排序代码要重写


	float dx_left=0.0f,dx_right=0.0f,dy=0.0f;
	int xs=0,xe=0;
	float k_left=0.0f,k_right=0.0f;

	if (floatEqual(v0.y,v1.y)){
		drawTopConstantTextureTriangle(triangle);
		//drawTopGouraudTriangle(triangle);

	}else if (floatEqual(v1.y,v2.y)){
		drawBottomConstantTextureTriangle(triangle);
		//drawBottomGouraudTriangle(triangle);
	}else {
		Vertex4D v;
		//v.x=triangle.vert_list_trans[1].x;
		Triangle t1;
		Vertex4D vertex;

		float k=(v2.y-v0.y)/(v2.x-v0.x);
		float xt=((v1.y-v0.y)/k)+v0.x;
		float ut=((v1.v-v0.v)/k)+v0.u;
		float vt=v1.v;
		float yt=v1.y;
		
		vertex.x=xt;
		vertex.y=yt;
		vertex.u=ut;
		vertex.v=vt;

		int r=color0.r*( (v1.y-v0.y)/(v2.y-v0.y) );
		int g=color0.g*( (v1.y-v0.y)/(v2.y-v0.y) );
		int b=color0.b*( (v1.y-v0.y)/(v2.y-v0.y) );



		t1.attr=triangle.attr;
		t1.state=triangle.state;
		t1.color=triangle.color;
		t1.texture=triangle.texture;
		t1.material=triangle.material;
		t1.normal=triangle.normal;
		t1.nlength=triangle.nlength;
		

		t1.vert_list_trans[0]=v0; //triangle.vert_list_trans[0];
		t1.lit_color[0]=color0; 
		

		t1.vert_list_trans[1]=v1; //triangle.vert_list_trans[1];
		t1.lit_color[1]=color1;
		//vertex.x=xt;
		//vertex.y=yt;
		//vertex.v=v1.v;
		//vertex.u=(-((v1.x-xt)*(v2.u-v0.u))/(v2.y-v0.y)) +v1.u;
		
		/*
		t1.vert_list_trans[2].x=xt;
		t1.vert_list_trans[2].y=yt;
		t1.vert_list_trans[2].u=v1.u ;
		float k2=v2.v;
		if (floatEqual(k2,0)){
			k2=v1.v;
			//v2.v=v1.v;
		}
		*/
		t1.vert_list_trans[2]=vertex;
		t1.lit_color[2]=Color32(r,g,b,255);
		

		if ( t1.vert_list_trans[0].x<300 ||t1.vert_list_trans[1].x<300 ||t1.vert_list_trans[2].x<300){
			CCLOG("ASDF");
		}
		drawBottomConstantTextureTriangle(t1);
		//drawBottomGouraudTriangle(t1);


		t1.vert_list_trans[0]=v1; //t1.vert_list_trans[1];
		t1.lit_color[0]=color1; //t1.lit_color[1];

		vertex.x=xt;
		vertex.y=yt;
		vertex.u=t1.vert_list_trans[2].u;
		vertex.v=t1.vert_list_trans[2].v;
		t1.vert_list_trans[1]=vertex;
		

		
		t1.lit_color[1]=Color32(r,g,b,255);//color2; //t1.lit_color[2];

		t1.vert_list_trans[2]=v2; //triangle.vert_list_trans[2];
		t1.lit_color[2]=color2; //triangle.lit_color[2];
		drawTopConstantTextureTriangle(t1);
		//drawTopGouraudTriangle(t1);


		if ( t1.vert_list_trans[0].x<300 ||t1.vert_list_trans[1].x<300 ||t1.vert_list_trans[2].x<300){
			CCLOG("ASDF");
		}

	}


}


/*
void Engine::drawConstantTextureTriangle( Triangle& poly){
	
	BitmapPixel_24 *texture_data = (BitmapPixel_24*)(poly.texture->data); 
	Color32* buffer=mPixelBuffer;
	for (int y=0;y<poly.texture->height;y++){
	
		for (int x=0;x<poly.texture->width;x++){
			buffer[x]=Color32(texture_data[x].r,texture_data[x].g,texture_data[x].b,255);
		}
		buffer=buffer+mWidth;
		texture_data+=poly.texture->width;
	}
	
	//BitmapPixel_24& textel=triangle.texture

	//*(pixelAddr+x)=(texture->data+u*texture->width+v+texture->height);
}
*/