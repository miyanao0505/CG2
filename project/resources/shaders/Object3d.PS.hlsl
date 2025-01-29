#include "object3d.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	
	float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	// cameraの方向を算出
    float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
	
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
		
		// DirectionLight
		// half lambert
        float32_t NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float32_t cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		
		// 拡散反射
        float32_t3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		// 鏡面反射
        // 入射角の反射ベクトル
		float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float32_t NdotH = dot(normalize(input.normal), halfVector);
        float32_t specularPow = pow(saturate(NdotH), gMaterial.shininess); // 反射強度
		float32_t3 specularDirectionalLight = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);

		// 点光源の方向を算出
        float32_t3 pointLightdirection = normalize(input.worldPosition - gPointLight.position);
        float32_t distance = length(gPointLight.position - input.worldPosition); // 点光源への距離
        float32_t factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay); // 指数によるコントロール
        float32_t3 LightColor = gPointLight.color.rgb * gPointLight.intensity * factor;
		// 拡散反射
        NdotL = dot(normalize(input.normal), -pointLightdirection);
        cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float32_t3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * LightColor.rgb;
		// 鏡面反射
		// 入射角の反射ベクトル
        halfVector = normalize(-pointLightdirection + toEye);
        NdotH = dot(normalize(input.normal), halfVector);
        specularPow = pow(saturate(NdotH), gMaterial.shininess);
        float32_t3 specularPointLight = LightColor.rgb * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		
		// 拡散反射 + 鏡面反射
        output.color.rgb = diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else { // Lightingしない場合。前回までと同じ演算
		output.color = gMaterial.color * textureColor;
		
	}
	
	// output.colorのα値が0の時にPixelを棄却
	if (output.color.a == 0.0)
	{
		discard;
	}
	
	return output;
}