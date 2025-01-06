#include "Particle.hlsli"

struct ParticleForGPU
{
    float4x4 WVP;
    float4x4 World;
    float4 Color;
};

StructuredBuffer<ParticleForGPU> gParticle : register(t0);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instancedId : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gParticle[instancedId].WVP);
    output.texcoord = input.texcoord;
    output.color = input.color * gParticle[instancedId].Color;
   
    return output;
}