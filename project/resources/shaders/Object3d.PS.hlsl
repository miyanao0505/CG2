#include "object3d.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	
	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	// textureのα値が0.5以下の時にPixelを棄却
	if (textureColor.a <= 0.5)
	{
		discard;
	}
	// textureのα値が0の時にPixelで棄却
	if (textureColor.a == 0.0)
	{
		discard;
	}
	
	if (gMaterial.enableLighting != 0)
	{ // Lightingする場合
		// half lambert
		float32_t NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		float32_t cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		
		// cameraの方向を産出
		float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
		float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess); // 反射強度
		
		// 拡散反射
		float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		// 鏡面反射
		float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		// 拡散反射 + 鏡面反射
		output.color.rgb = diffuse + specular;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else
	{ // Lightingしない場合。前回までと同じ演算
		output.color = gMaterial.color * textureColor;
	}
	
	// output.colorのα値が0の時にPixelを棄却
	if (output.color.a == 0.0)
	{
		discard;
	}
	
	return output;
}