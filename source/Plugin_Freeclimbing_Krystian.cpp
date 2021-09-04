// FOR_YOU:
// This is the main source of your plugin.
// It's in this source you'll type all your assembly or c++ code and all 
// the variables and memory zones you need.

// ************  Top/Header section ************
#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "bass.h"		// prototypes for extra sound library: bass.dll
	// Following header files will be updated for every new version of 
	// the tomb_NextGeneration.dll, so it's better you don't change them
	//  because they will be replaced for any new update.

#include "Tomb_NextGeneration.h" // mnemonic constants defined in tomb_NextGeneration.dll
#include "structures.h" // structure of tomb4 program and trng dll
#include "DefTomb4Funct.h" // defines of tomb4 procedure prototypes
#include "functions.h"  // assigments of real tomb4 address to each tomb4 procedures
#include "macros.h"  // definition of macros

	// FOR_YOU:
	// While the followings are the headers you can use 
	// to type your structures, constants and new tomb4 procedures you 
	// discovered. 
	// Following files are only yours and trng will not ever change them:
#include "macros_mine.h"  // here you define your own macros
#include "constants_mine.h" // here you define your mnemonic constants
#include "structures_mine.h" // here you type your structure definitions
#include "Tomb4Discoveries_mine.h" // here type tomb4 procedures you discovered

#include "trng.h" // list of trng functions imported from trng.cpp source. 

#pragma warning( error : 4706 )
#pragma warning(disable: 4996)

// ************  Early function declarations ************


// ************  Global Variables Section *************

// FOR_YOU:
// here you type the variables or memory zones you wish use in different
// C++ procedures.
// If you use the assembly you'll have to type ALL your variables in this
// section.
// for example if you wish have a 32 bits signed variable to store big
// numbers you can type:
// TYPE_HERE:
// int MyNumber;
// and then you can use it in asm in this way:
//		mov  eax, [MyNumber]
// or
//      mov eax, dword ptr [MyNumber]
// same speech for wide memory zones.
// If you wish having a memory zone of 13000 bytes you can type:
//  BYTE MyZone[13000];
// and then you can use it in this way:
//     mov  al, MyNumber[ecx]  ; read the ECXth byte and copy it in AL
// or also:
//     mov al, [MyNumber+ecx] ; same result of above instruction

// Variables and memory zone to TYPE_HERE:

HINSTANCE MyDllInstance=NULL;  // the instance handle of your dll

extern char BufferLog[4096]; // temporary global buffer to host error and warning messages

// FOR_YOU: If you mean create code patch (changes in tomb4 code to call your code in the plugin dll,
// you MUST change the MyTomb4PatcherAddress value (here below) 
// You can discover a free tomb4 zone using TrngPatcher program, with the 
// menu command "Tools->Get Assignment of Free Memory range"
// Note: choose an address, in the given range, terminating with hex 
// digits: 0,4,8, or C
// because it's usefull that the address was a multiple by 4
// If you chose an address used from other plugins you'll get an error and
// the game will be aborted
// note: if you don't mean use code patches you can let 0x0 in following line
DWORD MyTomb4PatcherAddress = 0x0; // <- TYPE_HERE: the new address you chose
								
// this text will contain your plugin name (omitting .dll extension).
// it will be used to give a title to messagebox for error messages or warnings
char TexMyPluginName[80];  
// in this MyData structure you can store all global variables for your plugin
// you have to define them in structures_mine.h source inside structure "StrMyData" or for variable
// to save and restore in savegames, in "StrSavegameGlobalData" or "StrSavegameLocalData" structures

StrMyData MyData;
#define SLOPE_CHECK (slope.tiltX == goal.tiltX && slope.tiltZ == goal.tiltZ)
#define SLOPE_INVERSE_CHECK  (slope.tiltX == -goal.tiltX && slope.tiltZ == -goal.tiltZ)

// ************  Utilities section  ****************

// replaces lara_as_* routine entry for given stateNum with func
void ReplaceLaraCtrlRoutine(short stateNum, void* func)
{
	void** lara_as = (void**)TRNG_LARA_AS;
	lara_as[stateNum] = func;
}

// replaces lara_col_* routine entry for given stateNum with func
void ReplaceLaraColRoutine(short stateNum, void* func)
{
	void** lara_col = (void**)TRNG_LARA_COL;
	lara_col[stateNum] = func;
}


// ************  Patcher Functions section  ***************
// Note: please, don't change or remove the C++ procedure you find here,
// because they are necessary to do work the plugin with the trng dll
// Anyway in many of these functions you can add your code


// FOR_YOU: In this function you insert the callings of functions used to change tomb4 code
// You can get these functions, in the correct format, using Trng Core -> Asm Souce Editor -> Debugging menu
// program
// note: if there is a conflict the function will return "false"
// and the tomb4 program will abort with an error message
bool CreateMyCodePatches(void)
{
	// the call of the code patch to TYPE_HERE:
	// example: 
	// SET_PATCH(Path_RedirCollision)
	// to call the function Patch_RedirCollision() created with TrngPatcher program (command Assmembly->Create Dynamic Patch Generator)

	return true;
}

// ************  Assembly Procedures section  ******************

// FOR_YOU: In the SubPatchArray you'll type all procedure names of your code in the dll you
// wish were called from tomb4 code.
// type them in the order of ax value. So first asm proc in the list, will be called
// with ax=0, while the second in the list will be called with ax=1 ect.



void *SubPatchArray[] = {
	
// TYPE_HERE your asm procedure names to call from tomb4 code
	NULL
};


// ************  MainPatcher() asm procedure  *****************

// FOR_YOU: This is your main patcher procedure.
// All your patch in tomb4 code will call this procedure passing to it, in ax register,
// the number of subpatch to call

BEGIN_ASM_PROC(MainPatcher)
	and eax, 0ffffh
	mov eax, dword ptr [SubPatchArray+eax*4];
	jmp eax
END_ASM_PROC





// **************  Floordata functions section  *************** //


/* test for monkey ceiling at bottom of room stack */
bool TestMonkey(Tr4FloorInfo *floor, int x, int z) 
{
	if (floor == NULL)
		return false;
	
	Tr4RoomInfo *rooms = (Tr4RoomInfo*)Trng.pGlobTomb4->pAdr->pVetRooms;
	Tr4RoomInfo *r;

	while (floor->pit_room != NO_ROOM) // probe to lowest room in stack
	{
		if (CheckNoColFloorTriangle(floor, x, z) == 1)
			break;
		r = &rooms[floor->pit_room];
		floor = &r->floor[((z - r->origZ)>>10) + ((x - r->origX)>>10) * r->z_size];  
	}

	int index = floor->index;

	if (index)
	{
		WORD dataEntry;
		WORD *floorData = *(WORD**)TR4_FLOORDATA_ADR;

		do
		{
			dataEntry = floorData[index];
			WORD currType = dataEntry&0x1f;

			switch (currType)
			{
			case MONKEY_TYPE:
				return true;

			case DOOR_TYPE:
			case TILT_TYPE:
			case ROOF_TYPE:
			case SPLIT1:
			case SPLIT2:
			case SPLIT3:
			case SPLIT4:
			case NOCOLF1T:
			case NOCOLF1B:
			case NOCOLF2T:
			case NOCOLF2B:
			case NOCOLC1T:
			case NOCOLC1B:
			case NOCOLC2T:
			case NOCOLC2B:
				index += 2;
				break;

			case LAVA_TYPE:
			case CLIMB_TYPE:
			case TRIGTRIGGER_TYPE:
			case MINER_TYPE:
				index++;
				break;
			
			case TRIGGER_TYPE:
				WORD subDataEntry;
				index += 2;
				do
				{
					subDataEntry = floorData[index];
					WORD trigAction = (subDataEntry & 0x7C00) >> 10;

					// if "camera" action, there is one more to skip!
					if (trigAction == TO_CAMERA || trigAction == TO_FLYBY) 
						index++;
					index++;

				} while (!(subDataEntry & 0x8000));

				break;
			}
		} while (!(dataEntry & 0x8000));
	}
	
	return false;
}


/* get climb wall status at bottom of room stack */
WORD GetClimbStatus(Tr4FloorInfo *floor, int x, int z) 
{
	if (!floor)
		return 0;

	Tr4RoomInfo *rooms = (Tr4RoomInfo*)Trng.pGlobTomb4->pAdr->pVetRooms;
	Tr4RoomInfo *r;

	while (floor->pit_room != NO_ROOM) // probe to lowest room in stack
	{
		if (CheckNoColFloorTriangle(floor, x, z) == 1)
			break;
		r = &rooms[floor->pit_room];
		floor = &r->floor[((z - r->origZ)>>10) + ((x - r->origX)>>10) * r->z_size];  
	}

	int index = floor->index;

	if (index)
	{
		WORD dataEntry;
		WORD *floorData = *(WORD**)TR4_FLOORDATA_ADR;

		do
		{
			dataEntry = floorData[index];
			WORD currType = dataEntry&0x1f;

			switch (currType)
			{
			case CLIMB_TYPE:
				return dataEntry;

			case DOOR_TYPE:
			case TILT_TYPE:
			case ROOF_TYPE:
			case SPLIT1:
			case SPLIT2:
			case SPLIT3:
			case SPLIT4:
			case NOCOLF1T:
			case NOCOLF1B:
			case NOCOLF2T:
			case NOCOLF2B:
			case NOCOLC1T:
			case NOCOLC1B:
			case NOCOLC2T:
			case NOCOLC2B:
				index += 2;
				break;

			case LAVA_TYPE:
			case MONKEY_TYPE:
			case TRIGTRIGGER_TYPE:
			case MINER_TYPE:
				index++;
				break;
			
			case TRIGGER_TYPE:
				WORD subDataEntry;
				index += 2;
				do
				{
					subDataEntry = floorData[index];
					WORD trigAction = (subDataEntry & 0x7C00) >> 10;

					// if "camera" action, there is one more to skip!
					if (trigAction == TO_CAMERA || trigAction == TO_FLYBY)
						index++;
					index++;

				} while (!(subDataEntry & 0x8000));

				break;
			}
		} while (!(dataEntry & 0x8000));
	}

	return 0;
}

