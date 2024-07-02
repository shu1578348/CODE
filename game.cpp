//=============================================================================
//
// タイトル画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "game.h"
#include "camera.h"
#include "model.h"
#include "input.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "tree.h"
#include "particle.h"
#include "bullet.h"
#include "score.h"
#include "house.h"
#include "barrel.h"
#include "fence.h"
#include "stall.h"
#include "dungeon.h"
#include "sky.h"
#include "bomb.h"
#include "fade.h"
#include "ui.h"
#include "boss.h"
#include "enemybullet.h"
#include "speartrap.h"
#include "carriage.h"
#include "box.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
int	g_Field;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	// フィールドの初期化
	g_Field = VILLAGE;

	// 最初のモードをセット
	SetField(g_Field); // ここはSetFieldのまま

	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{

	// 終了のモードをセット
	SetField(FIELD_MAX);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{

	PLAYER* player = GetPlayer();

	ENEMY* enemy = GetEnemy();

	switch (g_Field)
	{
	case VILLAGE:
		// UIの更新処理
		UpdateUi();

		// 地面処理の更新
		UpdateMeshField();

		// プレイヤーの更新処理
		UpdatePlayer();

		// 家の更新処理
		UpdateHouse();

		// 樽の更新処理
		UpdateBarrel();

		// 柵の更新処理
		UpdateFence();

		// 屋台の更新処理
		UpdateStall();

		// 馬車の更新処理
		UpdateCarriage();

		// 木箱の更新処理
		UpdateBox();

		// ダンジョンの更新処理
		UpdateDungeon();

		// 空の更新処理
		UpdateSky();

		// 木の更新処理
		UpdateTree();

		// パーティクルの更新
		UpdateParticle();

		// 弾の更新処理
		UpdateBullet();

		// 影の更新処理
		UpdateShadow();

		// 1階層目に入るときの処理
		if (GetKeyboardTrigger(DIK_0))
		{

			SetField(DUNGEON_FIRST_FLOOR);
		}


		break;

	case DUNGEON_FIRST_FLOOR:

		// UIの更新処理
		UpdateUi();

		// 壁処理の更新
		UpdateMeshWall();

		// プレイヤーの更新処理
		UpdatePlayer();

		// エネミーの更新処理
		UpdateEnemy();

		// 爆弾の更新処理
		UpdateBomb();

		// 槍の更新処理
		//UpdateTrap();

		// パーティクルの更新
		UpdateParticle();

		// 弾の更新処理
		UpdateBullet();

		// 影の更新処理
		UpdateShadow();

		// リザルトに画面切り替え用
		if(player[0].hp <= 0)
		{

			SetFade(FADE_OUT, MODE_RESULT);
		}

		// 2階層目に入るときの処理
		if (player[0].pos.z >= 1030.0f)
		{

			SetField(DUNGEON_SECOND_FLOOR);
		}

		// 2階層目に入るときの処理
		if (GetKeyboardTrigger(DIK_0))
		{

			SetField(DUNGEON_SECOND_FLOOR);
		}


		break;

	case DUNGEON_SECOND_FLOOR:
		// UIの更新処理
		UpdateUi();

		// 壁処理の更新
		UpdateMeshWall();

		// プレイヤーの更新処理
		UpdatePlayer();

		// ボスの更新処理
		UpdateBoss();

		// パーティクルの更新
		UpdateParticle();

		// 弾の更新処理
		UpdateBullet();

		// 敵の弾の更新処理
		UpdateEnemyBullet();

		// 影の更新処理
		UpdateShadow();

		// リザルトに画面切り替え用
		if (player[0].hp <= 0)
		{

			SetFade(FADE_OUT, MODE_RESULT);
		}

		// 2階層目に入るときの処理
		if (GetKeyboardTrigger(DIK_0))
		{

			SetField(VILLAGE);
		}

		break;
	}

#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{

	switch (g_Field)
	{
	case VILLAGE:

		// 空の描画処理
		DrawSky();

		// 地面の描画処理
		DrawMeshField();

		// 木の描画処理
		DrawTree();

		// 影の描画処理
		DrawShadow();

		// 家の描画処理
		DrawHouse();

		// 樽の描画処理
		DrawBarrel();

		// 柵の描画処理
		DrawFence();

		// 屋台の描画処理
		DrawStall();

		// 馬車の描画処理
		DrawCarriage();

		// 木箱の描画処理
		DrawBox();


		// ダンジョンの描画処理
		DrawDungeon();

		// 弾の描画処理
		DrawBullet();

		// プレイヤーの描画処理
		DrawPlayer();

		// パーティクルの描画処理
		DrawParticle();

		// 2DUIの描画処理
		DrawUi();

		break;

	case DUNGEON_FIRST_FLOOR:

		// 影の描画処理
		DrawShadow();

		// 爆弾の描画処理
		DrawBomb();

		// 槍の描画処理
		//DrawTrap();

		// 弾の描画処理
		DrawBullet();

		// エネミーの描画処理
		DrawEnemy();

		// プレイヤーの描画処理
		DrawPlayer();

		// 壁の描画処理
		DrawMeshWall();

		// パーティクルの描画処理
		DrawParticle();

		// 2DUIの描画処理
		DrawUi();

		break;

	case DUNGEON_SECOND_FLOOR:
		// 影の描画処理
		DrawShadow();

		// 弾の描画処理
		DrawBullet();

		// 敵の弾の更新処理
		DrawEnemyBullet();

		// ボスの描画処理
		DrawBoss();

		// プレイヤーの描画処理
		DrawPlayer();

		// 壁の描画処理
		DrawMeshWall();

		// パーティクルの描画処理
		DrawParticle();

		// 2DUIの描画処理
		DrawUi();

		break;
	}

}

//=============================================================================
// フィールドの設定
//=============================================================================
void SetField(int field)
{
	// UIの終了処理
	UninitUi();

	// 敵の弾の処理
	UninitEnemyBullet();

	// 弾の終了処理
	UninitBullet();

	// 木の終了処理
	UninitTree();

	// パーティクルの終了処理
	UninitParticle();

	// 家の終了処理
	UninitHouse();

	// 樽の終了処理
	UninitBarrel();

	// 爆弾の終了処理
	UninitBomb();

	// 槍の終了処理
	//UninitTrap();

	// 柵の終了処理
	UninitFence();

	// 屋台の終了処理
	UninitStall();

	// 屋台の終了処理
	UninitCarriage();

	// 木箱の終了処理
	UninitBox();

	// ダンジョンの終了処理
	UninitDungeon();

	// 空の終了処理
	UninitSky();

	// ボスの終了処理
	UninitBoss();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// 影の終了処理
	UninitShadow();

	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	// 次のフィールドをセットしている
	g_Field = field;

	switch (g_Field)
	{
	case VILLAGE:
		// カメラの初期化
		InitCamera();

		// UIの初期化
		InitUi();

		// フィールドの初期化
		InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 200, 200, 20.0f, 20.0f);

		// 影の初期化処理
		InitShadow();

		// プレイヤーの初期化
		InitPlayer();

		// 家の初期化
		InitHouse();

		// 樽の初期化
		InitBarrel();

		// 柵の初期化
		InitFence();

		// 屋台の初期化
		InitStall();

		// 馬車の初期化
		InitCarriage();

		// 木箱の初期化
		InitBox();

		// ダンジョンの初期化
		InitDungeon();

		// 空の初期化
		InitSky();

		// パーティクルの初期化
		InitParticle();

		// 木を生やす
		InitTree();

		// 弾の初期化
		InitBullet();
		break;

	case DUNGEON_FIRST_FLOOR:
		// カメラの初期化
		InitCamera();

		// UIの初期化
		InitUi();

		// 壁の初期化  
		{		// 最初の通路
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-100.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 80.0f, -480.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 2, 40.0f, 40.0f);
		}

		{ // 最初の部屋
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, -80.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				 XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 12, 12, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 80.0f, -80.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 12, 12, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-300.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-20.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(60.0f, 0.0f, -80.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 12, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-260.0f, 0.0f, 160.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, -120.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 2, 40.0f, 40.0f);
		}

		{ // 2つ目の通路
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 120.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-540.0f, 0.0f, 80.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-660.0f, 80.0f, 120.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 440.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 18, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 320.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 320.0f), XMFLOAT3(XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 80.0f, 320.0f), XMFLOAT3(-XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
		}

		{ // 2つ目の部屋
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 640.0f), XMFLOAT3(XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 8, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 80.0f, 640.0f), XMFLOAT3(-XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 8, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-460.0f, 0.0f, 480.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-540.0f, 0.0f, 800.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
		}

		{ // 3つ目の通路
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, 920.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 0.0f, 920.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, 920.0f), XMFLOAT3(XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 80.0f, 920.0f), XMFLOAT3(-XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
		}

		{ // 3つ目の部屋
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 1240.0f), XMFLOAT3(XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 80.0f, 1240.0f), XMFLOAT3(-XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-500.0f, 0.0f, 1040.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-260.0f, 0.0f, 1040.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 1240.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, 1240.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-380.0f, 0.0f, 1440.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 2, 40.0f, 40.0f);
		}

		// 壁(裏側用の半透明)
		{		// 最初の通路
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-100.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 2, 2, 40.0f, 40.0f);
		}

		{ // 最初の部屋
			InitMeshWall(XMFLOAT3(-300.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-20.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(60.0f, 0.0f, -80.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 12, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-260.0f, 0.0f, 160.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 16, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, -120.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 10, 2, 40.0f, 40.0f);
		}

		{// 2目の通路
			InitMeshWall(XMFLOAT3(-540.0f, 0.0f, 80.0f), XMFLOAT3(0.0f,0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 440.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 18, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 320.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 8, 2, 40.0f, 40.0f);
		}

		{ // 2つ目の部屋
			InitMeshWall(XMFLOAT3(-460.0f, 0.0f, 480.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-540.0f, 0.0f, 800.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
		}

		{ // 3つ目の通路
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, 920.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 0.0f, 920.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
		}

		{ // 3つ目の部屋
			InitMeshWall(XMFLOAT3(-500.0f, 0.0f, 1040.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-260.0f, 0.0f, 1040.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 1240.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 10, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, 1240.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 10, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-380.0f, 0.0f, 1440.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 10, 2, 40.0f, 40.0f);		
		}

		// 影の初期化処理
		InitShadow();

		// プレイヤーの初期化
		InitPlayer();

		// エネミーの初期化
		InitEnemy();

		// 爆弾の初期化
		InitBomb();

		// 槍の初期化
		//InitTrap();

		// パーティクルの初期化
		InitParticle();

		// 弾の初期化
		InitBullet();
		break;

	case DUNGEON_SECOND_FLOOR:
		// カメラの初期化
		InitCamera();

		// UIの初期化
		InitUi();

		// 壁の初期化  
		{		// 最初の通路
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-100.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 80.0f, -480.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 2, 40.0f, 40.0f);
		}

		{ // 最初の部屋
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f,  -40.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 14, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 240.0f, -40.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 14, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-300.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(20.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 80.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 4, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(140.0f, 0.0f, -40.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 0.0f, 240.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, -40.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 6, 40.0f, 40.0f);
		}

		// 影の初期化処理
		InitShadow();

		// プレイヤーの初期化
		InitPlayer();

		// ボスの初期化
		InitBoss();

		// パーティクルの初期化
		InitParticle();

		// 弾の初期化
		InitBullet();

		// 敵の弾の初期化
		InitEnemyBullet();

		break;

	}

}

//=============================================================================
// フィールドの取得
//=============================================================================
int GetField(void)
{
	return g_Field;
}

//=============================================================================
// 視錐台カリング
//=============================================================================
BOOL FrustumCulling(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 size)
{
	CAMERA* cam = GetCamera();

	// 距離を調べる
	if (GetDistance(cam[0].pos, pos) >= CULL_FAR_Z)
	{
		return FALSE;
	}

	// バウンディングボックスを作成
	BoundingOrientedBox box;
	box.Center = pos;
	box.Extents = size;
	XMVECTOR orientation = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMStoreFloat4(&box.Orientation, orientation);

	// フラスタムを作成
	BoundingFrustum frustum;
	XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&cam[0].pos), XMLoadFloat3(&cam[0].at), XMLoadFloat3(&cam[0].up));
	XMMATRIX projection = XMMatrixPerspectiveFovLH(CULL_ANGLE, CULL_ASPECT, CULL_NEAR_Z, CULL_FAR_Z);
	BoundingFrustum::CreateFromMatrix(frustum, projection);
	frustum.Transform(frustum, XMMatrixInverse(nullptr, view));

	// フラスタムとバウンディングボックスが接触しているかを判定
	if (frustum.Intersects(box))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;

}

//=============================================================================
// フィールドの当たり判定(静的オブジェクト)の取得
//=============================================================================
BOOL FieldHit(XMFLOAT3 newPos, XMFLOAT3 oldPos)
{

	switch (GetField())
	{
	case VILLAGE:

		// 柵との当たり判定
		{
			FENCE* fence = GetFence();

			for (int n = 0; n < FENCE_MAX; ++n)
			{

				float distance = GetDistance(newPos, fence[n].pos);

				if (distance < 100.0f)
				{
					if (fence[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider(fence[n].size, fence[n].pos, fence[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}

		}

		// 家との当たり判定
		{
			HOUSE* house = GetHouse();

			for (int n = 0; n < HOUSE_MAX; ++n)
			{
				
				float distance = GetDistance(newPos, house[n].pos);

				if (distance < 100.0f)
				{

					if (house[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider(house[n].size, house[n].pos, house[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}

		}

		// 樽との当たり判定
		{
			BARREL* barrel = GetBarrel();
			for (int m = 0; m < BARREL_MAX; m++)
			{
				if (barrel[m].use == TRUE)
				{
					if (CollisionBC(newPos, barrel[m].pos, 10.0f, BARREL_SIZE)) 
					{
						return TRUE;
					}
				}
			}
		}

		// 屋台との当たり判定
		{
			STALL* stall = GetStall();

			for (int n = 0; n < STALL_MAX; ++n)
			{

				float distance = GetDistance(newPos, stall[n].pos);

				if (distance < 100.0f)
				{

					if (stall[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider(stall[n].size, stall[n].pos, stall[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}
		}

		// 馬車との当たり判定
		{
			CARRIAGE* carriage = GetCarriage();

			for (int n = 0; n < CARRIAGE_MAX; ++n)
			{

				float distance = GetDistance(newPos, carriage[n].pos);

				if (distance < 100.0f)
				{

					if (carriage[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider(carriage[n].size, carriage[n].pos, carriage[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}
		}

		// ダンジョンとの当たり判定
		{
			DUNGEON* dungeon = GetDungeon();

			for (int n = 0; n < DUNGEON_MAX; ++n)
			{

				float distance = GetDistance(newPos, dungeon[n].pos);

				if (distance < 100.0f)
				{

					if (dungeon[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider( dungeon[n].size, dungeon[n].pos, dungeon[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}
		}

		break;

	case DUNGEON_FIRST_FLOOR:

		if (newPos.z <= -635.0f)
		{
			return TRUE;
		}

		if (((newPos.z >= -640.0f) && (newPos.z <= -315.0f)) && (newPos.x >= -110.0f))
		{
			return TRUE;
		}

		if (((newPos.z >= -640.0f) && (newPos.z <= -315.0f)) && (newPos.x <= -173.0f))
		{
			return TRUE;
		}

		if (newPos.x >= 54.0f)
		{
			return TRUE;
		}

		if ((newPos.z <= 85) && (newPos.x <= -415.0f))
		{
			return TRUE;
		}

		if (((newPos.z >= 149.0f) && (newPos.z <= 488.0f)) && (newPos.x >= -590.0f))
		{
			return TRUE;
		}

		if (newPos.x <= -649.0f)
		{
			return TRUE;
		}

		if (((newPos.z >= 781.0f) && (newPos.z <= 1050.0f)) && (newPos.x <= -411.0f))
		{
			return TRUE;
		}

		if (((newPos.z >= 480.0f) && (newPos.z <= 1050.0f)) && (newPos.x >= -350.0f))
		{
			return TRUE;
		}

		if ((newPos.z >= 1025.0f) && (newPos.x <= -570.0f))
		{
			return TRUE;
		}

		if ((newPos.z >= 1025.0f) && (newPos.x >= -190.0f))
		{
			return TRUE;
		}

		if ((newPos.z >= 1430.0f))
		{
			return TRUE;
		}

		break;

	case DUNGEON_SECOND_FLOOR:

		if (newPos.z <= -635.0f)
		{
			return TRUE;
		}

		if (((newPos.z >= -640.0f) && (newPos.z <= -315.0f)) && (newPos.x >= -110.0f))
		{
			return TRUE;
		}

		if (((newPos.z >= -640.0f) && (newPos.z <= -315.0f)) && (newPos.x <= -173.0f))
		{
			return TRUE;
		}

		if (newPos.x <= -410.0f)
		{
			return TRUE;
		}

		if (newPos.x >= 125.0f)
		{
			return TRUE;
		}

		if (newPos.z >= 230.0f)
		{
			return TRUE;
		}

		break;
	}

	return FALSE;

}