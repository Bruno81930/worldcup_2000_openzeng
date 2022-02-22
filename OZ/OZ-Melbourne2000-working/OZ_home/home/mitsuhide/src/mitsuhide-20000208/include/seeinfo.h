#define N_GOAL 			2 
#define N_FLAG 			(6 + 6 + 4 + 22 + 14 + 1) 
#define N_BALL                  1
#define N_PLAYER_PER_TEAM 	11 
#define N_PLAYER 		(N_PLAYER_PER_TEAM * 2) 
#define N_LINE 			4 

#define UNUM_ERR    0

typedef enum {
  S_RIGHT = -1,
  S_UNKNOWN = 0,
  S_LEFT = 1  
} Side;

#define DIST_ERR   -1
#define DIR_ERR  -360
#define CHNG_ERR 1024

typedef enum {
  GID_UNKNOWN,
  GID_LEFT,
  GID_RIGHT
} GoalID;

typedef struct { 
  GoalID id;
  double dist;
  double dist_error;
  double dir;
  double dir_error;
  double dist_chng;
  double dist_chng_error;
  double dir_chng;
  double dir_chng_error;
} SeenGoal;

typedef enum {
  FN_UNKNOWN,
  FN_c,
  FN_c_t,
  FN_c_b,
  FN_l_t,
  FN_l_b,
  FN_r_t,
  FN_r_b,
  FN_p_l_t,
  FN_p_l_c,
  FN_p_l_b,
  FN_p_r_t,
  FN_p_r_c,
  FN_p_r_b,
  FN_g_l_t,
  FN_g_l_b,
  FN_g_r_t,
  FN_g_r_b,
  FN_t_l_50,
  FN_t_l_40,
  FN_t_l_30,
  FN_t_l_20,
  FN_t_l_10,
  FN_t_0,
  FN_t_r_10,
  FN_t_r_20,
  FN_t_r_30,
  FN_t_r_40,
  FN_t_r_50,
  FN_b_l_50,
  FN_b_l_40,
  FN_b_l_30,
  FN_b_l_20,
  FN_b_l_10,
  FN_b_0,
  FN_b_r_10,
  FN_b_r_20,
  FN_b_r_30,
  FN_b_r_40,
  FN_b_r_50,
  FN_l_t_30,
  FN_l_t_20,
  FN_l_t_10,
  FN_l_0,
  FN_l_b_10,
  FN_l_b_20,
  FN_l_b_30,
  FN_r_t_30,
  FN_r_t_20,
  FN_r_t_10,
  FN_r_0,
  FN_r_b_10,
  FN_r_b_20,
  FN_r_b_30,
  FN_MAX
} FlagName;

typedef struct {
  FlagName name;
  FlagName begin;
  FlagName end;
} FlagID;

typedef struct {
  FlagID id;
  double dist;
  double dist_error;
  double dir;
  double dir_error;
  double dist_chng;
  double dist_chng_error;
  double dir_chng;
  double dir_chng_error;
} SeenFlag;

typedef struct {
  double dist;
  double dist_error;
  double dir;
  double dir_error;
  double dist_chng;
  double dist_chng_error;
  double dir_chng;
  double dir_chng_error;
} SeenBall;

typedef enum {
  TID_UNKNOWN,
  TID_TEAMMATE,
  TID_OPPONENT
} TeamID;

typedef struct {
  TeamID team;
  int unum;
} PlayerID;

typedef struct {
  PlayerID id;
  double dist;
  double dist_error;
  double dir;
  double dir_error;
  double dist_chng;
  double dist_chng_error;
  double dir_chng;
  double dir_chng_error;
  double body_dir;
  double body_dir_error;
  double face_dir;
  double face_dir_error;
} SeenPlayer;

typedef enum {
  LID_LEFT,
  LID_TOP,
  LID_RIGHT,
  LID_BOTTOM
} LineID;

typedef struct {
  LineID id;
  double dist;
  double dist_error;
  double dir;
  double dir_error;
} SeenLine;

#define PITCH_LENGTH           105.0
#define PITCH_WIDTH             68.0
#define PITCH_MARGIN             5.0
#define PENALTY_AREA_LENGTH	16.5
#define PENALTY_AREA_WIDTH	40.32
#define GOAL_WIDTH              14.02
#define HALF_PITCH_L		PITCH_LENGTH/2.0 
#define HALF_PITCH_W		PITCH_WIDTH/2.0 
#define PENALTY_CORNER_X	(HALF_PITCH_L - PENALTY_AREA_LENGTH)
#define PENALTY_CORNER_Y	(PENALTY_AREA_WIDTH/2.0)
#define HALF_PITCH_L_M		(HALF_PITCH_L + PITCH_MARGIN)
#define HALF_PITCH_W_M		(HALF_PITCH_W + PITCH_MARGIN)

