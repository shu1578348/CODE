//=============================================================================
//
// モデル処理 [stall.cpp]
// Author : 荒山　秀磨
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
static STALL	stall[STALL_MAX];			 // 屋台の最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStall(void)
{

	for (int i = 0; i < STALL_MAX; i++)
	{
		LoadModel(MODEL_STALL, &stall[i].model);
		stall[i].load = TRUE;

		stall[i].rot  = { 0.0f, 0.0f, 0.0f };
		stall[i].scl  = { 1.5f, 1.5f, 1.5f };
		stall[i].size = { STALL_SIZE_X, STALL_SIZE_Y, STALL_SIZE_Z };

		stall[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		stall[i].use  = TRUE;		// TRUE:使用 / FALSE:未使用

	}

	// 屋台の表示位置
	{
		stall[0].pos = { -120.0f, STALL_OFFSET_Y,  0.0f };
		stall[0].rot = { 0.0f, 0.0f, 0.0f };
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
		if (stall[i].load)
		{
			UnloadModel(&stall[i].model);

			stall[i].load = FALSE;
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
		stall[i].draw = FrustumCulling(stall[i].pos,stall[i].rot, stall[i].size);
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
		if (!stall[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!stall[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(stall[i].scl.x, stall[i].scl.y, stall[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(stall[i].rot.x, stall[i].rot.y + XM_PI, stall[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(stall[i].pos.x, stall[i].pos.y, stall[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&stall[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&stall[i].model);

	}

}

//=============================================================================
// 屋台の情報を取得
//=============================================================================
STALL* GetStall(void)
{
	return &stall[0];
}