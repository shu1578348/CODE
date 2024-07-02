//=============================================================================
//
// �o�������� [barrel.cpp]
// Author : �r�R �G��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "barrel.h"
#include "player.h"
#include "collision.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_BARREL  "data/MODEL/barrel.obj"  // �ǂݍ��ރ��f����

constexpr float BARREL_SHADOW_SIZE = 0.4f; // �e�̑傫��
constexpr float BARREL_OFFSET_Y = 0.0f;    // �o�����̉������킹��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BARREL barrel[BARREL_MAX];  // �o�����̍ő吔
static BOOL load = FALSE;          // ���f���̏������t���O
DX11_MODEL modelBarrel;              // ���f�����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBarrel()
{
    //-------------------------------------------------------------------------
    // ���̏�����
    //-------------------------------------------------------------------------
    for (int i = 0; i < BARREL_MAX; i++)
    {
        barrel[i].pos = { 0.0f, 0.0f, 0.0f };
        barrel[i].rot = { 0.0f, 0.0f, 0.0f };
        barrel[i].scl = { 0.8f, 0.8f, 0.8f };
        barrel[i].size = { BARREL_SIZE_X, BARREL_SIZE_Y, BARREL_SIZE_Z };

        barrel[i].modelNo = 0; // �\�����f���̎��

        barrel[i].draw = TRUE; // TRUE:�\�� / FALSE:��\��
        barrel[i].use = TRUE;  // TRUE:�g�p / FALSE:���g�p
    }

    //-------------------------------------------------------------------------
    // ���f���̏�����
    //-------------------------------------------------------------------------
    {
        load = TRUE;

        // ���f���̓ǂݍ���
        LoadModel(MODEL_BARREL, &modelBarrel);
    }

    //-------------------------------------------------------------------------
    // �ʒu�̐ݒ�
    //-------------------------------------------------------------------------
    {
        barrel[0].pos = { -158.0f, 7.0f, 300.0f };
        barrel[0].rot = { 0.0f, 0.0f, XM_PI / 2 };

        barrel[1].pos = { -158.0f, 7.0f, 285.0f };
        barrel[1].rot = { 0.0f, 0.0f, XM_PI / 2 };

        barrel[2].pos = { -158.0f, 21.0f, 292.0f };
        barrel[2].rot = { 0.0f, 0.0f, XM_PI / 2 };

        barrel[3].pos = { -400.0f, 0.0f, -50.0f };
        barrel[3].rot = { 0.0f, 0.0f, 0.0f };

        barrel[4].pos = { -158.0f, 21.0f, 292.0f };
        barrel[4].rot = { 0.0f, 0.0f, 0.0f };
    }

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBarrel()
{
    if (load)
    {
        UnloadModel(&modelBarrel);

        load = FALSE;
    }
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBarrel()
{
    //-------------------------------------------------------------------------
    // �����^�J�����O����
    //-------------------------------------------------------------------------
    for (int i = 0; i < BARREL_MAX; i++)
    {
        barrel[i].draw = FrustumCulling(barrel[i].pos, barrel[i].rot, barrel[i].size);
    }
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBarrel()
{
    for (int i = 0; i < BARREL_MAX; i++)
    {
        // TRUE:�g�p / FALSE:���g�p
        if (!barrel[i].use) continue;

        // TRUE:�\�� / FALSE:��\��
        if (!barrel[i].draw) continue;

        XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

        // ���[���h�}�g���b�N�X�̏�����
        mtxWorld = XMMatrixIdentity();

        // �X�P�[���𔽉f
        mtxScl = XMMatrixScaling(barrel[i].scl.x, barrel[i].scl.y, barrel[i].scl.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

        // ��]�𔽉f
        mtxRot = XMMatrixRotationRollPitchYaw(barrel[i].rot.x, barrel[i].rot.y + XM_PI, barrel[i].rot.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

        // �ړ��𔽉f
        mtxTranslate = XMMatrixTranslation(barrel[i].pos.x, barrel[i].pos.y, barrel[i].pos.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

        // ���[���h�}�g���b�N�X�̐ݒ�
        SetWorldMatrix(&mtxWorld);

        XMStoreFloat4x4(&barrel[i].mtxWorld, mtxWorld);

        // ���f���̕`��
        switch (barrel[i].modelNo)
        {
        case 0:
            DrawModel(&modelBarrel);
            break;
        }
    }
}

//=============================================================================
// �M�̏����擾
//=============================================================================
BARREL* GetBarrel()
{
    return &barrel[0];
}