#define FLAGINFOTAB {\
  {"UNKNOWN", FN_UNKNOWN, Vector(              0.0,               0.0)},\
  {"c",       FN_c,       Vector(              0.0,               0.0)},\
  {"c t",     FN_c_t,     Vector(              0.0,     -HALF_PITCH_W)},\
  {"c b",     FN_c_b,     Vector(              0.0,      HALF_PITCH_W)},\
  {"l t",     FN_l_t,     Vector(    -HALF_PITCH_L,     -HALF_PITCH_W)},\
  {"l b",     FN_l_b,     Vector(    -HALF_PITCH_L,      HALF_PITCH_W)},\
  {"r t",     FN_r_t,     Vector(     HALF_PITCH_L,     -HALF_PITCH_W)},\
  {"r b",     FN_r_b,     Vector(     HALF_PITCH_L,      HALF_PITCH_W)},\
  {"p l t",   FN_p_l_t,   Vector(-PENALTY_CORNER_X, -PENALTY_CORNER_Y)},\
  {"p l c",   FN_p_l_c,   Vector(-PENALTY_CORNER_X,               0.0)},\
  {"p l b",   FN_p_l_b,   Vector(-PENALTY_CORNER_X,  PENALTY_CORNER_Y)},\
  {"p r t",   FN_p_r_t,   Vector( PENALTY_CORNER_X, -PENALTY_CORNER_Y)},\
  {"p r c",   FN_p_r_c,   Vector( PENALTY_CORNER_X,               0.0)},\
  {"p r b",   FN_p_r_b,   Vector( PENALTY_CORNER_X,  PENALTY_CORNER_Y)},\
  {"g l t",   FN_g_l_t,   Vector(    -HALF_PITCH_L,   -GOAL_WIDTH/2.0)},\
  {"g l b",   FN_g_l_b,   Vector(    -HALF_PITCH_L,    GOAL_WIDTH/2.0)},\
  {"g r t",   FN_g_r_t,   Vector(     HALF_PITCH_L,   -GOAL_WIDTH/2.0)},\
  {"g r b",   FN_g_r_b,   Vector(     HALF_PITCH_L,    GOAL_WIDTH/2.0)},\
  {"t l 50",  FN_t_l_50,  Vector(              -50,   -HALF_PITCH_W_M)},\
  {"t l 40",  FN_t_l_40,  Vector(              -40,   -HALF_PITCH_W_M)},\
  {"t l 30",  FN_t_l_30,  Vector(              -30,   -HALF_PITCH_W_M)},\
  {"t l 20",  FN_t_l_20,  Vector(              -20,   -HALF_PITCH_W_M)},\
  {"t l 10",  FN_t_l_10,  Vector(              -10,   -HALF_PITCH_W_M)},\
  {"t 0",     FN_t_0,     Vector(                0,   -HALF_PITCH_W_M)},\
  {"t r 10",  FN_t_r_10,  Vector(               10,   -HALF_PITCH_W_M)},\
  {"t r 20",  FN_t_r_20,  Vector(               20,   -HALF_PITCH_W_M)},\
  {"t r 30",  FN_t_r_30,  Vector(               30,   -HALF_PITCH_W_M)},\
  {"t r 40",  FN_t_r_40,  Vector(               40,   -HALF_PITCH_W_M)},\
  {"t r 50",  FN_t_r_50,  Vector(               50,   -HALF_PITCH_W_M)},\
  {"b l 50",  FN_b_l_50,  Vector(              -50,    HALF_PITCH_W_M)},\
  {"b l 40",  FN_b_l_40,  Vector(              -40,    HALF_PITCH_W_M)},\
  {"b l 30",  FN_b_l_30,  Vector(              -30,    HALF_PITCH_W_M)},\
  {"b l 20",  FN_b_l_20,  Vector(              -20,    HALF_PITCH_W_M)},\
  {"b l 10",  FN_b_l_10,  Vector(              -10,    HALF_PITCH_W_M)},\
  {"b 0",     FN_b_0,     Vector(                0,    HALF_PITCH_W_M)},\
  {"b r 10",  FN_b_r_10,  Vector(               10,    HALF_PITCH_W_M)},\
  {"b r 20",  FN_b_r_20,  Vector(               20,    HALF_PITCH_W_M)},\
  {"b r 30",  FN_b_r_30,  Vector(               30,    HALF_PITCH_W_M)},\
  {"b r 40",  FN_b_r_40,  Vector(               40,    HALF_PITCH_W_M)},\
  {"b r 50",  FN_b_r_50,  Vector(               50,    HALF_PITCH_W_M)},\
  {"l t 30",  FN_l_t_30,  Vector(  -HALF_PITCH_L_M,               -30)},\
  {"l t 20",  FN_l_t_20,  Vector(  -HALF_PITCH_L_M,               -20)},\
  {"l t 10",  FN_l_t_10,  Vector(  -HALF_PITCH_L_M,               -10)},\
  {"l 0",     FN_l_0,     Vector(  -HALF_PITCH_L_M,                 0)},\
  {"l b 10",  FN_l_b_10,  Vector(  -HALF_PITCH_L_M,                10)},\
  {"l b 20",  FN_l_b_20,  Vector(  -HALF_PITCH_L_M,                20)},\
  {"l b 30",  FN_l_b_30,  Vector(  -HALF_PITCH_L_M,                30)},\
  {"r t 30",  FN_r_t_30,  Vector(   HALF_PITCH_L_M,               -30)},\
  {"r t 20",  FN_r_t_20,  Vector(   HALF_PITCH_L_M,               -20)},\
  {"r t 10",  FN_r_t_10,  Vector(   HALF_PITCH_L_M,               -10)},\
  {"r 0",     FN_r_0,     Vector(   HALF_PITCH_L_M,                 0)},\
  {"r b 10",  FN_r_b_10,  Vector(   HALF_PITCH_L_M,                10)},\
  {"r b 20",  FN_r_b_20,  Vector(   HALF_PITCH_L_M,                20)},\
  {"r b 30",  FN_r_b_30,  Vector(   HALF_PITCH_L_M,                30)},\
  {NULL}}

typedef struct {
  char* namestr;
  FlagName name;
  Vector pos;
} FlagInfo;

extern const FlagInfo Flags[];

