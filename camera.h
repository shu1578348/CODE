//=============================================================================
//
// �J�������� [camera.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	POS_X_CAM		(0.0f)		// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM		(5.0f)		// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM		(38.0f)		// �J�����̏����ʒu(Z���W)

#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						 // �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	 // �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(1.0f)											 // �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(10000.0f)										 // �r���[���ʂ�FarZ�l

#define	CULL_ANGLE		(XMConvertToRadians(45.0f))						 // �r���[���ʂ̎���p
#define	CULL_ASPECT		((float)16 / (float)9)							 // �r���[���ʂ̃A�X�y�N�g��	
#define	CULL_NEAR_Z		(0.1f)											 // �r���[���ʂ�NearZ�l
#define	CULL_FAR_Z		(2000.0f)										 // �r���[���ʂ�FarZ�l

struct CAMERA
{
	XMFLOAT4X4	mtxView;		// �r���[�}�g���b�N�X
	XMFLOAT4X4	mtxInvView;		// �r���[�}�g���b�N�X
	XMFLOAT4X4	mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X

	XMFLOAT3	pos;			// �J�����̎��_(�ʒu)
	XMFLOAT3	at;				// �J�����̒����_
	XMFLOAT3	up;				// �J�����̏�����x�N�g��
	XMFLOAT3	rot;			// �J�����̉�]

	BOOL		state;			// TRUE:����\ / FALSE:����s�\/�C�x���g��

	float		len;			// �J�����̎��_�ƒ����_�̋���
	float		time;			// ����
	int			tblNo;			// �s���f�[�^�̃e�[�u���ԍ�
	int			tblMax;			// ���̃e�[�u���̃f�[�^��

};

enum {
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_NONE,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

CAMERA* GetCamera(void);

void SetViewPort(int type);
int GetViewPortType(void);

void SetCameraAT(XMFLOAT3 pos);
