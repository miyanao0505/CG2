#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <unordered_map>

#pragma comment(lib, "xaudio2.lib")

// オーディオマネージャ
class AudioManager
{
private:	// オーディオ関係の構造体
	// チャンクヘッダ
	struct ChunkHeader {
		char id[4];		// チャンク毎のID
		int32_t size;	// チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk;	// "RIFF"
		char type[4];		// "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk;	// "fmt"
		WAVEFORMATEX fmt;	// 波形フォーマット
	};

	// 音声データ
	struct SoundData {
		WAVEFORMATEX wfex;			// 波形フォーマット
		BYTE* pBuffer;				// バッファの先頭アドレス
		unsigned int bufferSize;	// バッファのサイズ
	};

public:	// メンバ関数
	// シングルトンインスタンスの取得
	static AudioManager* GetInstance();

	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	/// 音声データの読み込み
	/// <summary>
	/// 音声データ(Wave)の読み込み
	/// </summary>
	/// <param name="filename">ファイル名 パスは "resources/" に続く</param>
	void LoadAudioWave(const std::string& filename);


	/// 音声再生
	/// <summary>
	/// 音声再生(Wave)
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <param name="volume">音量</param>
	/// <param name="loop">ループの有無</param>
	void PlayWave(const std::string& filename, const float& volume = 1.0f, const bool& loop = false);


	/// 音声停止
	/// <summary>
	/// 音声停止(Wave)
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void StopWave(const std::string& filename);


	/// 音声データの削除
	void UnLoadAudio(const std::string& filename);

public:	// getter


public:	// setter


public:	// シングルトンインスタンス
	static AudioManager* instance;

	AudioManager() = default;
	~AudioManager() = default;
	AudioManager(AudioManager&) = default;
	AudioManager& operator=(AudioManager&) = delete;

private:	// メンバ変数
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_ = nullptr;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	// 音声データ
	std::unordered_map<std::string, SoundData> soundDatas_;
	std::unordered_map<std::string, IXAudio2SourceVoice*> playSoundDatas_;
};

