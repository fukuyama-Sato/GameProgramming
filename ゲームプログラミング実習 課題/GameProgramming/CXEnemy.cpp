#include "CXEnemy.h"

void CXEnemy::Init(CModelX *model){
	CXCharacter::Init(model);
	//合成行列の設定
	mColSphereBody.mpMatrix = &mpCombinedMatrix[8];
	mColSphereHead.mpMatrix = &mpCombinedMatrix[11];
	mColSphereSword.mpMatrix = &mpCombinedMatrix[21];
}