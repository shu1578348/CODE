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
static DUNGEON	dungeon[DUNGEON_MAX];			 // ダンジョンの最大数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDungeon(void)
{

	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		LoadModel(MODEL_DUNGEON, &dungeon[i].model);
		dungeon[i].load = TRUE;

		dungeon[i].rot = { 0.0f, 0.0f, 0.0f };
		dungeon[i].scl = { 1.0f, 1.0f, 1.0f };

		dungeon[i].size = { DUNGEON_SIZE_X, DUNGEON_SIZE_Y, DUNGEON_SIZE_Z };

		dungeon[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		dungeon[i].use  = TRUE;		// TRUE:使用 / FALSE:未使用

	}

	// ダンジョンの表示位置
	{
		dungeon[0].pos = { -220.0f, DUNGEON_OFFSET_Y,  -200.0f };
		dungeon[0].rot = { 0.0f, XM_PI, 0.0f };
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
		if (dungeon[i].load)
		{
			UnloadModel(&dungeon[i].model);

			dungeon[i].load = FALSE;
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
		dungeon[i].draw = FrustumCulling(dungeon[i].pos, dungeon[i].rot, dungeon[i].size);

		//-------------------------------------------------------------------------
		// フィールドの変更
		//-------------------------------------------------------------------------
		PLAYER* player = GetPlayer();

		XMFLOAT3 pos = dungeon[i].pos;

		pos.x -= sinf(dungeon[i].rot.y) * 30.0f;
		pos.z -= cosf(dungeon[i].rot.y) * 30.0f;

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
		if (!dungeon[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!dungeon[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(dungeon[i].scl.x, dungeon[i].scl.y, dungeon[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(dungeon[i].rot.x, dungeon[i].rot.y + XM_PI, dungeon[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(dungeon[i].pos.x, dungeon[i].pos.y, dungeon[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&dungeon[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&dungeon[i].model);

	}

}

//=============================================================================
// ダンジョンの情報を取得
//=============================================================================
DUNGEON* GetDungeon(void)
{
	return &dungeon[0];
}