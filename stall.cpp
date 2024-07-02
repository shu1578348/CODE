//=============================================================================
//
// ���f������ [stall.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "stall.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_STALL		"data/MODEL/stall.obj"			// �ǂݍ��ރ��f����

#define STALL_SHADOW_SIZE	(1.0f)						// �e�̑傫��
#define STALL_OFFSET_Y		(0.0f)						// ����̉������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static STALL	g_Stall[STALL_MAX];			 // ����̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitStall(void)
{

	for (int i = 0; i < STALL_MAX; i++)
	{
		LoadModel(MODEL_STALL, &g_Stall[i].model);
		g_Stall[i].load = TRUE;

		g_Stall[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Stall[i].scl  = { 1.5f, 1.5f, 1.5f };
		g_Stall[i].size = { STALL_SIZE_X, STALL_SIZE_Y, STALL_SIZE_Z };

		g_Stall[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		g_Stall[i].use  = TRUE;		// TRUE:�g�p / FALSE:���g�p

	}

	// ����̕\���ʒu
	{
		g_Stall[0].pos = { -120.0f, STALL_OFFSET_Y,  0.0f };
		g_Stall[0].rot = { 0.0f, 0.0f, 0.0f };
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitStall(void)
{
	for (int i = 0; i < STALL_MAX; i++)
	{
		// ���f���̉������
		if (g_Stall[i].load)
		{
			UnloadModel(&g_Stall[i].model);

			g_Stall[i].load = FALSE;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateStall(void)
{
	//-------------------------------------------------------------------------
	// ������J�����O����
	//-------------------------------------------------------------------------
	for (int i = 0; i < STALL_MAX; i++)
	{
		g_Stall[i].draw = FrustumCulling(g_Stall[i].pos,g_Stall[i].rot, g_Stall[i].size);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStall(void)
{

	for (int i = 0; i < STALL_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!g_Stall[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!g_Stall[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Stall[i].scl.x, g_Stall[i].scl.y, g_Stall[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Stall[i].rot.x, g_Stall[i].rot.y + XM_PI, g_Stall[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Stall[i].pos.x, g_Stall[i].pos.y, g_Stall[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Stall[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&g_Stall[i].model);

	}

}

//=============================================================================
// ����̏����擾
//=============================================================================
STALL* GetStall(void)
{
	return &g_Stall[0];
}