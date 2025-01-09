#include "Matrix.h"

///
/// 2x2行列関数 ここから
/// 

/// 2x2回転行列の作成関数
Matrix::Matrix2x2 Matrix::MakeRotateMatrix2x2(const float& theta)
{
	Matrix2x2 ans = { 0 };

	ans.m[0][0] = std::cos(theta);
	ans.m[0][1] = std::sin(theta);
	ans.m[1][0] = std::sin(theta) * -1.f;
	ans.m[1][1] = std::cos(theta);

	return ans;
}

/// 2x2行列の加算を返す関数
Matrix::Matrix2x2 Matrix::Add(const Matrix2x2& matrix1, const Matrix2x2& matrix2)
{
	Matrix2x2 ans = { 0 };

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			ans.m[i][k] = matrix1.m[i][k] + matrix2.m[i][k];
		}
	}

	return ans;
}

/// 2x2行列の減算を返す関数
Matrix::Matrix2x2 Matrix::Subtract(const Matrix2x2& matrix1, const Matrix2x2& matrix2)
{
	Matrix2x2 ans = { 0 };

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			ans.m[i][k] = matrix1.m[i][k] - matrix2.m[i][k];
		}
	}

	return ans;
}

/// 2x2行列の積を返す関数
Matrix::Matrix2x2 Matrix::Multiply(const Matrix2x2& matrix1, const Matrix2x2& matrix2)
{
	Matrix2x2 ans = { 0 };

	ans.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0];
	ans.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1];
	ans.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0];
	ans.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1];

	return ans;
}

/// スカラーと2x2行列の積を返す関数
Matrix::Matrix2x2 Matrix::Multiply(const float& scalar, const Matrix2x2& matrix)
{
	Matrix2x2 ans = { 0 };

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			ans.m[i][k] = scalar * matrix.m[i][k];
		}
	}

	return ans;
}

/// 2次元ベクトルと2x2行列の積を返す関数
MyBase::Vector2 Matrix::Multiply(const Vector2& vector, const Matrix2x2& matrix)
{
	Vector2 ans;

	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1];

	return ans;
}

/// 2x2逆行列を取得する関数
Matrix::Matrix2x2 Matrix::Inverse(const Matrix2x2& matrix)
{
	Matrix2x2 ans = { 0 };

	if (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0] == 0)
	{
		ans = matrix;
	}
	else
	{
		float scalar = 1 / (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]);

		ans.m[0][0] = matrix.m[1][1];
		ans.m[0][1] = matrix.m[0][1] * -1.f;
		ans.m[1][0] = matrix.m[1][0] * -1.f;
		ans.m[1][1] = matrix.m[0][0];

		ans = Multiply(scalar, ans);
	}

	return ans;
}

/// 2x2転置行列を取得する関数
Matrix::Matrix2x2 Matrix::Transpose(const Matrix2x2& matrix)
{
	Matrix2x2 ans = { 0 };

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			ans.m[k][i] = matrix.m[i][k];
		}
	}

	return ans;
}

///
/// 2x2行列関数 ここまで
/// 

/// 
/// 3x3行列関数 ここから
/// 

/// 3x3拡縮行列の作成
Matrix::Matrix3x3 Matrix::MakeScaleMatrix(const Vector2& scale)
{
	Matrix3x3 ans = { 0 };

	ans.m[0][0] = scale.x;
	ans.m[1][1] = scale.y;
	ans.m[2][2] = 1;

	return ans;
}

/// 3x3回転行列の作成
Matrix::Matrix3x3 Matrix::MakeRotateMatrix3x3(const float& theta)
{
	Matrix3x3 ans = { 0 };

	ans.m[0][0] = std::cos(theta);
	ans.m[0][1] = std::sin(theta);
	ans.m[1][0] = std::sin(theta) * -1.f;
	ans.m[1][1] = std::cos(theta);
	ans.m[2][2] = 1;

	return ans;
}

/// 3x3平行移動行列の作成関数
Matrix::Matrix3x3 Matrix::MakeTranslateMatrix(const Vector2& translate)
{
	Matrix3x3 ans = { 0 };

	ans.m[0][0] = 1;
	ans.m[1][1] = 1;
	ans.m[2][2] = 1;

	ans.m[2][0] = translate.x;
	ans.m[2][1] = translate.y;

	return ans;
}

