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
#define BULLET_MAX		(256)	// バレットのMax数
#define BULLET_SPEED	(4.0f)	// バレットの移動スピード
#define BULLET_SIZE		(8.0f)	// バレットのサイズ
#define BULLET_DELETE   (100)	// バレットが消えるまでの時間
#define BULLET_TIME		(40)	// バレットの連射間隔

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct BULLET
{
	XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
	XMFLOAT3	pos;		// モデルの位置
	XMFLOAT3	rot;		// モデルの向き(回転)
	XMFLOAT3	scl;		// モデルの大きさ(スケール)

	BOOL		use;		// TRUE:使用 / FALSE:未使用
	BOOL		shot;		// TRUE:発射 / FALSE:未発射

	float		spd;		// 移動スピード
	float		dir;		// 向き
	float		size;		// 当たり判定のサイズ
	int			shotDir;	// 発射方向 0:中央 1:左前 2:右前
	int			modelNo;	// モデルの種類
	int			type;		// 弾の種類
	int			dTime;		// 消えるまでの時間
	int			shadowIdx;	// 影のインデックス番号
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(XMFLOAT3 pos, XMFLOAT3 rot, BOOL shot ,int type, int shotDir);

BULLET* GetBullet(void);

void BulletEfect(XMFLOAT3 pos, int type);

