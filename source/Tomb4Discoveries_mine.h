
// FOR_YOU: here you can type your discoveries about tomb4 procedures
// if you mean use the C language to call them, you can create the
// prototypes of them like it has been done in the "DefTomb4Funct.h" file
// and "functions.h" file.
// While if you wish use only assembly code, you can type here
// the #define ADR_NOME_PROC 0x404040  to remember and use in the asm
// code all addresses of the tomb4 procedures

// TYPE_HERE: prototype (c++ language), mnemonic constants for 
// addresses (assembly)


// FOR_YOU: here you can type your discoveries about tomb4 procedures
// if you mean use the C language to call them, you can create the
// prototypes of them like it has been done in the "DefTomb4Funct.h" file
// and "functions.h" file.
// While if you wish use only assembly code, you can type here
// the #define ADR_NOME_PROC 0x404040  to remember and use in the asm
// code all addresses of the tomb4 procedures

// TYPE_HERE: prototype (c++ language), mnemonic constants for 
// addresses (assembly)

/* floor/ceiling object procedure prototype */
typedef void (__cdecl *TYPE_FloorProc) (Tr4ItemInfo *item, int x, int y, int z, int *height);

/* general-use lara_as and lara_col procedures */
typedef void (__cdecl *TYPE_lara_void_func) (Tr4ItemInfo *pLara, Tr4CollInfo *pColl);
typedef void (__cdecl *TYPE_lara_default_col) (Tr4ItemInfo *pLara, Tr4CollInfo *pColl);
typedef void (__cdecl *TYPE_lara_as_null) (Tr4ItemInfo *pLara, Tr4CollInfo *pColl);

TYPE_lara_void_func lara_void_func = (TYPE_lara_void_func) 0x425D20;
TYPE_lara_default_col lara_default_col = (TYPE_lara_default_col) 0x428600;
TYPE_lara_as_null lara_as_null = (TYPE_lara_as_null) 0x426DE0;


/* useful tomb4 functions */
typedef void (__cdecl *TYPE_GetLaraCollisionInfo) (Tr4ItemInfo *pLara, Tr4CollInfo *pCollision);
typedef int (__cdecl *TYPE_TestWall) (Tr4ItemInfo *pItem, int Front, int Right, int Down);
typedef void (__cdecl *TYPE_GetCollisionInfo) (Tr4CollInfo *pCollision, int CordX, int CordY, int CordZ, short room_number, int obj_height);
typedef void (__cdecl *TYPE_TranslateItem) (Tr4ItemInfo *pItem, int OffsetX, int OffsetY, int OffsetZ);
typedef int (__cdecl *TYPE_CheckNoColFloorTriangle) (Tr4FloorInfo *floor, int x, int z);
typedef int (__cdecl *TYPE_CheckNoColCeilingTriangle) (Tr4FloorInfo *floor, int x, int z);

TYPE_GetLaraCollisionInfo GetLaraCollisionInfo = (TYPE_GetLaraCollisionInfo) 0x422180;
TYPE_TestWall TestWall = (TYPE_TestWall) 0x4226F0;
TYPE_GetCollisionInfo GetCollisionInfo = (TYPE_GetCollisionInfo) 0x445920;
TYPE_TranslateItem TranslateItem = (TYPE_TranslateItem) 0x449750;
TYPE_CheckNoColFloorTriangle CheckNoColFloorTriangle = (TYPE_CheckNoColFloorTriangle) 0x44BEB0;
TYPE_CheckNoColCeilingTriangle CheckNoColCeilingTriangle = (TYPE_CheckNoColCeilingTriangle) 0x44BF60;



//typedef int (__cdecl *TYPE_TestLaraSlide) (StrItemTr4 *pLara, StrCollisionLara *pCollision);
//typedef void (__cdecl *TYPE_LaraSlideSlope) (StrItemTr4 *pLara, StrCollisionLara *pCollision);
//typedef int (__cdecl *TYPE_LaraHitCeiling) (Tr4ItemInfo *pLara, Tr4CollInfo *pCollision);
//typedef int (__cdecl *TYPE_LaraDeflectEdge) (Tr4ItemInfo *pLara, Tr4CollInfo *pCollision);
//typedef int (__cdecl *TYPE_TestLaraVault) (StrItemTr4 *pLara, StrCollisionLara *pCollision);
//typedef void (__cdecl *TYPE_LaraCollideStop) (StrItemTr4 *pLara, StrCollisionLara *pCollision);
//typedef int (__cdecl *TYPE_LaraFallen) (StrItemTr4 *pLara, StrCollisionLara *pCollision);
//typedef void (__cdecl *TYPE_ShiftItem) (Tr4ItemInfo *pItem, Tr4CollInfo *pCollision);
//typedef int (__cdecl *TYPE_GetChange) (Tr4ItemInfo *pItem, StrAnimationTr4 *pAnim);

//TYPE_TestLaraSlide TestLaraSlide = (TYPE_TestLaraSlide) 0x420CD0; 
//TYPE_LaraSlideSlope LaraSlideSlope = (TYPE_LaraSlideSlope) 0x428150; 
//TYPE_LaraHitCeiling LaraHitCeiling = (TYPE_LaraHitCeiling) 0x4221C0;
//TYPE_LaraDeflectEdge LaraDeflectEdge = (TYPE_LaraDeflectEdge) 0x422230;
//TYPE_TestLaraVault TestLaraVault = (TYPE_TestLaraVault) 0x4222B0;
//TYPE_LaraCollideStop LaraCollideStop = (TYPE_LaraCollideStop) 0x4227F0;
//TYPE_LaraFallen LaraFallen = (TYPE_LaraFallen) 0x420E10;
//TYPE_ShiftItem ShiftItem = (TYPE_ShiftItem) 0x446700;
//TYPE_GetChange GetChange = (TYPE_GetChange) 0x4496B0;
