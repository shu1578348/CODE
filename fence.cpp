//=============================================================================
//
// ���f������ [fence.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "fence.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_FENCE	 "data/MODEL/fence.obj"	// �ǂݍ��ރ��f����

#define FENCE_SHADOW_SIZE	(0.4f)	// �e�̑傫��
#define FENCE_OFFSET_Y		(0.0f)	// ��̉������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static FENCE g_Fence[FENCE_MAX]; // ��̍ő吔
static BOOL	g_Load = FALSE;		 // ���f���̏������t���O
DX11_MODEL modelFence;			 // ���f�����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFence(void)
{
	//-------------------------------------------------------------------------
	//�@���̏�����
	//-------------------------------------------------------------------------
	for (int i = 0; i < FENCE_MAX; i++)
	{		
		g_Fence[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Fence[i].scl  = { 0.8f, 0.5f, 0.8f };
		g_Fence[i].size = { FENCE_SIZE_X, FENCE_SIZE_Y, FENCE_SIZE_Z };

		g_Fence[i].modelNo = 0;

		g_Fence[i].draw = TRUE;	// TRUE:�\�� / FALSE:��\��
		g_Fence[i].use  = TRUE;	// TRUE:�g�p / FALSE:���g�p
	}

	//-------------------------------------------------------------------------
	//�@���f���̓ǂݍ���
	//-------------------------------------------------------------------------
	{
		g_Load = TRUE;

		// ���f���̓ǂݍ���
		LoadModel(MODEL_FENCE, &modelFence);

	}

	//-------------------------------------------------------------------------
	//�@�ʒu�ݒ�
	//-------------------------------------------------------------------------
	{
		// house[0]�̎���
		{

			g_Fence[0].pos = { -160.0f, FENCE_OFFSET_Y, 100.0f };
			g_Fence[0].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[1].pos = { -120.0f, FENCE_OFFSET_Y, 100.0f };
			g_Fence[1].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[2].pos = { -80.0f, FENCE_OFFSET_Y,  100.0f };
			g_Fence[2].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[3].pos = { -63.0f, FENCE_OFFSET_Y,  115.0f };
			g_Fence[3].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[4].pos = { -63.0f, FENCE_OFFSET_Y,  155.0f };
			g_Fence[4].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[5].pos = { -63.0f, FENCE_OFFSET_Y,  195.0f };
			g_Fence[5].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[6].pos = { -63.0f, FENCE_OFFSET_Y,  235.0f };
			g_Fence[6].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[7].pos = { -63.0f, FENCE_OFFSET_Y,  275.0f };
			g_Fence[7].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[8].pos = { -63.0f, FENCE_OFFSET_Y,  315.0f };
			g_Fence[8].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[9].pos = { -63.0f, FENCE_OFFSET_Y,  355.0f };
			g_Fence[9].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[10].pos = { -80.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[10].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[11].pos = { -120.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[11].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[12].pos = { -160.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[12].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[13].pos = { -200.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[13].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[14].pos = { -240.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[14].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[15].pos = { -280.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[15].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[16].pos = { -320.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[16].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[17].pos = { -345.0f, FENCE_OFFSET_Y, 355.0f };
			g_Fence[17].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[18].pos = { -345.0f, FENCE_OFFSET_Y, 315.0f };
			g_Fence[18].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[19].pos = { -345.0f, FENCE_OFFSET_Y, 275.0f };
			g_Fence[19].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[20].pos = { -345.0f, FENCE_OFFSET_Y, 235.0f };
			g_Fence[20].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[21].pos = { -345.0f, FENCE_OFFSET_Y, 195.0f };
			g_Fence[21].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[22].pos = { -345.0f, FENCE_OFFSET_Y, 155.0f };
			g_Fence[22].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[23].pos = { -345.0f, FENCE_OFFSET_Y, 115.0f };
			g_Fence[23].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[24].pos = { -320.0f, FENCE_OFFSET_Y, 100.0f };
			g_Fence[24].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[25].pos = { -280.0f, FENCE_OFFSET_Y, 100.0f };
			g_Fence[25].rot = { 0.0f, 0.0f, 0.0f };

		}

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFence(void)
{
	//-------------------------------------------------------------------------
	//�@���f���̊J��
	//-------------------------------------------------------------------------
	if (g_Load)
	{
		UnloadModel(&modelFence);

		g_Load = FALSE;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFence(void)
{
	//-------------------------------------------------------------------------
	// ������J�����O
	//-------------------------------------------------------------------------
	for (int i = 0; i < FENCE_MAX; i++)
	{
		
		g_Fence[i].draw = FrustumCulling(g_Fence[i].pos, g_Fence[i].rot, g_Fence[i].size);
		
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFence(void)
{

	for (int i = 0; i < FENCE_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!g_Fence[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!g_Fence[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Fence[i].scl.x, g_Fence[i].scl.y, g_Fence[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Fence[i].rot.x, g_Fence[i].rot.y + XM_PI, g_Fence[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Fence[i].pos.x, g_Fence[i].pos.y, g_Fence[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Fence[i].mtxWorld, mtxWorld);

		// ���f���`��
		switch (g_Fence[i].modelNo)
		{
		case 0:
			DrawModel(&modelFence);
			break;
		}

	}

}


//=============================================================================
// ��̏����擾
//=============================================================================
FENCE* GetFence(void)
{
	return &g_Fence[0];
}

