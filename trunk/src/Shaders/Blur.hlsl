float4 g_textureSize : register(c0);

texture foregroundTexture;
sampler2D foreground = sampler_state {
	Texture = (foregroundTexture);
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
};

static const float PixelKernel[13] = {
	-6, -5, -4, -3, -2, -1,
	0, 1, 2, 3, 4, 5, 6,
};

static const float BorderDiff = 11.0f;

static const float BlurWeights[13] = {
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};

float4 sinusInterpolate(float4 src, float4 dst, float pct) {
	float sinval = sin(pct * 3.1415926 / 2.0f);
	return sinval * dst + (1 - sinval) * src;
}

struct PixelShaderInput {
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
	float4 color : TEXCOORD1;
};

// shader entry point
void PixelMain(in PixelShaderInput input, out float4 color : COLOR0) {
	color = 0;
	float2 samp = input.texCoord;

	for (int i = 0; i < 13; i++) {
		samp.x = input.texCoord.x + PixelKernel[i] * (1 / g_textureSize.x);
		samp.y = input.texCoord.y + PixelKernel[i] * (1 / g_textureSize.y);
		color += tex2D(foreground, samp.xy) * BlurWeights[i];
	}

	float2 position = input.texCoord * g_textureSize.xy;
	if (position.x < BorderDiff ||
		position.y < BorderDiff ||
		g_textureSize.x - position.x < BorderDiff ||
		g_textureSize.y - position.y < BorderDiff)
	{
		float numPixels = 0;
		for (float curDiff = 1; curDiff < BorderDiff; ++curDiff) {
			samp = input.texCoord;
			samp.x -= (curDiff / g_textureSize.x);
			numPixels += step(0, samp.x) * step(samp.x, 1);

			samp = input.texCoord;
			samp.x += (curDiff / g_textureSize.x);
			numPixels += step(0, samp.x) * step(samp.x, 1);

			samp = input.texCoord;
			samp.y -= (curDiff / g_textureSize.y);
			numPixels += step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.y += (curDiff / g_textureSize.y);
			numPixels += step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.x -= (curDiff / g_textureSize.x);
			samp.y -= (curDiff / g_textureSize.y);
			numPixels += step(0, samp.x) * step(samp.x, 1) *
						 step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.x += (curDiff / g_textureSize.x);
			samp.y -= (curDiff / g_textureSize.y);
			numPixels += step(0, samp.x) * step(samp.x, 1) *
						 step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.x += (curDiff / g_textureSize.x);
			samp.y += (curDiff / g_textureSize.y);
			numPixels += step(0, samp.x) * step(samp.x, 1) *
						 step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.x -= (curDiff / g_textureSize.x);
			samp.y += (curDiff / g_textureSize.y);
			numPixels += step(0, samp.x) * step(samp.x, 1) *
						 step(0, samp.y) * step(samp.y, 1);

			float pct = numPixels / (curDiff * 8.0f);
			color = sinusInterpolate(float4(color.rgb, 0), color, pct);
		}
	}

	color *= input.color;
}
