struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 worldPosition : POSITION0;
};

struct TransformationMatrix
{
	float4x4 WVP;
	float4x4 World;
    float4x4 WoeldInverseTranspose;
};

struct Material
{
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
	float shininess;
};

struct DirectionalLight
{
	float4 color;
	float3 direction;
	float intensity;
};

struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
};

struct SpotLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
};

struct Camera
{
	float3 worldPosition;
};

