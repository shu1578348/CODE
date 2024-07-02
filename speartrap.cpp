//=============================================================================
//
// モデル処理 [speartrap.cpp]
// Author : 荒山 秀磨
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "speartrap.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_SPEAR        "data/MODEL/spear_trap.obj"		// 読み込むモデル名

#define TRAP_SHADOW_SIZE	(0.4f)	// 影の大きさ
#define TRAP_OFFSET_Y		(0.0f)	// 槍の下をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static TRAP g_Trap[TRAP_MAX];	 // 罠の最大数
static BOOL	g_Load = FALSE;		 // モデルの初期化フラグ
DX11_MODEL modelSpear;			 // モデル情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTrap(void)
{
	//-------------------------------------------------------------------------
	// 情報の初期化
	//-------------------------------------------------------------------------
	for (int i = 0; i < TRAP_MAX; i++)
	{
		g_Trap[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Trap[i].scl = { 1.0f, 1.0f, 1.0f };

		g_Trap[i].time = 0;

		g_Trap[i].modelNo = 0; // 表示モデルの種類

		g_Trap[i].size = TRAP_SIZE;  // 当たり判定の大きさ

		g_Trap[i].draw = TRUE;  // TRUE:表示 / FALSE:非表示
		g_Trap[i].use  = TRUE;  // TRUE:使用 / FALSE:未使用
		g_Trap[i].status = 0;   // 状態
	}

	//-------------------------------------------------------------------------
	// モデルの読み込み
	//-------------------------------------------------------------------------
	{
		g_Load = TRUE;
		
		// モデルの読み込み
		LoadModel(MODEL_SPEAR,  &modelSpear);
	}

	//-------------------------------------------------------------------------
	// 位置の設定
	//-------------------------------------------------------------------------
	{
		g_Trap[0].pos  = { -240.0f, 30.0f, -380.0f };
		g_Trap[0].rot  = { 0.0f, 0.0f, XM_PI / 2};
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTrap(void)
{
	if (g_Load)
	{
		UnloadModel(&modelSpear);
		g_Load = FALSE;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTrap(void)
{		
	for (int i = 0; i < TRAP_MAX; i++)
	{ 
		switch (g_Trap[i].status)
		{
		case 0:
			break;
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTrap(void)
{
	for (int i = 0; i < TRAP_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Trap[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!g_Trap[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Trap[i].scl.x, g_Trap[i].scl.y, g_Trap[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Trap[i].rot.x, g_Trap[i].rot.y + XM_PI, g_Trap[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Trap[i].pos.x, g_Trap[i].pos.y, g_Trap[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Trap[i].mtxWorld, mtxWorld);

		// モデルの描画
		switch (g_Trap[i].modelNo)
		{
		case 0:
			DrawModel(&modelSpear);
			break;
		}

	}
}

//=============================================================================
// 樽の情報を取得
//=============================================================================
TRAP* GetTrap(void)
{
	return &g_Trap[0];
}