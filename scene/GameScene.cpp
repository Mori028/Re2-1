#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include <random>
#include "Function.h"
#define PI 3.14
#include <fstream>

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
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	/*textureHandle_ = TextureManager::Load("mario.jpg");*/
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
	player_->Initialize(model_, textureHandle_);
	player_->SetParent(railCamera_->GetWolrdMatrix());
	//天球の生成
	skydome_ = new Skydome();

	

	//天球の初期化
	skydome_->Initialize();

	//レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	
	railCamera_->Initialize(Vector3(0, 0, -50), Vector3(0, 0, 0));

	player_->SetParent(railCamera_->GetWolrdMatrix());

	//ファイルの読み込み
	LoadEnemyPopData();
}

void GameScene::Update() {

	//デスフラグの立った弾の削除
	enemybullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->isDead();
		});
	
	//デバックカメラの更新
	railCamera_->Update();
	//railCameraをゲームシーンの方に適応する
	viewProjection_.matView = railCamera_->getViewProjection().matView;
	viewProjection_.matProjection = railCamera_->getViewProjection().matProjection;
	viewProjection_.TransferMatrix();
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

	//敵の更新
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->SetGameScene(this);
		enemy_->Update();
	}

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
		bullet->Update();
	}
	//天球の更新

	CheckAllcollisions();
	skydome_->Update();
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

	skydome_->Draw(viewProjection_);

	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->Draw(viewProjection_);
	}

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
		bullet->Draw(viewProjection_);;
	}
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

void GameScene::CheckAllcollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list < std::unique_ptr <PlayerBullet>>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list < std::unique_ptr <EnemyBullet>>& enemyBullets = GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//AとBの距離を求める
		Vector3 len = Vector3sub(posA, posB);
		float distance = length(len);

		//自キャラ敵弾の半径
		float radius = player_->GetRadius() + bullet->GetRadius();

		//自キャラと敵弾の交差判定
		if (distance <= radius) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		posA = enemy_->GetWorldPosition();
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//敵弾の座標
			posB = bullet.get()->GetWorldPosition();

			//AとBの距離を求める
			Vector3 len = Vector3sub(posA, posB);
			float distance = length(len);

			//自弾と敵キャラの半径
			float radius = bullet->GetRadius() + enemy_->GetRadius();

			//自キャラと敵弾の交差判定
			if (distance <= radius) {
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				//敵キャラの衝突時コールバックを呼び出す
				enemy_->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& bulletB : enemyBullets) {

			//自弾の座標
			posA = bulletA.get()->GetWorldPosition();
			//敵弾の座標
			posB = bulletB.get()->GetWorldPosition();

			Vector3 len = Vector3sub(posA, posB);
			//座標AとBの距離を求める
			float distance = length(len);

			//自弾と敵弾の半径
			float radius = bulletA->GetRadius() + bulletB->GetRadius();

			//自弾と敵弾の交差判定
			if (distance <= radius) {
				bulletA->OnCollision();
				bulletB->OnCollision();
			}
		}
	}
#pragma endregion


}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet) {
	//リストに登録する
	enemybullets_.push_back(std::move(enemyBullet));
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

			GenerEnemy(Vector3(x, y, z));
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

void GameScene::GenerEnemy(Vector3 EnemyPos)
{
	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//敵キャラの初期化
	newEnemy->Initialize(model_, textureHandle_);

	//敵キャラにアドレスを渡す
	newEnemy->SetPlayer(player_);

	//リストに登録する
	enemys_.push_back(std::move(newEnemy));
}
Vector3 GameScene::vector3(float x, float y, float z) { return Vector3(x, y, z); }

Vector4 GameScene::vector4(int x, int y, int z, int w) { return Vector4(x, y, z, w); }
