#pragma once
#include <string>
#include <cstdint>
#include <vector>

class MyBase
{
public:
	// 構造体
	
	/// <summary>
	/// 2次元ベクトル
	/// </summary>
	struct Vector2 {
		float x;
		float y;
	};

	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	struct Vector3 {
		float x;
		float y;
		float z;
	};
	
	/// <summary>
	/// 4次元ベクトル
	/// </summary>
	struct Vector4 {
		float x;
		float y;
		float z;
		float w;
	};

	/// <summary>
	/// 2x2の行列
	/// </summary>
	struct Matrix2x2 {
		float m[2][2];
	};
	
	/// <summary>
	/// 3x3の行列
	/// </summary>
	struct Matrix3x3 {
		float m[3][3];
	};

	/// <summary>
	/// 4x4の行列
	/// </summary>
	struct Matrix4x4 {
		float m[4][4];
	};

	/// <summary>
	/// 円
	/// </summary>
	struct Ball {
		Vector2 pos;			// 中心点
		Vector2 velocity;		// 速度
		Vector2 acceleration;	// 加速度
		float mass;				// 質量
		float radius;			// 半径
		unsigned int color;		// 色
	};

	/// <summary>
	/// 矩形
	/// </summary>
	struct Box {
		Vector2 pos;			// 位置
		Vector2 size;			// サイズ
		Vector2 velocity;		// 速度
		Vector2 accleration;	// 加速度
		float mass;				// 質量
		unsigned int color;		// 色
	};

	/// <summary>
	/// 矩形の範囲
	/// </summary>
	struct Rect {
		float Top;				// 上辺 Y座標
		float Bottom;			// 下辺 Y座標
		float Left;				// 左辺 X座標
		float Right;			// 右辺 X座標
	};

	/// <summary>
	/// 線
	/// </summary>
	struct Line2 {
		Vector2 origin;			//!< 始点
		Vector2 diff;			//!< 終点への差分ベクトル
	};

	/// <summary>
	/// 球
	/// </summary>
	struct Sphere {
		Vector3 center;		//!< 中心
		float radius;		//!< 半径
	};

	/// <summary>
	/// 直線
	/// </summary>
	struct Line {
		Vector3 origin;		//!< 始点
		Vector3 diff;		//!< 終点への差分ベクトル
	};

	/// <summary>
	/// 半直線
	/// </summary>
	struct Ray {
		Vector3 origin;		//!< 始点
		Vector3 diff;		//!< 終点への差分ベクトル
	};

	/// <summary>
	/// 線分
	/// </summary>
	struct Segment {
		Vector3 origin;		//!< 始点
		Vector3 diff;		//!< 終点への差分ベクトル
	};

	/// <summary>
	/// 平面
	/// </summary>
	struct Plane {
		Vector3 normal;		//!< 法線
		float distance;		//!< 距離
	};

	/// <summary>
	/// 三角形
	/// </summary>
	struct Triangle {
		Vector3 vertices[3];	//!< 頂点
	};

	/// <summary>
	/// AABB
	/// </summary>
	struct AABB {
		Vector3 min;			//!< 最小点
		Vector3 max;			//!< 最大点
	};

	/// <summary>
	/// OBB
	/// </summary>
	struct OBB {
		Vector3 center;				//!< 中心点
		Vector3 orientations[3];	//!< 座標軸。正規化・直交必須
		Vector3 size;				//!< 座標軸方向の長さの半分。中心から面までの距離
	};

	/// <summary>
	/// バネ
	/// </summary>
	struct Spring
	{
		Vector3 anchor;				// アンカー。固定された端の位置
		float naturalLength;		// 自然長
		float stiffness;			// 剛性。バネ定数k
		float dampungCoefficient;	// 減衰係数
	};

	/// <summary>
	/// 振り子
	/// </summary>
	struct Pendulum {
		Vector3 anchor;					// アンカーポイント。固定された端の位置
		float length;					// 紐の長さ
		float angle;					// 現在の角度
		float angularVelocity;			// 角速度ω
		float angularAcceleration;		// 角加速度
	};

	/// <summary>
	/// 円錐振り子
	/// </summary>
	struct ConicalPendulum {
		Vector3 anchor;					// アンカーポイント。固定された端の位置
		float length;					// 紐の長さ
		float halfApexAngle;			// 円錐の頂角の半分
		float angle;					// 現在の角度
		float angularVelocity;			// 角速度ω
	};

	/// <summary>
	/// Transform
	/// </summary>
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	/// <summary>
	/// 頂点データ(スプライト用)
	/// </summary>
	struct SpriteVertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	/// <summary>
	/// 頂点データ(3Dオブジェクト用)
	/// </summary>
	struct ModelVertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	/// <summary>
	/// 頂点データ(パーティクル)
	/// </summary>
	struct ParticleVertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector4 color;
	};
	

	/// <summary>
	/// マテリアル(モデル用)
	/// </summary>
	struct ModelMaterial {
		Vector4 color;
		int enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	/// <summary>
	/// マテリアル(スプライト用)
	/// </summary>
	struct SpriteMaterial {
		Vector4 color;
		int enableLighting;
		float padding[3];
	};

	/// <summary>
	/// トランスフォーメーション行列
	/// </summary>
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

	/// <summary>
	/// 平行光源
	/// </summary>
	struct DirectionalLight {
		Vector4 color;			//!< ライトの色
		Vector3 direction;		//!< ライトの向き
		float intensity;		//!< 輝度
	};

	/// <summary>
	/// MaterialData
	/// </summary>
	struct MaterialData {
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	/// <summary>
	/// objモデルデータ
	/// </summary>
	struct ModelData {
		std::vector<ModelVertexData> vertices;
		MaterialData material;
	};

	/// <summary>
	/// モデルファイルパス
	/// </summary>
	struct ModelFilePath {
		std::string directoryPath;
		std::string filename;
	};
	
	/// <summary>
	/// パーティクル
	/// </summary>
	struct Particle {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	/// <summary>
	/// ParticleForGPU
	/// </summary>
	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	/// <summary>
	/// カメラの位置
	/// </summary>
	struct CameraForGPU {
		Vector3 worldPosition;
	};
};