/* get tilt grades of ceiling slope at x and z (GetTiltType, but for ceilings) */
SlopeTilts GetCeilingTilts(Tr4FloorInfo *floor, int x, int z) 
{
	SlopeTilts slant = {0, 0};
	if (!floor)
		return slant;

	Tr4RoomInfo *rooms = (Tr4RoomInfo*)Trng.pGlobTomb4->pAdr->pVetRooms;
	Tr4RoomInfo *r;

	while (floor->sky_room != 0xFF) // probe to highest room in stack
	{
		if (CheckNoColCeilingTriangle(floor, x, z) == 1)
			break;
		r = &rooms[floor->sky_room];
		floor = &r->floor[((z - r->origZ)>>10) + ((x - r->origX)>>10) * r->z_size];  
	}

	int index = floor->index;

	if (index)
	{
		WORD dataEntry;
		WORD *floorData = *(WORD**)TR4_FLOORDATA_ADR;
		int xoff = 0, zoff = 0;

		do
		{
			dataEntry = floorData[index];
			WORD currType = dataEntry & 0x1f;
			
			switch(currType)
			{
			case ROOF_TYPE:
				dataEntry = floorData[index+1];
				slant.tiltX = dataEntry & 0xFF;
				slant.tiltZ = dataEntry >> 8;
				return slant;

			case SPLIT3:
			case SPLIT4:
			case NOCOLC1T:
			case NOCOLC1B:
			case NOCOLC2T:
			case NOCOLC2B:
				dataEntry = floorData[index+1];

				int t0, t1, t2, t3, dx, dz;

				t0 = dataEntry & 0xF;
				t1 = (dataEntry >> 4) & 0xF;
				t2 = (dataEntry >> 8) & 0xF;
				t3 = (dataEntry >> 12) & 0xF;

				dx = x & 0x3FF;
				dz = z & 0x3FF;

				if (currType == SPLIT3 || currType == NOCOLC1T || currType == NOCOLC1B) // NW-SE split
				{
					if (dx <= 1024 - dz)
					{
						xoff = t2-t3;
						zoff = t1-t2;
					}
					else
					{
						xoff = t1-t0;
						zoff = t0-t3;
					}
				}
				else // NE-SW split
				{
					if (dx <= dz)
					{
						xoff = t1-t0;
						zoff = t1-t2;
					}
					else
					{
						xoff = t2-t3;
						zoff = t0-t3;
					}
				}

				slant.tiltX = xoff;
				slant.tiltZ = zoff;
				return slant;

			case DOOR_TYPE:
			case TILT_TYPE:
			case SPLIT1:
			case SPLIT2:
			case NOCOLF1T:
			case NOCOLF1B:
			case NOCOLF2T:
			case NOCOLF2B:
				index += 2;
				break;

			case LAVA_TYPE:
			case CLIMB_TYPE:
			case MONKEY_TYPE:
			case TRIGTRIGGER_TYPE:
			case MINER_TYPE:
				index++;
				break;

			case TRIGGER_TYPE:
				WORD subDataEntry;
				index += 2;
				do
				{
					subDataEntry = floorData[index];
					WORD trigAction = (subDataEntry & 0x7C00) >> 10;

					if (trigAction == TO_CAMERA || trigAction == TO_FLYBY) 
						index++;
					index++;

				} while (!(subDataEntry & 0x8000));

				break;
			}
		} while (!(dataEntry & 0x8000));
	}
	
	return slant;
}




// **************  Animation functions section  ************** //

// Set animNum animation for lara, with frameNum frame number and optionally force current and goal states
void SetAnimLara(WORD animNum, WORD frameNum = 0, short currentState = -1, short goalState = -1)
{
	Tr4ItemInfo *lara = (Tr4ItemInfo*) Trng.pGlobTomb4->pAdr->pLara;
	Tr4AnimStruct *anim = (Tr4AnimStruct*) Trng.pGlobTomb4->pAdr->pVetAnimations;
	
	int frameLength = anim[animNum].FrameEnd - anim[animNum].FrameBase;
	if (frameNum > frameLength)
		frameNum = frameLength;
	
	if (currentState < 0)
		currentState = anim[animNum].StateID;
	if (goalState < 0)
		goalState = anim[animNum].StateID;

	lara->anim_number = animNum;
	lara->frame_number = anim[animNum].FrameBase + frameNum;
	lara->current_state = currentState;
	lara->goal_state = goalState;
}


/* return frame number relative to the start frame of lara's current anim */
int GetRelativeFrame() 
{
	Tr4ItemInfo *lara = (Tr4ItemInfo*) Trng.pGlobTomb4->pAdr->pLara;
	Tr4AnimStruct *anim = (Tr4AnimStruct*) Trng.pGlobTomb4->pAdr->pVetAnimations;
	return (lara->frame_number - anim[lara->anim_number].FrameBase);
}


/* set Lara's current anim with animNum index from overhang anim slot */
int SetOverhangAnim(WORD animNum)
{
	if (MyData.overhangSlot <= 0)  // if overhang slot wasn't assigned
		return -1;

	Tr4ItemInfo *lara = (Tr4ItemInfo*) Trng.pGlobTomb4->pAdr->pLara;
	Tr4AnimStruct *anim = (Tr4AnimStruct*) Trng.pGlobTomb4->pAdr->pVetAnimations;
	Tr4ObjectInfo *slot = (Tr4ObjectInfo*) Trng.pGlobTomb4->pAdr->pVetSlot;

	int index = slot[MyData.overhangSlot].AnimIndex + animNum;
	
	lara->anim_number = index;
	lara->frame_number = anim[index].FrameBase;
	lara->current_state = anim[index].StateID;
	lara->goal_state = anim[index].StateID;
	lara->pos.xRot = 0;
	
	return 0;
}


/* return current anim index relative to overhang anim slot */
int GetOverhangAnim() 
{
	if (MyData.overhangSlot <= 0) // if overhang slot wasn't assigned
		return -1;

	Tr4ItemInfo *lara = *(Tr4ItemInfo**) TR4_LARAITEM_ADR;
	Tr4AnimStruct *anim = *(Tr4AnimStruct**)TR4_ANIMPTR_ADR;
	Tr4ObjectInfo *slot = (Tr4ObjectInfo*)TR4_SLOTPTR_ADR;

	int index = lara->anim_number - slot[MyData.overhangSlot].AnimIndex;

	return index;
}




// **************  Utility functions section  *************** //


/* get the signed diff between two orients */
short DirOrientDiff(short orient1, short orient2) 
{
	int diff = orient1 - orient2;

	if (diff > 32767)
		diff -= 65536;
	if (diff < -32768)
		diff += 65536;

	return short(diff);
}


/* test if inside sector strip (0-1023) in
currently faced quadrant, between min and max */
bool InStrip(int x, int z, short facing, int min, int max) 
{
	if (min > 1023)
		min = 1023;
	if (max > 1023)
		max = 1023;

	int quadrant = (WORD)(facing + 0x2000) / 0x4000;
	int dx = x & 1023;
	int dz = z & 1023;
	
	switch (quadrant)
	{
	case QUADRANT_NORTH:
		if (dz >= (1023 - max) && dz <= (1023 - min))
			return true;
		break;

	case QUADRANT_SOUTH:
		if (dz >= min && dz <= max)
			return true;
		break;

	case QUADRANT_EAST:
		if (dx >= (1023 - max) && dx <= (1023 - min))
			return true;
		break;

	case QUADRANT_WEST:
		if (dx >= min && dx <= max)
			return true;
		break;
	}
	
	return false;
}


/* align facing and X/Z pos to sector edge */
void AlignToEdge(Tr4ItemInfo *lara, WORD edgeDist) 
{
	if (edgeDist > 1023)
		edgeDist = 1023;

	// align to closest cardinal facing
	lara->pos.yRot += 0x2000; 
	lara->pos.yRot &= 0xC000;

	switch (lara->pos.yRot) // align to faced edge
	{
	case FACING_NORTH:
		lara->pos.zPos &= ~0x3FF;
		lara->pos.zPos += (1023-edgeDist);
		break;

	case FACING_SOUTH:
		lara->pos.zPos &= ~0x3FF;
		lara->pos.zPos += edgeDist+1;
		break;

	case FACING_EAST:
		lara->pos.xPos &= ~0x3FF;
		lara->pos.xPos += (1023-edgeDist);
		break;

	case FACING_WEST:
		lara->pos.xPos &= ~0x3FF;
		lara->pos.xPos += edgeDist+1;
		break;
	}
}


/* correct position after grabbing slope */
void AlignToGrab(Tr4ItemInfo *lara, bool &legLeft)
{ 
	lara->pos.yRot += 0x2000;
	lara->pos.yRot &= 0xC000;

	switch (lara->pos.yRot)
	{
	case FACING_NORTH:
		if (((lara->pos.zPos + 0x80) & ~0x3FF) == (lara->pos.zPos & ~0x3FF))
			lara->pos.zPos += 0x80;
		lara->pos.zPos = (lara->pos.zPos & ~0xFF) + 155;
		legLeft = (lara->pos.zPos & 0x100)? false : true;
		break;

	case FACING_SOUTH:
		if (((lara->pos.zPos-0x80) & ~0x3FF) == (lara->pos.zPos & ~0x3FF))
			lara->pos.zPos -= 0x80;
		lara->pos.zPos = (lara->pos.zPos & ~0xFF) + 101;
		legLeft = (lara->pos.zPos & 0x100)? true : false;
		break;

	case FACING_WEST:
		if (((lara->pos.xPos - 0x80) & ~0x3FF) == (lara->pos.xPos & ~0x3FF))
			lara->pos.xPos -= 0x80;
		lara->pos.xPos = (lara->pos.xPos & ~0xFF) + 101;
		legLeft = (lara->pos.xPos & 0x100)? true : false;
		break;

	case FACING_EAST:
		if (((lara->pos.xPos + 0x80) & ~0x3FF) == (lara->pos.xPos & ~0x3FF))
			lara->pos.xPos += 0x80;
		lara->pos.xPos = (lara->pos.xPos & ~0xFF) + 155;
		legLeft = (lara->pos.xPos & 0x100)? false : true;
		break;
	}
}

