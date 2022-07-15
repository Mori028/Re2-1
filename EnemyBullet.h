#pragma once
#include <WorldTransform.h>
#include <Model.h>

class EnemyBullet {
public:
	///< summary>
	///初期化
	///</summary>
	///<param name="model"モデル</param>
	///<param name="postion"初期座標</param>
	///<param name="velocity"速度</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

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
	//速度
	Vector3 velocity_;
};