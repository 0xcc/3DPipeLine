#ifndef _3D_BITMAP_H_
#define _3D_BITMAP_H_
#include "3dmath.h"
#include "fileutil.h"
#include <cocos2d.h>

#define BITMAP_FILE_HEADER_LEN 14
#define BITMAP_INFO_HEADER_LEN 40

struct BitmapFileHeader{
	uint16_t bfType;      // ���ͱ�־ 2 Byte
	uint32_t bfSize;      // �ļ���С 4
	uint16_t bfReserved1; // ������   2
	uint16_t bfReserved2; // ������   2
	uint32_t bfOffBits;   // λͼ����ƫ�� 4

	void init(const unsigned char* buff); // ���ڴ����(14�ֽ�)
};

// ��Ϣͷ
struct BitmapInfoHeader
{
	uint32_t  biSize;          // �ṹ��С 4Byte
	int32_t   biWidth;         // λͼ��� 4
	int32_t   biHeight;        // λͼ�߶�(��ֵ�ߵ�,��ֵ����) 4
	uint16_t  biPlanes;        // ����Ϊ1  2
	uint16_t  biBitCount;      // ÿ����λ��(1��4��8��16��24) 2
	uint32_t  biCompression;   // ѹ����־  4
	uint32_t  biSizeImage;     // λͼ���ݴ�С 4
	int32_t   biXPelsPerMeter; // ˮƽ�ֱ���  4
	int32_t   biYPelsPerMeter; // ��ֱ�ֱ���  4
	uint32_t  biClrUsed;       // 4
	uint32_t  biClrImportant;  // 4

	void init(const unsigned char* buff); // ���ڴ����(40�ֽ�)
};

struct BitmapPixel_24
{
	uint8_t b; // rgb
	uint8_t g;
	uint8_t r;
};


// λͼ���ݶ���
typedef struct BitmapImage
{
	BitmapImage() : width(0), height(0), bpp(0), data(nullptr) {}
	int width, height; // �ߴ�
	int bpp;           // λ��
	unsigned char *data; // λͼ����
	void reset();
	bool loadFromFile(char* filename); // ���غ����ж��ǲ����Լ���Ҫ��λ��
} BitmapImage;

#endif