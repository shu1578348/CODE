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
static ENEMY enemy[ENEMY_MAX]; // エネミー構造体
static BOOL	  load = FALSE;	 // 初期化を行ったかのフラグ
DX11_MODEL	  modelEnemy;		 // モデル情報

//----------------------------------------------------------------------------
// 線形補間
//----------------------------------------------------------------------------
static INTERPOLATION_DATA moveTbl0[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-220.0f, ENEMY_OFFSET_Y,  0.0f),	 XMFLOAT3(0.0f,  1.57f, 0.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3( -20.0f, ENEMY_OFFSET_Y,  0.0f),	 XMFLOAT3(0.0f, -1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA moveTbl1[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3( -20.0f, ENEMY_OFFSET_Y,  -100.0f), XMFLOAT3(0.0f, -1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(-220.0f, ENEMY_OFFSET_Y,  -100.0f), XMFLOAT3(0.0f,  1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
};

static INTERPOLATION_DATA moveTbl2[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y,    0.0f), XMFLOAT3(0.0f, 3.14f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -300.0f), XMFLOAT3(0.0f, 0.0f,  0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA moveTbl3[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
};

static INTERPOLATION_DATA moveTbl4[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA moveTbl5[] = {
	//座標													回転率						拡大率			時間

	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA moveTbl6[] = {
	//座標													回転率						拡大率			時間
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA* moveTblAdr[] =
{
	moveTbl0,
	moveTbl1,
	moveTbl2,
	moveTbl3,
	moveTbl4,
	moveTbl5,
	moveTbl6,
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
		enemy[i].pos = XMFLOAT3(175.0f, ENEMY_OFFSET_Y, -30.0f);
		enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		enemy[i].scl = XMFLOAT3(1.5f, 1.5f, 1.5f);

		enemy[i].tPos = XMFLOAT3(175.0f, ENEMY_OFFSET_Y, -30.0f);

		enemy[i].spd  = 0.0f;			// 移動スピードクリア
		enemy[i].size = {ENEMY_SIZE_X, ENEMY_SIZE_Y, ENEMY_SIZE_Z};	// 当たり判定の大きさ
		
		// ダンジョン(1階)なら
		if (GetField() == DUNGEON_FIRST_FLOOR)
		{
			XMFLOAT3 pos = enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		}

		enemy[i].draw = FALSE;	// TRUE:表示 / FALSE:非表示
		enemy[i].use  = TRUE;		// TRUE:使用 / FALSE:未使用
		enemy[i].chase = FALSE;    // TRUE:追跡中 / FALSE:非追跡中

		enemy[i].condition = 0;	// 通常状態にする
		
		enemy[i].hp = ENEMY_HP; // ヒットポイントの初期化

		enemy[i].state = FALSE;   // TRUE:移動可能 / FALSE:移動不可
		
		enemy[i].time  = 0.0f;	// 線形補間用のタイマーをクリア
		enemy[i].tblNo = 0;		// 再生する行動データテーブルNoをセット
		enemy[i].tblMax = sizeof(moveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	}
	//-------------------------------------------------------------------------
	// 初期位置の設定
	//-------------------------------------------------------------------------
	{
		enemy[0].pos = XMFLOAT3(-220.0f, ENEMY_OFFSET_Y, -0.0f);
		enemy[1].pos = XMFLOAT3(-120.0f, ENEMY_OFFSET_Y, -100.0f);
		enemy[2].pos = XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, 0.0f);
		enemy[3].pos = XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f);
		enemy[4].pos = XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f);
		enemy[5].pos = XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f);
		enemy[6].pos = XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f);
	}

	//-------------------------------------------------------------------------
	// 行動テーブルのセット
	//-------------------------------------------------------------------------
	{
		enemy[0].tblNo = 0;		// 再生する行動データテーブルNoをセット
		enemy[0].tblMax = sizeof(moveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		enemy[1].tblNo = 1;		// 再生する行動データテーブルNoをセット
		enemy[1].tblMax = sizeof(moveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		enemy[2].tblNo = 2;		// 再生する行動データテーブルNoをセット
		enemy[2].tblMax = sizeof(moveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		enemy[3].tblNo = 3;		// 再生する行動データテーブルNoをセット
		enemy[3].tblMax = sizeof(moveTbl3) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		enemy[4].tblNo = 4;		// 再生する行動データテーブルNoをセット
		enemy[4].tblMax = sizeof(moveTbl4) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		enemy[5].tblNo = 5;		// 再生する行動データテーブルNoをセット
		enemy[5].tblMax = sizeof(moveTbl5) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		enemy[6].tblNo = 6;		// 再生する行動データテーブルNoをセット
		enemy[6].tblMax = sizeof(moveTbl6) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	}

	//-------------------------------------------------------------------------
	// モデル読み込み
	//-------------------------------------------------------------------------
	{
		load = TRUE;

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
	if (load)
	{
		UnloadModel(&modelEnemy);
		load = FALSE;
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

		if (enemy[i].use)	  // 使用判定
		{
			// 移動前の座標を保存
			XMFLOAT3 oldPos = enemy[i].pos;
						
			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(enemy[i].shadowIdx, pos);

			//-------------------------------------------------------------------------
			// 視錐台カリング
			//-------------------------------------------------------------------------
			{
				enemy[i].draw = FrustumCulling(enemy[i].pos, enemy[i].rot, enemy[i].size);
			}

			//-------------------------------------------------------------------------
			// 移動処理
			//-------------------------------------------------------------------------
			{

				float distance = GetDistance(player[0].pos, enemy[i].pos);

				// 近づいたら追跡する
				if (distance < 100)
				{
					enemy[i].chase = TRUE;
				}
				else
				{
					enemy[i].chase = FALSE;
				}

				// ダメージを受けたらプレイヤーを追跡する
				if (enemy[i].hp != ENEMY_HP)
				{
					enemy[i].chase = TRUE;
				}

				if (enemy[i].chase)
				{
					enemy[i].tPos = player[0].pos;
				}

				if (!enemy[i].chase)
				{
					// 線形補間の処理
					{
						int nowNo = (int)enemy[i].time;			// 整数分であるテーブル番号を取り出している
						int maxNo = enemy[i].tblMax;				// 登録テーブル数を数えている
						int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
						INTERPOLATION_DATA* tbl = moveTblAdr[enemy[i].tblNo];	// 行動テーブルのアドレスを取得

						XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換

						XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
						float nowTime = enemy[i].time - nowNo;	// 時間部分である少数を取り出している

						Pos *= nowTime;		// 現在の移動量を計算している

						// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
						XMStoreFloat3(&enemy[i].tPos, nowPos + Pos);

						// frameを使て時間経過処理をする
						enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
						if ((int)enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
						{
							enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
						}
					}
				}

				// 敵の移動速度
				enemy[i].spd = VALUE_MOVE;

				XMVECTOR epos = XMLoadFloat3(&enemy[i].pos);
				XMVECTOR vec = XMLoadFloat3(&enemy[i].tPos) - epos;                   //エネミーとプレイヤーの差分を求めている

				// 目標の方向
				const float targetDir = static_cast<float>(atan2(vec.m128_f32[0], vec.m128_f32[2]) - XM_PI);;

				// 現在の方向から目標の方向への角度差を求める
				float angleDifference = targetDir - enemy[i].rot.y;

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
				enemy[i].rot.y = Lerp(enemy[i].rot.y, enemy[i].rot.y + angleDifference, VALUE_ROTATE);

				// プレイヤーの方に移動
				enemy[i].pos.x -= sinf(enemy[i].rot.y) * enemy[i].spd;
				enemy[i].pos.z -= cosf(enemy[i].rot.y) * enemy[i].spd;

				enemy[i].spd *= 0.0f;

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
						if (CollisionBC(enemy[i].pos, player[j].pos, ENEMY_SIZE, player[j].size))
						{

							if (atClTime >= 120)
							{
								player[j].hp -= 10;

								atClTime = 0;
							}

							enemy[i].pos = oldPos;

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
							BOOL ans = CollisionBC(enemy[k].pos, bomb[k].pos, 10.0f, 10.0f);

							if (ans == TRUE)
							{
								// ポジションをもとに戻す
								enemy[k].pos = oldPos;

								break;
							}
						}
					}
				}

				// フィールドとの当たり判定
				if (FieldHit(enemy[i].pos, oldPos))
				{
					enemy[i].pos = oldPos;
				}

			}

			//-------------------------------------------------------------------------
			// 状態異常 / エフェクト処理(パーティクル)
			//-------------------------------------------------------------------------
			{

				// 描画範囲内かどうか
				if (enemy[i].draw)
				{
					// 状態異常
					switch (enemy[i].condition)
					{
					case 0: // 通常
						break;

					case 1: // 炎上

						// HPを減らす
						enemy[i].hp -= 1.0f;

						XMFLOAT3 pos;
						XMFLOAT3 move;
						XMFLOAT3 scale;
						float fAngle, fLength;
						int nLife;
						float fSize;

						pos = enemy[i].pos;

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
						enemy[i].hp -= 2.0f;

						pos = enemy[i].pos;

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
				if (enemy[i].hp <= 0)
				{
					AddScore(100);

					enemy[i].use = FALSE;
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
		if (!enemy[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!enemy[i].draw) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(enemy[i].scl.x, enemy[i].scl.y, enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(enemy[i].rot.x, enemy[i].rot.y + XM_PI, enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(enemy[i].pos.x, enemy[i].pos.y, enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&enemy[i].mtxWorld, mtxWorld);

		switch (enemy[i].modelNo)
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
	return &enemy[0];
}