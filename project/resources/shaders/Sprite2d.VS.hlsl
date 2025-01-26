#include "sprite2d.hlsli"

ConstantBuffer<TransformationMatrix> gTransfomationMatrix : register(b0);

struct VertexShaderInput {
	float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
    output.position = mul(input.position, gTransfomationMatrix.WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3)gTransfomationMatrix.World));
	return output;
}