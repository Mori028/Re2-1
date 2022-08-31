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
	/// �X�V
	/// </summary>
	void Update();

	const ViewProjection& getViewProjection() { return viewProjection_; }

	//���[���h�s����擾
	WorldTransform* GetWolrdMatrix() { return &worldtransform_; }

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;
};