#define STATICOBJDISTTAB {\
  {0.000000, 0.025019},\
  {0.100000, 0.100178},\
  {0.200000, 0.200321},\
  {0.300000, 0.301007},\
  {0.400000, 0.401636},\
  {0.500000, 0.501572},\
  {0.600000, 0.599413},\
  {0.700000, 0.699639},\
  {0.800000, 0.799954},\
  {0.900000, 0.897190},\
  {1.000000, 0.996257},\
  {1.100000, 1.095282},\
  {1.200000, 1.198429},\
  {1.300000, 1.304474},\
  {1.400000, 1.405808},\
  {1.500000, 1.499977},\
  {1.600000, 1.600974},\
  {1.700000, 1.699463},\
  {1.800000, 1.795798},\
  {1.900000, 1.897073},\
  {2.000000, 1.994338},\
  {2.100000, 2.096590},\
  {2.200000, 2.204085},\
  {2.300000, 2.305275},\
  {2.400000, 2.399355},\
  {2.500000, 2.497274},\
  {2.600000, 2.599191},\
  {2.700000, 2.705266},\
  {2.800000, 2.801381},\
  {2.900000, 2.901419},\
  {3.000000, 3.004504},\
  {3.100000, 3.096005},\
  {3.200000, 3.190292},\
  {3.300000, 3.287451},\
  {3.400000, 3.387569},\
  {3.500000, 3.490736},\
  {3.600000, 3.597044},\
  {3.700000, 3.706591},\
  {3.800000, 3.800186},\
  {3.900000, 3.896632},\
  {4.000000, 3.995026},\
  {4.100000, 4.096416},\
  {4.200000, 4.199855},\
  {4.300000, 4.306444},\
  {4.400000, 4.415186},\
  {4.500000, 4.504379},\
  {4.600000, 4.595374},\
  {4.700000, 4.688206},\
  {4.800000, 4.782914},\
  {4.900000, 4.879536},\
  {5.000000, 4.978109},\
  {5.100000, 5.078673},\
  {5.200000, 5.181269},\
  {5.300000, 5.285938},\
  {5.400000, 5.392721},\
  {5.500000, 5.501661},\
  {5.600000, 5.612802},\
  {5.700000, 5.697415},\
  {5.800000, 5.783737},\
  {5.900000, 5.900576},\
  {6.000000, 6.019776},\
  {6.100000, 6.110524},\
  {6.200000, 6.203106},\
  {6.300000, 6.296617},\
  {6.400000, 6.392018},\
  {6.500000, 6.488378},\
  {6.600000, 6.586684},\
  {6.700000, 6.685978},\
  {6.800000, 6.787278},\
  {6.900000, 6.889597},\
  {7.000000, 6.993982},\
  {7.100000, 7.099416},\
  {7.200000, 7.206980},\
  {7.300000, 7.315626},\
  {7.400000, 7.389149},\
  {7.500000, 7.501102},\
  {7.600000, 7.614182},\
  {7.700000, 7.690706},\
  {7.800000, 7.807228},\
  {7.900000, 7.924922},\
  {8.000000, 8.004569},\
  {8.100000, 8.085016},\
  {8.200000, 8.207513},\
  {8.300000, 8.331242},\
  {8.400000, 8.414972},\
  {8.500000, 8.499544},\
  {8.600000, 8.584966},\
  {8.700000, 8.671246},\
  {8.800000, 8.802625},\
  {8.900000, 8.935325},\
  {9.000000, 9.025126},\
  {9.100000, 9.115830},\
  {9.200000, 9.207446},\
  {9.300000, 9.299982},\
  {9.400000, 9.393448},\
  {9.500000, 9.487854},\
  {9.600000, 9.583209},\
  {9.700000, 9.679522},\
  {9.800000, 9.776803},\
  {9.900000, 9.875061},\
  {10.000000, 9.974307},\
  {10.100000, 10.074550},\
  {10.200000, 10.175801},\
  {10.300000, 10.278070},\
  {10.400000, 10.381366},\
  {10.500000, 10.485700},\
  {10.600000, 10.591083},\
  {10.700000, 10.697526},\
  {10.800000, 10.805038},\
  {10.900000, 10.913630},\
  {11.000000, 11.023314},\
  {11.100000, 11.134100},\
  {11.200000, 11.246000},\
  {11.400000, 11.359024},\
  {11.500000, 11.473184},\
  {11.600000, 11.588491},\
  {11.700000, 11.704958},\
  {11.800000, 11.822595},\
  {11.900000, 11.941414},\
  {12.100000, 12.061427},\
  {12.200000, 12.182646},\
  {12.300000, 12.305083},\
  {12.400000, 12.428752},\
  {12.600000, 12.553663},\
  {12.700000, 12.679829},\
  {12.800000, 12.807264},\
  {12.900000, 12.935979},\
  {13.100000, 13.065988},\
  {13.200000, 13.197303},\
  {13.300000, 13.329938},\
  {13.500000, 13.463906},\
  {13.600000, 13.599221},\
  {13.700000, 13.735895},\
  {13.900000, 13.873943},\
  {14.000000, 14.013379},\
  {14.200000, 14.154216},\
  {14.300000, 14.296468},\
  {14.400000, 14.440149},\
  {14.600000, 14.585275},\
  {14.700000, 14.731860},\
  {14.900000, 14.879917},\
  {15.000000, 15.029463},\
  {15.200000, 15.180512},\
  {15.300000, 15.333078},\
  {15.500000, 15.487178},\
  {15.600000, 15.642827},\
  {15.800000, 15.800040},\
  {16.000000, 15.958833},\
  {16.100000, 16.119222},\
  {16.300000, 16.281223},\
  {16.400000, 16.444852},\
  {16.600000, 16.610125},\
  {16.800000, 16.777060},\
  {16.900000, 16.945672},\
  {17.100000, 17.115979},\
  {17.300000, 17.287998},\
  {17.500000, 17.461745},\
  {17.600000, 17.637239},\
  {17.800000, 17.814496},\
  {18.000000, 17.993534},\
  {18.200000, 18.174372},\
  {18.400000, 18.357028},\
  {18.500000, 18.541519},\
  {18.700000, 18.727865},\
  {18.900000, 18.916083},\
  {19.100000, 19.106192},\
  {19.300000, 19.298213},\
  {19.500000, 19.492163},\
  {19.700000, 19.688063},\
  {19.900000, 19.885931},\
  {20.100000, 20.085788},\
  {20.300000, 20.287653},\
  {20.500000, 20.491547},\
  {20.700000, 20.697491},\
  {20.900000, 20.905504},\
  {21.100000, 21.115609},\
  {21.300000, 21.327824},\
  {21.500000, 21.542172},\
  {21.800000, 21.758674},\
  {22.000000, 21.977353},\
  {22.200000, 22.198229},\
  {22.400000, 22.421325},\
  {22.600000, 22.646663},\
  {22.900000, 22.874266},\
  {23.100000, 23.104156},\
  {23.300000, 23.336357},\
  {23.600000, 23.570891},\
  {23.800000, 23.807782},\
  {24.000000, 24.047054},\
  {24.300000, 24.288731},\
  {24.500000, 24.532837},\
  {24.800000, 24.779396},\
  {25.000000, 25.028433},\
  {25.300000, 25.279973},\
  {25.500000, 25.534041},\
  {25.800000, 25.790663},\
  {26.000000, 26.049863},\
  {26.300000, 26.311668},\
  {26.600000, 26.576105},\
  {26.800000, 26.843200},\
  {27.100000, 27.112978},\
  {27.400000, 27.385468},\
  {27.700000, 27.660696},\
  {27.900000, 27.938691},\
  {28.200000, 28.219480},\
  {28.500000, 28.503090},\
  {28.800000, 28.789551},\
  {29.100000, 29.078891},\
  {29.400000, 29.371138},\
  {29.700000, 29.666323},\
  {30.000000, 29.964475},\
  {30.300000, 30.265623},\
  {30.600000, 30.569797},\
  {30.900000, 30.877029},\
  {31.200000, 31.187348},\
  {31.500000, 31.500786},\
  {31.800000, 31.817374},\
  {32.100000, 32.137144},\
  {32.500000, 32.460128},\
  {32.800000, 32.786358},\
  {33.100000, 33.115866},\
  {33.400000, 33.448686},\
  {33.800000, 33.784851},\
  {34.100000, 34.124394},\
  {34.500000, 34.467350},\
  {34.800000, 34.813753},\
  {35.200000, 35.163637},\
  {35.500000, 35.517037},\
  {35.900000, 35.873989},\
  {36.200000, 36.234529},\
  {36.600000, 36.598691},\
  {37.000000, 36.966514},\
  {37.300000, 37.338034},\
  {37.700000, 37.713288},\
  {38.100000, 38.092312},\
  {38.500000, 38.475147},\
  {38.900000, 38.861829},\
  {39.300000, 39.252396},\
  {39.600000, 39.646889},\
  {40.000000, 40.045347},\
  {40.400000, 40.447810},\
  {40.900000, 40.854317},\
  {41.300000, 41.264910},\
  {41.700000, 41.679629},\
  {42.100000, 42.098516},\
  {42.500000, 42.521613},\
  {42.900000, 42.948962},\
  {43.400000, 43.380607},\
  {43.800000, 43.816589},\
  {44.300000, 44.256953},\
  {44.700000, 44.701743},\
  {45.200000, 45.151003},\
  {45.600000, 45.604778},\
  {46.100000, 46.063114},\
  {46.500000, 46.526056},\
  {47.000000, 46.993650},\
  {47.500000, 47.465944},\
  {47.900000, 47.942985},\
  {48.400000, 48.424820},\
  {48.900000, 48.911498},\
  {49.400000, 49.403066},\
  {49.900000, 49.899575},\
  {50.400000, 50.401075},\
  {50.900000, 50.907614},\
  {51.400000, 51.419244},\
  {51.900000, 51.936016},\
  {52.500000, 52.457981},\
  {53.000000, 52.985193},\
  {53.500000, 53.517703},\
  {54.100000, 54.055565},\
  {54.600000, 54.598832},\
  {55.100000, 55.147560},\
  {55.700000, 55.701802},\
  {56.300000, 56.261614},\
  {56.800000, 56.827053},\
  {57.400000, 57.398175},\
  {58.000000, 57.975036},\
  {58.600000, 58.557694},\
  {59.100000, 59.146209},\
  {59.700000, 59.740638},\
  {60.300000, 60.341042},\
  {60.900000, 60.947479},\
  {61.600000, 61.560012},\
  {62.200000, 62.178700},\
  {62.800000, 62.803606},\
  {63.400000, 63.434793},\
  {64.100000, 64.072323},\
  {64.700000, 64.716261},\
  {65.400000, 65.366670},\
  {66.000000, 66.023616},\
  {66.700000, 66.687164},\
  {67.400000, 67.357381},\
  {68.000000, 68.034334},\
  {68.700000, 68.718091},\
  {69.400000, 69.408719},\
  {70.100000, 70.106289},\
  {70.800000, 70.810869},\
  {71.500000, 71.522530},\
  {72.200000, 72.241343},\
  {73.000000, 72.967380},\
  {73.700000, 73.700715},\
  {74.400000, 74.441419},\
  {75.200000, 75.189568},\
  {75.900000, 75.945235},\
  {76.700000, 76.708498},\
  {77.500000, 77.479431},\
  {78.300000, 78.258113},\
  {79.000000, 79.044620},\
  {79.800000, 79.839031},\
  {80.600000, 80.641427},\
  {81.500000, 81.451886},\
  {82.300000, 82.270492},\
  {83.100000, 83.097324},\
  {83.900000, 83.932466},\
  {84.800000, 84.776001},\
  {85.600000, 85.628014},\
  {86.500000, 86.488590},\
  {87.400000, 87.357815},\
  {88.200000, 88.235775},\
  {89.100000, 89.122560},\
  {90.000000, 90.018257},\
  {90.900000, 90.922955},\
  {91.800000, 91.836746},\
  {92.800000, 92.759720},\
  {93.700000, 93.691971},\
  {94.600000, 94.633591},\
  {95.600000, 95.584675},\
  {96.500000, 96.545317},\
  {97.500000, 97.515613},\
  {98.500000, 98.495661},\
  {99.500000, 99.485559},\
  {100.500000, 100.485406},\
  {101.500000, 101.495301},\
  {102.500000, 102.515346},\
  {103.500000, 103.545642},\
  {104.600000, 104.586293},\
  {105.600000, 105.637403},\
  {106.700000, 106.699076},\
  {107.800000, 107.771420},\
  {108.900000, 108.854540},\
  {109.900000, 109.948547},\
  {111.100000, 111.053548},\
  {112.200000, 112.169655},\
  {113.300000, 113.296978},\
  {114.400000, 114.435632},\
  {115.600000, 115.585729},\
  {116.700000, 116.747385},\
  {117.900000, 117.920716},\
  {119.100000, 119.105839},\
  {120.300000, 120.302872},\
  {121.500000, 121.511936},\
  {122.700000, 122.733152},\
  {124.000000, 123.966640},\
  {125.200000, 125.212526},\
  {126.500000, 126.470933},\
  {127.700000, 127.741987},\
  {129.000000, 129.025815},\
  {130.300000, 130.322546},\
  {131.600000, 131.632309},\
  {133.000000, 132.955236},\
  {134.300000, 134.291458},\
  {135.600000, 135.641110},\
  {137.000000, 137.004326},\
  {138.400000, 138.381242},\
  {139.800000, 139.771997},\
  {141.200000, 141.176729},\
  {142.600000, 142.595578},\
  {144.000000, 144.028688},\
  {145.500000, 145.476200},\
  {146.900000, 146.938260},\
  {148.400000, 148.415014},\
  {149.900000, 149.906610},\
  {151.400000, 151.413197},\
  {152.900000, 152.934924},\
  {154.500000, 154.471946},\
  {156.000000, 156.024415},\
  {157.600000, 157.592486},\
  {159.200000, 159.176317}}

