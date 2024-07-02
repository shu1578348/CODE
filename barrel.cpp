//=============================================================================
//
// バレル処理 [barrel.cpp]
// Author : 荒山 秀磨
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
// マクロ定義
//*****************************************************************************
#define MODEL_BARREL  "data/MODEL/barrel.obj"  // 読み込むモデル名

constexpr float BARREL_SHADOW_SIZE = 0.4f; // 影の大きさ
constexpr float BARREL_OFFSET_Y = 0.0f;    // バレルの下をあわせる

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BARREL g_Barrel[BARREL_MAX];  // バレルの最大数
static BOOL g_Load = FALSE;          // モデルの初期化フラグ
DX11_MODEL modelBarrel;              // モデル情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBarrel()
{
    //-------------------------------------------------------------------------
    // 情報の初期化
    //-------------------------------------------------------------------------
    for (int i = 0; i < BARREL_MAX; i++)
    {
        g_Barrel[i].pos = { 0.0f, 0.0f, 0.0f };
        g_Barrel[i].rot = { 0.0f, 0.0f, 0.0f };
        g_Barrel[i].scl = { 0.8f, 0.8f, 0.8f };
        g_Barrel[i].size = { BARREL_SIZE_X, BARREL_SIZE_Y, BARREL_SIZE_Z };

        g_Barrel[i].modelNo = 0; // 表示モデルの種類

        g_Barrel[i].draw = TRUE; // TRUE:表示 / FALSE:非表示
        g_Barrel[i].use = TRUE;  // TRUE:使用 / FALSE:未使用
    }

    //-------------------------------------------------------------------------
    // モデルの初期化
    //-------------------------------------------------------------------------
    {
        g_Load = TRUE;

        // モデルの読み込み
        LoadModel(MODEL_BARREL, &modelBarrel);
    }

    //-------------------------------------------------------------------------
    // 位置の設定
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
// 終了処理
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
// 更新処理
//=============================================================================
void UpdateBarrel()
{
    //-------------------------------------------------------------------------
    // 視錐型カリング処理
    //-------------------------------------------------------------------------
    for (int i = 0; i < BARREL_MAX; i++)
    {
        g_Barrel[i].draw = FrustumCulling(g_Barrel[i].pos, g_Barrel[i].rot, g_Barrel[i].size);
    }
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBarrel()
{
    for (int i = 0; i < BARREL_MAX; i++)
    {
        // TRUE:使用 / FALSE:未使用
        if (!g_Barrel[i].use) continue;

        // TRUE:表示 / FALSE:非表示
        if (!g_Barrel[i].draw) continue;

        XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

        // ワールドマトリックスの初期化
        mtxWorld = XMMatrixIdentity();

        // スケールを反映
        mtxScl = XMMatrixScaling(g_Barrel[i].scl.x, g_Barrel[i].scl.y, g_Barrel[i].scl.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

        // 回転を反映
        mtxRot = XMMatrixRotationRollPitchYaw(g_Barrel[i].rot.x, g_Barrel[i].rot.y + XM_PI, g_Barrel[i].rot.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

        // 移動を反映
        mtxTranslate = XMMatrixTranslation(g_Barrel[i].pos.x, g_Barrel[i].pos.y, g_Barrel[i].pos.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

        // ワールドマトリックスの設定
        SetWorldMatrix(&mtxWorld);

        XMStoreFloat4x4(&g_Barrel[i].mtxWorld, mtxWorld);

        // モデルの描画
        switch (g_Barrel[i].modelNo)
        {
        case 0:
            DrawModel(&modelBarrel);
            break;
        }
    }
}

//=============================================================================
// 樽の情報を取得
//=============================================================================
BARREL* GetBarrel()
{
    return &g_Barrel[0];
}
