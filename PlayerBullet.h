#pragma once
#include <WorldTransform.h>
#include <Model.h>

class PlayerBullet {
public:
	///< summary>
	///初期化
	///</summary>
	///<param name="model"モデル</param>
	///<param name="postion"初期座標</param>
	///<param name="velocity"速度</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	///< summary>
	///初期化
	///</summary>
	void Update();

	///< summary>
	///初期化
	///</summary>
	void Draw(const ViewProjection& viewProjection);

public:

	bool isDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldtransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;

	//寿命＜frm＞
	static const int32_t KLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = KLifeTime;
	//デスフラグ
	bool isDead_ = false;

};