//=============================================================================
//
// ���f������ [speartrap.cpp]
// Author : �r�R �G��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "speartrap.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_SPEAR        "data/MODEL/spear_trap.obj"		// �ǂݍ��ރ��f����

#define TRAP_SHADOW_SIZE	(0.4f)	// �e�̑傫��
#define TRAP_OFFSET_Y		(0.0f)	// ���̉������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static TRAP g_Trap[TRAP_MAX];	 // 㩂̍ő吔
static BOOL	g_Load = FALSE;		 // ���f���̏������t���O
DX11_MODEL modelSpear;			 // ���f�����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTrap(void)
{
	//-------------------------------------------------------------------------
	// ���̏�����
	//-------------------------------------------------------------------------
	for (int i = 0; i < TRAP_MAX; i++)
	{
		g_Trap[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Trap[i].scl = { 1.0f, 1.0f, 1.0f };

		g_Trap[i].time = 0;

		g_Trap[i].modelNo = 0; // �\�����f���̎��

		g_Trap[i].size = TRAP_SIZE;  // �����蔻��̑傫��

		g_Trap[i].draw = TRUE;  // TRUE:�\�� / FALSE:��\��
		g_Trap[i].use  = TRUE;  // TRUE:�g�p / FALSE:���g�p
		g_Trap[i].status = 0;   // ���
	}

	//-------------------------------------------------------------------------
	// ���f���̓ǂݍ���
	//-------------------------------------------------------------------------
	{
		g_Load = TRUE;
		
		// ���f���̓ǂݍ���
		LoadModel(MODEL_SPEAR,  &modelSpear);
	}

	//-------------------------------------------------------------------------
	// �ʒu�̐ݒ�
	//-------------------------------------------------------------------------
	{
		g_Trap[0].pos  = { -240.0f, 30.0f, -380.0f };
		g_Trap[0].rot  = { 0.0f, 0.0f, XM_PI / 2};
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTrap(void)
{
	if (g_Load)
	{
		UnloadModel(&modelSpear);
		g_Load = FALSE;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTrap(void)
{		
	for (int i = 0; i < TRAP_MAX; i++)
	{ 
		switch (g_Trap[i].status)
		{
		case 0:
			break;
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTrap(void)
{
	for (int i = 0; i < TRAP_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!g_Trap[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!g_Trap[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Trap[i].scl.x, g_Trap[i].scl.y, g_Trap[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Trap[i].rot.x, g_Trap[i].rot.y + XM_PI, g_Trap[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Trap[i].pos.x, g_Trap[i].pos.y, g_Trap[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Trap[i].mtxWorld, mtxWorld);

		// ���f���̕`��
		switch (g_Trap[i].modelNo)
		{
		case 0:
			DrawModel(&modelSpear);
			break;
		}

	}
}

//=============================================================================
// �M�̏����擾
//=============================================================================
TRAP* GetTrap(void)
{
	return &g_Trap[0];
}