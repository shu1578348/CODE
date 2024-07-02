//=============================================================================
//
// モデル処理 [fence.cpp]
// Author : 荒山　秀磨
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
static FENCE fence[FENCE_MAX]; // 柵の最大数
static BOOL	load = FALSE;		 // モデルの初期化フラグ
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
		fence[i].rot  = { 0.0f, 0.0f, 0.0f };
		fence[i].scl  = { 0.8f, 0.5f, 0.8f };
		fence[i].size = { FENCE_SIZE_X, FENCE_SIZE_Y, FENCE_SIZE_Z };

		fence[i].modelNo = 0;

		fence[i].draw = TRUE;	// TRUE:表示 / FALSE:非表示
		fence[i].use  = TRUE;	// TRUE:使用 / FALSE:未使用
	}

	//-------------------------------------------------------------------------
	//　モデルの読み込み
	//-------------------------------------------------------------------------
	{
		load = TRUE;

		// モデルの読み込み
		LoadModel(MODEL_FENCE, &modelFence);

	}

	//-------------------------------------------------------------------------
	//　位置設定
	//-------------------------------------------------------------------------
	{
		// house[0]の周囲
		{

			fence[0].pos = { -160.0f, FENCE_OFFSET_Y, 100.0f };
			fence[0].rot = { 0.0f, 0.0f, 0.0f };

			fence[1].pos = { -120.0f, FENCE_OFFSET_Y, 100.0f };
			fence[1].rot = { 0.0f, 0.0f, 0.0f };

			fence[2].pos = { -80.0f, FENCE_OFFSET_Y,  100.0f };
			fence[2].rot = { 0.0f, 0.0f, 0.0f };

			fence[3].pos = { -63.0f, FENCE_OFFSET_Y,  115.0f };
			fence[3].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[4].pos = { -63.0f, FENCE_OFFSET_Y,  155.0f };
			fence[4].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[5].pos = { -63.0f, FENCE_OFFSET_Y,  195.0f };
			fence[5].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[6].pos = { -63.0f, FENCE_OFFSET_Y,  235.0f };
			fence[6].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[7].pos = { -63.0f, FENCE_OFFSET_Y,  275.0f };
			fence[7].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[8].pos = { -63.0f, FENCE_OFFSET_Y,  315.0f };
			fence[8].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[9].pos = { -63.0f, FENCE_OFFSET_Y,  355.0f };
			fence[9].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[10].pos = { -80.0f, FENCE_OFFSET_Y, 377.0f };
			fence[10].rot = { 0.0f, 0.0f, 0.0f };

			fence[11].pos = { -120.0f, FENCE_OFFSET_Y, 377.0f };
			fence[11].rot = { 0.0f, 0.0f, 0.0f };

			fence[12].pos = { -160.0f, FENCE_OFFSET_Y, 377.0f };
			fence[12].rot = { 0.0f, 0.0f, 0.0f };

			fence[13].pos = { -200.0f, FENCE_OFFSET_Y, 377.0f };
			fence[13].rot = { 0.0f, 0.0f, 0.0f };

			fence[14].pos = { -240.0f, FENCE_OFFSET_Y, 377.0f };
			fence[14].rot = { 0.0f, 0.0f, 0.0f };

			fence[15].pos = { -280.0f, FENCE_OFFSET_Y, 377.0f };
			fence[15].rot = { 0.0f, 0.0f, 0.0f };

			fence[16].pos = { -320.0f, FENCE_OFFSET_Y, 377.0f };
			fence[16].rot = { 0.0f, 0.0f, 0.0f };

			fence[17].pos = { -345.0f, FENCE_OFFSET_Y, 355.0f };
			fence[17].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[18].pos = { -345.0f, FENCE_OFFSET_Y, 315.0f };
			fence[18].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[19].pos = { -345.0f, FENCE_OFFSET_Y, 275.0f };
			fence[19].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[20].pos = { -345.0f, FENCE_OFFSET_Y, 235.0f };
			fence[20].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[21].pos = { -345.0f, FENCE_OFFSET_Y, 195.0f };
			fence[21].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[22].pos = { -345.0f, FENCE_OFFSET_Y, 155.0f };
			fence[22].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[23].pos = { -345.0f, FENCE_OFFSET_Y, 115.0f };
			fence[23].rot = { 0.0f, XM_PI / 2 , 0.0f };

			fence[24].pos = { -320.0f, FENCE_OFFSET_Y, 100.0f };
			fence[24].rot = { 0.0f, 0.0f, 0.0f };

			fence[25].pos = { -280.0f, FENCE_OFFSET_Y, 100.0f };
			fence[25].rot = { 0.0f, 0.0f, 0.0f };

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
	if (load)
	{
		UnloadModel(&modelFence);

		load = FALSE;
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
		
		fence[i].draw = FrustumCulling(fence[i].pos, fence[i].rot, fence[i].size);
		
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
		if (!fence[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!fence[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(fence[i].scl.x, fence[i].scl.y, fence[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(fence[i].rot.x, fence[i].rot.y + XM_PI, fence[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(fence[i].pos.x, fence[i].pos.y, fence[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&fence[i].mtxWorld, mtxWorld);

		// モデル描画
		switch (fence[i].modelNo)
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
	return &fence[0];
}

