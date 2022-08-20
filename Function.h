#include "worldTransform.h"
#include "Input.h"
#include "Enemy.h"
#include <math.h>

// 単位行列作成
Matrix4 CreateMatIdentity();

// スケーリング行列の作成
Matrix4 CreateMatScale(Vector3 scale);

// Z軸の回転行列作成
Matrix4 CreateMatRotationZ(Vector3 rotation);

// X軸の回転行列作成a
Matrix4 CreateMatRotationX(Vector3 rotation);

// Y軸の回転行列
Matrix4 CreateMatRotationY(Vector3 rotation);

// 平行移動
Matrix4 CreateMatTranslation(Vector3 translation);

// ワールドトランスフォームの更新
void CreateMatrixUpdate(WorldTransform& worldTransform_);

// キャラクターの旋回処理
void Rotate(WorldTransform& worldTransform_, Input* input_);

//キャラの向きに応じた方向に球をだす
Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

Vector3 Vector3sub(Vector3& v1, Vector3& v2);

Vector3 Normalize(Vector3& vec);

float length(Vector3& vec);