//=============================================================================
//
// ���f������ [sky.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "sky.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_SKY		"data/MODEL/sky.obj"			// �ǂݍ��ރ��f����

#define SKY_MAX				(1)                         // ��̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static SKY			    g_Sky[SKY_MAX];			 // ��̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSky(void)
{
	for (int i = 0; i < SKY_MAX; i++)
	{
		LoadModel(MODEL_SKY, &g_Sky[i].model);
		g_Sky[i].load = TRUE;

		g_Sky[i].pos = { 0.0f, -100.0f, 0.0f };
		g_Sky[i].rot = { 0.0f,  0.0f, 0.0f };
		g_Sky[i].scl = { 50.0f, 50.0f, 50.0f };

		g_Sky[i].use = TRUE;

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSky(void)
{
	for (int i = 0; i < SKY_MAX; i++)
	{
		// ���f���̉������
		if (g_Sky[i].load)
		{
			UnloadModel(&g_Sky[i].model);
			g_Sky[i].load = FALSE;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSky(void)
{
	// ��]�����ċ�𓮂���
	g_Sky[0].rot.x += 0.0001f;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSky(void)
{
	for (int i = 0; i < SKY_MAX; i++)
	{
	
		// ���C�e�B���O�𖳌�
		SetLightEnable(FALSE);		

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Sky[i].scl.x, g_Sky[i].scl.y, g_Sky[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Sky[i].rot.x, g_Sky[i].rot.y + XM_PI, g_Sky[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Sky[i].pos.x, g_Sky[i].pos.y, g_Sky[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Sky[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&g_Sky[i].model);

		// ���C�e�B���O��L����
		SetLightEnable(TRUE);
	}


}

//=============================================================================
// �v���C���[�����擾
//=============================================================================
SKY* GetSky(void)
{
	return &g_Sky[0];
}

