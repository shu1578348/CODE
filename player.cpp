//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "collision.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "enemy.h"
#include "house.h"
#include "fence.h"
#include "bullet.h"
#include "barrel.h"
#include "stall.h"
#include "score.h"
#include "particle.h"
#include "game.h"
#include "bomb.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER_BODY			"data/MODEL/player/body.obj" // 読み込むモデル名
#define	MODEL_PLAYER_HEAD			"data/MODEL/player/head.obj"
#define	MODEL_PLAYER_HIP			"data/MODEL/player/hip.obj"

#define	MODEL_PLAYER_SHOULDER_L		"data/MODEL/player/Shoulder_L.obj"
#define	MODEL_PLAYER_UPPER_ARM_L	"data/MODEL/player/UpperArm_L.obj"
#define	MODEL_PLAYER_ELBOW_L		"data/MODEL/player/Elbow_L.obj"
#define	MODEL_PLAYER_FORE_ARM_L		"data/MODEL/player/ForeArm_L.obj"

#define	MODEL_PLAYER_SHOULDER_R		"data/MODEL/player/Shoulder_R.obj"
#define	MODEL_PLAYER_UPPER_ARM_R	"data/MODEL/player/UpperArm_R.obj"
#define	MODEL_PLAYER_ELBOW_R		"data/MODEL/player/Elbow_R.obj"
#define	MODEL_PLAYER_FORE_ARM_R		"data/MODEL/player/ForeArm_R.obj"

#define	MODEL_PLAYER_UPPER_LEG_L	"data/MODEL/player/UpperLeg_L.obj"
#define	MODEL_PLAYER_KNEE_L			"data/MODEL/player/Knee_L.obj"
#define	MODEL_PLAYER_LOWER_LEG_L	"data/MODEL/player/LowerLeg_L.obj"
#define	MODEL_PLAYER_FOOT_L			"data/MODEL/player/Foot_L.obj"

#define	MODEL_PLAYER_UPPER_LEG_R	"data/MODEL/player/UpperLeg_R.obj"
#define	MODEL_PLAYER_KNEE_R			"data/MODEL/player/Knee_R.obj"
#define	MODEL_PLAYER_LOWER_LEG_R	"data/MODEL/player/LowerLeg_R.obj"
#define	MODEL_PLAYER_FOOT_R			"data/MODEL/player/Foot_R.obj"

#define	MODEL_PLAYER_SWORD			"data/MODEL/player/Sword.obj"
#define	MODEL_PLAYER_TORCH			"data/MODEL/player/Torch.obj"

#define	VALUE_MOVE			(1.5f)			// 移動量
#define VALUE_DASH_MOVE		(2.0f)			// ダッシュ時の移動量
#define	VALUE_ROTATE		(XM_PI * 0.1f)	// 回転量

#define PLAYER_SHADOW_SIZE	(0.4f)			// 影の大きさ
#define PLAYER_OFFSET_Y		(17.0f)			// プレイヤーの足元をあわせる

#define PLAYER_PARTS_MAX	(19)			// プレイヤーのパーツの数

#define	PLAYER_HP	(100)					// ヒットポイント
#define PLAYER_SP   (100.0f)					// 攻撃用のポイント	

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Player;						// プレイヤー

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// プレイヤーのパーツ用

DX11_MODEL			mSword;	// 剣
DX11_MODEL			mTorch;	// 松明		

static BOOL g_Load; // ツール初期化用

int padTool = 0; // パッドのツール変更用

// プレイヤー旋回用
float targetDir = 0.0f;	// 目標の向き
float angleDifference = 0.0f; // 現在の方向との角度差


//*****************************************************************************
// ANIM IDLE // POSE 0  // FRAME 2
//*****************************************************************************

