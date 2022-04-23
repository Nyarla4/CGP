////////////////////////////////////////////////////////////////////////////////
// Filename: particle.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ParticlePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 finalColor;

    // 이 텍스처 좌표 위치에서 샘플러를 사용하여 텍스처에서 픽셀 색상을 샘플링합니다.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	// 텍스처 색상과 입자 색상을 결합하여 최종 색상 결과를 얻습니다.
    finalColor = textureColor * input.color;

    if(finalColor.r == 0.0f)
	{
		finalColor.a = 0.0f;
	}
	else if (finalColor.r < 1.0f)
	{
		finalColor.a = finalColor.r;
	}
	// If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
	else
	{
		//finalColor.rgb = pixelColor.rgb;
		finalColor.a = 0.3f;
	}

    return finalColor;
}