//=============================================================================
//
// ���f������ [carriage.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "carriage.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_CARRIAGE		"data/MODEL/carriage.obj"	// �ǂݍ��ރ��f����

#define CARRIAGE_SHADOW_SIZE	(1.0f)						// �e�̑傫��
#define CARRIAGE_OFFSET_Y		(0.0f)						// ����̉������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CARRIAGE	g_Carriage[CARRIAGE_MAX];			 // ����̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCarriage(void)
{

	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		LoadModel(MODEL_CARRIAGE, &g_Carriage[i].model);
		g_Carriage[i].load = TRUE;

		g_Carriage[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Carriage[i].scl  = { 0.6f, 0.6f, 0.6f };
		g_Carriage[i].size = { CARRIAGE_SIZE_X, CARRIAGE_SIZE_Y, CARRIAGE_SIZE_Z };

		g_Carriage[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		g_Carriage[i].use  = TRUE;	// TRUE:�g�p / FALSE:���g�p

	}

	// ����̕\���ʒu
	{
		g_Carriage[0].pos = { -400.0f, CARRIAGE_OFFSET_Y,  5.0f };
		g_Carriage[0].rot = { 0.0f, 240.0f, 0.0f };
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCarriage(void)
{
	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		// ���f���̉������
		if (g_Carriage[i].load)
		{
			UnloadModel(&g_Carriage[i].model);

			g_Carriage[i].load = FALSE;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCarriage(void)
{
	//-------------------------------------------------------------------------
	// ������J�����O����
	//-------------------------------------------------------------------------
	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		g_Carriage[i].draw = FrustumCulling(g_Carriage[i].pos,g_Carriage[i].rot, g_Carriage[i].size);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCarriage(void)
{

	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!g_Carriage[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!g_Carriage[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Carriage[i].scl.x, g_Carriage[i].scl.y, g_Carriage[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Carriage[i].rot.x, g_Carriage[i].rot.y + XM_PI, g_Carriage[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Carriage[i].pos.x, g_Carriage[i].pos.y, g_Carriage[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Carriage[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&g_Carriage[i].model);

	}

}

//=============================================================================
// ����̏����擾
//=============================================================================
CARRIAGE* GetCarriage(void)
{
	return &g_Carriage[0];
}