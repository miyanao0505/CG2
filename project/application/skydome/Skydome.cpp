#include "Skydome.h"
#include "ModelManager.h"
#include "imgui.h"

Skydome::~Skydome()
{
	delete skydome_;
}

// 初期化
void Skydome::Initialize(const std::string& filename)
{
	// 受け取った引数を記録
	/*MyBase::ModelFilePath filePath = { "resources/" + filename, filename + ".obj" };
	path_.directoryPath.resize(filePath.directoryPath.size());
	path_.directoryPath = filePath.directoryPath;
	path_.filename.insert(filePath.filename.length(), filePath.filename);*/
	path_.directoryPath = "resources/" + filename;
	path_.filename = filename + ".obj";

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel(path_.directoryPath, path_.filename);

	// オブジェクトの初期化
	skydome_ = new Object3d;
	skydome_->Initislize();
	skydome_->SetTranslate({ 0.0f, 0.0f, 0.0f });
	skydome_->SetScale({ 100.0f, 100.0f, 100.0f });
	skydome_->SetModel(path_.filename);
}

// 更新
void Skydome::Update()
{
#ifdef _DEBUG

	if (ImGui::CollapsingHeader("Skydome"))
	{
		MyBase::Transform transform{};
		transform.rotate = skydome_->GetRotate();
		ImGui::DragFloat3("rotate", &transform.rotate.x, 0.1f);
		skydome_->SetRotate(transform.rotate);
		transform.scale = skydome_->GetScale();
		ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
		skydome_->SetScale(transform.scale);
		transform.translate = skydome_->GetTranslate();
		ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
		skydome_->SetTranslate(transform.translate);
	}
	
#endif // _DEBUG

	skydome_->Update();
}

// 描画
void Skydome::Draw()
{
	skydome_->Draw();
}
