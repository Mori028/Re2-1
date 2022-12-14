#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "AxisIndicator.h"
#include "Player.h"
#include "Enemy.h"
#include <sstream>
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	//3Dモデル
	Model* model_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;
	
	// 敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//カメラ上方向の角度
	float viewAngle = 4.0f;

public:
	//パーツID
	/*enum Partid {
		kRoot,
		kSpine,
		kChest,
		kHead,
		kArmL,
		kArmR,
		kHip,
		kLegL,
		kLegR,

		kNumPartid
	};*/
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	float Angle(float angle);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdataEnemyPopCommands();

	/// <summary>
	/// 敵の発生
	/// </summary>
	void GenerEnemy(Vector3 EnemyPos);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	Vector3 vector3(float x, float y, float z);
	Vector4 vector4(int x, int y, int z, int w);

	//待機中フラグ
	bool isStand_ = false;

	//待機タイマー
	int standTime_ = 0;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};