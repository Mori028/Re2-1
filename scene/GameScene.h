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
#include "Skydome.h"
#include "RailCamera.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public:
	//パーツID
	enum Partid {
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
	};
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

	void CheckAllcollisions();

	//敵弾を追加する
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemybullets_; }

public: // メンバ関数
/// <summary>
/// コンストクラタ
/// </summary>
	GameScene();

	//3Dモデル
	Model* model_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;


	//ビュープロジェクション
	ViewProjection viewProjection_;

	//カメラ上方向の角度
	float viewAngle = 4.0f;

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;

	//弾 複数
	std::list<std::unique_ptr<EnemyBullet>> enemybullets_;


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	
};