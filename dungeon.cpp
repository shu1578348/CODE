//=============================================================================
//
// モデル処理 [dungeon.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "dungeon.h"
#include "player.h"
#include "game.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_DUNGEON		"data/MODEL/dungeon.obj"			// 読み込むモデル名

#define DUNGEON_SHADOW_SIZE	  (1.0f)						// 影の大きさ
#define DUNGEON_OFFSET_Y      (0.0f)						// ダンジョンの下をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static DUNGEON	g_Dungeon[DUNGEON_MAX];			 // ダンジョンの最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDungeon(void)
{

	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		LoadModel(MODEL_DUNGEON, &g_Dungeon[i].model);
		g_Dungeon[i].load = TRUE;

		g_Dungeon[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Dungeon[i].scl = { 1.0f, 1.0f, 1.0f };

		g_Dungeon[i].size = { DUNGEON_SIZE_X, DUNGEON_SIZE_Y, DUNGEON_SIZE_Z };

		g_Dungeon[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		g_Dungeon[i].use  = TRUE;		// TRUE:使用 / FALSE:未使用

	}

	// ダンジョンの表示位置
	{
		g_Dungeon[0].pos = { -220.0f, DUNGEON_OFFSET_Y,  -200.0f };
		g_Dungeon[0].rot = { 0.0f, XM_PI, 0.0f };
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDungeon(void)
{
	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		// モデルの解放処理
		if (g_Dungeon[i].load)
		{
			UnloadModel(&g_Dungeon[i].model);

			g_Dungeon[i].load = FALSE;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDungeon(void)
{
	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		//-------------------------------------------------------------------------
		// 視錐台カリング処理
		//-------------------------------------------------------------------------
		g_Dungeon[i].draw = FrustumCulling(g_Dungeon[i].pos, g_Dungeon[i].rot, g_Dungeon[i].size);

		//-------------------------------------------------------------------------
		// フィールドの変更
		//-------------------------------------------------------------------------
		PLAYER* player = GetPlayer();

		XMFLOAT3 pos = g_Dungeon[i].pos;

		pos.x -= sinf(g_Dungeon[i].rot.y) * 30.0f;
		pos.z -= cosf(g_Dungeon[i].rot.y) * 30.0f;

		if (CollisionBC(pos, player[0].pos, 30.0f, player[0].size))
		{
			// SetField(DUNGEON_FIRST_FLOOR);
			SetField(DUNGEON_FIRST_FLOOR);
		}

	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDungeon(void)
{

	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Dungeon[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!g_Dungeon[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Dungeon[i].scl.x, g_Dungeon[i].scl.y, g_Dungeon[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Dungeon[i].rot.x, g_Dungeon[i].rot.y + XM_PI, g_Dungeon[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Dungeon[i].pos.x, g_Dungeon[i].pos.y, g_Dungeon[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Dungeon[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Dungeon[i].model);

	}

}

//=============================================================================
// ダンジョンの情報を取得
//=============================================================================
DUNGEON* GetDungeon(void)
{
	return &g_Dungeon[0];
}