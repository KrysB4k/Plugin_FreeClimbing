// let below pragma directive at top of this source
#pragma pack(1)

// TYPE_HERE: here you can type your structure definitions like it has been done
// in the "structures.h" file for trng dll

// --------------- PRESET STRUCTURE ZONE -----------------------------
// Please, don't remove structures and fields you find in this "PRESET ZONE". They will be used by some
// preset functions of your plugin sources
// However, you can add new fields and structures in following structures, of course
typedef struct StrSavegameGlobalData {
	// FOR_YOU:
	// define here your variables that you wish were saved (and then restored) to/from savegame in GLOBAL section 
	//           (only one for all levels)
	// note: the size of this structure should be always even (if you add BYTE variable, remember to compensate that 
	//       with another BYTE vairable or placefolder)
}SavegameGlobalDataFields;

typedef struct StrSavegameLocalData {
	// FOR_YOU:
	// define here your variables that you wish were saved (and then restored) to/from savegame in LOCAL section (one for each different level)
	// note: the size of this structure should be always even (if you add BYTE variable, compensate it with another BYTE vairable or placefolder)
	
	bool leftLegUp;
	BYTE pad;
}SavegameLocalDataFields;


typedef struct StrSavegameData {
	StrSavegameGlobalData Global;
	StrSavegameLocalData  Local;
}SavegameDataFields;


typedef struct StrBaseAssignSlotMine {
	int TotAssign;
	StrRecordAssSlot VetAssignSlot[MAX_ASSIGN_SLOT_MINE];
}BaseAssignSlotMineFields;

typedef struct StrMyData {
	StrSavegameData Save;  // variable that it will be saved and restored to/from savegame
	// FOR_YOU:
	// define here all your global variables, i.e. those variables that will be seen from all procedures and keep 
	// their value between all cycles of the game.
	// after you defined a variabile like:
	// int  Alfa;
	// then you'll be able to access to them using syntax like:
	// MyData.Alfa = 3;
	// if (MyData.Alfa == 5) .
	int TotProgrActions;
	int LastProgrActionIndex; // used in case of overloading
	short overhangSlot;

	StrProgressiveAction VetProgrActions[MAX_MYPROGR_ACTIONS];
	StrBaseGenericCustomize BaseCustomizeMine;  // stored all your customize script commands for current level
	StrBaseGenericParameters BaseParametersMine; // store of all your parameters= script commands of current level
	StrBaseAssignSlotMine BaseAssignSlotMine; // stored all AssignSlot= command script commands of current level
}MyDataFields;
// ----------------- END PRESET STRUCTURE ZONE ---------------------------------------




// ******* Various Enums ******* //

enum climbingAnims
{
	idle_left,
	idle_right,
	climb_up_left,
	climb_up_right,
	climb_down_left,
	climb_down_right,
	drop_left,
	drop_right,
	idle_2_hang_left,
	idle_2_hang_right,
	hang_2_idle_left,
	hang_2_idle_right,
	hang_swing,
	hang_loop,
	hang_drop,
	shimmy_left,
	shimmy_left_stop,
	shimmy_right,
	shimmy_right_stop,
	ledge_vault_start,
	ledge_vault,
	ladder_slope_concave,
	slope_ladder_concave,
	ladder_slope_convex,
	slope_ladder_convex_start,
	slope_ladder_convex,
	monkey_slope_concave,
	slope_monkey_concave,
	slope_monkey_concave_end,
	monkey_slope_convex,
	monkey_slope_convex_end,
	slope_monkey_convex,
	exit_monkey_forward,
	exit_monkey_idle,
	exit_ladder,
	exit_vault,
	exit_drop,
	exit_hang
};

