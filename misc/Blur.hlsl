float4x4 orthoMatrix : register(c0);
float4 params : register(c4);

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

struct PixelShaderInput {
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
	float4 color : TEXCOORD1;
};

struct VertexShaderInput {
	float3 position : POSITION0;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR0;
};

void VertexMain(in VertexShaderInput input, out PixelShaderInput output) {
	output.position = mul(float4(input.position.xy, 0, 1), orthoMatrix);
	output.position.x -= 1.0f;
	output.position.y = 1.0f - output.position.y;

	output.texCoord = input.texCoord;
	output.texCoord.x += 1.0f / (params.z * 2.0f);
	output.texCoord.y += 1.0f / (params.w * 2.0f);

	output.color = input.color;
}

float4 sinusInterpolate(float4 src, float4 dst, float pct) {
	float sinval = sin(pct * 3.1415926 / 2.0f);
	return sinval * dst + (1 - sinval) * src;
}

void PixelMain(in PixelShaderInput input, out float4 color : COLOR0) {
	color = 0;
	float2 samp = input.texCoord;

	for (int i = 0; i < 13; i++) {
		samp.x = input.texCoord.x + PixelKernel[i] * (1 / params.x);
		samp.y = input.texCoord.y + PixelKernel[i] * (1 / params.y);
		color += tex2D(foreground, samp.xy) * BlurWeights[i];
	}

	float2 position = input.texCoord * params.xy;
	if (position.x < BorderDiff ||
		position.y < BorderDiff ||
		params.x - position.x < BorderDiff ||
		params.y - position.y < BorderDiff)
	{
		float numPixels = 0;
		for (float curDiff = 1; curDiff < BorderDiff; ++curDiff) {
			samp = input.texCoord;
			samp.x -= (curDiff / params.x);
			numPixels += step(0, samp.x) * step(samp.x, 1);

			samp = input.texCoord;
			samp.x += (curDiff / params.x);
			numPixels += step(0, samp.x) * step(samp.x, 1);

			samp = input.texCoord;
			samp.y -= (curDiff / params.y);
			numPixels += step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.y += (curDiff / params.y);
			numPixels += step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.x -= (curDiff / params.x);
			samp.y -= (curDiff / params.y);
			numPixels += step(0, samp.x) * step(samp.x, 1) *
						 step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.x += (curDiff / params.x);
			samp.y -= (curDiff / params.y);
			numPixels += step(0, samp.x) * step(samp.x, 1) *
						 step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.x += (curDiff / params.x);
			samp.y += (curDiff / params.y);
			numPixels += step(0, samp.x) * step(samp.x, 1) *
						 step(0, samp.y) * step(samp.y, 1);

			samp = input.texCoord;
			samp.x -= (curDiff / params.x);
			samp.y += (curDiff / params.y);
			numPixels += step(0, samp.x) * step(samp.x, 1) *
						 step(0, samp.y) * step(samp.y, 1);

			float pct = numPixels / (curDiff * 8.0f);
			color = sinusInterpolate(float4(color.rgb, 0), color, pct);
		}
	}

	color *= input.color;
}
