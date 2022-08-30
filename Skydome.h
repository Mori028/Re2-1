#pragma once
#include <3d/WorldTransform.h>
#include <3d/Model.h>

class Skydome
{
public:

	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection_);
	
private:

	//ワールド変換データ
	WorldTransform worldtransform_;
	//モデル
	Model* model_ = nullptr;
	
};

