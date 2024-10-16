#include <string>
#include <format>
#include <wrl.h>
#include <cassert>
#include <dxgidebug.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "input/Input.h"
#include "base/WindowsAPI.h"
#include "base/DirectXBase.h"
#include "Script/MyTools.h"
#include "Script/Matrix.h"
#include "Script/MyBase.h"
#include "externals/DirectXTex/d3dx12.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#pragma comment(lib, "dxcompiler.lib")

// ShaderをCompileをする関数
//IDxcBlob* CompileShader(
//	// CompilerするShaderファイルへのパス
//	const std::wstring& filePath,
//	// Compilerに使用するProfile
//	const wchar_t* profile,
//	// 初期化で生成したものを3つ
//	IDxcUtils* dxcUtils,
//	IDxcCompiler3* dxcCompiler,
//	IDxcIncludeHandler* includeHandler)
//{
//	// ここの中身をこの後書いていく
//	// 1. hlslファイルを読む
//	// これからシェーダーをコンパイルする旨をログに出す
//	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
//	// hlslファイルを読む
//	IDxcBlobEncoding* shaderSource = nullptr;
//	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
//	// 読めなかったら止める
//	assert(SUCCEEDED(hr));
//	// 読み込んだファイルの内容を設定する
//	DxcBuffer shaderSourceBuffer;
//	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
//	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
//	shaderSourceBuffer.Encoding = DXC_CP_UTF8;	// UTF8の文字コードであることを通知
//
//	// 2. Compileする
//	LPCWSTR arguments[] = {
//		filePath.c_str(),			// コンパイル対象のhlslファイル名
//		L"-E", L"main",				// エントリーポイントの指定。基本的にmain以外にはしない
//		L"-T", profile,				// ShaderProfileの設定
//		L"-Zi", L"-Qembed_debug",	// デバッグ用の情報を埋め込む
//		L"-Od",						// 最適化を外しておく
//		L"-Zpr",					// メモリレイアウトは行優先
//	};
//	// 実際にShaderをコンパイルする
//	IDxcResult* shaderResult = nullptr;
//	hr = dxcCompiler->Compile(
//		&shaderSourceBuffer,		// 読み込んだファイル
//		arguments,					// コンパイルオプション
//		_countof(arguments),		// コンパイルオプションの数
//		includeHandler,				// includeが含まれた諸々
//		IID_PPV_ARGS(&shaderResult)	// コンパイル結果
//	);
//	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
//	assert(SUCCEEDED(hr));
//	
//	// 3. 警告・エラーが出ていないか確認する
//	// 警告・エラーが出てたらログに出して止める
//	IDxcBlobUtf8* shaderError = nullptr;
//	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
//	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
//		Log(shaderError->GetStringPointer());
//		// 警告・エラーダメゼッタイ
//		assert(false);
//	}
//
//	// 4. Compile結果を受け取って返す
//	// コンパイル結果から実行用のバイナリ部分を取得
//	IDxcBlob* shaderBlob = nullptr;
//	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
//	assert(SUCCEEDED(hr));
//	// 成功したログを出す
//	Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}", filePath, profile)));
//	// もう使わないリソースを解放
//	shaderSource->Release();
//	shaderResult->Release();
//	// 実行用のバイナリを返却
//	return shaderBlob;
//}

// Resource作成の関数化
Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, size_t sizeInBytes)
{
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;				// UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC bufferResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	bufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferResourceDesc.Width = sizeInBytes;							// リソースのサイズ。今回はVector4を3頂点分
	// バッファの場合はこれらは1にする決まり
	bufferResourceDesc.Height = 1;
	bufferResourceDesc.DepthOrArraySize = 1;
	bufferResourceDesc.MipLevels = 1;
	bufferResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	bufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	Microsoft::WRL::ComPtr<ID3D12Resource> bufferResource = nullptr;

	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &bufferResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&bufferResource));

	assert(SUCCEEDED(hr));

	return bufferResource;
}

// Textureデータの読み込み関数
//DirectX::ScratchImage LoadTexture(const std::string& filePath)
//{
//	// テクスチャファイルを読んでプログラムで扱えるようにする
//	DirectX::ScratchImage image{};
//	std::wstring filePathW = ConvertString(filePath);
//	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
//	assert(SUCCEEDED(hr));
//
//	//ミニマップの作成
//	DirectX::ScratchImage mipImages{};
//	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
//	assert(SUCCEEDED(hr));
//
//	// ミニマップ付きのデータを返す
//	return mipImages;
//}

