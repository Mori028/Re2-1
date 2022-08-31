#pragma once
#include <3d/WorldTransform.h>
#include <3d/ViewProjection.h>
#include "DebugText.h"
#include "Input.h"

class RailCamera
{
public:
	
	void Initialize(const Vector3 position, const Vector3 rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	const ViewProjection& getViewProjection() { return viewProjection_; }

	//ワールド行列を取得
	WorldTransform* GetWolrdMatrix() { return &worldtransform_; }

private:

	//ワールド変換データ
	WorldTransform worldtransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;
};


