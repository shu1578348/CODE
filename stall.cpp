//=============================================================================
//
// モデル処理 [stall.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "stall.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_STALL		"data/MODEL/stall.obj"			// 読み込むモデル名

#define STALL_SHADOW_SIZE	(1.0f)						// 影の大きさ
#define STALL_OFFSET_Y		(0.0f)						// 屋台の下をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static STALL	g_Stall[STALL_MAX];			 // 屋台の最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStall(void)
{

	for (int i = 0; i < STALL_MAX; i++)
	{
		LoadModel(MODEL_STALL, &g_Stall[i].model);
		g_Stall[i].load = TRUE;

		g_Stall[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Stall[i].scl  = { 1.5f, 1.5f, 1.5f };
		g_Stall[i].size = { STALL_SIZE_X, STALL_SIZE_Y, STALL_SIZE_Z };

		g_Stall[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		g_Stall[i].use  = TRUE;		// TRUE:使用 / FALSE:未使用

	}

	// 屋台の表示位置
	{
		g_Stall[0].pos = { -120.0f, STALL_OFFSET_Y,  0.0f };
		g_Stall[0].rot = { 0.0f, 0.0f, 0.0f };
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStall(void)
{
	for (int i = 0; i < STALL_MAX; i++)
	{
		// モデルの解放処理
		if (g_Stall[i].load)
		{
			UnloadModel(&g_Stall[i].model);

			g_Stall[i].load = FALSE;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStall(void)
{
	//-------------------------------------------------------------------------
	// 視錐台カリング処理
	//-------------------------------------------------------------------------
	for (int i = 0; i < STALL_MAX; i++)
	{
		g_Stall[i].draw = FrustumCulling(g_Stall[i].pos,g_Stall[i].rot, g_Stall[i].size);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStall(void)
{

	for (int i = 0; i < STALL_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Stall[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!g_Stall[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Stall[i].scl.x, g_Stall[i].scl.y, g_Stall[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Stall[i].rot.x, g_Stall[i].rot.y + XM_PI, g_Stall[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Stall[i].pos.x, g_Stall[i].pos.y, g_Stall[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Stall[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Stall[i].model);

	}

}

//=============================================================================
// 屋台の情報を取得
//=============================================================================
STALL* GetStall(void)
{
	return &g_Stall[0];
}