// TextureResource作成の関数
//Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata)
//{
//	// 1. metadataを基にResourceの設定
//	D3D12_RESOURCE_DESC resourceDesc{};
//	resourceDesc.Width = UINT(metadata.width);								// Textureの幅
//	resourceDesc.Height = UINT(metadata.height);							// Textureの高さ
//	resourceDesc.MipLevels = UINT16(metadata.mipLevels);					// mipmapの数
//	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);				// 奥行き or 配列Textureの配列数
//	resourceDesc.Format = metadata.format;									// TextureのFormat
//	resourceDesc.SampleDesc.Count = 1;										// サンプリングカウント。1固定
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);	// Textureの次元数。普段使っているのは2次元
//	// 2. 利用するHeapの設定。非常に特殊な運用。
//	D3D12_HEAP_PROPERTIES heapProperties{};
//	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;							// 細かい設定を行う
//
//	// 3. Resourceを生成する
//	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
//	HRESULT hr = device->CreateCommittedResource(
//		&heapProperties,					// Heapの設定
//		D3D12_HEAP_FLAG_NONE,				// Heapの特殊な設定。特になし
//		&resourceDesc,						// Resourceの設定
//		D3D12_RESOURCE_STATE_COPY_DEST,		// 初回のResourceState。Textureは基本読むだけ
//		nullptr,							// Clear最適値。使わないのでnullptr
//		IID_PPV_ARGS(&resource)				// 作成するResourceポインタへのポインタ
//	);
//	assert(SUCCEEDED(hr));
//	return resource;
//}

// データを転送する関数
//[[nodiscard]]
//Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages, Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
//{
//	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
//	DirectX::PrepareUpload(device.Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
//	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
//	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(device, intermediateSize);
//	UpdateSubresources(commandList.Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
//	// Textureへの転送後は利用できるよう、D3D12RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
//	D3D12_RESOURCE_BARRIER barrier{};
//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//	barrier.Transition.pResource = texture.Get();
//	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
//	commandList->ResourceBarrier(1, &barrier);
//	return intermediateResource;
//}

MyBase::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	/// 1. 中で必要となる変数の宣言
	MyBase::MaterialData materialData;	// 構築するMateriallData
	std::string line;					// ファイルが読んだ1行を格納するもの

	/// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);		// ファイルを開く
	assert(file.is_open());									// 取り敢えず開けなかったら止める

	/// 3. 実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/../" + textureFilename;
		}
	}

	/// 4. MaterialDataを返す
	return materialData;
}


MyBase::ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	// 1. 中で必要となる変数の宣言
	MyBase::ModelData modelData;				// 構築するModelData
	std::vector<MyBase::Vector4> positions;		// 位置
	std::vector<MyBase::Vector3> normals;		// 法線
	std::vector<MyBase::Vector2> texcoords;		// テクスチャ座標
	std::string line;							// ファイルから呼んだ1行を格納するもの

	// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);		// ファイルを開く
	assert(file.is_open());									// とりあえず開けなかったら止める

	// 3. 実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;			// 先頭の識別子を読む

		// identifierに応じた処理
		if (identifier == "v")
		{
			MyBase::Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			MyBase::Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			MyBase::Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			//normal.z *= -1.f;
			//normal.y *= -1.f;
			normals.push_back(normal);
		}
		else if (identifier == "mtllib")
		{
			// materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
		else if (identifier == "f") {
			MyBase::VertexData triangle[3];
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndeces[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');		// 区切りでインデックスを読んでいく
					elementIndeces[element] = std::stoi(index);
				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				MyBase::Vector4 position = positions[elementIndeces[0] - 1];
				MyBase::Vector2 texcoord = texcoords[elementIndeces[1] - 1];
				MyBase::Vector3 normal = normals[elementIndeces[2] - 1];
				/*MyBase::VertexData vertex = { position, texcoord, normal };
				modelData.vertices.push_back(vertex);*/
				triangle[faceVertex] = { position, texcoord, normal };
			}
			// 頂点を逆順で登録することで、周り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
	}

	// 4. ModelDataを返す
	return modelData;
}

