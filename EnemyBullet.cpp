#include "EnemyBullet.h"
#include "Function.h"
#include <assert.h>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("shot.png");

	// ワールドトランスフォームの初期化
	worldtransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldtransform_.translation_ = position;
}

void EnemyBullet::Update() {
	
	// 座標を移動させる（１フレーム分の移動量を足しこむ）
	worldtransform_.translation_ += velocity_;
	//行列更新
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ワールドトランスフォームの更新
	CreateMatrixUpdate(worldtransform_);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldtransform_, viewProjection, textureHandle_);
}

Vector3  EnemyBullet::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;

}

void EnemyBullet::OnCollision() {

	//デスフラグ
	isDead_ = true;
}

float EnemyBullet::GetRadius() { return radius_; }