enum lara_states
{
	AS_WALK=0,
	AS_RUN,
	AS_STOP,
	AS_FORWARDJUMP,
	AS_POSE,
	AS_FASTBACK,
	AS_TURN_R,
	AS_TURN_L,
	AS_DEATH,
	AS_FASTFALL,
	AS_HANG,
	AS_REACH,
	AS_SPLAT,
	AS_TREAD,
	AS_LAND,
	AS_COMPRESS,
	AS_BACK,
	AS_SWIM,
	AS_GLIDE,
	AS_NULL,
	AS_FASTTURN,
	AS_STEPRIGHT,
	AS_STEPLEFT,
	AS_HIT,
	AS_SLIDE,
	AS_BACKJUMP,
	AS_RIGHTJUMP,
	AS_LEFTJUMP,
	AS_UPJUMP,
	AS_FALLBACK,
	AS_HANGLEFT,
	AS_HANGRIGHT,
	AS_SLIDEBACK,
	AS_SURFTREAD,
	AS_SURFSWIM,
	AS_DIVE,
	AS_PUSHBLOCK,
	AS_PULLBLOCK,
	AS_PPREADY,
	AS_PICKUP,
	AS_SWITCHON,
	AS_SWITCHOFF,
	AS_USEKEY,
	AS_USEPUZZLE,
	AS_UWDEATH,
	AS_ROLL,
	AS_SPECIAL,
	AS_SURFBACK,
	AS_SURFLEFT,
	AS_SURFRIGHT,
	AS_USEMIDAS,
	AS_DIEMIDAS,
	AS_SWANDIVE,
	AS_FASTDIVE,
	AS_GYMNAST,
	AS_WATEROUT,
	AS_CLIMBSTNC,
	AS_CLIMBING,
	AS_CLIMBLEFT,
	AS_CLIMBEND,
	AS_CLIMBRIGHT,
	AS_CLIMBDOWN,
	AS_LARATEST1,
	AS_LARATEST2,
	AS_LARATEST3,
	AS_WADE,
	AS_WATERROLL,
	AS_FLAREPICKUP,
	AS_TWIST,
	AS_KICK,
	AS_DEATHSLIDE,
	AS_DUCK,
	AS_DUCKROLL,
	AS_DASH,
	AS_DASHDIVE,
	AS_HANG2,
	AS_MONKEYSWING,
	AS_MONKEYL,
	AS_MONKEYR,
	AS_MONKEY180,
	AS_ALL4S,
	AS_CRAWL,
	AS_HANGTURNL,
	AS_HANGTURNR,
	AS_ALL4TURNL,
	AS_ALL4TURNR,		// 85
	AS_CRAWLBACK,
	AS_HANG2DUCK,
	AS_CRAWL2HANG,
	AS_CONTROLLED,
	AS_ROPELEFT,
	AS_ROPERIGHT,
	AS_BLOCKSWITCH,
	AS_LIFTTRAP,
	AS_PULLTRAP,
	AS_TURNSWITCH,
	AS_COGSWITCH,
	AS_RAILSWITCH,
	AS_HIDDENPICKUP,
	AS_POLESTAT,
	AS_POLEUP,		// 100
	AS_POLEDOWN,
	AS_POLELEFT,
	AS_POLERIGHT,
	AS_PULLEY,
	AS_DUCKROTL,		// 105
	AS_DUCKROTR,
	AS_CORNEREXTL,
	AS_CORNEREXTR,
	AS_CORNERINTL,
	AS_CORNERINTR,		// 110
	AS_ROPE,
	AS_CLIMBROPE,
	AS_SLIDEROPE,
	AS_ROPEFWD,
	AS_ROPEBACK,		// 115
	AS_MOVE,
	AS_TWODOOR,
	AS_SWIMCHEAT,
	AS_TROPEPOSE,
	AS_TROPETURN,		// 120
	AS_TROPEWALK,
	AS_TROPEFALL_L,
	AS_TROPEFALL_R,
	AS_TROPEGETON,
	AS_TROPEGETOFF, 	// 125
	AS_CROWDOVE,
	AS_TROPEUNDOFALL,
	AS_PBSPIN,
	AS_PBLEAP,
	/*new climbing states*/
	AS_SCLIMB_IDLE,		// 130
	AS_SCLIMB_UP,
	AS_SCLIMB_DOWN,
	AS_SCLIMB_FALL,
	AS_SCLIMB_HANG,
	AS_SCLIMB_SHIMMY,	// 135
	AS_SCLIMB_START,
	AS_SCLIMB_STOP,
	AS_SCLIMB_END
};