struct D3DResourceLeakChecker {
	~D3DResourceLeakChecker()
	{
		// リソースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	D3DResourceLeakChecker leakCheck;
	
	

	// ポインタ
	Input* input = nullptr;
	WindowsAPI* winApi = nullptr;
	DirectXBase* dxBase = nullptr;

#pragma region WindowsAPI
	// WindowsAPIの初期化
	winApi = new WindowsAPI();
	winApi->Initialize();
#pragma endregion WindowsAPI

#pragma region DirectXBase
	// DirectXBaseの初期化
	dxBase = new DirectXBase();
	dxBase->Initialize(winApi);
#pragma endregion DirectXBase

#pragma region Input
	// 入力の初期化
	input = new Input();
	input->Initialize(winApi);
#pragma endregion Input

	// ブレンドモード
	//enum BlendMode {
	//	kBlendModeNone,			// ブレンドなし
	//	kBlendModeNormal,		// 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	//	kBlendModeAdd,			// 加算。 Src * SrcA + Dest * 1
	//	kBlendModeSubtract,		// 減算。 Dest * 1 - Src * SrcA
	//	kBlendModeMultiply,		// 乗算。 Src * 0 + Dest * Src
	//	kBlendModeScreen,		// スクリーン。 Src * (1 - Dest) + Dest * 1
	//	kCountOfBlendMode,		// 利用してはいけない
	//};

	
	// FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

	// DescriptorRange作成
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;														// 0から始まる
	descriptorRange[0].NumDescriptors = 1;															// 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;									// SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// Offsetを自動計算

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;									// レジスタ番号0とバインド
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				// VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;									// レジスタ番号0とバインド
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;				// Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	// Tableで利用する数
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[3].Descriptor.ShaderRegister = 1;									// レジスタ番号1を使う
	descriptionRootSignature.pParameters = rootParameters;					// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);		// 配列の長さ

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;			// バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;		// 0～1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;		// 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;						// ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;								// レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	//ID3DBlob* signatureBlob = nullptr;
	//ID3DBlob* errorBlob = nullptr;
	//hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	//if (FAILED(hr)) {
	//	Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	//	assert(false);
	//}
	//// バイナリを元に生成
	//Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	//hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	//assert(SUCCEEDED(hr));

	// InputLayer
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面(時計回り)を表示しない
	//rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 裏面も表示する
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Shaderをコンパイルする
	//IDxcBlob* vertexShaderBlob = CompileShader(L"resources/Shaders/Object3D.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	//assert(vertexShaderBlob != nullptr);

	//IDxcBlob* pixelShaderBlob = CompileShader(L"resources/Shaders/Object3D.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	//assert(pixelShaderBlob != nullptr);

	// PSOを生成する
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	//graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();												// RootSignature
	//graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;													// InputLayout
	//graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };	// VertexShader
	//graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };	// PixelShader
	//graphicsPipelineStateDesc.BlendState = blendDesc;															// BlendState
	//graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;													// RasterizerState
	//// 書き込むRTVの情報
	//graphicsPipelineStateDesc.NumRenderTargets = 1;
	//graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//// 利用するトポロジ(形状)のタイプ。三角形
	//graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	//graphicsPipelineStateDesc.SampleDesc.Count = 1;
	//graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//// DepthStencilStateの設定
	//D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//// Depthの機能を有効する
	//depthStencilDesc.DepthEnable = true;
	//// 書き込みします
	//depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//// 比較関数はLessEqual。つまり、近ければ描画される
	//depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//// DepthStencilの設定
	//graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	//graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//// 実際に生成
	//Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
	//hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	//assert(SUCCEEDED(hr));

#pragma region Sphere
	//// 球の情報
	//const uint32_t kSubdivision = 16;									// 分割数
	//const uint32_t vertexNum = kSubdivision * kSubdivision * 4;			// 頂点数
	//const uint32_t indexNum = kSubdivision * kSubdivision * 6;			// Index数
	//size_t sizeInByteSphere = sizeof(MyBase::VertexData) * vertexNum;	// 頂点数のサイズ
	//uint32_t sizeInByteSphereIndex = sizeof(uint32_t) * indexNum;		// Index数のサイズ
	//const float kLonEvery = 2.f * float(M_PI) / float(kSubdivision);	// 経度分割1つ分の角度 φd
	//const float kLatEvery = float(M_PI) / float(kSubdivision);			// 緯度分割1つ分の角度 θd
	//
	//// 実際に頂点リソースを作る
	//Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(device, sizeInByteSphere);
	//
	//// 頂点バッファビューを作成する
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//// リソースの先頭のアドレスから使う
	//vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();
	//// 使用するリソースのサイズは頂点
	//vertexBufferView.SizeInBytes = (UINT)sizeInByteSphere;
	//// 1頂点あたりのサイズ
	//vertexBufferView.StrideInBytes = sizeof(MyBase::VertexData);
	//
	//// 頂点リソースにデータを書き込む
	//MyBase::VertexData* vertexData = nullptr;
	//// 書き込むためのアドレス
	//vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//// 緯度の方向に分割
	//for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
	//	float lat = -float(M_PI) / 2.0f + kLatEvery * float(latIndex);		// θ
	//	// 経度の方向に分割しながら線を描く
	//	for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
	//		uint32_t index = (latIndex * kSubdivision + lonIndex) * 4;
	//		float lon = lonIndex * kLonEvery;						// φ
	//		// 頂点にデータを入力する。基準点a
	//		// a
	//		vertexData[index].position.x = std::cos(lat) * std::cos(lon);
	//		vertexData[index].position.y = std::sin(lat);
	//		vertexData[index].position.z = std::cos(lat) * std::sin(lon);
	//		vertexData[index].position.w = 1.0f;
	//		vertexData[index].texcoord = { float(lonIndex) / float(kSubdivision), 1.0f - float(latIndex) / float(kSubdivision) };
	//		vertexData[index].normal.x = vertexData[index].position.x;
	//		vertexData[index].normal.y = vertexData[index].position.y;
	//		vertexData[index].normal.z = vertexData[index].position.z;
	//		// b
	//		vertexData[index + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
	//		vertexData[index + 1].position.y = std::sin(lat + kLatEvery);
	//		vertexData[index + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
	//		vertexData[index + 1].position.w = 1.0f;
	//		vertexData[index + 1].texcoord = { float(lonIndex) / float(kSubdivision), 1.0f - float(latIndex + 1) / float(kSubdivision) };
	//		vertexData[index + 1].normal.x = vertexData[index + 1].position.x;
	//		vertexData[index + 1].normal.y = vertexData[index + 1].position.y;
	//		vertexData[index + 1].normal.z = vertexData[index + 1].position.z;
	//		// c
	//		vertexData[index + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
	//		vertexData[index + 2].position.y = std::sin(lat);
	//		vertexData[index + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
	//		vertexData[index + 2].position.w = 1.0f;
	//		vertexData[index + 2].texcoord = { float(lonIndex + 1) / float(kSubdivision), 1.0f - float(latIndex) / float(kSubdivision) };
	//		vertexData[index + 2].normal.x = vertexData[index + 2].position.x;
	//		vertexData[index + 2].normal.y = vertexData[index + 2].position.y;
	//		vertexData[index + 2].normal.z = vertexData[index + 2].position.z;
	//		// d
	//		vertexData[index + 3].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
	//		vertexData[index + 3].position.y = std::sin(lat + kLatEvery);
	//		vertexData[index + 3].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
	//		vertexData[index + 3].position.w = 1.0f;
	//		vertexData[index + 3].texcoord = { float(lonIndex + 1) / float(kSubdivision), 1.0f - float(latIndex + 1) / float(kSubdivision) };
	//		vertexData[index + 3].normal.x = vertexData[index + 3].position.x;
	//		vertexData[index + 3].normal.y = vertexData[index + 3].position.y;
	//		vertexData[index + 3].normal.z = vertexData[index + 3].position.z;
	//	}
	//}
	//
	//// Index用の頂点リソースを作る
	//Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = CreateBufferResource(device, sizeInByteSphereIndex);
	//
	//// Index頂点バッファビューを作成
	//D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	//// リソースの先頭のアドレスから使う
	//indexBufferView.BufferLocation = indexResource.Get()->GetGPUVirtualAddress();
	//// 使用するリソースのサイズはインデックス6つ分のサイズ
	//indexBufferView.SizeInBytes = sizeInByteSphereIndex;
	//// インデックスはuint32_tとする
	//indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	//
	//// インデックスリソースにデータを書き込む
	//uint32_t* indexData = nullptr;
	//indexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	//// 緯度の方向に分割
	//for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
	//	// 経度の方向に分割しながら線を描く
	//	for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
	//		uint32_t index = (latIndex * kSubdivision + lonIndex) * 6;
	//		uint32_t vertex = (latIndex * kSubdivision + lonIndex) * 4;
	//		// Index用に頂点データを入力する。基準点a
	//		indexData[index] = vertex;			indexData[index + 1] = vertex + 1;	indexData[index + 2] = vertex + 2;
	//		indexData[index + 3] = vertex + 1;	indexData[index + 4] = vertex + 3;	indexData[index + 5] = vertex + 2;
	//	}
	//}

	//// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	//Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = CreateBufferResource(device, sizeof(MyBase::Material));
	//// マテリアルにデータを書き込む
	//MyBase::Material* materialData = nullptr;
	//// 書き込むためのアドレスを取得
	//materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//// 白で読み込む
	//materialData->color = MyBase::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//// Lightingを有効にする
	//materialData->enableLighting = true;
	//// 単位行列で初期化
	//materialData->uvTransform = Matrix::MakeIdentity4x4();
#pragma endregion

//#pragma region Model
//	// モデル読み込み
//	MyBase::ModelData modelData = LoadObjFile("resources/fence", "fence.obj");
//	// 頂点リソースを作る
//	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(device, sizeof(MyBase::VertexData) * modelData.vertices.size());
//	// 頂点バッファビューを作成する
//	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
//	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();						// リソースの先頭のアドレスから使う
//	vertexBufferView.SizeInBytes = UINT(sizeof(MyBase::VertexData) * modelData.vertices.size());	// 使用するリソースのサイズは頂点のサイズ
//	vertexBufferView.StrideInBytes = sizeof(MyBase::VertexData);									// 頂点あたりのサイズ
//
//	// 頂点リソースにデータを書き込む
//	MyBase::VertexData* vertexData = nullptr;
//	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));										// 書き込むためのアドレスを取得
//	std::memcpy(vertexData, modelData.vertices.data(), sizeof(MyBase::VertexData)* modelData.vertices.size());	// 頂点データをリソースにコピー
//
//	// モデル用のTransformationMatrix用のリソースを作る。TransformationMatrix 1つ分のサイズを用意する
//	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource = CreateBufferResource(device, sizeof(MyBase::TransformationMatrix));
//	// データを書き込む
//	MyBase::TransformationMatrix* transformationMatrixData = nullptr;
//	// 書き込むためのアドレスを取得
//	transformationMatrixResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
//	// 単位行列を書き込んでおく
//	transformationMatrixData->WVP = Matrix::MakeIdentity4x4();
//	transformationMatrixData->World = Matrix::MakeIdentity4x4();
//
//	// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
//	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = CreateBufferResource(device, sizeof(MyBase::Material));
//	// マテリアルにデータを書き込む
//	MyBase::Material* materialData = nullptr;
//	// 書き込むためのアドレスを取得
//	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
//	// 白で読み込む
//	materialData->color = MyBase::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	// Lightingを有効にする
//	materialData->enableLighting = true;
//	// 単位行列で初期化
//	materialData->uvTransform = Matrix::MakeIdentity4x4();
//#pragma endregion

//#pragma region Sprite
//	// Sprite用の頂点リソースを作る
//	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSprite = CreateBufferResource(device, sizeof(MyBase::VertexData) * 6);
//
//	// 頂点バッファビューを作成する
//	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
//	// リソースの先頭のアドレスから使う
//	vertexBufferViewSprite.BufferLocation = vertexResourceSprite.Get()->GetGPUVirtualAddress();
//	// 使用するリソースのサイズは頂点6つ分のサイズ
//	vertexBufferViewSprite.SizeInBytes = sizeof(MyBase::VertexData) * 4;
//	// 1頂点あたりのサイズ
//	vertexBufferViewSprite.StrideInBytes = sizeof(MyBase::VertexData);
//
//	// Sprite用の頂点リソースにデータを書き込む
//	MyBase::VertexData* vertexDataSprite = nullptr;
//	// 書き込むためのアドレス
//	vertexResourceSprite.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
//	// 三角形
//	vertexDataSprite[0].position = { 0.0f, 360.0f, 0.0f, 1.0f };	// 左下
//	vertexDataSprite[0].texcoord = { 0.0f, 1.0f };
//	vertexDataSprite[0].normal = { 0.0f, 0.0f, -1.0f };
//	vertexDataSprite[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };		// 左上
//	vertexDataSprite[1].texcoord = { 0.0f, 0.0f };
//	vertexDataSprite[1].normal = { 0.0f, 0.0f, -1.0f };
//	vertexDataSprite[2].position = { 640.f, 360.f, 0.0f, 1.0f };	// 右下
//	vertexDataSprite[2].texcoord = { 1.0f, 1.0f };
//	vertexDataSprite[2].normal = { 0.0f, 0.0f, -1.0f };
//	vertexDataSprite[3].position = { 640.f, 0.0f, 0.0f, 1.0f };		// 右上
//	vertexDataSprite[3].texcoord = { 1.0f, 0.0f };
//	vertexDataSprite[3].normal = { 0.0f, 0.0f, -1.0f };
//
//	// Index用の頂点リソースを作る
//	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite = CreateBufferResource(device, sizeof(uint32_t) * 6);
//
//	// Index頂点バッファビューを作成
//	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
//	// リソースの先頭のアドレスから使う
//	indexBufferViewSprite.BufferLocation = indexResourceSprite.Get()->GetGPUVirtualAddress();
//	// 使用するリソースのサイズはインデックス6つ分のサイズ
//	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
//	// インデックスはuint32_tとする
//	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;
//
//	// インデックスリソースにデータを書き込む
//	uint32_t* indexDataSprite = nullptr;
//	indexResourceSprite.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
//	indexDataSprite[0] = 0;		indexDataSprite[1] = 1;		indexDataSprite[2] = 2;
//	indexDataSprite[3] = 1;		indexDataSprite[4] = 3;		indexDataSprite[5] = 2;
//
//	// Sprite用のTransformationMatrix用のリソースを作る。TransformationMatrix 1つ分のサイズを用意する
//	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResourceSparite = CreateBufferResource(device, sizeof(MyBase::TransformationMatrix));
//	// データを書き込む
//	MyBase::TransformationMatrix* transformationMatrixDataSprite = nullptr;
//	// 書き込むためのアドレスを取得
//	transformationMatrixResourceSparite.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
//	// 単位行列を書き込んでおく
//	transformationMatrixDataSprite->WVP = Matrix::MakeIdentity4x4();
//	transformationMatrixDataSprite->World = Matrix::MakeIdentity4x4();
//
//	// Sprite用のマテリアルリソースを作る
//	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSprite = CreateBufferResource(device, sizeof(MyBase::Material));
//	// データを書き込む
//	MyBase::Material* materialDataSprite = nullptr;
//	// 書き込むためのアドレスを取得
//	materialResourceSprite.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
//	// 白で読み込む
//	materialDataSprite->color = MyBase::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	// SpriteはLightingしないのでfalseを設定する
//	materialDataSprite->enableLighting = false;
//	// 単位行列で初期化
//	materialDataSprite->uvTransform = Matrix::MakeIdentity4x4();
//#pragma endregion

	// カメラ用のリソースを作る。TransformationMatrix 1つ分のサイズを用意する
	//Microsoft::WRL::ComPtr<ID3D12Resource> transformationResourceCamera = CreateBufferResource(device, sizeof(MyBase::TransformationMatrix));
	// データを書き込む
	//MyBase::TransformationMatrix* transformationMatrixDataCamera = nullptr;
	// 書き込むためのアドレスを取得
	//transformationResourceCamera.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataCamera));
	// 単位行列を書き込んでおく
	//transformationMatrixDataCamera->WVP = Matrix::MakeIdentity4x4();
	//transformationMatrixDataCamera->World = Matrix::MakeIdentity4x4();

	// 平行光源用のリソースを作る
	//Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = CreateBufferResource(device, sizeof(MyBase::DirectionalLight));
	// データを書き込む
	//MyBase::DirectionalLight* directionalLightData = nullptr;
	// 書き込むためのアドレス取得
	//directionalLightResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	// デフォルト値はとりあえず以下のようにしておく
	//directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//directionalLightData->direction = { 0.0f, -1.0f, 0.0f };
	//directionalLightData->intensity = 1.0f;

	// Transform変数を作る
	//MyBase::Transform transform{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
	//MyBase::Transform transformSprite{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	MyBase::Transform cameraTransform{ { 1.0f, 1.0f, 1.0f }, { 0.3f, 0.0f, 0.0f }, { 0.0f, 4.0f, -10.0f } };
	/*MyBase::Transform uvTransformSprite{
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};*/

	// テクスチャ切り替え用
	//bool useMonsterBall = true;

	// ブレンドモード切替用
	//BlendMode blendMode = kBlendModeNone;
	//int blendIndex = 0;

	// Textureを読んで転送する
	//DirectX::ScratchImage mipImages = LoadTexture("resources/uvChecker.png");
	//const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = CreateTextureResource(device, metadata);
	//Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(textureResource, mipImages, device, commandList);
	//// 2枚目のTextureを読んで転送する
	////DirectX::ScratchImage mipImages2 = LoadTexture("resources/monsterBall.png");
	//DirectX::ScratchImage mipImages2 = LoadTexture(modelData.material.textureFilePath);
	//const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	//Microsoft::WRL::ComPtr<ID3D12Resource> textureResource2 = CreateTextureResource(device, metadata2);
	//Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource2 = UploadTextureData(textureResource2, mipImages2, device, commandList);

	// DepthStencilTextureをウィンドウのサイズで作成
	//Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = CreateDepthStencilTextureResource(device, WindowsAPI::kClientWidth, WindowsAPI::kClientHeight);

	// metaDataを基にSRVの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//srvDesc.Format = metadata.format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;							// 2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// metaData2を基にSRVの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	//srvDesc2.Format = metadata2.format;
	//srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;							// 2Dテクスチャ
	//srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	// SRVを作成するDescriptorHeapの場所を決める
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 1);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 1);
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	//// SRVの生成
	//device->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
	//device->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);


	// ウィンドウの×ボタンが押されるまでループ
	while (true) {	// ゲームループ
		// Windowsのメッセージ処理
		if (winApi->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		}
		
		// 入力の更新
		input->Update();

		// ImGuiにここからフレームが始まる旨を告げる
#ifdef _DEBUG
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
#endif // _DEBUG

		// Transformの更新(Y軸回転)
		//transform.rotate.y += 1.f / 360.f * float(M_PI);

#ifdef _DEBUG
		// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
		//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.6f, 0.2f, 1.0f));	// ウィンドウを選択中の時のタイトルバーの色
		//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));			// ウィンドウを未選択の時のタイトルバーの色
		//ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);							// ウィンドウの座標(プログラム起動時のみ読み込み)
		//ImGui::SetNextWindowSize(ImVec2(500, 80), ImGuiCond_Once);							// ウィンドウのサイズ(プログラム起動時のみ読み込み)

		//ImGui::Begin("Camera");
		//ImGui::DragFloat3("translate", &cameraTransform.translate.x, 0.05f);
		//ImGui::DragFloat3("rotate", &cameraTransform.rotate.x, 0.05f);
		//ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(850, 20), ImGuiCond_Once);							// ウィンドウの座標(プログラム起動時のみ読み込み)
		ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Once);							// ウィンドウのサイズ(プログラム起動時のみ読み込み)

		ImGui::Begin("Settings");

		// カメラ
		/*ImGui::DragFloat3("CameraTranslate", &cameraTransform.translate.x, 0.05f);
		ImGui::SliderAngle("CameraRotateX", &cameraTransform.rotate.x);
		ImGui::SliderAngle("CameraRotateY", &cameraTransform.rotate.y);
		ImGui::SliderAngle("CameraRotateZ", &cameraTransform.rotate.z);*/
		
		// 球 / Model
		/*ImGui::DragFloat3("PlaneTranslate", &transform.translate.x, 0.05f);
		ImGui::SliderAngle("PlaneRotateX", &transform.rotate.x);
		ImGui::SliderAngle("PlaneRotateY", &transform.rotate.y);
		ImGui::SliderAngle("PlaneRotateZ", &transform.rotate.z);
		ImGui::ColorEdit4("color", &materialData[0].color.x);
		ImGui::Checkbox("enableLighting", (bool*)&materialData[0].enableLighting);*/

		// スプライト
		/*ImGui::ColorEdit4("colorSprite", &materialDataSprite[0].color.x);
		ImGui::SliderFloat3("translateSprite", &transformSprite.translate.x, 0.0f, 1280.0f);*/

		// テクスチャ
		//ImGui::Checkbox("useMonsterBall", &useMonsterBall);

		/*const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
		ImGui::Combo("Blend", &blendIndex, blendModeIndex, IM_ARRAYSIZE(blendModeIndex));
		blendMode = (BlendMode)blendIndex;*/

		// ブレンドモード
		/*switch (blendMode)
		{
		case kBlendModeNone:
			blendDesc.RenderTarget[0].BlendEnable = false;
			break;
		case kBlendModeNormal:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			break;
		case kBlendModeAdd:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		case kBlendModeSubtract:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		case kBlendModeMultiply:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
			break;
		case kBlendModeScreen:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		default:
			break;
		}*/

		// 平行光源
		/*ImGui::ColorEdit3("LightColor", &directionalLightData[0].color.x);
		ImGui::SliderFloat3("LightDirection", &directionalLightData[0].direction.x, -1.0f, 1.0f);
		directionalLightData[0].direction = MyTools::Normalize(directionalLightData[0].direction);
		ImGui::DragFloat("Intensity", &directionalLightData[0].intensity, 0.05f);*/

		// UV
		/*ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);*/

		ImGui::End();
