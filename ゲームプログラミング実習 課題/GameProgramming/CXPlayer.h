#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter{
public:

	//コライダ宣言
	CCollider mColSphereBody;	//体
	CCollider mColSphereHead;	//頭
	CCollider mColSphereSword;	//剣

	CXPlayer()
		:mColSphereBody(this, nullptr, CVector(), 0.5f)
		,mColSphereHead(this, nullptr, 
		CVector(0.0f,5.0f,-3.0f), 0.5f)
		,mColSphereSword(this, nullptr, 
		CVector(-10.0f, 10.0f, 50.0f), 0.3f)
	{
		mTag = EPLAYER;
	}

	void Init(CModelX *model);

	void Update();

};

#endif