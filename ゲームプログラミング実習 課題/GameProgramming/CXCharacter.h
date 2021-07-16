#ifndef CXCHARACTER
#define CXCHARACTER

#include"CModelX.h"
#include"CMatrix.h"
#include"CCharacter.h"

class CXCharacter : public CCharacter{
public:

	CModelX *mpModel;
	CMatrix *mpCombinedMatrix;	//合成行列退避

	int mAnimationIndex;
	bool mAnimationLoopFlg;
	float mAnimationFrame;
	float mAnimationFrameSize;

	CXCharacter();

	virtual ~CXCharacter(){
		SAFE_DELETE_ARRAY(mpCombinedMatrix);
	}

	//初期化処理
	void Init(CModelX *modelx);

	//アニメーションの変更
	void ChangeAnimation(int index, bool loop, float framsize);

	//更新処理
	void Update(CMatrix &m);

	//更新処理
	void Update();

	//描画処理
	void Render();
};

#endif