#pragma once

#include <d3d9.h>

struct expressionEntry;
struct ExpressionSupportingData;
struct FxElemDef;
struct GfxPortal;
struct itemDef_s;
struct MaterialTechnique;
struct MenuEventHandler;
struct pathnode_t;
struct pathnode_tree_t;
struct Statement_s;

// 38
//
enum XAssetType
{
  ASSET_TYPE_PHYSPRESET = 0x0,
  ASSET_TYPE_PHYSCOLLMAP = 0x1,
  ASSET_TYPE_XANIMPARTS = 0x2,
  ASSET_TYPE_XMODEL_SURFS = 0x3,
  ASSET_TYPE_XMODEL = 0x4,
  ASSET_TYPE_MATERIAL = 0x5,
  ASSET_TYPE_PIXELSHADER = 0x6,
  ASSET_TYPE_VERTEXSHADER = 0x7,
  ASSET_TYPE_VERTEXDECL = 0x8,
  ASSET_TYPE_TECHNIQUE_SET = 0x9,
  ASSET_TYPE_IMAGE = 0xA,
  ASSET_TYPE_SOUND = 0xB,
  ASSET_TYPE_SOUND_CURVE = 0xC,
  ASSET_TYPE_LOADED_SOUND = 0xD,
  ASSET_TYPE_CLIPMAP_SP = 0xE,
  ASSET_TYPE_CLIPMAP_MP = 0xF,
  ASSET_TYPE_COMWORLD = 0x10,
  ASSET_TYPE_GAMEWORLD_SP = 0x11,
  ASSET_TYPE_GAMEWORLD_MP = 0x12,
  ASSET_TYPE_MAP_ENTS = 0x13,
  ASSET_TYPE_FXWORLD = 0x14,
  ASSET_TYPE_GFXWORLD = 0x15,
  ASSET_TYPE_LIGHT_DEF = 0x16,
  ASSET_TYPE_UI_MAP = 0x17,
  ASSET_TYPE_FONT = 0x18,
  ASSET_TYPE_MENULIST = 0x19,
  ASSET_TYPE_MENU = 0x1A,
  ASSET_TYPE_LOCALIZE_ENTRY = 0x1B,
  ASSET_TYPE_WEAPON = 0x1C,
  ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1D,
  ASSET_TYPE_FX = 0x1E,
  ASSET_TYPE_IMPACT_FX = 0x1F,
  ASSET_TYPE_AITYPE = 0x20,
  ASSET_TYPE_MPTYPE = 0x21,
  ASSET_TYPE_CHARACTER = 0x22,
  ASSET_TYPE_XMODELALIAS = 0x23,
  ASSET_TYPE_RAWFILE = 0x24,
  ASSET_TYPE_STRINGTABLE = 0x25,
  ASSET_TYPE_LEADERBOARD = 0x26,
  ASSET_TYPE_STRUCTURED_DATA_DEF = 0x27,
  ASSET_TYPE_TRACER = 0x28,
  ASSET_TYPE_VEHICLE = 0x29,
  ASSET_TYPE_ADDON_MAP_ENTS = 0x2A,
  ASSET_TYPE_COUNT = 0x2B,
  ASSET_TYPE_STRING = 0x2B,
  ASSET_TYPE_ASSETLIST = 0x2C,
};

// 68
//
enum clientMigState_t
{
  CMSTATE_INACTIVE = 0x0,
  CMSTATE_OLDHOSTLEAVING = 0x1,
  CMSTATE_LIMBO = 0x2,
  CMSTATE_NEWHOSTCONNECT = 0x3,
  CMSTATE_COUNT = 0x4,
};

// 77
//
enum weapFireType_t
{
  WEAPON_FIRETYPE_FULLAUTO = 0x0,
  WEAPON_FIRETYPE_SINGLESHOT = 0x1,
  WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
  WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
  WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
  WEAPON_FIRETYPE_DOUBLEBARREL = 0x5,
  WEAPON_FIRETYPECOUNT = 0x6,
  WEAPON_FIRETYPE_BURSTFIRE_FIRST = 0x2,
  WEAPON_FIRETYPE_BURSTFIRE_LAST = 0x4,
};

// 79
//
enum OffhandClass
{
  OFFHAND_CLASS_NONE = 0x0,
  OFFHAND_CLASS_FRAG_GRENADE = 0x1,
  OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
  OFFHAND_CLASS_FLASH_GRENADE = 0x3,
  OFFHAND_CLASS_THROWINGKNIFE = 0x4,
  OFFHAND_CLASS_OTHER = 0x5,
  OFFHAND_CLASS_COUNT = 0x6,
};

// 82
//
enum connstate_t
{
  CA_DISCONNECTED = 0x0,
  CA_CINEMATIC = 0x1,
  CA_LOGO = 0x2,
  CA_CONNECTING = 0x3,
  CA_CHALLENGING = 0x4,
  CA_CONNECTED = 0x5,
  CA_SENDINGSTATS = 0x6,
  CA_LOADING = 0x7,
  CA_PRIMED = 0x8,
  CA_ACTIVE = 0x9,
};

// 83
//
enum WeapStickinessType
{
  WEAPSTICKINESS_NONE = 0x0,
  WEAPSTICKINESS_ALL = 0x1,
  WEAPSTICKINESS_ALL_ORIENT = 0x2,
  WEAPSTICKINESS_GROUND = 0x3,
  WEAPSTICKINESS_GROUND_WITH_YAW = 0x4,
  WEAPSTICKINESS_KNIFE = 0x5,
  WEAPSTICKINESS_COUNT = 0x6,
};

// 93
//
enum MaterialTechniqueType
{
  TECHNIQUE_DEPTH_PREPASS = 0x0,
  TECHNIQUE_BUILD_FLOAT_Z = 0x1,
  TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
  TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
  TECHNIQUE_UNLIT = 0x4,
  TECHNIQUE_EMISSIVE = 0x5,
  TECHNIQUE_EMISSIVE_DFOG = 0x6,
  TECHNIQUE_EMISSIVE_SHADOW = 0x7,
  TECHNIQUE_EMISSIVE_SHADOW_DFOG = 0x8,
  TECHNIQUE_LIT_BEGIN = 0x9,
  TECHNIQUE_LIT = 0x9,
  TECHNIQUE_LIT_DFOG = 0xA,
  TECHNIQUE_LIT_SUN = 0xB,
  TECHNIQUE_LIT_SUN_DFOG = 0xC,
  TECHNIQUE_LIT_SUN_SHADOW = 0xD,
  TECHNIQUE_LIT_SUN_SHADOW_DFOG = 0xE,
  TECHNIQUE_LIT_SPOT = 0xF,
  TECHNIQUE_LIT_SPOT_DFOG = 0x10,
  TECHNIQUE_LIT_SPOT_SHADOW = 0x11,
  TECHNIQUE_LIT_SPOT_SHADOW_DFOG = 0x12,
  TECHNIQUE_LIT_OMNI = 0x13,
  TECHNIQUE_LIT_OMNI_DFOG = 0x14,
  TECHNIQUE_LIT_OMNI_SHADOW = 0x15,
  TECHNIQUE_LIT_OMNI_SHADOW_DFOG = 0x16,
  TECHNIQUE_LIT_INSTANCED = 0x17,
  TECHNIQUE_LIT_INSTANCED_DFOG = 0x18,
  TECHNIQUE_LIT_INSTANCED_SUN = 0x19,
  TECHNIQUE_LIT_INSTANCED_SUN_DFOG = 0x1A,
  TECHNIQUE_LIT_INSTANCED_SUN_SHADOW = 0x1B,
  TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG = 0x1C,
  TECHNIQUE_LIT_INSTANCED_SPOT = 0x1D,
  TECHNIQUE_LIT_INSTANCED_SPOT_DFOG = 0x1E,
  TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW = 0x1F,
  TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG = 0x20,
  TECHNIQUE_LIT_INSTANCED_OMNI = 0x21,
  TECHNIQUE_LIT_INSTANCED_OMNI_DFOG = 0x22,
  TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW = 0x23,
  TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG = 0x24,
  TECHNIQUE_LIT_END = 0x25,
  TECHNIQUE_LIGHT_SPOT = 0x25,
  TECHNIQUE_LIGHT_OMNI = 0x26,
  TECHNIQUE_LIGHT_SPOT_SHADOW = 0x27,
  TECHNIQUE_FAKELIGHT_NORMAL = 0x28,
  TECHNIQUE_FAKELIGHT_VIEW = 0x29,
  TECHNIQUE_SUNLIGHT_PREVIEW = 0x2A,
  TECHNIQUE_CASE_TEXTURE = 0x2B,
  TECHNIQUE_WIREFRAME_SOLID = 0x2C,
  TECHNIQUE_WIREFRAME_SHADED = 0x2D,
  TECHNIQUE_DEBUG_BUMPMAP = 0x2E,
  TECHNIQUE_DEBUG_BUMPMAP_INSTANCED = 0x2F,
  TECHNIQUE_COUNT = 0x30,
  TECHNIQUE_TOTAL_COUNT = 0x31,
  TECHNIQUE_NONE = 0x32,
};

// 97
//
enum MigrationVerboseState
{
  MVSTATE_INACTIVE = 0x0,
  MVSTATE_WAITING = 0x1,
  MVSTATE_RATING = 0x2,
  MVSTATE_SENDING = 0x3,
  MVSTATE_MIGRATING = 0x4,
  MVSTATE_COUNT = 0x5,
};

// 98
//
enum StanceState
{
  CL_STANCE_STAND = 0x0,
  CL_STANCE_CROUCH = 0x1,
  CL_STANCE_PRONE = 0x2,
};

// 99
//
enum ImpactType
{
  IMPACT_TYPE_NONE = 0x0,
  IMPACT_TYPE_BULLET_SMALL = 0x1,
  IMPACT_TYPE_BULLET_LARGE = 0x2,
  IMPACT_TYPE_BULLET_AP = 0x3,
  IMPACT_TYPE_BULLET_EXPLODE = 0x4,
  IMPACT_TYPE_SHOTGUN = 0x5,
  IMPACT_TYPE_SHOTGUN_EXPLODE = 0x6,
  IMPACT_TYPE_GRENADE_BOUNCE = 0x7,
  IMPACT_TYPE_GRENADE_EXPLODE = 0x8,
  IMPACT_TYPE_ROCKET_EXPLODE = 0x9,
  IMPACT_TYPE_PROJECTILE_DUD = 0xA,
  IMPACT_TYPE_COUNT = 0xB,
};

// 102
//
enum weapType_t
{
  WEAPTYPE_BULLET = 0x0,
  WEAPTYPE_GRENADE = 0x1,
  WEAPTYPE_PROJECTILE = 0x2,
  WEAPTYPE_RIOTSHIELD = 0x3,
  WEAPTYPE_NUM = 0x4,
};

// 103
//
enum WeapOverlayInteface_t
{
  WEAPOVERLAYINTERFACE_NONE = 0x0,
  WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
  WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
  WEAPOVERLAYINTERFACECOUNT = 0x3,
};

// 105
//
enum weapClass_t
{
  WEAPCLASS_RIFLE = 0x0,
  WEAPCLASS_SNIPER = 0x1,
  WEAPCLASS_MG = 0x2,
  WEAPCLASS_SMG = 0x3,
  WEAPCLASS_SPREAD = 0x4,
  WEAPCLASS_PISTOL = 0x5,
  WEAPCLASS_GRENADE = 0x6,
  WEAPCLASS_ROCKETLAUNCHER = 0x7,
  WEAPCLASS_TURRET = 0x8,
  WEAPCLASS_THROWINGKNIFE = 0x9,
  WEAPCLASS_NON_PLAYER = 0xA,
  WEAPCLASS_ITEM = 0xB,
  WEAPCLASS_NUM = 0xC,
};

// 106
//
enum weapOverlayReticle_t
{
  WEAPOVERLAYRETICLE_NONE = 0x0,
  WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
  WEAPOVERLAYRETICLE_NUM = 0x2,
};

// 108
//
enum weapStance_t
{
  WEAPSTANCE_STAND = 0x0,
  WEAPSTANCE_DUCK = 0x1,
  WEAPSTANCE_PRONE = 0x2,
  WEAPSTANCE_NUM = 0x3,
};

// 109
//
enum weaponIconRatioType_t
{
  WEAPON_ICON_RATIO_1TO1 = 0x0,
  WEAPON_ICON_RATIO_2TO1 = 0x1,
  WEAPON_ICON_RATIO_4TO1 = 0x2,
  WEAPON_ICON_RATIO_COUNT = 0x3,
};

// 111
//
enum PenetrateType
{
  PENETRATE_TYPE_NONE = 0x0,
  PENETRATE_TYPE_SMALL = 0x1,
  PENETRATE_TYPE_MEDIUM = 0x2,
  PENETRATE_TYPE_LARGE = 0x3,
  PENETRATE_TYPE_COUNT = 0x4,
};

