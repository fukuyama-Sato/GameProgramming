#include "CXPlayer.h"
#include "CKey.h"

void CXPlayer::Update(){

	if (CKey::Push('W')){
		ChangeAnimation(1, true, 60);
		mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;
	}

	else{
		ChangeAnimation(0, true, 60);
	}

	if (CKey::Push('A')){
		mRotation.mY += 2;
	}

	if (CKey::Push('D')){
		mRotation.mY -= 2;
	}

	CXCharacter::Update();
}