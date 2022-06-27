#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle){

	//NULLポインタチェック
	assert(model);

	//シングルトンインスタンスを取得する
	/*input_ = Input::GetInstance();
	debugText_ = DebuguText::GetInstance();*/

	worldtransform_;
}

void Player::Draw()
{
}

void Player::Update()
{
}

