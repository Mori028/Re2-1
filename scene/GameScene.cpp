#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#define PI 3.14
#include <random>
//スケーリング行列
Matrix4 CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
	return matScale;
}
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲の指定
std::uniform_real_distribution<float> dist(-5.0f,5.0f);
//乱数エンジンを渡し、指定範囲からランダムな数値を得る
float value = dist(engine);
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = 5*(10.0f);
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//	デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());


	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0, 2*PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	//範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		// X, Y, Z 方向のスケーリングを設定
		worldTransform.scale_ = {1 ,1 ,1};

		// X, Y, Z 軸の周りの回転角を設定
		worldTransform.rotation_ = { rotDist(engine), rotDist(engine), rotDist(engine) };

		// X,Y,Z 軸周りの平行移動を設定
		worldTransform.translation_ = { posDist(engine) ,posDist(engine) ,posDist(engine) };

		// 単位行列
		Matrix4 matIdentity;
		matIdentity.m[0][0] = 1;
		matIdentity.m[1][1] = 1;
		matIdentity.m[2][2] = 1;
		matIdentity.m[3][3] = 1;

		// 合成回転行列を宣言                     // 3.14f / 4
		Matrix4 matRot = matIdentity;
		// 各軸用回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;

		// 各軸の回転行列を合成
		/*matRot = matRotZ * matRotX * matRotY;*/


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

		matScale.m[0][0] = worldTransform.scale_.x; // Sx
		matScale.m[1][1] = worldTransform.scale_.y; // Sy
		matScale.m[2][2] = worldTransform.scale_.z; // Sz
		matScale.m[3][3] = 1;

		/*
			cos  ,sin ,0 ,0
			-sin ,cos ,0 ,0
			0    ,0   ,1 ,0
			0    ,0   ,0 ,1
		*/

		// Z軸の回転行列を設宣言

		matRotZ = matIdentity;
		matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform.rotation_.z);

		matRotX = matIdentity;
		matRotX.m[1][1] = cos(worldTransform.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform.rotation_.x);

		matRotY = matIdentity;
		matRotY.m[0][0] = cos(worldTransform.rotation_.y);
		matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matRotY.m[2][0] = sin(worldTransform.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform.rotation_.y);

		// Z.X.Y 軸回転行列の各要素を設定する
		matRot *= matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;

		// 移動量を行列に設定
		matTrans.m[0][2] = 0;
		matTrans.m[1][2] = 0;
		matTrans.m[3][2] = worldTransform.translation_.z;

		matTrans.m[1][0] = 0;
		matTrans.m[2][0] = 0;
		matTrans.m[3][0] = worldTransform.translation_.x;

		matTrans.m[0][1] = 0;
		matTrans.m[2][1] = 0;
		matTrans.m[3][1] = worldTransform.translation_.y;

		matTrans.m[0][3] = 0;
		matTrans.m[1][3] = 0;
		matTrans.m[3][3] = 1;

		worldTransform.matWorld_ = matIdentity;
		worldTransform.matWorld_ *= matScale;
		/*worldTransform_.matWorld_ *= matRotZ;*/
		worldTransform.matWorld_ *= matRot;
		worldTransform.matWorld_ *= matTrans;

		// 行列の転送
		worldTransform.TransferMatrix();
	}	
	
}

void GameScene::Update() {
	debugCamera_->Update();

	////視点移動処理
	//{
	//	//視点の移動ベクトル
	//	Vector3 move = { 0,0,0 };

	//	//視点の移動速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move.z += kEyeSpeed;
	//	}
	//	else if (input_->PushKey(DIK_S)) {
	//		move.z -= kEyeSpeed;
	//	}
	//	//視点移動（ベクトルの加算）
	//	viewProjection_.eye += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();
	//	//デバッグ用表示
	//	debugText_->SetPos(50, 50);
	//	debugText_->Printf(
	//		"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//}
	////注視点移動処理
	//{
	//	//視注点の移動ベクトル
	//	Vector3 move = { 0,0,0 };

	//	//視点の移動速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move.x += kTargetSpeed;
	//	}
	//	else if (input_->PushKey(DIK_RIGHT)) {
	//		move.x -= kTargetSpeed;
	//	}
	//	//視点移動（ベクトルの加算）
	//	viewProjection_.target += move;

	//	//行列の再計算fw
	//	viewProjection_.UpdateMatrix();
	//	//デバッグ用表示
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf(
	//	"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	//}
	////上方向回転処理
	//{
	//	
	//	//上方向の回転速さ
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;
	//		//2πを超えたら０に戻す
	//		viewAngle = fmodf(viewAngle, PI * 2.0f);
	//	}
	//	//上方向ベクトルを計算（半径１の円周上の座標）
	//	viewProjection_.up = {cosf(viewAngle),sinf(viewAngle),0.0f};

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();
	//	//デバッグ用表示
	//	debugText_->SetPos(50, 90);
	//	debugText_->Printf(
	//		"up:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//}
	//Fov変更処理
	{
		//上キーで視野角が上がる
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += 1;
			viewProjection_.fovAngleY <= PI;
		}
		if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= 1;
			viewProjection_.fovAngleY > 0.0;
		}
		//行列の再計算
		viewProjection_.UpdateMatrix();
		//デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree):%f", (viewProjection_.fovAngleY));
	}
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
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform,viewProjection_, textureHandle_);
	}

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