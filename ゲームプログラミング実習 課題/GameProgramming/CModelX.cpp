#include "glut.h"
#include <stdio.h>
#include <string.h>	//������֐��̃C���N���[�h
#include "CModelX.h"
#include "CMaterial.h"

void CModelX::Load(char *file){
	//
	//�t�@�C���T�C�Y���擾
	//
	FILE *fp;	//�t�@�C���|�C���^�ϐ��̍쐬

	fp = fopen(file, "rb");	//�t�@�C�����I�[�v��
	if (fp == NULL){	//�G���[�`�F�b�N
		printf("fopen error:%s\n", file);
		return;
	}

	//�t�@�C���̍Ō�ֈړ�
	fseek(fp, 0L, SEEK_END);
	//�t�@�C���T�C�Y�̎擾
	int size = ftell(fp);
	//�t�@�C���T�C�Y+1�o�C�g���̗̈���m��
	char *buf = mpPointer = new char[size + 1];

	//
	//�t�@�C������3D���f���̃f�[�^��ǂݍ���

	//
	//�t�@�C���̐擪�ֈړ�
	fseek(fp, 0L, SEEK_SET);
	//�m�ۂ����̈�Ƀt�@�C���T�C�Y���f�[�^��ǂݍ���
	fread(buf, size, 1, fp);
	//�Ō��\0��ݒ肷��(������̏I�[)
	buf[size] = '\0';
	fclose(fp);	//�t�@�C�����N���[�Y����

	//������̍Ō�܂ŌJ��Ԃ�
	while (*mpPointer != '\0'){
		GetToken();	//�P��̎擾
		//�P�ꂪFrame�̏ꍇ
		if (strcmp(mToken, "Frame") == 0){
			//�t���[�����쐬����
			new CModelXFrame(this);
		}
		else if (strcmp(mToken, "AnimationSet") == 0){
			new CAnimationSet(this);
		}
	}

	SAFE_DELETE_ARRAY(buf);	//�m�ۂ����̈���J������
}

void CModelX::GetToken(){
	char* p = mpPointer;
	char* q = mToken;
	/*
	strchr(����,������)
	������ɕ������܂܂�Ă���΁A�������������ւ̃|�C���^��Ԃ�
	������Ȃ����NULL��Ԃ�
	*/
	//��( )�^�u(\t)���s(\r)(\n), ; "�̕����Ȃ�ǂݔ�΂�
	while (*p != '\0'&& strchr(" \t\r\n,;\"", *p))p++;

	if (*p == '{' || *p == '}'){
		// { �܂��� } �Ȃ�mToken�ɑ�������̕�����
		*q++ = *p++;
	}
	else{
		//��( )�^�u(\t)���s(\r)(\n), ; "�ȊO�̕����ɂȂ�܂�mToken�ɑ������
		while (*p != '\0' && !strchr(" \t\r\n,;\"}", *p))
			*q++ = *p++;
	}
	*q = '\0';	//mToken�̍Ō��\0����
	mpPointer = p;	//���̓ǂݍ��ރ|�C���g���X�V����

	/*
	strcmp(������1,������2)
	������1�ƕ�����2���������ꍇ�A0��Ԃ��܂�
	������1�ƕ�����2���������Ȃ��ꍇ�A0�ȊO��Ԃ��܂�
	*/
	//����mToken��//�̏ꍇ��,�R�����g�Ȃ̂ŉ��s�܂œǂݔ�΂�
	if (!strcmp("//", mToken)){
		//���s�܂œǂݔ�΂�
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//�ǂݍ��݈ʒu�̍X�V
		mpPointer = p;
		//�P����擾����(�ċA�Ăяo��)
		GetToken();
	}
}

