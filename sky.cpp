//=============================================================================
//
// モデル処理 [sky.cpp]
// Author : 荒山　秀磨
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
// マクロ定義
//*****************************************************************************
#define	MODEL_SKY		"data/MODEL/sky.obj"			// 読み込むモデル名

#define SKY_MAX				(1)                         // 空の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static SKY			    sky[SKY_MAX];			 // 空の最大数

//=============================================================================
// 初期化処理
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
// 終了処理
//=============================================================================
void UninitSky(void)
{
	for (int i = 0; i < SKY_MAX; i++)
	{
		// モデルの解放処理
		if (sky[i].load)
		{
			UnloadModel(&sky[i].model);
			sky[i].load = FALSE;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSky(void)
{
	// 回転させて空を動かす
	sky[0].rot.x += 0.0001f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSky(void)
{
	for (int i = 0; i < SKY_MAX; i++)
	{
	
		// ライティングを無効
		SetLightEnable(FALSE);		

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(sky[i].scl.x, sky[i].scl.y, sky[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(sky[i].rot.x, sky[i].rot.y + XM_PI, sky[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(sky[i].pos.x, sky[i].pos.y, sky[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&sky[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&sky[i].model);

		// ライティングを有効に
		SetLightEnable(TRUE);
	}


}

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
SKY* GetSky(void)
{
	return &sky[0];
}