#endif // _DEBUG

		// WorldMatrixの作成
		//MyBase::Matrix4x4 worldMatrix = Matrix::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		// カメラのWorldMatrixの作成
		MyBase::Matrix4x4 cameraMatrix = Matrix::MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
		// ビュー行列の作成
		MyBase::Matrix4x4 viewMatrix = Matrix::Inverse(cameraMatrix);
		// 透視投影行列の作成
		MyBase::Matrix4x4 projectionMatrix = Matrix::MakePerspectiveFovMatrix(0.45f, float(WindowsAPI::kClientWidth) / float(WindowsAPI::kClientHeight), 0.1f, 500.0f);
		// WVP行列の作成
		//MyBase::Matrix4x4 worldViewProjectionMatrix = Matrix::Multiply(worldMatrix, Matrix::Multiply(viewMatrix, projectionMatrix));
		// CBufferの中身を更新
		//transformationMatrixData[0].WVP = worldViewProjectionMatrix;
		//transformationMatrixData[0].World = worldMatrix;

		// Sprite用のWorldViewProgectionMatrixを作る
		/*Matrix::Matrix4x4 worldMatrixSprite = Matrix::MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
		Matrix::Matrix4x4 viewMatrixSprite = Matrix::MakeIdentity4x4();
		Matrix::Matrix4x4 projectionMatrixSprite = Matrix::MakeOrthographicMatrix(0.0f, 0.0f, float(WindowsAPI::kClientWidth), float(WindowsAPI::kClientHeight), 0.0f, 100.0f);
		Matrix::Matrix4x4 worldViewProjectionMatrixSprite = Matrix::Multiply(worldMatrixSprite, Matrix::Multiply(viewMatrixSprite, projectionMatrixSprite));*/
		//transformationMatrixDataSprite[0].WVP = worldViewProjectionMatrixSprite;
		//transformationMatrixDataSprite[0].World = worldMatrixSprite;

		// UVTransform用
		/*Matrix::Matrix4x4 uvTransformMatrix = Matrix::MakeScaleMatrix(uvTransformSprite.scale);
		uvTransformMatrix = Matrix::Multiply(uvTransformMatrix, Matrix::MakeRotateZMatrix4x4(uvTransformSprite.rotate.z));
		uvTransformMatrix = Matrix::Multiply(uvTransformMatrix, Matrix::MakeTranslateMatrix(uvTransformSprite.translate));*/
		//materialDataSprite->uvTransform = uvTransformMatrix;

		// これから書き込むバックバッファのインデックスを取得
		//UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();

		// TransitionBarrierの設定
		D3D12_RESOURCE_BARRIER barrier{};
		// 今回のバリアはTransition
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		// Novneにしておく
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		// バリアを張る対象のリソース。現在のバックバッファに対して行う
		//barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
		// 遷移前(現在)のResourceState
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		// 遷移後のResourceState
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		// TransitionBarrierを張る
		//commandList->ResourceBarrier(1, &barrier);

		// ImGuiの内部コマンドを生成する
