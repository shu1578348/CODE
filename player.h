//=============================================================================
//
// モデル処理 [player.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MAX		(1)					// プレイヤーの数

#define	PLAYER_SIZE		(6.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYER
{
	XMFLOAT3		pos;		// ポリゴンの位置
	XMFLOAT3		rot;		// ポリゴンの向き(回転)
	XMFLOAT3		scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT4X4		mtxWorld;	// ワールドマトリックス

	BOOL			load;
	BOOL			use;
	BOOL			jump;
	DX11_MODEL		model;		// モデル情報

	float			spd;		// 移動スピード
	float			dir;		// 向き
	float			size;		// 当たり判定の大きさ
	float           sp;			// 攻撃用のポイント
	int				shadowIdx;	// 影のIndex
	int				jumpCnt;	// ジャンプカウンター
	int				hp;			// ヒットポイント

	// 階層アニメーション用のメンバー変数
	float			time;		// 線形補間用
	int				tblNo;		// 行動データのテーブル番号
	int				tblMax;		// そのテーブルのデータ数

	BOOL			anim;       // アニメーションの再生判定

	int				pose;		// プレイヤーの姿勢
	int				tool;		// 所持品

	// 親は、NULL、子供は親のアドレスを入れる(セーブ＆ロードの時は↓ここ気をつける事)
	PLAYER* parent;	// 自分が親ならNULL、自分が子供なら親のplayerアドレス
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);

float GetDistance(XMFLOAT3 mpos, XMFLOAT3 ypos);

float Lerp(float a, float b, float t);