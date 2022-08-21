#include "Player.h"
#include "Function.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
    // NULLポインタチェック
    assert(model);

    // メンバ変数に代入
    model_ = model;
    textureHandle_ = textureHandle;

    // シングルトンインスタンスを取得する
    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();

    // ワールド変換の初期化
    worldTransform_.Initialize();

    // 弾を発射
    Attack();
}

void Player::Update() {

    // デスフラグの立った弾を削除
    bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
        return bullet->isDead();
        });

    // キャラクターの移動ベクトル
    Vector3 move = { 0,0,0 };

    //キャラクターの移動の速さ
    const float characterSpeed = 0.3f;

    // 移動ベクトルを変更する処理
    if (input_->PushKey(DIK_LEFT)) {
        move.x -= characterSpeed;
    }
    if (input_->PushKey(DIK_RIGHT)) {
        move.x += characterSpeed;
    }
    if (input_->PushKey(DIK_UP)) {
        move.y += characterSpeed;
    }
    if (input_->PushKey(DIK_DOWN)) {
        move.y -= characterSpeed;
    }

    // 座標移動(ベクトルの加算)

    // 移動限界座標
    const float kMoveLimitX = 35;
    const float kMoveLimitY = 19;

    // 範囲を超えない処理worldTrandform_.translation_値に制限をかける
    worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
    worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
    worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
    worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
    worldTransform_.translation_ += move;

    // 行列更新
    worldTransform_.matWorld_ = CreateMatIdentity();
    worldTransform_.matWorld_ *= CreateMatScale(worldTransform_.scale_);
    worldTransform_.matWorld_ *= CreateMatRotationX(worldTransform_.rotation_);
    worldTransform_.matWorld_ *= CreateMatRotationY(worldTransform_.rotation_);
    worldTransform_.matWorld_ *= CreateMatRotationZ(worldTransform_.rotation_);
    worldTransform_.matWorld_ *= CreateMatTranslation(worldTransform_.translation_);
    worldTransform_.TransferMatrix();

    //キャラクター旋回処理
     //押した方向で移動ベクトルを変更
    const float kCharacterRootSpeed = 0.02f;

    if (input_->PushKey(DIK_U)) {
        worldTransform_.rotation_.y -= kCharacterRootSpeed;
    }
    else if (input_->PushKey(DIK_I)) {
        worldTransform_.rotation_.y += kCharacterRootSpeed;
    }

    // キャラクター攻撃処理
    Attack();

    // 弾更新
    for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
        bullet->Update();
    }
}

void Player::Draw(ViewProjection viewProjection_) {
    model_->Draw(worldTransform_, viewProjection_, textureHandle_);

    // 弾描画
    for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
        bullet->Draw(viewProjection_);
    }
}

void Player::Attack() {
    if (input_->PushKey(DIK_SPACE)) {
        // 弾の速度
        const float kBulletSpeed = 1.0f;
        Vector3 velocity(0, 0, kBulletSpeed);

        // 速度ベクトルを自機の向きに合わせて回転させる
        velocity = bVelocity(velocity, worldTransform_);

        // 自キャラの座標をコピー
        Vector3 position = worldTransform_.translation_;

        // 弾を生成し、初期化
        std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
        newBullet->Initialize(model_, position, velocity);

        // 球を登録する
        bullets_.push_back(std::move(newBullet));
    }
}

Vector3  Player::GetWorldPosition() {
    //ワールド座標を入れる変数
    Vector3 worldPos;
    //ワールド行列の平行移動成分を取得(ワールド座標)
    worldPos.x = worldTransform_.matWorld_.m[3][0];
    worldPos.y = worldTransform_.matWorld_.m[3][1];
    worldPos.z = worldTransform_.matWorld_.m[3][2];
    return worldPos;
}

// 衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision() {}

float Player::GetRadius() { return radius_; }