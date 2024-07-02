//=============================================================================
//
// ���f������ [player.cpp]
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
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER_BODY			"data/MODEL/player/body.obj" // �ǂݍ��ރ��f����
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

#define	VALUE_MOVE			(1.5f)			// �ړ���
#define VALUE_DASH_MOVE		(2.0f)			// �_�b�V�����̈ړ���
#define	VALUE_ROTATE		(XM_PI * 0.1f)	// ��]��

#define PLAYER_SHADOW_SIZE	(0.4f)			// �e�̑傫��
#define PLAYER_OFFSET_Y		(17.0f)			// �v���C���[�̑��������킹��

#define PLAYER_PARTS_MAX	(19)			// �v���C���[�̃p�[�c�̐�

#define	PLAYER_HP	(100)					// �q�b�g�|�C���g
#define PLAYER_SP   (100.0f)					// �U���p�̃|�C���g	

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;						// �v���C���[

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p

DX11_MODEL			mSword;	// ��
DX11_MODEL			mTorch;	// ����		

static BOOL g_Load; // �c�[���������p

int padTool = 0; // �p�b�h�̃c�[���ύX�p

// �v���C���[����p
float targetDir = 0.0f;	// �ڕW�̌���
float angleDifference = 0.0f; // ���݂̕����Ƃ̊p�x��


//*****************************************************************************
// ANIM IDLE // POSE 0  // FRAME 2
//*****************************************************************************