#ifdef _DEBUG
		ImGui::Render();
#endif // _DEBUG

		// 描画先のRTVとDSVを設定する
		//D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		//commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, &dsvHandle);
		//// 指定した深度で画面全体をクリアする
		//commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		//// 指定した色で画面全体をクリアする
		//float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };	// 青っぽい色。RGBAの順
		//commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);

		//// 描画用のDescriptorHeapの設定
		//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap };
		//commandList->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

		//// コマンドを積む
		//commandList->RSSetViewports(1, &viewport);			// Viewportを設定
		//commandList->RSSetScissorRects(1, &scissorRect);	// Scirssorを設定
		//// RootSignatureを設定。PSOに設定しているけど別途設定が必要
		//commandList->SetGraphicsRootSignature(rootSignature.Get());
		//commandList->SetPipelineState(graphicsPipelineState.Get());		// PSOを設定
		//commandList->IASetVertexBuffers(0, 1, &vertexBufferView);	// VBVを設定
		////commandList->IASetIndexBuffer(&indexBufferView);			// IBVを設定
		//// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えて置けば良い
		//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//// マテリアルCBufferの場所を設定
		//commandList->SetGraphicsRootConstantBufferView(0, materialResource.Get()->GetGPUVirtualAddress());
		//// WVP用のCBufferの場所を設定
		//commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource.Get()->GetGPUVirtualAddress());
		//// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
		////commandList->SetGraphicsRootDescriptorTable(2, useMonsterBall ? textureSrvHandleGPU2 : textureSrvHandleGPU);
		//commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU2);
		//// 平行光源用のCBufferの場所を設定
		//commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource.Get()->GetGPUVirtualAddress());
		//// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		////commandList->DrawIndexedInstanced(indexNum, 1, 0, 0, 0);
		//commandList->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

		//// Spriteの描画。変更が必要なものだけ変更する
		//commandList->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);			// VBVを設定
		//commandList->IASetIndexBuffer(&indexBufferViewSprite);					// IBVを設定
		//// マテリアルCBufferの場所を設定
		//commandList->SetGraphicsRootConstantBufferView(0, materialResourceSprite.Get()->GetGPUVirtualAddress());
		//// TransformationMatrixCBufferの場所を設定
		//commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSparite.Get()->GetGPUVirtualAddress());
		//// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
		//commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
		//// 平行光源用のCBufferの場所を設定
		//commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource.Get()->GetGPUVirtualAddress());
		// 描画！(DrawCall/ドローコール)
		//commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

		// 実際のcommandListのImGuiの描画コマンドを積む