/* find a bridge item with given params (tilt, facing, pos, min and max ceiling heights) */
short FindBridge(int tiltGrade, short facing, phd_vector &pos, int *returnHeight, int minCeilY = 0, int maxCeilY = 0)
{
	short bridgeSlot;

	switch(tiltGrade)
	{
	case 0:
		bridgeSlot = enumSLOT.BRIDGE_FLAT;
		break;
	case 1:
		bridgeSlot = enumSLOT.BRIDGE_TILT1;
		break;
	case 2:
		bridgeSlot = enumSLOT.BRIDGE_TILT2;
		break;
	case 3:
		bridgeSlot = enumSLOT.BRIDGE_TILT3;
		break;
	case 4:
		bridgeSlot = enumSLOT.BRIDGE_TILT4;
		break;
	default:
		bridgeSlot = enumSLOT.BRIDGE_CUSTOM;
		break;
	}

	Tr4ObjectInfo *slot = (Tr4ObjectInfo*) Trng.pGlobTomb4->pAdr->pVetSlot;
	Tr4ItemInfo *items = (Tr4ItemInfo*) Trng.pGlobTomb4->pAdr->pVetItems;

	int itemsMax = Trng.pGlobTomb4->pAdr->TotItemsMax;

	int xmin = pos.x & ~0x3FF;
	int xmax = xmin + 0x3FF;
	int zmin = pos.z & ~0x3FF;
	int zmax = zmin + 0x3FF;

	for (int i = 0; i < itemsMax; i++)
	{
		short itemSlot = items[i].slot_number;

		if (itemSlot == bridgeSlot)
		{
			short facingDiff = (short)(items[i].pos.yRot - facing);

			bool facingCheck = false;
			if (facingDiff == 0x4000)
				facingCheck = true;
			else if (itemSlot == SLOT_BRIDGE_FLAT)
				facingCheck = true;
			else if ((itemSlot == SLOT_BRIDGE_TILT1 || itemSlot == SLOT_BRIDGE_TILT2) && abs(facingDiff) == 0x4000)
				facingCheck = true;

			if (facingCheck)
			{
				if (items[i].pos.xPos >= xmin && items[i].pos.xPos <= xmax && items[i].pos.zPos >= zmin && items[i].pos.zPos <= zmax)
				{
					if (minCeilY || maxCeilY)
					{
						TYPE_FloorProc ceilProc = (TYPE_FloorProc) slot[itemSlot].CeilingProc;
						if (ceilProc)
						{
							ceilProc(&items[i], pos.x, pos.y, pos.z, returnHeight);

							int ceilingDist = *returnHeight - pos.y;
							if (ceilingDist >= minCeilY && ceilingDist <= maxCeilY)
								return i;
						}
					}
					else
						return i;
				}
			}
		}
	}

	return NO_ITEM;
}

/* Get goal parameters (climb wall orient, goal slope tilts,
goal alignment orient, displacement from current Lara pos)
Not very proud of this... but at least it's not global vars :) */
void FillSlopeData(short orient, SlopeTilts &goal, short &climbOrient, short &goalOrient, phd_vector &pos)
{
	int quadrant = (unsigned short)(orient + 0x2000) / 0x4000;

	switch (quadrant)
	{
	case 0:
		climbOrient = CLIMB_EAST_WALL;
		goalOrient = 0;
		goal.tiltZ = 4;
		pos.z = ONE_STEP;
		break;
	case 1:
		climbOrient = CLIMB_SOUTH_WALL;
		goalOrient = 16384;
		goal.tiltX = -4;
		pos.x = ONE_STEP;
		break;
	case 2:
		climbOrient = CLIMB_WEST_WALL;
		goalOrient = -32768;
		goal.tiltZ = -4;
		pos.z = -ONE_STEP;
		break;
	case 3:
		climbOrient = CLIMB_NORTH_WALL;
		goalOrient = -16384;
		goal.tiltX = 4;
		pos.x = -ONE_STEP;
		break;
	}
}


// ************  Climbing logic (control & collision routines) ************* //

void lara_col_slopeclimb(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	GetLaraCollisionInfo(lara, coll);

	phd_vector now = {lara->pos.xPos, lara->pos.yPos, lara->pos.zPos};
	phd_vector offset = {0, 0, 0};
	SlopeTilts slope = {0, 0};
	SlopeTilts goal = {0, 0};
	short climbOrient = 0;
	short goalOrient = 0;

	FillSlopeData(lara->pos.yRot, goal, climbOrient, goalOrient, offset);
	phd_vector up = {lara->pos.xPos - offset.x, lara->pos.yPos - ONE_STEP, lara->pos.zPos - offset.z};
	phd_vector down = {lara->pos.xPos + offset.x, lara->pos.yPos + ONE_STEP, lara->pos.zPos + offset.z};

	short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
	int overhangAnim = GetOverhangAnim();
	short tempRoom = 0;

	if (overhangAnim == ladder_slope_concave)
		return;

	Tr4FloorInfo *floorNow = (Tr4FloorInfo*) GetFloor(now.x, now.y, now.z, &(tempRoom = lara->room_number));
	int ceiling = GetCeiling(floorNow, now.x, now.y, now.z);
	lara->pos.yPos = ceiling + HITE_ADJUST;

	// drop down if action not pressed
	if (!(input & CMD_ACTION))
	{
		SetOverhangAnim(overhangAnim == idle_left ? drop_left : drop_right);
		return;
	}

	// engage shimmy mode if left (sidestep) or right (sidestep) key is pressed
	if (input & CMD_LEFT || input & CMD_RIGHT )
	{
		MyData.Save.Local.leftLegUp = (overhangAnim == idle_left) ? true : false;
		SetOverhangAnim(overhangAnim == idle_left ? idle_2_hang_left : idle_2_hang_right);
		return;
	}

	if (input & CMD_UP) // UP key pressed
	{
		// get floor_info 256 upstream of lara
		Tr4FloorInfo *floorNext = (Tr4FloorInfo*) GetFloor(up.x, up.y, up.z, &(tempRoom = lara->room_number));

		// test for ledge over slope
		tempRoom = floorNext->sky_room;
		if (tempRoom != 0xFF)
		{
			short oldRoomCam = Trng.pGlobTomb4->pAdr->Camera.pCameraSrc->Room;
			short oldRoomTarg = Trng.pGlobTomb4->pAdr->Camera.pCameraTarget->Room;

			Tr4FloorInfo *testLedge = (Tr4FloorInfo*) GetFloor(now.x, now.y - 3*ONE_STEP, now.z, &tempRoom);
			int ledgeCeiling = GetCeiling(testLedge, now.x, now.y - 3*ONE_STEP, now.z);
			int ledgeHeight = GetHeight(testLedge, now.x, now.y - 3*ONE_STEP, now.z);
			if ((ledgeHeight - ledgeCeiling >= 3*ONE_STEP) && abs((lara->pos.yPos-688) - ledgeHeight) < 64)
			{
				AlignToEdge(lara, 868);
				SetOverhangAnim(ledge_vault_start); // ledge climb-up from slope
			}
		}

		// test for slope to overhead ladder transition (convex)
		if (GetClimbStatus(floorNext, up.x, up.z) & climbOrient &&
			InStrip(lara->pos.xPos, lara->pos.zPos, lara->pos.yRot, 3*ONE_STEP, 4*ONE_STEP))
		{
			if (TestWall(lara, 0, 0, -4*ONE_STEP) &&
				GetCeiling(floorNext, up.x, up.y, up.z) - lara->pos.yPos <= 1456)  // check if a wall is actually there...
			{
				AlignToEdge(lara, 868);
				SetOverhangAnim(slope_ladder_convex_start); 
			}
		}

		// test for monkey at next position
		if (TestMonkey(floorNext, up.x, up.z))
		{
			int yDiff = GetCeiling(floorNext, up.x, up.y, up.z) - ceiling;

			slope = GetCeilingTilts(floorNext, up.x, up.z);

			int height; // height variable for bridge ceiling functions

			// test for upwards slope to climb
			short bridge1 = FindBridge(4, lara->pos.yRot, up, &height, -5*ONE_STEP/2, -3*ONE_STEP/2);
			if (yDiff >= -5*ONE_STEP/4 && yDiff <= -3*ONE_STEP/4 && (SLOPE_CHECK || bridge1 >= 0))
			{
				// do one more check for wall/ceiling step 2*offX/Z further to avoid lara sinking her head in wall/step
				Tr4FloorInfo *testWall = (Tr4FloorInfo*) GetFloor((up.x - offset.x), (up.y - ONE_STEP), (up.z - offset.z),
											&(tempRoom = lara->room_number));
				int testCeiling = GetCeiling(testWall, (up.x - offset.x), (up.y - ONE_STEP), (up.z - offset.z));

				if ((testWall->floor != -127) && (ceiling - testCeiling) > 0x40) // no wall or downwards ceiling step
				{
					TranslateItem(lara, 0, -256, -256);
					SetOverhangAnim(overhangAnim == idle_left ? climb_up_left : climb_up_right);
					//lara->goal_state = 62;
				}
			}

			// test for flat monkey (abs(slope) < 2)
			bridge1 = FindBridge(0, goalOrient, up, &height, -9*ONE_STEP/4, -5*ONE_STEP/4);
			if (bridge1 < 0)
				bridge1 = FindBridge(1, goalOrient, up, &height, -9*ONE_STEP/4, -5*ONE_STEP/4);

			if (yDiff > -ONE_STEP && yDiff <= -ONE_STEP/2 &&
				((abs(slope.tiltX) <= 2 && abs(slope.tiltZ) <= 2) || bridge1 >= 0 ))
				SetOverhangAnim(slope_monkey_concave); // slope to overhead monkey transition (concave)
		}
	}
	else if (input & CMD_DOWN)
	{
		// get floor_info 256 downstream of Lara
		Tr4FloorInfo *floorNext = (Tr4FloorInfo*) GetFloor(down.x, down.y, down.z, &(tempRoom = lara->room_number));

		if ((GetClimbStatus(floorNow, lara->pos.xPos, lara->pos.zPos) & climbOrient) &&
			InStrip(lara->pos.xPos, lara->pos.zPos, lara->pos.yRot, 0, ONE_STEP))
		{
			AlignToEdge(lara, 100);
			SetOverhangAnim(slope_ladder_concave); // slope to underlying ladder transition (concave)
			return;
		}

		if (TestMonkey(floorNext, down.x, down.z))
		{
			slope = GetCeilingTilts(floorNext, down.x, down.z);

			int height;
			int yDiff = GetCeiling(floorNext, down.x, down.y, down.z) - ceiling;

			// test for flat monkey (abs(slope) < 2)
			short bridge1 = FindBridge(0, goalOrient, down, &height, -3*ONE_STEP, -2*ONE_STEP);
			if (bridge1 < 0)
				bridge1 = FindBridge(1, goalOrient, down, &height, -3*ONE_STEP, -2*ONE_STEP);

			if ((abs(yDiff) < ONE_STEP && abs(slope.tiltX) <= 2 && abs(slope.tiltZ) <= 2) || bridge1 >= 0)
				SetOverhangAnim(slope_monkey_convex); // Force slope to underlying monkey transition (convex)
			
			// test for downwards slope to climb
			bridge1 = FindBridge(4, goalOrient, down, &height, -5*ONE_STEP/2, -3*ONE_STEP/2);
			if (yDiff >= 3*ONE_STEP/4 && yDiff <= 5*ONE_STEP/4 && (SLOPE_CHECK || bridge1 >= 0))
			{
				SetOverhangAnim(overhangAnim == idle_left ? climb_down_left : climb_down_right);
				return;
			}
		}
	}
}