// 112
//
enum weapInventoryType_t
{
  WEAPINVENTORY_PRIMARY = 0x0,
  WEAPINVENTORY_OFFHAND = 0x1,
  WEAPINVENTORY_ITEM = 0x2,
  WEAPINVENTORY_ALTMODE = 0x3,
  WEAPINVENTORY_EXCLUSIVE = 0x4,
  WEAPINVENTORY_SCAVENGER = 0x5,
  WEAPINVENTORYCOUNT = 0x6,
};

// 113
//
enum activeReticleType_t
{
  VEH_ACTIVE_RETICLE_NONE = 0x0,
  VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
  VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
  VEH_ACTIVE_RETICLE_COUNT = 0x3,
};

// 114
//
enum ammoCounterClipType_t
{
  AMMO_COUNTER_CLIP_NONE = 0x0,
  AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
  AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
  AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
  AMMO_COUNTER_CLIP_ROCKET = 0x4,
  AMMO_COUNTER_CLIP_BELTFED = 0x5,
  AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
  AMMO_COUNTER_CLIP_COUNT = 0x7,
};

// 115
//
enum weapProjExposion_t
{
  WEAPPROJEXP_GRENADE = 0x0,
  WEAPPROJEXP_ROCKET = 0x1,
  WEAPPROJEXP_FLASHBANG = 0x2,
  WEAPPROJEXP_NONE = 0x3,
  WEAPPROJEXP_DUD = 0x4,
  WEAPPROJEXP_SMOKE = 0x5,
  WEAPPROJEXP_HEAVY = 0x6,
  WEAPPROJEXP_NUM = 0x7,
};

// 116
//
enum guidedMissileType_t
{
  MISSILE_GUIDANCE_NONE = 0x0,
  MISSILE_GUIDANCE_SIDEWINDER = 0x1,
  MISSILE_GUIDANCE_HELLFIRE = 0x2,
  MISSILE_GUIDANCE_JAVELIN = 0x3,
  MISSILE_GUIDANCE_COUNT = 0x4,
};

// 165
//
enum expDataType
{
  VAL_INT = 0x0,
  VAL_FLOAT = 0x1,
  VAL_STRING = 0x2,
  NUM_INTERNAL_DATATYPES = 0x3,
  VAL_FUNCTION = 0x3,
  NUM_DATATYPES = 0x4,
};

// 249
//
enum LbColType
{
  LBCOL_TYPE_NUMBER = 0x0,
  LBCOL_TYPE_TIME = 0x1,
  LBCOL_TYPE_LEVELXP = 0x2,
  LBCOL_TYPE_PRESTIGE = 0x3,
  LBCOL_TYPE_BIGNUMBER = 0x4,
  LBCOL_TYPE_PERCENT = 0x5,
  LBCOL_TYPE_COUNT = 0x6,
};

// 250
//
enum LbAggType
{
  LBAGG_TYPE_MIN = 0x0,
  LBAGG_TYPE_MAX = 0x1,
  LBAGG_TYPE_SUM = 0x2,
  LBAGG_TYPE_LAST = 0x3,
  LBAGG_TYPE_COUNT = 0x4,
};

// 261
//
enum VehicleAxleType
{
  VEH_AXLE_FRONT = 0x0,
  VEH_AXLE_REAR = 0x1,
  VEH_AXLE_ALL = 0x2,
  VEH_AXLE_COUNT = 0x3,
};

// 262
//
enum VehicleType
{
  VEH_WHEELS_4 = 0x0,
  VEH_TANK = 0x1,
  VEH_PLANE = 0x2,
  VEH_BOAT = 0x3,
  VEH_ARTILLERY = 0x4,
  VEH_HELICOPTER = 0x5,
  VEH_SNOWMOBILE = 0x6,
  VEH_TYPE_COUNT = 0x7,
};

// 313
//
enum DynEntityType
{
  DYNENT_TYPE_INVALID = 0x0,
  DYNENT_TYPE_CLUTTER = 0x1,
  DYNENT_TYPE_DESTRUCT = 0x2,
  DYNENT_TYPE_COUNT = 0x3,
};

// 368
//
enum StructuredDataTypeCategory
{
  DATA_INT = 0x0,
  DATA_BYTE = 0x1,
  DATA_BOOL = 0x2,
  DATA_STRING = 0x3,
  DATA_ENUM = 0x4,
  DATA_STRUCT = 0x5,
  DATA_INDEXED_ARRAY = 0x6,
  DATA_ENUM_ARRAY = 0x7,
  DATA_FLOAT = 0x8,
  DATA_SHORT = 0x9,
  DATA_COUNT = 0xA,
};

// 548
//
enum nodeType
{
  NODE_ERROR = 0x0,
  NODE_PATHNODE = 0x1,
  NODE_COVER_STAND = 0x2,
  NODE_COVER_CROUCH = 0x3,
  NODE_COVER_CROUCH_WINDOW = 0x4,
  NODE_COVER_PRONE = 0x5,
  NODE_COVER_RIGHT = 0x6,
  NODE_COVER_LEFT = 0x7,
  NODE_AMBUSH = 0x8,
  NODE_EXPOSED = 0x9,
  NODE_CONCEALMENT_STAND = 0xA,
  NODE_CONCEALMENT_CROUCH = 0xB,
  NODE_CONCEALMENT_PRONE = 0xC,
  NODE_DOOR = 0xD,
  NODE_DOOR_INTERIOR = 0xE,
  NODE_SCRIPTED = 0xF,
  NODE_NEGOTIATION_BEGIN = 0x10,
  NODE_NEGOTIATION_END = 0x11,
  NODE_TURRET = 0x12,
  NODE_GUARD = 0x13,
  NODE_NUMTYPES = 0x14,
  NODE_DONTLINK = 0x14,
};

// 551
//
enum PathNodeErrorCode
{
  PNERR_NONE = 0x0,
  PNERR_INSOLID = 0x1,
  PNERR_FLOATING = 0x2,
  PNERR_NOLINK = 0x3,
  PNERR_DUPLICATE = 0x4,
  PNERR_NOSTANCE = 0x5,
  PNERR_INVALIDDOOR = 0x6,
  PNERR_NOANGLES = 0x7,
  PNERR_BADPLACEMENT = 0x8,
  NUM_PATH_NODE_ERRORS = 0x9,
};

// 771
//
struct _AILSOUNDINFO
{
  int format;
  const void *data_ptr;
  unsigned int data_len;
  unsigned int rate;
  int bits;
  int channels;
  unsigned int samples;
  unsigned int block_size;
  const void *initial_ptr;
};

// 772
//
struct MssSound
{
  _AILSOUNDINFO info;
  char *data;
};

// 773
//
struct LoadedSound
{
  const char *name;
  MssSound sound;
};

// 774
//
struct StreamFileNameRaw
{
  const char *dir;
  const char *name;
};

// 775
//
union StreamFileInfo
{
  StreamFileNameRaw raw;
};

// 776
//
struct StreamFileName
{
  StreamFileInfo info;
};

// 777
//
struct StreamedSound
{
  StreamFileName filename;
};

// 778
//
union SoundFileRef
{
  LoadedSound *loadSnd;
  StreamedSound streamSnd;
};

// 779
//
struct SoundFile
{
  char type;
  char exists;
  SoundFileRef u;
};

// 780
//
struct SndCurve
{
  const char *filename;
  unsigned __int16 knotCount;
  float knots[16][2];
};

// 781
//
struct MSSSpeakerLevels
{
  int speaker;
  int numLevels;
  float levels[2];
};

// 782
//
struct MSSChannelMap
{
  int speakerCount;
  MSSSpeakerLevels speakers[6];
};

// 783
//
struct SpeakerMap
{
  bool isDefault;
  const char *name;
  MSSChannelMap channelMaps[2][2];
};

// 785
//
struct snd_alias_t
{
  const char *aliasName;
  const char *subtitle;
  const char *secondaryAliasName;
  const char *chainAliasName;
  const char *mixerGroup;
  SoundFile *soundFile;
  int sequence;
  float volMin;
  float volMax;
  float pitchMin;
  float pitchMax;
  float distMin;
  float distMax;
  float velocityMin;
  int flags;
  union
  {
    float slavePercentage;
    float masterPercentage;
  };
  float probability;
  float lfePercentage;
  float centerPercentage;
  int startDelay;
  SndCurve *volumeFalloffCurve;
  float envelopMin;
  float envelopMax;
  float envelopPercentage;
  SpeakerMap *speakerMap;
};

// 791
//
union DvarValue
{
  bool enabled;
  int integer;
  unsigned int unsignedInt;
  float value;
  float vector[4];
  const char *string;
  char color[4];
};

// 795
//
union DvarLimits
{
  struct
  {
    int stringCount;
    const char** strings;
  } enumeration;

  struct
  {
    int min;
    int max;
  } integer;

  struct
  {
    float min;
    float max;
  } value;

  struct
  {
    float min;
    float max;
  } vector;
};

// 796
//
struct dvar_t
{
  const char *name;
  const char *description;
  unsigned int flags;
  char type;
  bool modified;
  DvarValue current;
  DvarValue latched;
  DvarValue reset;
  DvarLimits domain;
  bool (__cdecl *domainFunc)(dvar_t *, DvarValue);
  dvar_t *hashNext;
};

struct snd_alias_list_t
{
  const char *aliasName;
  snd_alias_t *head;
  int count;
};

// 962
//
struct GfxDrawSurfFields
{
  unsigned __int64 objectId : 16;
  unsigned __int64 reflectionProbeIndex : 8;
  unsigned __int64 hasGfxEntIndex : 1;
  unsigned __int64 customIndex : 5;
  unsigned __int64 materialSortedIndex : 12;
  unsigned __int64 prepass : 2;
  unsigned __int64 useHeroLighting : 1;
  unsigned __int64 sceneLightIndex : 8;
  unsigned __int64 surfType : 4;
  unsigned __int64 primarySortKey : 6;
  unsigned __int64 unused : 1;
};

// 963
//
union GfxDrawSurf
{
  GfxDrawSurfFields fields;
  unsigned __int64 packed;
};

// 964
//
struct MaterialInfo
{
  const char *name;
  char gameFlags;
  char sortKey;
  char textureAtlasRowCount;
  char textureAtlasColumnCount;
  GfxDrawSurf drawSurf;
  unsigned int surfaceTypeBits;
  unsigned short hashIndex;
};

// 965
//
struct MaterialTechniqueSet
{
  const char *name;
  char worldVertFormat;
  bool hasBeenUploaded;
  char unused[1];
  MaterialTechniqueSet *remappedTechniqueSet;
  MaterialTechnique *techniques[48];
};

// 966
//
struct MaterialStreamRouting
{
  char source;
  char dest;
};

// 968
//
struct MaterialVertexStreamRouting
{
  MaterialStreamRouting data[13];
  IDirect3DVertexDeclaration9 *decl[16];
};

// 969
//
struct MaterialVertexDeclaration
{
  const char *name;
  char streamCount;
  bool hasOptionalSource;
  MaterialVertexStreamRouting routing;
};

// 971
//
struct GfxVertexShaderLoadDef
{
  unsigned int *program;
  unsigned short programSize;
  unsigned short loadForRenderer;
};

// 972
//
struct MaterialVertexShaderProgram
{
  IDirect3DVertexShader9 *vs;
  GfxVertexShaderLoadDef loadDef;
};

// 973
//
struct MaterialVertexShader
{
  const char *name;
  MaterialVertexShaderProgram prog;
};

// 975
//
struct GfxPixelShaderLoadDef
{
  unsigned int *program;
  unsigned short programSize;
  unsigned short loadForRenderer;
};

// 976
//
struct MaterialPixelShaderProgram
{
  IDirect3DPixelShader9 *ps;
  GfxPixelShaderLoadDef loadDef;
};

// 977
//
struct MaterialPixelShader
{
  const char *name;
  MaterialPixelShaderProgram prog;
};

// 978
//
struct MaterialArgumentCodeConst
{
  unsigned short index;
  char firstRow;
  char rowCount;
};

// 979
//
union MaterialArgumentDef
{
  const float *literalConst;
  MaterialArgumentCodeConst codeConst;
  unsigned int codeSampler;
  unsigned int nameHash;
};

// 980
//
struct MaterialShaderArgument
{
  unsigned short type;
  unsigned short dest;
  MaterialArgumentDef u;
};

// 981
//
struct MaterialPass
{
  MaterialVertexDeclaration *vertexDecl;
  MaterialVertexShader *vertexShader;
  MaterialPixelShader *pixelShader;
  char perPrimArgCount;
  char perObjArgCount;
  char stableArgCount;
  char customSamplerFlags;
  MaterialShaderArgument *args;
};

// 982
//
struct MaterialTechnique
{
  const char *name;
  unsigned short flags;
  unsigned short passCount;
  MaterialPass passArray[1];
};

// 988
//
struct GfxImageLoadDef
{
  char levelCount;
  char pad[3];
  int flags;
  int format;
  int resourceSize;
  char data[1];
};