#define STATICOBJDISTERRORTAB {\
  {0.000000, 0.025019},\
  {0.100000, 0.050142},\
  {0.200000, 0.050003},\
  {0.300000, 0.050684},\
  {0.400000, 0.049945},\
  {0.500000, 0.049991},\
  {0.600000, 0.047852},\
  {0.700000, 0.052376},\
  {0.800000, 0.047940},\
  {0.900000, 0.049297},\
  {1.000000, 0.049771},\
  {1.100000, 0.049255},\
  {1.200000, 0.053893},\
  {1.300000, 0.052152},\
  {1.400000, 0.049183},\
  {1.500000, 0.044986},\
  {1.600000, 0.056011},\
  {1.700000, 0.042478},\
  {1.800000, 0.053858},\
  {1.900000, 0.047417},\
  {2.000000, 0.049849},\
  {2.100000, 0.052405},\
  {2.200000, 0.055091},\
  {2.300000, 0.046100},\
  {2.400000, 0.047981},\
  {2.500000, 0.049940},\
  {2.600000, 0.051978},\
  {2.700000, 0.054099},\
  {2.800000, 0.042018},\
  {2.900000, 0.058021},\
  {3.000000, 0.045065},\
  {3.100000, 0.046437},\
  {3.200000, 0.047851},\
  {3.300000, 0.049309},\
  {3.400000, 0.050810},\
  {3.500000, 0.052358},\
  {3.600000, 0.053952},\
  {3.700000, 0.055596},\
  {3.800000, 0.038001},\
  {3.900000, 0.058446},\
  {4.000000, 0.039950},\
  {4.100000, 0.061442},\
  {4.200000, 0.041998},\
  {4.300000, 0.064592},\
  {4.400000, 0.044151},\
  {4.500000, 0.045043},\
  {4.600000, 0.045953},\
  {4.700000, 0.046881},\
  {4.800000, 0.047828},\
  {4.900000, 0.048795},\
  {5.000000, 0.049780},\
  {5.100000, 0.050786},\
  {5.200000, 0.051812},\
  {5.300000, 0.052859},\
  {5.400000, 0.053926},\
  {5.500000, 0.055016},\
  {5.600000, 0.056127},\
  {5.700000, 0.028488},\
  {5.800000, 0.057836},\
  {5.900000, 0.059004},\
  {6.000000, 0.060197},\
  {6.100000, 0.030553},\
  {6.200000, 0.062030},\
  {6.300000, 0.031483},\
  {6.400000, 0.063919},\
  {6.500000, 0.032443},\
  {6.600000, 0.065865},\
  {6.700000, 0.033430},\
  {6.800000, 0.067871},\
  {6.900000, 0.034449},\
  {7.000000, 0.069938},\
  {7.100000, 0.035497},\
  {7.200000, 0.072068},\
  {7.300000, 0.036579},\
  {7.400000, 0.036946},\
  {7.500000, 0.075009},\
  {7.600000, 0.038072},\
  {7.700000, 0.038454},\
  {7.800000, 0.078070},\
  {7.900000, 0.039625},\
  {8.000000, 0.040023},\
  {8.100000, 0.040425},\
  {8.200000, 0.082073},\
  {8.300000, 0.041656},\
  {8.400000, 0.042075},\
  {8.500000, 0.042498},\
  {8.600000, 0.042925},\
  {8.700000, 0.043356},\
  {8.800000, 0.088024},\
  {8.900000, 0.044677},\
  {9.000000, 0.045125},\
  {9.100000, 0.045579},\
  {9.200000, 0.046037},\
  {9.300000, 0.046500},\
  {9.400000, 0.046967},\
  {9.500000, 0.047439},\
  {9.600000, 0.047916},\
  {9.700000, 0.048398},\
  {9.800000, 0.048884},\
  {9.900000, 0.049375},\
  {10.000000, 0.049871},\
  {10.100000, 0.050372},\
  {10.200000, 0.050879},\
  {10.300000, 0.051390},\
  {10.400000, 0.051907},\
  {10.500000, 0.052428},\
  {10.600000, 0.052955},\
  {10.700000, 0.053488},\
  {10.800000, 0.054025},\
  {10.900000, 0.054568},\
  {11.000000, 0.055116},\
  {11.100000, 0.055670},\
  {11.200000, 0.056230},\
  {11.400000, 0.056795},\
  {11.500000, 0.057366},\
  {11.600000, 0.057942},\
  {11.700000, 0.058525},\
  {11.800000, 0.059113},\
  {11.900000, 0.059707},\
  {12.100000, 0.060307},\
  {12.200000, 0.060913},\
  {12.300000, 0.061525},\
  {12.400000, 0.062144},\
  {12.600000, 0.062768},\
  {12.700000, 0.063399},\
  {12.800000, 0.064036},\
  {12.900000, 0.064680},\
  {13.100000, 0.065330},\
  {13.200000, 0.065986},\
  {13.300000, 0.066649},\
  {13.500000, 0.067319},\
  {13.600000, 0.067996},\
  {13.700000, 0.068679},\
  {13.900000, 0.069369},\
  {14.000000, 0.070067},\
  {14.200000, 0.070771},\
  {14.300000, 0.071482},\
  {14.400000, 0.072200},\
  {14.600000, 0.072926},\
  {14.700000, 0.073659},\
  {14.900000, 0.074399},\
  {15.000000, 0.075147},\
  {15.200000, 0.075902},\
  {15.300000, 0.076665},\
  {15.500000, 0.077435},\
  {15.600000, 0.078214},\
  {15.800000, 0.079000},\
  {16.000000, 0.079794},\
  {16.100000, 0.080596},\
  {16.300000, 0.081406},\
  {16.400000, 0.082224},\
  {16.600000, 0.083051},\
  {16.800000, 0.083885},\
  {16.900000, 0.084729},\
  {17.100000, 0.085580},\
  {17.300000, 0.086440},\
  {17.500000, 0.087309},\
  {17.600000, 0.088186},\
  {17.800000, 0.089072},\
  {18.000000, 0.089968},\
  {18.200000, 0.090872},\
  {18.400000, 0.091785},\
  {18.500000, 0.092708},\
  {18.700000, 0.093639},\
  {18.900000, 0.094580},\
  {19.100000, 0.095531},\
  {19.300000, 0.096491},\
  {19.500000, 0.097461},\
  {19.700000, 0.098440},\
  {19.900000, 0.099429},\
  {20.100000, 0.100429},\
  {20.300000, 0.101438},\
  {20.500000, 0.102458},\
  {20.700000, 0.103487},\
  {20.900000, 0.104528},\
  {21.100000, 0.105578},\
  {21.300000, 0.106639},\
  {21.500000, 0.107711},\
  {21.800000, 0.108793},\
  {22.000000, 0.109887},\
  {22.200000, 0.110991},\
  {22.400000, 0.112107},\
  {22.600000, 0.113233},\
  {22.900000, 0.114371},\
  {23.100000, 0.115521},\
  {23.300000, 0.116682},\
  {23.600000, 0.117854},\
  {23.800000, 0.119038},\
  {24.000000, 0.120235},\
  {24.300000, 0.121443},\
  {24.500000, 0.122664},\
  {24.800000, 0.123896},\
  {25.000000, 0.125142},\
  {25.300000, 0.126399},\
  {25.500000, 0.127670},\
  {25.800000, 0.128953},\
  {26.000000, 0.130249},\
  {26.300000, 0.131558},\
  {26.600000, 0.132880},\
  {26.800000, 0.134216},\
  {27.100000, 0.135564},\
  {27.400000, 0.136927},\
  {27.700000, 0.138303},\
  {27.900000, 0.139693},\
  {28.200000, 0.141097},\
  {28.500000, 0.142515},\
  {28.800000, 0.143947},\
  {29.100000, 0.145394},\
  {29.400000, 0.146855},\
  {29.700000, 0.148331},\
  {30.000000, 0.149822},\
  {30.300000, 0.151328},\
  {30.600000, 0.152848},\
  {30.900000, 0.154385},\
  {31.200000, 0.155936},\
  {31.500000, 0.157503},\
  {31.800000, 0.159086},\
  {32.100000, 0.160685},\
  {32.500000, 0.162300},\
  {32.800000, 0.163930},\
  {33.100000, 0.165578},\
  {33.400000, 0.167242},\
  {33.800000, 0.168923},\
  {34.100000, 0.170621},\
  {34.500000, 0.172336},\
  {34.800000, 0.174067},\
  {35.200000, 0.175817},\
  {35.500000, 0.177584},\
  {35.900000, 0.179369},\
  {36.200000, 0.181171},\
  {36.600000, 0.182992},\
  {37.000000, 0.184831},\
  {37.300000, 0.186689},\
  {37.700000, 0.188565},\
  {38.100000, 0.190460},\
  {38.500000, 0.192375},\
  {38.900000, 0.194308},\
  {39.300000, 0.196260},\
  {39.600000, 0.198233},\
  {40.000000, 0.200225},\
  {40.400000, 0.202238},\
  {40.900000, 0.204270},\
  {41.300000, 0.206323},\
  {41.700000, 0.208397},\
  {42.100000, 0.210491},\
  {42.500000, 0.212606},\
  {42.900000, 0.214743},\
  {43.400000, 0.216902},\
  {43.800000, 0.219081},\
  {44.300000, 0.221283},\
  {44.700000, 0.223507},\
  {45.200000, 0.225753},\
  {45.600000, 0.228022},\
  {46.100000, 0.230314},\
  {46.500000, 0.232629},\
  {47.000000, 0.234966},\
  {47.500000, 0.237328},\
  {47.900000, 0.239713},\
  {48.400000, 0.242122},\
  {48.900000, 0.244556},\
  {49.400000, 0.247013},\
  {49.900000, 0.249496},\
  {50.400000, 0.252004},\
  {50.900000, 0.254536},\
  {51.400000, 0.257095},\
  {51.900000, 0.259678},\
  {52.500000, 0.262288},\
  {53.000000, 0.264924},\
  {53.500000, 0.267587},\
  {54.100000, 0.270276},\
  {54.600000, 0.272992},\
  {55.100000, 0.275736},\
  {55.700000, 0.278507},\
  {56.300000, 0.281306},\
  {56.800000, 0.284133},\
  {57.400000, 0.286989},\
  {58.000000, 0.289873},\
  {58.600000, 0.292786},\
  {59.100000, 0.295729},\
  {59.700000, 0.298701},\
  {60.300000, 0.301703},\
  {60.900000, 0.304735},\
  {61.600000, 0.307798},\
  {62.200000, 0.310891},\
  {62.800000, 0.314015},\
  {63.400000, 0.317172},\
  {64.100000, 0.320359},\
  {64.700000, 0.323579},\
  {65.400000, 0.326831},\
  {66.000000, 0.330116},\
  {66.700000, 0.333433},\
  {67.400000, 0.336784},\
  {68.000000, 0.340169},\
  {68.700000, 0.343588},\
  {69.400000, 0.347041},\
  {70.100000, 0.350529},\
  {70.800000, 0.354052},\
  {71.500000, 0.357610},\
  {72.200000, 0.361204},\
  {73.000000, 0.364834},\
  {73.700000, 0.368501},\
  {74.400000, 0.372204},\
  {75.200000, 0.375945},\
  {75.900000, 0.379723},\
  {76.700000, 0.383540},\
  {77.500000, 0.387394},\
  {78.300000, 0.391288},\
  {79.000000, 0.395220},\
  {79.800000, 0.399192},\
  {80.600000, 0.403204},\
  {81.500000, 0.407256},\
  {82.300000, 0.411350},\
  {83.100000, 0.415483},\
  {83.900000, 0.419659},\
  {84.800000, 0.423876},\
  {85.600000, 0.428137},\
  {86.500000, 0.432440},\
  {87.400000, 0.436786},\
  {88.200000, 0.441175},\
  {89.100000, 0.445610},\
  {90.000000, 0.450088},\
  {90.900000, 0.454611},\
  {91.800000, 0.459180},\
  {92.800000, 0.463795},\
  {93.700000, 0.468456},\
  {94.600000, 0.473164},\
  {95.600000, 0.477920},\
  {96.500000, 0.482723},\
  {97.500000, 0.487574},\
  {98.500000, 0.492474},\
  {99.500000, 0.497424},\
  {100.500000, 0.502423},\
  {101.500000, 0.507473},\
  {102.500000, 0.512573},\
  {103.500000, 0.517724},\
  {104.600000, 0.522928},\
  {105.600000, 0.528183},\
  {106.700000, 0.533491},\
  {107.800000, 0.538853},\
  {108.900000, 0.544268},\
  {109.900000, 0.549739},\
  {111.100000, 0.555263},\
  {112.200000, 0.560844},\
  {113.300000, 0.566480},\
  {114.400000, 0.572174},\
  {115.600000, 0.577924},\
  {116.700000, 0.583732},\
  {117.900000, 0.589599},\
  {119.100000, 0.595525},\
  {120.300000, 0.601509},\
  {121.500000, 0.607555},\
  {122.700000, 0.613661},\
  {124.000000, 0.619828},\
  {125.200000, 0.626058},\
  {126.500000, 0.632350},\
  {127.700000, 0.638705},\
  {129.000000, 0.645124},\
  {130.300000, 0.651608},\
  {131.600000, 0.658156},\
  {133.000000, 0.664771},\
  {134.300000, 0.671452},\
  {135.600000, 0.678200},\
  {137.000000, 0.685016},\
  {138.400000, 0.691901},\
  {139.800000, 0.698855},\
  {141.200000, 0.705878},\
  {142.600000, 0.712972},\
  {144.000000, 0.720138},\
  {145.500000, 0.727375},\
  {146.900000, 0.734685},\
  {148.400000, 0.742069},\
  {149.900000, 0.749527},\
  {151.400000, 0.757060},\
  {152.900000, 0.764668},\
  {154.500000, 0.772354},\
  {156.000000, 0.780116},\
  {157.600000, 0.787956},\
  {159.200000, 0.795875}}

