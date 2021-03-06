#include "CMaterial.h"
//memsetのインクルード
#include <string.h>
#include "glut.h"

//デフォルトコンストラクタ
CMaterial::CMaterial()
:mVertexNum(0)
, mpTextureFilename(nullptr)
{
	//名前を0で埋め
	memset(mName, 0, sizeof(mName));
	//0で埋める
	memset(mDiffuse, 0, sizeof(mDiffuse));
}

/*Materialデータの読み込みと設定*/
CMaterial::CMaterial(CModelX *model)
:mpTextureFilename(0)
{
	//CModelXにマテリアルを追加
	model->mMaterial.push_back(this);
	model->GetToken();	// { ? Name
	if (strcmp(model->mToken, "{") != 0){
		//{ではないときはマテリアル名
		strcpy(mName, model->mToken);
		model->GetToken();	//{
	}
	//printf("%s\n",mName);
	mDiffuse[0] = model->GetFloatToken();
	mDiffuse[1] = model->GetFloatToken();
	mDiffuse[2] = model->GetFloatToken();
	mDiffuse[3] = model->GetFloatToken();
	//printf("Diffuse:%9f", mDiffuse[0]);
	//printf("%9f", mDiffuse[1]);
	//printf("%9f", mDiffuse[2]);
	//printf("%9f\n", mDiffuse[3]);

	mPower = model->GetFloatToken();
	//printf("Power:%f\n", mPower);

	mSpecular[0] = model->GetFloatToken();
	mSpecular[1] = model->GetFloatToken();
	mSpecular[2] = model->GetFloatToken();
	//printf("Specular:%9f", mSpecular[0]);
	//printf("%9f", mSpecular[1]);
	//printf("%9f\n", mSpecular[2]);

	mEmissive[0] = model->GetFloatToken();
	mEmissive[1] = model->GetFloatToken();
	mEmissive[2] = model->GetFloatToken();
	//printf("Emissive:%9f", mEmissive[0]);
	//printf("%9f", mEmissive[1]);
	//printf("%9f\n", mEmissive[2]);

	model->GetToken();	// TextureFilename or }

	if (strcmp(model->mToken, "TextureFilename") == 0){
		//テクスチャありの場合、テクスチャファイル名取得
		model->GetToken();	//{
		model->GetToken();	//File name
		mpTextureFilename = new char[strlen(model->mToken) + 1];
		strcpy(mpTextureFilename, model->mToken);
		
		//テクスチャファイルの読み込み
		mTexture.Load(mpTextureFilename);

		model->GetToken();	// }
		model->GetToken();	// }
	}
}

//マテリアルを有効にする
void CMaterial::Enabled() {
	//拡散光の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	//テクスチャ有り
	if (mTexture.mId)
	{
		//テクスチャを使用可能にする
		glEnable(GL_TEXTURE_2D);
		//テクスチャをバインドする
		glBindTexture(GL_TEXTURE_2D, mTexture.mId);
		//アルファブレンドを有効にする
		glEnable(GL_BLEND);
		//ブレンド方法を指定
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

//テクスチャを読み込む
void CMaterial::LoadTexture(char *file)
{
	mTexture.Load(file);
}

//マテリアルを無効にする
void CMaterial::Disabled()
{
	//テクスチャ有り
	if (mTexture.mId)
	{
		//アルファブレンドを無効
		glDisable(GL_BLEND);
		//テクスチャのバインドを解く
		glBindTexture(GL_TEXTURE_2D, 0);
		//テクスチャを無効にする
		glDisable(GL_TEXTURE_2D);
	}
}