void lara_as_slopeclimb(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	CAMERA_INFO* camera = (CAMERA_INFO*)TR4_CAMERA_ADR;

	if (*Trng.pGlobTomb4->pAdr->pFrameCounter % 2)
		lara->pos.xRot--;
	else
		lara->pos.xRot++;

	camera->flags = 1;

	if (camera->type != CHASE_CAMERA)
		return;

	camera->target_elevation = -3072;
	camera->target_distance = 1792;
	camera->speed = 15;
}


void lara_as_slopefall(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	lara->flagsMain |= FITEM_GRAVITY_AFFECTED;

	CAMERA_INFO* camera = (CAMERA_INFO*)TR4_CAMERA_ADR;

	if (*Trng.pGlobTomb4->pAdr->pFrameCounter % 2)
		lara->pos.xRot--;
	else
		lara->pos.xRot++;

	camera->flags = 1;

	if (camera->type != CHASE_CAMERA)
		return;

	camera->target_elevation = -3072;
	camera->target_distance = 1792;
	camera->speed = 15;

}


void lara_col_slopehang(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	GetLaraCollisionInfo(lara, coll);

	phd_vector now = {lara->pos.xPos, lara->pos.yPos, lara->pos.zPos};
	phd_vector offset = {0, 0, 0};
	SlopeTilts slope = {0, 0};
	SlopeTilts goal = {0, 0};
	short climbOrient = 0;
	short goalOrient = 0;

	FillSlopeData(lara->pos.yRot, goal, climbOrient, goalOrient, offset);

	short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
	int overhangAnim = GetOverhangAnim();
	short tempRoom = 0;

	Tr4FloorInfo *floorNow = (Tr4FloorInfo*) GetFloor(now.x, now.y, now.z, &(tempRoom = lara->room_number));
	int ceiling = GetCeiling(floorNow, now.x, now.y, now.z);
	lara->pos.yPos = ceiling + HITE_ADJUST;

	// drop down if action not pressed 
	if (!(input & CMD_ACTION))
	{
		SetOverhangAnim(hang_drop);
		return;
	}

	if (overhangAnim != hang_swing)
	{
		// return to climbing mode
		if (input & CMD_UP || input & CMD_DOWN)
			SetOverhangAnim(MyData.Save.Local.leftLegUp ? hang_2_idle_left : hang_2_idle_right);

		// shimmy control
		if (input & CMD_LEFT || input & CMD_RIGHT)
		{
			phd_vector shimmy = {now.x, now.y, now.z};
			short dir = 0;
			if (input & CMD_LEFT)
			{
				shimmy.x -= offset.z/2;
				shimmy.z += offset.x/2;
				dir = -16384;
			}
			else if (input & CMD_RIGHT)
			{
				shimmy.x += offset.z/2;
				shimmy.z -= offset.x/2;
				dir = 16384;
			}

			Tr4FloorInfo *floorNext = (Tr4FloorInfo*) GetFloor(shimmy.x, shimmy.y, shimmy.z, &(tempRoom = lara->room_number));

			if (TestMonkey(floorNext, shimmy.x, shimmy.z))
			{
				slope = GetCeilingTilts(floorNext, shimmy.x, shimmy.z);

				int yDiff = GetCeiling(floorNext, shimmy.x, shimmy.y, shimmy.z) - ceiling;

				int height;
				short bridge1 = FindBridge(4, goalOrient, shimmy, &height, -5*ONE_STEP/2, -3*ONE_STEP/2);

				if ((SLOPE_CHECK && abs(yDiff) < 64) || bridge1 >= 0)
					SetOverhangAnim(dir < 0 ? shimmy_left : shimmy_right);
			}
		}
	}
	
}


void lara_as_slopehang(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	CAMERA_INFO* camera = (CAMERA_INFO*)TR4_CAMERA_ADR;

	if (*Trng.pGlobTomb4->pAdr->pFrameCounter % 2)
		lara->pos.xRot--;
	else
		lara->pos.xRot++;

	if (camera->type != CHASE_CAMERA)
		return;

	camera->target_elevation = -1024;
	camera->target_distance = 1664;
	camera->speed = 15;
}


void lara_col_slopeshimmy(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	GetLaraCollisionInfo(lara, coll);

	phd_vector now = {lara->pos.xPos, lara->pos.yPos, lara->pos.zPos};
	phd_vector offset = {0, 0, 0};
	SlopeTilts slope = {0, 0};
	SlopeTilts goal = {0, 0};
	short climbOrient = 0;
	short goalOrient = 0;

	FillSlopeData(lara->pos.yRot, goal, climbOrient, goalOrient, offset);

	short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
	int overhangAnim = GetOverhangAnim();
	short tempRoom = 0;

	Tr4FloorInfo *floorNow = (Tr4FloorInfo*) GetFloor(now.x, now.y, now.z, &(tempRoom = lara->room_number));
	int ceiling = GetCeiling(floorNow, now.x, now.y, now.z);
	lara->pos.yPos = ceiling + HITE_ADJUST;

	phd_vector shimmy = {lara->pos.xPos, lara->pos.yPos, lara->pos.zPos};
	if (overhangAnim == shimmy_left)
	{
		shimmy.x -= offset.z / 2;
		shimmy.z += offset.x / 2;
	}
	else
	{
		shimmy.x += offset.z / 2;
		shimmy.z -= offset.x / 2;
	}

	Tr4FloorInfo *floorNext = (Tr4FloorInfo*) GetFloor(shimmy.x, shimmy.y, shimmy.z, &(tempRoom = lara->room_number));

	bool cancelShimmy = true;
	if (TestMonkey(floorNext, shimmy.x, shimmy.z))
	{
		slope = GetCeilingTilts(floorNext, shimmy.x, shimmy.z);

		int yDiff = GetCeiling(floorNext, shimmy.x, shimmy.y, shimmy.z) - ceiling;

		int height;
		short bridge1 = FindBridge(4, goalOrient, shimmy, &height, -5*ONE_STEP/2, -3*ONE_STEP/2);

		if ((SLOPE_CHECK && abs(yDiff) < 64) || bridge1 >= 0)
			cancelShimmy = false;
	}

	if (cancelShimmy)
		SetOverhangAnim((overhangAnim == shimmy_left) ? shimmy_left_stop : shimmy_right_stop);
}

void lara_as_slopeshimmy(Tr4ItemInfo *lara, Tr4CollInfo* coll)
{
	CAMERA_INFO* camera = (CAMERA_INFO*)TR4_CAMERA_ADR;

	if (*Trng.pGlobTomb4->pAdr->pFrameCounter % 2)
		lara->pos.xRot--;
	else
		lara->pos.xRot++;

	if (camera->type != CHASE_CAMERA)
		return;

	camera->target_elevation = -1024;
	camera->target_distance = 1664;
	camera->speed = 15;
	
	short *moveAngle = (short*) 0x80DF64; // lara_info.move_angle
	if (GetOverhangAnim() == shimmy_left)
	{
		*moveAngle = lara->pos.yRot - 16384;
		camera->target_angle = -4096;
	}
	else
	{
		*moveAngle = lara->pos.yRot + 16384;
		camera->target_angle = 4096;	
	}
}


void lara_as_slopeclimbup(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	CAMERA_INFO* camera = (CAMERA_INFO*)TR4_CAMERA_ADR;

	if (*Trng.pGlobTomb4->pAdr->pFrameCounter % 2)
		lara->pos.xRot--;
	else
		lara->pos.xRot++;

	camera->flags = 1;

	if (camera->type != CHASE_CAMERA)
		return; // if camera mode isn't chase (0) then don't change camera angles

	camera->target_elevation = 2048;
	camera->target_distance = 1792;
	camera->speed = 15;

	short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
	Tr4AnimStruct *anim = (Tr4AnimStruct*) Trng.pGlobTomb4->pAdr->pVetAnimations;

	if (!(input & CMD_ACTION))
	{
		int frame = GetRelativeFrame();
		int length = anim[lara->anim_number].FrameEnd - anim[lara->anim_number].FrameBase;
		int dPos = ONE_STEP - (frame * ONE_STEP / length);

		TranslateItem(lara, 0, dPos, dPos);
		if (GetOverhangAnim() == climb_up_left)
			SetOverhangAnim(frame <= 2*length/3 ? drop_left : drop_right);
		else
			SetOverhangAnim(frame <= 2*length/3 ? drop_right : drop_left);
	} 
}


void lara_as_slopeclimbdown(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	CAMERA_INFO* camera = (CAMERA_INFO*)TR4_CAMERA_ADR;

	if (*Trng.pGlobTomb4->pAdr->pFrameCounter % 2)
		lara->pos.xRot--;
	else
		lara->pos.xRot++;

	camera->flags = 1;

	if (camera->type != CHASE_CAMERA)
		return;

	camera->target_elevation = -3072;
	camera->target_distance = 1664;
	camera->speed = 15;

	short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
	Tr4AnimStruct *anim = (Tr4AnimStruct*) Trng.pGlobTomb4->pAdr->pVetAnimations;

	if (!(input & CMD_ACTION))
	{
		int frame = GetRelativeFrame();
		int length = anim[lara->anim_number].FrameEnd - anim[lara->anim_number].FrameBase;
		int dPos = frame * ONE_STEP / length;

		TranslateItem(lara, 0, dPos, dPos);
		if (GetOverhangAnim() == climb_down_left)
			SetOverhangAnim(frame <= length/2 ? drop_left : drop_right);
		else
			SetOverhangAnim(frame <= length/2 ? drop_right : drop_left);
	} 
}


