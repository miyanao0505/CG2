#include "AudioManager.h"
#include <cassert>

AudioManager* AudioManager::instance = nullptr;

// シングルトンインスタンスの取得
AudioManager* AudioManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new AudioManager;
	}
	return instance;
}

// 初期化
void AudioManager::Initialize()
{
	HRESULT hr;

	// xAudioエンジンのインスタンスを生成
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));

	// マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));
}

// 終了
void AudioManager::Finalize()
{
	// xAudio2の解放
	xAudio2_.Reset();
	// 音声データの解放
	soundDatas_.clear();
	playSoundDatas_.clear();

	delete instance;
	instance = nullptr;
}

// 音声データ(Wave)の読み込み
void AudioManager::LoadAudioWave(const std::string& filename)
{
	// 読み込み済みなら早期リターン
	if (soundDatas_.contains(filename)) {
		return;
	}
	
	std::string filePath = "resources/" + filename;

	/// 1. ファイルオープン
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filePath, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	/// 2. .wavデータ読み込み
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	
	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		// 読み込み一をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	/// 3. ファイルクローズ
	file.close();

	/// 4. 読み込んだ音声データの参照を取得する
	SoundData& soundData = soundDatas_[filename];
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
}

// 音声再生(Wave)
void AudioManager::PlayWave(const std::string& filename, const float& volume, const bool& loop)
{
	HRESULT hr;
	SoundData& soundData = soundDatas_.at(filename);

	// 波形フォーマットを元にSoundVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	hr = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(hr));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// ループの設定
	buf.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	// 波形データの再生
	hr = pSourceVoice->SubmitSourceBuffer(&buf);
	hr = pSourceVoice->Start();
	hr = pSourceVoice->SetVolume(volume);

	// ループするなら再生中リストにデータを格納
	if (loop)
	{
		if (playSoundDatas_.contains(filename)) {
			return;
		}
		playSoundDatas_[filename] = pSourceVoice;
	}
}

// 音声停止(Wave)
void AudioManager::StopWave(const std::string& filename)
{
	IXAudio2SourceVoice* pSourceVoice = playSoundDatas_[filename];

	// 波形データの停止
	pSourceVoice->Stop();

	// 再生中リストから削除
	playSoundDatas_.erase(filename);
}

// 音声データの解放
void AudioManager::UnLoadAudio(const std::string& filename)
{
	// 見つからなかったら早期リターン
	if (!soundDatas_.contains(filename)) {
		return;
	}

	SoundData& soundData = soundDatas_.at(filename);

	// バッファのメモリを解放
	delete[] soundData.pBuffer;

	soundData.pBuffer = 0;
	soundData.bufferSize = 0;
	soundData.wfex = {};
}