/// 2次元アフィン変換行列の作成
Matrix::Matrix3x3 Matrix::MakeAffineMatrix(const Vector2& scale, const float& rotate, const Vector2& translate)
{
	Matrix3x3 scaleMatrix = { 0 };
	Matrix3x3 rotateMatrix = { 0 };
	Matrix3x3 translateMatrix = { 0 };
	Matrix3x3 worldMatrix = { 0 };

	// 拡縮行列を作成
	scaleMatrix = MakeScaleMatrix(scale);

	// 回転行列を作成
	rotateMatrix = MakeRotateMatrix3x3(rotate);

	// 平行移動行列を作成
	translateMatrix = MakeTranslateMatrix(translate);

	// 結合
	worldMatrix = Multiply(scaleMatrix, rotateMatrix);
	worldMatrix = Multiply(worldMatrix, translateMatrix);

	return worldMatrix;
}

/// 3x3正射影行列の作成
Matrix::Matrix3x3 Matrix::MakeOrthographicMatrix(const float& left, const float& top, const float& right, const float& bottom)
{
	Matrix3x3 ans = { 0 };

	ans.m[0][0] = 2.f / (right - left);
	ans.m[1][1] = 2.f / (top - bottom);
	ans.m[2][0] = (left + right) / (left - right);
	ans.m[2][1] = (top + bottom) / (bottom - top);
	ans.m[2][2] = 1;

	return ans;
}

/// 3x3ビューポート行列の作成
Matrix::Matrix3x3 Matrix::MakeViewportMatrix(const float& left, const float& top, const float& width, const float& height)
{
	Matrix3x3 ans = { 0 };

	ans.m[0][0] = width / 2;
	ans.m[1][1] = height / 2 * -1.f;
	ans.m[2][0] = left + width / 2;
	ans.m[2][1] = top + height / 2;
	ans.m[2][2] = 1;

	return ans;
}

/// 3x3行列の加算を返す関数
Matrix::Matrix3x3 Matrix::Add(const Matrix3x3& matrix1, Matrix3x3& matrix2)
{
	Matrix3x3 ans = { 0 };

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			ans.m[i][k] = matrix1.m[i][k] + matrix2.m[i][k];
		}
	}

	return ans;
}

/// 3x3行列の減算を返す関数
Matrix::Matrix3x3 Matrix::Subtract(const Matrix3x3& matrix1, Matrix3x3& matrix2)
{
	Matrix3x3 ans = { 0 };

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			ans.m[i][k] = matrix1.m[i][k] - matrix2.m[i][k];
		}
	}

	return ans;
}

/// 3x3行列の積を返す関数
Matrix::Matrix3x3 Matrix::Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
{
	Matrix3x3 ans = { 0 };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				ans.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}

	return ans;
}

/// スカラーと3x3行列の積を返す関数
Matrix::Matrix3x3 Matrix::Multipty(const float& scalar, const Matrix3x3& matrix)
{
	Matrix3x3 ans;

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			ans.m[i][k] = scalar * matrix.m[i][k];
		}
	}

	return ans;
}

/// 2次元ベクトルを同次座標として変換する関数
MyBase::Vector2 Matrix::Transform(const Vector2& vector, const Matrix3x3& matrix)
{
	Vector2 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.f * matrix.m[2][2];

	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;

	return result;
}

/// 3x3逆行列の取得する関数
Matrix::Matrix3x3 Matrix::Inverse(const Matrix3x3& matrix)
{
	Matrix3x3 ans;

	if (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] == 0)
	{
		ans = matrix;
	}
	else
	{
		float scalar = 1 / (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]);

		ans.m[0][0] = matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1];
		ans.m[0][1] = -(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]);
		ans.m[0][2] = matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1];

		ans.m[1][0] = -(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]);
		ans.m[1][1] = matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0];
		ans.m[1][2] = -(matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]);
		
		ans.m[2][0] = matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0];
		ans.m[2][1] = -(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]);
		ans.m[2][2] = matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0];

		ans = Multipty(scalar, ans);
	}

	return ans;
}

