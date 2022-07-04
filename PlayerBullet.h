#pragma once
#include <WorldTransform.h>
#include <Model.h>

class PlayerBullet {
public:
	///< summary>
	///初期化
	///</summary>
	void Initialize(Model* model, const Vector3& position);

	///< summary>
	///初期化
	///</summary>
	void Update();

	///< summary>
	///初期化
	///</summary>
	void Draw(const ViewProjection& viewProjection);


private:
	//ワールド変換データ
	WorldTransform worldtransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};