#define DYNAMICOBJDISTTAB {\
  {0.000000, 0.026170},\
  {0.100000, 0.104789},\
  {0.200000, 0.208239},\
  {0.300000, 0.304589},\
  {0.400000, 0.411152},\
  {0.500000, 0.524658},\
  {0.600000, 0.607289},\
  {0.700000, 0.708214},\
  {0.800000, 0.819754},\
  {0.900000, 0.905969},\
  {1.000000, 1.001251},\
  {1.100000, 1.106553},\
  {1.200000, 1.222930},\
  {1.300000, 1.351546},\
  {1.500000, 1.493690},\
  {1.600000, 1.650783},\
  {1.800000, 1.824397},\
  {2.000000, 2.016270},\
  {2.200000, 2.228323},\
  {2.500000, 2.462678},\
  {2.700000, 2.721681},\
  {3.000000, 3.007922},\
  {3.300000, 3.324268},\
  {3.700000, 3.673884},\
  {4.100000, 4.060270},\
  {4.500000, 4.487293},\
  {5.000000, 4.959225},\
  {5.500000, 5.480791},\
  {6.000000, 6.057211},\
  {6.700000, 6.694254},\
  {7.400000, 7.398295},\
  {8.200000, 8.176380},\
  {9.000000, 9.036297},\
  {10.000000, 9.986652},\
  {11.000000, 11.036958},\
  {12.200000, 12.197725},\
  {13.500000, 13.480571},\
  {14.900000, 14.898335},\
  {16.400000, 16.465206},\
  {18.200000, 18.196867},\
  {20.100000, 20.110649},\
  {22.200000, 22.225705},\
  {24.500000, 24.563202},\
  {27.100000, 27.146537},\
  {30.000000, 30.001563},\
  {33.100000, 33.156855},\
  {36.600000, 36.643992},\
  {40.400000, 40.497874},\
  {44.700000, 44.757073},\
  {49.400000, 49.464215},\
  {54.600000, 54.666412},\
  {60.300000, 60.415729},\
  {66.700000, 66.769706},\
  {73.700000, 73.791938},\
  {81.500000, 81.552704},\
  {90.000000, 90.129676},\
  {99.500000, 99.608697},\
  {109.900000, 110.084635},\
  {121.500000, 121.662337},\
  {134.300000, 134.457677},\
  {148.400000, 148.598714}}