// 989
//
union GfxTexture
{
  IDirect3DBaseTexture9 *basemap;
  IDirect3DTexture9 *map;
  IDirect3DVolumeTexture9 *volmap;
  IDirect3DCubeTexture9 *cubemap;
  GfxImageLoadDef *loadDef;
};

// 990
//
struct Picmip
{
  char platform[2];
};

// 991
//
struct CardMemory
{
  int platform[2];
};

// 992
//
struct GfxImage
{
  GfxTexture texture;
  char mapType;
  char semantic;
  char category;
  bool useSrgbReads;
  Picmip picmip;
  bool noPicmip;
  char track;
  CardMemory cardMemory;
  unsigned short width;
  unsigned short height;
  unsigned short depth;
  bool delayLoadPixels;
  const char *name;
};

// 993
//
struct WaterWritable
{
  float floatTime;
};

// 994
//
struct complex_s
{
  float real;
  float imag;
};

// 995
//
struct water_t
{
  WaterWritable writable;
  complex_s *H0;
  float *wTerm;
  int M;
  int N;
  float Lx;
  float Lz;
  float gravity;
  float windvel;
  float winddir[2];
  float amplitude;
  float codeConstant[4];
  GfxImage *image;
};

// 996
//
union MaterialTextureDefInfo
{
  GfxImage *image;
  water_t *water;
};

// 997
//
struct MaterialTextureDef
{
  unsigned int nameHash;
  char nameStart;
  char nameEnd;
  char samplerState;
  char semantic;
  MaterialTextureDefInfo u;
};

// 998
//
struct MaterialConstantDef
{
  unsigned int nameHash;
  char name[12];
  float literal[4];
};

// 999
//
struct GfxStateBits
{
  unsigned int loadBits[2];
};

// 1000
//
struct Material
{
  MaterialInfo info;
  char stateBitsEntry[48];
  char textureCount;
  char constantCount;
  char stateBitsCount;
  char stateFlags;
  char cameraRegion;
  MaterialTechniqueSet *techniqueSet;
  MaterialTextureDef *textureTable;
  MaterialConstantDef *constantTable;
  GfxStateBits *stateBitsTable;
};

// 1002
//
struct Glyph
{
  unsigned short letter;
  char x0;
  char y0;
  char dx;
  char pixelWidth;
  char pixelHeight;
  float s0;
  float t0;
  float s1;
  float t1;
};

// 1003
//
struct Font_s
{
  const char *fontName;
  int pixelHeight;
  int glyphCount;
  Material *material;
  Material *glowMaterial;
  Glyph *glyphs;
};

// 1060
//
struct cplane_s
{
  float normal[3];
  float dist;
  char type;
  char pad[3];
};

// 1061
//
struct cbrushside_t
{
  cplane_s *plane;
  unsigned short materialNum;
  char firstAdjacentSideOffset;
  char edgeCount;
};

// 1062
//
struct cbrush_t
{
  unsigned short numsides;
  unsigned short glassPieceIndex;
  cbrushside_t *sides;
  char *baseAdjacentSide;
  short axialMaterialNum[2][3];
  char firstAdjacentSideOffsets[2][3];
  char edgeCount[2][3];
};

// 1063
//
struct ProfileScriptWritable
{
  int refCount;
  unsigned int startTime;
  unsigned int totalTime;
};

// 1064
//
struct ProfileScript
{
  ProfileScriptWritable write[40];
  volatile unsigned int totalTime[40];
  volatile unsigned int avgTime[40];
  volatile unsigned int maxTime[40];
  volatile float cumulative[40];
  char profileScriptNames[40][20];
  int scriptSrcBufferIndex[32];
  unsigned int srcTotal;
  unsigned int srcAvgTime;
  unsigned int srcMaxTime;
};

// 1065
//
struct Bounds
{
  float midPoint[3];
  float halfSize[3];
};

// 1066
//
struct BrushWrapper
{
  Bounds bounds;
  cbrush_t brush;
  int totalEdgeCount;
  cplane_s *planes;
};

// 1067
//
struct XModelCollTri_s
{
  float plane[4];
  float svec[4];
  float tvec[4];
};

// 1068
//
struct TriggerSlab
{
  float dir[3];
  float midPoint;
  float halfSize;
};

// 1069
//
struct PhysMass
{
  float centerOfMass[3];
  float momentsOfInertia[3];
  float productsOfInertia[3];
};

// 1070
//
struct TriggerHull
{
  Bounds bounds;
  int contents;
  unsigned short slabCount;
  unsigned short firstSlab;
};

// 1071
//
struct FxSpawnDefLooping
{
  int intervalMsec;
  int count;
};

// 1072
//
struct FxIntRange
{
  int base;
  int amplitude;
};

// 1073
//
struct FxSpawnDefOneShot
{
  FxIntRange count;
};

// 1074
//
union FxSpawnDef
{
  FxSpawnDefLooping looping;
  FxSpawnDefOneShot oneShot;
};

// 1075
//
struct FxFloatRange
{
  float base;
  float amplitude;
};

// 1076
//
struct FxElemAtlas
{
  char behavior;
  char index;
  char fps;
  char loopCount;
  char colIndexBits;
  char rowIndexBits;
  short entryCount;
};

// 1077
//
struct FxElemVec3Range
{
  float base[3];
  float amplitude[3];
};

// 1078
//
struct FxElemVelStateInFrame
{
  FxElemVec3Range velocity;
  FxElemVec3Range totalDelta;
};

// 1079
//
struct FxElemVelStateSample
{
  FxElemVelStateInFrame local;
  FxElemVelStateInFrame world;
};

// 1080
//
struct FxElemVisualState
{
  char color[4];
  float rotationDelta;
  float rotationTotal;
  float size[2];
  float scale;
};

// 1081
//
struct FxElemVisStateSample
{
  FxElemVisualState base;
  FxElemVisualState amplitude;
};

// 1082
//
struct FxElemMarkVisuals
{
  Material *materials[2];
};

// 1083
//
struct DObjAnimMat
{
  float quat[4];
  float trans[3];
  float transWeight;
};

// 1084
//
struct XSurfaceVertexInfo
{
  short vertCount[4];
  unsigned short *vertsBlend;
};

// 1085
//
union GfxColor
{
  unsigned int packed;
  char array[4];
};

// 1086
//
union PackedTexCoords
{
  unsigned int packed;
};

// 1087
//
union PackedUnitVec
{
  unsigned int packed;
  char array[4];
};

// 1088
//
struct GfxPackedVertex
{
  float xyz[3];
  float binormalSign;
  GfxColor color;
  PackedTexCoords texCoord;
  PackedUnitVec normal;
  PackedUnitVec tangent;
};

// 1089
//
struct XSurfaceCollisionAabb
{
  unsigned short mins[3];
  unsigned short maxs[3];
};

// 1090
//
struct XSurfaceCollisionNode
{
  XSurfaceCollisionAabb aabb;
  unsigned short childBeginIndex;
  unsigned short childCount;
};

// 1091
//
struct XSurfaceCollisionLeaf
{
  unsigned short triangleBeginIndex;
};

// 1092
//
struct XSurfaceCollisionTree
{
  float trans[3];
  float scale[3];
  unsigned int nodeCount;
  XSurfaceCollisionNode *nodes;
  unsigned int leafCount;
  XSurfaceCollisionLeaf *leafs;
};

// 1093
//
struct XRigidVertList
{
  unsigned short boneOffset;
  unsigned short vertCount;
  unsigned short triOffset;
  unsigned short triCount;
  XSurfaceCollisionTree *collisionTree;
};

// 1094
//
struct XSurface
{
  char tileMode;
  bool deformed;
  unsigned short vertCount;
  unsigned short triCount;
  char zoneHandle;
  unsigned short baseTriIndex;
  unsigned short baseVertIndex;
  unsigned short *triIndices;
  XSurfaceVertexInfo vertInfo;
  GfxPackedVertex *verts0;
  unsigned int vertListCount;
  XRigidVertList *vertList;
  int partBits[6];
};

// 1095
//
struct XModelSurfs
{
  const char *name;
  XSurface *surfs;
  unsigned short numsurfs;
  int partBits[6];
};

// 1096
//
struct XModelLodInfo
{
  float dist;
  unsigned short numsurfs;
  unsigned short surfIndex;
  XModelSurfs *modelSurfs;
  int partBits[6];
  XSurface *surfs;
  char lod;
  char smcBaseIndexPlusOne;
  char smcSubIndexMask;
  char smcBucket;
};

// 1097
//
struct XModelCollSurf_s
{
  XModelCollTri_s *collTris;
  int numCollTris;
  Bounds bounds;
  int boneIdx;
  int contents;
  int surfFlags;
};

// 1098
//
struct XBoneInfo
{
  Bounds bounds;
  float radiusSquared;
};

// 1099
//
struct PhysPreset
{
  const char *name;
  int type;
  float mass;
  float bounce;
  float friction;
  float bulletForceScale;
  float explosiveForceScale;
  const char *sndAliasPrefix;
  float piecesSpreadFraction;
  float piecesUpwardVelocity;
  bool tempDefaultToCylinder;
  bool perSurfaceSndAlias;
};

// 1100
//
struct PhysGeomInfo
{
  BrushWrapper *brushWrapper;
  int type;
  float orientation[3][3];
  Bounds bounds;
};

// 1101
//
struct PhysCollmap
{
  const char *name;
  unsigned int count;
  PhysGeomInfo *geoms;
  PhysMass mass;
  Bounds bounds;
};

// 1102
//
struct XModel
{
  const char *name;
  char numBones;
  char numRootBones;
  char numsurfs;
  char lodRampType;
  float scale;
  unsigned int noScalePartBits[6];
  unsigned short *boneNames;
  char *parentList;
  short *quats;
  float *trans;
  char *partClassification;
  DObjAnimMat *baseMat;
  Material **materialHandles;
  XModelLodInfo lodInfo[4];
  char maxLoadedLod;
  char numLods;
  char collLod;
  char flags;
  XModelCollSurf_s *collSurfs;
  int numCollSurfs;
  int contents;
  XBoneInfo *boneInfo;
  float radius;
  Bounds bounds;
  int memUsage;
  bool bad;
  PhysPreset *physPreset;
  PhysCollmap *physCollmap;
};

// 1103
//
struct FxEffectDef
{
  const char *name;
  int flags;
  int totalSize;
  int msecLoopingLife;
  int elemDefCountLooping;
  int elemDefCountOneShot;
  int elemDefCountEmission;
  FxElemDef *elemDefs;
};

// 1104
//
union FxEffectDefRef
{
  FxEffectDef *handle;
  const char *name;
};

// 1105
//
union FxElemVisuals
{
  const void *anonymous;
  Material *material;
  XModel *model;
  FxEffectDefRef effectDef;
  const char *soundName;
};

// 1106
//
union FxElemDefVisuals
{
  FxElemMarkVisuals *markArray;
  FxElemVisuals *array;
  FxElemVisuals instance;
};

// 1107
//
struct FxTrailVertex
{
  float pos[2];
  float normal[2];
  float texCoord;
};

// 1108
//
struct FxTrailDef
{
  int scrollTimeMsec;
  int repeatDist;
  float invSplitDist;
  float invSplitArcDist;
  float invSplitTime;
  int vertCount;
  FxTrailVertex *verts;
  int indCount;
  unsigned short *inds;
};

// 1109
//
struct FxSparkFountainDef
{
  float gravity;
  float bounceFrac;
  float bounceRand;
  float sparkSpacing;
  float sparkLength;
  int sparkCount;
  float loopTime;
  float velMin;
  float velMax;
  float velConeFrac;
  float restSpeed;
  float boostTime;
  float boostFactor;
};

// 1110
//
union FxElemExtendedDefPtr
{
  FxTrailDef *trailDef;
  FxSparkFountainDef *sparkFountainDef;
  void *unknownDef;
};

// 1111
//
struct FxElemDef
{
  int flags;
  FxSpawnDef spawn;
  FxFloatRange spawnRange;
  FxFloatRange fadeInRange;
  FxFloatRange fadeOutRange;
  float spawnFrustumCullRadius;
  FxIntRange spawnDelayMsec;
  FxIntRange lifeSpanMsec;
  FxFloatRange spawnOrigin[3];
  FxFloatRange spawnOffsetRadius;
  FxFloatRange spawnOffsetHeight;
  FxFloatRange spawnAngles[3];
  FxFloatRange angularVelocity[3];
  FxFloatRange initialRotation;
  FxFloatRange gravity;
  FxFloatRange reflectionFactor;
  FxElemAtlas atlas;
  char elemType;
  char visualCount;
  char velIntervalCount;
  char visStateIntervalCount;
  FxElemVelStateSample *velSamples;
  FxElemVisStateSample *visSamples;
  FxElemDefVisuals visuals;
  Bounds collBounds;
  FxEffectDefRef effectOnImpact;
  FxEffectDefRef effectOnDeath;
  FxEffectDefRef effectEmitted;
  FxFloatRange emitDist;
  FxFloatRange emitDistVariance;
  FxElemExtendedDefPtr extended;
  char sortOrder;
  char lightingFrac;
  char useItemClip;
  char fadeInfo;
};

