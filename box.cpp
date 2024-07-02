//=============================================================================
//
// �{�b�N�X���� [box.cpp]
// Author :�@�r�R �G�� 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "box.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BOX		"data/MODEL/box.obj"	// �ǂݍ��ރ��f����

#define BOX_SHADOW_SIZE		(1.0f)				// �e�̑傫��
#define BOX_OFFSET_Y		(0.0f)				// �ؔ��̉������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BOX	box[BOX_MAX];			 // �ؔ��̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBox(void)
{

	for (int i = 0; i < BOX_MAX; i++)
	{
		LoadModel(MODEL_BOX, &box[i].model);
		box[i].load = TRUE;

		box[i].rot  = { 0.0f, 0.0f, 0.0f };
		box[i].scl  = { 0.3f, 0.3f, 0.3f };
		box[i].size = { BOX_SIZE_X, BOX_SIZE_Y, BOX_SIZE_Z };

		box[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		box[i].use  = TRUE;	// TRUE:�g�p / FALSE:���g�p

	}

	// �ؔ��̕\���ʒu
	{
		box[0].pos = { -400.0f, 15.0f,  0.0f };
		box[0].rot = { 0.0f, 240.0f, 0.0f };
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBox(void)
{
	for (int i = 0; i < BOX_MAX; i++)
	{
		// ���f���̉������
		if (box[i].load)
		{
			UnloadModel(&box[i].model);

			box[i].load = FALSE;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBox(void)
{
	//-------------------------------------------------------------------------
	// ������J�����O����
	//-------------------------------------------------------------------------
	for (int i = 0; i < BOX_MAX; i++)
	{
		box[i].draw = FrustumCulling(box[i].pos,box[i].rot, box[i].size);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBox(void)
{

	for (int i = 0; i < BOX_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!box[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!box[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(box[i].scl.x, box[i].scl.y, box[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(box[i].rot.x, box[i].rot.y + XM_PI, box[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(box[i].pos.x, box[i].pos.y, box[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&box[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&box[i].model);

	}

}

//=============================================================================
// ����̏����擾
//=============================================================================
BOX* GetBox(void)
{
	return &box[0];
}