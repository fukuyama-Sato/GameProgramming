#ifndef CMODELX_H
#define CMODELX_H

#define MODEL_FILE "ラグナ.x"	//入力ファイル名

//領域解放をマクロ化
#define SAFE_DELETE_ARRAY(a) {if(a)delete[] a; a = 0;}
//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))

#include <vector>
#include "CMatrix.h"
#include "CVector.h"

class CModelX;
class CMaterial;

/* CSkinWeights
スキンウェイトクラス*/
class CSkinWeights{
public:
	char *mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号
	int mIndexNum;		//頂点番号数
	int *mpIndex;		//頂点番号配列
	float *mpWeight;	//頂点ウェイト配列
	CMatrix mOffset;	//オフセットマトリックス

	CSkinWeights(CModelX *model);

	~CSkinWeights(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}
};

/* CAnimationKey
アニメーションキークラス*/
class CAnimationKey{
public:
	//時間
	float mTime;
	//行列
	CMatrix mMatrix;
};

/* CAnimation
アニメーションクラス*/
class CAnimation{
public:
	char *mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号

	int mKeyNum;	//キー数(時間数)
	CAnimationKey *mpKey;	//キーの配列

	CAnimation(CModelX *model);

	~CAnimation(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpKey);
	}
};

/* CAnimationSet
アニメーションセット*/
class CAnimationSet{
public:
	//アニメーションセット名
	char *mpName;

	float mTime;	//現在時間
	float mWeight;	//重み
	float mMaxTime;	//最大時間

	CAnimationSet(CModelX *model);
	std::vector<CAnimation*> mAnimation;

	~CAnimationSet(){
		SAFE_DELETE_ARRAY(mpName);
		//アニメーション要素の削除
		for (int i = 0; i < mAnimation.size(); i++){
			delete mAnimation[i];
		}
	}
};

/* CMesh
 メッシュクラス*/
class CMesh{
public:
	int mVertexNum;		//頂点数
	CVector *mpVertex;	//頂点データ

	int mFaceNum;	//面積
	int *mpVertexIndex;	//面を構成する頂点番号

	int mNormalNum;	  //法線数
	CVector *mpNormal;//法線データ

	int mMaterialNum;		//マテリアル数
	int mMaterialIndexNum;	//マテリアル番号数(面数)
	int *mpMaterialIndex;	//マテリアル番号
	std::vector<CMaterial*> mMaterial;	//マテリアルデータ

	CVector *mpAnimateVertex;	//アニメーション用頂点
	CVector *mpAnimateNormal;	//アニメーション用法線

	//スキンウェイト
	std::vector<CSkinWeights*>mSkinWeights;

	//テクスチャ座標データ
	float *mpTextureCoords;

	//コンストラクタ
	CMesh()
		: mVertexNum(0)
		, mpVertex(nullptr)

		, mFaceNum(0)
		, mpVertexIndex(nullptr)

		, mNormalNum(0)
		, mpNormal(nullptr)

		, mMaterialNum(0)
		, mMaterialIndexNum(0)
		, mpMaterialIndex(nullptr)

		, mpAnimateVertex(nullptr)
		, mpAnimateNormal(nullptr)

		, mpTextureCoords(nullptr)
	{}
	//デストラクタ
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		SAFE_DELETE_ARRAY(mpAnimateVertex);
		SAFE_DELETE_ARRAY(mpAnimateNormal);
		SAFE_DELETE_ARRAY(mpTextureCoords);
		//スキンウェイトの削除
		for (int i = 0; i < mSkinWeights.size(); i++){
			delete mSkinWeights[i];
		}
	}

	//読み込み処理
	void Init(CModelX *model);

	void AnimateVertex(CModelX *model);

	void Render();
};

/* CModelXFrame
 モデルXフレームクラス*/
class CModelXFrame{
public:
	std::vector<CModelXFrame*>mChild;	//子フレームの配列
	CMatrix mTransformMatrix;	//変換行列
	CMatrix mCombinedMatrix;
	CMesh mMesh;	//Meshデータ

	char* mpName;	//フレーム名前
	int mIndex;		//フレーム番号
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame(){
		//子フレームを全て解放する
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++){
			delete *itr;
		}
		//名前のエリアを解放する
		SAFE_DELETE_ARRAY(mpName);
	}
	void AnimateCombined(CMatrix* parent);

	void Render();
};

/* CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する*/
class CModelX {
public:

	char* mpPointer;	//読み込み位置
	char mToken[1024];	//取り出した単語の領域
	std::vector<CModelXFrame*>mFrame;	//フレームの配列

	//アニメーションセットの配列
	std::vector<CAnimationSet*> mAnimationSet;

	std::vector<CMaterial*> mMaterial;	//マテリアルの配列

	CModelX()
	:mpPointer(nullptr)
	{}

	~CModelX(){
		if (mFrame.size() > 0){
			delete mFrame[0];
		}
		for (int i = 0; i < mAnimationSet.size(); i++){
			delete mAnimationSet[i];
		}
		for (int i = 0; i < mMaterial.size(); i++){
			delete mMaterial[i];
		}
	}

	//ファイル読み込み
	void Load(char* file);
	//単語の取り出し
	void GetToken();
	//ノードの読み飛ばし
	void SkipNode();
	//描画
	void Render();

	void AnimateFrame();

	void AnimateVertex();

	void SetSkinWeightFrameIndex();

	//マテリアルの検索
	CMaterial* FindMaterial(char *name);


	//浮動小数点データの取得
	float GetFloatToken();
	//整数データの取得
	int GetIntToken();
	//フレーム名に該当するフレームのアドレスを返す
	CModelXFrame* FindFrame(char* name);

};


#endif