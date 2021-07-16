#ifndef CXCHARACTER
#define CXCHARACTER

#include"CModelX.h"
#include"CMatrix.h"
#include"CCharacter.h"

class CXCharacter : public CCharacter{
public:

	CModelX *mpModel;
	CMatrix *mpCombinedMatrix;	//�����s��ޔ�

	int mAnimationIndex;
	bool mAnimationLoopFlg;
	float mAnimationFrame;
	float mAnimationFrameSize;

	CXCharacter();

	virtual ~CXCharacter(){
		SAFE_DELETE_ARRAY(mpCombinedMatrix);
	}

	//����������
	void Init(CModelX *modelx);

	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framsize);

	//�X�V����
	void Update(CMatrix &m);

	//�X�V����
	void Update();

	//�`�揈��
	void Render();
};

#endif