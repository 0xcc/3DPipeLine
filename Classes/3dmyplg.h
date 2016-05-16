#ifndef _3D_MY_PLG_H_
#define _3D_MY_PLG_H_

#define PLX_RGB_MASK				 0x8000  //��ɫģʽ
#define PLX_SHADE_MODE_MASK          0x6000 //��ɫģʽ����
#define PLX_2SIDED_MASK				 0x1000 //�Ƿ�˫������

//��ɫģʽ
#define PLX_COLOR_MODE_RGB_FLAG     0x8000
#define PLX_COLOR_MODE_INDEXED_FLAG 0x0000 

#define PLX_SHADE_MODE_PURE_FLAG      0x0000 //�̶���ɫ���޹���
#define PLX_SHADE_MODE_CONSTANT_FLAG  0x0000 
#define PLX_SHADE_MODE_FLAT_FLAG      0x2000 // �㶨��ɫ �й���
#define PLX_SHADE_MODE_GOURAUD_FLAG   0x4000 // Gouraud����
#define PLX_SHADE_MODE_PHONG_FLAG     0x6000 // Phong����

#define PLX_2SIDED_FLAG              0x1000



#endif