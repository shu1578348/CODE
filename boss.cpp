//=============================================================================
//
// ボス処理 [boss.cpp]
// Author : 荒山秀磨
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
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
#include "boss.h"
#include "enemybullet.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BOSS_BODY	 "data/MODEL/boss/boss_body.obj"	// 読み込むモデル名
#define MODEL_BOSS_HEAD	 "data/MODEL/boss/boss_head.obj"

#define BOSS_HP				(500.0f)		 // ボスのHP

#define	VALUE_MOVE			(0.1f)			 // 移動量
#define	VALUE_ROTATE		(XM_PI * 0.1f)	 // 回転量
#define BOSS_SHADOW_SIZE	(0.8f)			 // 影の大きさ
#define BOSS_OFFSET_Y		(2.0f)			 // ボスの足元をあわせる

#define PARTICLE_ROTATION   (6.0f)			 // パーティクルの回転量
#define MODE_CHANGE			(200)			 // 攻撃モードの切り替え時間
#define CIRCLE_SPEED        (0.5f)           // 円状攻撃が広がるスピード
#define BLESS_DAMAGE		(1)				 // ブレスの攻撃力
#define CIRCLE_DAMAGE		(2)				 // 円状攻撃の攻撃力

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BOSS g_Boss[BOSS_MAX];	 // ボス構造体
static BOOL	  g_Load = FALSE;	 // 初期化を行ったかのフラグ
DX11_MODEL	  modelBossBody;	 // モデル情報
DX11_MODEL	  modelBossHead;	 // モデル情報

