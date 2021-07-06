#include "CXPlayer.h"
#include "CKey.h"

void CXPlayer::Update(){

	if (CKey::Push('W')){
		ChangeAnimation(1, true, 60);
		mPosition.mZ += 0.1f;
	}
	else{
		ChangeAnimation(0, true, 60);
	}
	CXCharacter::Update();
}