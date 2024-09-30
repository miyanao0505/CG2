#include "Particle.hlsli"

StructuredBuffer<TransformationMatrix> gTransfomationMatrix : register(t0);

struct VertexShaderInput {
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID) {
	VertexShaderOutput output;
    output.position = mul(input.position, gTransfomationMatrix[instanceId].WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3)gTransfomationMatrix[instanceId].World));
	return output;
}