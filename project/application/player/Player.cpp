#include "Player.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "MyTools.h"

Player::~Player()
{
	delete model_;
}

void Player::Initialize(const std::string& filename)
{
	input_ = Input::GetInstance();

	// 受け取った引数を記録
	path_.directoryPath = "resources/" + filename;
	path_.filename = filename + ".obj";

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel(path_.directoryPath, path_.filename);

	// モデルの初期化
	model_ = new Object3d();
	model_->Initislize();
	model_->SetTranslate({ 0.0f, 0.0f, 0.0f });
	model_->SetScale({ 0.5f, 0.5f, 0.5f });
	model_->SetModel(path_.filename);

	SetRadius(0.5f);

	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 0.0f, 4.0f, -10.0f });
}

void Player::Update()
{
	MyBase::Transform transform = { 0.0f };
	float velocity = 0.05f;

	// 入力
	if (input_->PushKey(DIK_W)) {
		transform.translate.z += velocity;
	}
	if (input_->PushKey(DIK_S)) {
		transform.translate.z -= velocity;
	}
	if (input_->PushKey(DIK_A)) {
		transform.translate.x -= velocity;
	}
	if (input_->PushKey(DIK_D)) {
		transform.translate.x += velocity;
	}

	model_->SetTranslate(MyTools::Add(model_->GetTranslate(), transform.translate));

	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetTranslate(MyTools::Add(CameraManager::GetInstance()->GetCamera()->GetTranslate(), transform.translate));

	model_->Update();
}

void Player::Draw()
{
	model_->Draw();
}

void Player::OnCollision()
{
	
}


MyBase::Vector3 Player::GetWorldPosition()
{
	return model_->GetTranslate();
}

