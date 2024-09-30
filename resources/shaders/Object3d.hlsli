struct VertexShaderOutput
{
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

struct TransformationMatrix
{
	float32_t4x4 WVP;
	float32_t4x4 World;
};

struct Material
{
	float32_t4 color;
	int32_t enableLighting;
    float32_t4x4 uvTransform;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float32_t intensity;
};