/*SkipNode
�@�m�[�h��ǂݔ�΂�*/
void CModelX::SkipNode(){
	//�������I�������I��
	while (*mpPointer != '\0'){
		GetToken();	//���̒P��擾
		//{�����������烋�[�v�I��
		if (strchr(mToken, '{'))break;
	}
	int count = 1;
	//�������I��邩�A�J�E���g��0�ɂȂ�����I��
	while (*mpPointer != '\0' && count > 0){
		GetToken();	//���̒P��擾
		//{��������ƃJ�E���g�A�b�v
		if (strchr(mToken, '{'))count++;
		//}��������ƃJ�E���g�_�E��
		else if (strchr(mToken, '}'))count--;
	}
}


/* class CModelXFrame*/
/*CModelXFrame
  model::CModelX�C���X�^���X�ւ̃|�C���^
  �t���[�����쐬����
  �ǂݍ��ݒ���Frame��������΁A�t���[�����쐬���A
  �q�t���[���ɒǉ�����*/
CModelXFrame::CModelXFrame(CModelX* model){
	//���݂̃t���[���z��̗v�f�����擾���A�ݒ肷��
	mIndex = model->mFrame.size();

	//CModelX�̃t���[���z��ɒǉ�
	model->mFrame.push_back(this);

	//�ϊ��s���P�ʍs��ɂ���
	mTransformMatrix.Identity();

	//���̒P��(�t���[�����̗\��)���擾
	model->GetToken();	//frame name

	//�t���[�������G���A���m�ۂ���
	mpName = new char[strlen(model->mToken) + 1];

	//�t���[�������R�s�[����
	strcpy(mpName, model->mToken);

	//���̒P��({�̗\��)���擾����
	model->GetToken();	//{

	//�����������Ȃ�����I���
	while (*model->mpPointer != '\0'){
		//���̒P��擾
		model->GetToken();	// Frame
		//}�������̏ꍇ�͏I��
		if (strchr(model->mToken, '}'))break;
		//�V�ȃt���[���̏ꍇ�́A�q�t���[���ɒǉ�
		if (strcmp(model->mToken, "Frame") == 0){
			//�t���[�����쐬���A�q�t���[���̔z��ɒǉ�
			mChild.push_back(
				new CModelXFrame(model));
		}
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0){
			model->GetToken();	// {
			for (int i = 0; i < ARRAY_SIZE(mTransformMatrix.mF); i++){
				mTransformMatrix.mF[i] = model->GetFloatToken();
			}
			model->GetToken();	// }
		}
		else if (strcmp(model->mToken, "Mesh") == 0){
			mMesh.Init(model);
		}
		else{
			//��L�ȊO�̗v�f�͓ǂݔ�΂�
			model->SkipNode();
		}
	}

//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef _DEBUG
	printf("%s\n", mpName);
	mTransformMatrix.Print();
#endif
	
}

/*Render
 ���b�V���̖ʐ���0�ȊO�Ȃ�`�悷��*/
void CModelXFrame::Render(){
	if (mMesh.mFaceNum != 0)
		mMesh.Render();
}


/* class CModelX*/
/*GetFloatToken
  �P��𕂓������_�^�̃f�[�^�ŕԂ�*/
float CModelX::GetFloatToken(){
	GetToken();
	//atof
	//�������float�^�֕ϊ�
	return atof(mToken);
}

/*GetIntToken
�P��𐮐��^�̃f�[�^�ŕԂ�*/
int CModelX::GetIntToken(){
	GetToken();
	return atoi(mToken);
}

/*FindFrame
  �t���[�����ɊY������t���[���̃A�h���X��Ԃ�*/
CModelXFrame* CModelX::FindFrame(char* name){
	//�C�e���[�^�̍쐬
	std::vector<CModelXFrame*>::iterator itr;
	//�擪����Ō�܂ŌJ��Ԃ�
	for (itr = mFrame.begin(); itr != mFrame.end(); itr++){
		//���O����v���Ă��邩
		if (strcmp(name, (*itr)->mpName) == 0){
			//��v���Ă����炻�̃A�h���X��Ԃ�
			return *itr;
		}
	}
	//��v����t���[���������ꍇ��NULL��Ԃ�
	return NULL;
}

/*Init
  Mesh�̃f�[�^����荞��*/
