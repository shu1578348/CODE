//=============================================================================
//
// ���f������ [stall.cpp]
// Author : �r�R�@�G��
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
static STALL	stall[STALL_MAX];			 // ����̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitStall(void)
{

	for (int i = 0; i < STALL_MAX; i++)
	{
		LoadModel(MODEL_STALL, &stall[i].model);
		stall[i].load = TRUE;

		stall[i].rot  = { 0.0f, 0.0f, 0.0f };
		stall[i].scl  = { 1.5f, 1.5f, 1.5f };
		stall[i].size = { STALL_SIZE_X, STALL_SIZE_Y, STALL_SIZE_Z };

		stall[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		stall[i].use  = TRUE;		// TRUE:�g�p / FALSE:���g�p

	}

	// ����̕\���ʒu
	{
		stall[0].pos = { -120.0f, STALL_OFFSET_Y,  0.0f };
		stall[0].rot = { 0.0f, 0.0f, 0.0f };
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
		if (stall[i].load)
		{
			UnloadModel(&stall[i].model);

			stall[i].load = FALSE;
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
		stall[i].draw = FrustumCulling(stall[i].pos,stall[i].rot, stall[i].size);
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
		if (!stall[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!stall[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(stall[i].scl.x, stall[i].scl.y, stall[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(stall[i].rot.x, stall[i].rot.y + XM_PI, stall[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(stall[i].pos.x, stall[i].pos.y, stall[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&stall[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&stall[i].model);

	}

}

//=============================================================================
// ����̏����擾
//=============================================================================
STALL* GetStall(void)
{
	return &stall[0];
}