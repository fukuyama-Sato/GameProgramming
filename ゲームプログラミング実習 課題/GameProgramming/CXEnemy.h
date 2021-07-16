#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter{
public:
	//ƒRƒ‰ƒCƒ_éŒ¾
	CCollider mColSphereBody;	//‘Ì
	CCollider mColSphereHead;	//“ª
	CCollider mColSphereSword;	//Œ•

	CXEnemy()
		:mColSphereBody(this, nullptr, CVector(), 0.5f)
		, mColSphereHead(this, nullptr,
		CVector(0.0f, 5.0f, -3.0f), 0.5f)
		, mColSphereSword(this, nullptr,
		CVector(-10.0f, 10.0f, 50.0f), 0.3f)
	{}

	void Init(CModelX *model);


};

#endif