#ifndef _3D_BITMAP_H_
#define _3D_BITMAP_H_
#include "3dmath.h"
#include "fileutil.h"
#include <cocos2d.h>

#define BITMAP_FILE_HEADER_LEN 14
#define BITMAP_INFO_HEADER_LEN 40

struct BitmapFileHeader{
	uint16_t bfType;      // 类型标志 2 Byte
	uint32_t bfSize;      // 文件大小 4
	uint16_t bfReserved1; // 保留字   2
	uint16_t bfReserved2; // 保留字   2
	uint32_t bfOffBits;   // 位图数据偏移 4

	void init(const unsigned char* buff); // 从内存解析(14字节)
};

// 信息头
struct BitmapInfoHeader
{
	uint32_t  biSize;          // 结构大小 4Byte
	int32_t   biWidth;         // 位图宽度 4
	int32_t   biHeight;        // 位图高度(正值颠倒,负值正常) 4
	uint16_t  biPlanes;        // 必须为1  2
	uint16_t  biBitCount;      // 每像素位深(1、4、8、16、24) 2
	uint32_t  biCompression;   // 压缩标志  4
	uint32_t  biSizeImage;     // 位图数据大小 4
	int32_t   biXPelsPerMeter; // 水平分辨率  4
	int32_t   biYPelsPerMeter; // 垂直分辨率  4
	uint32_t  biClrUsed;       // 4
	uint32_t  biClrImportant;  // 4

	void init(const unsigned char* buff); // 从内存解析(40字节)
};

struct BitmapPixel_24
{
	uint8_t b; // rgb
	uint8_t g;
	uint8_t r;
};


// 位图数据对象
typedef struct BitmapImage
{
	BitmapImage() : width(0), height(0), bpp(0), data(nullptr) {}
	int width, height; // 尺寸
	int bpp;           // 位深
	unsigned char *data; // 位图数据
	void reset();
	bool loadFromFile(char* filename); // 加载后请判断是不是自己需要的位深
} BitmapImage;

#endif