#include "CSceneGame.h"
//
#include "CCamera.h"
//
#include "CUtil.h"
//
#include "CRes.h"
//
#include "CKey.h"
//
#include "CMaterial.h"
//
#include "CCollisionManager.h"
//
#include "CXPlayer.h"
//
#include "CXEnemy.h"
//
#include "CXCharacter.h"

CMatrix Matrix;

CXPlayer mPlayer;

CXEnemy mEnemy;

CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {
	//3Dモデルファイルの読み込み
	CRes::sModelX.Load(MODEL_FILE);
	//キャラクターにモデルを設定
	mPlayer.Init(&CRes::sModelX);

	CRes::sKnight.Load("knight\\knight_low.X");
	//敵の初期位置
	mEnemy.Init(&CRes::sKnight);
	//敵の配置
	mEnemy.mPosition = CVector(7.0f, 0.0f, 0.0f);
	//アニメーションの最大フレームサイズを設定
	mEnemy.mAnimationFrameSize = 1024;

	//テキストフォントの読み込みと設定
	mFont.LoadTexture("FontG.tga", 1, 4096 / 64);

}

void CSceneGame::Update() {

	//キャラクタークラスの更新
	mPlayer.Update();

	//敵の更新
	mEnemy.Update();

	CCollisionManager::Get()->Collision();

	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(-1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);

	//カメラクラスの設定
	Camera.Set(e, c, u);
	Camera.Render();

	//X軸 + 回転
	if (CKey::Push('K')){
		Matrix = Matrix * CMatrix().RotateX(1);
	}
	if (CKey::Push('I')){
		Matrix = Matrix * CMatrix().RotateX(-1);
	}
	//Y軸 + 回転
	if (CKey::Push('L')){
		Matrix = Matrix * CMatrix().RotateY(1);
	}
	if (CKey::Push('J')){
		Matrix = Matrix * CMatrix().RotateY(-1);
	}

	//行列設定
	glMultMatrixf(Matrix.mF);

	//モデル描画
	mPlayer.Render();
	//コライダ描画
	CCollisionManager::Get()->Render();

	//敵の描画
	mEnemy.Render();

	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);

	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);

	//2Dの描画終了
	CUtil::End2D();

	return;
}

