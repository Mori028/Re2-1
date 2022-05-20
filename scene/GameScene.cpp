#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#define PI 3.14

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//	デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	// ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	// 単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	// X, Y, Z 方向のスケーリングを設定
	worldTransform_.scale_ = { 5 ,5 ,5 };
	// X, Y, Z 軸の周りの回転角を設定
	worldTransform_.rotation_ = { Angle(45.0), Angle(45.0), Angle(45.0) };
	// 合成回転行列を宣言                     // 3.14f / 4
	Matrix4 matRot = matIdentity;
	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// 各軸の回転行列を合成
	/*matRot = matRotZ * matRotX * matRotY;*/

	// X,Y,Z 軸周りの平行移動を設定
	worldTransform_.translation_ = { 0 ,10 ,0 };
	// 平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	// スケーリング行列を宣言
	Matrix4 matScale;

	/*
		Sx ,0  ,0  ,0
		0  ,Sy ,0  ,0
		0  ,0  ,Sz ,0
		0  ,0  ,0  ,1
	*/

	matScale.m[0][0] = worldTransform_.scale_.x; // Sx
	matScale.m[1][1] = worldTransform_.scale_.y; // Sy
	matScale.m[2][2] = worldTransform_.scale_.z; // Sz
	matScale.m[3][3] = 1;

	/*
		cos  ,sin ,0 ,0
		-sin ,cos ,0 ,0
		0    ,0   ,1 ,0
		0    ,0   ,0 ,1
	*/

	// Z軸の回転行列を設宣言

	matRotZ = matIdentity;
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	matRotX = matIdentity;
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);

	matRotY = matIdentity;
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);

	// Z.X.Y 軸回転行列の各要素を設定する
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	// 移動量を行列に設定
	matTrans.m[0][2] = 0;
	matTrans.m[1][2] = 0;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	matTrans.m[1][0] = 0;
	matTrans.m[2][0] = 0;
	matTrans.m[3][0] = worldTransform_.translation_.x;

	matTrans.m[0][1] = 0;
	matTrans.m[2][1] = 0;
	matTrans.m[3][1] = worldTransform_.translation_.y;

	matTrans.m[0][3] = 0;
	matTrans.m[1][3] = 0;
	matTrans.m[3][3] = 1;

	worldTransform_.matWorld_ = matIdentity;
	worldTransform_.matWorld_ *= matScale;
	/*worldTransform_.matWorld_ *= matRotZ;*/
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	// 行列の転送
	worldTransform_.TransferMatrix();
}

void GameScene::Update() {
	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/*model_->Draw(worldTransform_, viewProjection_, textureHandle_);*/
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// ライン描画が参照するビュープロジェクションを指定(アドレス渡し)
	/*PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, { 0,0,10 }, { 0,0,1,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, { 10,0,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, { 0,10,0 }, { 0,1,0,1 });*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}