#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include "DebugText.h"
#include "Input.h"
#include "ViewProjection.h"
#include <PlayerBullet.h>
#include <memory> 
#include <list>

/// <summary>
/// 自キャラ
/// </summary>

class Player {
public:
	/// <summary>
	///初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	///更新
	/// </summary>
	void Update(ViewProjection viewProjection);

	/// <summary>
	///描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	///< summary>
	///初期化
	///</summary>
	void Attack();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾のリストを取得
	const std::list < std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	float GetRadius();

	void SetParent(WorldTransform* worldTransform) {
		worldtransform_.parent_ = worldTransform;
	}
private:
	//ワールド変換データ
	WorldTransform worldtransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	
	const float radius_ = 1.0f;

	//3Dレティクル用トランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite>sprite2DReticle_;
};