#ifdef _DEBUG
		//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
#endif // _DEBUG

		// 画面に描く処理はすべて終わり、画面に映すので、状態を遷移
		// 今回はRenderTargetからPresentにする
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		// TransitionBarrierを張る
		//commandList->ResourceBarrier(1, &barrier);

		// コマンドリストの内容を確定させる。すべてコマンドを積んでからCloseすること
		//hr = commandList->Close();
		//assert(SUCCEEDED(hr));

		// GPUにコマンドリストの実行を行わせる
		//Microsoft::WRL::ComPtr<ID3D12CommandList> commandLists[] = { commandList.Get()};
		//commandQueue->ExecuteCommandLists(1, commandLists->GetAddressOf());
		// GPUとOSに画面の交換を行うよう通知する
		//swapChain->Present(1, 0);

		// Fenceの値を更新
		//fenceValue++;
		// GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
		//commandQueue->Signal(fence.Get(), fenceValue);

		// Fenceの値が指定したSignal値にたどり着いているか確認する
		// GetCompletedValueの初期値はFence作成時に渡した初期値
		//if (fence->GetCompletedValue() < fenceValue)
		//{
		//	// 指定したSignalにたどりついていないので、たどり着くまで待つようにイベントを設定する
		//	fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//	// イベント待つ
		//	WaitForSingleObject(fenceEvent, INFINITE);
		//}

		// 次のフレーム用のコマンドリストを準備
		//hr = commandAllocator->Reset();
		//assert(SUCCEEDED(hr));
		//hr = commandList->Reset(commandAllocator.Get(), nullptr);
		//assert(SUCCEEDED(hr));
	}

	// COMの終了処理
	CoUninitialize();

	// ImGuiの終了処理
#ifdef _DEBUG
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // _DEBUG

	// 解放処理
	//signatureBlob->Release();
	//if (errorBlob) {
	//	errorBlob->Release();
	//}
	//pixelShaderBlob->Release();
	//vertexShaderBlob->Release();

	// 入力解放
	delete input;
	// DirectX解放
	delete dxBase;

	CloseHandle(fenceEvent);
	// WindowsAPIの終了処理
	winApi->Finalize();

	// WindowsAPIの解放
	delete winApi;
	winApi = nullptr;

	return 0;
}