float pCnt  = 0;	// パーティクルの回転速度
float pRad  = 0;	// パーティクルの半径
int clBTime = 0;	// バレットの連射間隔
int clCTime = 0;	// modeの切り替え時間

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoss(void)
{
	//-------------------------------------------------------------------------
	// 情報の初期化
	//-------------------------------------------------------------------------
	for (int i = 0; i < BOSS_MAX; i++)
	{
		g_Boss[i].pos = XMFLOAT3(175.0f, BOSS_OFFSET_Y, -30.0f);
		g_Boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Boss[i].scl = XMFLOAT3(2.5f, 2.5f, 2.5f);

		g_Boss[i].tPos = XMFLOAT3(175.0f, BOSS_OFFSET_Y, -30.0f);

		g_Boss[i].spd  = 0.0f;		// 移動スピードクリア
		g_Boss[i].size = {BOSS_SIZE_X, BOSS_SIZE_Y, BOSS_SIZE_Z};	// 当たり判定の大きさ

		g_Boss[i].aRot = XMFLOAT3(0.0f, -0.5f, 0.0f); // 攻撃方向
		
		g_Boss[i].mode = 1;		    // 攻撃の種類

		g_Boss[i].draw  = TRUE;		// TRUE:表示   / FALSE:非表示
		g_Boss[i].use   = TRUE;		// TRUE:使用   / FALSE:未使用
		g_Boss[i].chase = FALSE;    // TRUE:追跡中 / FALSE:非追跡中

		g_Boss[i].condition = 0;	// 通常状態にする
		
		g_Boss[i].hp = BOSS_HP;     // ヒットポイントの初期化

		g_Boss[i].state = FALSE;    // 状態
		
	}
	//-------------------------------------------------------------------------
	// 初期位置の設定
	//-------------------------------------------------------------------------
	{
		g_Boss[0].pos = XMFLOAT3(-220.0f, BOSS_OFFSET_Y, -0.0f);
	}

	//-------------------------------------------------------------------------
	// モデル読み込み
	//-------------------------------------------------------------------------
	{
		g_Load = TRUE;

		// モデルの読み込み
		LoadModel(MODEL_BOSS_BODY, &modelBossBody);
		LoadModel(MODEL_BOSS_HEAD, &modelBossHead);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{
	if (g_Load)
	{
		UnloadModel(&modelBossBody);
		UnloadModel(&modelBossHead);
		g_Load = FALSE;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
	//-------------------------------------------------------------------------
	// 情報の取得
	//-------------------------------------------------------------------------
	PLAYER * player = GetPlayer();  // プレイヤー
	BULLET * bullet = GetBullet();  // バレット
	CAMERA * cam    = GetCamera();  // カメラ

	for (int i = 0; i < BOSS_MAX; i++)
	{

		if (g_Boss[i].use)	  // 使用判定
		{
			// 移動前の座標を保存
			XMFLOAT3 oldPos = g_Boss[i].pos;
						
			// 影もボスの位置に合わせる
			XMFLOAT3 pos = g_Boss[i].pos;
			pos.y -= (BOSS_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Boss[i].shadowIdx, pos);

			//-------------------------------------------------------------------------------
			// 視錐台カリング
			//-------------------------------------------------------------------------------
			{
				g_Boss[i].draw = FrustumCulling(g_Boss[i].pos, g_Boss[i].rot, g_Boss[i].size);
			}
		
			//-------------------------------------------------------------------------------
			// 移動処理
			//-------------------------------------------------------------------------------
			{
				g_Boss[i].spd = VALUE_MOVE;

				XMVECTOR epos = XMLoadFloat3(&g_Boss[i].pos);
				XMVECTOR vec  = XMLoadFloat3(&player[0].pos) - epos;  // エネミーとプレイヤーの差分を求めている

				// 目標の方向
				g_Boss[i].rot.y = static_cast<float>(atan2(vec.m128_f32[0], vec.m128_f32[2]) - XM_PI);

				g_Boss[i].pos.x -= sinf(g_Boss[i].rot.y) * g_Boss[i].spd;
				g_Boss[i].pos.z -= cosf(g_Boss[i].rot.y) * g_Boss[i].spd;
			}

			//-------------------------------------------------------------------------
			// 攻撃処理
			//-------------------------------------------------------------------------
			{
				// 攻撃モード切り替え時間の減少
				clCTime++;

				pCnt -= PARTICLE_ROTATION; // パーティクル回転用

				// モード切替処理
				if (clCTime >= MODE_CHANGE)
				{
					g_Boss[i].mode = (g_Boss[i].mode + 1) % 3;

					g_Boss[i].aRot.y = g_Boss[i].rot.y - 0.5f;

					pRad = 0;

					clCTime = 0;
				}

				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				float fAngle, fLength;
				int nLife;
				float fSize;

				switch (g_Boss[i].mode)
				{
				case 0:

					// バレット攻撃のクールタイム
					clBTime++;

					g_Boss[i].aRot.y = g_Boss[i].rot.y;

					if (clBTime >= 10)
					{
						// バレットによる攻撃
						SetEnemyBullet(g_Boss[i].pos, g_Boss[i].rot, 0);

						clBTime = 0;
					}

					break;

				case 1:

					// ブレスの発射方向
					if (g_Boss[i].aRot.y <= 0.5)
					{
						g_Boss[i].aRot.y += 0.005f;
					}

					for (int j = 0; j <= 10; j++)
					{
						pos = g_Boss[i].pos;

						pos.y +=  80.0f;

						pos.x -= sinf(g_Boss[i].aRot.y) * j;
						pos.z -= cosf(g_Boss[i].aRot.y) * j;

						pos.x -= sinf(g_Boss[i].aRot.y + pCnt + (XM_PI * 0.2f * j)) * 0.5f * j;
						pos.z -= cosf(g_Boss[i].aRot.y + pCnt + (XM_PI * 0.2f * j)) * 0.5f * j;
						pos.y -= cosf(g_Boss[i].aRot.z + pCnt + (XM_PI * 0.2f * j)) * 0.5f * j;

						scale = { 0.25f, 0.25f, 0.25f };

						move.x = -sinf(g_Boss[i].aRot.y) * 8.0f;
						move.z = -cosf(g_Boss[i].aRot.y) * 8.0f;
						move.y = -2.5f;

						nLife = rand() % 100 + 150;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.2f, 1.0f, 1.0f), fSize, fSize, nLife, 2);

						pos = g_Boss[i].pos;

						pos.y = g_Boss[i].pos.y + 80.0f;

						pos.x -= sinf(g_Boss[i].aRot.y) * j;
						pos.z -= cosf(g_Boss[i].aRot.y) * j;

						pos.x -= sinf(g_Boss[i].aRot.y + pCnt + (XM_PI * 0.2f * j)) * 0.6f * j;
						pos.z -= cosf(g_Boss[i].aRot.y + pCnt + (XM_PI * 0.2f * j)) * 0.6f * j;
						pos.y -= cosf(g_Boss[i].aRot.z + pCnt + (XM_PI * 0.2f * j)) * 0.6f * j;

						scale = { 0.15f, 0.15f, 0.15f };

						move.x = -sinf(g_Boss[i].aRot.y) * 8.0f;
						move.z = -cosf(g_Boss[i].aRot.y) * 8.0f;
						move.y = -2.5f;

						nLife = rand() % 100 + 150;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.5f, 1.0f, 1.0f), fSize, fSize, nLife, 2);

					}
					break;

				case 2:

					pRad += CIRCLE_SPEED; // 円を広げる

					for (int j = 0; j <= 20; j++)
					{

						pos = g_Boss[i].pos;

						pos.x -= sinf(g_Boss[i].rot.y + pCnt + (XM_PI * 0.1f * j)) * (100.0f + pRad);
						pos.z -= cosf(g_Boss[i].rot.y + pCnt + (XM_PI * 0.1f * j)) * (100.0f + pRad);

						scale = { 0.3f, 0.3f, 0.3f };

						fAngle = (float)(rand() % 628 - 314) / 20;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 8;

						move.x = sinf(fAngle) * fLength;
						move.y = (rand() % 300 / 100.0f + 10) / 10;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 80 + 100;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);

					}

					for (int j = 0; j <= 20; j++)
					{

						pos = g_Boss[i].pos;

						pos.x -= sinf(g_Boss[i].rot.y - pCnt + (XM_PI * 0.1f * j)) * (95.0f + pRad);
						pos.z -= cosf(g_Boss[i].rot.y - pCnt + (XM_PI * 0.1f * j)) * (95.0f + pRad);

						scale = { 0.3f, 0.3f, 0.3f };

						fAngle = (float)(rand() % 628 - 314) / 20;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 8;

						move.x = sinf(fAngle) * fLength;
						move.y = (rand() % 300 / 100.0f + 10) / 20;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 80 + 100;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);

					}

					break;

				}


			}

			//-------------------------------------------------------------------------
			// 攻撃の当たり判定処理
			//-------------------------------------------------------------------------
			{

				XMFLOAT3 aPos;
				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				float fAngle, fLength;
				int nLife;
				float fSize;

				switch (g_Boss[i].mode)
				{
				case 0:
					break;

				case 1: // ブレス

					aPos = g_Boss[i].pos;

					aPos.x -= sinf(g_Boss[i].aRot.y - 0.1f) * 230.0f;
					aPos.z -= cosf(g_Boss[i].aRot.y - 0.1f) * 230.0f;

					for (int n = 0; n < PLAYER_MAX; n++)
					{
						if (player[n].use == TRUE)
						{
							if (CollisionBC(aPos, player[0].pos, 40.0f, player[0].size))
							{
								// プレイヤーへのダメージ
								player[0].hp -= BLESS_DAMAGE;

								// 接触エフェクト
								pos = player[0].pos;

								scale = { 0.3f, 0.3f, 0.3f };

								fAngle = (float)(rand() % 628 - 314) * 0.1f;
								fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.25f;

								move.x = sinf(fAngle) * fLength;
								move.y = (rand() % 300 / 100.0f + 10) * 0.1f;
								move.z = cosf(fAngle) * fLength;

								nLife = rand() % 80 + 80;

								fSize = (float)(rand() % 30 + 10);

								// ビルボードの設定
								SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);
							}

						}

					}

					break;

				case 2: // 円状攻撃

					for (int n = 0; n < PLAYER_MAX; n++)
					{
						if (player[n].use == TRUE)
						{
							if (CollisionBC(g_Boss[i].pos, player[0].pos, 100.0f + pRad, player[0].size))
							{
								if (CollisionBC(g_Boss[i].pos, player[0].pos, 90.0f + pRad, player[0].size) == FALSE)
								{

									player[0].hp -= CIRCLE_DAMAGE;

									// 接触エフェクト
									for (int k = 0; k < 3; k++)
									{
										pos = player[0].pos;

										scale = { 0.3f, 0.3f, 0.3f };

										fAngle = (float)(rand() % 628 - 314) * 0.1f;
										fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.25f;

										move.x = sinf(fAngle) * fLength;
										move.y = (rand() % 300 / 100.0f + 10) * 0.1f;
										move.z = cosf(fAngle) * fLength;

										nLife = rand() % 80 + 80;

										fSize = (float)(rand() % 30 + 10);

										// ビルボードの設定
										SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);
									}


								}

							}

						}

					}

					break;

				}

			}

			//-------------------------------------------------------------------------
			// エフェクト処理(パーティクル)
			//-------------------------------------------------------------------------
			{
				// 目のエフェクト
				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				float fAngle, fLength;
				float fSize;
				int nLife;

				pos = g_Boss[i].pos;

				pos.y = 90.0f;

				pos.x -= sinf(g_Boss[i].aRot.y - 0.1f) * 50.0f;
				pos.z -= cosf(g_Boss[i].aRot.y - 0.1f) * 50.0f;

				scale = { 0.25f, 0.25f, 0.25f };
				fAngle = (float)(rand() % 628 - 314) / 50;
				fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 30;

				move.x = sinf(fAngle) * fLength;
				move.y = (rand() % 300 / 100.0f + 10) / 15;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 80 + 80;

				fSize = (float)(rand() % 30 + 10);

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(0.1f, 1.0f, 0.1f, 1.0f), fSize, fSize, nLife, 1);

				pos = g_Boss[i].pos;

				pos.y = 90.0f;

				pos.x -= sinf(g_Boss[i].aRot.y + 0.2f) * 50.0f;
				pos.z -= cosf(g_Boss[i].aRot.y + 0.2f) * 50.0f;

				// ビルボードの設定
				SetParticle(pos, pos, move, scale, XMFLOAT4(0.1f, 1.0f, 0.1f, 1.0f), fSize, fSize, nLife, 1);
			}


			{ // mode0のときのブレスをためるエフェクト

				XMFLOAT3 tPos = g_Boss[i].pos;
				tPos.y += 75.0f;
				float radius = 70.0f;
				tPos.x -= sinf(g_Boss[i].aRot.y) * 40.0f;
				tPos.z -= cosf(g_Boss[i].aRot.y) * 40.0f;

				if (g_Boss[i].mode == 0)
				{
					// アフィン変換行列の作成
					XMMATRIX transformMatrix = XMMatrixTranslation(tPos.x, tPos.y, tPos.z); // 平行移動行列

					// 初期位置のランダムな球状設定
					float u = static_cast<float>(rand()) / RAND_MAX; // [0, 1]
					float v = static_cast<float>(rand()) / RAND_MAX; // [0, 1]
					float theta = 2 * XM_PI * u;  // [0, 2pi]
					float phi = acosf(2 * v - 1); // [0, pi]

					// 極座標系から直交座標系へ変換
					float x = radius * sinf(phi) * cosf(theta);
					float y = radius * sinf(phi) * sinf(theta);
					float z = radius * cosf(phi);

					// 初期位置をアフィン変換して目標位置を計算
					XMVECTOR initialPos = XMVectorSet(x, y, z, 1.0f); // 初期位置
					initialPos = XMVector4Transform(initialPos, transformMatrix); // 初期位置を変換

					// 目標位置に向かう移動ベクトルの計算
					XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&tPos), initialPos); // 目標位置から初期位置を引く
					XMVECTOR normalizedDirection = XMVector3Normalize(direction); // 正規化
					XMFLOAT3 move;
					XMStoreFloat3(&move, normalizedDirection);

					// move を1.5倍にする
					move.x *= 1.5f;
					move.y *= 1.5f;
					move.z *= 1.5f;

					// その他のパーティクル属性の設定
					XMFLOAT3 scale = { 0.2f, 0.2f, 0.2f };
					int nLife = rand() % 150 + 120;
					float fSize = static_cast<float>(rand() % 30 + 10);

					// ビルボードの設定
					SetParticle(XMFLOAT3(initialPos.m128_f32[0], initialPos.m128_f32[1], initialPos.m128_f32[2]), tPos, move, scale, XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), fSize, fSize, nLife, 3);

				}

			}

			//-------------------------------------------------------------------------
			// 死亡判定処理
			//-------------------------------------------------------------------------
			{
				if (g_Boss[i].hp <= 0)
				{
					g_Boss[i].use = FALSE;
					SetFade(FADE_OUT, MODE_RESULT);
					AddScore(1000);
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
void DrawBoss(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < BOSS_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Boss[i].use)  continue;

		// TRUE:表示 / FALSE:非表示
		if (!g_Boss[i].draw) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Boss[i].scl.x, g_Boss[i].scl.y, g_Boss[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Boss[i].rot.x, g_Boss[i].rot.y + XM_PI, g_Boss[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Boss[i].pos.x, g_Boss[i].pos.y, g_Boss[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Boss[i].mtxWorld, mtxWorld);

		// 体の描画
		switch (g_Boss[i].modelNo)
		{
		case 0:
			DrawModel(&modelBossBody);
			break;
		}

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Boss[i].scl.x, g_Boss[i].scl.y, g_Boss[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Boss[i].aRot.x, g_Boss[i].aRot.y + XM_PI, g_Boss[i].aRot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Boss[i].pos.x, g_Boss[i].pos.y, g_Boss[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Boss[i].mtxWorld, mtxWorld);

		// 頭の描画
		switch (g_Boss[i].modelNo)
		{
		case 0:
			DrawModel(&modelBossHead);
			break;
		}

	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// ボスの取得
//=============================================================================
BOSS* GetBoss()
{
	return &g_Boss[0];
}