/// 3x3転置行列の取得する関数
Matrix::Matrix3x3 Matrix::Transpose(const Matrix3x3& matrix)
{
	Matrix3x3 ans;

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			ans.m[k][i] = matrix.m[i][k];
		}
	}

	return ans;
}


///
/// 3x3行列関数 ここまで
/// 

///
/// 4x4行列関数 ここから
/// 

/// 4x4拡縮行列の作成
MyBase::Matrix4x4 Matrix::MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = scale.x;
	ans.m[1][1] = scale.y;
	ans.m[2][2] = scale.z;
	ans.m[3][3] = 1;

	return ans;
}

/// X軸回転行列の作成
MyBase::Matrix4x4 Matrix::MakeRotateXMatrix4x4(const float& radian)
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = 1;
	ans.m[1][1] = std::cos(radian);
	ans.m[1][2] = std::sin(radian);
	ans.m[2][1] = -std::sin(radian);
	ans.m[2][2] = std::cos(radian);
	ans.m[3][3] = 1;

	return ans;
}

/// Y軸回転行列の作成
MyBase::Matrix4x4 Matrix::MakeRotateYMatrix4x4(const float& radian)
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = std::cos(radian);
	ans.m[0][2] = -std::sin(radian);
	ans.m[1][1] = 1;
	ans.m[2][0] = std::sin(radian);
	ans.m[2][2] = std::cos(radian);
	ans.m[3][3] = 1;

	return ans;
}

/// Z軸回転行列の作成
MyBase::Matrix4x4 Matrix::MakeRotateZMatrix4x4(const float& radian)
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = std::cos(radian);
	ans.m[0][1] = std::sin(radian);
	ans.m[1][0] = -std::sin(radian);
	ans.m[1][1] = std::cos(radian);
	ans.m[2][2] = 1;
	ans.m[3][3] = 1;

	return ans;
}

/// 4x4回転行列の作成
MyBase::Matrix4x4 Matrix::MakeRotateMatrix4x4(const float& radianX, const float& radianY, const float& radianZ)
{
	Matrix4x4 rotateX = MakeRotateXMatrix4x4(radianX);
	Matrix4x4 rotateY = MakeRotateYMatrix4x4(radianY);
	Matrix4x4 rotateZ = MakeRotateZMatrix4x4(radianZ);
	Matrix4x4 ans = { 0 };

	ans = Multiply(rotateX, Multiply(rotateY, rotateZ));

	return ans;
}

/// 4x4平行移動行列の作成
MyBase::Matrix4x4 Matrix::MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = 1;
	ans.m[1][1] = 1;
	ans.m[2][2] = 1;
	ans.m[3][3] = 1;

	ans.m[3][0] = translate.x;
	ans.m[3][1] = translate.y;
	ans.m[3][2] = translate.z;

	return ans;
}

/// 3次元アフィン変換行列の作成
MyBase::Matrix4x4 Matrix::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = MakeRotateMatrix4x4(rotate.x, rotate.y, rotate.z);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 worldMatrix = { 0 };

	worldMatrix = Multiply(scaleMatrix, rotateMatrix);
	worldMatrix = Multiply(worldMatrix, translateMatrix);

	return worldMatrix;
}

/// 4x4透視投影行列の作成
MyBase::Matrix4x4 Matrix::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = (1 / aspectRatio) * (1 / std::tan(fovY / 2));
	ans.m[1][1] = (1 / std::tan(fovY / 2));
	ans.m[2][2] = farClip / (farClip - nearClip);
	ans.m[2][3] = 1;
	ans.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return ans;
}

/// 4x4正射影行列の作成
MyBase::Matrix4x4 Matrix::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = 2 / (right - left);
	ans.m[1][1] = 2 / (top - bottom);
	ans.m[2][2] = 1 / (farClip - nearClip);
	ans.m[3][0] = (left + right) / (left - right);
	ans.m[3][1] = (top + bottom) / (bottom - top);
	ans.m[3][2] = nearClip / (nearClip - farClip);
	ans.m[3][3] = 1;

	return ans;
}
	
/// 4x4ビューポート変換行列の作成
MyBase::Matrix4x4 Matrix::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = width / 2;
	ans.m[1][1] = -height / 2;
	ans.m[2][2] = maxDepth - minDepth;
	ans.m[3][0] = left + width / 2;
	ans.m[3][1] = top + height / 2;
	ans.m[3][2] = minDepth;
	ans.m[3][3] = 1;

	return ans;
}

