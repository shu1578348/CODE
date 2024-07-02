//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "enemy.h"
#include "shadow.h"
#include "player.h"
#include "camera.h"
#include "collision.h"
#include "bullet.h"
#include "particle.h"
#include "fade.h"
#include "score.h"
#include "game.h"
#include "bomb.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/enemy.obj"		// 読み込むモデル名

#define ENEMY_HP			(500.0f)					// エネミーのHP

#define	VALUE_MOVE			(1.2f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.1f)				// 回転量
#define ENEMY_SHADOW_SIZE	(0.8f)						// 影の大きさ
#define ENEMY_OFFSET_Y		(2.0f)						// エネミーの足元をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY g_Enemy[ENEMY_MAX]; // エネミー構造体
static BOOL	  g_Load = FALSE;	 // 初期化を行ったかのフラグ
DX11_MODEL	  modelEnemy;		 // モデル情報

//----------------------------------------------------------------------------
// 線形補間
//----------------------------------------------------------------------------
static INTERPOLATION_DATA g_MoveTbl0[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-220.0f, ENEMY_OFFSET_Y,  0.0f),	 XMFLOAT3(0.0f,  1.57f, 0.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3( -20.0f, ENEMY_OFFSET_Y,  0.0f),	 XMFLOAT3(0.0f, -1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA g_MoveTbl1[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3( -20.0f, ENEMY_OFFSET_Y,  -100.0f), XMFLOAT3(0.0f, -1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(-220.0f, ENEMY_OFFSET_Y,  -100.0f), XMFLOAT3(0.0f,  1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
};

static INTERPOLATION_DATA g_MoveTbl2[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y,    0.0f), XMFLOAT3(0.0f, 3.14f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -300.0f), XMFLOAT3(0.0f, 0.0f,  0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA g_MoveTbl3[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
};

static INTERPOLATION_DATA g_MoveTbl4[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA g_MoveTbl5[] = {
	//座標													回転率						拡大率			時間

	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA g_MoveTbl6[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,
	g_MoveTbl3,
	g_MoveTbl4,
	g_MoveTbl5,
	g_MoveTbl6,
};

int atClTime = 0;	// 攻撃間隔

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	//-------------------------------------------------------------------------
	// 情報の初期化
	//-------------------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].pos = XMFLOAT3(175.0f, ENEMY_OFFSET_Y, -30.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.5f, 1.5f, 1.5f);

		g_Enemy[i].tPos = XMFLOAT3(175.0f, ENEMY_OFFSET_Y, -30.0f);

		g_Enemy[i].spd  = 0.0f;			// 移動スピードクリア
		g_Enemy[i].size = {ENEMY_SIZE_X, ENEMY_SIZE_Y, ENEMY_SIZE_Z};	// 当たり判定の大きさ
		
		// ダンジョン(1階)なら
		if (GetField() == DUNGEON_FIRST_FLOOR)
		{
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		}

		g_Enemy[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		g_Enemy[i].use  = TRUE;		// TRUE:使用 / FALSE:未使用
		g_Enemy[i].chase = FALSE;    // TRUE:追跡中 / FALSE:非追跡中

		g_Enemy[i].condition = 0;	// 通常状態にする
		
		g_Enemy[i].hp = ENEMY_HP; // ヒットポイントの初期化

		g_Enemy[i].state = FALSE;   // TRUE:移動可能 / FALSE:移動不可
		
		g_Enemy[i].time  = 0.0f;	// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = 0;		// 再生する行動データテーブルNoをセット
		g_Enemy[i].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	}
	//-------------------------------------------------------------------------
	// 初期位置の設定
	//-------------------------------------------------------------------------
	{
		g_Enemy[0].pos = XMFLOAT3(-220.0f, ENEMY_OFFSET_Y, -0.0f);
		g_Enemy[1].pos = XMFLOAT3(-120.0f, ENEMY_OFFSET_Y, -100.0f);
		g_Enemy[2].pos = XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, 0.0f);
		g_Enemy[3].pos = XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f);
		g_Enemy[4].pos = XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f);
		g_Enemy[5].pos = XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f);
		g_Enemy[6].pos = XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f);
	}

	//-------------------------------------------------------------------------
	// 行動テーブルのセット
	//-------------------------------------------------------------------------
	{
		g_Enemy[0].tblNo = 0;		// 再生する行動データテーブルNoをセット
		g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		g_Enemy[1].tblNo = 1;		// 再生する行動データテーブルNoをセット
		g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		g_Enemy[2].tblNo = 2;		// 再生する行動データテーブルNoをセット
		g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		g_Enemy[3].tblNo = 3;		// 再生する行動データテーブルNoをセット
		g_Enemy[3].tblMax = sizeof(g_MoveTbl3) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		g_Enemy[4].tblNo = 4;		// 再生する行動データテーブルNoをセット
		g_Enemy[4].tblMax = sizeof(g_MoveTbl4) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		g_Enemy[5].tblNo = 5;		// 再生する行動データテーブルNoをセット
		g_Enemy[5].tblMax = sizeof(g_MoveTbl5) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		g_Enemy[6].tblNo = 6;		// 再生する行動データテーブルNoをセット
		g_Enemy[6].tblMax = sizeof(g_MoveTbl6) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	}

	//-------------------------------------------------------------------------
	// モデル読み込み
	//-------------------------------------------------------------------------
	{
		g_Load = TRUE;

		// モデルの読み込み
		LoadModel(MODEL_ENEMY, &modelEnemy);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load)
	{
		UnloadModel(&modelEnemy);
		g_Load = FALSE;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	//-------------------------------------------------------------------------
	// OBJ情報の取得
	//-------------------------------------------------------------------------
	PLAYER* player = GetPlayer(); // プレイヤー
	BULLET* bullet = GetBullet(); // バレット
	CAMERA* cam = GetCamera();    // カメラ

	for (int i = 0; i < ENEMY_MAX; i++)
	{

		if (g_Enemy[i].use)	  // 使用判定
		{
			// 移動前の座標を保存
			XMFLOAT3 oldPos = g_Enemy[i].pos;
						
			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);

			//-------------------------------------------------------------------------
			// 視錐台カリング
			//-------------------------------------------------------------------------
			{
				g_Enemy[i].draw = FrustumCulling(g_Enemy[i].pos, g_Enemy[i].rot, g_Enemy[i].size);
			}

			//-------------------------------------------------------------------------
			// 移動処理
			//-------------------------------------------------------------------------
			{

				float distance = GetDistance(player[0].pos, g_Enemy[i].pos);

				// 近づいたら追跡する
				if (distance < 100)
				{
					g_Enemy[i].chase = TRUE;
				}
				else
				{
					g_Enemy[i].chase = FALSE;
				}

				// ダメージを受けたらプレイヤーを追跡する
				if (g_Enemy[i].hp != ENEMY_HP)
				{
					g_Enemy[i].chase = TRUE;
				}

				if (g_Enemy[i].chase)
				{
					g_Enemy[i].tPos = player[0].pos;
				}

				if (!g_Enemy[i].chase)
				{
					// 線形補間の処理
					{
						int nowNo = (int)g_Enemy[i].time;			// 整数分であるテーブル番号を取り出している
						int maxNo = g_Enemy[i].tblMax;				// 登録テーブル数を数えている
						int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
						INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// 行動テーブルのアドレスを取得

						XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換

						XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
						float nowTime = g_Enemy[i].time - nowNo;	// 時間部分である少数を取り出している

						Pos *= nowTime;		// 現在の移動量を計算している

						// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
						XMStoreFloat3(&g_Enemy[i].tPos, nowPos + Pos);

						// frameを使て時間経過処理をする
						g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
						if ((int)g_Enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
						{
							g_Enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
						}
					}
				}

				// 敵の移動速度
				g_Enemy[i].spd = VALUE_MOVE;

				XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);
				XMVECTOR vec = XMLoadFloat3(&g_Enemy[i].tPos) - epos;                   //エネミーとプレイヤーの差分を求めている

				// 目標の方向
				const float targetDir = static_cast<float>(atan2(vec.m128_f32[0], vec.m128_f32[2]) - XM_PI);;

				// 現在の方向から目標の方向への角度差を求める
				float angleDifference = targetDir - g_Enemy[i].rot.y;

				// 角度差を正確な範囲に調整
				while (angleDifference > XM_PI)
				{
					angleDifference -= 2 * XM_PI;
				}

				while (angleDifference < -XM_PI)
				{
					angleDifference += 2 * XM_PI;
				}

				// 補間を使用してg_Player.dirを目標の方向に近づける
				g_Enemy[i].rot.y = Lerp(g_Enemy[i].rot.y, g_Enemy[i].rot.y + angleDifference, VALUE_ROTATE);

				// プレイヤーの方に移動
				g_Enemy[i].pos.x -= sinf(g_Enemy[i].rot.y) * g_Enemy[i].spd;
				g_Enemy[i].pos.z -= cosf(g_Enemy[i].rot.y) * g_Enemy[i].spd;

				g_Enemy[i].spd *= 0.0f;

			}
			//-------------------------------------------------------------------------
			// 当たり判定処理
			//-------------------------------------------------------------------------
			{

				atClTime++;

				// プレイヤーとの当たり判定(攻撃)
				for (int j = 0; j < PLAYER_MAX; j++)
				{

					if (player[j].use)
					{
						if (CollisionBC(g_Enemy[i].pos, player[j].pos, ENEMY_SIZE, player[j].size))
						{

							if (atClTime >= 120)
							{
								player[j].hp -= 10;

								atClTime = 0;
							}

							g_Enemy[i].pos = oldPos;

							break;
						}
					}
				}

				// 爆弾との当たり判定
				{
					BOMB* bomb = GetBomb();
					for (int k = 0; k < BOMB_MAX; k++)
					{
						if (bomb[k].use == TRUE)
						{
							BOOL ans = CollisionBC(g_Enemy[k].pos, bomb[k].pos, 10.0f, 10.0f);

							if (ans == TRUE)
							{
								// ポジションをもとに戻す
								g_Enemy[k].pos = oldPos;

								break;
							}
						}
					}
				}

				// フィールドとの当たり判定
				if (FieldHit(g_Enemy[i].pos, oldPos))
				{
					g_Enemy[i].pos = oldPos;
				}

			}

			//-------------------------------------------------------------------------
			// 状態異常 / エフェクト処理(パーティクル)
			//-------------------------------------------------------------------------
			{

				// 描画範囲内かどうか
				if (g_Enemy[i].draw)
				{
					// 状態異常
					switch (g_Enemy[i].condition)
					{
					case 0: // 通常
						break;

					case 1: // 炎上

						// HPを減らす
						g_Enemy[i].hp -= 1.0f;

						XMFLOAT3 pos;
						XMFLOAT3 move;
						XMFLOAT3 scale;
						float fAngle, fLength;
						int nLife;
						float fSize;

						pos = g_Enemy[i].pos;

						pos.y += 6.0f;

						scale = { 0.2f, 0.2f, 0.2f };

						fAngle = (float)(rand() % 628 - 314) / 100;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 40;

						move.x = sinf(fAngle) * fLength;
						move.y = (rand() % 300 / 100.0f + 10) / 50;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 50;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);

						break;

					case 2: // 毒

						// HPを減らす
						g_Enemy[i].hp -= 2.0f;

						pos = g_Enemy[i].pos;

						pos.y += 12.0f;

						scale = { 0.3f, 0.3f, 0.3f };

						move.x = (300.0f - (rand() % 600)) / 800;
						move.y = (300.0f - (rand() % 600)) / 800;
						move.z = (300.0f - (rand() % 600)) / 800;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 20);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 0.8f), fSize, fSize, nLife, 1);

						break;
					}

				}

			}


			//-------------------------------------------------------------------------
			// 死亡判定処理
			//-------------------------------------------------------------------------
			{
				if (g_Enemy[i].hp <= 0)
				{
					AddScore(100);

					g_Enemy[i].use = FALSE;
				}
			}
		
		}

	}

#ifdef _DEBUG

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Enemy[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!g_Enemy[i].draw) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		switch (g_Enemy[i].modelNo)
		{
		case 0:
			DrawModel(&modelEnemy);
			break;
		}

	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY* GetEnemy()
{
	return &g_Enemy[0];
}