void CMesh::Init(CModelX *model){
	model->GetToken();	// { or ���O
	if (!strchr(model->mToken, '{')){
		//���O�̏ꍇ�A����{
		model->GetToken();	//{
	}
	//���_�̎擾
	mVertexNum = model->GetIntToken();

	printf("VertexNum:%i\n", mVertexNum);
	//���_�����G���A�m��
	mpVertex = new CVector[mVertexNum];
	//���_�����f�[�^����荞��
	for (int i = 0; i < mVertexNum; i++){
		mpVertex[i].mX = model->GetFloatToken();
		mpVertex[i].mY = model->GetFloatToken();
		mpVertex[i].mZ = model->GetFloatToken();
		printf("%10f", mpVertex[i].mX);
		printf("%10f", mpVertex[i].mY);
		printf("%10f\n", mpVertex[i].mZ);
	}
	mFaceNum = model->GetIntToken();	//�ʐϓǂݍ���
	printf("FaceNum:%i\n", mVertexNum);
	//���_����1�ʂ�3���_
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3){
		model->GetToken();	//���_���ǂݔ�΂�
		mpVertexIndex[i] = model->GetIntToken();
		mpVertexIndex[i + 1] = model->GetIntToken();
		mpVertexIndex[i + 2] = model->GetIntToken();
		printf("%3i", mpVertexIndex[i]);
		printf("%3i", mpVertexIndex[i + 1]);
		printf("%3i\n", mpVertexIndex[i + 2]);
	}

	//�����������Ȃ�����I���
	while (model->mpPointer != '\0'){
		model->GetToken();	//MeshNormals
		//}�������̏ꍇ�͏I��
		if (strchr(model->mToken, '}'))
			break;
		if (strcmp(model->mToken, "MeshNormals") == 0){
			model->GetToken();	//{
			//�@���f�[�^�����擾
			mNormalNum = model->GetIntToken();

			//�@���̃f�[�^��z��Ɏ�荞��
			CVector *pNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i++){
				pNormal[i].mX = model->GetFloatToken();
				pNormal[i].mY = model->GetFloatToken();
				pNormal[i].mZ = model->GetFloatToken();

			}

			//�@���� = �ʐ� �~ 3
			mNormalNum = model->GetIntToken() * 3;	//FaceNum
			printf("NormalNum:%i\n", mNormalNum);

			int ni;
			//���_���ɖ@���f�[�^��ݒ肷��
			mpNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i += 3){
				model->GetToken();	//3

				ni = model->GetIntToken();
				mpNormal[i] = pNormal[ni];

				ni = model->GetIntToken();
				mpNormal[i + 1] = pNormal[ni];

				ni = model->GetIntToken();
				mpNormal[i + 2] = pNormal[ni];

				for (int c = 0; c < 3; c++){
					printf("%10f", mpNormal[i + c].mX);
					printf("%10f", mpNormal[i + c].mY);
					printf("%10f\n", mpNormal[i + c].mZ);
				}

			}
			delete[] pNormal;
			model->GetToken();	// }
		}

		//MeshMaterialList�̂Ƃ�
		else if (strcmp(model->mToken, "MeshMaterialList") == 0){
			model->GetToken();	// {
			//Material�̐�
			mMaterialNum = model->GetIntToken();
			// FaceNum
			mMaterialIndexNum = model->GetIntToken();
			// �}�e���A���C���f�b�N�X�̍쐬
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++){
				mpMaterialIndex[i] = model->GetIntToken();
			}
			//�}�e���A���f�[�^�̍쐬
			for (int i = 0; i < mMaterialNum; i++){
				model->GetToken();	// Material
				if (strcmp(model->mToken, "Material") == 0){
					mMaterial.push_back(new CMaterial(model));
				}
			}
			model->GetToken();	// } //End of MeshMaterialList
		}
		//SkinWeights�̂Ƃ�
		else if (strcmp(model->mToken, "SkinWeights") == 0){
			//CSkinWeights�N���X�̃C���X�^���X���쐬���A�z��ɒǉ�
			mSkinWeights.push_back(new CSkinWeights(model));
		}
		else {
			//�ȊO�̃m�[�h�͓ǂݔ�΂�
			model->SkipNode();
		}
	}
}