void lara_as_sclimbstart(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	CAMERA_INFO* camera = (CAMERA_INFO*)TR4_CAMERA_ADR;
	Tr4AnimStruct *anim = (Tr4AnimStruct*)Trng.pGlobTomb4->pAdr->pVetAnimations;

	int overhangAnim = GetOverhangAnim();
	// rotating camera effect during monkey to overhead slope transition
	if (overhangAnim == monkey_slope_convex)
	{
		int frame = GetRelativeFrame();
		float frac = 0.0f;
		int numFrames = anim[lara->anim_number].FrameEnd - anim[lara->anim_number].FrameBase;

		frac = (frame*1.5f) / (float)(numFrames);
		if (frac > 1.0f)
			frac = 1.0f;

		camera->flags = 1;

		int distance = 1664;
		if (TestWall(lara, 0, 1536, 0))
		{
			distance = 1024;
		}

		if (lara->frame_number < anim[lara->anim_number].FrameEnd)
		{
			*Trng.pGlobTomb4->pAdr->pSetCameraDistance = distance;
			*Trng.pGlobTomb4->pAdr->pSetCameraVOrient = int(3072 * frac);
			*Trng.pGlobTomb4->pAdr->pSetCameraHOrient = int(-32767 * frac);
			*Trng.pGlobTomb4->pAdr->pSetCameraSpeed = 15;
		}
		else
		{

			*Trng.pGlobTomb4->pAdr->pSetCameraDistance = distance;
			*Trng.pGlobTomb4->pAdr->pSetCameraVOrient = 3072;
			*Trng.pGlobTomb4->pAdr->pSetCameraHOrient = 0;
			*Trng.pGlobTomb4->pAdr->pSetCameraSpeed = 15;
		}
	}
	else// if (overhangAnim == monkey_slope_concave)
	{
		//camera->flags = 1;
		camera->target_elevation = -2048;
		camera->target_distance = 1664;
		camera->speed = 15;
	}

	if (*Trng.pGlobTomb4->pAdr->pFrameCounter % 2)
		lara->pos.xRot++;
	else
		lara->pos.xRot--;
}


void lara_as_sclimbstop(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	CAMERA_INFO* camera = (CAMERA_INFO*)TR4_CAMERA_ADR;
	Tr4AnimStruct *anim = (Tr4AnimStruct*)Trng.pGlobTomb4->pAdr->pVetAnimations;

	// rotating camera effect during monkey to overhead slope transition
	int overhangAnim = GetOverhangAnim();
	
	// following camera effect during the slope to underlying monkey transition
	if (overhangAnim == slope_monkey_convex)
	{
		camera->flags = 1;
		*Trng.pGlobTomb4->pAdr->pSetCameraDistance = 1664;
		*Trng.pGlobTomb4->pAdr->pSetCameraVOrient = 2048;
		*Trng.pGlobTomb4->pAdr->pSetCameraSpeed = 15;
	}
	// rotating camera effect during concave slope to monkey transition
	else if (overhangAnim == slope_monkey_concave)
	{
		int frame = GetRelativeFrame();
		int numFrames = anim[lara->anim_number].FrameEnd - anim[lara->anim_number].FrameBase;

		float frac = (frame*1.25f) / (float)(numFrames);
		if (frac > 1.0f)
			frac = 1.0f;

		camera->flags = 1;

		if (lara->frame_number < anim[lara->anim_number].FrameEnd)
		{
			*Trng.pGlobTomb4->pAdr->pSetCameraHOrient = int(-16384 * frac);
			*Trng.pGlobTomb4->pAdr->pSetCameraDistance = 1792 - int(512 * frac);
			*Trng.pGlobTomb4->pAdr->pSetCameraSpeed = 15;
		}
		else
		{
			*Trng.pGlobTomb4->pAdr->pSetCameraHOrient = 16384;
			*Trng.pGlobTomb4->pAdr->pSetCameraDistance = 1280;
			*Trng.pGlobTomb4->pAdr->pSetCameraSpeed = 15;
		}
	}
	else
	{
		*Trng.pGlobTomb4->pAdr->pSetCameraDistance = 1664;
		*Trng.pGlobTomb4->pAdr->pSetCameraVOrient = -2048;
		*Trng.pGlobTomb4->pAdr->pSetCameraSpeed = 15;
	}


	if (*Trng.pGlobTomb4->pAdr->pFrameCounter % 2)
		lara->pos.xRot++;
	else
		lara->pos.xRot--;
}


void lara_as_sclimbend(Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	switch (GetOverhangAnim())
	{
	case exit_monkey_forward:
		SetAnimLara(236);
		break;
	case exit_monkey_idle:
		SetAnimLara(234);
		break;
	case exit_ladder:
		SetAnimLara(164);
		break;
	case exit_vault:
		SetAnimLara(102);
		break;
	case exit_drop:
		SetAnimLara(35);
		break;
	case exit_hang:
		SetAnimLara(28);
		break;
	}

	lara->pos.xRot = 0;
}


// ********  Callback procedures for extending existing state routines  ********
// added via RequireMyCallbacks() (scroll down below)

/* extends state 10 (AS_HANG) */
void hang_slope_extra(WORD stateID, WORD CBTtype, Tr4ItemInfo *lara, Tr4CollInfo *col)
{
	if (MyData.overhangSlot == NO_ITEM)
		return;

	phd_vector offset = {0, 0, 0};
	SlopeTilts goal = {0, 0};
	short climbOrient = 0;
	short goalOrient = 0;

	FillSlopeData(lara->pos.yRot, goal, climbOrient, goalOrient, offset);

	short tempRoom = 0;
	phd_vector down = {lara->pos.xPos + offset.x, lara->pos.yPos + ONE_STEP, lara->pos.zPos + offset.z};
	Tr4FloorInfo *floorNext = (Tr4FloorInfo*) GetFloor(down.x, down.y, down.z, &(tempRoom = lara->room_number));
	int ceilDist = lara->pos.yPos - GetCeiling(floorNext, down.x, down.y, down.z);

	if (lara->goal_state == AS_CLIMBSTNC) // prevent going from hang to climb mode if slope is under ladder
	{
		if (ceilDist >= ONE_STEP && ceilDist < 2*ONE_STEP)
		{
			SlopeTilts slope = GetCeilingTilts(floorNext, down.x, down.z);

			if ((slope.tiltX / 3) == (goal.tiltX / 3) || (slope.tiltZ / 3) == (goal.tiltZ / 3))
			{
				lara->goal_state = AS_HANG;
				short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
				if (input & CMD_UP)
					SetAnimLara(187);
				/*else if (input & CMD_DOWN)
					SetAnimLara(188);*/
			}
		}
	}
	/*else if (lara->goal_state == AS_HANG)
	{
		if (lara->anim_number == 188)
		{
			if (ceilDist < ONE_STEP)
			{
				SlopeTilts slope = GetCeilingTilts(floorNext, down.x, down.z);

				if ((slope.tiltX / 3) == (goal.tiltX / 3) || (slope.tiltZ / 3) == (goal.tiltZ / 3))
					SetAnimLara(96, 21);
			}
		}
	}*/
}

/* extends state 11 (AS_REACH) */
void reach_slope_extra(WORD stateID, WORD CBType, Tr4ItemInfo *lara, Tr4CollInfo *col)
{
	if (MyData.overhangSlot == NO_ITEM)
		return;

	phd_vector now = {lara->pos.xPos, lara->pos.yPos, lara->pos.zPos};
	phd_vector offset = {0, 0, 0};
	SlopeTilts slope = {0, 0};
	SlopeTilts goal = {0, 0};
	short climbOrient = 0;
	short goalOrient = 0;

	FillSlopeData(lara->pos.yRot, goal, climbOrient, goalOrient, offset);
	phd_vector down = {lara->pos.xPos + offset.x, lara->pos.yPos + ONE_STEP, lara->pos.zPos + offset.z};

	short tempRoom = 0;

	Tr4FloorInfo *floorNow = (Tr4FloorInfo*) GetFloor(now.x, now.y, now.z, &(tempRoom = lara->room_number));
	int ceilDist = lara->pos.yPos - GetCeiling(floorNow, now.x, now.y, now.z);

	if (TestMonkey(floorNow, now.x, now.z) && ceilDist <= 7 * ONE_STEP/2)
	{
		
		slope = GetCeilingTilts(floorNow, now.x, now.z);
				
		int height;
		short bridge1 = FindBridge(4, goalOrient, now, &height, -4*ONE_STEP, -5*(ONE_STEP/2));

		if (abs(slope.tiltX) > 2 || abs(slope.tiltZ) > 2 || bridge1 >= 0)
		{
			bool disableGrab = true;
			if (SLOPE_CHECK || bridge1 >= 0)
			{
				if (abs(DirOrientDiff(lara->pos.yRot, goalOrient)) < 0x1800)
					disableGrab = false;
			}

			if (disableGrab)
				*Trng.pGlobTomb4->pAdr->pInputGameCommands &= ~CMD_ACTION;
		}
	}
}

/* extends state 56 (AS_CLIMBSTNC) */
void climb_slope_extra(WORD stateID, WORD CBTtype, Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	if (MyData.overhangSlot == NO_ITEM)
		return;

	phd_vector now = {lara->pos.xPos, lara->pos.yPos, lara->pos.zPos};
	phd_vector offset = {0, 0, 0};
	SlopeTilts slope = {0, 0};
	SlopeTilts goal = {0, 0};
	short climbOrient = 0;
	short goalOrient = 0;

	FillSlopeData(lara->pos.yRot, goal, climbOrient, goalOrient, offset);
	phd_vector down = {lara->pos.xPos + offset.x, lara->pos.yPos + ONE_STEP, lara->pos.zPos + offset.z};

	short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
	short tempRoom = 0;

	Tr4FloorInfo *floorNow = (Tr4FloorInfo*) GetFloor(now.x, now.y, now.z, &(tempRoom = lara->room_number));
	int ceiling = GetCeiling(floorNow, now.x, now.y, now.z);

	// block for ladder to overhead slope transition
	if (lara->anim_number == 164)
	{
		if (input & CMD_UP)
		{
			int ceiling = GetCeiling(floorNow, now.x, now.y, now.z);
			int ceilDist = ceiling - lara->pos.yPos;

			if (TestMonkey(floorNow, now.x, now.z) && ceilDist >= -4*ONE_STEP && ceilDist <= -3*ONE_STEP)
			{
				slope = GetCeilingTilts(floorNow, lara->pos.xPos, lara->pos.zPos);

				int height;

				short facing = lara->pos.yRot + 0x2000;
				facing &= 0xC000;

				short bridge1 = FindBridge(4, facing, now, &height, -4*ONE_STEP, -3*ONE_STEP);

				if (SLOPE_CHECK || bridge1 >= 0)
				{
					lara->pos.yPos = ceiling + 900;
					SetOverhangAnim(ladder_slope_concave); // ladder to overhead slope transition (concave)
				}
			}
		}

		if (input & CMD_DOWN)
		{
			Tr4FloorInfo *floorNext = (Tr4FloorInfo*) GetFloor(down.x, down.y, down.z, &(tempRoom = lara->room_number));
			int ceiling = GetCeiling(floorNext, down.x, down.y, down.z);
			int ceilDist = ceiling - lara->pos.yPos;

			if (TestMonkey(floorNext, down.x, down.z) && ceilDist >= 0 && ceilDist <= ONE_STEP)
			{
				slope = GetCeilingTilts(floorNext, down.x, down.z);

				int height;

				short facing = lara->pos.yRot + 0x2000;
				facing &= 0xC000;

				short bridge1 = FindBridge(4, facing, down, &height, -ONE_STEP/2, -ONE_STEP/4);

				if (SLOPE_CHECK || bridge1 >= 0)
				{
					lara->pos.yPos = ceiling - 156;
					SetOverhangAnim(ladder_slope_convex); // ladder to underlying slope transition (convex)
				}
			}
		}
	}
}