// 1112
//
struct FxImpactEntry
{
  FxEffectDef *nonflesh[31];
  FxEffectDef *flesh[4];
};

// 1113
//
struct MigrationPers
{
  int time;
  bool stanceHeld;
  StanceState stance;
  StanceState stancePosition;
  int stanceTime;
  int cgameUserCmdWeapon;
  int cgameUserCmdOffHandIndex;
  unsigned int weaponSelect;
  int weaponSelectTime;
  int weaponForcedSelectTime;
  unsigned int weaponLatestPrimaryIdx;
  unsigned short primaryWeaponForAlt[1400];
  int holdBreathTime;
  int holdBreathInTime;
  int holdBreathDelay;
  float holdBreathFrac;
};

// 1114
//
struct clientUIActive_t
{
  bool active;
  bool isRunning;
  bool cgameInitialized;
  bool cgameInitCalled;
  bool mapPreloaded;
  clientMigState_t migrationState;
  MigrationPers migrationPers;
  MigrationVerboseState verboseMigrationState;
  int verboseMigrationData;
  int keyCatchers;
  bool displayHUDWithKeycatchUI;
  connstate_t connectionState;
  bool invited;
  char itemsUnlocked[256];
  bool itemsUnlockedInited;
  bool itemsUnlockedLastGameDirty;
  unsigned short itemsUnlockedLastGame[16];
  int itemsUnlockedLastGameCount;
  char *itemsUnlockedBuffer;
  int itemsUnlockedLocalClientNum;
  int itemsUnlockedControllerIndex;
  int itemsUnlockedStatsSource;
};

// 1115
//
struct netProfilePacket_t
{
  int iTime;
  int iSize;
  int bFragment;
};

// 1116
//
struct netProfileStream_t
{
  netProfilePacket_t packets[60];
  int iCurrPacket;
  int iBytesPerSecond;
  int iLastBPSCalcTime;
  int iCountedPackets;
  int iCountedFragments;
  int iFragmentPercentage;
  int iLargestPacket;
  int iSmallestPacket;
};

// 1117
//
struct ClientVoicePacket_t
{
  char data[256];
  int dataSize;
};

// 1118
//
struct voiceCommunication_t
{
  ClientVoicePacket_t voicePackets[10];
  int voicePacketCount;
  int voicePacketLastTransmit;
  int packetsPerSec;
  int packetsPerSecStart;
};

// 1119
//
struct MaterialInfoRaw
{
  unsigned int nameOffset;
  unsigned int refImageNameOffset;
  char gameFlags;
  char pad[1];
  char textureAtlasRowCount;
  char textureAtlasColumnCount;
  unsigned int sortKeyNameOffset;
  float maxDeformMove;
  char deformFlags;
  char usage;
  unsigned short toolFlags;
  unsigned int locale;
  unsigned short autoTexScaleWidth;
  unsigned short autoTexScaleHeight;
  float tessSize;
  int surfaceFlags;
  int contents;
};

// 1120
//
struct ProfileAtom
{
  unsigned int value[1];
};

// 1121
//
struct ProfileWritable
{
  int nesting;
  unsigned int hits;
  ProfileAtom start[3];
  ProfileAtom total;
  ProfileAtom child;
};

// 1122
//
struct ProfileReadable
{
  unsigned int hits;
  ProfileAtom total;
  ProfileAtom self;
};

// 1123
//
struct profile_t
{
  ProfileWritable write;
  ProfileReadable read;
};

// 1124
//
struct profile_guard_t
{
  int id;
  profile_t **ppStack;
};

// 1125
//
struct ProfileStack
{
  profile_t prof_root;
  profile_t *prof_pStack[16384];
  profile_t **prof_ppStack;
  profile_t prof_array[443];
  ProfileAtom prof_overhead_internal;
  ProfileAtom prof_overhead_external;
  profile_guard_t prof_guardstack[32];
  int prof_guardpos;
  float prof_timescale;
};

// 1126
//
struct TriggerModel
{
  int contents;
  unsigned short hullCount;
  unsigned short firstHull;
};

// 1127
//
struct FxImpactTable
{
  const char *name;
  FxImpactEntry *table;
};

// 1128
//
union XAnimIndices
{
  char *_1;
  unsigned short *_2;
  void *data;
};

// 1129
//
struct XAnimNotifyInfo
{
  unsigned short name;
  float time;
};

// 1130
//
union XAnimDynamicFrames
{
  char (*_1)[3];
  unsigned short (*_2)[3];
};

// 1131
//
union XAnimDynamicIndices
{
  char _1[1];
  unsigned short _2[1];
};

// 1132
//
struct XAnimPartTransFrames
{
  float mins[3];
  float size[3];
  XAnimDynamicFrames frames;
  XAnimDynamicIndices indices;
};

// 1133
//
union XAnimPartTransData
{
  XAnimPartTransFrames frames;
  float frame0[3];
};

// 1134
//
struct XAnimPartTrans
{
  unsigned short size;
  char smallTrans;
  XAnimPartTransData u;
};

// 1135
//
struct XAnimDeltaPartQuatDataFrames2
{
  short (*frames)[2];
  XAnimDynamicIndices indices;
};

// 1136
//
union XAnimDeltaPartQuatData2
{
  XAnimDeltaPartQuatDataFrames2 frames;
  short frame0[2];
};

// 1137
//
struct XAnimDeltaPartQuat2
{
  unsigned short size;
  XAnimDeltaPartQuatData2 u;
};

// 1138
//
struct XAnimDeltaPartQuatDataFrames
{
  short (*frames)[4];
  XAnimDynamicIndices indices;
};

// 1139
//
union XAnimDeltaPartQuatData
{
  XAnimDeltaPartQuatDataFrames frames;
  short frame0[4];
};

// 1140
//
struct XAnimDeltaPartQuat
{
  unsigned short size;
  XAnimDeltaPartQuatData u;
};

// 1141
//
struct XAnimDeltaPart
{
  XAnimPartTrans *trans;
  XAnimDeltaPartQuat2 *quat2;
  XAnimDeltaPartQuat *quat;
};

// 1142
//
struct XAnimParts
{
  const char *name;
  unsigned short dataByteCount;
  unsigned short dataShortCount;
  unsigned short dataIntCount;
  unsigned short randomDataByteCount;
  unsigned short randomDataIntCount;
  unsigned short numframes;
  char flags;
  char boneCount[10];
  char notifyCount;
  char assetType;
  bool isDefault;
  unsigned int randomDataShortCount;
  unsigned int indexCount;
  float framerate;
  float frequency;
  unsigned short *names;
  char *dataByte;
  short *dataShort;
  int *dataInt;
  short *randomDataShort;
  char *randomDataByte;
  int *randomDataInt;
  XAnimIndices indices;
  XAnimNotifyInfo *notify;
  XAnimDeltaPart *deltaPart;
};

// 1143
//
struct cStaticModel_s
{
  XModel *xmodel;
  float origin[3];
  float invScaledAxis[3][3];
  Bounds absBounds;
};

// 1144
//
struct ClipMaterial
{
  const char *name;
  int surfaceFlags;
  int contents;
};

// 1145
//
struct cNode_t
{
  cplane_s *plane;
  short children[2];
};

// 1146
//
struct cLeaf_t
{
  unsigned short firstCollAabbIndex;
  unsigned short collAabbCount;
  int brushContents;
  int terrainContents;
  Bounds bounds;
  int leafBrushNode;
};

// 1147
//
struct cLeafBrushNodeLeaf_t
{
  unsigned short *brushes;
};

// 1148
//
struct cLeafBrushNodeChildren_t
{
  float dist;
  float range;
  unsigned short childOffset[2];
};

// 1149
//
union cLeafBrushNodeData_t
{
  cLeafBrushNodeLeaf_t leaf;
  cLeafBrushNodeChildren_t children;
};

// 1150
//
struct cLeafBrushNode_s
{
  char axis;
  short leafBrushCount;
  int contents;
  cLeafBrushNodeData_t data;
};

// 1151
//
struct CollisionBorder
{
  float distEq[3];
  float zBase;
  float zSlope;
  float start;
  float length;
};

// 1152
//
struct CollisionPartition
{
  char triCount;
  char borderCount;
  char firstVertSegment;
  int firstTri;
  CollisionBorder *borders;
};

// 1153
//
union CollisionAabbTreeIndex
{
  int firstChildIndex;
  int partitionIndex;
};

// 1154
//
struct CollisionAabbTree
{
  float midPoint[3];
  unsigned short materialIndex;
  unsigned short childCount;
  float halfSize[3];
  CollisionAabbTreeIndex u;
};

// 1155
//
struct cmodel_t
{
  Bounds bounds;
  float radius;
  cLeaf_t leaf;
};

// 1156
//
struct MapTriggers
{
  unsigned int count;
  TriggerModel *models;
  unsigned int hullCount;
  TriggerHull *hulls;
  unsigned int slabCount;
  TriggerSlab *slabs;
};

// 1157
//
struct Stage
{
  const char *name;
  float origin[3];
  unsigned short triggerIndex;
  char sunPrimaryLightIndex;
};

// 1158
//
struct MapEnts
{
  const char *name;
  char *entityString;
  int numEntityChars;
  MapTriggers trigger;
  Stage *stages;
  char stageCount;
};

// 1159
//
struct SModelAabbNode
{
  Bounds bounds;
  unsigned short firstChild;
  unsigned short childCount;
};

// 1160
//
struct GfxPlacement
{
  float quat[4];
  float origin[3];
};

// 1161
//
struct DynEntityDef
{
  DynEntityType type;
  GfxPlacement pose;
  XModel *xModel;
  unsigned short brushModel;
  unsigned short physicsBrushModel;
  FxEffectDef *destroyFx;
  PhysPreset *physPreset;
  int health;
  PhysMass mass;
  int contents;
};

// 1162
//
struct DynEntityPose
{
  GfxPlacement pose;
  float radius;
};

// 1163
//
struct DynEntityClient
{
  int physObjId;
  unsigned short flags;
  unsigned short lightingHandle;
  int health;
};

// 1164
//
struct DynEntityColl
{
  unsigned short sector;
  unsigned short nextEntInSector;
  float linkMins[2];
  float linkMaxs[2];
};

// 1165
//
struct clipMap_t
{
  const char *name;
  int isInUse;
  int planeCount;
  cplane_s *planes;
  unsigned int numStaticModels;
  cStaticModel_s *staticModelList;
  unsigned int numMaterials;
  ClipMaterial *materials;
  unsigned int numBrushSides;
  cbrushside_t *brushsides;
  unsigned int numBrushEdges;
  char *brushEdges;
  unsigned int numNodes;
  cNode_t *nodes;
  unsigned int numLeafs;
  cLeaf_t *leafs;
  unsigned int leafbrushNodesCount;
  cLeafBrushNode_s *leafbrushNodes;
  unsigned int numLeafBrushes;
  unsigned short *leafbrushes;
  unsigned int numLeafSurfaces;
  unsigned int *leafsurfaces;
  unsigned int vertCount;
  float (*verts)[3];
  int triCount;
  unsigned short *triIndices;
  char *triEdgeIsWalkable;
  int borderCount;
  CollisionBorder *borders;
  int partitionCount;
  CollisionPartition *partitions;
  int aabbTreeCount;
  CollisionAabbTree *aabbTrees;
  unsigned int numSubModels;
  cmodel_t *cmodels;
  unsigned short numBrushes;
  cbrush_t *brushes;
  Bounds *brushBounds;
  int *brushContents;
  MapEnts *mapEnts;
  unsigned short smodelNodeCount;
  SModelAabbNode *smodelNodes;
  unsigned short dynEntCount[2];
  DynEntityDef *dynEntDefList[2];
  DynEntityPose *dynEntPoseList[2];
  DynEntityClient *dynEntClientList[2];
  DynEntityColl *dynEntCollList[2];
  unsigned int checksum;
};

// 1166
//
struct ComPrimaryLight
{
  char type;
  char canUseShadowMap;
  char exponent;
  char unused;
  float color[3];
  float dir[3];
  float origin[3];
  float radius;
  float cosHalfFovOuter;
  float cosHalfFovInner;
  float cosHalfFovExpanded;
  float rotationLimit;
  float translationLimit;
  const char *defName;
};

// 1167
//
struct ComWorld
{
  const char *name;
  int isInUse;
  unsigned int primaryLightCount;
  ComPrimaryLight *primaryLights;
};

// 1168
//
struct pathlink_s
{
  float fDist;
  unsigned short nodeNum;
  char disconnectCount;
  char negotiationLink;
  char flags;
  char ubBadPlaceCount[3];
};

// 1170
//
struct pathnode_constant_t
{
  nodeType type;
  unsigned short spawnflags;
  unsigned short targetname;
  unsigned short script_linkName;
  unsigned short script_noteworthy;
  unsigned short target;
  unsigned short animscript;
  int animscriptfunc;
  float vOrigin[3];
  float fAngle;
  float forward[2];
  float fRadius;
  union
  {
    float minUseDistSq;
    PathNodeErrorCode error;
  };
  short wOverlapNode[2];
  unsigned short totalLinkCount;
  pathlink_s *Links;
};

