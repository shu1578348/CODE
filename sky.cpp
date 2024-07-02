//=============================================================================
//
// ���f������ [sky.cpp]
// Author : �r�R�@�G��
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
static SKY			    sky[SKY_MAX];			 // ��̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSky(void)
{
	for (int i = 0; i < SKY_MAX; i++)
	{
		LoadModel(MODEL_SKY, &sky[i].model);
		sky[i].load = TRUE;

		sky[i].pos = { 0.0f, -100.0f, 0.0f };
		sky[i].rot = { 0.0f,  0.0f, 0.0f };
		sky[i].scl = { 50.0f, 50.0f, 50.0f };

		sky[i].use = TRUE;

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
		if (sky[i].load)
		{
			UnloadModel(&sky[i].model);
			sky[i].load = FALSE;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSky(void)
{
	// ��]�����ċ�𓮂���
	sky[0].rot.x += 0.0001f;
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
		mtxScl = XMMatrixScaling(sky[i].scl.x, sky[i].scl.y, sky[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(sky[i].rot.x, sky[i].rot.y + XM_PI, sky[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(sky[i].pos.x, sky[i].pos.y, sky[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&sky[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&sky[i].model);

		// ���C�e�B���O��L����
		SetLightEnable(TRUE);
	}


}

//=============================================================================
// �v���C���[�����擾
//=============================================================================
SKY* GetSky(void)
{
	return &sky[0];
}

