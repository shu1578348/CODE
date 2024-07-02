//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : 荒山　秀磨
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "score.h"
#include "fade.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(7)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(480)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(80)			// 


#define TEXTURE_SWTCH_W				(SCREEN_WIDTH/3)	// 背景サイズ
#define TEXTURE_SWTCH_H				(SCREEN_HEIGHT/4)	// 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*vertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *texturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg001.jpg",
	"data/TEXTURE/result_logo.png",
	"data/TEXTURE/num.png",
	"data/TEXTURE/bar_white.png",
	"data/TEXTURE/result.png",
	"data/TEXTURE/bg000.jpg",
	"data/TEXTURE/bg001.jpg",
};


static BOOL						use;						// TRUE:使っている  FALSE:未使用
static float					w, h;					// 幅と高さ
static XMFLOAT3					pos;						// ポリゴンの座標
static int						texNo;					// テクスチャ番号

static BOOL						load = FALSE;

static SWITCH2	swtch;
static float	alpha;
static BOOL		flag_alpha;

int SwitchResultWait;
BOOL starthoi;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			texturName[i],
			NULL,
			NULL,
			&texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);


	// 変数の初期化
	use   = TRUE;
	w     = TEXTURE_WIDTH;
	h     = TEXTURE_HEIGHT;
	pos   = { w / 2, 50.0f, 0.0f };
	texNo = 0;

	// BGM再生

	// 変数の初期化
	swtch.w = TEXTURE_SWTCH_W;
	swtch.h = TEXTURE_SWTCH_H;//  x				H				Z
	swtch.pos = XMFLOAT3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT * 3 / 5, 0.0f);
	//↑画像調整
	swtch.texNo = 3;

	swtch.u = 0.0f;
	swtch.v = 0.0f;

	SwitchResultWait = 0;


	starthoi = FALSE;
	//}


	alpha = 1.0f;
	flag_alpha = TRUE;


	load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if (load == FALSE) return;

	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (texture[i])
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

	load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{

	if (GetKeyboardTrigger(DIK_RETURN) || (IsButtonTriggered(1, BUTTON_C)))
	{// Enter押したら、ステージを切り替える
		SetFade(FADE_OUT, MODE_TITLE);
	}
	// ゲームパッドで入力処理
	else if (IsButtonTriggered(0, BUTTON_START))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}


	if (SwitchResultWait > 50)		//押した後に80フレーム後シーン遷移
	{
		SetMode(MODE_TITLE);
		SwitchResultWait = 0;
		starthoi = FALSE;
	}

	if (starthoi == TRUE)
	{


		SwitchResultWait++;

	}




#ifdef _DEBUG	// デバッグ情報を表示する
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// 桁数分処理する
	int number = GetScore();

	{
		// リザルトの背景を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &texture[4]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLeftTop(vertexBuffer, 0.0f, 0.0f, w, h, 0.0f, 0.0f, 1.0f, 1.0f);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

	}

	// スコア表示
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &texture[2]);

		// 桁数分処理する
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(number % 10);

			// スコアの位置やテクスチャー座標を反映
			float pw = 16*4;			// スコアの表示幅
			float ph = 32*4;			// スコアの表示高さ
			float px = 600.0f - i*pw;	// スコアの表示位置X
			float py = 300.0f;			// スコアの表示位置Y

			float tw = 1.0f / 10;		// テクスチャの幅
			float th = 1.0f / 1;		// テクスチャの高さ
			float tx = x * tw;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(vertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			number /= 10;
		}

	}



}




