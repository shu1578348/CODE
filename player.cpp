//=============================================================================
//
// ���f������ [player.cpp]
// Author : �r�R�@�G��
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
static PLAYER		player;						// �v���C���[

static PLAYER		parts[PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p

DX11_MODEL			mSword;	// ��
DX11_MODEL			mTorch;	// ����		

static BOOL load; // �c�[���������p

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

int clTime = 0;	//	�ˌ��Ԋu

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER_BODY, &player.model);
	player.load = TRUE;

	// �t�B�[���h�ɂ���Ă̏����ʒu��ς���
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

	player.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	player.use = TRUE;

	player.anim = TRUE;	// �A�j���[�V�����̐؂�ւ�

	// �v���C���[�p��
	player.pose = 0;	// 0:���� / 1:���s / 2:�\�� / 3:���� / 4:�U�� / 5:����

	// �v���C���[����Ɏ����Ă������
	player.tool = 0;	// 0:�� / 1:���� / 2:�����i�C�t / 3:�����i�C�t(��) / 4:�����i�C�t(��) / 5:�����i�C�t(�~3) / 5:�����i�C�t(���~3) / 5:�����i�C�t(�Ł~3)

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	// �K�w�A�j���[�V�����p�̏���������
	player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	// �p�[�c�̏�����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		parts[i].use = FALSE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// �e�q�֌W
		parts[i].parent = &player;		// �� �����ɐe�̃A�h���X������
	//	parts[�r].parent= &player;		// �r��������e�͖{�́i�v���C���[�j
	//	parts[��].parent= &parts[�r];	// �w���r�̎q���������ꍇ�̗�

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		parts[i].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		parts[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		parts[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		parts[i].load = 0;
	}

	// ��
	parts[0].use = TRUE;
	parts[0].parent = &player;	// �e���Z�b�g
	parts[0].tblNo = 0;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[0].tblMax = sizeof(move_tbl_idle_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[0].load = 1;
	LoadModel(MODEL_PLAYER_HEAD, &parts[0].model);


	// ��
	parts[1].use = TRUE;
	parts[1].parent = &player;	// �e���Z�b�g
	parts[1].tblNo = 1;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[1].tblMax = sizeof(move_tbl_idle_hip) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[1].load = 1;
	LoadModel(MODEL_PLAYER_HIP, &parts[1].model);


	// ���r
	parts[2].use = TRUE;
	parts[2].parent = &player; // �e���Z�b�g
	parts[2].tblNo = 2;			 // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[2].tblMax = sizeof(move_tbl_idle_lshoulder) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[2].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER_L, &parts[2].model);

	parts[3].use = TRUE;
	parts[3].parent = &parts[2];  // �e���Z�b�g
	parts[3].tblNo = 3;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[3].tblMax = sizeof(move_tbl_idle_larm01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[3].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_ARM_L, &parts[3].model);

	parts[4].use = TRUE;
	parts[4].parent = &parts[3];  // �e���Z�b�g
	parts[4].tblNo = 4;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[4].tblMax = sizeof(move_tbl_idle_lelbow) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[4].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW_L, &parts[4].model);

	parts[5].use = TRUE;
	parts[5].parent = &parts[4];  // �e���Z�b�g
	parts[5].tblNo = 5;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[5].tblMax = sizeof(move_tbl_idle_larm02) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[5].load = 1;
	LoadModel(MODEL_PLAYER_FORE_ARM_L, &parts[5].model);


	// �E�r
	parts[6].use = TRUE;
	parts[6].parent = &player;	// �e���Z�b�g
	parts[6].tblNo = 6;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[6].tblMax = sizeof(move_tbl_idle_rshoulder) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[6].load = 1;
	LoadModel(MODEL_PLAYER_SHOULDER_R, &parts[6].model);

	parts[7].use = TRUE;
	parts[7].parent = &parts[6];  // �e���Z�b�g
	parts[7].tblNo = 7;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[7].tblMax = sizeof(move_tbl_idle_rarm01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[7].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_ARM_R, &parts[7].model);

	parts[8].use = TRUE;
	parts[8].parent = &parts[7];  // �e���Z�b�g
	parts[8].tblNo = 8;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[8].tblMax = sizeof(move_tbl_idle_relbow) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[8].load = 1;
	LoadModel(MODEL_PLAYER_ELBOW_R, &parts[8].model);

	parts[9].use = TRUE;
	parts[9].parent = &parts[8];  // �e���Z�b�g
	parts[9].tblNo = 9;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[9].tblMax = sizeof(move_tbl_idle_rarm02) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[9].load = 1;
	LoadModel(MODEL_PLAYER_FORE_ARM_R, &parts[9].model);


	// ����
	parts[10].use = TRUE;
	parts[10].parent = &parts[1];  // �e���Z�b�g
	parts[10].tblNo = 10;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[10].tblMax = sizeof(move_tbl_idle_lleg01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[10].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_LEG_L, &parts[10].model);

	parts[11].use = TRUE;
	parts[11].parent = &parts[10];  // �e���Z�b�g
	parts[11].tblNo = 11;			    // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[11].tblMax = sizeof(move_tbl_idle_lknee) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[11].load = 1;
	LoadModel(MODEL_PLAYER_KNEE_L, &parts[11].model);

	parts[12].use = TRUE;
	parts[12].parent = &parts[11];  // �e���Z�b�g
	parts[12].tblNo = 12;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[12].tblMax = sizeof(move_tbl_idle_lleg02) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[12].load = 1;
	LoadModel(MODEL_PLAYER_LOWER_LEG_L, &parts[12].model);

	parts[13].use = TRUE;
	parts[13].parent = &parts[12];  // �e���Z�b�g
	parts[13].tblNo = 13;			    // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[13].tblMax = sizeof(move_tbl_idle_lfoot) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[13].load = 1;
	LoadModel(MODEL_PLAYER_FOOT_L, &parts[13].model);


	// �E��
	parts[14].use = TRUE;
	parts[14].parent = &parts[1];  // �e���Z�b�g
	parts[14].tblNo = 14;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[14].tblMax = sizeof(move_tbl_idle_rleg01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[14].load = 1;
	LoadModel(MODEL_PLAYER_UPPER_LEG_R, &parts[14].model);

	parts[15].use = TRUE;
	parts[15].parent = &parts[14];  // �e���Z�b�g
	parts[15].tblNo = 15;			  // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[15].tblMax = sizeof(move_tbl_idle_rknee) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[15].load = 1;
	LoadModel(MODEL_PLAYER_KNEE_R, &parts[15].model);

	parts[16].use = TRUE;
	parts[16].parent = &parts[15];  // �e���Z�b�g
	parts[16].tblNo = 16;				// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[16].tblMax = sizeof(move_tbl_idle_rleg02) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[16].load = 1;
	LoadModel(MODEL_PLAYER_LOWER_LEG_R, &parts[16].model);

	parts[17].use = TRUE;
	parts[17].parent = &parts[16];  // �e���Z�b�g
	parts[17].tblNo = 17;			    // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[17].tblMax = sizeof(move_tbl_idle_rfoot) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[17].load = 1;
	LoadModel(MODEL_PLAYER_FOOT_R, &parts[17].model);

	// �c�[��
	parts[18].use = TRUE;
	parts[18].parent = &parts[9];  // �e���Z�b�g
	parts[18].tblNo = 18;			    // �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	parts[18].tblMax = sizeof(move_tbl_idle_tool) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	parts[18].load = 1;

	// �c�[���̓ǂݍ���
	{
		load = TRUE;

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
	if (player.load)
	{
		UnloadModel(&player.model);
		player.load = FALSE;

		// �p�[�c�̉������
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			UnloadModel(&parts[i].model);
			parts[i].load = FALSE;
		}

	}

	//�@�c�[���̉������
	if (load)
	{
		load = FALSE;

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
	XMFLOAT3 oldPos = player.pos;

	// ���͑O�̎p����ێ�
	int oldPose = player.pose;

	// �|�[�Y����{��Ԃɂ���
	player.pose = 0;

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
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI * 3 / 4;

		}
		else if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_W))
		{
			// �E��ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = -XM_PI * 3 / 4;
		}
		else if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_S))
		{
			// �����ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI / 4;
		}
		else if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_S))
		{
			// �E���ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = -XM_PI / 4;
		}
		else if (GetKeyboardPress(DIK_A))
		{
			// ���ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI / 2;
		}
		else if (GetKeyboardPress(DIK_D))
		{
			// �E�ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = -XM_PI / 2;

		}
		else if (GetKeyboardPress(DIK_W))
		{
			// ��ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			// ���ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = 0.0f;
		}

		// �R���g���[���[�̈ړ�
		if (IsButtonPressed(1, BUTTON_UP))
		{
			// ��ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI;

		}
		else if (IsButtonPressed(1, BUTTON_DOWN))
		{
			// ���ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = 0.0f;

		}
		else if (IsButtonPressed(1, BUTTON_LEFT))
		{
			// ���ֈړ�
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = XM_PI / 2;

		}
		else if (IsButtonPressed(1, BUTTON_RIGHT))
		{
			player.spd = VALUE_MOVE;

			// �ڕW�̕���
			targetDir = -XM_PI / 2;

		}

		if (player.spd >= 0.0f)
		{

			// ���݂̕�������ڕW�̕����ւ̊p�x�������߂�
			angleDifference = targetDir - player.dir;

			// �p�x���𐳊m�Ȕ͈͂ɒ���
			while (angleDifference > XM_PI)
			{
				angleDifference -= 2 * XM_PI;
			}

			while (angleDifference < -XM_PI)
			{
				angleDifference += 2 * XM_PI;
			}

			// ��Ԃ��g�p����player.dir��ڕW�̕����ɋ߂Â���
			player.dir = Lerp(player.dir, player.dir + angleDifference, speed);

		}

		// �_�b�V���p
		if (GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(1, BUTTON_Z))
		{
			if (player.spd == VALUE_MOVE)
			{
				player.spd = VALUE_DASH_MOVE;

				// �_�b�V���A�j���[�V����
				player.pose = 5;
			}
		}

		// Key���͂���������ړ���������
		if (player.spd > 0.0f)
		{
			player.rot.y = player.dir + cam->rot.y;

			// ���͂̂����������փv���C���[���������Ĉړ�������
			player.pos.x -= sinf(player.rot.y) * player.spd;
			player.pos.z -= cosf(player.rot.y) * player.spd;
		}

		// �e���v���C���[�̈ʒu�ɍ��킹��
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
				parts[t].time = 0;
			}
		}

		// ���s�A�j���[�V�����؂�ւ�
		if (player.spd == VALUE_MOVE)
		{
			player.pose = 1;
		}
	}

	//-------------------------------------------------------------------------
	// �c�[���؂�ւ�
	//-------------------------------------------------------------------------
	{
		// �A�j���[�V�������Đ����́A�c�[����؂�ւ����Ȃ��悤�ɂ���
		if ((player.anim) && (oldPose != 2))
		{
			// �c�[���؂�ւ�
			if (GetKeyboardTrigger(DIK_1))
			{// ��
				player.tool = 0;
			}

			if (GetKeyboardTrigger(DIK_2))
			{// ����
				player.tool = 1;
			}

			if (GetKeyboardTrigger(DIK_3))
			{// �����i�C�t�p
				player.tool = 2;
			}

			if (GetKeyboardTrigger(DIK_4))
			{// �����i�C�t(��)�p
				player.tool = 3;
			}

			if (GetKeyboardTrigger(DIK_5))
			{// �����i�C�t(��)�p
				player.tool = 4;
			}

			if (GetKeyboardTrigger(DIK_6))
			{// �����i�C�t(�~3)�p
				player.tool = 5;
			}

			if (GetKeyboardTrigger(DIK_7))
			{// �����i�C�t(���~3)�p
				player.tool = 6;
			}

			if (GetKeyboardTrigger(DIK_8))
			{// �����i�C�t(�Ł~3)�p
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
	// �U��
	//-------------------------------------------------------------------------
	if (player.anim)
	{


		//-------------------------------------------------------------------------
		// ���ɂ��U��
		//-------------------------------------------------------------------------
		if (player.tool == 0)
		{
			if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_B))
			{
				// �A�j���[�V�������Đ�����
				player.anim = FALSE;

				// �p���̍Đݒ�
				oldPose = 4;

				player.pose = 4;

				// �A�j���[�V�����Đ����Ԃ̏�����
				for (int s = 0; s < PLAYER_PARTS_MAX; s++)
				{
					parts[s].time = 0;
				}

			}
		}

	}

	//-------------------------------------------------------------------------
	// �����i�C�t�ɂ��U��
	//-------------------------------------------------------------------------
	{
		if ((player.tool == 2) || (player.tool == 3) || (player.tool == 4) || (player.tool == 5) || (player.tool == 6) || (player.tool == 7))
		{
			if (BULLET_TIME < clTime)
			{
				// �\����
				{
					if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_B))
					{
						// sp������Ȃ���΃A�j���[�V������ύX���Ȃ�
						switch (player.tool)
						{
						case 2:
							if (player.sp >= 5.0f)
							{
								clTime = 0;				// �N�[���^�C��
								player.pose = 2;		// �A�j���[�V�����̐؂�ւ�
							}
							break;
						case 3:
						case 4:
							if (player.sp >= 10.0f)
							{
								clTime = 0;				// �N�[���^�C��
								player.pose = 2;		// �A�j���[�V�����̐؂�ւ�
							}
							break;
						case 5:
							if (player.sp >= 15.0f)
							{
								clTime = 0;				// �N�[���^�C��
								player.pose = 2;		// �A�j���[�V�����̐؂�ւ�
							}
							break;
						case 6:
						case 7:
							if (player.sp >= 30.0f)
							{
								clTime = 0;				// �N�[���^�C��
								player.pose = 2;		// �A�j���[�V�����̐؂�ւ�
							}
							break;
						}

						// �A�j���[�V�����Đ����Ԃ̏�����
						for (int m = 0; m < PLAYER_PARTS_MAX; m++)
						{
							parts[m].time = 0;
						}

						XMFLOAT3 pos = player.pos;

						pos.y += 5.8f;

						//�@�v���C���[�̌����Ă���������������Ĉʒu��ݒ肷��
						pos.x -= sinf(player.rot.y - 0.55f) * 9.0f;
						pos.z -= cosf(player.rot.y - 0.55f) * 9.0f;

						XMFLOAT3 rot = player.rot;
						rot.z += XM_PI / 2;
						BOOL shot = FALSE;
						int type = 0;

						// �o���b�g�^�C�v�ݒ�
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

								// ����
								SetBullet(pos, rot, shot, type, 0);

								// ��
								SetBullet(pos, rot, shot, type, 1);

								// �E
								SetBullet(pos, rot, shot, type, 2);

							}

							break;

						case 6:

							if (player.sp >= 30.0f)
							{

								player.sp -= 30.0f;

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

							if (player.sp >= 30.0f)
							{

								player.sp -= 30.0f;

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
						player.anim = FALSE;

						// �p���̍Đݒ�
						oldPose = 3;

						player.pose = 3;

					}

				}
			}
		}

		// �N�[���^�C���̎��Ԍo��
		clTime++;

		// �U���p�̃|�C���g�̉�
		if (player.sp <= PLAYER_SP - 0.1f)
		{
			player.sp += 0.1f;
		}

		// �\�����ǂ���
		for (int o = 0; o < BULLET_MAX; o++)
		{
			if ((bullet[o].shot == FALSE) && (bullet[o].use == TRUE))
			{
				player.pose = 2;
			}
		}

	}


	//-------------------------------------------------------------------------
	// �K�w�A�j���[�V����
	//-------------------------------------------------------------------------
	{
		// �A�j���[�V�����Đ���
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

		// �A�j���[�V�����ؑ֗p
		for (int j = 0; j < PLAYER_PARTS_MAX; j++)
		{

			switch (player.pose)
			{
			case 0: // ����



					// �A�j���[�V������؂�ւ���
				parts[j].tblNo = j;

				// �p����ύX����
				player.rot.x = 0.0f;

				// �ő�e�[�u����
				parts[j].tblMax = sizeof(move_tbl_idle_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g



				break;

			case 1: // ���s

					// �A�j���[�V������؂�ւ���
				parts[j].tblNo = j + PLAYER_PARTS_MAX;

				// �p����ύX����
				player.rot.x = 0.1f;

				// �ő�e�[�u����
				parts[j].tblMax = sizeof(move_tbl_walk_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


				break;

			case 2: // �\��

					// �A�j���[�V������؂�ւ���
				parts[j].tblNo = j + PLAYER_PARTS_MAX * 2;

				// �p����ύX����
				player.rot.x = 0.0f;

				// �ő�e�[�u����
				parts[j].tblMax = sizeof(move_tbl_stance_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

				break;

			case 3: // ����

					// �A�j���[�V������؂�ւ���
				parts[j].tblNo = j + PLAYER_PARTS_MAX * 3;

				// �p����ύX����
				player.rot.x = 0.0f;

				// �ő�e�[�u����
				parts[j].tblMax = sizeof(move_tbl_throw_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

				break;

			case 4: // �U��


					// �A�j���[�V������؂�ւ���
				parts[j].tblNo = j + PLAYER_PARTS_MAX * 4;

				// �p����ύX����
				player.rot.x = 0.0f;

				// �ő�e�[�u����
				parts[j].tblMax = sizeof(move_tbl_attak_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


				break;

			case 5: // ����

					// �A�j���[�V������؂�ւ���
				parts[j].tblNo = j + PLAYER_PARTS_MAX * 5;

				// �p����ύX����
				player.rot.x = 0.2f;

				// �ő�e�[�u����
				parts[j].tblMax = sizeof(move_tbl_run_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


			}

		}

		// �����Ă�������s�A�j���[�V�������Đ�����
		if (player.spd >= VALUE_MOVE)
		{
			// ���̃p�[�c�̂݃A�j���[�V������ς���
			for (int p = 10; p < 18; ++p)
			{
				// ���̃p�[�c
				parts[p].tblNo = p + PLAYER_PARTS_MAX;

				// �ő�e�[�u����(parts[10]�)
				parts[p].tblMax = sizeof(move_tbl_walk_lleg01) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
			}
		}

		// �A�j���[�V�����p
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			// �g���Ă���Ȃ珈������
			if ((parts[i].use) && (parts[i].tblMax > 0))
			{
				// ���`��Ԃ̏���
				int nowNo = (int)parts[i].time;	// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = parts[i].tblMax;		// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;	// �ړ���e�[�u���̔ԍ������߂Ă���

				INTERPOLATION_DATA* tbl = moveTblAdr[parts[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

				float nowTime = parts[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;			// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;			// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;			// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&parts[i].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&parts[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&parts[i].scl, nowScl + Scl);

				// frame���g�Ď��Ԍo�ߏ���������
				parts[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���

				if ((int)parts[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					parts[i].time -= maxNo;				// 0�ԖڂɃ��Z�b�g�������������������p���ł���
				}

				// �A�j���[�V�������Ō�܂ōĐ�������
				if ((int)parts[i].time >= maxNo - 1)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					player.anim = TRUE;

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
		XMFLOAT3 pos = player.pos;

		pos.x += sinf(cam[0].rot.y - XM_PI / 2) * 12.0f;
		pos.z += cosf(cam[0].rot.y - XM_PI / 2) * 12.0f;

		pos.y = player.pos.y + POS_Y_CAM;

		SetCameraAT(pos);
		SetCamera();

		//// �J�����̊g��k��
		//if ((player.pose == 2) || (player.pose == 3))
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
		float distance = GetDistance(player.pos, { 0.0f, 0.0f, 0.0f });

		if (distance > 1000)
		{
			player.pos.x = -player.pos.x;

			player.pos.z = -player.pos.z;
		}

	}


	//-------------------------------------------------------------------------
	// �U������
	//-------------------------------------------------------------------------
	// �_���W����(1�K)�̂�
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

					// �v���C���[�̑O���Ɉړ�
					pos.x -= sinf(player.rot.y) * 20.0f;
					pos.z -= cosf(player.rot.y) * 20.0f;

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

		if (FieldHit(player.pos, oldPos))
		{
			// �|�W�V���������Ƃɖ߂�
			player.pos = oldPos;
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
				BOOL ans = CollisionBC(player.pos, bomb[k].pos, 10.0f, 10.0f);

				if (ans == TRUE)
				{
					// �|�W�V���������Ƃɖ߂�
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
	// �G�t�F�N�g����
	//-------------------------------------------------------------------------
	{
		// �����̉�
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

			// ���͂̂����������փv���C���[���������Ĉړ�������
			pos.x -= sinf(player.rot.y - 0.6f) * 9.0f;
			pos.z -= cosf(player.rot.y - 0.6f) * 9.0f;

			// �����Ă���܂��͑����Ă��邩���ǂ���
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
			pos = player.pos;
			move.x = 0.0f;
			move.y = 0.0f;
			move.z = 0.0f;

			nLife = 200;

			fSize = 10.0f;

			// �p���ɂ���Ėڂ̈ʒu�𒲐�����
			switch (player.pose)
			{
			case 0: // �ʏ�p��

				pos.x -= sinf(player.rot.y + parts[0].rot.y - 0.3f) * 2.3f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y - 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				break;

			case 1: // �O�X�p��

				pos.x -= sinf(player.rot.y + parts[0].rot.y - 0.2f) * 3.0f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y - 0.2f) * 3.0f;

				pos.y += 7.3f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			case 2: // �����p��
			case 3: // �\���p��
			case 4: // �U���p��

				pos.x -= sinf(player.rot.y + parts[0].rot.y - 0.3f) * 2.3f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y - 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				// �����Ă�����T�C�Y��傫������
				if (player.spd == VALUE_MOVE)
				{
					scale = { 0.02f, 0.02f, 0.02f };
				}

				break;

			case 5: // ����p��

				pos.x -= sinf(player.rot.y + parts[0].rot.y - 0.15f) * 3.7f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y - 0.15f) * 3.7f;

				pos.y += 7.0f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			}

			// ������Ƃ����̏���
			if (player.spd == VALUE_MOVE)
			{

				nLife = 80;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y);
				pos.z += cosf(player.rot.y);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y);
				pos.z += cosf(player.rot.y);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}
			else if (player.spd == VALUE_DASH_MOVE) // �����Ă���Ƃ��̏���
			{

				nLife = 80;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;

				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;
				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

			}
			else // �����Ă��Ȃ���
			{
				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}


			// ���W�̏�����
			pos = player.pos;

			// �p���ɂ���Ėڂ̈ʒu�𒲐�����
			switch (player.pose)
			{
			case 0: // �ʏ�p��

				pos.x -= sinf(player.rot.y + parts[0].rot.y + 0.3f) * 2.3f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y + 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				break;

			case 1: // �O�X�p��

				pos.x -= sinf(player.rot.y + parts[0].rot.y + 0.2f) * 3.0f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y + 0.2f) * 3.0f;

				pos.y += 7.3f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			case 2: // �\���p��
			case 3: // �����p��
			case 4: // �U���p��

				pos.x -= sinf(player.rot.y + parts[0].rot.y + 0.3f) * 2.3f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y + 0.3f) * 2.3f;

				pos.y += 7.4f;

				scale = { 0.007f, 0.007f, 0.007f };

				// �����Ă�����T�C�Y��傫������
				if (player.spd == VALUE_MOVE)
				{
					scale = { 0.02f, 0.02f, 0.02f };
				}

				break;

			case 5: // ����p��

				pos.x -= sinf(player.rot.y + parts[0].rot.y + 0.15f) * 3.7f;
				pos.z -= cosf(player.rot.y + parts[0].rot.y + 0.15f) * 3.7f;

				pos.y += 7.0f;

				scale = { 0.02f, 0.02f, 0.02f };

				break;

			}

			// ������Ƃ��̏���
			if (player.spd == VALUE_MOVE)
			{

				nLife = 80;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y);
				pos.z += cosf(player.rot.y);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y);
				pos.z += cosf(player.rot.y);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
			}
			else if (player.spd == VALUE_DASH_MOVE) // �����Ă���Ƃ��̏���
			{

				nLife = 80;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;

				SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

				pos.x += sinf(player.rot.y) / 2;
				pos.z += cosf(player.rot.y) / 2;
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
	player.spd *= 0.5f;

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{

	if (player.use == FALSE) return;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �܂��͊�{�ƂȂ�a�n�c�x��`�悷��

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(player.scl.x, player.scl.y, player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(player.rot.x, player.rot.y + XM_PI, player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(player.pos.x, player.pos.y, player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	// ������`�悵���Ƃ��ɂ��悤�����}�g���N�X��ۑ����Ă���
	XMStoreFloat4x4(&player.mtxWorld, mtxWorld);

	// ���f���`��
	DrawModel(&player.model);


	// �p�[�c�̊K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(parts[i].scl.x, parts[i].scl.y, parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(parts[i].rot.x, parts[i].rot.y, parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(parts[i].pos.x, parts[i].pos.y, parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&parts[i].parent->mtxWorld));
			// ��
			// player.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
		if (parts[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		if (i != PLAYER_PARTS_MAX - 1)
		{
			// ���f���`��(��)
			DrawModel(&parts[i].model);
		}
		else
		{	// �����i�̕`��

			switch (player.tool) // ����Ɏ����Ă�����̂�`�悷��
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
	return &player;
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