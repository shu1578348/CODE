//=============================================================================
//
// �{�b�N�X���� [box.cpp]
// Author :�@�r�R�G�� 
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
static BOX	g_Box[BOX_MAX];			 // �ؔ��̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBox(void)
{

	for (int i = 0; i < BOX_MAX; i++)
	{
		LoadModel(MODEL_BOX, &g_Box[i].model);
		g_Box[i].load = TRUE;

		g_Box[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Box[i].scl  = { 0.3f, 0.3f, 0.3f };
		g_Box[i].size = { BOX_SIZE_X, BOX_SIZE_Y, BOX_SIZE_Z };

		g_Box[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		g_Box[i].use  = TRUE;	// TRUE:�g�p / FALSE:���g�p

	}

	// �ؔ��̕\���ʒu
	{
		g_Box[0].pos = { -400.0f, 15.0f,  0.0f };
		g_Box[0].rot = { 0.0f, 240.0f, 0.0f };
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
		if (g_Box[i].load)
		{
			UnloadModel(&g_Box[i].model);

			g_Box[i].load = FALSE;
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
		g_Box[i].draw = FrustumCulling(g_Box[i].pos,g_Box[i].rot, g_Box[i].size);
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
		if (!g_Box[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!g_Box[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Box[i].scl.x, g_Box[i].scl.y, g_Box[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Box[i].rot.x, g_Box[i].rot.y + XM_PI, g_Box[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Box[i].pos.x, g_Box[i].pos.y, g_Box[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Box[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&g_Box[i].model);

	}

}

//=============================================================================
// ����̏����擾
//=============================================================================
BOX* GetBox(void)
{
	return &g_Box[0];
}