/* extends state 61 (AS_CLIMBDOWN) */
void climbdown_slope_extra(WORD stateID, WORD CBType, Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	if (MyData.overhangSlot == NO_ITEM)
		return;

	phd_vector now = {lara->pos.xPos, lara->pos.yPos, lara->pos.zPos};
	phd_vector offset = {0, 0, 0};
	SlopeTilts slope = {0, 0};
	SlopeTilts goal = {0, 0};
	short climbOrient = 0;
	short goalOrient = 0;

	FillSlopeData(lara->pos.yRot, goal, climbOrient, goalOrient, offset);
	phd_vector down = {lara->pos.xPos + offset.x, lara->pos.yPos + ONE_STEP, lara->pos.zPos + offset.z};

	short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
	short tempRoom = 0;

	Tr4FloorInfo *floorNow = (Tr4FloorInfo*) GetFloor(now.x, now.y, now.z, &(tempRoom = lara->room_number));
	int ceiling = GetCeiling(floorNow, now.x, now.y, now.z);

	if (lara->anim_number == 168) // make lara stop before underlying slope ceiling at correct height
	{
		if (input & CMD_DOWN)
		{
			Tr4FloorInfo *floorNext = (Tr4FloorInfo*) GetFloor(down.x, down.y, down.z, &(tempRoom = lara->room_number));
			int ceiling = GetCeiling(floorNext, down.x, down.y, down.z);
			int ceilDist = ceiling - lara->pos.yPos;

			if (TestMonkey(floorNext, down.x, down.z))
			{
				slope = GetCeilingTilts(floorNext, down.x, down.z);
				BYTE midpoint = *(BYTE*)0x42D24A; // retrieve midpoint frame (low) for climbing down
				//down.y += 256;
				int height;
				if (!GetRelativeFrame())
				{
					short bridge1 = FindBridge(4, goalOrient, down, &height, -ONE_STEP*3, ONE_STEP*4);
					if (ceilDist < ONE_STEP && ( bridge1 >= 0 || SLOPE_CHECK))
						lara->goal_state = 56;
				}
				else if (GetRelativeFrame() == midpoint)
				{
					short bridge1 = FindBridge(4, goalOrient, down, &height, -ONE_STEP*2, ONE_STEP*5);
					if (ceilDist < ONE_STEP*2 && ( bridge1 >= 0 || SLOPE_CHECK))
					{
						lara->pos.yPos += ONE_STEP; // do midpoint Y translation
						lara->goal_state = 56;
					}
				}
			}
		}
	}
}


/* extends state 75 (AS_HANG2) */
void monkey_slope_extra(WORD stateID, WORD CBType, Tr4ItemInfo *lara, Tr4CollInfo *coll)
{
	if (MyData.overhangSlot == NO_ITEM)
		return;

	phd_vector now = {lara->pos.xPos, lara->pos.yPos, lara->pos.zPos};
	phd_vector offset = {0, 0, 0};
	SlopeTilts slope = {0, 0};
	SlopeTilts goal = {0, 0};
	short climbOrient = 0;
	short goalOrient = 0;

	FillSlopeData(lara->pos.yRot, goal, climbOrient, goalOrient, offset);
	phd_vector down = {lara->pos.xPos + offset.x, lara->pos.yPos + ONE_STEP, lara->pos.zPos + offset.z};

	short input = *Trng.pGlobTomb4->pAdr->pInputGameCommands;
	short tempRoom = 0;

	Tr4FloorInfo *floorNow = (Tr4FloorInfo*) GetFloor(now.x, now.y, now.z, &(tempRoom = lara->room_number));
	int ceiling = GetCeiling(floorNow, now.x, now.y, now.z);

	if (lara->anim_number == 150 && !GetRelativeFrame()) // manage proper grabbing of monkey slope on forward jump
	{
		int ceiling = GetCeiling(floorNow, now.x, now.y, now.z);
		int ceilDist = lara->pos.yPos - ceiling;

		if (TestMonkey(floorNow, now.x, now.z) && ceilDist <= 7*ONE_STEP/2)
		{
			slope = GetCeilingTilts(floorNow, now.x, now.z);

			int height;

			short facing = lara->pos.yRot + 0x2000;
			facing &= 0xC000;

			short bridge1 = FindBridge(4, facing, now, &height, -7*ONE_STEP/2, -5*ONE_STEP/2);

			if (SLOPE_CHECK || bridge1 >= 0)
			{
				AlignToGrab(lara, MyData.Save.Local.leftLegUp);

				int ceiling2 = GetCeiling(floorNow, lara->pos.xPos, lara->pos.yPos, lara->pos.zPos);
				lara->pos.yPos = ceiling2 + HITE_ADJUST;

				SetOverhangAnim(hang_swing);
			}
		}
	}

	if (input & CMD_UP) // monkey to slope transitions
	{
		if (TestMonkey(floorNow, now.x, now.z) && ((lara->anim_number == 150 && GetRelativeFrame() >= 54) || lara->anim_number == 234))
		{
			if (abs(DirOrientDiff(goalOrient, lara->pos.yRot)) <= 0x1400 &&
				InStrip(lara->pos.xPos, lara->pos.zPos, lara->pos.yRot, 0, ONE_STEP/2))
			{
				Tr4FloorInfo *floorNext = (Tr4FloorInfo*) GetFloor(down.x, down.y, down.z, &(tempRoom = lara->room_number));

				if (TestMonkey(floorNext, down.x, down.z))
				{
					int ceiling = GetCeiling(floorNext, down.x, now.y, down.z);
					int yDiff = ceiling - GetCeiling(floorNow, now.x, now.y, now.z);

					slope = GetCeilingTilts(floorNext, down.x, down.z);

					int height;

					short bridge1 = FindBridge(4, goalOrient, down, &height, -7*ONE_STEP>>1, -5*ONE_STEP>>1);
					if ((SLOPE_CHECK && yDiff > 0 && yDiff < ONE_STEP) || bridge1 >= 0)
					{
						AlignToEdge(lara, 154);
						SetOverhangAnim(monkey_slope_concave); // transition from monkey to underlying slope (concave)
						//lara->pos.yPos = ceiling + 496;
						PerformFlipeffect(NULL, 51, 1, 2); // disable the UP key command for 2 sec
					}

					bridge1 = FindBridge(4, goalOrient+0x8000, down, &height, -5*ONE_STEP, -4*ONE_STEP);
					if ((SLOPE_INVERSE_CHECK && yDiff > -ONE_STEP && yDiff < 0) || bridge1 >= 0)
					{
						AlignToEdge(lara, 154);
						SetOverhangAnim(monkey_slope_convex); // transition from monkey to overhanging slope (convex)
						//lara->pos.yPos = ceiling + 914;
					}
				}
			}
		}
	}
}



// ************  CallBack functions section  ************

void cbInitProgram(int NumberLoadedPlugins, char *VetPluginNames[]) 
{

	// save number of plugins (enclosed yours and the tomb_nextgeneration.dll ) and their names
	// these data will be used to locate IDs of any loaded plugins using FindPluginID() function
	Trng.TotPlugins = NumberLoadedPlugins;
	Trng.pVetPluginNames = VetPluginNames;

	// clear all my global variables
	ClearMemory(&MyData, sizeof(StrMyData));
}


void cbInitGame(void)
{
	/* add lara control and collision routines for states:
	130, 131, 132, 133, 134, 135, 136, 137, 138 */
		
	// new control routines
	ReplaceLaraCtrlRoutine(AS_SCLIMB_IDLE, lara_as_slopeclimb);		// 130
	ReplaceLaraCtrlRoutine(AS_SCLIMB_UP, lara_as_slopeclimbup);		// 131
	ReplaceLaraCtrlRoutine(AS_SCLIMB_DOWN, lara_as_slopeclimbdown);	// 132
	ReplaceLaraCtrlRoutine(AS_SCLIMB_FALL, lara_as_slopefall);		// 133
	ReplaceLaraCtrlRoutine(AS_SCLIMB_HANG, lara_as_slopehang);		// 134
	ReplaceLaraCtrlRoutine(AS_SCLIMB_SHIMMY, lara_as_slopeshimmy);	// 135
	ReplaceLaraCtrlRoutine(AS_SCLIMB_START, lara_as_sclimbstart);	// 136
	ReplaceLaraCtrlRoutine(AS_SCLIMB_STOP, lara_as_sclimbstop);		// 137
	ReplaceLaraCtrlRoutine(AS_SCLIMB_END, lara_as_sclimbend);		// 138

	// new collision routines
	ReplaceLaraColRoutine(AS_SCLIMB_IDLE, lara_col_slopeclimb);		// 130
	ReplaceLaraColRoutine(AS_SCLIMB_UP, lara_default_col);			// 131
	ReplaceLaraColRoutine(AS_SCLIMB_DOWN, lara_default_col);		// 132
	ReplaceLaraColRoutine(AS_SCLIMB_FALL, lara_default_col);		// 133
	ReplaceLaraColRoutine(AS_SCLIMB_HANG, lara_col_slopehang);		// 134
	ReplaceLaraColRoutine(AS_SCLIMB_SHIMMY, lara_col_slopeshimmy);	// 135
	ReplaceLaraColRoutine(AS_SCLIMB_START, lara_default_col);		// 136
	ReplaceLaraColRoutine(AS_SCLIMB_STOP, lara_default_col);		// 137
	ReplaceLaraColRoutine(AS_SCLIMB_END, lara_default_col);			// 138
}


