#include "CMaterial.h"
//memset�̃C���N���[�h
#include <string.h>
#include "glut.h"

//�f�t�H���g�R���X�g���N�^
CMaterial::CMaterial()
:mVertexNum(0)
, mpTextureFilename(nullptr)
{
	//���O��0�Ŗ���
	memset(mName, 0, sizeof(mName));
	//0�Ŗ��߂�
	memset(mDiffuse, 0, sizeof(mDiffuse));
}

/*Material�f�[�^�̓ǂݍ��݂Ɛݒ�*/
CMaterial::CMaterial(CModelX *model)
:mpTextureFilename(0)
{
	model->GetToken();	// { ? Name
	if (strcmp(model->mToken, "{") != 0){
		//{�ł͂Ȃ��Ƃ��̓}�e���A����
		strcpy(mName, model->mToken);
		model->GetToken();	//{
	}
	printf("Material\n");
	mDiffuse[0] = model->GetFloatToken();
	mDiffuse[1] = model->GetFloatToken();
	mDiffuse[2] = model->GetFloatToken();
	mDiffuse[3] = model->GetFloatToken();
	printf("Diffuse:%9f", mDiffuse[0]);
	printf("%9f", mDiffuse[1]);
	printf("%9f", mDiffuse[2]);
	printf("%9f\n", mDiffuse[3]);

	mPower = model->GetFloatToken();
	printf("Power:%f\n", mPower);

	mSpecular[0] = model->GetFloatToken();
	mSpecular[1] = model->GetFloatToken();
	mSpecular[2] = model->GetFloatToken();
	printf("Specular:%9f", mSpecular[0]);
	printf("%9f", mSpecular[1]);
	printf("%9f\n", mSpecular[2]);

	mEmissive[0] = model->GetFloatToken();
	mEmissive[1] = model->GetFloatToken();
	mEmissive[2] = model->GetFloatToken();
	printf("Emissive:%9f", mEmissive[0]);
	printf("%9f", mEmissive[1]);
	printf("%9f\n", mEmissive[2]);

	model->GetToken();	// TextureFilename or }

	if (strcmp(model->mToken, "TextureFilename") == 0){
		//�e�N�X�`������̏ꍇ�A�e�N�X�`���t�@�C�����擾
		model->GetToken();	//{
		model->GetToken();	//File name
		mpTextureFilename = new char[strlen(model->mToken) + 1];
		strcpy(mpTextureFilename, model->mToken);
		model->GetToken();	// }
		model->GetToken();	// }
	}
}

//�}�e���A����L���ɂ���
void CMaterial::Enabled() {
	//�g�U���̐ݒ�
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	//�e�N�X�`���L��
	if (mTexture.mId)
	{
		//�e�N�X�`�����g�p�\�ɂ���
		glEnable(GL_TEXTURE_2D);
		//�e�N�X�`�����o�C���h����
		glBindTexture(GL_TEXTURE_2D, mTexture.mId);
		//�A���t�@�u�����h��L���ɂ���
		glEnable(GL_BLEND);
		//�u�����h���@���w��
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

//�e�N�X�`����ǂݍ���
void CMaterial::LoadTexture(char *file)
{
	mTexture.Load(file);
}
//�}�e���A���𖳌��ɂ���
void CMaterial::Disabled()
{
	//�e�N�X�`���L��
	if (mTexture.mId)
	{
		//�A���t�@�u�����h�𖳌�
		glDisable(GL_BLEND);
		//�e�N�X�`���̃o�C���h������
		glBindTexture(GL_TEXTURE_2D, 0);
		//�e�N�X�`���𖳌��ɂ���
		glDisable(GL_TEXTURE_2D);
	}
}
