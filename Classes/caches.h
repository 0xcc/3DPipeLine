#ifndef _3D_CACHES_H_
#define _3D_CACHES_H_

struct CachesTable
{
public:
	// ͸����(0��255)����ɫ����(0��255)�˻���ÿһ�ж�Ӧһ��͸�����������ɫ�����ĳ˻�
	static unsigned char alpha_table[256][256]; 

	//log2 n ���ұ�
	static const int mLogBase2_of_x[513];

public:
	static void init();
};

#endif