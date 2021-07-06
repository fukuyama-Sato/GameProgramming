#ifndef CXCHARACTER
#define CXCHARACTER

#include"CModelX.h"
#include"CMatrix.h"

class CXCharacter{
public:

	CModelX *mpModel;
	int mAnimationIndex;
	bool mAnimationLoopFlg;
	float mAnimationFrame;
	float mAnimationFrameSize;

	//����������
	void Init(CModelX *modelx);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framsize);
	//�X�V����
	void Update(CMatrix &m);
	//�`�揈��
	void Render();
};

#endif