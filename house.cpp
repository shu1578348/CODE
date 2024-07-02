//=============================================================================
//
// モデル処理 [house.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "house.h"
#include "player.h"
#include "particle.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 読み込むモデル名
#define	HOUSE_HIGH			"data/MODEL/house_high.obj"	
#define	HOUSE_MIDDLE		"data/MODEL/house_middle.obj"	
#define HOUSE_LOW			"data/MODEL/house_low.obj"

#define HOUSE_SHADOW_SIZE	(0.4f)	// 影の大きさ
#define HOUSE_OFFSET_Y		(0.0f)	// 家の下をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static HOUSE g_House[HOUSE_MAX]; // 家の最大数

DX11_MODEL modelHigh;    // 高いモデル情報
DX11_MODEL modelMiddle;  // 中程度のモデル情報
DX11_MODEL modelLow;     // 低いモデル情報

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHouse(void)
{
	//-------------------------------------------------------------------------
	//　情報の初期化
	//-------------------------------------------------------------------------
	for (int i = 0; i < HOUSE_MAX; i++)
	{

		g_House[i].pos = { -200.0f, HOUSE_OFFSET_Y, 300.0f };

		g_House[i].rot = { 0.0f, 0.0f, 0.0f };
		g_House[i].scl = { 2.0f, 2.0f, 2.0f };

		g_House[i].size = { HOUSE_SIZE_X, HOUSE_SIZE_Y, HOUSE_SIZE_Z };

		g_House[i].draw = TRUE;	// TRUE:表示 / FALSE:非表示
		g_House[i].use  = TRUE;	// TRUE:使用 / FALSE:未使用

		g_House[i].modelNo = 0;

	}

	//-------------------------------------------------------------------------
	//　モデルの読み込み
	//-------------------------------------------------------------------------
	{
		LoadModel(HOUSE_HIGH, &modelHigh);
		LoadModel(HOUSE_MIDDLE, &modelMiddle);
		LoadModel(HOUSE_LOW, &modelLow);
	}

	//-------------------------------------------------------------------------
	//　位置に設定
	//-------------------------------------------------------------------------
	{
		g_House[0].pos = { -200.0f, HOUSE_OFFSET_Y, 300.0f };
		g_House[1].pos = {    0.0f, HOUSE_OFFSET_Y, 150.0f };
		g_House[2].pos = {  200.0f, HOUSE_OFFSET_Y, 0.0f   };
	}

	g_Load = TRUE;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHouse(void)
{
	//-------------------------------------------------------------------------
	// モデルの解放処理
	//-------------------------------------------------------------------------
	if (g_Load)
	{
		UnloadModel(&modelHigh);
		UnloadModel(&modelMiddle);
		UnloadModel(&modelLow);

		g_Load = FALSE;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHouse(void)
{
	//-------------------------------------------------------------------------
	// OBJ情報の取得
	//-------------------------------------------------------------------------
	PLAYER* player = GetPlayer();

	//-------------------------------------------------------------------------
	// 表示/非表示の処理
	//-------------------------------------------------------------------------
	for (int i = 0; i < HOUSE_MAX; i++)
	{

		float distance = GetDistance(player[0].pos, g_House[i].pos);

		//-----------------------------------------------------------------------
		// LOD処理
		//-----------------------------------------------------------------------
		if (distance < DRAW_RANGE - 400)
		{
			g_House[i].modelNo = 0;
		}
		else if (distance < DRAW_RANGE - 200)
		{
			g_House[i].modelNo = 1;
		}
		else if (distance > DRAW_RANGE)
		{
			g_House[i].modelNo = 2;
		}

		//-------------------------------------------------------------------------
		// 視錐型カリング処理
		//-------------------------------------------------------------------------		
		g_House[i].draw = FrustumCulling(g_House[i].pos, g_House[i].rot, g_House[i].size);
		
		
		//-------------------------------------------------------------------------
		// エフェクトの処理(パーティクル)
		//-------------------------------------------------------------------------
		if (g_House[i].draw)
		{
			if (rand() % 1 == 0)
			{
				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				float fAngle, fLength;
				int nLife;
				float fSize;

				pos = g_House[i].pos;

				pos.y = 150.0f;

				pos.x -= 30.0f;

				pos.z += 20.0f;

				scale = { 0.6f, 0.6f, 0.6f };

				fAngle = (float)(rand() % 628 - 314) / 150;
				fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 80;

				move.x = sinf(fAngle) * fLength;
				move.y = 1.2f;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 300 + 80;

				fSize = (float)(rand() % 30 + 20);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(0.7f, 0.7f, 0.7f, 0.2f), fSize, fSize, nLife, 0);
			}

		}

	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHouse(void)
{

	for (int i = 0; i < HOUSE_MAX; i++)
	{

		if (!g_House[i].use)  continue;

		if (!g_House[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_House[i].scl.x, g_House[i].scl.y, g_House[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_House[i].rot.x, g_House[i].rot.y + XM_PI, g_House[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_House[i].pos.x, g_House[i].pos.y, g_House[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_House[i].mtxWorld, mtxWorld);

		// モデル描画
		
		// LOD処理
		switch (g_House[i].modelNo)
		{
		case 0:
			DrawModel(&modelHigh);
			break;

		case 1:
			DrawModel(&modelMiddle);
			break;

		case 2:
			DrawModel(&modelLow);
			break;
		}
		
	}

}

//=============================================================================
// 家情報を取得
//=============================================================================
HOUSE* GetHouse(void)
{
	return &g_House[0];
}

