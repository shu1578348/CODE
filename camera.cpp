//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
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
static CAMERA			g_Camera;		// �J�����f�[�^

static int				g_ViewPortType = TYPE_FULL_SCREEN;

//-------------------------------------------------------------------------
// ���`��Ԃ̃f�[�^
//-------------------------------------------------------------------------
static INTERPOLATION_DATA g_MoveTbl0[] = {
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

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
};

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = { POS_X_CAM, POS_Y_CAM, POS_Z_CAM };
	g_Camera.at  = { 0.0f, 0.0f, 0.0f };
	g_Camera.up  = { 0.0f, 1.0f, 0.0f };
	g_Camera.rot = { 0.0f, 0.0f, 0.0f };

	if (GetField() == VILLAGE)
	{
		// �J�������ړ��\��
		g_Camera.state = TRUE;
	}
	else
	{
		// �J�������ړ��\��
		g_Camera.state = TRUE;
	}

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);

	// ���`��Ԃɂ��J�����ړ�
	{
		g_Camera.time  = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		g_Camera.tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Camera.tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	}

	// �r���[�|�[�g�^�C�v�̏�����
	SetViewPort(g_ViewPortType);
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
	if (g_Camera.state)
	{
		if (GetKeyboardPress(DIK_Z))
		{// ���_����u���v
			g_Camera.rot.y += VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y > XM_PI)
			{
				g_Camera.rot.y -= XM_PI * 2.0f;
			}

			g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		}

		if (GetKeyboardPress(DIK_C))
		{// ���_����u�E�v
			g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y < -XM_PI)
			{
				g_Camera.rot.y += XM_PI * 2.0f;
			}

			g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		}

		if (GetKeyboardPress(DIK_Y))
		{
			g_Camera.pos.y += VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_N))
		{// ���_�ړ��u���v
			g_Camera.pos.y -= VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_T))
		{// �����_�ړ��u��v
			g_Camera.at.y += VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_B))
		{// �����_�ړ��u���v
			g_Camera.at.y -= VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_U))
		{// �߂Â�
			g_Camera.len -= VALUE_MOVE_CAMERA;
			g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		}

		if (GetKeyboardPress(DIK_M))
		{// �����
			g_Camera.len += VALUE_MOVE_CAMERA;
			g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
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
	if (g_Camera.state)
	{		
		// ���_�̍��E�ړ�
		g_Camera.rot.y += VALUE_ROTATE_CAMERA * (GetMouseX() / 10);
		if (g_Camera.rot.y > XM_PI)
		{
			g_Camera.rot.y -= XM_PI * 2.0f;
		}

		// ���E�ړ�
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;


		// ���_�̏㉺�ړ�
		g_Camera.rot.x += VALUE_ROTATE_CAMERA * -(GetMouseY() / 10);

		if (g_Camera.rot.x > XM_PI / 2.0f)
		{
			g_Camera.rot.x = XM_PI / 1.999f;
		}
		else if (g_Camera.rot.x < -XM_PI / 2.0f)
		{
			g_Camera.rot.x = -XM_PI / 1.999f;
		}

		// �㉺�ړ�	
		g_Camera.pos.y = g_Camera.at.y + sinf(g_Camera.rot.x) * g_Camera.len; 

		// �L�[�{�[�h�p
		if (GetKeyboardPress(DIK_Q))
		{// �����_����u���v
			g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y < -XM_PI)
			{
				g_Camera.rot.y += XM_PI * 2.0f;
			}

			g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
		}

		if (GetKeyboardPress(DIK_E))
		{// �����_����u�E�v
			g_Camera.rot.y += VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y > XM_PI)
			{
				g_Camera.rot.y -= XM_PI * 2.0f;
			}

			g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
		}

		// �L�[�{�[�h�p
		if (IsButtonPressed(1,BUTTON_L))
		{// �����_����u���v
			g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y < -XM_PI)
			{
				g_Camera.rot.y += XM_PI * 2.0f;
			}

			g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
		}

		if (IsButtonPressed(1,BUTTON_R))
		{// �����_����u�E�v
			g_Camera.rot.y += VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y > XM_PI)
			{
				g_Camera.rot.y -= XM_PI * 2.0f;
			}

			g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
		}

	}

	//-------------------------------------------------------------------------
	// ���`��Ԃɂ��J�����ړ�
	//-------------------------------------------------------------------------
	if (!g_Camera.state)
	{
		int nowNo = (int)g_Camera.time;							// �������ł���e�[�u���ԍ������o���Ă���
		int maxNo = g_Camera.tblMax;							// �o�^�e�[�u�����𐔂��Ă���
		int nextNo = (nowNo + 1) % maxNo;						// �ړ���e�[�u���̔ԍ������߂Ă���
		INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Camera.tblNo];	// �s���e�[�u���̃A�h���X���擾

		XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
		XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�

		XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
		XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���

		float nowTime = g_Camera.time - nowNo;		// ���ԕ����ł��鏭�������o���Ă���

		Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
		Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���

		// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
		XMStoreFloat3(&g_Camera.pos, nowPos + Pos);

		// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
		XMStoreFloat3(&g_Camera.rot, nowRot + Rot);

		// frame���g�Ď��Ԍo�ߏ���������
		g_Camera.time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
		if ((int)g_Camera.time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
		{
			g_Camera.time -= maxNo;					// �O�ԖڂɃ��Z�b�g�������������������p���ł���
		}

		if ((int)g_Camera.time >= maxNo - 1)
		{
			g_Camera.state = TRUE;
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
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Camera.pos), XMLoadFloat3(&g_Camera.at), XMLoadFloat3(&g_Camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Camera.mtxInvView, mtxInvView);

	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Camera.mtxProjection, mtxProjection);

	SetShaderCamera(g_Camera.pos);

}

//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA* GetCamera(void)
{
	return &g_Camera;
}

//=============================================================================
// �r���[�|�[�g�̐ݒ�
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext* g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = type;

	//-------------------------------------------------------------------------
	// �r���[�|�[�g�ݒ�
	//-------------------------------------------------------------------------
	switch (g_ViewPortType)
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
	return g_ViewPortType;
}

// �J�����̎��_�ƒ����_���Z�b�g
void SetCameraAT(XMFLOAT3 pos)
{
	// �J�����̒����_�������̍��W�ɂ��Ă݂�
	g_Camera.at = pos;

	if (g_Camera.state)
	{
		// �J�����̎��_���J������Y����]�ɑΉ������Ă���
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * cosf(g_Camera.rot.x) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * cosf(g_Camera.rot.x) * g_Camera.len;

		g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	}

}