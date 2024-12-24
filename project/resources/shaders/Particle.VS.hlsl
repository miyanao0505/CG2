#include "Particle.hlsli"

StructuredBuffer<TransformationMatrix> gTransfomationMatrices : register(t0);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instancedId : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransfomationMatrices[instancedId].WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gTransfomationMatrices[instancedId].World));
    return output;
}