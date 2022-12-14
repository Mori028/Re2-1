﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include <random>
#include "Function.h"
#include <fstream>
#define PI 3.14

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}

float Degree(const float& degree) {
	float n = degree * PI / 180;
	return n;
}

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲の指定
std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
std::uniform_real_distribution<float> rotDist(0.0f, 3.14f);



//Matrix4 CreateMatRot(Vector3 Rotation) {
//	Matrix4 Rot = CreateMatIdentity();
//	Rot *= CreateMatRotationZ(worldTransform.rotation_);
//	Rot *= CreateMatRotationY(worldTransform.rotation_);
//	Rot *= CreateMatRotationX(worldTransform.rotation_);
//
//	return Rot;
//}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//ビュープロジェクション
	viewProjection_.Initialize();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(600, 400);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	
	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);

	//敵キャラの生成
	enemy_ = new Enemy();

	//敵キャラの初期化
	enemy_->Initialize(model_, textureHandle_);

	//ファイルの読み込み
	LoadEnemyPopData();
}

void GameScene::Update() {
	//デバックカメラの更新
	debugCamera_->Update();

	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= 0.5f;
		}
		if (input_->PushKey(DIK_RIGHT)) {
			move.x += 0.5f;
		}
	}

	//自キャラの更新
	player_->Update();
	//敵発生
	UpdataEnemyPopCommands();
	//自キャラの更新
	enemy_->Update();
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
	
	//自キャラの描画
	player_->Draw(viewProjection_);
	//敵キャラの描画
	enemy_->Draw(viewProjection_);
	////3Dモデル
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

void GameScene::LoadEnemyPopData() {

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");

	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

}

void GameScene::UpdataEnemyPopCommands()
{
	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			/*GenerEnemy(Vector3(x, y, z));*/
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			standTime_ = waitTime;

			//ループを抜ける
			break;
		}
	}

}

//void GameScene::GenerEnemy(Vector3 EnemyPos)
//{
//	//敵キャラの生成
//	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
//	//敵キャラの初期化
//	newEnemy->Initialize(model_, textureHandle_);
//
//	//敵キャラにアドレスを渡す
//	newEnemy->SetPlayer(player_);
//
//	//リストに登録する
//	enemys_.push_back(std::move(newEnemy));
//}
Vector3 GameScene::vector3(float x, float y, float z) { return Vector3(x, y, z); }

Vector4 GameScene::vector4(int x, int y, int z, int w) { return Vector4(x, y, z, w); }