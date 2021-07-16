#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter{
public:

	//�R���C�_�錾
	CCollider mColSphereBody;	//��
	CCollider mColSphereHead;	//��
	CCollider mColSphereSword;	//��

	CXPlayer()
		:mColSphereBody(this, nullptr, CVector(), 0.5f)
		,mColSphereHead(this, nullptr, 
		CVector(0.0f,5.0f,-3.0f), 0.5f)
		,mColSphereSword(this, nullptr, 
		CVector(-10.0f, 10.0f, 50.0f), 0.3f)
	{
	}

	void Init(CModelX *model);

	void Update();

};

#endif