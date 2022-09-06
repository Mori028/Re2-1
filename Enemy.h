#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "EnemyBullet.h"
#include "Player.h"
#include <memory>
#include <list>

//自機クラスの前方宣言
class Player;

class GameScene;

class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name = "model">モデル</param>
	/// <param mame = "textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjectione">ビュープロジェクション（参照渡し）</param>
	void Draw(ViewProjection viewProjection_);

	/// <summary>
	/// 弾の発射
	/// </summary>
	void Fire();

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	// 発射間隔
	static const int kFireInterval = 60;

	// 接近フェーズの初期化
	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	
	float GetRadius();

public:

	bool isDead() const { return isDead_; }
private:
	// ワールド変換データ
	WorldTransform worldtransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 入力処理するため
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;
	// フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// 接近フェーズの更新
	void AccessPhaseUpdate();
	// 離脱フェーズの更新
	void EliminationPhaseUpdate();
	// 発射タイマー
	int32_t fireTimer = 0;

	//自キャラ
	Player* player_ = nullptr;

	// デスフラグ
	bool isDead_ = false;

	//半径
	const float radius_ = 1.0f;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

};