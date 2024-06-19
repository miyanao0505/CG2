#pragma once

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
	/// Transform
	/// </summary>
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	/// <summary>
	/// 頂点データ
	/// </summary>
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
};

