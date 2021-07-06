#ifndef CXCHARACTER
#define CXCHARACTER

#include"CModelX.h"
#include"CMatrix.h"

class CXCharacter{
public:

	CModelX *mpModel;
	int mAnimationIndex;
	bool mAnimationLoopFlg;
	float mAnimationFrame;
	float mAnimationFrameSize;

	//初期化処理
	void Init(CModelX *modelx);
	//アニメーションの変更
	void ChangeAnimation(int index, bool loop, float framsize);
	//更新処理
	void Update(CMatrix &m);
	//描画処理
	void Render();
};

#endif