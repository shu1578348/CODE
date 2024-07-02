//=============================================================================
//
//　チュートリアル画面処理 [tutorial.cpp]
// Author : 
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(6)				// テクスチャの数

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
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* texturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg001.jpg",
	"data/TEXTURE/Tutorial_logo.png",
	"data/TEXTURE/num.png",
	"data/TEXTURE/bar_white.png",
	"data/TEXTURE/tutorial.jpg",
	"data/TEXTURE/bg.jpg",
};


static BOOL						use;						// TRUE:使っている  FALSE:未使用
static float					w, h;					// 幅と高さ
static XMFLOAT3					pos;						// ポリゴンの座標
static int						texNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;

static SWITCH1	g_swtch;
static float	alpha;
static BOOL		flag_alpha;

int SwitchTutorialWait;
BOOL starthoi1;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(void)
{
	ID3D11Device* pDevice = GetDevice();

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
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 変数の初期化
	use = TRUE;
	w = TEXTURE_WIDTH;
	h = TEXTURE_HEIGHT;
	pos = { w / 2, 50.0f, 0.0f };
	texNo = 0;


	// 変数の初期化
	g_swtch.w = TEXTURE_SWTCH_W;
	g_swtch.h = TEXTURE_SWTCH_H;//  x				H				Z
	g_swtch.pos = XMFLOAT3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT * 3 / 5, 0.0f);
	//↑画像調整
	g_swtch.texNo = 3;

	g_swtch.u = 0.0f;
	g_swtch.v = 0.0f;

	SwitchTutorialWait = 0;


	starthoi1 = FALSE;
	//}


	alpha = 1.0f;
	flag_alpha = TRUE;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (texture[i])
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(1,BUTTON_C))
	{// Enter押したら、ステージを切り替える
		SetFade(FADE_OUT, MODE_GAME);
	}


	if (SwitchTutorialWait > 50)		//押した後に80フレーム後シーン遷移
	{
		SetMode(MODE_GAME);
		SwitchTutorialWait = 0;
		starthoi1 = FALSE;
	}

	if (starthoi1 == TRUE)
	{


		SwitchTutorialWait++;

	}




#ifdef _DEBUG	// デバッグ情報を表示する

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//背景
	{
		// リザルトの背景を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &texture[4]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, w, h, 0.0f, 0.0f, 1.0f, 1.0f);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

	}

}