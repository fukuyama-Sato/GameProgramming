#include "CXEnemy.h"

void CXEnemy::Init(CModelX *model){
	CXCharacter::Init(model);
	//‡¬s—ñ‚ÌÝ’è
	mColSphereBody.mpMatrix = &mpCombinedMatrix[8];
	mColSphereHead.mpMatrix = &mpCombinedMatrix[11];
	mColSphereSword.mpMatrix = &mpCombinedMatrix[21];
}

void CXEnemy::Collision(CCollider *m, CCollider *o){

	if (m->mType == CCollider::ESPHERE){
		if (o->mType == CCollider::ESPHERE){
			if (o->mpParent->mTag == EPLAYER){
				if (o->mTag == CCollider::ESWORD){
					if (CCollider::Collision(m, o)){
						ChangeAnimation(11, false, 30);
					}
				}
			}
		}
	}
}