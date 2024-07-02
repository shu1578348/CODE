//=============================================================================
//
// ボックス処理 [box.cpp]
// Author :　荒山 秀磨 
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
static BOX	box[BOX_MAX];			 // 木箱の最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBox(void)
{

	for (int i = 0; i < BOX_MAX; i++)
	{
		LoadModel(MODEL_BOX, &box[i].model);
		box[i].load = TRUE;

		box[i].rot  = { 0.0f, 0.0f, 0.0f };
		box[i].scl  = { 0.3f, 0.3f, 0.3f };
		box[i].size = { BOX_SIZE_X, BOX_SIZE_Y, BOX_SIZE_Z };

		box[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		box[i].use  = TRUE;	// TRUE:使用 / FALSE:未使用

	}

	// 木箱の表示位置
	{
		box[0].pos = { -400.0f, 15.0f,  0.0f };
		box[0].rot = { 0.0f, 240.0f, 0.0f };
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
		if (box[i].load)
		{
			UnloadModel(&box[i].model);

			box[i].load = FALSE;
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
		box[i].draw = FrustumCulling(box[i].pos,box[i].rot, box[i].size);
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
		if (!box[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!box[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(box[i].scl.x, box[i].scl.y, box[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(box[i].rot.x, box[i].rot.y + XM_PI, box[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(box[i].pos.x, box[i].pos.y, box[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&box[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&box[i].model);

	}

}

//=============================================================================
// 屋台の情報を取得
//=============================================================================
BOX* GetBox(void)
{
	return &box[0];
}