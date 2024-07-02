//=============================================================================
//
// モデル処理 [carriage.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "carriage.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_CARRIAGE		"data/MODEL/carriage.obj"	// 読み込むモデル名

#define CARRIAGE_SHADOW_SIZE	(1.0f)						// 影の大きさ
#define CARRIAGE_OFFSET_Y		(0.0f)						// 屋台の下をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CARRIAGE	g_Carriage[CARRIAGE_MAX];			 // 屋台の最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCarriage(void)
{

	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		LoadModel(MODEL_CARRIAGE, &g_Carriage[i].model);
		g_Carriage[i].load = TRUE;

		g_Carriage[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Carriage[i].scl  = { 0.6f, 0.6f, 0.6f };
		g_Carriage[i].size = { CARRIAGE_SIZE_X, CARRIAGE_SIZE_Y, CARRIAGE_SIZE_Z };

		g_Carriage[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		g_Carriage[i].use  = TRUE;	// TRUE:使用 / FALSE:未使用

	}

	// 屋台の表示位置
	{
		g_Carriage[0].pos = { -400.0f, CARRIAGE_OFFSET_Y,  5.0f };
		g_Carriage[0].rot = { 0.0f, 240.0f, 0.0f };
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCarriage(void)
{
	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		// モデルの解放処理
		if (g_Carriage[i].load)
		{
			UnloadModel(&g_Carriage[i].model);

			g_Carriage[i].load = FALSE;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCarriage(void)
{
	//-------------------------------------------------------------------------
	// 視錐台カリング処理
	//-------------------------------------------------------------------------
	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		g_Carriage[i].draw = FrustumCulling(g_Carriage[i].pos,g_Carriage[i].rot, g_Carriage[i].size);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCarriage(void)
{

	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Carriage[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!g_Carriage[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Carriage[i].scl.x, g_Carriage[i].scl.y, g_Carriage[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Carriage[i].rot.x, g_Carriage[i].rot.y + XM_PI, g_Carriage[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Carriage[i].pos.x, g_Carriage[i].pos.y, g_Carriage[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Carriage[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Carriage[i].model);

	}

}

//=============================================================================
// 屋台の情報を取得
//=============================================================================
CARRIAGE* GetCarriage(void)
{
	return &g_Carriage[0];
}