void cbInitLevel(void)
{
	/* check if OBJ_LARA_FREECLIMB has been defined to a particular slot
	with AssignSlot= script conmmand. If yes, set MyData.overhangSlot to
	that slot num*/

	// initially set to -1 (NO_ITEM)
	MyData.overhangSlot = NO_ITEM;

	// check if the extra object was assigned in level script
	for (int i = 0; i < MyData.BaseAssignSlotMine.TotAssign; i++)
	{
		if (MyData.BaseAssignSlotMine.VetAssignSlot[i].TipoSlot == OBJ_LARA_OVERHANG_CLIMB)
			MyData.overhangSlot = MyData.BaseAssignSlotMine.VetAssignSlot[i].MioSlot; // found it!
	}
}


// called everytime player save the game (but also when lara move from a level to another HUB saving). 
// in this procedure your plugin will save its own data in savegame
// SavingType inform about the kind of saving it is happening (SAVT_... values)
// pAdrZone is a byte pointer where this procedure will save the address from where begin data to save in savegame
// this function will return the size of data to save
// note: It's better you don't change anything of this code. It will save byself of data you stored in MyData.Save
// structure. If you wish adding other variables to save, just you add new fields in Structures of MyData.Save 
DWORD cbSaveMyData(BYTE **pAdrZone, int SavingType)
{
	DWORD SizeData;
	int i;
	static WORD *pVetExtras;
	int TotNWords;
	int TotNewActions;


	if (SavingType & SAVT_COMPLETED) {
		// this call is not to save data but only it is a confirm that the previous saving has been completed
		// now we can free the temporary memory used to save the data in previous call
		if (pVetExtras != NULL) {
			FreeMemory(pVetExtras);
			pVetExtras=NULL;
		}

		return 0;
	}


	TotNWords=0;
	pVetExtras = (WORD *) GetMemory(16);
	// save id of my plugin in first word

	pVetExtras[TotNWords++] = Trng.IdMyPlugin;

	if (SavingType & SAVT_LOCAL_DATA) {
		// save local data

		// save Local structure
		AddNGToken(NGTAG_LOCAL_DATA, NO_ARRAY, sizeof(StrSavegameLocalData), &MyData.Save.Local, 
						&pVetExtras, &TotNWords);


		// save all (currently enabled) progressive actions
		// before saving, compact progressive action array to remove intermediate free records
		TotNewActions=0;

		for (i=0;i<MyData.TotProgrActions;i++) {
			if (MyData.VetProgrActions[i].ActionType != AXN_FREE) {

				MyData.VetProgrActions[TotNewActions] = MyData.VetProgrActions[i];

				TotNewActions++;
			}
		}
		// update new valuese after recompatting
		MyData.LastProgrActionIndex =0;
		MyData.TotProgrActions= TotNewActions;

		// store all progressive action records
		AddNGToken(NGTAG_PROGRESSIVE_ACTIONS, MyData.TotProgrActions, sizeof(StrProgressiveAction), 
				&MyData.VetProgrActions[0], &pVetExtras, &TotNWords);

	}

	if (SavingType & SAVT_GLOBAL_DATA) {
		// save global data
		AddNGToken(NGTAG_GLOBAL_DATA, NO_ARRAY, sizeof(StrSavegameGlobalData), &MyData.Save.Global , 
						&pVetExtras, &TotNWords);
	}
	// write final sequence
	AddTokenFinalSequence(&pVetExtras, &TotNWords);

	// return to trng the infos about start of memory where there are our data and their size:
	*pAdrZone = (BYTE *) pVetExtras;
	SizeData = TotNWords * 2;

	return SizeData;
	
}

// called when a savegame will be loaded (but also when lara move from a level to another)
// pAdrZone will point to memory zone with data just loaded from savegame
// SizeData is the size of data pointed by pAdrZone
// note: it's better you don't change anything of this code. It's alread thought to reload all data you saved in MyData.Save 
// structure. There is no need of changes by you
void cbLoadMyData(BYTE *pAdrZone, DWORD SizeData)
{

	WORD *pVetExtras;
	StrParseNGField  ParseField;
	int Indice;
	int i;
	WORD TotActions;
	
	pVetExtras = (WORD*) pAdrZone;

	Indice=0;

	while (ParseNgField(pVetExtras ,Indice, &ParseField)==true) {
		
		// recover different ng token
		switch (ParseField.Type) {
		case NGTAG_LOCAL_DATA:
			// local data
			memcpy(&MyData.Save.Local, ParseField.pData, sizeof(StrSavegameLocalData));
			break;

		case NGTAG_GLOBAL_DATA:
			// global data
			memcpy(&MyData.Save.Global, ParseField.pData, sizeof(StrSavegameGlobalData));
			break;
			
		case NGTAG_PROGRESSIVE_ACTIONS:
			// progressive actions
			i= ParseField.StartDataIndex;
			// read tot actions value
			TotActions = pVetExtras[i++];
			// copy all tot records
			memcpy(&MyData.VetProgrActions[0], &pVetExtras[i], sizeof(StrProgressiveAction) * TotActions);
			MyData.TotProgrActions = TotActions;
			break;
		}
		Indice= ParseField.NextIndex; 
	}

}


// free memory used to store all data about your customize commands loaded in previous level
void FreeMemoryCustomize(void)
{
	int i;

	for (i=0;i<MyData.BaseCustomizeMine.TotCustomize;i++) {
		FreeMemory(MyData.BaseCustomizeMine.pVetCustomize[i].pVetArg);
	}

	if (MyData.BaseCustomizeMine.TotCustomize > 0) {
		FreeMemory(MyData.BaseCustomizeMine.pVetCustomize);
		MyData.BaseCustomizeMine.TotCustomize=0;
	}


	MyData.BaseCustomizeMine.pVetCustomize=NULL;
}


// free memory used to store all data about your parameters commands loaded in previous level
void FreeMemoryParameters(void)
{
	int i;

	for (i=0;i<MyData.BaseParametersMine.TotParameters;i++) {
		FreeMemory(MyData.BaseParametersMine.pVetParameters[i].pVetArg);
	}

	if (MyData.BaseParametersMine.TotParameters > 0) {
		FreeMemory(MyData.BaseParametersMine.pVetParameters);
		MyData.BaseParametersMine.TotParameters=0;
	}

	MyData.BaseParametersMine.pVetParameters=NULL;
}


// this procedure will be called at end of any level
// you can type here code to free resources allocated for level (that quits now)
void FreeLevelResources(void)
{

	// free memory used to store all data about your customize commands loaded in previous level
	FreeMemoryCustomize();
	// free memory used to store all data about your parameters commands loaded in previous level
	FreeMemoryParameters();
	MyData.BaseAssignSlotMine.TotAssign=0;

}

// it will be called before beginning the loading for a new level.
// you can type here code to initialise all variables used for level (to clear old values changed by previous level)
// and to free resources allocated in old level since now we'are going to another new level.
void cbInitLoadNewLevel(void)
{
	int i;

	StrProgressiveAction *pAction;

	// clear all LOCAL variables
	ClearMemory(&MyData.Save.Local,sizeof(StrSavegameLocalData));

	// clear progressive actions
	pAction= &MyData.VetProgrActions[0];

	for (i=0;i<MyData.TotProgrActions;i++) {
		if (pAction->ActionType != AXN_FREE) {
			// here you could analise to free resoruce allocated from this specific action

			pAction->ActionType = AXN_FREE;
		}
	}

	MyData.TotProgrActions=0;
	MyData.LastProgrActionIndex=0;

	// here you can initialise other variables of MyData different than Local and progressive actions
	// free resources allocate in previous level
	FreeLevelResources();

}


// this procedure will be called everytime a flipeffect of yours will be engaged
// you have to elaborate it and then return a TRET_.. value (most common is TRET_PERFORM_ONCE_AND_GO)
int cbFlipEffectMine(WORD FlipIndex, WORD Timer, WORD Extra, WORD ActivationMode)
{
	int RetValue;
	WORD TimerFull;

	RetValue = enumTRET.PERFORM_ONCE_AND_GO;
	// if the flip has no Extra paremeter you can handle a Timer value with values upto 32767
	// in this case you'll use the following TimerFull variable, where (with following code) we set a unique big number 
	// pasting togheter the timer+extra arguments:
	TimerFull = Timer | (Extra << 8);

	switch (FlipIndex) {
		// here type the "case Number:" for each flipeffect number. At end of the code you'll use the "break;" instruction to signal the code ending
		// Note: when you'll add your first "case Number:" then you can remove the following "case -1: and break;" instructions
	case -1: 
		break;
	default:
		SendToLog("WARNING: Flipeffect trigger number %d has not been handled in cbFlipEffectMine() function", FlipIndex);
		break;
	}

	// if there was the one-shot button enabled, return TRET_PERFORM_NEVER_MORE
	if (ActivationMode & enumSCANF.BUTTON_ONE_SHOT) RetValue= enumTRET.PERFORM_NEVER_MORE; 
	return RetValue;
}


// this procedure will be called everytime an action trigger of yours will be engaged 
// you have to elaborate it and then return a TRET_.. value (most common is TRET_PERFORM_ONCE_AND_GO)
int cbActionMine(WORD ActionIndex, int ItemIndex, WORD Extra, WORD ActivationMode)
{
	int RetValue;
	
	RetValue=TRET_PERFORM_ONCE_AND_GO;

	switch (ActionIndex) {
		// type here the code per your action trigger.
		// add "case Number:" and complete the code with "break;" instruction
	case -1:
		// note: remove this "case -1:" and its "break;" it has been added only to avoid warning messages about empty switch
		break;
	default:
		SendToLog("WARNING: action trigger number %d has not been handled in cbActionMine() function", ActionIndex);
		break;
	}
	
	// if there was the one-shot button enabled, return TRET_PERFORM_NEVER_MORE
	if (ActivationMode & enumSCANF.BUTTON_ONE_SHOT) RetValue= enumTRET.PERFORM_NEVER_MORE;
	return RetValue;


}


