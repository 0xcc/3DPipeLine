#include "3dbitmap.h"

void BitmapFileHeader::init(const unsigned char* buff)
{
	this->bfType = fromLittleEndian16(&buff[0]);
	this->bfSize = fromLittleEndian32(&buff[2]);
	this->bfReserved1 = fromLittleEndian16(&buff[6]);
	this->bfReserved2 = fromLittleEndian16(&buff[8]);
	this->bfOffBits = fromLittleEndian32(&buff[10]);
}


void BitmapInfoHeader::init(const unsigned char* buff)
{
	this->biSize = fromLittleEndian32(&buff[0]);
	this->biWidth = fromLittleEndian32(&buff[4]);
	this->biHeight = fromLittleEndian32(&buff[8]);
	this->biPlanes = fromLittleEndian16(&buff[12]);
	this->biBitCount = fromLittleEndian16(&buff[14]);
	this->biCompression = fromLittleEndian32(&buff[16]);
	this->biSizeImage = fromLittleEndian32(&buff[20]);
	this->biXPelsPerMeter = fromLittleEndian32(&buff[24]);
	this->biYPelsPerMeter = fromLittleEndian32(&buff[28]);
	this->biClrUsed = fromLittleEndian32(&buff[32]);
	this->biClrImportant = fromLittleEndian32(&buff[36]);
}

void BitmapImage::reset(){
	this->width=0;
	this->height=0;
	this->bpp=0;
	if (this->data){
		free(this->data);
		this->data=nullptr;
	}
}


bool BitmapImage::loadFromFile(char* filename){
	reset();
	FileUtils* fileUtil=FileUtils::getInstance();
	std::string fileContentString= fileUtil->getStringFromFile(filename);

	unsigned char* filebuff=(unsigned char*) fileContentString.c_str();
	BitmapFileHeader fileheader;
	fileheader.init(filebuff);

	if (fileheader.bfType!=0x4D42){
		CCLOG("Bitmap file Flag error: %s", filename);
		return false;
	}

	BitmapInfoHeader infoheader;
	infoheader.init(filebuff + BITMAP_FILE_HEADER_LEN);

	// 长度检测 注意,这里的filedata.getSize()一定是非负数了 因为上面已经判断过
	if (fileContentString.size()< BITMAP_FILE_HEADER_LEN + BITMAP_INFO_HEADER_LEN + infoheader.biSizeImage)
	{
		CCLOG("Bitmap file length error: %s", filename);
		return false;
	}

	// 尺寸检测
	if (infoheader.biWidth == 0 || infoheader.biHeight == 0)
	{
		CCLOG("Bitmap file size error: %s", filename);
		return false;
	}

	this->width = infoheader.biWidth;
	this->height = abs(infoheader.biHeight); // 有可能是负数
	this->bpp = infoheader.biBitCount;

	// 每行字节数 (必须为4的倍数,不足的以0填充)
	int per_line_bytes = ((this->width * this->bpp + 31) & ~31) / 8;

	// 仅原样复制 (注意每行之间可能有间隙,当每行有效数据量<per_line_bytes时)
	this->data = static_cast<unsigned char *>(malloc(infoheader.biSizeImage));

	if (infoheader.biHeight<0){
		unsigned char* imagebuff=filebuff+fileContentString.size()-infoheader.biSizeImage;

		memcpy(this->data,imagebuff,infoheader.biSizeImage);
	}else{
		unsigned char* imagebuff=filebuff+fileContentString.size()-per_line_bytes;
		unsigned char* destbuff = this->data;
		for (int j = 0; j < this->height; j++)
		{
			memcpy(destbuff, imagebuff, per_line_bytes);

			imagebuff -= per_line_bytes;
			destbuff += per_line_bytes;
		}
	}


	return true;
}