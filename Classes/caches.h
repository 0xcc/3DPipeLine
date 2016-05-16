#ifndef _3D_CACHES_H_
#define _3D_CACHES_H_

struct CachesTable
{
public:
	// 透明度(0～255)与颜色分量(0～255)乘积表，每一行对应一种透明度与各种颜色分量的乘积
	static unsigned char alpha_table[256][256]; 

	//log2 n 查找表
	static const int mLogBase2_of_x[513];

public:
	static void init();
};

#endif