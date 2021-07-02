#include "CSceneGame.h"
//
#include "CCamera.h"
//
#include "CUtil.h"
//
#include "CRes.h"
//
#include "CKey.h"

CMatrix Matrix;

CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {
	//3D���f���t�@�C���̓ǂݍ���
	CRes::sModelX.Load(MODEL_FILE);
	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	mFont.LoadTexture("FontG.tga", 1, 4096 / 64);

}


void CSceneGame::Update() {

	//�A�j���[�V�����̎��Ԃ����Z
	CRes::sModelX.mAnimationSet[0]->mTime += 1.0f;
	CRes::sModelX.mAnimationSet[0]->mTime =
		(int)CRes::sModelX.mAnimationSet[0]->mTime %
		(int)(CRes::sModelX.mAnimationSet[0]->mMaxTime + 1);

	//�ŏ��̃A�j���[�V�����̏d�݂�1.0�ɂ���
	CRes::sModelX.mAnimationSet[0]->mWeight = 1.0f;
	//�t���[���̕ϊ��s����A�j���[�V�����ōX�V����
	CRes::sModelX.AnimateFrame();
	//�t���[���̍����s����v�Z����
	CRes::sModelX.mFrame[0]->AnimateCombined(&Matrix);

//#ifdef _DEBUG
//	for (int j = 0; j < 6; j++){
//		printf("Frame:%s\n", CRes::sModelX.mFrame[j]->mpName);
//		for (int n = 0; n < 4; n++){
//			for (int t = 0; t < 4; t++){
//				printf("%10f", CRes::sModelX.mFrame[j]->mCombinedMatrix.mM[n][t]);
//				if (t == 3)printf("\n");
//			}
//		}
//	}
//#endif

	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����
	//���_�����߂�
	e = CVector(-1.0f, 2.0f, 10.0f);
	//�����_�����߂�
	c = CVector();
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f);

	//�J�����N���X�̐ݒ�
	Camera.Set(e, c, u);
	Camera.Render();

	//X�� + ��]
	if (CKey::Push('K')){
		Matrix = Matrix * CMatrix().RotateX(1);
	}
	if (CKey::Push('I')){
		Matrix = Matrix * CMatrix().RotateX(-1);
	}
	//Y�� + ��]
	if (CKey::Push('L')){
		Matrix = Matrix * CMatrix().RotateY(1);
	}
	if (CKey::Push('J')){
		Matrix = Matrix * CMatrix().RotateY(-1);
	}

	//�s��ݒ�
	glMultMatrixf(Matrix.mF);
	//���_�ɃA�j���[�V������K�p����
	CRes::sModelX.AnimateVertex();
	//���f���`��
	CRes::sModelX.Render();

	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);

	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);

	//2D�̕`��I��
	CUtil::End2D();

	return;
}

