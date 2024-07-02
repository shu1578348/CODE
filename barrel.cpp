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
static BARREL g_Barrel[BARREL_MAX];  // �o�����̍ő吔
static BOOL g_Load = FALSE;          // ���f���̏������t���O
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
        g_Barrel[i].pos = { 0.0f, 0.0f, 0.0f };
        g_Barrel[i].rot = { 0.0f, 0.0f, 0.0f };
        g_Barrel[i].scl = { 0.8f, 0.8f, 0.8f };
        g_Barrel[i].size = { BARREL_SIZE_X, BARREL_SIZE_Y, BARREL_SIZE_Z };

        g_Barrel[i].modelNo = 0; // �\�����f���̎��

        g_Barrel[i].draw = TRUE; // TRUE:�\�� / FALSE:��\��
        g_Barrel[i].use = TRUE;  // TRUE:�g�p / FALSE:���g�p
    }

    //-------------------------------------------------------------------------
    // ���f���̏�����
    //-------------------------------------------------------------------------
    {
        g_Load = TRUE;

        // ���f���̓ǂݍ���
        LoadModel(MODEL_BARREL, &modelBarrel);
    }

    //-------------------------------------------------------------------------
    // �ʒu�̐ݒ�
    //-------------------------------------------------------------------------
    {
        g_Barrel[0].pos = { -158.0f, 7.0f, 300.0f };
        g_Barrel[0].rot = { 0.0f, 0.0f, XM_PI / 2 };

        g_Barrel[1].pos = { -158.0f, 7.0f, 285.0f };
        g_Barrel[1].rot = { 0.0f, 0.0f, XM_PI / 2 };

        g_Barrel[2].pos = { -158.0f, 21.0f, 292.0f };
        g_Barrel[2].rot = { 0.0f, 0.0f, XM_PI / 2 };

        g_Barrel[3].pos = { -400.0f, 0.0f, -50.0f };
        g_Barrel[3].rot = { 0.0f, 0.0f, 0.0f };

        g_Barrel[4].pos = { -158.0f, 21.0f, 292.0f };
        g_Barrel[4].rot = { 0.0f, 0.0f, 0.0f };
    }

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBarrel()
{
    if (g_Load)
    {
        UnloadModel(&modelBarrel);

        g_Load = FALSE;
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
        g_Barrel[i].draw = FrustumCulling(g_Barrel[i].pos, g_Barrel[i].rot, g_Barrel[i].size);
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
        if (!g_Barrel[i].use) continue;

        // TRUE:�\�� / FALSE:��\��
        if (!g_Barrel[i].draw) continue;

        XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

        // ���[���h�}�g���b�N�X�̏�����
        mtxWorld = XMMatrixIdentity();

        // �X�P�[���𔽉f
        mtxScl = XMMatrixScaling(g_Barrel[i].scl.x, g_Barrel[i].scl.y, g_Barrel[i].scl.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

        // ��]�𔽉f
        mtxRot = XMMatrixRotationRollPitchYaw(g_Barrel[i].rot.x, g_Barrel[i].rot.y + XM_PI, g_Barrel[i].rot.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

        // �ړ��𔽉f
        mtxTranslate = XMMatrixTranslation(g_Barrel[i].pos.x, g_Barrel[i].pos.y, g_Barrel[i].pos.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

        // ���[���h�}�g���b�N�X�̐ݒ�
        SetWorldMatrix(&mtxWorld);

        XMStoreFloat4x4(&g_Barrel[i].mtxWorld, mtxWorld);

        // ���f���̕`��
        switch (g_Barrel[i].modelNo)
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
    return &g_Barrel[0];
}