/// 4x4単位行列の作成
MyBase::Matrix4x4 Matrix::MakeIdentity4x4()
{
	Matrix4x4 ans = { 0 };

	ans.m[0][0] = 1;
	ans.m[1][1] = 1;
	ans.m[2][2] = 1;
	ans.m[3][3] = 1;

	return ans;
}

/// 4x4行列の加算を返す関数
MyBase::Matrix4x4 Matrix::Add(const Matrix4x4& matrix1, const Matrix4x4& matrix2)
{
	Matrix4x4 ans = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ans.m[i][j] = matrix1.m[i][j] + matrix2.m[i][j];
		}
	}

	return ans;
}

/// 4x4行列の減算を返す関数
MyBase::Matrix4x4 Matrix::Subtract(const Matrix4x4& matrix1, const Matrix4x4& matrix2)
{
	Matrix4x4 ans = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ans.m[i][j] = matrix1.m[i][j] - matrix2.m[i][j];
		}
	}

	return ans;
}

/// 4x4行列の積を返す関数
MyBase::Matrix4x4 Matrix::Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2)
{
	Matrix4x4 ans = { 0 };

	/*for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ans.m[i][j] = matrix1.m[i][0] * matrix2.m[0][j] + matrix1.m[i][1] * matrix2.m[1][j] + matrix1.m[i][2] * matrix2.m[2][j] + matrix1.m[i][3] * matrix2.m[3][j];
		}
	}*/

	ans.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] + matrix1.m[0][2] * matrix2.m[2][0] + matrix1.m[0][3] * matrix2.m[3][0];
	ans.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] + matrix1.m[0][2] * matrix2.m[2][1] + matrix1.m[0][3] * matrix2.m[3][1];
	ans.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] + matrix1.m[0][2] * matrix2.m[2][2] + matrix1.m[0][3] * matrix2.m[3][2];
	ans.m[0][3] = matrix1.m[0][0] * matrix2.m[0][3] + matrix1.m[0][1] * matrix2.m[1][3] + matrix1.m[0][2] * matrix2.m[2][3] + matrix1.m[0][3] * matrix2.m[3][3];

	ans.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] + matrix1.m[1][2] * matrix2.m[2][0] + matrix1.m[1][3] * matrix2.m[3][0];
	ans.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] + matrix1.m[1][2] * matrix2.m[2][1] + matrix1.m[1][3] * matrix2.m[3][1];
	ans.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] + matrix1.m[1][2] * matrix2.m[2][2] + matrix1.m[1][3] * matrix2.m[3][2];
	ans.m[1][3] = matrix1.m[1][0] * matrix2.m[0][3] + matrix1.m[1][1] * matrix2.m[1][3] + matrix1.m[1][2] * matrix2.m[2][3] + matrix1.m[1][3] * matrix2.m[3][3];

	ans.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] + matrix1.m[2][2] * matrix2.m[2][0] + matrix1.m[2][3] * matrix2.m[3][0];
	ans.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] + matrix1.m[2][2] * matrix2.m[2][1] + matrix1.m[2][3] * matrix2.m[3][1];
	ans.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] + matrix1.m[2][2] * matrix2.m[2][2] + matrix1.m[2][3] * matrix2.m[3][2];
	ans.m[2][3] = matrix1.m[2][0] * matrix2.m[0][3] + matrix1.m[2][1] * matrix2.m[1][3] + matrix1.m[2][2] * matrix2.m[2][3] + matrix1.m[2][3] * matrix2.m[3][3];

	ans.m[3][0] = matrix1.m[3][0] * matrix2.m[0][0] + matrix1.m[3][1] * matrix2.m[1][0] + matrix1.m[3][2] * matrix2.m[2][0] + matrix1.m[3][3] * matrix2.m[3][0];
	ans.m[3][1] = matrix1.m[3][0] * matrix2.m[0][1] + matrix1.m[3][1] * matrix2.m[1][1] + matrix1.m[3][2] * matrix2.m[2][1] + matrix1.m[3][3] * matrix2.m[3][1];
	ans.m[3][2] = matrix1.m[3][0] * matrix2.m[0][2] + matrix1.m[3][1] * matrix2.m[1][2] + matrix1.m[3][2] * matrix2.m[2][2] + matrix1.m[3][3] * matrix2.m[3][2];
	ans.m[3][3] = matrix1.m[3][0] * matrix2.m[0][3] + matrix1.m[3][1] * matrix2.m[1][3] + matrix1.m[3][2] * matrix2.m[2][3] + matrix1.m[3][3] * matrix2.m[3][3];

	return ans;
}