// 1171
//
struct pathnode_dynamic_t
{
  void *pOwner;
  int iFreeTime;
  int iValidTime[3];
  int dangerousNodeTime[3];
  int inPlayerLOSTime;
  short wLinkCount;
  short wOverlapCount;
  short turretEntNumber;
  char userCount;
  bool hasBadPlaceLink;
};

// 1174
//
struct pathnode_transient_t
{
  int iSearchFrame;
  pathnode_t *pNextOpen;
  pathnode_t *pPrevOpen;
  pathnode_t *pParent;
  float fCost;
  float fHeuristic;
  union
  {
    float nodeCost;
    int linkIndex;
  };
};

// 1172
//
struct pathnode_t
{
  pathnode_constant_t constant;
  pathnode_dynamic_t dynamic;
  pathnode_transient_t transient;
};

// 1175
//
struct pathbasenode_t
{
  float vOrigin[3];
  unsigned int type;
};

// 1177
//
struct pathnode_tree_nodes_t
{
  int nodeCount;
  unsigned short *nodes;
};

// 1178
//
union pathnode_tree_info_t
{
  pathnode_tree_t *child[2];
  pathnode_tree_nodes_t s;
};

// 1176
//
struct pathnode_tree_t
{
  int axis;
  float dist;
  pathnode_tree_info_t u;
};



// 1179
//
struct PathData
{
  unsigned int nodeCount;
  pathnode_t *nodes;
  pathbasenode_t *basenodes;
  unsigned int chainNodeCount;
  unsigned short *chainNodeForNode;
  unsigned short *nodeForChainNode;
  int visBytes;
  char *pathVis;
  int nodeTreeCount;
  pathnode_tree_t *nodeTree;
};

// 1180
//
struct VehicleTrackObstacle
{
  float origin[2];
  float radius;
};

// 1181
//
struct VehicleTrackSector
{
  float startEdgeDir[2];
  float startEdgeDist;
  float leftEdgeDir[2];
  float leftEdgeDist;
  float rightEdgeDir[2];
  float rightEdgeDist;
  float sectorLength;
  float sectorWidth;
  float totalPriorLength;
  float totalFollowingLength;
  VehicleTrackObstacle *obstacles;
  unsigned int obstacleCount;
};

// 1182
//
struct VehicleTrackSegment
{
  const char *targetName;
  VehicleTrackSector *sectors;
  unsigned int sectorCount;
  VehicleTrackSegment **nextBranches;
  unsigned int nextBranchesCount;
  VehicleTrackSegment **prevBranches;
  unsigned int prevBranchesCount;
  float endEdgeDir[2];
  float endEdgeDist;
  float totalLength;
};

// 1183
//
struct VehicleTrack
{
  VehicleTrackSegment *segments;
  unsigned int segmentCount;
};

// 1184
//
struct G_GlassPiece
{
  unsigned short damageTaken;
  unsigned short collapseTime;
  int lastStateChangeTime;
  char impactDir;
  char impactPos[2];
};

// 1185
//
struct G_GlassName
{
  char *nameStr;
  unsigned short name;
  unsigned short pieceCount;
  unsigned short *pieceIndices;
};

// 1186
//
struct G_GlassData
{
  G_GlassPiece *glassPieces;
  unsigned int pieceCount;
  unsigned short damageToWeaken;
  unsigned short damageToDestroy;
  unsigned int glassNameCount;
  G_GlassName *glassNames;
  char pad[108];
};

// 1187
//
struct GameWorldSp
{
  const char *name;
  PathData path;
  VehicleTrack vehicleTrack;
  G_GlassData *g_glassData;
};

// 1188
//
struct GameWorldMp
{
  const char *name;
  G_GlassData *g_glassData;
};

// 1189
//
struct FxGlassDef
{
  float halfThickness;
  float texVecs[2][2];
  GfxColor color;
  Material *material;
  Material *materialShattered;
  PhysPreset *physPreset;
};

// 1190
//
struct FxSpatialFrame
{
  float quat[4];
  float origin[3];
};

// 1191
//
struct $E43DBA5037697D705289B74D87E76C70
{
  FxSpatialFrame frame;
  float radius;
};

// 1192
//
union FxGlassPiecePlace
{
  $E43DBA5037697D705289B74D87E76C70 __s0;
  unsigned int nextFree;
};

// 1193
//
struct FxGlassPieceState
{
  float texCoordOrigin[2];
  unsigned int supportMask;
  unsigned short initIndex;
  unsigned short geoDataStart;
  char defIndex;
  char pad[5];
  char vertCount;
  char holeDataCount;
  char crackDataCount;
  char fanDataCount;
  unsigned short flags;
  float areaX2;
};

// 1194
//
struct FxGlassPieceDynamics
{
  int fallTime;
  int physObjId;
  int physJointId;
  float vel[3];
  float avel[3];
};

// 1195
//
struct FxGlassVertex
{
  short x;
  short y;
};

// 1196
//
struct FxGlassHoleHeader
{
  unsigned short uniqueVertCount;
  char touchVert;
  char pad[1];
};

// 1197
//
struct FxGlassCrackHeader
{
  unsigned short uniqueVertCount;
  char beginVertIndex;
  char endVertIndex;
};

// 1198
//
union FxGlassGeometryData
{
  FxGlassVertex vert;
  FxGlassHoleHeader hole;
  FxGlassCrackHeader crack;
  char asBytes[4];
  short anonymous[2];
};

// 1199
//
struct FxGlassInitPieceState
{
  FxSpatialFrame frame;
  float radius;
  float texCoordOrigin[2];
  unsigned int supportMask;
  float areaX2;
  char defIndex;
  char vertCount;
  char fanDataCount;
  char pad[1];
};

// 1200
//
struct FxGlassSystem
{
  int time;
  int prevTime;
  unsigned int defCount;
  unsigned int pieceLimit;
  unsigned int pieceWordCount;
  unsigned int initPieceCount;
  unsigned int cellCount;
  unsigned int activePieceCount;
  unsigned int firstFreePiece;
  unsigned int geoDataLimit;
  unsigned int geoDataCount;
  unsigned int initGeoDataCount;
  FxGlassDef *defs;
  FxGlassPiecePlace *piecePlaces;
  FxGlassPieceState *pieceStates;
  FxGlassPieceDynamics *pieceDynamics;
  FxGlassGeometryData *geoData;
  unsigned int *isInUse;
  unsigned int *cellBits;
  char *visData;
  float (*linkOrg)[3];
  float *halfThickness;
  unsigned short *lightingHandles;
  FxGlassInitPieceState *initPieceStates;
  FxGlassGeometryData *initGeoData;
  bool needToCompactData;
  char initCount;
  float effectChanceAccum;
  int lastPieceDeletionTime;
};

// 1201
//
struct FxWorld
{
  const char *name;
  FxGlassSystem glassSys;
};

// 1202
//
struct GfxSky
{
  int skySurfCount;
  int *skyStartSurfs;
  GfxImage *skyImage;
  char skySamplerState;
};

// 1203
//
struct GfxWorldDpvsPlanes
{
  int cellCount;
  cplane_s *planes;
  unsigned short *nodes;
  unsigned int *sceneEntCellBits;
};

// 1204
//
struct GfxCellTreeCount
{
  int aabbTreeCount;
};

// 1205
//
struct GfxAabbTree
{
  Bounds bounds;
  unsigned short childCount;
  unsigned short surfaceCount;
  unsigned short startSurfIndex;
  unsigned short surfaceCountNoDecal;
  unsigned short startSurfIndexNoDecal;
  unsigned short smodelIndexCount;
  unsigned short *smodelIndexes;
  int childrenOffset;
};

// 1206
//
struct GfxCellTree
{
  GfxAabbTree *aabbTree;
};

// 1208
//
struct GfxPortalWritable
{
  bool isQueued;
  bool isAncestor;
  char recursionDepth;
  char hullPointCount;
  float (*hullPoints)[2];
  GfxPortal *queuedParent;
};

// 1209
//
struct DpvsPlane
{
  float coeffs[4];
};

// 1207
//
struct GfxPortal
{
  GfxPortalWritable writable;
  DpvsPlane plane;
  float (*vertices)[3];
  unsigned short cellIndex;
  char vertexCount;
  float hullAxis[2][3];
};

// 1210
//
struct GfxCell
{
  Bounds bounds;
  int portalCount;
  GfxPortal *portals;
  char reflectionProbeCount;
  char *reflectionProbes;
};

// 1211
//
struct GfxReflectionProbe
{
  float origin[3];
};

// 1212
//
struct GfxLightmapArray
{
  GfxImage *primary;
  GfxImage *secondary;
};

// 1213
//
struct GfxWorldVertex
{
  float xyz[3];
  float binormalSign;
  GfxColor color;
  float texCoord[2];
  float lmapCoord[2];
  PackedUnitVec normal;
  PackedUnitVec tangent;
};

// 1215
//
struct GfxWorldVertexData
{
  GfxWorldVertex *vertices;
  IDirect3DVertexBuffer9 *worldVb;
};

// 1216
//
struct GfxWorldVertexLayerData
{
  char *data;
  IDirect3DVertexBuffer9 *layerVb;
};

// 1217
//
struct GfxWorldDraw
{
  unsigned int reflectionProbeCount;
  GfxImage **reflectionProbes;
  GfxReflectionProbe *reflectionProbeOrigins;
  GfxTexture *reflectionProbeTextures;
  int lightmapCount;
  GfxLightmapArray *lightmaps;
  GfxTexture *lightmapPrimaryTextures;
  GfxTexture *lightmapSecondaryTextures;
  GfxImage *lightmapOverridePrimary;
  GfxImage *lightmapOverrideSecondary;
  unsigned int vertexCount;
  GfxWorldVertexData vd;
  unsigned int vertexLayerDataSize;
  GfxWorldVertexLayerData vld;
  unsigned int indexCount;
  unsigned short *indices;
};

// 1218
//
struct GfxLightGridEntry
{
  unsigned short colorsIndex;
  char primaryLightIndex;
  char needsTrace;
};

// 1219
//
struct GfxLightGridColors
{
  char rgb[56][3];
};

// 1220
//
struct GfxLightGrid
{
  bool hasLightRegions;
  unsigned int lastSunPrimaryLightIndex;
  unsigned short mins[3];
  unsigned short maxs[3];
  unsigned int rowAxis;
  unsigned int colAxis;
  unsigned short *rowDataStart;
  unsigned int rawRowDataSize;
  char *rawRowData;
  unsigned int entryCount;
  GfxLightGridEntry *entries;
  unsigned int colorCount;
  GfxLightGridColors *colors;
};

// 1221
//
struct GfxBrushModelWritable
{
  Bounds bounds;
};

// 1222
//
struct GfxBrushModel
{
  GfxBrushModelWritable writable;
  Bounds bounds;
  float radius;
  unsigned short surfaceCount;
  unsigned short startSurfIndex;
  unsigned short surfaceCountNoDecal;
};

// 1223
//
struct MaterialMemory
{
  Material *material;
  int memory;
};

// 1224
//
struct sunflare_t
{
  bool hasValidData;
  Material *spriteMaterial;
  Material *flareMaterial;
  float spriteSize;
  float flareMinSize;
  float flareMinDot;
  float flareMaxSize;
  float flareMaxDot;
  float flareMaxAlpha;
  int flareFadeInTime;
  int flareFadeOutTime;
  float blindMinDot;
  float blindMaxDot;
  float blindMaxDarken;
  int blindFadeInTime;
  int blindFadeOutTime;
  float glareMinDot;
  float glareMaxDot;
  float glareMaxLighten;
  int glareFadeInTime;
  int glareFadeOutTime;
  float sunFxPosition[3];
};

// 1225
//
struct XModelDrawInfo
{
  char hasGfxEntIndex;
  char lod;
  unsigned short surfId;
};

// 1226
//
struct GfxSceneDynModel
{
  XModelDrawInfo info;
  unsigned short dynEntId;
};

// 1227
//
struct BModelDrawInfo
{
  unsigned short surfId;
};

// 1228
//
struct GfxSceneDynBrush
{
  BModelDrawInfo info;
  unsigned short dynEntId;
};

// 1229
//
struct GfxShadowGeometry
{
  unsigned short surfaceCount;
  unsigned short smodelCount;
  unsigned short *sortedSurfIndex;
  unsigned short *smodelIndex;
};

// 1230
//
struct GfxLightRegionAxis
{
  float dir[3];
  float midPoint;
  float halfSize;
};

// 1231
//
struct GfxLightRegionHull
{
  float kdopMidPoint[9];
  float kdopHalfSize[9];
  unsigned int axisCount;
  GfxLightRegionAxis *axis;
};

// 1232
//
struct GfxLightRegion
{
  unsigned int hullCount;
  GfxLightRegionHull *hulls;
};

// 1233
//
struct GfxStaticModelInst
{
  Bounds bounds;
  float lightingOrigin[3];
};

