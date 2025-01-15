#include "Player.h"
#include "ModelManager.h"
#include "MyTools.h"

/// 初期化
void Player::Initialize(const std::string& filename)
{
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 受け取ったファイル名をパスに変更して記録
	path_.directoryPath = "resources/" + filename;
	path_.filename = filename + ".obj";

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel(path_.directoryPath, path_.filename);

	// 3Dモデルの生成
	player_ = new Object3d();
	player_->Initislize();
	player_->SetTranslate({ 0.0f, 0.0f, 0.0f });
	player_->SetModel(path_.filename);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);

	// ステータスの設定
	hp_ = 10;
}

/// 更新
void Player::Update()
{
	// 旋回処理

	// キャラクターの移動ベクトル
	MyBase::Vector3 move = { 0.0f, 0.0f, 0.0f };

	// 現在のキャラクターの位置
	MyBase::Transform transform = { player_->GetScale(), player_->GetRotate(), player_->GetTranslate() };

	// キーボード操作
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed_;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed_;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed_;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed_;
	}

	// 座標移動(ベクトルの加算)
	transform.translate = MyTools::Add(transform.translate, move);

	// 自機のワールド座標から3Dレティクルのワールド座標を計算


	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算


	// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置


	// 攻撃処理
	Attack();

	// 弾更新


#ifdef _DEBUG

#endif // _DEBUG

	// 移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	transform.translate.x = MyTools::Clamp(transform.translate.x, -kMoveLimitX, kMoveLimitX);
	transform.translate.y = MyTools::Clamp(transform.translate.y, -kMoveLimitY, kMoveLimitY);

	// 変更したSRTをセット
	player_->SetScale(transform.scale);
	player_->SetRotate(transform.rotate);
	player_->SetTranslate(transform.translate);
}

/// 攻撃
void Player::Attack()
{

}

/// 描画
void Player::Draw()
{

}