/// スカラーと4x4行列の積を返す関数
MyBase::Matrix4x4 Matrix::Multiply(const float& scalar, const Matrix4x4& matrix)
{
	Matrix4x4 ans = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ans.m[i][j] = scalar * matrix.m[i][j];
		}
	}

	return ans;
}

/// 3次元ベクトルを同時座標として変換する
MyBase::Vector3 Matrix::Transform(const Vector3& vector, const Matrix4x4 matrix)
{
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.f * matrix.m[3][3];

	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

/// 4x4行列をスケールと回転の3次元ベクトルに変換して取得
MyBase::Vector3 Matrix::TransformNormal(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2];

	return result;
}

/// 4x4逆行列の作成
MyBase::Matrix4x4 Matrix::Inverse(const Matrix4x4& matrix)
{
	Matrix4x4 ans = { 0 };
	float determinant = Determinant(matrix);

	if (determinant == 0)
	{
		ans = matrix;
	}
	else
	{
		float scalar = 1 / determinant;

		ans.m[0][0] = matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]
					- matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2];
		ans.m[0][1] = -matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2]
					+ matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2];
		ans.m[0][2] = matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2]
					- matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2];
		ans.m[0][3] = -matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2]
					+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2];

		ans.m[1][0] = -matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]
					+ matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2];
		ans.m[1][1] = matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2]
					- matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2];
		ans.m[1][2] = -matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2]
					+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2];
		ans.m[1][3] = matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2]
					- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2];

		ans.m[2][0] = matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1]
					- matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1];
		ans.m[2][1] = -matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1]
					+ matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1];
		ans.m[2][2] = matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1]
					- matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1];
		ans.m[2][3] = -matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1]
					+ matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1];

		ans.m[3][0] = -matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] - matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1]
					+ matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] + matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1];
		ans.m[3][1] = matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] + matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1]
					- matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1];
		ans.m[3][2] = -matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] - matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1]
					+ matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1];
		ans.m[3][3] = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1]
					- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];

		ans = Multiply(scalar, ans);
	}

	return ans;
}

/// 4x4行列式の作成
float Matrix::Determinant(const Matrix4x4& matrix)
{
	float ans = 0;

	ans = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]
		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]
		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]
		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]
		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]
		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]
		- matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]
		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];

	return ans;
}

/// 4x4転置行列の作成
MyBase::Matrix4x4 Matrix::Transpose(const Matrix4x4& matrix)
{
	Matrix4x4 ans = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ans.m[j][i] = matrix.m[i][j];
		}
	}

	return ans;
}

/// 
/// 4x4行列関数 ここまで
/// 

///
/// 描画関数 ここから
/// 

/// 2x2行列の表示
//void Matrix::MatrixScreenPrintf(int x, int y, const Matrix2x2& matrix, const char* label)
//{
//	
//
//	for (int row = 0; row < 2; ++row)
//	{
//		for (int column = 0; column < 2; ++column)
//		{
//			
//		}
//	}
//}

/// 3x3行列の表示
//void Matrix::MatrixScreenPrintf(int x, int y, const Matrix3x3& matrix, const char* label)
//{
//	
//
//	for (int row = 0; row < 3; ++row)
//	{
//		for (int column = 0; column < 3; ++column)
//		{
//			
//		}
//	}
//}

/// 4x4行列の表示
//void Matrix::MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
//{
//	
//
//	for (int row = 0; row < 4; ++row)
//	{
//		for (int column = 0; column < 4; ++column)
//		{
//			
//		}
//	}
//}

///
/// 描画関数 ここまで
/// 