// 頭
static INTERPOLATION_DATA move_tbl_idle_head[] = {// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f,  0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, -0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 腰
static INTERPOLATION_DATA move_tbl_idle_hip[] = {// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 左腕
static INTERPOLATION_DATA move_tbl_idle_lshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(12.0f, 20.0f, 0.0f), XMFLOAT3(0.0f, -0.5f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(12.0f, 18.0f, 0.0f), XMFLOAT3(0.0f, -0.5f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_larm01[] = {// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_lelbow[] = {// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_larm02[] = {// pos, rot, scl, frame
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右腕
static INTERPOLATION_DATA move_tbl_idle_rshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-12.0f, 20.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-12.0f, 18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_rarm01[] = {// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_relbow[] = {// pos, rot, scl, frame
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_rarm02[] = {// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 左足
static INTERPOLATION_DATA move_tbl_idle_lleg01[] = {// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_lknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_lleg02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_lfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右足
static INTERPOLATION_DATA move_tbl_idle_rleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_rknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_rleg02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_idle_rfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ツール
static INTERPOLATION_DATA move_tbl_idle_tool[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


//*****************************************************************************
// ANIM WALK // POSE 1 // FRAME 2
//*****************************************************************************

// 頭
static INTERPOLATION_DATA move_tbl_walk_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


// 腰
static INTERPOLATION_DATA move_tbl_walk_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


// 左腕
static INTERPOLATION_DATA move_tbl_walk_lshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(12.0f, 18.0f, 0.0f), XMFLOAT3(0.3f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(12.0f, 18.0f, 0.0f), XMFLOAT3(-0.3f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_larm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_lelbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_larm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


// 右腕
static INTERPOLATION_DATA move_tbl_walk_rshoulder[] = {	// pos, rot, scl, frame

	{ XMFLOAT3(-12.0f, 18.0f, 0.0f), XMFLOAT3(-0.3f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-12.0f, 18.0f, 0.0f), XMFLOAT3(0.3f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_rarm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_relbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_rarm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


// 左足
static INTERPOLATION_DATA move_tbl_walk_lleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(-0.7f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_lknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.6f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_lleg02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_lfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.2f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(-0.5f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


// 右足
static INTERPOLATION_DATA move_tbl_walk_rleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(-0.7f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_rknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.6f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_rleg02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_walk_rfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ツール
static INTERPOLATION_DATA move_tbl_walk_tool[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


//*****************************************************************************
// ANIM STANCE // POSE 2  // FRAME 2
//*****************************************************************************

// 頭
static INTERPOLATION_DATA move_tbl_stance_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, -0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f,  0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 腰
static INTERPOLATION_DATA move_tbl_stance_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 左腕
static INTERPOLATION_DATA move_tbl_stance_lshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(12.0f, 20.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(12.0f, 18.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_larm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_lelbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_larm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右腕
static INTERPOLATION_DATA move_tbl_stance_rshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-12.0f, 20.0f, 2.0f), XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-12.0f, 20.0f, 2.0f), XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
};

static INTERPOLATION_DATA move_tbl_stance_rarm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_relbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_rarm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 左足
static INTERPOLATION_DATA move_tbl_stance_lleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_lknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_lleg02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_lfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右足
static INTERPOLATION_DATA move_tbl_stance_rleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_rknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_rleg02[] = { // pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_stance_rfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ツール
static INTERPOLATION_DATA move_tbl_stance_tool[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

//*****************************************************************************
// ANIM THROW // POSE 3  // FRAME 2
//*****************************************************************************

static INTERPOLATION_DATA move_tbl_throw_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, -0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f,  0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 腰
static INTERPOLATION_DATA move_tbl_throw_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 左腕
static INTERPOLATION_DATA move_tbl_throw_lshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(12.0f, 20.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(12.0f, 18.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_larm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_lelbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_larm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右腕
static INTERPOLATION_DATA move_tbl_throw_rshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-12.0f, 20.0f, 2.0f), XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-12.0f, 20.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA move_tbl_throw_rarm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_relbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_rarm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

// 左足
static INTERPOLATION_DATA move_tbl_throw_lleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_lknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_lleg02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_lfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右足
static INTERPOLATION_DATA move_tbl_throw_rleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_rknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_rleg02[] = { // pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_throw_rfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ツール
static INTERPOLATION_DATA move_tbl_throw_tool[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


//*****************************************************************************
// ANIM ATTAK // POSE 4  // FRAME 2
//*****************************************************************************

static INTERPOLATION_DATA move_tbl_attak_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, -0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f,  0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 腰
static INTERPOLATION_DATA move_tbl_attak_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 左腕
static INTERPOLATION_DATA move_tbl_attak_lshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(12.0f, 20.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(12.0f, 18.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_larm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_lelbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_larm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右腕
static INTERPOLATION_DATA move_tbl_attak_rshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-12.0f, 20.0f, 2.0f), XMFLOAT3(-3.14f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
	{ XMFLOAT3(-12.0f, 20.0f, 2.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
};

static INTERPOLATION_DATA move_tbl_attak_rarm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
};

static INTERPOLATION_DATA move_tbl_attak_relbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
};

static INTERPOLATION_DATA move_tbl_attak_rarm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
};

// 左足
static INTERPOLATION_DATA move_tbl_attak_lleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_lknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_lleg02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_lfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右足
static INTERPOLATION_DATA move_tbl_attak_rleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_rknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_rleg02[] = { // pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_attak_rfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ツール
static INTERPOLATION_DATA move_tbl_attak_tool[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


//*****************************************************************************
// ANIM RUN // POSE 5  // FRAME 2
//*****************************************************************************

static INTERPOLATION_DATA move_tbl_run_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 腰
static INTERPOLATION_DATA move_tbl_run_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 左腕
static INTERPOLATION_DATA move_tbl_run_lshoulder[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(12.0f, 20.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(12.0f, 18.0f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_larm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_lelbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -17.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_larm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_rshoulder[] = {	// pos, rot, scl, frame

	{ XMFLOAT3(-12.0f, 18.0f, 0.0f), XMFLOAT3(-0.3f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-12.0f, 18.0f, 0.0f), XMFLOAT3(0.3f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_rarm01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, -5.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_relbow[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-4.0f, -15.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_rarm02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 左足
static INTERPOLATION_DATA move_tbl_run_lleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(7.0f, 4.0f, -2.0f), XMFLOAT3(0.8f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_lknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_lleg02[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_lfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(2.0f, -18.0f, 0.0f), XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// 右足
static INTERPOLATION_DATA move_tbl_run_rleg01[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(0.8f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-7.0f, 4.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_rknee[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-2.0f, -23.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_rleg02[] = { // pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -4.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

static INTERPOLATION_DATA move_tbl_run_rfoot[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -18.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ツール
static INTERPOLATION_DATA move_tbl_run_tool[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	move_tbl_idle_head,
	move_tbl_idle_hip,

	move_tbl_idle_lshoulder,
	move_tbl_idle_larm01,
	move_tbl_idle_lelbow,
	move_tbl_idle_larm02,

	move_tbl_idle_rshoulder,
	move_tbl_idle_rarm01,
	move_tbl_idle_relbow,
	move_tbl_idle_rarm02,

	move_tbl_idle_lleg01,
	move_tbl_idle_lknee,
	move_tbl_idle_lleg02,
	move_tbl_idle_lfoot,

	move_tbl_idle_rleg01,
	move_tbl_idle_rknee,
	move_tbl_idle_rleg02,
	move_tbl_idle_rfoot,

	move_tbl_idle_tool,

	move_tbl_walk_head,
	move_tbl_walk_hip,

	move_tbl_walk_lshoulder,
	move_tbl_walk_larm01,
	move_tbl_walk_lelbow,
	move_tbl_walk_larm02,

	move_tbl_walk_rshoulder,
	move_tbl_walk_rarm01,
	move_tbl_walk_relbow,
	move_tbl_walk_rarm02,

	move_tbl_walk_lleg01,
	move_tbl_walk_lknee,
	move_tbl_walk_lleg02,
	move_tbl_walk_lfoot,

	move_tbl_walk_rleg01,
	move_tbl_walk_rknee,
	move_tbl_walk_rleg02,
	move_tbl_walk_rfoot,

	move_tbl_walk_tool,


	move_tbl_stance_head,
	move_tbl_stance_hip,

	move_tbl_stance_lshoulder,
	move_tbl_stance_larm01,
	move_tbl_stance_lelbow,
	move_tbl_stance_larm02,

	move_tbl_stance_rshoulder,
	move_tbl_stance_rarm01,
	move_tbl_stance_relbow,
	move_tbl_stance_rarm02,

	move_tbl_stance_lleg01,
	move_tbl_stance_lknee,
	move_tbl_stance_lleg02,
	move_tbl_stance_lfoot,

	move_tbl_stance_rleg01,
	move_tbl_stance_rknee,
	move_tbl_stance_rleg02,
	move_tbl_stance_rfoot,

	move_tbl_stance_tool,


	move_tbl_throw_head,
	move_tbl_throw_hip,

	move_tbl_throw_lshoulder,
	move_tbl_throw_larm01,
	move_tbl_throw_lelbow,
	move_tbl_throw_larm02,

	move_tbl_throw_rshoulder,
	move_tbl_throw_rarm01,
	move_tbl_throw_relbow,
	move_tbl_throw_rarm02,

	move_tbl_throw_lleg01,
	move_tbl_throw_lknee,
	move_tbl_throw_lleg02,
	move_tbl_throw_lfoot,

	move_tbl_throw_rleg01,
	move_tbl_throw_rknee,
	move_tbl_throw_rleg02,
	move_tbl_throw_rfoot,

	move_tbl_throw_tool,


	move_tbl_attak_head,
	move_tbl_attak_hip,

	move_tbl_attak_lshoulder,
	move_tbl_attak_larm01,
	move_tbl_attak_lelbow,
	move_tbl_attak_larm02,

	move_tbl_attak_rshoulder,
	move_tbl_attak_rarm01,
	move_tbl_attak_relbow,
	move_tbl_attak_rarm02,

	move_tbl_attak_lleg01,
	move_tbl_attak_lknee,
	move_tbl_attak_lleg02,
	move_tbl_attak_lfoot,

	move_tbl_attak_rleg01,
	move_tbl_attak_rknee,
	move_tbl_attak_rleg02,
	move_tbl_attak_rfoot,

	move_tbl_attak_tool,


	move_tbl_run_head,
	move_tbl_run_hip,

	move_tbl_run_lshoulder,
	move_tbl_run_larm01,
	move_tbl_run_lelbow,
	move_tbl_run_larm02,

	move_tbl_run_rshoulder,
	move_tbl_run_rarm01,
	move_tbl_run_relbow,
	move_tbl_run_rarm02,

	move_tbl_run_lleg01,
	move_tbl_run_lknee,
	move_tbl_run_lleg02,
	move_tbl_run_lfoot,

	move_tbl_run_rleg01,
	move_tbl_run_rknee,
	move_tbl_run_rleg02,
	move_tbl_run_rfoot,

	move_tbl_run_tool,

};

int clTime = 0;	//	射撃間隔

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER_BODY, &g_Player.model);
	g_Player.load = TRUE;

	// フィールドによっての初期位置を変える
	switch (GetField())
	{
	case VILLAGE:
		g_Player.pos = { -220, PLAYER_OFFSET_Y, 120.0f };
		g_Player.rot = { 0.0f,  0.0, 0.0f };
		g_Player.scl = { 0.22f, 0.22f, 0.22f };

		g_Player.hp = PLAYER_HP;

		break;

	case DUNGEON_FIRST_FLOOR:
		g_Player.pos = { -140, PLAYER_OFFSET_Y, -600.0f };
		g_Player.rot = { 0.0f,  0.0, 0.0f };
		g_Player.scl = { 0.22f, 0.22f, 0.22f };

		break;

	case DUNGEON_SECOND_FLOOR:
		g_Player.pos = { -140, PLAYER_OFFSET_Y, -600.0f };
		g_Player.rot = { 0.0f,  0.0, 0.0f };
		g_Player.scl = { 0.22f, 0.22f, 0.22f };

		g_Player.hp = PLAYER_HP;


		break;
	}


#ifdef _DEBUG

	g_Player.hp = PLAYER_HP;

#endif

	g_Player.sp = PLAYER_SP;

	g_Player.spd = 0.0f;			// 移動スピードクリア
	g_Player.size = PLAYER_SIZE;	// 当たり判定の大きさ

	g_Player.use = TRUE;

	g_Player.anim = TRUE;	// アニメーションの切り替え

	// プレイヤー姿勢
	g_Player.pose = 0;	// 0:直立 / 1:歩行 / 2:構え / 3:投擲 / 4:攻撃 / 5:走り

	// プレイヤーが手に持っているもの
	g_Player.tool = 0;	// 0:剣 / 1:松明 / 2:投げナイフ / 3:投げナイフ(炎) / 4:投げナイフ(毒) / 5:投げナイフ(×3) / 5:投げナイフ(炎×3) / 5:投げナイフ(毒×3)

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号

	// 階層アニメーション用の初期化処理
	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる

	// パーツの初期化
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 親子関係
		g_Parts[i].parent = &g_Player;		// ← ここに親のアドレスを入れる
	//	g_Parts[腕].parent= &g_Player;		// 腕だったら親は本体（プレイヤー）
	//	g_Parts[手].parent= &g_Parts[腕];	// 指が腕の子供だった場合の例

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].time = 0.0f;		// 線形補間用のタイマーをクリア
		g_Parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		// パーツの読み込みはまだしていない
		g_Parts[i].load = 0;
	}

	// 頭
	g_Parts[0].use = TRUE;
	g_Parts[0].parent = &g_Player;	// 親をセット
	g_Parts[0].tblNo = 0;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0].tblMax = sizeof(move_tbl_idle_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[0].load = 1;
	LoadModel(MODEL_PLAYER_HEAD, &g_Parts[0].model);


	// 腰
	g_Parts[1].use = TRUE;
	g_Parts[1].parent = &g_Player;	// 親をセット
	g_Parts[1].tblNo = 1;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[1].tblMax = sizeof(move_tbl_idle_hip) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[1].load = 1;
	LoadModel(MODEL_PLAYER_HIP, &g_Parts[1].model);


	// 左腕
	g_Parts[2].use = TRUE;
	g_Parts[2].parent = &g_Player; // 親をセット
	g_Parts[2].tblNo = 2;			 // 再生するアニメデータの先頭アドレスをセット
	g_Parts[2].tblMax = sizeof(move_tbl_idle_lshoulder) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[2].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER_L, &g_Parts[2].model);

	g_Parts[3].use = TRUE;
	g_Parts[3].parent = &g_Parts[2];  // 親をセット
	g_Parts[3].tblNo = 3;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[3].tblMax = sizeof(move_tbl_idle_larm01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[3].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_ARM_L, &g_Parts[3].model);

	g_Parts[4].use = TRUE;
	g_Parts[4].parent = &g_Parts[3];  // 親をセット
	g_Parts[4].tblNo = 4;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[4].tblMax = sizeof(move_tbl_idle_lelbow) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[4].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW_L, &g_Parts[4].model);

	g_Parts[5].use = TRUE;
	g_Parts[5].parent = &g_Parts[4];  // 親をセット
	g_Parts[5].tblNo = 5;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[5].tblMax = sizeof(move_tbl_idle_larm02) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[5].load = 1;
	LoadModel(MODEL_PLAYER_FORE_ARM_L, &g_Parts[5].model);


	// 右腕
	g_Parts[6].use = TRUE;
	g_Parts[6].parent = &g_Player;	// 親をセット
	g_Parts[6].tblNo = 6;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[6].tblMax = sizeof(move_tbl_idle_rshoulder) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[6].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER_R, &g_Parts[6].model);

	g_Parts[7].use = TRUE;
	g_Parts[7].parent = &g_Parts[6];  // 親をセット
	g_Parts[7].tblNo = 7;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[7].tblMax = sizeof(move_tbl_idle_rarm01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[7].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_ARM_R, &g_Parts[7].model);

	g_Parts[8].use = TRUE;
	g_Parts[8].parent = &g_Parts[7];  // 親をセット
	g_Parts[8].tblNo = 8;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[8].tblMax = sizeof(move_tbl_idle_relbow) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[8].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW_R, &g_Parts[8].model);

	g_Parts[9].use = TRUE;
	g_Parts[9].parent = &g_Parts[8];  // 親をセット
	g_Parts[9].tblNo = 9;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[9].tblMax = sizeof(move_tbl_idle_rarm02) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[9].load = 1;
	LoadModel(MODEL_PLAYER_FORE_ARM_R, &g_Parts[9].model);


	// 左足
	g_Parts[10].use = TRUE;
	g_Parts[10].parent = &g_Parts[1];  // 親をセット
	g_Parts[10].tblNo = 10;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[10].tblMax = sizeof(move_tbl_idle_lleg01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[10].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_LEG_L, &g_Parts[10].model);

	g_Parts[11].use = TRUE;
	g_Parts[11].parent = &g_Parts[10];  // 親をセット
	g_Parts[11].tblNo = 11;			    // 再生するアニメデータの先頭アドレスをセット
	g_Parts[11].tblMax = sizeof(move_tbl_idle_lknee) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[11].load = 1;
	LoadModel(MODEL_PLAYER_KNEE_L, &g_Parts[11].model);

	g_Parts[12].use = TRUE;
	g_Parts[12].parent = &g_Parts[11];  // 親をセット
	g_Parts[12].tblNo = 12;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[12].tblMax = sizeof(move_tbl_idle_lleg02) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[12].load = 1;
	LoadModel(MODEL_PLAYER_LOWER_LEG_L, &g_Parts[12].model);

	g_Parts[13].use = TRUE;
	g_Parts[13].parent = &g_Parts[12];  // 親をセット
	g_Parts[13].tblNo = 13;			    // 再生するアニメデータの先頭アドレスをセット
	g_Parts[13].tblMax = sizeof(move_tbl_idle_lfoot) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[13].load = 1;
	LoadModel(MODEL_PLAYER_FOOT_L, &g_Parts[13].model);


	// 右足
	g_Parts[14].use = TRUE;
	g_Parts[14].parent = &g_Parts[1];  // 親をセット
	g_Parts[14].tblNo = 14;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[14].tblMax = sizeof(move_tbl_idle_rleg01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[14].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_LEG_R, &g_Parts[14].model);

	g_Parts[15].use = TRUE;
	g_Parts[15].parent = &g_Parts[14];  // 親をセット
	g_Parts[15].tblNo = 15;			  // 再生するアニメデータの先頭アドレスをセット
	g_Parts[15].tblMax = sizeof(move_tbl_idle_rknee) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[15].load = 1;
	LoadModel(MODEL_PLAYER_KNEE_R, &g_Parts[15].model);

	g_Parts[16].use = TRUE;
	g_Parts[16].parent = &g_Parts[15];  // 親をセット
	g_Parts[16].tblNo = 16;				// 再生するアニメデータの先頭アドレスをセット
	g_Parts[16].tblMax = sizeof(move_tbl_idle_rleg02) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[16].load = 1;
	LoadModel(MODEL_PLAYER_LOWER_LEG_R, &g_Parts[16].model);

	g_Parts[17].use = TRUE;
	g_Parts[17].parent = &g_Parts[16];  // 親をセット
	g_Parts[17].tblNo = 17;			    // 再生するアニメデータの先頭アドレスをセット
	g_Parts[17].tblMax = sizeof(move_tbl_idle_rfoot) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[17].load = 1;
	LoadModel(MODEL_PLAYER_FOOT_R, &g_Parts[17].model);

	// ツール
	g_Parts[18].use = TRUE;
	g_Parts[18].parent = &g_Parts[9];  // 親をセット
	g_Parts[18].tblNo = 18;			    // 再生するアニメデータの先頭アドレスをセット
	g_Parts[18].tblMax = sizeof(move_tbl_idle_tool) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[18].load = 1;

	// ツールの読み込み
	{
		g_Load = TRUE;

		LoadModel(MODEL_PLAYER_SWORD, &mSword);
		LoadModel(MODEL_PLAYER_TORCH, &mTorch);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// モデルの解放処理
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;

		// パーツの解放処理
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}

	}

	//　ツールの解放処理
	if (g_Load)
	{
		g_Load = FALSE;

		UnloadModel(&mSword);

		UnloadModel(&mTorch);
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//-------------------------------------------------------------------------
	// OBJ情報の取得
	//-------------------------------------------------------------------------
	CAMERA* cam = GetCamera();		// カメラ
	LIGHT* light = GetLightData(1); // ライト
	BULLET* bullet = GetBullet();   // バレット

	// 移動前のポジションを保存
	XMFLOAT3 oldPos = g_Player.pos;

	// 入力前の姿勢を保持
	int oldPose = g_Player.pose;

	// ポーズを基本状態にする
	g_Player.pose = 0;

	// 旋回速度
	const float speed = VALUE_ROTATE; // 適切な速度を選択

	//-------------------------------------------------------------------------
	// 移動処理
	//-------------------------------------------------------------------------
	{
		// キーボードの移動
		if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_W))
		{
			// 左上へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI * 3 / 4;

		}
		else if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_W))
		{
			// 右上へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = -XM_PI * 3 / 4;
		}
		else if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_S))
		{
			// 左下へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI / 4;
		}
		else if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_S))
		{
			// 右下へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = -XM_PI / 4;
		}
		else if (GetKeyboardPress(DIK_A))
		{
			// 左へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI / 2;
		}
		else if (GetKeyboardPress(DIK_D))
		{
			// 右へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = -XM_PI / 2;

		}
		else if (GetKeyboardPress(DIK_W))
		{
			// 上へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			// 下へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = 0.0f;
		}

		// コントローラーの移動
		if (IsButtonPressed(1, BUTTON_UP))
		{
			// 上へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI;

		}
		else if (IsButtonPressed(1, BUTTON_DOWN))
		{
			// 下へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = 0.0f;

		}
		else if (IsButtonPressed(1, BUTTON_LEFT))
		{
			// 左へ移動
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI / 2;

		}
		else if (IsButtonPressed(1, BUTTON_RIGHT))
		{
			g_Player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = -XM_PI / 2;

		}

		if (g_Player.spd >= 0.0f)
		{

			// 現在の方向から目標の方向への角度差を求める
			angleDifference = targetDir - g_Player.dir;

			// 角度差を正確な範囲に調整
			while (angleDifference > XM_PI)
			{
				angleDifference -= 2 * XM_PI;
			}

			while (angleDifference < -XM_PI)
			{
				angleDifference += 2 * XM_PI;
			}

			// 補間を使用してg_Player.dirを目標の方向に近づける
			g_Player.dir = Lerp(g_Player.dir, g_Player.dir + angleDifference, speed);

		}

		// ダッシュ用
		if (GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(1, BUTTON_Z))
		{
			if (g_Player.spd == VALUE_MOVE)
			{
				g_Player.spd = VALUE_DASH_MOVE;

				// ダッシュアニメーション
				g_Player.pose = 5;
			}
		}

		// Key入力があったら移動処理する
		if (g_Player.spd > 0.0f)
		{
			g_Player.rot.y = g_Player.dir + cam->rot.y;

			// 入力のあった方向へプレイヤーを向かせて移動させる
			g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
			g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
		}

		// 影もプレイヤーの位置に合わせる
		XMFLOAT3 pos = g_Player.pos;
		pos.y -= (PLAYER_OFFSET_Y - 0.1f);
		SetPositionShadow(g_Player.shadowIdx, pos);

	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd = 0.0f;


	}
#endif

	//-------------------------------------------------------------------------
	// 歩行アニメーション用
	//-------------------------------------------------------------------------
	{
		//アニメーションの初期化
		if (GetKeyboardTrigger(DIK_W) ||
			GetKeyboardTrigger(DIK_A) ||
			GetKeyboardTrigger(DIK_S) ||
			GetKeyboardTrigger(DIK_D) ||
			IsButtonTriggered(1, BUTTON_UP) ||
			IsButtonTriggered(1, BUTTON_DOWN) ||
			IsButtonTriggered(1, BUTTON_LEFT) ||
			IsButtonTriggered(1, BUTTON_RIGHT))
		{
			// アニメーション再生時間の初期化
			for (int t = 0; t < PLAYER_PARTS_MAX; t++)
			{
				g_Parts[t].time = 0;
			}
		}

		// 歩行アニメーション切り替え
		if (g_Player.spd == VALUE_MOVE)
		{
			g_Player.pose = 1;
		}
	}

	//-------------------------------------------------------------------------
	// ツール切り替え
	//-------------------------------------------------------------------------
	{
		// アニメーションを再生中は、ツールを切り替えられないようにする
		if ((g_Player.anim) && (oldPose != 2))
		{
			// ツール切り替え
			if (GetKeyboardTrigger(DIK_1))
			{// 剣
				g_Player.tool = 0;
			}

			if (GetKeyboardTrigger(DIK_2))
			{// 松明
				g_Player.tool = 1;
			}

			if (GetKeyboardTrigger(DIK_3))
			{// 投げナイフ用
				g_Player.tool = 2;
			}

			if (GetKeyboardTrigger(DIK_4))
			{// 投げナイフ(炎)用
				g_Player.tool = 3;
			}

			if (GetKeyboardTrigger(DIK_5))
			{// 投げナイフ(毒)用
				g_Player.tool = 4;
			}

			if (GetKeyboardTrigger(DIK_6))
			{// 投げナイフ(×3)用
				g_Player.tool = 5;
			}

			if (GetKeyboardTrigger(DIK_7))
			{// 投げナイフ(炎×3)用
				g_Player.tool = 6;
			}

			if (GetKeyboardTrigger(DIK_8))
			{// 投げナイフ(毒×3)用
				g_Player.tool = 7;
			}

			if (IsButtonTriggered(1, BUTTON_A))
			{
				padTool++;

				g_Player.tool = padTool % 8;

			}

		}
	}

	//-------------------------------------------------------------------------
	// 攻撃
	//-------------------------------------------------------------------------
	if (g_Player.anim)
	{


		//-------------------------------------------------------------------------
		// 剣による攻撃
		//-------------------------------------------------------------------------
		if (g_Player.tool == 0)
		{
			if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_B))
			{
				// アニメーションを再生する
				g_Player.anim = FALSE;

				// 姿勢の再設定
				oldPose = 4;

				g_Player.pose = 4;

				// アニメーション再生時間の初期化
				for (int s = 0; s < PLAYER_PARTS_MAX; s++)
				{
					g_Parts[s].time = 0;
				}

			}
		}

	}

	//-------------------------------------------------------------------------
	// 投げナイフによる攻撃
	//-------------------------------------------------------------------------
	{
		if ((g_Player.tool == 2) || (g_Player.tool == 3) || (g_Player.tool == 4) || (g_Player.tool == 5) || (g_Player.tool == 6) || (g_Player.tool == 7))
		{
			if (BULLET_TIME < clTime)
			{
				// 構え中
				{
					if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_B))
					{
						// spが足りなければアニメーションを変更しない
						switch (g_Player.tool)
						{
						case 2:
							if (g_Player.sp >= 5.0f)
							{
								clTime = 0;				// クールタイム
								g_Player.pose = 2;		// アニメーションの切り替え
							}
							break;
						case 3:
						case 4:
							if (g_Player.sp >= 10.0f)
							{
								clTime = 0;				// クールタイム
								g_Player.pose = 2;		// アニメーションの切り替え
							}
							break;
						case 5:
							if (g_Player.sp >= 15.0f)
							{
								clTime = 0;				// クールタイム
								g_Player.pose = 2;		// アニメーションの切り替え
							}
							break;
						case 6:
						case 7:
							if (g_Player.sp >= 30.0f)
							{
								clTime = 0;				// クールタイム
								g_Player.pose = 2;		// アニメーションの切り替え
							}
							break;
						}

						// アニメーション再生時間の初期化
						for (int m = 0; m < PLAYER_PARTS_MAX; m++)
						{
							g_Parts[m].time = 0;
						}

						XMFLOAT3 pos = g_Player.pos;

						pos.y += 5.8f;

						//　プレイヤーの向いている方向を向かせて位置を設定する
						pos.x -= sinf(g_Player.rot.y - 0.55f) * 9.0f;
						pos.z -= cosf(g_Player.rot.y - 0.55f) * 9.0f;

						XMFLOAT3 rot = g_Player.rot;
						rot.z += XM_PI / 2;
						BOOL shot = FALSE;
						int type = 0;

						// バレットタイプ設定
						switch (g_Player.tool)
						{

						case 2:

							if (g_Player.sp >= 5.0f)
							{

								g_Player.sp -= 5.0f;

								type = 0;

								SetBullet(pos, rot, shot, type, 0);

							}
							break;

						case 3:

							if (g_Player.sp >= 10.0f)
							{

								g_Player.sp -= 10.0f;

								type = 1;

								SetBullet(pos, rot, shot, type, 0);

							}
							break;


						case 4:

							if (g_Player.sp >= 10.0f)
							{

								g_Player.sp -= 10.0f;

								type = 2;

								SetBullet(pos, rot, shot, type, 0);
							}
							break;

						case 5:

							if (g_Player.sp >= 15.0f)
							{

								g_Player.sp -= 15.0f;

								type = 0;

								// 中央
								SetBullet(pos, rot, shot, type, 0);

								// 左
								SetBullet(pos, rot, shot, type, 1);

								// 右
								SetBullet(pos, rot, shot, type, 2);

							}

							break;

						case 6:

							if (g_Player.sp >= 30.0f)
							{

								g_Player.sp -= 30.0f;

								type = 1;

								// 中央
								SetBullet(pos, rot, shot, type, 0);

								// 左
								SetBullet(pos, rot, shot, type, 1);

								// 右
								SetBullet(pos, rot, shot, type, 2);

							}

							break;

						case 7:

							if (g_Player.sp >= 30.0f)
							{

								g_Player.sp -= 30.0f;

								type = 2;

								// 中央
								SetBullet(pos, rot, shot, type, 0);

								// 左
								SetBullet(pos, rot, shot, type, 1);

								// 右
								SetBullet(pos, rot, shot, type, 2);

							}

							break;

						}

					}

					// 発射した時のアニメーション
					if (GetKeyboardRelease(DIK_SPACE) || IsButtonReleased(1, BUTTON_B))
					{
						// アニメーションを再生する
						g_Player.anim = FALSE;

						// 姿勢の再設定
						oldPose = 3;

						g_Player.pose = 3;

					}

				}
			}
		}

		// クールタイムの時間経過
		clTime++;

		// 攻撃用のポイントの回復
		if (g_Player.sp <= PLAYER_SP - 0.1f)
		{
			g_Player.sp += 0.1f;
		}

		// 構え中どうか
		for (int o = 0; o < BULLET_MAX; o++)
		{
			if ((bullet[o].shot == FALSE) && (bullet[o].use == TRUE))
			{
				g_Player.pose = 2;
			}
		}

	}


	//-------------------------------------------------------------------------
	// 階層アニメーション
	//-------------------------------------------------------------------------
	{
		// アニメーション再生中
		switch (g_Player.tool)
		{
		case 0:
			if ((oldPose == 4) && (!g_Player.anim))
			{
				g_Player.pose = oldPose;
			}
			break;

		case 1:
			break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			if ((oldPose == 3) && (!g_Player.anim))
			{
				g_Player.pose = oldPose;
			}
			break;
		}

		// アニメーション切替用
		for (int j = 0; j < PLAYER_PARTS_MAX; j++)
		{

			switch (g_Player.pose)
			{
			case 0: // 直立



					// アニメーションを切り替える
				g_Parts[j].tblNo = j;

				// 姿勢を変更する
				g_Player.rot.x = 0.0f;

				// 最大テーブル数
				g_Parts[j].tblMax = sizeof(move_tbl_idle_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット



				break;

			case 1: // 歩行

					// アニメーションを切り替える
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX;

				// 姿勢を変更する
				g_Player.rot.x = 0.1f;

				// 最大テーブル数
				g_Parts[j].tblMax = sizeof(move_tbl_walk_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


				break;

			case 2: // 構え

					// アニメーションを切り替える
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX * 2;

				// 姿勢を変更する
				g_Player.rot.x = 0.0f;

				// 最大テーブル数
				g_Parts[j].tblMax = sizeof(move_tbl_stance_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

				break;

			case 3: // 投擲

					// アニメーションを切り替える
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX * 3;

				// 姿勢を変更する
				g_Player.rot.x = 0.0f;

				// 最大テーブル数
				g_Parts[j].tblMax = sizeof(move_tbl_throw_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

				break;

			case 4: // 攻撃


					// アニメーションを切り替える
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX * 4;

				// 姿勢を変更する
				g_Player.rot.x = 0.0f;

				// 最大テーブル数
				g_Parts[j].tblMax = sizeof(move_tbl_attak_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


				break;

			case 5: // 走り

					// アニメーションを切り替える
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX * 5;

				// 姿勢を変更する
				g_Player.rot.x = 0.2f;

				// 最大テーブル数
				g_Parts[j].tblMax = sizeof(move_tbl_run_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


			}

		}

		// 歩いていたら歩行アニメーションを再生する
		if (g_Player.spd >= VALUE_MOVE)
		{
			// 足のパーツのみアニメーションを変える
			for (int p = 10; p < 18; ++p)
			{
				// 足のパーツ
				g_Parts[p].tblNo = p + PLAYER_PARTS_MAX;

				// 最大テーブル数(g_Parts[10]基準)
				g_Parts[p].tblMax = sizeof(move_tbl_walk_lleg01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
			}
		}

		// アニメーション用
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			// 使われているなら処理する
			if ((g_Parts[i].use) && (g_Parts[i].tblMax > 0))
			{
				// 線形補間の処理
				int nowNo = (int)g_Parts[i].time;	// 整数分であるテーブル番号を取り出している
				int maxNo = g_Parts[i].tblMax;		// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;	// 移動先テーブルの番号を求めている

				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = g_Parts[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;			// 現在の移動量を計算している
				Rot *= nowTime;			// 現在の回転量を計算している
				Scl *= nowTime;			// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

				// frameを使て時間経過処理をする
				g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている

				if ((int)g_Parts[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Parts[i].time -= maxNo;				// 0番目にリセットしつつも小数部分を引き継いでいる
				}

				// アニメーションを最後まで再生したか
				if ((int)g_Parts[i].time >= maxNo - 1)			// 登録テーブル最後まで移動したか？
				{
					g_Player.anim = TRUE;

				}

			}
		}

	}

	//-------------------------------------------------------------------------
	// カメラ処理
	//-------------------------------------------------------------------------
	// カメラの追従
	{
		// プレイヤー視点
		XMFLOAT3 pos = g_Player.pos;

		pos.x += sinf(cam[0].rot.y - XM_PI / 2) * 12.0f;
		pos.z += cosf(cam[0].rot.y - XM_PI / 2) * 12.0f;

		pos.y = g_Player.pos.y + POS_Y_CAM;

		SetCameraAT(pos);
		SetCamera();

		//// カメラの拡大縮小
		//if ((g_Player.pose == 2) || (g_Player.pose == 3))
		//{// 近づく
		//	if (cam[0].len > POS_Z_CAM / 2)
		//	{
		//		cam[0].len -= VALUE_MOVE;
		//	}
		//	cam[0].pos.x = cam[0].at.x - sinf(cam[0].rot.y) * cam[0].len;
		//	cam[0].pos.z = cam[0].at.z - cosf(cam[0].rot.y) * cam[0].len;

		//}
		//else
		//{// 離れる
		//	if (cam[0].len < POS_Z_CAM)
		//	{
		//		cam[0].len += VALUE_MOVE;
		//	}
		//	cam[0].pos.x = cam[0].at.x - sinf(cam[0].rot.y) * cam[0].len;
		//	cam[0].pos.z = cam[0].at.z - cosf(cam[0].rot.y) * cam[0].len;

		//}

	}

	//-------------------------------------------------------------------------
	// マップ外処理
	//-------------------------------------------------------------------------

	// 村の中のみ
	if (GetField() == VILLAGE)
	{
		float distance = GetDistance(g_Player.pos, { 0.0f, 0.0f, 0.0f });

		if (distance > 1000)
		{
			g_Player.pos.x = -g_Player.pos.x;

			g_Player.pos.z = -g_Player.pos.z;
		}

	}


	//-------------------------------------------------------------------------
	// 攻撃判定
	//-------------------------------------------------------------------------
	// ダンジョン(1階)のみ
	if (GetField() == DUNGEON_FIRST_FLOOR)
	{
		if (g_Player.pose == 4)
		{
			ENEMY* enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				if (enemy[j].use == TRUE)
				{

					XMFLOAT3 pos = g_Player.pos;

					// プレイヤーの前方に移動
					pos.x -= sinf(g_Player.rot.y) * 20.0f;
					pos.z -= cosf(g_Player.rot.y) * 20.0f;

					if (CollisionBC(pos, enemy[j].pos, 15.0f, 15.0f))
					{
						// エネミーのHPを減らす
						enemy[j].hp -= 16.0f;

						ReleaseShadow(enemy[j].shadowIdx);

						// 攻撃エフェクト
						for (int k = 0; k < 100; k++)
						{
							// 敵の血
							XMFLOAT3 pos;
							XMFLOAT3 move;
							XMFLOAT3 scale;
							int nLife;
							float fSize;

							pos = g_Player.pos;

							pos.y = 15.0f;

							pos.x -= sinf(g_Player.rot.y - 0.6f) * 9.0f;
							pos.z -= cosf(g_Player.rot.y - 0.6f) * 9.0f;


							scale = { 0.08f, 0.08f, 0.08f };

							move.x = (300.0f - (rand() % 600)) / 1000;
							move.y = (300.0f - (rand() % 600)) / 1000;
							move.z = (300.0f - (rand() % 600)) / 1000;

							nLife = rand() % 50 + 60;

							fSize = (float)(rand() % 30 + 20);

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.1f, 1.0f, 0.3f, 0.6f), fSize, fSize, nLife, 1);

						}



					}

				}

			}
		}

	}

	//-------------------------------------------------------------------------
	// 当たり判定処理
	//-------------------------------------------------------------------------

	{	// 静的オブジェクトの当たり判定

		if (FieldHit(g_Player.pos, oldPos))
		{
			// ポジションをもとに戻す
			g_Player.pos = oldPos;
		}

	}

	//フィールドによって当たり判定を変える
	switch (GetField())
	{
	case VILLAGE:
		break;

	case DUNGEON_FIRST_FLOOR:

	// 爆弾との当たり判定
	{
		BOMB* bomb = GetBomb();
		for (int k = 0; k < BOMB_MAX; k++)
		{
			if (bomb[k].use == TRUE)
			{
				BOOL ans = CollisionBC(g_Player.pos, bomb[k].pos, 10.0f, 10.0f);

				if (ans == TRUE)
				{
					// ポジションをもとに戻す
					g_Player.pos = oldPos;

					break;
				}
			}
		}
	}

	break;

	case DUNGEON_SECOND_FLOOR:
		break;
	}

	//-------------------------------------------------------------------------
	// エフェクト処理
	//-------------------------------------------------------------------------
	{
		// 松明の炎
		if (g_Player.tool == 1)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			XMFLOAT3 scale;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = g_Player.pos;

			pos.y -= 0.8f;

			// 入力のあった方向へプレイヤーを向かせて移動させる
			pos.x -= sinf(g_Player.rot.y - 0.6f) * 9.0f;
			pos.z -= cosf(g_Player.rot.y - 0.6f) * 9.0f;

			// 歩いているまたは走っているかかどうか
			if ((g_Player.pose == 1) || (g_Player.pose == 5))
			{
				pos.y -= g_Parts[6].rot.x * 12;

				pos.y += 1;

			}

			scale = { 0.09f, 0.09f, 0.09f };

			fAngle = (float)(rand() % 628 - 314) / 200;
			fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 80;

			move.x = sinf(fAngle) * fLength;
			move.y = (rand() % 300 / 100.0f + 10) / 50;
			move.z = cosf(fAngle) * fLength;

			nLife = rand() % 50 + 60;

			fSize = (float)(rand() % 30 + 10);

			// ビルボードの設定
			SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);


			// 炎の明かり
			{
				pos.y += 20.0f;
				light->Position = pos;

				light->Diffuse = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);


				light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				light->Type = LIGHT_TYPE_POINT;
				light->Enable = TRUE;
				SetLightData(1, light);
			}

		}
		else // 持っているのが松明以外なら明かりを消す
		{
			light->Enable = FALSE;
			SetLightData(1, light);
		}


		// 目の赤
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			XMFLOAT3 scale;
			int nLife;
			float fSize;

			// 初期化
			pos = g_Player.pos;
			move.x = 0.0f;
			move.y = 0.0f;
			move.z = 0.0f;

			nLife = 200;

			fSize = 10.0f;

			// 姿勢によって目の位置を調整する
			switch (g_Player.pose)
			{
			case 0: // 通常姿勢

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y - 0.3f) * 2.3f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y - 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				break;

			case 1: // 前傾姿勢

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y - 0.2f) * 3.0f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y - 0.2f) * 3.0f;

				pos.y += 7.3f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			case 2: // 投擲姿勢
			case 3: // 構え姿勢
			case 4: // 攻撃姿勢

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y - 0.3f) * 2.3f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y - 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				// 歩いていたらサイズを大きくする
				if (g_Player.spd == VALUE_MOVE)
				{
					scale = { 0.02f, 0.02f, 0.02f };
				}

				break;

			case 5: // 走り姿勢

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y - 0.15f) * 3.7f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y - 0.15f) * 3.7f;

				pos.y += 7.0f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			}

			// 歩いるとき時の処理
			if (g_Player.spd == VALUE_MOVE)
			{

				nLife = 80;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y);
				pos.z += cosf(g_Player.rot.y);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y);
				pos.z += cosf(g_Player.rot.y);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}
			else if (g_Player.spd == VALUE_DASH_MOVE) // 走っているときの処理
			{

				nLife = 80;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;

				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;
				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

			}
			else // 歩いていない時
			{
				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}


			// 座標の初期化
			pos = g_Player.pos;

			// 姿勢によって目の位置を調整する
			switch (g_Player.pose)
			{
			case 0: // 通常姿勢

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y + 0.3f) * 2.3f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y + 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				break;

			case 1: // 前傾姿勢

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y + 0.2f) * 3.0f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y + 0.2f) * 3.0f;

				pos.y += 7.3f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			case 2: // 構え姿勢
			case 3: // 投擲姿勢
			case 4: // 攻撃姿勢

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y + 0.3f) * 2.3f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y + 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				// 歩いていたらサイズを大きくする
				if (g_Player.spd == VALUE_MOVE)
				{
					scale = { 0.02f, 0.02f, 0.02f };
				}

				break;

			case 5: // 走り姿勢

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y + 0.15f) * 3.7f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y + 0.15f) * 3.7f;

				pos.y += 7.0f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			}

			// 歩いるときの処理
			if (g_Player.spd == VALUE_MOVE)
			{

				nLife = 80;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y);
				pos.z += cosf(g_Player.rot.y);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y);
				pos.z += cosf(g_Player.rot.y);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}
			else if (g_Player.spd == VALUE_DASH_MOVE) // 走っているときの処理
			{

				nLife = 80;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;

				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;
				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

			}
			else // 歩いていない時
			{
				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}

		}

	}

	// 入力がない時にスピードを減少させる
	g_Player.spd *= 0.5f;

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{

	if (g_Player.use == FALSE) return;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// まずは基本となるＢＯＤＹを描画する

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	// 自分を描画したときにしようしたマトリクスを保存しておく
	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	// モデル描画
	DrawModel(&g_Player.model);


	// パーツの階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ↑
			// g_Player.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
		if (g_Parts[i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		if (i != PLAYER_PARTS_MAX - 1)
		{
			// モデル描画(体)
			DrawModel(&g_Parts[i].model);
		}
		else
		{	// 所持品の描画

			switch (g_Player.tool) // 今手に持っているものを描画する
			{
			case 0: // 剣
				DrawModel(&mSword);
				break;

			case 1: // 松明
				DrawModel(&mTorch);
				break;

			case 2: //投げナイフ
				break;
			}

		}
	}


	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}
//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// 距離計算の関数
//=============================================================================
float GetDistance(XMFLOAT3 mpos, XMFLOAT3 ypos)
{
	XMVECTOR temp = XMLoadFloat3(&mpos) - XMLoadFloat3(&ypos);
	XMVECTOR distanceSq = XMVector3LengthSq(temp);
	float distance = 0.0f;
	XMStoreFloat(&distance, distanceSq);

	return sqrtf(distance);  // 距離の平方根を返す
}
//=============================================================================
// 旋回用の関数
//=============================================================================
float Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}