// 1234
//
struct srfTriangles_t
{
  unsigned int vertexLayerData;
  unsigned int firstVertex;
  unsigned short vertexCount;
  unsigned short triCount;
  unsigned int baseIndex;
};

// 1235
//
struct GfxSurfaceLightingAndFlagsFields
{
  char lightmapIndex;
  char reflectionProbeIndex;
  char primaryLightIndex;
  char flags;
};

// 1236
//
union GfxSurfaceLightingAndFlags
{
  GfxSurfaceLightingAndFlagsFields fields;
  unsigned int packed;
};

// 1237
//
struct GfxSurface
{
  srfTriangles_t tris;
  Material *material;
  GfxSurfaceLightingAndFlags laf;
};

// 1238
//
struct GfxSurfaceBounds
{
  Bounds bounds;
};

// 1239
//
struct GfxPackedPlacement
{
  float origin[3];
  float axis[3][3];
  float scale;
};

// 1240
//
struct GfxStaticModelDrawInst
{
  GfxPackedPlacement placement;
  XModel *model;
  unsigned short cullDist;
  unsigned short lightingHandle;
  char reflectionProbeIndex;
  char primaryLightIndex;
  char flags;
  char firstMtlSkinIndex;
  GfxColor groundLighting;
  unsigned short cacheId[4];
};

// 1241
//
struct GfxWorldDpvsStatic
{
  unsigned int smodelCount;
  unsigned int staticSurfaceCount;
  unsigned int staticSurfaceCountNoDecal;
  unsigned int litOpaqueSurfsBegin;
  unsigned int litOpaqueSurfsEnd;
  unsigned int litTransSurfsBegin;
  unsigned int litTransSurfsEnd;
  unsigned int shadowCasterSurfsBegin;
  unsigned int shadowCasterSurfsEnd;
  unsigned int emissiveSurfsBegin;
  unsigned int emissiveSurfsEnd;
  unsigned int smodelVisDataCount;
  unsigned int surfaceVisDataCount;
  char *smodelVisData[3];
  char *surfaceVisData[3];
  unsigned short *sortedSurfIndex;
  GfxStaticModelInst *smodelInsts;
  GfxSurface *surfaces;
  GfxSurfaceBounds *surfacesBounds;
  GfxStaticModelDrawInst *smodelDrawInsts;
  GfxDrawSurf *surfaceMaterials;
  unsigned int *surfaceCastsSunShadow;
  volatile int usageCount;
};

// 1242
//
struct GfxWorldDpvsDynamic
{
  unsigned int dynEntClientWordCount[2];
  unsigned int dynEntClientCount[2];
  unsigned int *dynEntCellBits[2];
  char *dynEntVisData[2][3];
};

// 1243
//
struct GfxHeroOnlyLight
{
  char type;
  char unused[3];
  float color[3];
  float dir[3];
  float origin[3];
  float radius;
  float cosHalfFovOuter;
  float cosHalfFovInner;
  int exponent;
};

// 1244
//
struct GfxWorld
{
  const char *name;
  const char *baseName;
  int planeCount;
  int nodeCount;
  unsigned int surfaceCount;
  int skyCount;
  GfxSky *skies;
  unsigned int lastSunPrimaryLightIndex;
  unsigned int primaryLightCount;
  unsigned int sortKeyLitDecal;
  unsigned int sortKeyEffectDecal;
  unsigned int sortKeyEffectAuto;
  unsigned int sortKeyDistortion;
  GfxWorldDpvsPlanes dpvsPlanes;
  GfxCellTreeCount *aabbTreeCounts;
  GfxCellTree *aabbTrees;
  GfxCell *cells;
  GfxWorldDraw draw;
  GfxLightGrid lightGrid;
  int modelCount;
  GfxBrushModel *models;
  Bounds bounds;
  unsigned int checksum;
  int materialMemoryCount;
  MaterialMemory *materialMemory;
  sunflare_t sun;
  float outdoorLookupMatrix[4][4];
  GfxImage *outdoorImage;
  unsigned int *cellCasterBits;
  unsigned int *cellHasSunLitSurfsBits;
  GfxSceneDynModel *sceneDynModel;
  GfxSceneDynBrush *sceneDynBrush;
  unsigned int *primaryLightEntityShadowVis;
  unsigned int *primaryLightDynEntShadowVis[2];
  char *nonSunPrimaryLightForModelDynEnt;
  GfxShadowGeometry *shadowGeom;
  GfxLightRegion *lightRegion;
  GfxWorldDpvsStatic dpvs;
  GfxWorldDpvsDynamic dpvsDyn;
  unsigned int mapVtxChecksum;
  unsigned int heroOnlyLightCount;
  GfxHeroOnlyLight *heroOnlyLights;
  char fogTypesAllowed;
};

// 1245
//
struct GfxLightImage
{
  GfxImage *image;
  char samplerState;
};

// 1246
//
struct GfxLightDef
{
  const char *name;
  GfxLightImage attenuation;
  int lmapLookupStart;
};

// 1247
//
struct rectDef_s
{
  float x;
  float y;
  float w;
  float h;
  char horzAlign;
  char vertAlign;
};

// 1248
//
struct windowDef_t
{
  const char *name;
  rectDef_s rect;
  rectDef_s rectClient;
  const char *group;
  int style;
  int border;
  int ownerDraw;
  int ownerDrawFlags;
  float borderSize;
  int staticFlags;
  int dynamicFlags[1];
  int nextTime;
  float foreColor[4];
  float backColor[4];
  float borderColor[4];
  float outlineColor[4];
  float disableColor[4];
  Material *background;
};

// 1249
//
struct MenuEventHandlerSet
{
  int eventHandlerCount;
  MenuEventHandler **eventHandlers;
};

// 1250
//
struct ExpressionString
{
  const char *string;
};

// 1252
//
union operandInternalDataUnion
{
  int intVal;
  float floatVal;
  ExpressionString stringVal;
  Statement_s *function;
};

// 1253
//
struct Operand
{
  expDataType dataType;
  operandInternalDataUnion internals;
};

// 1251
//
struct Statement_s
{
  int numEntries;
  expressionEntry *entries;
  ExpressionSupportingData *supportingData;
  int lastExecuteTime;
  Operand lastResult;
};

// 1254
//
union entryInternalData
{
  int op;
  Operand operand;
};

// 1255
//
struct expressionEntry
{
  int type;
  entryInternalData data;
};

// 1256
//
struct UIFunctionList
{
  int totalFunctions;
  Statement_s **functions;
};

// 1257
//
struct StaticDvar
{
  dvar_t *dvar;
  char *dvarName;
};

// 1258
//
struct StaticDvarList
{
  int numStaticDvars;
  StaticDvar **staticDvars;
};

// 1259
//
struct StringList
{
  int totalStrings;
  const char **strings;
};

// 1260
//
struct ExpressionSupportingData
{
  UIFunctionList uifunctions;
  StaticDvarList staticDvarList;
  StringList uiStrings;
};

// 1261
//
struct ConditionalScript
{
  MenuEventHandlerSet *eventHandlerSet;
  Statement_s *eventExpression;
};

// 1262
//
struct SetLocalVarData
{
  const char *localVarName;
  Statement_s *expression;
};

// 1263
//
union EventData
{
  const char *unconditionalScript;
  ConditionalScript *conditionalScript;
  MenuEventHandlerSet *elseScript;
  SetLocalVarData *setLocalVarData;
};

// 1264
//
struct MenuEventHandler
{
  EventData eventData;
  char eventType;
};

// 1265
//
struct ItemKeyHandler
{
  int key;
  MenuEventHandlerSet *action;
  ItemKeyHandler *next;
};

// 1276
//
struct menuTransition
{
  int transitionType;
  int targetField;
  int startTime;
  float startVal;
  float endVal;
  float time;
  int endTriggerType;
};

// 1266
//
struct menuDef_t
{
  windowDef_t window;
  const char *font;
  int fullScreen;
  int itemCount;
  int fontIndex;
  int cursorItem[1];
  int fadeCycle;
  float fadeClamp;
  float fadeAmount;
  float fadeInAmount;
  float blurRadius;
  MenuEventHandlerSet *onOpen;
  MenuEventHandlerSet *onCloseRequest;
  MenuEventHandlerSet *onClose;
  MenuEventHandlerSet *onESC;
  ItemKeyHandler *onKey;
  Statement_s *visibleExp;
  const char *allowedBinding;
  const char *soundName;
  int imageTrack;
  float focusColor[4];
  Statement_s *rectXExp;
  Statement_s *rectYExp;
  Statement_s *rectWExp;
  Statement_s *rectHExp;
  Statement_s *openSoundExp;
  Statement_s *closeSoundExp;
  itemDef_s **items;
  menuTransition scaleTransition[1];
  menuTransition alphaTransition[1];
  menuTransition xTransition[1];
  menuTransition yTransition[1];
  ExpressionSupportingData *expressionData;
};

// 1267
//
struct columnInfo_s
{
  int pos;
  int width;
  int maxChars;
  int alignment;
};

// 1268
//
struct listBoxDef_s
{
  int mousePos;
  int startPos[1];
  int endPos[1];
  int drawPadding;
  float elementWidth;
  float elementHeight;
  int elementStyle;
  int numColumns;
  columnInfo_s columnInfo[16];
  MenuEventHandlerSet *onDoubleClick;
  int notselectable;
  int noScrollBars;
  int usePaging;
  float selectBorder[4];
  Material *selectIcon;
};

// 1269
//
struct editFieldDef_s
{
  float minVal;
  float maxVal;
  float defVal;
  float range;
  int maxChars;
  int maxCharsGotoNext;
  int maxPaintChars;
  int paintOffset;
};

// 1270
//
struct multiDef_s
{
  const char *dvarList[32];
  const char *dvarStr[32];
  float dvarValue[32];
  int count;
  int strDef;
};

// 1271
//
struct newsTickerDef_s
{
  int feedId;
  int speed;
  int spacing;
  int lastTime;
  int start;
  int end;
  float x;
};

// 1272
//
struct textScrollDef_s
{
  int startTime;
};

// 1273
//
union itemDefData_t
{
  listBoxDef_s *listBox;
  editFieldDef_s *editField;
  multiDef_s *multi;
  const char *enumDvarName;
  newsTickerDef_s *ticker;
  textScrollDef_s *scroll;
  void *data;
};

// 1274
//
struct ItemFloatExpression
{
  int target;
  Statement_s *expression;
};

// 1275
//
struct itemDef_s
{
  windowDef_t window;
  rectDef_s textRect[1];
  int type;
  int dataType;
  int alignment;
  int fontEnum;
  int textAlignMode;
  float textalignx;
  float textaligny;
  float textscale;
  int textStyle;
  int gameMsgWindowIndex;
  int gameMsgWindowMode;
  const char *text;
  int itemFlags;
  menuDef_t *parent;
  MenuEventHandlerSet *mouseEnterText;
  MenuEventHandlerSet *mouseExitText;
  MenuEventHandlerSet *mouseEnter;
  MenuEventHandlerSet *mouseExit;
  MenuEventHandlerSet *action;
  MenuEventHandlerSet *accept;
  MenuEventHandlerSet *onFocus;
  MenuEventHandlerSet *leaveFocus;
  const char *dvar;
  const char *dvarTest;
  ItemKeyHandler *onKey;
  const char *enableDvar;
  const char *localVar;
  int dvarFlags;
  snd_alias_list_t *focusSound;
  float special;
  int cursorPos[1];
  itemDefData_t typeData;
  int imageTrack;
  int floatExpressionCount;
  ItemFloatExpression *floatExpressions;
  Statement_s *visibleExp;
  Statement_s *disabledExp;
  Statement_s *textExp;
  Statement_s *materialExp;
  float glowColor[4];
  bool decayActive;
  int fxBirthTime;
  int fxLetterTime;
  int fxDecayStartTime;
  int fxDecayDuration;
  int lastSoundPlayedTime;
};

// 1277
//
struct MenuList
{
  const char *name;
  int menuCount;
  menuDef_t **menus;
};

// 1278
//
struct LocalizeEntry
{
  const char *value;
  const char *name;
};

// 1279
//
struct TracerDef
{
  const char *name;
  Material *material;
  unsigned int drawInterval;
  float speed;
  float beamLength;
  float beamWidth;
  float screwRadius;
  float screwDist;
  float colors[5][4];
};

