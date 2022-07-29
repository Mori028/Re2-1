#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

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

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
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
};