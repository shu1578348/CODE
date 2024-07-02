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
static BARREL barrel[BARREL_MAX];  // バレルの最大数
static BOOL load = FALSE;          // モデルの初期化フラグ
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
        barrel[i].pos = { 0.0f, 0.0f, 0.0f };
        barrel[i].rot = { 0.0f, 0.0f, 0.0f };
        barrel[i].scl = { 0.8f, 0.8f, 0.8f };
        barrel[i].size = { BARREL_SIZE_X, BARREL_SIZE_Y, BARREL_SIZE_Z };

        barrel[i].modelNo = 0; // 表示モデルの種類

        barrel[i].draw = TRUE; // TRUE:表示 / FALSE:非表示
        barrel[i].use = TRUE;  // TRUE:使用 / FALSE:未使用
    }

    //-------------------------------------------------------------------------
    // モデルの初期化
    //-------------------------------------------------------------------------
    {
        load = TRUE;

        // モデルの読み込み
        LoadModel(MODEL_BARREL, &modelBarrel);
    }

    //-------------------------------------------------------------------------
    // 位置の設定
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
// 終了処理
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
// 更新処理
//=============================================================================
void UpdateBarrel()
{
    //-------------------------------------------------------------------------
    // 視錐型カリング処理
    //-------------------------------------------------------------------------
    for (int i = 0; i < BARREL_MAX; i++)
    {
        barrel[i].draw = FrustumCulling(barrel[i].pos, barrel[i].rot, barrel[i].size);
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
        if (!barrel[i].use) continue;

        // TRUE:表示 / FALSE:非表示
        if (!barrel[i].draw) continue;

        XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

        // ワールドマトリックスの初期化
        mtxWorld = XMMatrixIdentity();

        // スケールを反映
        mtxScl = XMMatrixScaling(barrel[i].scl.x, barrel[i].scl.y, barrel[i].scl.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

        // 回転を反映
        mtxRot = XMMatrixRotationRollPitchYaw(barrel[i].rot.x, barrel[i].rot.y + XM_PI, barrel[i].rot.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

        // 移動を反映
        mtxTranslate = XMMatrixTranslation(barrel[i].pos.x, barrel[i].pos.y, barrel[i].pos.z);
        mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

        // ワールドマトリックスの設定
        SetWorldMatrix(&mtxWorld);

        XMStoreFloat4x4(&barrel[i].mtxWorld, mtxWorld);

        // モデルの描画
        switch (barrel[i].modelNo)
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
    return &barrel[0];
}