/* Render
 ��ʂɕ`�悷��*/
void CMesh::Render(){
	/*���_�f�[�^,�@���f�[�^�̔z���L���ɂ���*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	/*���_�f�[�^,�@���f�[�^�̏ꏊ���w�肷��*/
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);

	/*���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��*/
	for (int i = 0; i < mFaceNum; i++){
		//�}�e���A����K�p
		mMaterial[mpMaterialIndex[i]]->Enabled();
		glDrawElements(GL_TRIANGLES, 3,
			GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
	}

	/*���_�f�[�^,�@���f�[�^�̔z��𖳌��ɂ���*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

/* Render
 �S�Ẵt���[���̕`�揈�����Ăяo��*/
void CModelX::Render(){
	for (int i = 0; i < mFrame.size(); i++){
		mFrame[i]->Render();
	}
}


/* class CSkinWeights*/
/*CSkinWeights
�X�L���E�F�C�g�̓ǂݍ���*/
CSkinWeights::CSkinWeights(CModelX *model)
:mpFrameName(0)
, mFrameIndex(0)
, mIndexNum(0)
, mpIndex(nullptr)
, mpWeight(nullptr)
{
	model->GetToken();	//{
	model->GetToken();	// FrameName
	//�t���[�����G���A�m��,�ݒ�
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	printf("SkinWeights\ %s\n", mpFrameName);
	//���_�ԍ����擾
	mIndexNum = model->GetIntToken();
	//���_�ԍ�����0�𒴂���
	if (mIndexNum > 0){
		//���_�ԍ��ƒ��_�E�F�C�g�̃G���A�m��
		mpIndex = new int[mIndexNum];
		mpWeight = new float[mIndexNum];
		//���_�ԍ��擾
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = model->GetIntToken();
		//���_�E�F�C�g�擾
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = model->GetFloatToken();

		for (int i = 0; i < mIndexNum; i++){
			printf("%d", mpIndex[i]);
			printf("%10f\n", mpWeight[i]);
		}
	}
	//�I�t�Z�b�g�s��擾
	for (int i = 0; i < 16; i++){
		mOffset.mF[i] = model->GetFloatToken();
		printf("%10f", mOffset.mF[i]);
		if (i == 3 || i == 7 || i == 11 || i == 15){
			printf("\n");
		}
	}
	model->GetToken();	//}
}


/* class CAnimationSet*/
/*CAnimationSet�@�R���X�g���N�^*/
CAnimationSet::CAnimationSet(CModelX *model)
:mpName(nullptr)
{
	model->mAnimationSet.push_back(this);
	model->GetToken();	// Animation Name
	//�A�j���[�V�����Z�b�g����ޔ�
	mpName = new char[strlen(model->mToken) + 1];
	strcpy(mpName, model->mToken);
	printf("Animation:%s\n", mpName);
	model->GetToken();	// {
	while (*model->mpPointer != '\0'){
		model->GetToken(); // } �܂��� Animation
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "Animation") == 0){
			//Animation�v�f�ǂݍ���
			mAnimation.push_back(new CAnimation(model));
		}
	}
}


/* class CAnimation*/
/*CAnimation �R���X�g���N�^*/
CAnimation::CAnimation(CModelX *model)
:mpFrameName(0)
,mFrameIndex(0)
{
	model->GetToken();	//{ or Animation Name
	if (strchr(model->mToken, '{')){
		model->GetToken();	//{
	}
	else{
		model->GetToken();	//{
		model->GetToken();	//{
	}

	model->GetToken();	//FrameName
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	printf("Animation:%s\n",mpFrameName);
	mFrameIndex = model->FindFrame(model->mToken)->mIndex;
	model->GetToken();	// }

	while (*model->mpPointer != '\0'){
		model->GetToken();	// } �� AnimationKey
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "AnimationKey") == 0){
			model->SkipNode();
		}
	}
}