#define DYNAMICOBJDISTERRORTAB {\
  {0.000000, 0.026170},\
  {0.100000, 0.052450},\
  {0.200000, 0.051002},\
  {0.300000, 0.045349},\
  {0.400000, 0.061215},\
  {0.500000, 0.052292},\
  {0.600000, 0.030340},\
  {0.700000, 0.070587},\
  {0.800000, 0.040954},\
  {0.900000, 0.045262},\
  {1.000000, 0.050021},\
  {1.100000, 0.055282},\
  {1.200000, 0.061096},\
  {1.300000, 0.067521},\
  {1.500000, 0.074623},\
  {1.600000, 0.082471},\
  {1.800000, 0.091144},\
  {2.000000, 0.100731},\
  {2.200000, 0.111324},\
  {2.500000, 0.123032},\
  {2.700000, 0.135972},\
  {3.000000, 0.150271},\
  {3.300000, 0.166076},\
  {3.700000, 0.183542},\
  {4.100000, 0.202845},\
  {4.500000, 0.224179},\
  {5.000000, 0.247755},\
  {5.500000, 0.273812},\
  {6.000000, 0.302609},\
  {6.700000, 0.334435},\
  {7.400000, 0.369608},\
  {8.200000, 0.408479},\
  {9.000000, 0.451439},\
  {10.000000, 0.498917},\
  {11.000000, 0.551389},\
  {12.200000, 0.609379},\
  {13.500000, 0.673468},\
  {14.900000, 0.744297},\
  {16.400000, 0.822576},\
  {18.200000, 0.909087},\
  {20.100000, 1.004696},\
  {22.200000, 1.110361},\
  {24.500000, 1.227138},\
  {27.100000, 1.356198},\
  {30.000000, 1.498830},\
  {33.100000, 1.656463},\
  {36.600000, 1.830675},\
  {40.400000, 2.023208},\
  {44.700000, 2.235991},\
  {49.400000, 2.471152},\
  {54.600000, 2.731046},\
  {60.300000, 3.018272},\
  {66.700000, 3.335706},\
  {73.700000, 3.686526},\
  {81.500000, 4.074241},\
  {90.000000, 4.502732},\
  {99.500000, 4.976289},\
  {109.900000, 5.499650},\
  {121.500000, 6.078053},\
  {134.300000, 6.717287},\
  {148.400000, 7.423750}}

