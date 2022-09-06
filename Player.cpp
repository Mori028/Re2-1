#include "Player.h"
#include "Function.h"
#include <cassert>

/// <summary>
/// 初期化
/// 引数からモデルデータとテクスチャデータを受け取る
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle){

	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("player.png");

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldtransform_.translation_.z = 50;
	worldtransform_.Initialize();
}

void Player::Update()
{
	//デスフラグの立った弾の削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->isDead();
		});


	Vector3 move = { 0,0,0 };

	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	//行列更新
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.matWorld_ *= worldtransform_.parent_->matWorld_;
	worldtransform_.TransferMatrix();
	
	//キーボード入力による移動処理
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, +kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, +kMoveLimitY);


	worldtransform_.translation_ += move;

	//キャラクター旋回処理
	//押した方向で移動ベクトルを変更
	const float kCharacterRootSpeed = 0.02f;

	if (input_->PushKey(DIK_U)) {
		worldtransform_.rotation_.y -= kCharacterRootSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldtransform_.rotation_.y += kCharacterRootSpeed;
	}

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldtransform_);

		Vector3 position = GetWorldPosition();
		//弾を生成し初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position,velocity);

		//弾の登録
		bullets_.push_back(std::move(newBullet));
	}

}
Vector3  Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;

}

void Player::OnCollision(){}

float Player::GetRadius() { return radius_; }