enum floor_types {
	FLOOR_TYPE, DOOR_TYPE, TILT_TYPE, ROOF_TYPE, TRIGGER_TYPE, LAVA_TYPE, CLIMB_TYPE, SPLIT1, SPLIT2, SPLIT3, SPLIT4,
	NOCOLF1T, NOCOLF1B, NOCOLF2T, NOCOLF2B, NOCOLC1T, NOCOLC1B, NOCOLC2T, NOCOLC2B,
	MONKEY_TYPE, TRIGTRIGGER_TYPE, MINER_TYPE
};


typedef enum camera_type {
	CHASE_CAMERA,
	FIXED_CAMERA,
	LOOK_CAMERA,
	COMBAT_CAMERA,
	CINEMATIC_CAMERA,
	HEAVY_CAMERA
}CAMERA_TYPE;



// ******** Tomb4 Structs ******** //

typedef struct
{
	int x, y, z;
}phd_vector;

typedef struct
{
	int xPos, yPos, zPos;
	short xRot, yRot, zRot;
}phd_3dpos;


typedef struct // 08
{
	WORD index;     // 00
	WORD box_flags; // 02
	BYTE pit_room;  // 04
	char floor;     // 05
	BYTE sky_room;  // 06
	char ceiling;   // 07
}Tr4FloorInfo;


typedef struct // 94
{
	short *data;					//00
	short *door;					//04
	Tr4FloorInfo *floor;			//08
	void *light;					//0C
	void *mesh;						//10
	int origX;						//14
	int origY;						//18
	int origZ;						//1C
	int minfloor, maxceiling;		//20, 24
	short z_size, x_size;			//28, 2A
	short ambient, lighting;		//2C, 2E
	short num_lights;				//30
	short num_meshes;				//32
	BYTE water_intensity;			//34
	char flipmap_index;				//35
	char indexTabWater;				//36
	char bound_active;				//37
	short left, right, top, bottom;	//38, 3A, 3C, 3E
	short test_left, test_right, test_top, test_bottom;	//40, 42, 44, 46
	short item_number;				//48
	short fx_number;				//4A
	short flipped_room;				//4C
	WORD flags;						//4E
	int vertices;					//50
	int vertices_water;				//54
	int vertices_dry;				//58
	void *D3Dvertexbuffer;			//5C
	void *rectangles;				//60
	float forigX, forigY, forigZ;	//64, 68, 6C
	int vertexOffset;				//70
	void *polygons;					//74
	void *quadVertices;				//78
	void *triVertices;				//7C
	int unknown80;					//80
	void *verts;					//84
	int tris;						//88
	int quads;						//8C
	void *light_directx;			//90
}Tr4RoomInfo;


typedef struct
{
	int floor, ceiling, type;
}CollData;


typedef struct // 86
{
	CollData mid;					// 00
	CollData front;					// 0C
	CollData left;					// 18
	CollData right;					// 24
	CollData left2;					// 30
	CollData right2;				// 3C

	int radius;                     // 48
	int bad_pos;                    // 4C
	int bad_neg;                    // 50
	int bad_ceiling;                // 54

	int shiftX, shiftY, shiftZ;     // 58
	int oldposX, oldposY, oldposZ;  // 64

	short old_anim_state;           // 70
	short old_anim_number;          // 72
	short old_frame_number;         // 74

	short facing;                   // 76
	short quadrant;                 // 78
	short coll_type;                // 7A
	short *trigger;                 // 7C

	char tiltX, tiltZ;              // 80
	                                // 81
	char hit_by_baddie;             // 82
	char hit_static;                // 83
	WORD flags;                     // 84
}Tr4CollInfo;


