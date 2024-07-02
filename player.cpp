//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 荒山　秀磨
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
static PLAYER		player;						// プレイヤー

static PLAYER		parts[PLAYER_PARTS_MAX];		// プレイヤーのパーツ用

DX11_MODEL			mSword;	// 剣
DX11_MODEL			mTorch;	// 松明		

static BOOL load; // ツール初期化用

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


static INTERPOLATION_DATA* moveTblAdr[] =
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
	LoadModel(MODEL_PLAYER_BODY, &player.model);
	player.load = TRUE;

	// フィールドによっての初期位置を変える
	switch (GetField())
	{
	case VILLAGE:
		player.pos = { -220, PLAYER_OFFSET_Y, 120.0f };
		player.rot = { 0.0f,  0.0, 0.0f };
		player.scl = { 0.22f, 0.22f, 0.22f };

		player.hp = PLAYER_HP;

		break;

	case DUNGEON_FIRST_FLOOR:
		player.pos = { -140, PLAYER_OFFSET_Y, -600.0f };
		player.rot = { 0.0f,  0.0, 0.0f };
		player.scl = { 0.22f, 0.22f, 0.22f };

		break;

	case DUNGEON_SECOND_FLOOR:
		player.pos = { -140, PLAYER_OFFSET_Y, -600.0f };
		player.rot = { 0.0f,  0.0, 0.0f };
		player.scl = { 0.22f, 0.22f, 0.22f };

		player.hp = PLAYER_HP;


		break;
	}


#ifdef _DEBUG

	player.hp = PLAYER_HP;