// ��
static INTERPOLATION_DATA move_tbl_idle_head[] = {// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f,  0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, -0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ��
static INTERPOLATION_DATA move_tbl_idle_hip[] = {// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ���r
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

// �E�r
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

// ����
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

// �E��
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

// �c�[��
static INTERPOLATION_DATA move_tbl_idle_tool[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


//*****************************************************************************
// ANIM WALK // POSE 1 // FRAME 2
//*****************************************************************************

// ��
static INTERPOLATION_DATA move_tbl_walk_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


// ��
static INTERPOLATION_DATA move_tbl_walk_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


// ���r
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


// �E�r
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


// ����
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


// �E��
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

// �c�[��
static INTERPOLATION_DATA move_tbl_walk_tool[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f,  -20.0f, 7.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};


//*****************************************************************************
// ANIM STANCE // POSE 2  // FRAME 2
//*****************************************************************************

// ��
static INTERPOLATION_DATA move_tbl_stance_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f, -0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 33.0f, 0.0f), XMFLOAT3(0.0f,  0.4f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ��
static INTERPOLATION_DATA move_tbl_stance_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ���r
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

// �E�r
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

// ����
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

// �E��
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

// �c�[��
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

// ��
static INTERPOLATION_DATA move_tbl_throw_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ���r
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

// �E�r
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

// ����
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

// �E��
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

// �c�[��
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

// ��
static INTERPOLATION_DATA move_tbl_attak_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ���r
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

// �E�r
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

// ����
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

// �E��
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

// �c�[��
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

// ��
static INTERPOLATION_DATA move_tbl_run_hip[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

// ���r
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

// ����
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

// �E��
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

// �c�[��
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

int clTime = 0;	//	�ˌ��Ԋu

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER_BODY, &g_Player.model);
	g_Player.load = TRUE;

	// �t�B�[���h�ɂ���Ă̏����ʒu��ς���
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

	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	g_Player.use = TRUE;

	g_Player.anim = TRUE;	// �A�j���[�V�����̐؂�ւ�

	// �v���C���[�p��
	g_Player.pose = 0;	// 0:���� / 1:���s / 2:�\�� / 3:���� / 4:�U�� / 5:����

	// �v���C���[����Ɏ����Ă������
	g_Player.tool = 0;	// 0:�� / 1:���� / 2:�����i�C�t / 3:�����i�C�t(��) / 4:�����i�C�t(��) / 5:�����i�C�t(�~3) / 5:�����i�C�t(���~3) / 5:�����i�C�t(�Ł~3)

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	// �K�w�A�j���[�V�����p�̏���������
	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	// �p�[�c�̏�����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// �e�q�֌W
		g_Parts[i].parent = &g_Player;		// �� �����ɐe�̃A�h���X������
	//	g_Parts[�r].parent= &g_Player;		// �r��������e�͖{�́i�v���C���[�j
	//	g_Parts[��].parent= &g_Parts[�r];	// �w���r�̎q���������ꍇ�̗�

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		g_Parts[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Parts[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		g_Parts[i].load = 0;
	}

	// ��
	g_Parts[0].use = TRUE;
	g_Parts[0].parent = &g_Player;	// �e���Z�b�g
	g_Parts[0].tblNo = 0;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0].tblMax = sizeof(move_tbl_idle_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[0].load = 1;
	LoadModel(MODEL_PLAYER_HEAD, &g_Parts[0].model);


	// ��
	g_Parts[1].use = TRUE;
	g_Parts[1].parent = &g_Player;	// �e���Z�b�g
	g_Parts[1].tblNo = 1;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[1].tblMax = sizeof(move_tbl_idle_hip) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[1].load = 1;
	LoadModel(MODEL_PLAYER_HIP, &g_Parts[1].model);


	// ���r
	g_Parts[2].use = TRUE;
	g_Parts[2].parent = &g_Player; // �e���Z�b�g
	g_Parts[2].tblNo = 2;			 // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[2].tblMax = sizeof(move_tbl_idle_lshoulder) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[2].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER_L, &g_Parts[2].model);

	g_Parts[3].use = TRUE;
	g_Parts[3].parent = &g_Parts[2];  // �e���Z�b�g
	g_Parts[3].tblNo = 3;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[3].tblMax = sizeof(move_tbl_idle_larm01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[3].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_ARM_L, &g_Parts[3].model);

	g_Parts[4].use = TRUE;
	g_Parts[4].parent = &g_Parts[3];  // �e���Z�b�g
	g_Parts[4].tblNo = 4;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[4].tblMax = sizeof(move_tbl_idle_lelbow) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[4].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW_L, &g_Parts[4].model);

	g_Parts[5].use = TRUE;
	g_Parts[5].parent = &g_Parts[4];  // �e���Z�b�g
	g_Parts[5].tblNo = 5;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[5].tblMax = sizeof(move_tbl_idle_larm02) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[5].load = 1;
	LoadModel(MODEL_PLAYER_FORE_ARM_L, &g_Parts[5].model);


	// �E�r
	g_Parts[6].use = TRUE;
	g_Parts[6].parent = &g_Player;	// �e���Z�b�g
	g_Parts[6].tblNo = 6;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[6].tblMax = sizeof(move_tbl_idle_rshoulder) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[6].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER_R, &g_Parts[6].model);

	g_Parts[7].use = TRUE;
	g_Parts[7].parent = &g_Parts[6];  // �e���Z�b�g
	g_Parts[7].tblNo = 7;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[7].tblMax = sizeof(move_tbl_idle_rarm01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[7].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_ARM_R, &g_Parts[7].model);

	g_Parts[8].use = TRUE;
	g_Parts[8].parent = &g_Parts[7];  // �e���Z�b�g
	g_Parts[8].tblNo = 8;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[8].tblMax = sizeof(move_tbl_idle_relbow) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[8].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW_R, &g_Parts[8].model);

	g_Parts[9].use = TRUE;
	g_Parts[9].parent = &g_Parts[8];  // �e���Z�b�g
	g_Parts[9].tblNo = 9;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[9].tblMax = sizeof(move_tbl_idle_rarm02) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[9].load = 1;
	LoadModel(MODEL_PLAYER_FORE_ARM_R, &g_Parts[9].model);


	// ����
	g_Parts[10].use = TRUE;
	g_Parts[10].parent = &g_Parts[1];  // �e���Z�b�g
	g_Parts[10].tblNo = 10;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[10].tblMax = sizeof(move_tbl_idle_lleg01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[10].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_LEG_L, &g_Parts[10].model);

	g_Parts[11].use = TRUE;
	g_Parts[11].parent = &g_Parts[10];  // �e���Z�b�g
	g_Parts[11].tblNo = 11;			    // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[11].tblMax = sizeof(move_tbl_idle_lknee) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[11].load = 1;
	LoadModel(MODEL_PLAYER_KNEE_L, &g_Parts[11].model);

	g_Parts[12].use = TRUE;
	g_Parts[12].parent = &g_Parts[11];  // �e���Z�b�g
	g_Parts[12].tblNo = 12;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[12].tblMax = sizeof(move_tbl_idle_lleg02) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[12].load = 1;
	LoadModel(MODEL_PLAYER_LOWER_LEG_L, &g_Parts[12].model);

	g_Parts[13].use = TRUE;
	g_Parts[13].parent = &g_Parts[12];  // �e���Z�b�g
	g_Parts[13].tblNo = 13;			    // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[13].tblMax = sizeof(move_tbl_idle_lfoot) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[13].load = 1;
	LoadModel(MODEL_PLAYER_FOOT_L, &g_Parts[13].model);


	// �E��
	g_Parts[14].use = TRUE;
	g_Parts[14].parent = &g_Parts[1];  // �e���Z�b�g
	g_Parts[14].tblNo = 14;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[14].tblMax = sizeof(move_tbl_idle_rleg01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[14].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_LEG_R, &g_Parts[14].model);

	g_Parts[15].use = TRUE;
	g_Parts[15].parent = &g_Parts[14];  // �e���Z�b�g
	g_Parts[15].tblNo = 15;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[15].tblMax = sizeof(move_tbl_idle_rknee) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[15].load = 1;
	LoadModel(MODEL_PLAYER_KNEE_R, &g_Parts[15].model);

	g_Parts[16].use = TRUE;
	g_Parts[16].parent = &g_Parts[15];  // �e���Z�b�g
	g_Parts[16].tblNo = 16;				// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[16].tblMax = sizeof(move_tbl_idle_rleg02) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[16].load = 1;
	LoadModel(MODEL_PLAYER_LOWER_LEG_R, &g_Parts[16].model);

	g_Parts[17].use = TRUE;
	g_Parts[17].parent = &g_Parts[16];  // �e���Z�b�g
	g_Parts[17].tblNo = 17;			    // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[17].tblMax = sizeof(move_tbl_idle_rfoot) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[17].load = 1;
	LoadModel(MODEL_PLAYER_FOOT_R, &g_Parts[17].model);

	// �c�[��
	g_Parts[18].use = TRUE;
	g_Parts[18].parent = &g_Parts[9];  // �e���Z�b�g
	g_Parts[18].tblNo = 18;			    // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[18].tblMax = sizeof(move_tbl_idle_tool) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[18].load = 1;

	// �c�[���̓ǂݍ���
	{
		g_Load = TRUE;

		LoadModel(MODEL_PLAYER_SWORD, &mSword);
		LoadModel(MODEL_PLAYER_TORCH, &mTorch);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;

		// �p�[�c�̉������
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}

	}

	//�@�c�[���̉������
	if (g_Load)
	{
		g_Load = FALSE;

		UnloadModel(&mSword);

		UnloadModel(&mTorch);
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	//-------------------------------------------------------------------------
	// OBJ���̎擾
	//-------------------------------------------------------------------------
	CAMERA* cam = GetCamera();		// �J����
	LIGHT* light = GetLightData(1); // ���C�g
	BULLET* bullet = GetBullet();   // �o���b�g

	// �ړ��O�̃|�W�V������ۑ�
	XMFLOAT3 oldPos = g_Player.pos;

	// ���͑O�̎p����ێ�
	int oldPose = g_Player.pose;

	// �|�[�Y����{��Ԃɂ���
	g_Player.pose = 0;

	// ���񑬓x
	const float speed = VALUE_ROTATE; // �K�؂ȑ��x��I��

	//-------------------------------------------------------------------------
	// �ړ�����
	//-------------------------------------------------------------------------
	{
		// �L�[�{�[�h�̈ړ�
		if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_W))
		{
			// ����ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI * 3 / 4;

		}
		else if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_W))
		{
			// �E��ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = -XM_PI * 3 / 4;
		}
		else if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_S))
		{
			// �����ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI / 4;
		}
		else if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_S))
		{
			// �E���ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = -XM_PI / 4;
		}
		else if (GetKeyboardPress(DIK_A))
		{
			// ���ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI / 2;
		}
		else if (GetKeyboardPress(DIK_D))
		{
			// �E�ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = -XM_PI / 2;

		}
		else if (GetKeyboardPress(DIK_W))
		{
			// ��ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			// ���ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = 0.0f;
		}

		// �R���g���[���[�̈ړ�
		if (IsButtonPressed(1, BUTTON_UP))
		{
			// ��ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI;

		}
		else if (IsButtonPressed(1, BUTTON_DOWN))
		{
			// ���ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = 0.0f;

		}
		else if (IsButtonPressed(1, BUTTON_LEFT))
		{
			// ���ֈړ�
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI / 2;

		}
		else if (IsButtonPressed(1, BUTTON_RIGHT))
		{
			g_Player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = -XM_PI / 2;

		}

		if (g_Player.spd >= 0.0f)
		{

			// ���݂̕�������ڕW�̕����ւ̊p�x�������߂�
			angleDifference = targetDir - g_Player.dir;

			// �p�x���𐳊m�Ȕ͈͂ɒ���
			while (angleDifference > XM_PI)
			{
				angleDifference -= 2 * XM_PI;
			}

			while (angleDifference < -XM_PI)
			{
				angleDifference += 2 * XM_PI;
			}

			// ��Ԃ��g�p����g_Player.dir��ڕW�̕����ɋ߂Â���
			g_Player.dir = Lerp(g_Player.dir, g_Player.dir + angleDifference, speed);

		}

		// �_�b�V���p
		if (GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(1, BUTTON_Z))
		{
			if (g_Player.spd == VALUE_MOVE)
			{
				g_Player.spd = VALUE_DASH_MOVE;

				// �_�b�V���A�j���[�V����
				g_Player.pose = 5;
			}
		}

		// Key���͂���������ړ���������
		if (g_Player.spd > 0.0f)
		{
			g_Player.rot.y = g_Player.dir + cam->rot.y;

			// ���͂̂����������փv���C���[���������Ĉړ�������
			g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
			g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
		}

		// �e���v���C���[�̈ʒu�ɍ��킹��
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
	// ���s�A�j���[�V�����p
	//-------------------------------------------------------------------------
	{
		//�A�j���[�V�����̏�����
		if (GetKeyboardTrigger(DIK_W) ||
			GetKeyboardTrigger(DIK_A) ||
			GetKeyboardTrigger(DIK_S) ||
			GetKeyboardTrigger(DIK_D) ||
			IsButtonTriggered(1, BUTTON_UP) ||
			IsButtonTriggered(1, BUTTON_DOWN) ||
			IsButtonTriggered(1, BUTTON_LEFT) ||
			IsButtonTriggered(1, BUTTON_RIGHT))
		{
			// �A�j���[�V�����Đ����Ԃ̏�����
			for (int t = 0; t < PLAYER_PARTS_MAX; t++)
			{
				g_Parts[t].time = 0;
			}
		}

		// ���s�A�j���[�V�����؂�ւ�
		if (g_Player.spd == VALUE_MOVE)
		{
			g_Player.pose = 1;
		}
	}

	//-------------------------------------------------------------------------
	// �c�[���؂�ւ�
	//-------------------------------------------------------------------------
	{
		// �A�j���[�V�������Đ����́A�c�[����؂�ւ����Ȃ��悤�ɂ���
		if ((g_Player.anim) && (oldPose != 2))
		{
			// �c�[���؂�ւ�
			if (GetKeyboardTrigger(DIK_1))
			{// ��
				g_Player.tool = 0;
			}

			if (GetKeyboardTrigger(DIK_2))
			{// ����
				g_Player.tool = 1;
			}

			if (GetKeyboardTrigger(DIK_3))
			{// �����i�C�t�p
				g_Player.tool = 2;
			}

			if (GetKeyboardTrigger(DIK_4))
			{// �����i�C�t(��)�p
				g_Player.tool = 3;
			}

			if (GetKeyboardTrigger(DIK_5))
			{// �����i�C�t(��)�p
				g_Player.tool = 4;
			}

			if (GetKeyboardTrigger(DIK_6))
			{// �����i�C�t(�~3)�p
				g_Player.tool = 5;
			}

			if (GetKeyboardTrigger(DIK_7))
			{// �����i�C�t(���~3)�p
				g_Player.tool = 6;
			}

			if (GetKeyboardTrigger(DIK_8))
			{// �����i�C�t(�Ł~3)�p
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
	// �U��
	//-------------------------------------------------------------------------
	if (g_Player.anim)
	{


		//-------------------------------------------------------------------------
		// ���ɂ��U��
		//-------------------------------------------------------------------------
		if (g_Player.tool == 0)
		{
			if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_B))
			{
				// �A�j���[�V�������Đ�����
				g_Player.anim = FALSE;

				// �p���̍Đݒ�
				oldPose = 4;

				g_Player.pose = 4;

				// �A�j���[�V�����Đ����Ԃ̏�����
				for (int s = 0; s < PLAYER_PARTS_MAX; s++)
				{
					g_Parts[s].time = 0;
				}

			}
		}

	}

	//-------------------------------------------------------------------------
	// �����i�C�t�ɂ��U��
	//-------------------------------------------------------------------------
	{
		if ((g_Player.tool == 2) || (g_Player.tool == 3) || (g_Player.tool == 4) || (g_Player.tool == 5) || (g_Player.tool == 6) || (g_Player.tool == 7))
		{
			if (BULLET_TIME < clTime)
			{
				// �\����
				{
					if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_B))
					{
						// sp������Ȃ���΃A�j���[�V������ύX���Ȃ�
						switch (g_Player.tool)
						{
						case 2:
							if (g_Player.sp >= 5.0f)
							{
								clTime = 0;				// �N�[���^�C��
								g_Player.pose = 2;		// �A�j���[�V�����̐؂�ւ�
							}
							break;
						case 3:
						case 4:
							if (g_Player.sp >= 10.0f)
							{
								clTime = 0;				// �N�[���^�C��
								g_Player.pose = 2;		// �A�j���[�V�����̐؂�ւ�
							}
							break;
						case 5:
							if (g_Player.sp >= 15.0f)
							{
								clTime = 0;				// �N�[���^�C��
								g_Player.pose = 2;		// �A�j���[�V�����̐؂�ւ�
							}
							break;
						case 6:
						case 7:
							if (g_Player.sp >= 30.0f)
							{
								clTime = 0;				// �N�[���^�C��
								g_Player.pose = 2;		// �A�j���[�V�����̐؂�ւ�
							}
							break;
						}

						// �A�j���[�V�����Đ����Ԃ̏�����
						for (int m = 0; m < PLAYER_PARTS_MAX; m++)
						{
							g_Parts[m].time = 0;
						}

						XMFLOAT3 pos = g_Player.pos;

						pos.y += 5.8f;

						//�@�v���C���[�̌����Ă���������������Ĉʒu��ݒ肷��
						pos.x -= sinf(g_Player.rot.y - 0.55f) * 9.0f;
						pos.z -= cosf(g_Player.rot.y - 0.55f) * 9.0f;

						XMFLOAT3 rot = g_Player.rot;
						rot.z += XM_PI / 2;
						BOOL shot = FALSE;
						int type = 0;

						// �o���b�g�^�C�v�ݒ�
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

								// ����
								SetBullet(pos, rot, shot, type, 0);

								// ��
								SetBullet(pos, rot, shot, type, 1);

								// �E
								SetBullet(pos, rot, shot, type, 2);

							}

							break;

						case 6:

							if (g_Player.sp >= 30.0f)
							{

								g_Player.sp -= 30.0f;

								type = 1;

								// ����
								SetBullet(pos, rot, shot, type, 0);

								// ��
								SetBullet(pos, rot, shot, type, 1);

								// �E
								SetBullet(pos, rot, shot, type, 2);

							}

							break;

						case 7:

							if (g_Player.sp >= 30.0f)
							{

								g_Player.sp -= 30.0f;

								type = 2;

								// ����
								SetBullet(pos, rot, shot, type, 0);

								// ��
								SetBullet(pos, rot, shot, type, 1);

								// �E
								SetBullet(pos, rot, shot, type, 2);

							}

							break;

						}

					}

					// ���˂������̃A�j���[�V����
					if (GetKeyboardRelease(DIK_SPACE) || IsButtonReleased(1, BUTTON_B))
					{
						// �A�j���[�V�������Đ�����
						g_Player.anim = FALSE;

						// �p���̍Đݒ�
						oldPose = 3;

						g_Player.pose = 3;

					}

				}
			}
		}

		// �N�[���^�C���̎��Ԍo��
		clTime++;

		// �U���p�̃|�C���g�̉�
		if (g_Player.sp <= PLAYER_SP - 0.1f)
		{
			g_Player.sp += 0.1f;
		}

		// �\�����ǂ���
		for (int o = 0; o < BULLET_MAX; o++)
		{
			if ((bullet[o].shot == FALSE) && (bullet[o].use == TRUE))
			{
				g_Player.pose = 2;
			}
		}

	}


	//-------------------------------------------------------------------------
	// �K�w�A�j���[�V����
	//-------------------------------------------------------------------------
	{
		// �A�j���[�V�����Đ���
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

		// �A�j���[�V�����ؑ֗p
		for (int j = 0; j < PLAYER_PARTS_MAX; j++)
		{

			switch (g_Player.pose)
			{
			case 0: // ����



					// �A�j���[�V������؂�ւ���
				g_Parts[j].tblNo = j;

				// �p����ύX����
				g_Player.rot.x = 0.0f;

				// �ő�e�[�u����
				g_Parts[j].tblMax = sizeof(move_tbl_idle_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g



				break;

			case 1: // ���s

					// �A�j���[�V������؂�ւ���
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX;

				// �p����ύX����
				g_Player.rot.x = 0.1f;

				// �ő�e�[�u����
				g_Parts[j].tblMax = sizeof(move_tbl_walk_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


				break;

			case 2: // �\��

					// �A�j���[�V������؂�ւ���
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX * 2;

				// �p����ύX����
				g_Player.rot.x = 0.0f;

				// �ő�e�[�u����
				g_Parts[j].tblMax = sizeof(move_tbl_stance_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

				break;

			case 3: // ����

					// �A�j���[�V������؂�ւ���
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX * 3;

				// �p����ύX����
				g_Player.rot.x = 0.0f;

				// �ő�e�[�u����
				g_Parts[j].tblMax = sizeof(move_tbl_throw_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

				break;

			case 4: // �U��


					// �A�j���[�V������؂�ւ���
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX * 4;

				// �p����ύX����
				g_Player.rot.x = 0.0f;

				// �ő�e�[�u����
				g_Parts[j].tblMax = sizeof(move_tbl_attak_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


				break;

			case 5: // ����

					// �A�j���[�V������؂�ւ���
				g_Parts[j].tblNo = j + PLAYER_PARTS_MAX * 5;

				// �p����ύX����
				g_Player.rot.x = 0.2f;

				// �ő�e�[�u����
				g_Parts[j].tblMax = sizeof(move_tbl_run_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


			}

		}

		// �����Ă�������s�A�j���[�V�������Đ�����
		if (g_Player.spd >= VALUE_MOVE)
		{
			// ���̃p�[�c�̂݃A�j���[�V������ς���
			for (int p = 10; p < 18; ++p)
			{
				// ���̃p�[�c
				g_Parts[p].tblNo = p + PLAYER_PARTS_MAX;

				// �ő�e�[�u����(g_Parts[10]�)
				g_Parts[p].tblMax = sizeof(move_tbl_walk_lleg01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
			}
		}

		// �A�j���[�V�����p
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			// �g���Ă���Ȃ珈������
			if ((g_Parts[i].use) && (g_Parts[i].tblMax > 0))
			{
				// ���`��Ԃ̏���
				int nowNo = (int)g_Parts[i].time;	// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Parts[i].tblMax;		// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;	// �ړ���e�[�u���̔ԍ������߂Ă���

				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

				float nowTime = g_Parts[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;			// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;			// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;			// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

				// frame���g�Ď��Ԍo�ߏ���������
				g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���

				if ((int)g_Parts[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Parts[i].time -= maxNo;				// 0�ԖڂɃ��Z�b�g�������������������p���ł���
				}

				// �A�j���[�V�������Ō�܂ōĐ�������
				if ((int)g_Parts[i].time >= maxNo - 1)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Player.anim = TRUE;

				}

			}
		}

	}

	//-------------------------------------------------------------------------
	// �J��������
	//-------------------------------------------------------------------------
	// �J�����̒Ǐ]
	{
		// �v���C���[���_
		XMFLOAT3 pos = g_Player.pos;

		pos.x += sinf(cam[0].rot.y - XM_PI / 2) * 12.0f;
		pos.z += cosf(cam[0].rot.y - XM_PI / 2) * 12.0f;

		pos.y = g_Player.pos.y + POS_Y_CAM;

		SetCameraAT(pos);
		SetCamera();

		//// �J�����̊g��k��
		//if ((g_Player.pose == 2) || (g_Player.pose == 3))
		//{// �߂Â�
		//	if (cam[0].len > POS_Z_CAM / 2)
		//	{
		//		cam[0].len -= VALUE_MOVE;
		//	}
		//	cam[0].pos.x = cam[0].at.x - sinf(cam[0].rot.y) * cam[0].len;
		//	cam[0].pos.z = cam[0].at.z - cosf(cam[0].rot.y) * cam[0].len;

		//}
		//else
		//{// �����
		//	if (cam[0].len < POS_Z_CAM)
		//	{
		//		cam[0].len += VALUE_MOVE;
		//	}
		//	cam[0].pos.x = cam[0].at.x - sinf(cam[0].rot.y) * cam[0].len;
		//	cam[0].pos.z = cam[0].at.z - cosf(cam[0].rot.y) * cam[0].len;

		//}

	}

	//-------------------------------------------------------------------------
	// �}�b�v�O����
	//-------------------------------------------------------------------------

	// ���̒��̂�
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
	// �U������
	//-------------------------------------------------------------------------
	// �_���W����(1�K)�̂�
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

					// �v���C���[�̑O���Ɉړ�
					pos.x -= sinf(g_Player.rot.y) * 20.0f;
					pos.z -= cosf(g_Player.rot.y) * 20.0f;

					if (CollisionBC(pos, enemy[j].pos, 15.0f, 15.0f))
					{
						// �G�l�~�[��HP�����炷
						enemy[j].hp -= 16.0f;

						ReleaseShadow(enemy[j].shadowIdx);

						// �U���G�t�F�N�g
						for (int k = 0; k < 100; k++)
						{
							// �G�̌�
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

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.1f, 1.0f, 0.3f, 0.6f), fSize, fSize, nLife, 1);

						}



					}

				}

			}
		}

	}

	//-------------------------------------------------------------------------
	// �����蔻�菈��
	//-------------------------------------------------------------------------

	{	// �ÓI�I�u�W�F�N�g�̓����蔻��

		if (FieldHit(g_Player.pos, oldPos))
		{
			// �|�W�V���������Ƃɖ߂�
			g_Player.pos = oldPos;
		}

	}

	//�t�B�[���h�ɂ���ē����蔻���ς���
	switch (GetField())
	{
	case VILLAGE:
		break;

	case DUNGEON_FIRST_FLOOR:

	// ���e�Ƃ̓����蔻��
	{
		BOMB* bomb = GetBomb();
		for (int k = 0; k < BOMB_MAX; k++)
		{
			if (bomb[k].use == TRUE)
			{
				BOOL ans = CollisionBC(g_Player.pos, bomb[k].pos, 10.0f, 10.0f);

				if (ans == TRUE)
				{
					// �|�W�V���������Ƃɖ߂�
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
	// �G�t�F�N�g����
	//-------------------------------------------------------------------------
	{
		// �����̉�
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

			// ���͂̂����������փv���C���[���������Ĉړ�������
			pos.x -= sinf(g_Player.rot.y - 0.6f) * 9.0f;
			pos.z -= cosf(g_Player.rot.y - 0.6f) * 9.0f;

			// �����Ă���܂��͑����Ă��邩���ǂ���
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

			// �r���{�[�h�̐ݒ�
			SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);


			// ���̖�����
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
		else // �����Ă���̂������ȊO�Ȃ疾���������
		{
			light->Enable = FALSE;
			SetLightData(1, light);
		}


		// �ڂ̐�
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			XMFLOAT3 scale;
			int nLife;
			float fSize;

			// ������
			pos = g_Player.pos;
			move.x = 0.0f;
			move.y = 0.0f;
			move.z = 0.0f;

			nLife = 200;

			fSize = 10.0f;

			// �p���ɂ���Ėڂ̈ʒu�𒲐�����
			switch (g_Player.pose)
			{
			case 0: // �ʏ�p��

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y - 0.3f) * 2.3f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y - 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				break;

			case 1: // �O�X�p��

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y - 0.2f) * 3.0f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y - 0.2f) * 3.0f;

				pos.y += 7.3f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			case 2: // �����p��
			case 3: // �\���p��
			case 4: // �U���p��

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y - 0.3f) * 2.3f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y - 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				// �����Ă�����T�C�Y��傫������
				if (g_Player.spd == VALUE_MOVE)
				{
					scale = { 0.02f, 0.02f, 0.02f };
				}

				break;

			case 5: // ����p��

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y - 0.15f) * 3.7f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y - 0.15f) * 3.7f;

				pos.y += 7.0f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			}

			// ������Ƃ����̏���
			if (g_Player.spd == VALUE_MOVE)
			{

				nLife = 80;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y);
				pos.z += cosf(g_Player.rot.y);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y);
				pos.z += cosf(g_Player.rot.y);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}
			else if (g_Player.spd == VALUE_DASH_MOVE) // �����Ă���Ƃ��̏���
			{

				nLife = 80;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;

				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;
				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

			}
			else // �����Ă��Ȃ���
			{
				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}


			// ���W�̏�����
			pos = g_Player.pos;

			// �p���ɂ���Ėڂ̈ʒu�𒲐�����
			switch (g_Player.pose)
			{
			case 0: // �ʏ�p��

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y + 0.3f) * 2.3f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y + 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				break;

			case 1: // �O�X�p��

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y + 0.2f) * 3.0f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y + 0.2f) * 3.0f;

				pos.y += 7.3f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			case 2: // �\���p��
			case 3: // �����p��
			case 4: // �U���p��

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y + 0.3f) * 2.3f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y + 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				// �����Ă�����T�C�Y��傫������
				if (g_Player.spd == VALUE_MOVE)
				{
					scale = { 0.02f, 0.02f, 0.02f };
				}

				break;

			case 5: // ����p��

				pos.x -= sinf(g_Player.rot.y + g_Parts[0].rot.y + 0.15f) * 3.7f;
				pos.z -= cosf(g_Player.rot.y + g_Parts[0].rot.y + 0.15f) * 3.7f;

				pos.y += 7.0f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			}

			// ������Ƃ��̏���
			if (g_Player.spd == VALUE_MOVE)
			{

				nLife = 80;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y);
				pos.z += cosf(g_Player.rot.y);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y);
				pos.z += cosf(g_Player.rot.y);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}
			else if (g_Player.spd == VALUE_DASH_MOVE) // �����Ă���Ƃ��̏���
			{

				nLife = 80;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;

				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(g_Player.rot.y) / 2;
				pos.z += cosf(g_Player.rot.y) / 2;
				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

			}
			else // �����Ă��Ȃ���
			{
				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}

		}

	}

	// ���͂��Ȃ����ɃX�s�[�h������������
	g_Player.spd *= 0.5f;

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{

	if (g_Player.use == FALSE) return;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �܂��͊�{�ƂȂ�a�n�c�x��`�悷��

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	// ������`�悵���Ƃ��ɂ��悤�����}�g���N�X��ۑ����Ă���
	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	// ���f���`��
	DrawModel(&g_Player.model);


	// �p�[�c�̊K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ��
			// g_Player.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
		if (g_Parts[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		if (i != PLAYER_PARTS_MAX - 1)
		{
			// ���f���`��(��)
			DrawModel(&g_Parts[i].model);
		}
		else
		{	// �����i�̕`��

			switch (g_Player.tool) // ����Ɏ����Ă�����̂�`�悷��
			{
			case 0: // ��
				DrawModel(&mSword);
				break;

			case 1: // ����
				DrawModel(&mTorch);
				break;

			case 2: //�����i�C�t
				break;
			}

		}
	}


	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}
//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// �����v�Z�̊֐�
//=============================================================================
float GetDistance(XMFLOAT3 mpos, XMFLOAT3 ypos)
{
	XMVECTOR temp = XMLoadFloat3(&mpos) - XMLoadFloat3(&ypos);
	XMVECTOR distanceSq = XMVector3LengthSq(temp);
	float distance = 0.0f;
	XMStoreFloat(&distance, distanceSq);

	return sqrtf(distance);  // �����̕�������Ԃ�
}
//=============================================================================
// ����p�̊֐�
//=============================================================================
float Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}