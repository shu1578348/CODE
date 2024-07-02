//=============================================================================
//
// ボックス処理 [box.cpp]
// Author :　荒山秀磨 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "box.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BOX		"data/MODEL/box.obj"	// 読み込むモデル名

#define BOX_SHADOW_SIZE		(1.0f)				// 影の大きさ
#define BOX_OFFSET_Y		(0.0f)				// 木箱の下をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BOX	g_Box[BOX_MAX];			 // 木箱の最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBox(void)
{

	for (int i = 0; i < BOX_MAX; i++)
	{
		LoadModel(MODEL_BOX, &g_Box[i].model);
		g_Box[i].load = TRUE;

		g_Box[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Box[i].scl  = { 0.3f, 0.3f, 0.3f };
		g_Box[i].size = { BOX_SIZE_X, BOX_SIZE_Y, BOX_SIZE_Z };

		g_Box[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		g_Box[i].use  = TRUE;	// TRUE:使用 / FALSE:未使用

	}

	// 木箱の表示位置
	{
		g_Box[0].pos = { -400.0f, 15.0f,  0.0f };
		g_Box[0].rot = { 0.0f, 240.0f, 0.0f };
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBox(void)
{
	for (int i = 0; i < BOX_MAX; i++)
	{
		// モデルの解放処理
		if (g_Box[i].load)
		{
			UnloadModel(&g_Box[i].model);

			g_Box[i].load = FALSE;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBox(void)
{
	//-------------------------------------------------------------------------
	// 視錐台カリング処理
	//-------------------------------------------------------------------------
	for (int i = 0; i < BOX_MAX; i++)
	{
		g_Box[i].draw = FrustumCulling(g_Box[i].pos,g_Box[i].rot, g_Box[i].size);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBox(void)
{

	for (int i = 0; i < BOX_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Box[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!g_Box[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Box[i].scl.x, g_Box[i].scl.y, g_Box[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Box[i].rot.x, g_Box[i].rot.y + XM_PI, g_Box[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Box[i].pos.x, g_Box[i].pos.y, g_Box[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Box[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Box[i].model);

	}

}

//=============================================================================
// 屋台の情報を取得
//=============================================================================
BOX* GetBox(void)
{
	return &g_Box[0];
}