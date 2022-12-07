#include "Player.h"
#include "Function.h"
#include <cassert>
#include <base/WinApp.h>
#include "PrimitiveDrawer.h"

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

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldtransform_.Initialize();
}

void Player::Update()
{
	Vector3 move = { 0,0,0 };

	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		move.y += kCharacterSpeed;

	}
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}
	
	//キーボード入力による移動処理
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, +kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, +kMoveLimitY);


	worldtransform_.translation_ += move;

	//行列更新
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();

	//マウスカーソル
	{
		POINT mousePosition;
		//マウス座標(スクリーン座標)を取得する
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		//マウス座標を2Dレティクルに代入
		sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));



		//ビューポート行列
		Matrix4 Viewport =
		{ 1280 / 2,0,0,0,
		0,-720 / 2,0,0,
		0,0,1,0,
		1280 / 2, 720 / 2,0,1 };

		// ビュープロジェクションビューポート合成
		Matrix4 matVPV = viewProjection_.matView *= viewProjection_.matProjection *= Viewport;

		//合成行列の逆行列を計算する
		Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);

		//スクリーン座標			  ↓マウスかコントローラーか
		Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
		Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

		//スクリーン座標系からワールド座標系へ
		posNear = Affin_->WDivision(matInverseVPV, posNear);
		posFar = Affin_->WDivision(matInverseVPV, posFar);

		//マウスレイの方向
		Vector3 mouseDirection = posFar -= posNear;

		float len = sqrt(mouseDirection.x * mouseDirection.x + mouseDirection.y * mouseDirection.y + mouseDirection.z * mouseDirection.z);
		if (len != 0)
		{
			mouseDirection /= len;
		}

		//カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 180.0f;
		worldTransform3DReticle_.translation_ = posNear += (mouseDirection *= kDistanceTestObject);

		//行列更新
		worldtransform_.matWorld_ = CreateMatIdentity();
		worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
		worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
		worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
		worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
		worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
		worldtransform_.TransferMatrix();


	}
}

void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

}
