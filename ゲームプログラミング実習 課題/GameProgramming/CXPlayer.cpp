#include "CXPlayer.h"
#include "CKey.h"


void CXPlayer::Init(CModelX *model){
	CXCharacter::Init(model);
	//‡¬s—ñ‚ÌÝ’è
	mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];

	mColSphereSword.mTag = CCollider::ESWORD;
}


void CXPlayer::Update(){

	//UŒ‚
	if (CKey::Once(VK_SPACE)){
		ChangeAnimation(3, false, 30);
	}
	if (mAnimationIndex == 3 && mAnimationFrame >= mAnimationFrameSize){
		ChangeAnimation(4, false, 30);
	}
	if (mAnimationIndex == 4 && mAnimationFrame >= mAnimationFrameSize){
		ChangeAnimation(0, true, 60);
	}
	if (mAnimationIndex == 3, 4 && mAnimationFrame < mAnimationFrameSize){
		mAnimationFrame;
	}

	if (mAnimationIndex != 3 && mAnimationIndex != 4){
		//‘Oi
		if (CKey::Push('W')){
			ChangeAnimation(1, true, 60);
			mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;
		}
		//‘Ò‹@
		else{
			ChangeAnimation(0, true, 60);
		}


		//¶‰ñ“]
		if (CKey::Push('A')){
			mRotation.mY += 2;
		}
		//‰E‰ñ“]
		if (CKey::Push('D')){
			mRotation.mY -= 2;
		}
	}
	CXCharacter::Update();
}