#endif

	player.sp = PLAYER_SP;

	player.spd = 0.0f;			// 移動スピードクリア
	player.size = PLAYER_SIZE;	// 当たり判定の大きさ

	player.use = TRUE;

	player.anim = TRUE;	// アニメーションの切り替え

	// プレイヤー姿勢
	player.pose = 0;	// 0:直立 / 1:歩行 / 2:構え / 3:投擲 / 4:攻撃 / 5:走り

	// プレイヤーが手に持っているもの
	player.tool = 0;	// 0:剣 / 1:松明 / 2:投げナイフ / 3:投げナイフ(炎) / 4:投げナイフ(毒) / 5:投げナイフ(×3) / 5:投げナイフ(炎×3) / 5:投げナイフ(毒×3)

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号

	// 階層アニメーション用の初期化処理
	player.parent = NULL;			// 本体（親）なのでNULLを入れる

	// パーツの初期化
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 親子関係
		parts[i].parent = &player;		// ← ここに親のアドレスを入れる
	//	parts[腕].parent= &player;		// 腕だったら親は本体（プレイヤー）
	//	parts[手].parent= &parts[腕];	// 指が腕の子供だった場合の例

		// 階層アニメーション用のメンバー変数の初期化
		parts[i].time = 0.0f;		// 線形補間用のタイマーをクリア
		parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		// パーツの読み込みはまだしていない
		parts[i].load = 0;
	}

	// 頭
	parts[0].use = TRUE;
	parts[0].parent = &player;	// 親をセット
	parts[0].tblNo = 0;			// 再生するアニメデータの先頭アドレスをセット
	parts[0].tblMax = sizeof(move_tbl_idle_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[0].load = 1;
	LoadModel(MODEL_PLAYER_HEAD, &parts[0].model);


	// 腰
	parts[1].use = TRUE;
	parts[1].parent = &player;	// 親をセット
	parts[1].tblNo = 1;			// 再生するアニメデータの先頭アドレスをセット
	parts[1].tblMax = sizeof(move_tbl_idle_hip) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[1].load = 1;
	LoadModel(MODEL_PLAYER_HIP, &parts[1].model);


	// 左腕
	parts[2].use = TRUE;
	parts[2].parent = &player; // 親をセット
	parts[2].tblNo = 2;			 // 再生するアニメデータの先頭アドレスをセット
	parts[2].tblMax = sizeof(move_tbl_idle_lshoulder) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[2].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER_L, &parts[2].model);

	parts[3].use = TRUE;
	parts[3].parent = &parts[2];  // 親をセット
	parts[3].tblNo = 3;			  // 再生するアニメデータの先頭アドレスをセット
	parts[3].tblMax = sizeof(move_tbl_idle_larm01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[3].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_ARM_L, &parts[3].model);

	parts[4].use = TRUE;
	parts[4].parent = &parts[3];  // 親をセット
	parts[4].tblNo = 4;			  // 再生するアニメデータの先頭アドレスをセット
	parts[4].tblMax = sizeof(move_tbl_idle_lelbow) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[4].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW_L, &parts[4].model);

	parts[5].use = TRUE;
	parts[5].parent = &parts[4];  // 親をセット
	parts[5].tblNo = 5;			  // 再生するアニメデータの先頭アドレスをセット
	parts[5].tblMax = sizeof(move_tbl_idle_larm02) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[5].load = 1;
	LoadModel(MODEL_PLAYER_FORE_ARM_L, &parts[5].model);


	// 右腕
	parts[6].use = TRUE;
	parts[6].parent = &player;	// 親をセット
	parts[6].tblNo = 6;			// 再生するアニメデータの先頭アドレスをセット
	parts[6].tblMax = sizeof(move_tbl_idle_rshoulder) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[6].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER_R, &parts[6].model);

	parts[7].use = TRUE;
	parts[7].parent = &parts[6];  // 親をセット
	parts[7].tblNo = 7;			  // 再生するアニメデータの先頭アドレスをセット
	parts[7].tblMax = sizeof(move_tbl_idle_rarm01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[7].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_ARM_R, &parts[7].model);

	parts[8].use = TRUE;
	parts[8].parent = &parts[7];  // 親をセット
	parts[8].tblNo = 8;			  // 再生するアニメデータの先頭アドレスをセット
	parts[8].tblMax = sizeof(move_tbl_idle_relbow) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[8].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW_R, &parts[8].model);

	parts[9].use = TRUE;
	parts[9].parent = &parts[8];  // 親をセット
	parts[9].tblNo = 9;			  // 再生するアニメデータの先頭アドレスをセット
	parts[9].tblMax = sizeof(move_tbl_idle_rarm02) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[9].load = 1;
	LoadModel(MODEL_PLAYER_FORE_ARM_R, &parts[9].model);


	// 左足
	parts[10].use = TRUE;
	parts[10].parent = &parts[1];  // 親をセット
	parts[10].tblNo = 10;			  // 再生するアニメデータの先頭アドレスをセット
	parts[10].tblMax = sizeof(move_tbl_idle_lleg01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[10].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_LEG_L, &parts[10].model);

	parts[11].use = TRUE;
	parts[11].parent = &parts[10];  // 親をセット
	parts[11].tblNo = 11;			    // 再生するアニメデータの先頭アドレスをセット
	parts[11].tblMax = sizeof(move_tbl_idle_lknee) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[11].load = 1;
	LoadModel(MODEL_PLAYER_KNEE_L, &parts[11].model);

	parts[12].use = TRUE;
	parts[12].parent = &parts[11];  // 親をセット
	parts[12].tblNo = 12;			  // 再生するアニメデータの先頭アドレスをセット
	parts[12].tblMax = sizeof(move_tbl_idle_lleg02) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[12].load = 1;
	LoadModel(MODEL_PLAYER_LOWER_LEG_L, &parts[12].model);

	parts[13].use = TRUE;
	parts[13].parent = &parts[12];  // 親をセット
	parts[13].tblNo = 13;			    // 再生するアニメデータの先頭アドレスをセット
	parts[13].tblMax = sizeof(move_tbl_idle_lfoot) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[13].load = 1;
	LoadModel(MODEL_PLAYER_FOOT_L, &parts[13].model);


	// 右足
	parts[14].use = TRUE;
	parts[14].parent = &parts[1];  // 親をセット
	parts[14].tblNo = 14;			  // 再生するアニメデータの先頭アドレスをセット
	parts[14].tblMax = sizeof(move_tbl_idle_rleg01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[14].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_LEG_R, &parts[14].model);

	parts[15].use = TRUE;
	parts[15].parent = &parts[14];  // 親をセット
	parts[15].tblNo = 15;			  // 再生するアニメデータの先頭アドレスをセット
	parts[15].tblMax = sizeof(move_tbl_idle_rknee) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[15].load = 1;
	LoadModel(MODEL_PLAYER_KNEE_R, &parts[15].model);

	parts[16].use = TRUE;
	parts[16].parent = &parts[15];  // 親をセット
	parts[16].tblNo = 16;				// 再生するアニメデータの先頭アドレスをセット
	parts[16].tblMax = sizeof(move_tbl_idle_rleg02) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[16].load = 1;
	LoadModel(MODEL_PLAYER_LOWER_LEG_R, &parts[16].model);

	parts[17].use = TRUE;
	parts[17].parent = &parts[16];  // 親をセット
	parts[17].tblNo = 17;			    // 再生するアニメデータの先頭アドレスをセット
	parts[17].tblMax = sizeof(move_tbl_idle_rfoot) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[17].load = 1;
	LoadModel(MODEL_PLAYER_FOOT_R, &parts[17].model);

	// ツール
	parts[18].use = TRUE;
	parts[18].parent = &parts[9];  // 親をセット
	parts[18].tblNo = 18;			    // 再生するアニメデータの先頭アドレスをセット
	parts[18].tblMax = sizeof(move_tbl_idle_tool) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	parts[18].load = 1;

	// ツールの読み込み
	{
		load = TRUE;

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
	if (player.load)
	{
		UnloadModel(&player.model);
		player.load = FALSE;

		// パーツの解放処理
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			UnloadModel(&parts[i].model);
			parts[i].load = FALSE;
		}

	}

	//　ツールの解放処理
	if (load)
	{
		load = FALSE;

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
	XMFLOAT3 oldPos = player.pos;

	// 入力前の姿勢を保持
	int oldPose = player.pose;

	// ポーズを基本状態にする
	player.pose = 0;

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
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI * 3 / 4;

		}
		else if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_W))
		{
			// 右上へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = -XM_PI * 3 / 4;
		}
		else if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_S))
		{
			// 左下へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI / 4;
		}
		else if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_S))
		{
			// 右下へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = -XM_PI / 4;
		}
		else if (GetKeyboardPress(DIK_A))
		{
			// 左へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI / 2;
		}
		else if (GetKeyboardPress(DIK_D))
		{
			// 右へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = -XM_PI / 2;

		}
		else if (GetKeyboardPress(DIK_W))
		{
			// 上へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			// 下へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = 0.0f;
		}

		// コントローラーの移動
		if (IsButtonPressed(1, BUTTON_UP))
		{
			// 上へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI;

		}
		else if (IsButtonPressed(1, BUTTON_DOWN))
		{
			// 下へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = 0.0f;

		}
		else if (IsButtonPressed(1, BUTTON_LEFT))
		{
			// 左へ移動
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = XM_PI / 2;

		}
		else if (IsButtonPressed(1, BUTTON_RIGHT))
		{
			player.spd = VALUE_MOVE;

			// 目標の方向
			targetDir = -XM_PI / 2;

		}

		if (player.spd >= 0.0f)
		{

			// 現在の方向から目標の方向への角度差を求める
			angleDifference = targetDir - player.dir;

			// 角度差を正確な範囲に調整
			while (angleDifference > XM_PI)
			{
				angleDifference -= 2 * XM_PI;
			}

			while (angleDifference < -XM_PI)
			{
				angleDifference += 2 * XM_PI;
			}

			// 補間を使用してplayer.dirを目標の方向に近づける
			player.dir = Lerp(player.dir, player.dir + angleDifference, speed);

		}

		// ダッシュ用
		if (GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(1, BUTTON_Z))
		{
			if (player.spd == VALUE_MOVE)
			{
				player.spd = VALUE_DASH_MOVE;

				// ダッシュアニメーション
				player.pose = 5;
			}
		}

		// Key入力があったら移動処理する
		if (player.spd > 0.0f)
		{
			player.rot.y = player.dir + cam->rot.y;

			// 入力のあった方向へプレイヤーを向かせて移動させる
			player.pos.x -= sinf(player.rot.y) * player.spd;
			player.pos.z -= cosf(player.rot.y) * player.spd;
		}

		// 影もプレイヤーの位置に合わせる
		XMFLOAT3 pos = player.pos;
		pos.y -= (PLAYER_OFFSET_Y - 0.1f);
		SetPositionShadow(player.shadowIdx, pos);

	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		player.pos.z = player.pos.x = 0.0f;
		player.rot.y = player.dir = 0.0f;
		player.spd = 0.0f;


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
				parts[t].time = 0;
			}
		}

		// 歩行アニメーション切り替え
		if (player.spd == VALUE_MOVE)
		{
			player.pose = 1;
		}
	}

	//-------------------------------------------------------------------------
	// ツール切り替え
	//-------------------------------------------------------------------------
	{
		// アニメーションを再生中は、ツールを切り替えられないようにする
		if ((player.anim) && (oldPose != 2))
		{
			// ツール切り替え
			if (GetKeyboardTrigger(DIK_1))
			{// 剣
				player.tool = 0;
			}

			if (GetKeyboardTrigger(DIK_2))
			{// 松明
				player.tool = 1;
			}

			if (GetKeyboardTrigger(DIK_3))
			{// 投げナイフ用
				player.tool = 2;
			}

			if (GetKeyboardTrigger(DIK_4))
			{// 投げナイフ(炎)用
				player.tool = 3;
			}

			if (GetKeyboardTrigger(DIK_5))
			{// 投げナイフ(毒)用
				player.tool = 4;
			}

			if (GetKeyboardTrigger(DIK_6))
			{// 投げナイフ(×3)用
				player.tool = 5;
			}

			if (GetKeyboardTrigger(DIK_7))
			{// 投げナイフ(炎×3)用
				player.tool = 6;
			}

			if (GetKeyboardTrigger(DIK_8))
			{// 投げナイフ(毒×3)用
				player.tool = 7;
			}

			if (IsButtonTriggered(1, BUTTON_A))
			{
				padTool++;

				player.tool = padTool % 8;

			}

		}
	}

	//-------------------------------------------------------------------------
	// 攻撃
	//-------------------------------------------------------------------------
	if (player.anim)
	{


		//-------------------------------------------------------------------------
		// 剣による攻撃
		//-------------------------------------------------------------------------
		if (player.tool == 0)
		{
			if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_B))
			{
				// アニメーションを再生する
				player.anim = FALSE;

				// 姿勢の再設定
				oldPose = 4;

				player.pose = 4;

				// アニメーション再生時間の初期化
				for (int s = 0; s < PLAYER_PARTS_MAX; s++)
				{
					parts[s].time = 0;
				}

			}
		}

	}

	//-------------------------------------------------------------------------
	// 投げナイフによる攻撃
	//-------------------------------------------------------------------------
	{
		if ((player.tool == 2) || (player.tool == 3) || (player.tool == 4) || (player.tool == 5) || (player.tool == 6) || (player.tool == 7))
		{
			if (BULLET_TIME < clTime)
			{
				// 構え中
				{
					if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_B))
					{
						// spが足りなければアニメーションを変更しない
						switch (player.tool)
						{
						case 2:
							if (player.sp >= 5.0f)
							{
								clTime = 0;				// クールタイム
								player.pose = 2;		// アニメーションの切り替え
							}
							break;
						case 3:
						case 4:
							if (player.sp >= 10.0f)
							{
								clTime = 0;				// クールタイム
								player.pose = 2;		// アニメーションの切り替え
							}
							break;
						case 5:
							if (player.sp >= 15.0f)
							{
								clTime = 0;				// クールタイム
								player.pose = 2;		// アニメーションの切り替え
							}
							break;
						case 6:
						case 7:
							if (player.sp >= 30.0f)
							{
								clTime = 0;				// クールタイム
								player.pose = 2;		// アニメーションの切り替え
							}
							break;
						}

						// アニメーション再生時間の初期化
						for (int m = 0; m < PLAYER_PARTS_MAX; m++)
						{
							parts[m].time = 0;
						}

						XMFLOAT3 pos = player.pos;

						pos.y += 5.8f;

						//　プレイヤーの向いている方向を向かせて位置を設定する
						pos.x -= sinf(player.rot.y - 0.55f) * 9.0f;
						pos.z -= cosf(player.rot.y - 0.55f) * 9.0f;

						XMFLOAT3 rot = player.rot;
						rot.z += XM_PI / 2;
						BOOL shot = FALSE;
						int type = 0;

						// バレットタイプ設定
						switch (player.tool)
						{

						case 2:

							if (player.sp >= 5.0f)
							{

								player.sp -= 5.0f;

								type = 0;

								SetBullet(pos, rot, shot, type, 0);

							}
							break;

						case 3:

							if (player.sp >= 10.0f)
							{

								player.sp -= 10.0f;

								type = 1;

								SetBullet(pos, rot, shot, type, 0);

							}
							break;


						case 4:

							if (player.sp >= 10.0f)
							{

								player.sp -= 10.0f;

								type = 2;

								SetBullet(pos, rot, shot, type, 0);
							}
							break;

						case 5:

							if (player.sp >= 15.0f)
							{

								player.sp -= 15.0f;

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

							if (player.sp >= 30.0f)
							{

								player.sp -= 30.0f;

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

							if (player.sp >= 30.0f)
							{

								player.sp -= 30.0f;

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
						player.anim = FALSE;

						// 姿勢の再設定
						oldPose = 3;

						player.pose = 3;

					}

				}
			}
		}

		// クールタイムの時間経過
		clTime++;

		// 攻撃用のポイントの回復
		if (player.sp <= PLAYER_SP - 0.1f)
		{
			player.sp += 0.1f;
		}

		// 構え中どうか
		for (int o = 0; o < BULLET_MAX; o++)
		{
			if ((bullet[o].shot == FALSE) && (bullet[o].use == TRUE))
			{
				player.pose = 2;
			}
		}

	}


	//-------------------------------------------------------------------------
	// 階層アニメーション
	//-------------------------------------------------------------------------
	{
		// アニメーション再生中
		switch (player.tool)
		{
		case 0:
			if ((oldPose == 4) && (!player.anim))
			{
				player.pose = oldPose;
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
			if ((oldPose == 3) && (!player.anim))
			{
				player.pose = oldPose;
			}
			break;
		}

		// アニメーション切替用
		for (int j = 0; j < PLAYER_PARTS_MAX; j++)
		{

			switch (player.pose)
			{
			case 0: // 直立



					// アニメーションを切り替える
				parts[j].tblNo = j;

				// 姿勢を変更する
				player.rot.x = 0.0f;

				// 最大テーブル数
				parts[j].tblMax = sizeof(move_tbl_idle_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット



				break;

			case 1: // 歩行

					// アニメーションを切り替える
				parts[j].tblNo = j + PLAYER_PARTS_MAX;

				// 姿勢を変更する
				player.rot.x = 0.1f;

				// 最大テーブル数
				parts[j].tblMax = sizeof(move_tbl_walk_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


				break;

			case 2: // 構え

					// アニメーションを切り替える
				parts[j].tblNo = j + PLAYER_PARTS_MAX * 2;

				// 姿勢を変更する
				player.rot.x = 0.0f;

				// 最大テーブル数
				parts[j].tblMax = sizeof(move_tbl_stance_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

				break;

			case 3: // 投擲

					// アニメーションを切り替える
				parts[j].tblNo = j + PLAYER_PARTS_MAX * 3;

				// 姿勢を変更する
				player.rot.x = 0.0f;

				// 最大テーブル数
				parts[j].tblMax = sizeof(move_tbl_throw_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

				break;

			case 4: // 攻撃


					// アニメーションを切り替える
				parts[j].tblNo = j + PLAYER_PARTS_MAX * 4;

				// 姿勢を変更する
				player.rot.x = 0.0f;

				// 最大テーブル数
				parts[j].tblMax = sizeof(move_tbl_attak_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


				break;

			case 5: // 走り

					// アニメーションを切り替える
				parts[j].tblNo = j + PLAYER_PARTS_MAX * 5;

				// 姿勢を変更する
				player.rot.x = 0.2f;

				// 最大テーブル数
				parts[j].tblMax = sizeof(move_tbl_run_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


			}

		}

		// 歩いていたら歩行アニメーションを再生する
		if (player.spd >= VALUE_MOVE)
		{
			// 足のパーツのみアニメーションを変える
			for (int p = 10; p < 18; ++p)
			{
				// 足のパーツ
				parts[p].tblNo = p + PLAYER_PARTS_MAX;

				// 最大テーブル数(parts[10]基準)
				parts[p].tblMax = sizeof(move_tbl_walk_lleg01) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
			}
		}

		// アニメーション用
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			// 使われているなら処理する
			if ((parts[i].use) && (parts[i].tblMax > 0))
			{
				// 線形補間の処理
				int nowNo = (int)parts[i].time;	// 整数分であるテーブル番号を取り出している
				int maxNo = parts[i].tblMax;		// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;	// 移動先テーブルの番号を求めている

				INTERPOLATION_DATA* tbl = moveTblAdr[parts[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = parts[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;			// 現在の移動量を計算している
				Rot *= nowTime;			// 現在の回転量を計算している
				Scl *= nowTime;			// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&parts[i].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&parts[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&parts[i].scl, nowScl + Scl);

				// frameを使て時間経過処理をする
				parts[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている

				if ((int)parts[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					parts[i].time -= maxNo;				// 0番目にリセットしつつも小数部分を引き継いでいる
				}

				// アニメーションを最後まで再生したか
				if ((int)parts[i].time >= maxNo - 1)			// 登録テーブル最後まで移動したか？
				{
					player.anim = TRUE;

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
		XMFLOAT3 pos = player.pos;

		pos.x += sinf(cam[0].rot.y - XM_PI / 2) * 12.0f;
		pos.z += cosf(cam[0].rot.y - XM_PI / 2) * 12.0f;

		pos.y = player.pos.y + POS_Y_CAM;

		SetCameraAT(pos);
		SetCamera();

		//// カメラの拡大縮小
		//if ((player.pose == 2) || (player.pose == 3))
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
		float distance = GetDistance(player.pos, { 0.0f, 0.0f, 0.0f });

		if (distance > 1000)
		{
			player.pos.x = -player.pos.x;

			player.pos.z = -player.pos.z;
		}

	}


	//-------------------------------------------------------------------------
	// 攻撃判定
	//-------------------------------------------------------------------------
	// ダンジョン(1階)のみ
	if (GetField() == DUNGEON_FIRST_FLOOR)
	{
		if (player.pose == 4)
		{
			ENEMY* enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				if (enemy[j].use == TRUE)
				{

					XMFLOAT3 pos = player.pos;

					// プレイヤーの前方に移動
					pos.x -= sinf(player.rot.y) * 20.0f;
					pos.z -= cosf(player.rot.y) * 20.0f;

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

							pos = player.pos;

							pos.y = 15.0f;

							pos.x -= sinf(player.rot.y - 0.6f) * 9.0f;
							pos.z -= cosf(player.rot.y - 0.6f) * 9.0f;


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

		if (FieldHit(player.pos, oldPos))
		{
			// ポジションをもとに戻す
			player.pos = oldPos;
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
				BOOL ans = CollisionBC(player.pos, bomb[k].pos, 10.0f, 10.0f);

				if (ans == TRUE)
				{
					// ポジションをもとに戻す
					player.pos = oldPos;

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
		if (player.tool == 1)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			XMFLOAT3 scale;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = player.pos;

			pos.y -= 0.8f;

			// 入力のあった方向へプレイヤーを向かせて移動させる
			pos.x -= sinf(player.rot.y - 0.6f) * 9.0f;
			pos.z -= cosf(player.rot.y - 0.6f) * 9.0f;

			// 歩いているまたは走っているかかどうか
			if ((player.pose == 1) || (player.pose == 5))
			{
				pos.y -= parts[6].rot.x * 12;

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
			pos = player.pos;
			move.x = 0.0f;
			move.y = 0.0f;
			move.z = 0.0f;

			nLife = 200;

			fSize = 10.0f;

			// 姿勢によって目の位置を調整する
			switch (player.pose)
			{
			case 0: // 通常姿勢

				pos.x -= sinf(player.rot.y + parts[0].rot.y - 0.3f) * 2.3f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y - 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				break;

			case 1: // 前傾姿勢

				pos.x -= sinf(player.rot.y + parts[0].rot.y - 0.2f) * 3.0f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y - 0.2f) * 3.0f;

				pos.y += 7.3f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			case 2: // 投擲姿勢
			case 3: // 構え姿勢
			case 4: // 攻撃姿勢

				pos.x -= sinf(player.rot.y + parts[0].rot.y - 0.3f) * 2.3f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y - 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				// 歩いていたらサイズを大きくする
				if (player.spd == VALUE_MOVE)
				{
					scale = { 0.02f, 0.02f, 0.02f };
				}

				break;

			case 5: // 走り姿勢

				pos.x -= sinf(player.rot.y + parts[0].rot.y - 0.15f) * 3.7f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y - 0.15f) * 3.7f;

				pos.y += 7.0f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			}

			// 歩いるとき時の処理
			if (player.spd == VALUE_MOVE)
			{

				nLife = 80;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y);
				pos.z += cosf(player.rot.y);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y);
				pos.z += cosf(player.rot.y);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}
			else if (player.spd == VALUE_DASH_MOVE) // 走っているときの処理
			{

				nLife = 80;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;

				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;
				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

			}
			else // 歩いていない時
			{
				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}


			// 座標の初期化
			pos = player.pos;

			// 姿勢によって目の位置を調整する
			switch (player.pose)
			{
			case 0: // 通常姿勢

				pos.x -= sinf(player.rot.y + parts[0].rot.y + 0.3f) * 2.3f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y + 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				break;

			case 1: // 前傾姿勢

				pos.x -= sinf(player.rot.y + parts[0].rot.y + 0.2f) * 3.0f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y + 0.2f) * 3.0f;

				pos.y += 7.3f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			case 2: // 構え姿勢
			case 3: // 投擲姿勢
			case 4: // 攻撃姿勢

				pos.x -= sinf(player.rot.y + parts[0].rot.y + 0.3f) * 2.3f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y + 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				// 歩いていたらサイズを大きくする
				if (player.spd == VALUE_MOVE)
				{
					scale = { 0.02f, 0.02f, 0.02f };
				}

				break;

			case 5: // 走り姿勢

				pos.x -= sinf(player.rot.y + parts[0].rot.y + 0.15f) * 3.7f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y + 0.15f) * 3.7f;

				pos.y += 7.0f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			}

			// 歩いるときの処理
			if (player.spd == VALUE_MOVE)
			{

				nLife = 80;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y);
				pos.z += cosf(player.rot.y);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y);
				pos.z += cosf(player.rot.y);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}
			else if (player.spd == VALUE_DASH_MOVE) // 走っているときの処理
			{

				nLife = 80;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;

				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;
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
	player.spd *= 0.5f;

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{

	if (player.use == FALSE) return;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// まずは基本となるＢＯＤＹを描画する

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(player.scl.x, player.scl.y, player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(player.rot.x, player.rot.y + XM_PI, player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(player.pos.x, player.pos.y, player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	// 自分を描画したときにしようしたマトリクスを保存しておく
	XMStoreFloat4x4(&player.mtxWorld, mtxWorld);

	// モデル描画
	DrawModel(&player.model);


	// パーツの階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(parts[i].scl.x, parts[i].scl.y, parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(parts[i].rot.x, parts[i].rot.y, parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(parts[i].pos.x, parts[i].pos.y, parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&parts[i].parent->mtxWorld));
			// ↑
			// player.mtxWorldを指している
		}

		XMStoreFloat4x4(&parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
		if (parts[i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		if (i != PLAYER_PARTS_MAX - 1)
		{
			// モデル描画(体)
			DrawModel(&parts[i].model);
		}
		else
		{	// 所持品の描画

			switch (player.tool) // 今手に持っているものを描画する
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
	return &player;
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