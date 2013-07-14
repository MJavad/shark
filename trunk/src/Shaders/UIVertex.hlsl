float4x4 g_orthoMatrix : register(c0);

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

// shader entry point
void VertexMain(in VertexShaderInput input, out PixelShaderInput output) {
	output.position = mul(float4(input.position.xy, 0, 1), g_orthoMatrix);
	output.position.x -= 1.0f;
	output.position.y = 1.0f - output.position.y;
	output.texCoord = input.texCoord;
	output.color = input.color;
}
