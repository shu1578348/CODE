//=============================================================================
//
// モデル処理 [fence.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "fence.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_FENCE	 "data/MODEL/fence.obj"	// 読み込むモデル名

#define FENCE_SHADOW_SIZE	(0.4f)	// 影の大きさ
#define FENCE_OFFSET_Y		(0.0f)	// 柵の下をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static FENCE g_Fence[FENCE_MAX]; // 柵の最大数
static BOOL	g_Load = FALSE;		 // モデルの初期化フラグ
DX11_MODEL modelFence;			 // モデル情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFence(void)
{
	//-------------------------------------------------------------------------
	//　情報の初期化
	//-------------------------------------------------------------------------
	for (int i = 0; i < FENCE_MAX; i++)
	{		
		g_Fence[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Fence[i].scl  = { 0.8f, 0.5f, 0.8f };
		g_Fence[i].size = { FENCE_SIZE_X, FENCE_SIZE_Y, FENCE_SIZE_Z };

		g_Fence[i].modelNo = 0;

		g_Fence[i].draw = TRUE;	// TRUE:表示 / FALSE:非表示
		g_Fence[i].use  = TRUE;	// TRUE:使用 / FALSE:未使用
	}

	//-------------------------------------------------------------------------
	//　モデルの読み込み
	//-------------------------------------------------------------------------
	{
		g_Load = TRUE;

		// モデルの読み込み
		LoadModel(MODEL_FENCE, &modelFence);

	}

	//-------------------------------------------------------------------------
	//　位置設定
	//-------------------------------------------------------------------------
	{
		// house[0]の周囲
		{

			g_Fence[0].pos = { -160.0f, FENCE_OFFSET_Y, 100.0f };
			g_Fence[0].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[1].pos = { -120.0f, FENCE_OFFSET_Y, 100.0f };
			g_Fence[1].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[2].pos = { -80.0f, FENCE_OFFSET_Y,  100.0f };
			g_Fence[2].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[3].pos = { -63.0f, FENCE_OFFSET_Y,  115.0f };
			g_Fence[3].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[4].pos = { -63.0f, FENCE_OFFSET_Y,  155.0f };
			g_Fence[4].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[5].pos = { -63.0f, FENCE_OFFSET_Y,  195.0f };
			g_Fence[5].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[6].pos = { -63.0f, FENCE_OFFSET_Y,  235.0f };
			g_Fence[6].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[7].pos = { -63.0f, FENCE_OFFSET_Y,  275.0f };
			g_Fence[7].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[8].pos = { -63.0f, FENCE_OFFSET_Y,  315.0f };
			g_Fence[8].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[9].pos = { -63.0f, FENCE_OFFSET_Y,  355.0f };
			g_Fence[9].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[10].pos = { -80.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[10].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[11].pos = { -120.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[11].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[12].pos = { -160.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[12].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[13].pos = { -200.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[13].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[14].pos = { -240.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[14].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[15].pos = { -280.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[15].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[16].pos = { -320.0f, FENCE_OFFSET_Y, 377.0f };
			g_Fence[16].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[17].pos = { -345.0f, FENCE_OFFSET_Y, 355.0f };
			g_Fence[17].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[18].pos = { -345.0f, FENCE_OFFSET_Y, 315.0f };
			g_Fence[18].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[19].pos = { -345.0f, FENCE_OFFSET_Y, 275.0f };
			g_Fence[19].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[20].pos = { -345.0f, FENCE_OFFSET_Y, 235.0f };
			g_Fence[20].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[21].pos = { -345.0f, FENCE_OFFSET_Y, 195.0f };
			g_Fence[21].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[22].pos = { -345.0f, FENCE_OFFSET_Y, 155.0f };
			g_Fence[22].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[23].pos = { -345.0f, FENCE_OFFSET_Y, 115.0f };
			g_Fence[23].rot = { 0.0f, XM_PI / 2 , 0.0f };

			g_Fence[24].pos = { -320.0f, FENCE_OFFSET_Y, 100.0f };
			g_Fence[24].rot = { 0.0f, 0.0f, 0.0f };

			g_Fence[25].pos = { -280.0f, FENCE_OFFSET_Y, 100.0f };
			g_Fence[25].rot = { 0.0f, 0.0f, 0.0f };

		}

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFence(void)
{
	//-------------------------------------------------------------------------
	//　モデルの開放
	//-------------------------------------------------------------------------
	if (g_Load)
	{
		UnloadModel(&modelFence);

		g_Load = FALSE;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFence(void)
{
	//-------------------------------------------------------------------------
	// 視錐台カリング
	//-------------------------------------------------------------------------
	for (int i = 0; i < FENCE_MAX; i++)
	{
		
		g_Fence[i].draw = FrustumCulling(g_Fence[i].pos, g_Fence[i].rot, g_Fence[i].size);
		
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFence(void)
{

	for (int i = 0; i < FENCE_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Fence[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!g_Fence[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Fence[i].scl.x, g_Fence[i].scl.y, g_Fence[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Fence[i].rot.x, g_Fence[i].rot.y + XM_PI, g_Fence[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Fence[i].pos.x, g_Fence[i].pos.y, g_Fence[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Fence[i].mtxWorld, mtxWorld);

		// モデル描画
		switch (g_Fence[i].modelNo)
		{
		case 0:
			DrawModel(&modelFence);
			break;
		}

	}

}


//=============================================================================
// 柵の情報を取得
//=============================================================================
FENCE* GetFence(void)
{
	return &g_Fence[0];
}

