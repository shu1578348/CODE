//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �r�R �G��
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE_CAMERA	(2.0f)					// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)			// �J�����̉�]��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			camera;		// �J�����f�[�^

static int				viewPortType = TYPE_FULL_SCREEN;

//-------------------------------------------------------------------------
// ���`��Ԃ̃f�[�^
//-------------------------------------------------------------------------
static INTERPOLATION_DATA moveTbl0[] = {
	//���W							    ��]��						�g�嗦						����
	{ XMFLOAT3( 500.0f, 2400,  120.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3( 200.0f, 2300, -100.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(   0.0f, 2200, -300.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-200.0f, 2050, -500.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-350.0f, 1900, -350.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-500.0f, 1750, -150.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-650.0f, 1600,  100.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
	{ XMFLOAT3(-500.0f, 1400,  200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
	{ XMFLOAT3(-400.0f, 1200,  300.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-300.0f, 1000,  400.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 40 },
	{ XMFLOAT3(-200.0f,  750,  200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 40 },
	{ XMFLOAT3(-100.0f,  500,  120.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-200.0f,  250,  -40.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-220.0f,   10,  -80.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA* moveTblAdr[] =
{
	moveTbl0,
};

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	camera.pos = { POS_X_CAM, POS_Y_CAM, POS_Z_CAM };
	camera.at  = { 0.0f, 0.0f, 0.0f };
	camera.up  = { 0.0f, 1.0f, 0.0f };
	camera.rot = { 0.0f, 0.0f, 0.0f };

	if (GetField() == VILLAGE)
	{
		// �J�������ړ��\��
		camera.state = TRUE;
	}
	else
	{
		// �J�������ړ��\��
		camera.state = TRUE;
	}

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = camera.pos.x - camera.at.x;
	vz = camera.pos.z - camera.at.z;
	camera.len = sqrtf(vx * vx + vz * vz);

	// ���`��Ԃɂ��J�����ړ�
	{
		camera.time  = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		camera.tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		camera.tblMax = sizeof(moveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	}

	// �r���[�|�[�g�^�C�v�̏�����
	SetViewPort(viewPortType);
}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{

#ifdef _DEBUG
	//-------------------------------------------------------------------------
	// �f�o�b�O�p�J�����ړ�
	//-------------------------------------------------------------------------
	if (camera.state)
	{
		if (GetKeyboardPress(DIK_Z))
		{// ���_����u���v
			camera.rot.y += VALUE_ROTATE_CAMERA;
			if (camera.rot.y > XM_PI)
			{
				camera.rot.y -= XM_PI * 2.0f;
			}

			camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
			camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;
		}

		if (GetKeyboardPress(DIK_C))
		{// ���_����u�E�v
			camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (camera.rot.y < -XM_PI)
			{
				camera.rot.y += XM_PI * 2.0f;
			}

			camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
			camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;
		}

		if (GetKeyboardPress(DIK_Y))
		{
			camera.pos.y += VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_N))
		{// ���_�ړ��u���v
			camera.pos.y -= VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_T))
		{// �����_�ړ��u��v
			camera.at.y += VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_B))
		{// �����_�ړ��u���v
			camera.at.y -= VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_U))
		{// �߂Â�
			camera.len -= VALUE_MOVE_CAMERA;
			camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
			camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;
		}

		if (GetKeyboardPress(DIK_M))
		{// �����
			camera.len += VALUE_MOVE_CAMERA;
			camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
			camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;
		}

		
	}

	// �J�����������ɖ߂�
	if (GetKeyboardPress(DIK_R))
	{
		UninitCamera();
		InitCamera();
	}

#endif
	//-------------------------------------------------------------------------
	// �}�E�X�ɂ��J�����ړ�
	//-------------------------------------------------------------------------
	if (camera.state)
	{		
		// ���_�̍��E�ړ�
		camera.rot.y += VALUE_ROTATE_CAMERA * (GetMouseX() / 10);
		if (camera.rot.y > XM_PI)
		{
			camera.rot.y -= XM_PI * 2.0f;
		}

		// ���E�ړ�
		camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
		camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;


		// ���_�̏㉺�ړ�
		camera.rot.x += VALUE_ROTATE_CAMERA * -(GetMouseY() / 10);

		if (camera.rot.x > XM_PI / 2.0f)
		{
			camera.rot.x = XM_PI / 1.999f;
		}
		else if (camera.rot.x < -XM_PI / 2.0f)
		{
			camera.rot.x = -XM_PI / 1.999f;
		}

		// �㉺�ړ�	
		camera.pos.y = camera.at.y + sinf(camera.rot.x) * camera.len; 

		// �L�[�{�[�h�p
		if (GetKeyboardPress(DIK_Q))
		{// �����_����u���v
			camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (camera.rot.y < -XM_PI)
			{
				camera.rot.y += XM_PI * 2.0f;
			}

			camera.at.x = camera.pos.x + sinf(camera.rot.y) * camera.len;
			camera.at.z = camera.pos.z + cosf(camera.rot.y) * camera.len;
		}

		if (GetKeyboardPress(DIK_E))
		{// �����_����u�E�v
			camera.rot.y += VALUE_ROTATE_CAMERA;
			if (camera.rot.y > XM_PI)
			{
				camera.rot.y -= XM_PI * 2.0f;
			}

			camera.at.x = camera.pos.x + sinf(camera.rot.y) * camera.len;
			camera.at.z = camera.pos.z + cosf(camera.rot.y) * camera.len;
		}

		// �L�[�{�[�h�p
		if (IsButtonPressed(1,BUTTON_L))
		{// �����_����u���v
			camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (camera.rot.y < -XM_PI)
			{
				camera.rot.y += XM_PI * 2.0f;
			}

			camera.at.x = camera.pos.x + sinf(camera.rot.y) * camera.len;
			camera.at.z = camera.pos.z + cosf(camera.rot.y) * camera.len;
		}

		if (IsButtonPressed(1,BUTTON_R))
		{// �����_����u�E�v
			camera.rot.y += VALUE_ROTATE_CAMERA;
			if (camera.rot.y > XM_PI)
			{
				camera.rot.y -= XM_PI * 2.0f;
			}

			camera.at.x = camera.pos.x + sinf(camera.rot.y) * camera.len;
			camera.at.z = camera.pos.z + cosf(camera.rot.y) * camera.len;
		}

	}

	//-------------------------------------------------------------------------
	// ���`��Ԃɂ��J�����ړ�
	//-------------------------------------------------------------------------
	if (!camera.state)
	{
		int nowNo = (int)camera.time;							// �������ł���e�[�u���ԍ������o���Ă���
		int maxNo = camera.tblMax;							// �o�^�e�[�u�����𐔂��Ă���
		int nextNo = (nowNo + 1) % maxNo;						// �ړ���e�[�u���̔ԍ������߂Ă���
		INTERPOLATION_DATA* tbl = moveTblAdr[camera.tblNo];	// �s���e�[�u���̃A�h���X���擾

		XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
		XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�

		XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
		XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���

		float nowTime = camera.time - nowNo;		// ���ԕ����ł��鏭�������o���Ă���

		Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
		Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���

		// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
		XMStoreFloat3(&camera.pos, nowPos + Pos);

		// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
		XMStoreFloat3(&camera.rot, nowRot + Rot);

		// frame���g�Ď��Ԍo�ߏ���������
		camera.time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
		if ((int)camera.time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
		{
			camera.time -= maxNo;					// �O�ԖڂɃ��Z�b�g�������������������p���ł���
		}

		if ((int)camera.time >= maxNo - 1)
		{
			camera.state = TRUE;
		}

	}

}

//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void)
{
	// �r���[�}�g���b�N�X�ݒ�
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&camera.pos), XMLoadFloat3(&camera.at), XMLoadFloat3(&camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&camera.mtxInvView, mtxInvView);

	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&camera.mtxProjection, mtxProjection);

	SetShaderCamera(camera.pos);

}

//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA* GetCamera(void)
{
	return &camera;
}

//=============================================================================
// �r���[�|�[�g�̐ݒ�
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext* g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	viewPortType = type;

	//-------------------------------------------------------------------------
	// �r���[�|�[�g�ݒ�
	//-------------------------------------------------------------------------
	switch (viewPortType)
	{
	case TYPE_FULL_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_LEFT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_RIGHT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_UP_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_DOWN_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;


	}
	g_ImmediateContext->RSSetViewports(1, &vp);

}

int GetViewPortType(void)
{
	return viewPortType;
}

// �J�����̎��_�ƒ����_���Z�b�g
void SetCameraAT(XMFLOAT3 pos)
{
	// �J�����̒����_�������̍��W�ɂ��Ă݂�
	camera.at = pos;

	if (camera.state)
	{
		// �J�����̎��_���J������Y����]�ɑΉ������Ă���
		camera.pos.x = camera.at.x - sinf(camera.rot.y) * cosf(camera.rot.x) * camera.len;
		camera.pos.z = camera.at.z - cosf(camera.rot.y) * cosf(camera.rot.x) * camera.len;

		camera.pos.y = camera.at.y - sinf(camera.rot.x) * camera.len;
	}

}