// 1280
//
struct WeaponDef
{
  const char *szOverlayName;
  XModel **gunXModel;
  XModel *handXModel;
  const char **szXAnimsRightHanded;
  const char **szXAnimsLeftHanded;
  const char *szModeName;
  unsigned short *notetrackSoundMapKeys;
  unsigned short *notetrackSoundMapValues;
  unsigned short *notetrackRumbleMapKeys;
  unsigned short *notetrackRumbleMapValues;
  int playerAnimType;
  weapType_t weapType;
  weapClass_t weapClass;
  PenetrateType penetrateType;
  weapInventoryType_t inventoryType;
  weapFireType_t fireType;
  OffhandClass offhandClass;
  weapStance_t stance;
  FxEffectDef *viewFlashEffect;
  FxEffectDef *worldFlashEffect;
  snd_alias_list_t *pickupSound;
  snd_alias_list_t *pickupSoundPlayer;
  snd_alias_list_t *ammoPickupSound;
  snd_alias_list_t *ammoPickupSoundPlayer;
  snd_alias_list_t *projectileSound;
  snd_alias_list_t *pullbackSound;
  snd_alias_list_t *pullbackSoundPlayer;
  snd_alias_list_t *fireSound;
  snd_alias_list_t *fireSoundPlayer;
  snd_alias_list_t *fireSoundPlayerAkimbo;
  snd_alias_list_t *fireLoopSound;
  snd_alias_list_t *fireLoopSoundPlayer;
  snd_alias_list_t *fireStopSound;
  snd_alias_list_t *fireStopSoundPlayer;
  snd_alias_list_t *fireLastSound;
  snd_alias_list_t *fireLastSoundPlayer;
  snd_alias_list_t *emptyFireSound;
  snd_alias_list_t *emptyFireSoundPlayer;
  snd_alias_list_t *meleeSwipeSound;
  snd_alias_list_t *meleeSwipeSoundPlayer;
  snd_alias_list_t *meleeHitSound;
  snd_alias_list_t *meleeMissSound;
  snd_alias_list_t *rechamberSound;
  snd_alias_list_t *rechamberSoundPlayer;
  snd_alias_list_t *reloadSound;
  snd_alias_list_t *reloadSoundPlayer;
  snd_alias_list_t *reloadEmptySound;
  snd_alias_list_t *reloadEmptySoundPlayer;
  snd_alias_list_t *reloadStartSound;
  snd_alias_list_t *reloadStartSoundPlayer;
  snd_alias_list_t *reloadEndSound;
  snd_alias_list_t *reloadEndSoundPlayer;
  snd_alias_list_t *detonateSound;
  snd_alias_list_t *detonateSoundPlayer;
  snd_alias_list_t *nightVisionWearSound;
  snd_alias_list_t *nightVisionWearSoundPlayer;
  snd_alias_list_t *nightVisionRemoveSound;
  snd_alias_list_t *nightVisionRemoveSoundPlayer;
  snd_alias_list_t *altSwitchSound;
  snd_alias_list_t *altSwitchSoundPlayer;
  snd_alias_list_t *raiseSound;
  snd_alias_list_t *raiseSoundPlayer;
  snd_alias_list_t *firstRaiseSound;
  snd_alias_list_t *firstRaiseSoundPlayer;
  snd_alias_list_t *putawaySound;
  snd_alias_list_t *putawaySoundPlayer;
  snd_alias_list_t *scanSound;
  snd_alias_list_t **bounceSound;
  FxEffectDef *viewShellEjectEffect;
  FxEffectDef *worldShellEjectEffect;
  FxEffectDef *viewLastShotEjectEffect;
  FxEffectDef *worldLastShotEjectEffect;
  Material *reticleCenter;
  Material *reticleSide;
  int iReticleCenterSize;
  int iReticleSideSize;
  int iReticleMinOfs;
  activeReticleType_t activeReticleType;
  float vStandMove[3];
  float vStandRot[3];
  float strafeMove[3];
  float strafeRot[3];
  float vDuckedOfs[3];
  float vDuckedMove[3];
  float vDuckedRot[3];
  float vProneOfs[3];
  float vProneMove[3];
  float vProneRot[3];
  float fPosMoveRate;
  float fPosProneMoveRate;
  float fStandMoveMinSpeed;
  float fDuckedMoveMinSpeed;
  float fProneMoveMinSpeed;
  float fPosRotRate;
  float fPosProneRotRate;
  float fStandRotMinSpeed;
  float fDuckedRotMinSpeed;
  float fProneRotMinSpeed;
  XModel **worldModel;
  XModel *worldClipModel;
  XModel *rocketModel;
  XModel *knifeModel;
  XModel *worldKnifeModel;
  Material *hudIcon;
  weaponIconRatioType_t hudIconRatio;
  Material *pickupIcon;
  weaponIconRatioType_t pickupIconRatio;
  Material *ammoCounterIcon;
  weaponIconRatioType_t ammoCounterIconRatio;
  ammoCounterClipType_t ammoCounterClip;
  int iStartAmmo;
  const char *szAmmoName;
  int iAmmoIndex;
  const char *szClipName;
  int iClipIndex;
  int iMaxAmmo;
  int shotCount;
  const char *szSharedAmmoCapName;
  int iSharedAmmoCapIndex;
  int iSharedAmmoCap;
  int damage;
  int playerDamage;
  int iMeleeDamage;
  int iDamageType;
  int iFireDelay;
  int iMeleeDelay;
  int meleeChargeDelay;
  int iDetonateDelay;
  int iRechamberTime;
  int rechamberTimeOneHanded;
  int iRechamberBoltTime;
  int iHoldFireTime;
  int iDetonateTime;
  int iMeleeTime;
  int meleeChargeTime;
  int iReloadTime;
  int reloadShowRocketTime;
  int iReloadEmptyTime;
  int iReloadAddTime;
  int iReloadStartTime;
  int iReloadStartAddTime;
  int iReloadEndTime;
  int iDropTime;
  int iRaiseTime;
  int iAltDropTime;
  int quickDropTime;
  int quickRaiseTime;
  int iBreachRaiseTime;
  int iEmptyRaiseTime;
  int iEmptyDropTime;
  int sprintInTime;
  int sprintLoopTime;
  int sprintOutTime;
  int stunnedTimeBegin;
  int stunnedTimeLoop;
  int stunnedTimeEnd;
  int nightVisionWearTime;
  int nightVisionWearTimeFadeOutEnd;
  int nightVisionWearTimePowerUp;
  int nightVisionRemoveTime;
  int nightVisionRemoveTimePowerDown;
  int nightVisionRemoveTimeFadeInStart;
  int fuseTime;
  int aiFuseTime;
  float autoAimRange;
  float aimAssistRange;
  float aimAssistRangeAds;
  float aimPadding;
  float enemyCrosshairRange;
  float moveSpeedScale;
  float adsMoveSpeedScale;
  float sprintDurationScale;
  float fAdsZoomInFrac;
  float fAdsZoomOutFrac;
  Material *overlayMaterial;
  Material *overlayMaterialLowRes;
  Material *overlayMaterialEMP;
  Material *overlayMaterialEMPLowRes;
  weapOverlayReticle_t overlayReticle;
  WeapOverlayInteface_t overlayInterface;
  float overlayWidth;
  float overlayHeight;
  float overlayWidthSplitscreen;
  float overlayHeightSplitscreen;
  float fAdsBobFactor;
  float fAdsViewBobMult;
  float fHipSpreadStandMin;
  float fHipSpreadDuckedMin;
  float fHipSpreadProneMin;
  float hipSpreadStandMax;
  float hipSpreadDuckedMax;
  float hipSpreadProneMax;
  float fHipSpreadDecayRate;
  float fHipSpreadFireAdd;
  float fHipSpreadTurnAdd;
  float fHipSpreadMoveAdd;
  float fHipSpreadDuckedDecay;
  float fHipSpreadProneDecay;
  float fHipReticleSidePos;
  float fAdsIdleAmount;
  float fHipIdleAmount;
  float adsIdleSpeed;
  float hipIdleSpeed;
  float fIdleCrouchFactor;
  float fIdleProneFactor;
  float fGunMaxPitch;
  float fGunMaxYaw;
  float swayMaxAngle;
  float swayLerpSpeed;
  float swayPitchScale;
  float swayYawScale;
  float swayHorizScale;
  float swayVertScale;
  float swayShellShockScale;
  float adsSwayMaxAngle;
  float adsSwayLerpSpeed;
  float adsSwayPitchScale;
  float adsSwayYawScale;
  float adsSwayHorizScale;
  float adsSwayVertScale;
  float adsViewErrorMin;
  float adsViewErrorMax;
  PhysCollmap *physCollmap;
  float dualWieldViewModelOffset;
  weaponIconRatioType_t killIconRatio;
  int iReloadAmmoAdd;
  int iReloadStartAdd;
  int ammoDropStockMin;
  int ammoDropClipPercentMin;
  int ammoDropClipPercentMax;
  int iExplosionRadius;
  int iExplosionRadiusMin;
  int iExplosionInnerDamage;
  int iExplosionOuterDamage;
  float damageConeAngle;
  float bulletExplDmgMult;
  float bulletExplRadiusMult;
  int iProjectileSpeed;
  int iProjectileSpeedUp;
  int iProjectileSpeedForward;
  int iProjectileActivateDist;
  float projLifetime;
  float timeToAccelerate;
  float projectileCurvature;
  XModel *projectileModel;
  weapProjExposion_t projExplosion;
  FxEffectDef *projExplosionEffect;
  FxEffectDef *projDudEffect;
  snd_alias_list_t *projExplosionSound;
  snd_alias_list_t *projDudSound;
  WeapStickinessType stickiness;
  float lowAmmoWarningThreshold;
  float ricochetChance;
  float *parallelBounce;
  float *perpendicularBounce;
  FxEffectDef *projTrailEffect;
  FxEffectDef *projBeaconEffect;
  float vProjectileColor[3];
  guidedMissileType_t guidedMissileType;
  float maxSteeringAccel;
  int projIgnitionDelay;
  FxEffectDef *projIgnitionEffect;
  snd_alias_list_t *projIgnitionSound;
  float fAdsAimPitch;
  float fAdsCrosshairInFrac;
  float fAdsCrosshairOutFrac;
  int adsGunKickReducedKickBullets;
  float adsGunKickReducedKickPercent;
  float fAdsGunKickPitchMin;
  float fAdsGunKickPitchMax;
  float fAdsGunKickYawMin;
  float fAdsGunKickYawMax;
  float fAdsGunKickAccel;
  float fAdsGunKickSpeedMax;
  float fAdsGunKickSpeedDecay;
  float fAdsGunKickStaticDecay;
  float fAdsViewKickPitchMin;
  float fAdsViewKickPitchMax;
  float fAdsViewKickYawMin;
  float fAdsViewKickYawMax;
  float fAdsViewScatterMin;
  float fAdsViewScatterMax;
  float fAdsSpread;
  int hipGunKickReducedKickBullets;
  float hipGunKickReducedKickPercent;
  float fHipGunKickPitchMin;
  float fHipGunKickPitchMax;
  float fHipGunKickYawMin;
  float fHipGunKickYawMax;
  float fHipGunKickAccel;
  float fHipGunKickSpeedMax;
  float fHipGunKickSpeedDecay;
  float fHipGunKickStaticDecay;
  float fHipViewKickPitchMin;
  float fHipViewKickPitchMax;
  float fHipViewKickYawMin;
  float fHipViewKickYawMax;
  float fHipViewScatterMin;
  float fHipViewScatterMax;
  float fightDist;
  float maxDist;
  const char *accuracyGraphName[2];
  float (*originalAccuracyGraphKnots[2])[2];
  unsigned short originalAccuracyGraphKnotCount[2];
  int iPositionReloadTransTime;
  float leftArc;
  float rightArc;
  float topArc;
  float bottomArc;
  float accuracy;
  float aiSpread;
  float playerSpread;
  float minTurnSpeed[2];
  float maxTurnSpeed[2];
  float pitchConvergenceTime;
  float yawConvergenceTime;
  float suppressTime;
  float maxRange;
  float fAnimHorRotateInc;
  float fPlayerPositionDist;
  const char *szUseHintString;
  const char *dropHintString;
  int iUseHintStringIndex;
  int dropHintStringIndex;
  float horizViewJitter;
  float vertViewJitter;
  float scanSpeed;
  float scanAccel;
  int scanPauseTime;
  const char *szScript;
  float fOOPosAnimLength[2];
  int minDamage;
  int minPlayerDamage;
  float fMaxDamageRange;
  float fMinDamageRange;
  float destabilizationRateTime;
  float destabilizationCurvatureMax;
  int destabilizeDistance;
  float *locationDamageMultipliers;
  const char *fireRumble;
  const char *meleeImpactRumble;
  TracerDef *tracerType;
  float turretScopeZoomRate;
  float turretScopeZoomMin;
  float turretScopeZoomMax;
  float turretOverheatUpRate;
  float turretOverheatDownRate;
  float turretOverheatPenalty;
  snd_alias_list_t *turretOverheatSound;
  FxEffectDef *turretOverheatEffect;
  const char *turretBarrelSpinRumble;
  float turretBarrelSpinSpeed;
  float turretBarrelSpinUpTime;
  float turretBarrelSpinDownTime;
  snd_alias_list_t *turretBarrelSpinMaxSnd;
  snd_alias_list_t *turretBarrelSpinUpSnd[4];
  snd_alias_list_t *turretBarrelSpinDownSnd[4];
  snd_alias_list_t *missileConeSoundAlias;
  snd_alias_list_t *missileConeSoundAliasAtBase;
  float missileConeSoundRadiusAtTop;
  float missileConeSoundRadiusAtBase;
  float missileConeSoundHeight;
  float missileConeSoundOriginOffset;
  float missileConeSoundVolumescaleAtCore;
  float missileConeSoundVolumescaleAtEdge;
  float missileConeSoundVolumescaleCoreSize;
  float missileConeSoundPitchAtTop;
  float missileConeSoundPitchAtBottom;
  float missileConeSoundPitchTopSize;
  float missileConeSoundPitchBottomSize;
  float missileConeSoundCrossfadeTopSize;
  float missileConeSoundCrossfadeBottomSize;
  bool sharedAmmo;
  bool lockonSupported;
  bool requireLockonToFire;
  bool bigExplosion;
  bool noAdsWhenMagEmpty;
  bool avoidDropCleanup;
  bool inheritsPerks;
  bool crosshairColorChange;
  bool bRifleBullet;
  bool armorPiercing;
  bool bBoltAction;
  bool aimDownSight;
  bool bRechamberWhileAds;
  bool bBulletExplosiveDamage;
  bool bCookOffHold;
  bool bClipOnly;
  bool noAmmoPickup;
  bool adsFireOnly;
  bool cancelAutoHolsterWhenEmpty;
  bool disableSwitchToWhenEmpty;
  bool suppressAmmoReserveDisplay;
  bool laserSightDuringNightvision;
  bool markableViewmodel;
  bool noDualWield;
  bool flipKillIcon;
  bool bNoPartialReload;
  bool bSegmentedReload;
  bool blocksProne;
  bool silenced;
  bool isRollingGrenade;
  bool projExplosionEffectForceNormalUp;
  bool bProjImpactExplode;
  bool stickToPlayers;
  bool hasDetonator;
  bool disableFiring;
  bool timedDetonation;
  bool rotate;
  bool holdButtonToThrow;
  bool freezeMovementWhenFiring;
  bool thermalScope;
  bool altModeSameWeapon;
  bool turretBarrelSpinEnabled;
  bool missileConeSoundEnabled;
  bool missileConeSoundPitchshiftEnabled;
  bool missileConeSoundCrossfadeEnabled;
  bool offhandHoldIsCancelable;
};