typedef struct {
  double see_dist;
  double near_dist;
} StaticObjDist;

extern const StaticObjDist static_obj_dist_table[];
extern const StaticObjDist static_obj_dist_error_table[];
extern const StaticObjDist dynamic_obj_dist_table[];
extern const StaticObjDist dynamic_obj_dist_error_table[];
double calc_static_dist(double see_dist);
double calc_static_dist2(double dist, int left, int right);
double calc_static_dist_error(double see_dist);
double calc_static_dist_error2(double dist, int left, int right);
double calc_dynamic_dist(double see_dist);
double calc_dynamic_dist2(double see_dist, int left, int right);
double calc_dynamic_dist_error(double see_dist);
double calc_dynamic_dist_error2(double see_dist, int left, int right);
double calc_dir(double see_dir);
double calc_dir_error(double see_dir);
double calc_dist_chng(double see_dist, double see_dist_chng, double dist);
double calc_dist_chng_error(double see_dist, double see_dist_chng, 
			    double dist, double dist_error);
double calc_dir_chng(double see_dist, double see_dir_chng);
double calc_dir_chng_error(void);
double max_dist_by_sin(double dist, double dist_error, 
                                  double dir, double dir_error);
double min_dist_by_sin(double dist, double dist_error, 
                                  double dir, double dir_error);
