//=============================================================================
//
// モデル処理 [carriage.cpp]
// Author : 荒山 秀磨
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
static CARRIAGE	carriage[CARRIAGE_MAX];			 // 屋台の最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCarriage(void)
{

	for (int i = 0; i < CARRIAGE_MAX; i++)
	{
		LoadModel(MODEL_CARRIAGE, &carriage[i].model);
		carriage[i].load = TRUE;

		carriage[i].rot  = { 0.0f, 0.0f, 0.0f };
		carriage[i].scl  = { 0.6f, 0.6f, 0.6f };
		carriage[i].size = { CARRIAGE_SIZE_X, CARRIAGE_SIZE_Y, CARRIAGE_SIZE_Z };

		carriage[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		carriage[i].use  = TRUE;	// TRUE:使用 / FALSE:未使用

	}

	// 屋台の表示位置
	{
		carriage[0].pos = { -400.0f, CARRIAGE_OFFSET_Y,  5.0f };
		carriage[0].rot = { 0.0f, 240.0f, 0.0f };
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
		if (carriage[i].load)
		{
			UnloadModel(&carriage[i].model);

			carriage[i].load = FALSE;
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
		carriage[i].draw = FrustumCulling(carriage[i].pos,carriage[i].rot, carriage[i].size);
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
		if (!carriage[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!carriage[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(carriage[i].scl.x, carriage[i].scl.y, carriage[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(carriage[i].rot.x, carriage[i].rot.y + XM_PI, carriage[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(carriage[i].pos.x, carriage[i].pos.y, carriage[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&carriage[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&carriage[i].model);

	}

}

//=============================================================================
// 屋台の情報を取得
//=============================================================================
CARRIAGE* GetCarriage(void)
{
	return &carriage[0];
}