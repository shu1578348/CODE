//=============================================================================
//
// ���f������ [carriage.cpp]
// Author : �r�R �G��
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
static CARRIAGE	carriage[CARRIAGE_MAX];			 // ����̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCarriage(void)
{

	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		LoadModel(MODEL_CARRIAGE, &carriage[i].model);
		carriage[i].load = TRUE;

		carriage[i].rot  = { 0.0f, 0.0f, 0.0f };
		carriage[i].scl  = { 0.6f, 0.6f, 0.6f };
		carriage[i].size = { CARRIAGE_SIZE_X, CARRIAGE_SIZE_Y, CARRIAGE_SIZE_Z };

		carriage[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		carriage[i].use  = TRUE;	// TRUE:�g�p / FALSE:���g�p

	}

	// ����̕\���ʒu
	{
		carriage[0].pos = { -400.0f, CARRIAGE_OFFSET_Y,  5.0f };
		carriage[0].rot = { 0.0f, 240.0f, 0.0f };
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
		if (carriage[i].load)
		{
			UnloadModel(&carriage[i].model);

			carriage[i].load = FALSE;
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
		carriage[i].draw = FrustumCulling(carriage[i].pos,carriage[i].rot, carriage[i].size);
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
		if (!carriage[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!carriage[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(carriage[i].scl.x, carriage[i].scl.y, carriage[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(carriage[i].rot.x, carriage[i].rot.y + XM_PI, carriage[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(carriage[i].pos.x, carriage[i].pos.y, carriage[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&carriage[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&carriage[i].model);

	}

}

//=============================================================================
// ����̏����擾
//=============================================================================
CARRIAGE* GetCarriage(void)
{
	return &carriage[0];
}