// this procedure will be called everytime a conditional trigger of yours will be engaged
// you have to elaborate it and then return a CTRET_.. value (most common is CTRET_ONLY_ONCE_ON_TRUE)
int cbConditionMine(WORD ConditionIndex, int ItemIndex, WORD Extra, WORD ActivationMode)
{
	int RetValue;
	
	RetValue=CTRET_ONLY_ONCE_ON_TRUE;

	switch (ConditionIndex){
		// type here the code for your condition trigger, inserting the code in the section
		// beginning with "case NumberOfAction:" and ending with row "break;"
	case -1:
		// note: remove this "case -1:" and its "break;" it has been added only to avoid warning messages about empty switch
		break;
	default:
		SendToLog("WARNING: condition trigger number %d has not been handled in cbConditionMine() function", ConditionIndex);
		break;


	}
	return RetValue;
	  
}

// this procedure vill be called for each Customize=CUST_... command read from script
// having one of yours CUST_ constant
// CustomizeValue will be the value of your CUST_ constant
// NumberOfItems will be the number of following Item (signed 16 bit values) following
// the CUST_ constant in the customize= script command
// pItemArray is the array with all NumberOfItems arguments of customize command
void cbCustomizeMine(WORD CustomizeValue, int NumberOfItems, short *pItemArray)
{
	// here you can replace this default management of anonymous customize commands
	// with your procedure where you can recognize each different CUST_ value and 
	// save its arguments in meaningful names fields, or elaboriting them immediatly
	// when it is possible (warning: in this moment nothing of level it has been yet loaded, excepting the script section)

	// ----- default management (optional)----
	// all customize values will be saved in MyData structure
	DWORD SizeMem;
	StrGenericCustomize *pMyCust;
	int TotCust;

	// ask memory to have another (new) record of StrGenericCustomize structure
	TotCust= MyData.BaseCustomizeMine.TotCustomize;
	TotCust++;
	SizeMem = TotCust * sizeof(StrGenericCustomize);
	MyData.BaseCustomizeMine.pVetCustomize = 
				(StrGenericCustomize *) ResizeMemory(MyData.BaseCustomizeMine.pVetCustomize, SizeMem);

	pMyCust = & MyData.BaseCustomizeMine.pVetCustomize[TotCust-1];
	
	// now require memory for all arguments (NumberOfItems) store in pItemArray

	pMyCust->pVetArg = (short *) GetMemory(2 * NumberOfItems);
	// copy data
	pMyCust->NArguments = NumberOfItems;
	memcpy(pMyCust->pVetArg, pItemArray, 2*NumberOfItems);
	pMyCust->CustValue = CustomizeValue;

	MyData.BaseCustomizeMine.TotCustomize= TotCust;
	// ---- end of default managemnt for generic customize -------------	
}

// callback called everytime in current level section of the script it has been found an AssignSlot command
// with one of your OBJ_ constants
void cbAssignSlotMine(WORD Slot, WORD ObjType)
{
	int i;

	i = MyData.BaseAssignSlotMine.TotAssign;

	if (i >= MAX_ASSIGN_SLOT_MINE) {
		SendToLog("ERROR: too many AssignSlot= commands for current plugin");
		return;
	}

	MyData.BaseAssignSlotMine.VetAssignSlot[i].MioSlot = Slot;
	MyData.BaseAssignSlotMine.VetAssignSlot[i].TipoSlot = ObjType;
	MyData.BaseAssignSlotMine.TotAssign++;

}

// this procedure vill be called for each Parameters=PARAM_... command read from script
// having one of yours PARAM_ constants
// ParameterValue will be the value of your PARAM_ constant
// NumberOfItems will be the number of following Item (signed 16 bit values) following
// the PARAM_ constant in the customize= script command
// pItemArray is the array with all NumberOfItems arguments of Parameter command
void cbParametersMine(WORD ParameterValue, int NumberOfItems, short *pItemArray)
{
	// here you can replace this default management of anonymous parameters commands
	// with your procedure where you can recognize each different Param_ value and 
	// save its arguments in meaningful names fields, or elaboriting them immediatly
	// when it is possible (warning: in this moment nothing of level it has been yet loaded, excepting the script section)

	// ----- default management (optional)----
	// all parameters values will be saved in MyData structure
	DWORD SizeMem;
	StrGenericParameters *pMyParam;
	int TotParam;

	// ask memory to have another (new) record of StrGenericparameters structure
	TotParam= MyData.BaseParametersMine.TotParameters;
	TotParam++;
	SizeMem = TotParam * sizeof(StrGenericParameters);
	MyData.BaseParametersMine.pVetParameters = 
		(StrGenericParameters *) ResizeMemory(MyData.BaseParametersMine.pVetParameters, SizeMem);

	pMyParam = & MyData.BaseParametersMine.pVetParameters[TotParam-1];
	
	// now require memory for all arguments (NumberOfItems) store in pItemArray

	pMyParam->pVetArg = (short *) GetMemory(2 * NumberOfItems);
	// copy data
	pMyParam->NArguments = NumberOfItems;
	memcpy(pMyParam->pVetArg, pItemArray, 2*NumberOfItems);

	MyData.BaseParametersMine.TotParameters= TotParam;
	// ---- end of default managemnt for generic parameters -------------


}


// this procedure will be called every game cycle (at begin of cycle)
void cbCycleBegin(void)
{

}


// this procedure will be called everygame cycle, at end.
// you have to return a RET_CYCLE_ value
int cbCycleEnd(void)
{
	return RET_CYCLE_CONTINUE;	
}

// this function will be called for each your (common) progressive action to be peformed
void PerformMyProgrAction(StrProgressiveAction *pAction)
{


	switch (pAction->ActionType) {
// replace the "case -1:" with your first "case AXN_...:" progressive action to manage)		
	case -1:
		break;

	}

}

// callback called from trng for each frame in game cycle to perform your (common) progressive action
void cbProgrActionMine(void)
{
	int i;
	StrProgressiveAction *pAction;

	pAction = &MyData.VetProgrActions[0];
	for (i=0;i<MyData.TotProgrActions;i++) {
		if (pAction->ActionType != AXN_FREE) {
			PerformMyProgrAction(pAction);
		}
		pAction++;
	}

}


void cbInitObjects(void)
{

}


bool RequireMyCallBacks(void)
{
	//* state extension callbacks for existing states: 10, 11, 56, 61, 75 *//
	GET_CALLBACK(CB_STATE_ID_LARA_CTRL, CBT_AFTER, AS_HANG, hang_slope_extra);				// 10
	GET_CALLBACK(CB_STATE_ID_LARA_CTRL, CBT_AFTER, AS_REACH, reach_slope_extra);			// 11
	GET_CALLBACK(CB_STATE_ID_LARA_CTRL, CBT_AFTER, AS_CLIMBSTNC, climb_slope_extra);		// 56
	GET_CALLBACK(CB_STATE_ID_LARA_CTRL, CBT_AFTER, AS_CLIMBDOWN, climbdown_slope_extra);	// 61
	GET_CALLBACK(CB_STATE_ID_LARA_CTRL, CBT_AFTER, AS_HANG2, monkey_slope_extra);			// 75


	//* standard plugin callbacks *//
	GET_CALLBACK(CB_INIT_PROGRAM, 0, 0, cbInitProgram)
	GET_CALLBACK(CB_INIT_GAME, 0, 0, cbInitGame)
	GET_CALLBACK(CB_INIT_LEVEL, 0,0, cbInitLevel)
	GET_CALLBACK(CB_SAVING_GAME, 0, 0, cbSaveMyData)
	GET_CALLBACK(CB_LOADING_GAME, 0, 0, cbLoadMyData)
	GET_CALLBACK(CB_INIT_LOAD_NEW_LEVEL, 0,0, cbInitLoadNewLevel);
	GET_CALLBACK(CB_ASSIGN_SLOT_MINE, 0,0, cbAssignSlotMine);
	//GET_CALLBACK(CB_CYCLE_BEGIN, 0, 0, cbCycleBegin);
	//GET_CALLBACK(CB_CYCLE_END, 0, 0, cbCycleEnd);
	//GET_CALLBACK(CB_FLIPEFFECT_MINE, 0, 0, cbFlipEffectMine);
	//GET_CALLBACK(CB_ACTION_MINE, 0,0, cbActionMine);
	//GET_CALLBACK(CB_CONDITION_MINE,0,0,cbConditionMine);
	//GET_CALLBACK(CB_CUSTOMIZE_MINE, 0,0, cbCustomizeMine);
	//GET_CALLBACK(CB_PARAMETER_MINE, 0, 0, cbParametersMine);
	//GET_CALLBACK(CB_PROGR_ACTION_MINE, 0, 0, cbProgrActionMine);
	//GET_CALLBACK(CB_INIT_OBJECTS, 0, 0, cbInitObjects);

	/* I didn't use some callbacks so they were commented
	feel free to uncomment any callback which you may want to use */

	return true;
}
// FOR_YOU:
// This function will be the first code to be executed of your plugin
// It happens when trng will load your plugin.
// In this moment no directX graphic is yet active, so you can show
// message boxes (TryMessageBox) to advise your players or level designer 
// about something
// Note: if you wish abort all (because there is an error or something
// is missing) you have to exit from this function returning: false
bool InitializeAll(void)
{
// ************  InitializeAll() function  ****************
	//  perform all your patches
	CALL_CHECK(CreateMyCodePatches)

	// call the function that requires all callback you need
	CALL_CHECK(RequireMyCallBacks)

	// TYPE_HERE: code to allocate global resource to use in the whole game

	return true;
}

// FOR_YOU: Tyis function will be called when tomb4 game is to be closed.
// you should type in this function further codes to free the global
// resource you had allocated in the InitializeAll() function 
void ReleaseAll(void)
{
// ************  ReleaseAll() function  ******************
	FreeLevelResources();
}


BOOL APIENTRY DllMain( HINSTANCE hInstanceDll, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved)
{

    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			MyDllInstance = hInstanceDll;
			GetTrngInfo();
			// control per check control value about size and alignment with globtomb4 structure
			if (CheckControlGlobTomb4() == false) return FALSE;

			if  (InitializeAll()==false) {
				return FALSE;
			}
			return TRUE;
			

		case DLL_PROCESS_DETACH:
			ReleaseAll();
			break;
    }
    return TRUE;
}


