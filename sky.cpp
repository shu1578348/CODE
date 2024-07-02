//=============================================================================
//
// モデル処理 [sky.cpp]
// Author : 
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
static SKY			    g_Sky[SKY_MAX];			 // 空の最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSky(void)
{
	for (int i = 0; i < SKY_MAX; i++)
	{
		LoadModel(MODEL_SKY, &g_Sky[i].model);
		g_Sky[i].load = TRUE;

		g_Sky[i].pos = { 0.0f, -100.0f, 0.0f };
		g_Sky[i].rot = { 0.0f,  0.0f, 0.0f };
		g_Sky[i].scl = { 50.0f, 50.0f, 50.0f };

		g_Sky[i].use = TRUE;

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
		if (g_Sky[i].load)
		{
			UnloadModel(&g_Sky[i].model);
			g_Sky[i].load = FALSE;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSky(void)
{
	// 回転させて空を動かす
	g_Sky[0].rot.x += 0.0001f;
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
		mtxScl = XMMatrixScaling(g_Sky[i].scl.x, g_Sky[i].scl.y, g_Sky[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Sky[i].rot.x, g_Sky[i].rot.y + XM_PI, g_Sky[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Sky[i].pos.x, g_Sky[i].pos.y, g_Sky[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Sky[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Sky[i].model);

		// ライティングを有効に
		SetLightEnable(TRUE);
	}


}

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
SKY* GetSky(void)
{
	return &g_Sky[0];
}