// 1281
//
struct WeaponCompleteDef
{
  const char *szInternalName;
  WeaponDef *weapDef;
  const char *szDisplayName;
  unsigned short *hideTags;
  const char **szXAnims;
  float fAdsZoomFov;
  int iAdsTransInTime;
  int iAdsTransOutTime;
  int iClipSize;
  ImpactType impactType;
  int iFireTime;
  weaponIconRatioType_t dpadIconRatio;
  float penetrateMultiplier;
  float fAdsViewKickCenterSpeed;
  float fHipViewKickCenterSpeed;
  const char *szAltWeaponName;
  unsigned int altWeaponIndex;
  int iAltRaiseTime;
  Material *killIcon;
  Material *dpadIcon;
  int fireAnimLength;
  int iFirstRaiseTime;
  int ammoDropStockMax;
  float adsDofStart;
  float adsDofEnd;
  unsigned short accuracyGraphKnotCount[2];
  float (*accuracyGraphKnots[2])[2];
  bool motionTracker;
  bool enhanced;
  bool dpadIconShowsAmmo;
};

// 1282
//
struct SndDriverGlobals
{
  const char *name;
};

// 1283
//
struct RawFile
{
  const char *name;
  int compressedLen;
  int len;
  const char *buffer;
};

// 1284
//
struct StringTableCell
{
  const char *string;
  int hash;
};

// 1285
//
struct StringTable
{
  const char *name;
  int columnCount;
  int rowCount;
  StringTableCell *values;
};

// 1286
//
struct LbColumnDef
{
  const char *name;
  int id;
  int propertyId;
  bool hidden;
  const char *statName;
  LbColType type;
  int precision;
  LbAggType agg;
};

// 1287
//
struct LeaderboardDef
{
  const char *name;
  int id;
  int columnCount;
  int xpColId;
  int prestigeColId;
  LbColumnDef *columns;
};

// 1288
//
struct StructuredDataEnumEntry
{
  const char *string;
  unsigned short index;
};

// 1289
//
struct StructuredDataEnum
{
  int entryCount;
  int reservedEntryCount;
  StructuredDataEnumEntry *entries;
};

// 1290
//
union StructuredDataTypeUnion
{
  unsigned int stringDataLength;
  int enumIndex;
  int structIndex;
  int indexedArrayIndex;
  int enumedArrayIndex;
};

// 1291
//
struct StructuredDataType
{
  StructuredDataTypeCategory type;
  StructuredDataTypeUnion u;
};

// 1292
//
struct StructuredDataStructProperty
{
  const char *name;
  StructuredDataType type;
  unsigned int offset;
};

// 1293
//
struct StructuredDataStruct
{
  int propertyCount;
  StructuredDataStructProperty *properties;
  int size;
  unsigned int bitOffset;
};

// 1294
//
struct StructuredDataIndexedArray
{
  int arraySize;
  StructuredDataType elementType;
  unsigned int elementSize;
};

// 1295
//
struct StructuredDataEnumedArray
{
  int enumIndex;
  StructuredDataType elementType;
  unsigned int elementSize;
};

// 1296
//
struct StructuredDataDef
{
  int version;
  unsigned int formatChecksum;
  int enumCount;
  StructuredDataEnum *enums;
  int structCount;
  StructuredDataStruct *structs;
  int indexedArrayCount;
  StructuredDataIndexedArray *indexedArrays;
  int enumedArrayCount;
  StructuredDataEnumedArray *enumedArrays;
  StructuredDataType rootType;
  unsigned int size;
};

// 1297
//
struct StructuredDataDefSet
{
  const char *name;
  unsigned int defCount;
  StructuredDataDef *defs;
};

// 1298
//
struct VehiclePhysDef
{
  int physicsEnabled;
  const char *physPresetName;
  PhysPreset *physPreset;
  const char *accelGraphName;
  VehicleAxleType steeringAxle;
  VehicleAxleType powerAxle;
  VehicleAxleType brakingAxle;
  float topSpeed;
  float reverseSpeed;
  float maxVelocity;
  float maxPitch;
  float maxRoll;
  float suspensionTravelFront;
  float suspensionTravelRear;
  float suspensionStrengthFront;
  float suspensionDampingFront;
  float suspensionStrengthRear;
  float suspensionDampingRear;
  float frictionBraking;
  float frictionCoasting;
  float frictionTopSpeed;
  float frictionSide;
  float frictionSideRear;
  float velocityDependentSlip;
  float rollStability;
  float rollResistance;
  float pitchResistance;
  float yawResistance;
  float uprightStrengthPitch;
  float uprightStrengthRoll;
  float targetAirPitch;
  float airYawTorque;
  float airPitchTorque;
  float minimumMomentumForCollision;
  float collisionLaunchForceScale;
  float wreckedMassScale;
  float wreckedBodyFriction;
  float minimumJoltForNotify;
  float slipThresholdFront;
  float slipThresholdRear;
  float slipFricScaleFront;
  float slipFricScaleRear;
  float slipFricRateFront;
  float slipFricRateRear;
  float slipYawTorque;
};

// 1299
//
struct VehicleDef
{
  const char *name;
  VehicleType type;
  const char *useHintString;
  int health;
  int quadBarrel;
  float texScrollScale;
  float topSpeed;
  float accel;
  float rotRate;
  float rotAccel;
  float maxBodyPitch;
  float maxBodyRoll;
  float fakeBodyAccelPitch;
  float fakeBodyAccelRoll;
  float fakeBodyVelPitch;
  float fakeBodyVelRoll;
  float fakeBodySideVelPitch;
  float fakeBodyPitchStrength;
  float fakeBodyRollStrength;
  float fakeBodyPitchDampening;
  float fakeBodyRollDampening;
  float fakeBodyBoatRockingAmplitude;
  float fakeBodyBoatRockingPeriod;
  float fakeBodyBoatRockingRotationPeriod;
  float fakeBodyBoatRockingFadeoutSpeed;
  float boatBouncingMinForce;
  float boatBouncingMaxForce;
  float boatBouncingRate;
  float boatBouncingFadeinSpeed;
  float boatBouncingFadeoutSteeringAngle;
  float collisionDamage;
  float collisionSpeed;
  float killcamOffset[3];
  int playerProtected;
  int bulletDamage;
  int armorPiercingDamage;
  int grenadeDamage;
  int projectileDamage;
  int projectileSplashDamage;
  int heavyExplosiveDamage;
  VehiclePhysDef vehPhysDef;
  float boostDuration;
  float boostRechargeTime;
  float boostAcceleration;
  float suspensionTravel;
  float maxSteeringAngle;
  float steeringLerp;
  float minSteeringScale;
  float minSteeringSpeed;
  int camLookEnabled;
  float camLerp;
  float camPitchInfluence;
  float camRollInfluence;
  float camFovIncrease;
  float camFovOffset;
  float camFovSpeed;
  const char *turretWeaponName;
  WeaponCompleteDef *turretWeapon;
  float turretHorizSpanLeft;
  float turretHorizSpanRight;
  float turretVertSpanUp;
  float turretVertSpanDown;
  float turretRotRate;
  snd_alias_list_t *turretSpinSnd;
  snd_alias_list_t *turretStopSnd;
  int trophyEnabled;
  float trophyRadius;
  float trophyInactiveRadius;
  int trophyAmmoCount;
  float trophyReloadTime;
  unsigned short trophyTags[4];
  Material *compassFriendlyIcon;
  Material *compassEnemyIcon;
  int compassIconWidth;
  int compassIconHeight;
  snd_alias_list_t *idleLowSnd;
  snd_alias_list_t *idleHighSnd;
  snd_alias_list_t *engineLowSnd;
  snd_alias_list_t *engineHighSnd;
  float engineSndSpeed;
  snd_alias_list_t *engineStartUpSnd;
  int engineStartUpLength;
  snd_alias_list_t *engineShutdownSnd;
  snd_alias_list_t *engineIdleSnd;
  snd_alias_list_t *engineSustainSnd;
  snd_alias_list_t *engineRampUpSnd;
  int engineRampUpLength;
  snd_alias_list_t *engineRampDownSnd;
  int engineRampDownLength;
  snd_alias_list_t *suspensionSoftSnd;
  float suspensionSoftCompression;
  snd_alias_list_t *suspensionHardSnd;
  float suspensionHardCompression;
  snd_alias_list_t *collisionSnd;
  float collisionBlendSpeed;
  snd_alias_list_t *speedSnd;
  float speedSndBlendSpeed;
  const char *surfaceSndPrefix;
  snd_alias_list_t *surfaceSnds[31];
  float surfaceSndBlendSpeed;
  float slideVolume;
  float slideBlendSpeed;
  float inAirPitch;
};

// 1300
//
struct AddonMapEnts
{
  const char *name;
  char *entityString;
  int numEntityChars;
  MapTriggers trigger;
};

// 1301
//
union XAssetHeader
{
  PhysPreset *physPreset;
  PhysCollmap *physCollmap;
  XAnimParts *parts;
  XModelSurfs *modelSurfs;
  XModel *model;
  Material *material;
  MaterialPixelShader *pixelShader;
  MaterialVertexShader *vertexShader;
  MaterialVertexDeclaration *vertexDecl;
  MaterialTechniqueSet *techniqueSet;
  GfxImage *image;
  snd_alias_list_t *sound;
  SndCurve *sndCurve;
  LoadedSound *loadSnd;
  clipMap_t *clipMap;
  ComWorld *comWorld;
  GameWorldSp *gameWorldSp;
  GameWorldMp *gameWorldMp;
  MapEnts *mapEnts;
  FxWorld *fxWorld;
  GfxWorld *gfxWorld;
  GfxLightDef *lightDef;
  Font_s *font;
  MenuList *menuList;
  menuDef_t *menu;
  LocalizeEntry *localize;
  WeaponCompleteDef *weapon;
  SndDriverGlobals *sndDriverGlobals;
  FxEffectDef *fx;
  FxImpactTable *impactFx;
  RawFile *rawfile;
  StringTable *stringTable;
  LeaderboardDef *leaderboardDef;
  StructuredDataDefSet *structuredDataDefSet;
  TracerDef *tracerDef;
  VehicleDef *vehDef;
  AddonMapEnts *addonMapEnts;
  void *data;
};

// 2096
//
struct ScriptStringList
{
  int count;
  const char **strings;
};

// 2686
//
struct XAsset
{
  XAssetType type;
  XAssetHeader header;
};

// 2687
//
struct XAssetList
{
  ScriptStringList stringList;
  int assetCount;
  XAsset *assets;
};

// 2795
//
struct XAssetEntry
{
  XAsset asset;
  char zoneIndex;
  volatile char inuseMask;
  bool printedMissingAsset;
  unsigned short nextHash;
  unsigned short nextOverride;
};

// 2796
//
union XAssetEntryPoolEntry
{
  XAssetEntry entry;
  XAssetEntryPoolEntry *next;
};