double max_dist_by_cos(double dist, double dist_error, 
                                  double dir, double dir_error);
double min_dist_by_cos(double dist, double dist_error, 
                                  double dir, double dir_error);
double ave_dist_by_sin(double dist, double dist_error, 
                                  double dir, double dir_error);
double error_dist_by_sin(double dist, double dist_error, 
                                    double dir, double dir_error);
double ave_dist_by_cos(double dist, double dist_error, 
				  double dir, double dir_error);
double error_dist_by_cos(double dist, double dist_error, 
                      double dir, double dir_error);
double multiplication_error(double a, double ae, double b, double be);
double division_error(double a, double ae, double b, double be);

#define max(x,y) ( ((x) > (y)) ? (x) : (y) )
#define min(x,y) ( ((x) > (y)) ? (y) : (x) )
#define NormalizeDashPower(p)	(max(min((p),MAXPOWER),MINPOWER))
#define ReduceDashPower(p)		(p * DASH_POWER_RATE)
#define NormalizeKickPower(p)	(max(min((p),MAXPOWER),MINPOWER)) 
#define NormalizeMoment(p) max(min((p),MAXMOMENT),MINMOMENT)
#define NormalizeNeckMoment(p) (max(min((p),MAXNECKMOMENT),MINNECKMOMENT))
#define NormalizeNeckAngle(p) (max(min((p),MAXNECKANG),MINNECKANG))
#define NormalizeAngleError(p) (min((p),ANGLE_ERROR_MAX))

#define POS_ERR   1024
#define VEL_ERR   1024
#define ANGLE_ERR -360
#define ANGLE_ERROR_MAX 180