typedef struct
{
	short *FrameOffset;				// 00
	BYTE FrameRate;					// 04
	BYTE FrameSize;					// 05

	short StateID;					// 06

	int Speed;						// 08
	int Accel;						// 0C
	int SpeedLateral;				// 10
	int AccelLateral;				// 14

	short FrameBase;				// 18
	short FrameEnd;					// 1A
	short NextAnim;					// 1C
	short NextFrame;				// 1E

	short NumStateChanges;			// 20
	short StateChangeIndex;			// 22
	short NumAnimCommands;			// 24
	short AnimCommandIndex;			// 26
}Tr4AnimStruct;


typedef struct
{
	WORD NumMeshes;			// 00
	WORD MeshIndex;			// 02
	int BoneIndex;			// 04
	short *FrameBase;		// 08
	void *InitialiseProc;	// 0C
	void *ControlProc;		// 10
	void *FloorProc;		// 14
	void *CeilingProc;		// 18
	void *DrawRoutine;		// 1C
	void *CollisionProc;	// 20
	short DistanceMIP;		// 24
	short AnimIndex;		// 26
	short HitPoints;		// 28
	short PivotLength;		// 2A
	short Radius;			// 2C
	short ShadowSize;		// 2E
	WORD BiteOffset;		// 30
	WORD Flags;				// 32
	void *ExtraDrawProc;	// 34
	DWORD ShatterMask;		// 38
	DWORD Unknown;			// 3C
}Tr4ObjectInfo;


typedef struct
{
	int floorheight;        // 00
	DWORD touch_bits;       // 04
	DWORD mesh_bits;        // 08
	short slot_number;      // 0C
	short current_state;    // 0E
	short goal_state;       // 10
	short required_state;   // 12
	short anim_number;      // 14
	short frame_number;     // 16
	short room_number;      // 18
	short next_item;        // 1A
	short next_active;      // 1C
	short speed;            // 1E
	short fallspeed;        // 20
	short hit_points;       // 22
	WORD box_number;        // 24
	short timer;            // 26
	short timer_flags;      // 28
	short intensity;        // 2A
	WORD ocbcode;           // 2C
	BYTE sprite1, sprite2;  // 2E
	WORD transparency;      // 30
	WORD fired_weapon;      // 32
	short item_flags[4];    // 34
	void *savedata;         // 3C
	phd_3dpos pos;   // 40
	DWORD Red, Green, Blue; // 52
	DWORD RGB;              // 5E
	DWORD RedDiff, GreenDiff, BlueDiff; // 62
	DWORD LightChange;      // 6E
	BYTE LightData[5488];   // 72
	BYTE *ptrZone1;         // 15E2
	BYTE *ptrZone2;         // 15E6
	DWORD flagsMain;        // 15EA
	DWORD flagsMeshswap;    // 15EE
	DWORD unknown;          // 15F2
}Tr4ItemInfo;


typedef struct {
	int x, y, z;
	short room, box;
}game_vector;


typedef struct {
	int x;
	int y;
	int z;
	short data;
	short flags;
}object_vector;


typedef struct
{
	game_vector pos; //00
	game_vector target; //10
	CAMERA_TYPE type; //20
	CAMERA_TYPE old_type; //24
	int shift; //28
	int flags; //2C
	int fixed_camera; //30
	int number_frames; //34
	int bounce; // 38
	int underwater; //3C
	int target_distance; //40
	short target_angle; //44
	short target_elevation; //46
	short actual_elevation; //48
	short actual_angle; //4A
	short number; //4C
	short last; //4E
	short timer; //50
	short speed; //52
	Tr4ItemInfo *item, *last_item;
	object_vector* fixed;
	int mike_at_lara;
	phd_vector mike_pos;
}CAMERA_INFO;



// ******* custom structs ******* //

typedef struct // returned by GetCeilingTilts()
{
	char tiltX;
	char tiltZ;
}SlopeTilts;



// let below pragma directive at bottom of this source
#pragma pack(2)