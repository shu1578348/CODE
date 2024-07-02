//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 荒山 秀磨
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE_CAMERA	(2.0f)					// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)			// カメラの回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA			camera;		// カメラデータ

static int				viewPortType = TYPE_FULL_SCREEN;

//-------------------------------------------------------------------------
// 線形補間のデータ
//-------------------------------------------------------------------------
static INTERPOLATION_DATA moveTbl0[] = {
	//座標							    回転率						拡大率						時間
	{ XMFLOAT3( 500.0f, 2400,  120.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3( 200.0f, 2300, -100.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(   0.0f, 2200, -300.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-200.0f, 2050, -500.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-350.0f, 1900, -350.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-500.0f, 1750, -150.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-650.0f, 1600,  100.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
	{ XMFLOAT3(-500.0f, 1400,  200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 20 },
	{ XMFLOAT3(-400.0f, 1200,  300.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-300.0f, 1000,  400.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 40 },
	{ XMFLOAT3(-200.0f,  750,  200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 40 },
	{ XMFLOAT3(-100.0f,  500,  120.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-200.0f,  250,  -40.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-220.0f,   10,  -80.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA* moveTblAdr[] =
{
	moveTbl0,
};

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	camera.pos = { POS_X_CAM, POS_Y_CAM, POS_Z_CAM };
	camera.at  = { 0.0f, 0.0f, 0.0f };
	camera.up  = { 0.0f, 1.0f, 0.0f };
	camera.rot = { 0.0f, 0.0f, 0.0f };

	if (GetField() == VILLAGE)
	{
		// カメラを移動可能か
		camera.state = TRUE;
	}
	else
	{
		// カメラを移動可能か
		camera.state = TRUE;
	}

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = camera.pos.x - camera.at.x;
	vz = camera.pos.z - camera.at.z;
	camera.len = sqrtf(vx * vx + vz * vz);

	// 線形補間によるカメラ移動
	{
		camera.time  = 0.0f;		// 線形補間用のタイマーをクリア
		camera.tblNo = 0;			// 再生する行動データテーブルNoをセット
		camera.tblMax = sizeof(moveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	}

	// ビューポートタイプの初期化
	SetViewPort(viewPortType);
}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{

#ifdef _DEBUG
	//-------------------------------------------------------------------------
	// デバッグ用カメラ移動
	//-------------------------------------------------------------------------
	if (camera.state)
	{
		if (GetKeyboardPress(DIK_Z))
		{// 視点旋回「左」
			camera.rot.y += VALUE_ROTATE_CAMERA;
			if (camera.rot.y > XM_PI)
			{
				camera.rot.y -= XM_PI * 2.0f;
			}

			camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
			camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;
		}

		if (GetKeyboardPress(DIK_C))
		{// 視点旋回「右」
			camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (camera.rot.y < -XM_PI)
			{
				camera.rot.y += XM_PI * 2.0f;
			}

			camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
			camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;
		}

		if (GetKeyboardPress(DIK_Y))
		{
			camera.pos.y += VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_N))
		{// 視点移動「下」
			camera.pos.y -= VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_T))
		{// 注視点移動「上」
			camera.at.y += VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_B))
		{// 注視点移動「下」
			camera.at.y -= VALUE_MOVE_CAMERA;
		}

		if (GetKeyboardPress(DIK_U))
		{// 近づく
			camera.len -= VALUE_MOVE_CAMERA;
			camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
			camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;
		}

		if (GetKeyboardPress(DIK_M))
		{// 離れる
			camera.len += VALUE_MOVE_CAMERA;
			camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
			camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;
		}

		
	}

	// カメラを初期に戻す
	if (GetKeyboardPress(DIK_R))
	{
		UninitCamera();
		InitCamera();
	}

#endif
	//-------------------------------------------------------------------------
	// マウスによるカメラ移動
	//-------------------------------------------------------------------------
	if (camera.state)
	{		
		// 視点の左右移動
		camera.rot.y += VALUE_ROTATE_CAMERA * (GetMouseX() / 10);
		if (camera.rot.y > XM_PI)
		{
			camera.rot.y -= XM_PI * 2.0f;
		}

		// 左右移動
		camera.pos.x = camera.at.x - sinf(camera.rot.y) * camera.len;
		camera.pos.z = camera.at.z - cosf(camera.rot.y) * camera.len;


		// 視点の上下移動
		camera.rot.x += VALUE_ROTATE_CAMERA * -(GetMouseY() / 10);

		if (camera.rot.x > XM_PI / 2.0f)
		{
			camera.rot.x = XM_PI / 1.999f;
		}
		else if (camera.rot.x < -XM_PI / 2.0f)
		{
			camera.rot.x = -XM_PI / 1.999f;
		}

		// 上下移動	
		camera.pos.y = camera.at.y + sinf(camera.rot.x) * camera.len; 

		// キーボード用
		if (GetKeyboardPress(DIK_Q))
		{// 注視点旋回「左」
			camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (camera.rot.y < -XM_PI)
			{
				camera.rot.y += XM_PI * 2.0f;
			}

			camera.at.x = camera.pos.x + sinf(camera.rot.y) * camera.len;
			camera.at.z = camera.pos.z + cosf(camera.rot.y) * camera.len;
		}

		if (GetKeyboardPress(DIK_E))
		{// 注視点旋回「右」
			camera.rot.y += VALUE_ROTATE_CAMERA;
			if (camera.rot.y > XM_PI)
			{
				camera.rot.y -= XM_PI * 2.0f;
			}

			camera.at.x = camera.pos.x + sinf(camera.rot.y) * camera.len;
			camera.at.z = camera.pos.z + cosf(camera.rot.y) * camera.len;
		}

		// キーボード用
		if (IsButtonPressed(1,BUTTON_L))
		{// 注視点旋回「左」
			camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (camera.rot.y < -XM_PI)
			{
				camera.rot.y += XM_PI * 2.0f;
			}

			camera.at.x = camera.pos.x + sinf(camera.rot.y) * camera.len;
			camera.at.z = camera.pos.z + cosf(camera.rot.y) * camera.len;
		}

		if (IsButtonPressed(1,BUTTON_R))
		{// 注視点旋回「右」
			camera.rot.y += VALUE_ROTATE_CAMERA;
			if (camera.rot.y > XM_PI)
			{
				camera.rot.y -= XM_PI * 2.0f;
			}

			camera.at.x = camera.pos.x + sinf(camera.rot.y) * camera.len;
			camera.at.z = camera.pos.z + cosf(camera.rot.y) * camera.len;
		}

	}

	//-------------------------------------------------------------------------
	// 線形補間によるカメラ移動
	//-------------------------------------------------------------------------
	if (!camera.state)
	{
		int nowNo = (int)camera.time;							// 整数分であるテーブル番号を取り出している
		int maxNo = camera.tblMax;							// 登録テーブル数を数えている
		int nextNo = (nowNo + 1) % maxNo;						// 移動先テーブルの番号を求めている
		INTERPOLATION_DATA* tbl = moveTblAdr[camera.tblNo];	// 行動テーブルのアドレスを取得

		XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
		XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換

		XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
		XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している

		float nowTime = camera.time - nowNo;		// 時間部分である少数を取り出している

		Pos *= nowTime;								// 現在の移動量を計算している
		Rot *= nowTime;								// 現在の回転量を計算している

		// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
		XMStoreFloat3(&camera.pos, nowPos + Pos);

		// 計算して求めた回転量を現在の移動テーブルに足している
		XMStoreFloat3(&camera.rot, nowRot + Rot);

		// frameを使て時間経過処理をする
		camera.time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
		if ((int)camera.time >= maxNo)			// 登録テーブル最後まで移動したか？
		{
			camera.time -= maxNo;					// ０番目にリセットしつつも小数部分を引き継いでいる
		}

		if ((int)camera.time >= maxNo - 1)
		{
			camera.state = TRUE;
		}

	}

}

//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void)
{
	// ビューマトリックス設定
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&camera.pos), XMLoadFloat3(&camera.at), XMLoadFloat3(&camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&camera.mtxInvView, mtxInvView);

	// プロジェクションマトリックス設定
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&camera.mtxProjection, mtxProjection);

	SetShaderCamera(camera.pos);

}

//=============================================================================
// カメラの取得
//=============================================================================
CAMERA* GetCamera(void)
{
	return &camera;
}

//=============================================================================
// ビューポートの設定
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext* g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	viewPortType = type;

	//-------------------------------------------------------------------------
	// ビューポート設定
	//-------------------------------------------------------------------------
	switch (viewPortType)
	{
	case TYPE_FULL_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_LEFT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_RIGHT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_UP_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_DOWN_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;


	}
	g_ImmediateContext->RSSetViewports(1, &vp);

}

int GetViewPortType(void)
{
	return viewPortType;
}

// カメラの視点と注視点をセット
void SetCameraAT(XMFLOAT3 pos)
{
	// カメラの注視点を引数の座標にしてみる
	camera.at = pos;

	if (camera.state)
	{
		// カメラの視点をカメラのY軸回転に対応させている
		camera.pos.x = camera.at.x - sinf(camera.rot.y) * cosf(camera.rot.x) * camera.len;
		camera.pos.z = camera.at.z - cosf(camera.rot.y) * cosf(camera.rot.x) * camera.len;

		camera.pos.y = camera.at.y - sinf(camera.rot.x) * camera.len;
	}

}