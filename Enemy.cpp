#include "Enemy.h"
#include <cassert>
#include <Function.h>


void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("raiden.jpg");
	//ワールド変換の初期化
	worldtransform_.Initialize();
}

void Enemy::Update()
{
	

	Vector3 move = { 0,0,0 };

	//敵の移動の速さ
	const float enemySpeed = 0.03f;
	//敵の移動処理
	move = { 0.0f, 0.0f,enemySpeed };
	/*worldtransform_.translation_ += move;*/

	//行列更新
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();

	
	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
	    //移動（ベクトルを加算）
	    worldtransform_.translation_ -= move;
	    //規定の位置に到達したら離脱
	    if (worldtransform_.translation_.z < -10.0f) {
	        phase_ = Enemy::Phase::Leave;
	    }
	    break;

	case Enemy::Phase::Leave:
	    //移動（べクトルを加算）
		worldtransform_.translation_ += {0.02,0.02,0};
	    break;

	}
}

void Enemy::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Enemy::Attack() {

	if (input_->PushKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldtransform_);

		//弾を生成し初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldtransform_.translation_, velocity);

		//弾の登録
		bullets_.push_back(std::move(newBullet));

	}
}

void Enemy::Fire()
{
}

