
// TYPE_HERE: here you can type your mnemonic constants, using the #define directive
// like trng did it in the "Tomb_NextGeneration.h" file
// in this source you can also type your declaration of tomb raider function
// like it happens for "functions.h" source

// -------- START PRESET CONSTANTS ---------------------------------
// please don't remove the constants in this "PRESET CONSTANT" zone. 
// They will be used by some preset function in your plugin source

#define MAX_MYPROGR_ACTIONS 100
#define MAX_ASSIGN_SLOT_MINE 200

#define ENDLESS_DURATE 0xFFFF // to use as number of frames to set an infinite durate
#define NGTAG_END_SEQUENCE		   0   // signals the end of ng token sequence
#define NGTAG_PROGRESSIVE_ACTIONS  1
#define NGTAG_LOCAL_DATA           2
#define NGTAG_GLOBAL_DATA          3
#define AXN_FREE  0  // this record is to free an action record. You type this value in ActionType to free a action progress record
// --------- END PRESET CONSTANTS ---------------------------------
// type here the constant name for new progressive action you create
// use progressive number to have always different value for each AXN_ constant/action

// OBJ_ constant for animation object containing climbing anims
#define OBJ_LARA_OVERHANG_CLIMB 1

// useful tomb4 addresses
#define TR4_SLOTPTR_ADR 0x52b720 // tomb4 slots[] array pointer address
#define TR4_ROOMPTR_ADR 0x533934 // tomb4 rooms[] array pointer address
#define TR4_ANIMPTR_ADR 0x533938 // tomb4 anims[] array pointer address
#define TR4_FLOORDATA_ADR 0x533948 // tomb4 floordata[] array pointer address
#define TR4_INPUTBITS_ADR 0x5355d8 // tomb4 input pointer address
#define TR4_LARAITEM_ADR 0x80E01C // tomb4 lara item pointer address
#define TR4_LARAINFO_ADR 0x80DEC0 // tomb4 LaraInfo struct address
#define TR4_CAMERA_ADR 0x7FE700

// useful TRNG addresses
#define TRNG_LARA_AS 0x10153B90
#define TRNG_LARA_COL 0x10153F90


// useful constants
#define NO_ITEM -1
#define NO_ROOM 0xFF
#define NO_HEIGHT 0x7F00

#define ONE_STEP 256
#define ONE_SECTOR ONE_STEP*4
#define HITE_ADJUST 20 + ONE_STEP*2

#define FACING_NORTH 0
#define FACING_EAST 16384
#define FACING_SOUTH -32768
#define FACING_WEST -16384
#define QUADRANT_NORTH 0
#define QUADRANT_EAST 1
#define QUADRANT_SOUTH 2
#define QUADRANT_WEST 3
