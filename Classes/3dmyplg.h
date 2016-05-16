#ifndef _3D_MY_PLG_H_
#define _3D_MY_PLG_H_

#define PLX_RGB_MASK				 0x8000  //颜色模式
#define PLX_SHADE_MODE_MASK          0x6000 //着色模式掩码
#define PLX_2SIDED_MASK				 0x1000 //是否双面掩码

//颜色模式
#define PLX_COLOR_MODE_RGB_FLAG     0x8000
#define PLX_COLOR_MODE_INDEXED_FLAG 0x0000 

#define PLX_SHADE_MODE_PURE_FLAG      0x0000 //固定着色，无光照
#define PLX_SHADE_MODE_CONSTANT_FLAG  0x0000 
#define PLX_SHADE_MODE_FLAT_FLAG      0x2000 // 恒定着色 有光照
#define PLX_SHADE_MODE_GOURAUD_FLAG   0x4000 // Gouraud光照
#define PLX_SHADE_MODE_PHONG_FLAG     0x6000 // Phong光照

#define PLX_2SIDED_FLAG              0x1000



#endif