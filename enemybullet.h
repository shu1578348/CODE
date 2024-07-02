//=============================================================================
//
// 弾発射処理 [bullet.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_BULLET_MAX	(256)	// バレットのMax数
#define ENEMY_BULLET_SPEED	(4.0f)	// バレットの移動スピード
#define ENEMY_BULLET_SIZE	(9.0f)	// バレットのサイズ
#define ENEMY_BULLET_DELETE (200)	// バレットが消えるまでの時間
#define ENEMY_BULLET_TIME	(40)	// バレットの連射間隔

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMYBULLET
{
	XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
	XMFLOAT3	pos;		// モデルの位置
	XMFLOAT3	rot;		// モデルの向き(回転)
	XMFLOAT3	scl;		// モデルの大きさ(スケール)

	BOOL		use;		// TRUE:使用 / FALSE:未使用

	float		spd;		// 移動スピード
	float		dir;		// 向き
	float		size;		// 当たり判定のサイズ
	int			type;		// 弾の種類
	int			dTime;		// 消えるまでの時間
	int			shadowIdx;	// 影のインデックス番号
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);

void SetEnemyBullet(XMFLOAT3 pos, XMFLOAT3 rot,int type);

ENEMYBULLET* GetEnemyBullet(void);

void EnemyBulletEfect(XMFLOAT3 pos, int type);

