
float4x4 orthoMatrix : register(c0);
float4 params1 : register(c4);
float4 params2 : register(c5);

float4 horizRadius : register(c6);
float4 vertRadius : register(c7);

struct VertexShaderInput
{
	float3 position : POSITION0;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR0;
};

struct PixelShaderInput
{
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
	float4 color : TEXCOORD1;
};

void VertexMain(in VertexShaderInput vsInput, out PixelShaderInput output)
{
	output.position = mul(float4(vsInput.position.xy, 0, 1), orthoMatrix);
	output.position.x -= 1.0f;
	output.position.y = 1.0f - output.position.y;
	output.position.x -= 1.0f / (params2.z * 2.0f);
	output.position.y -= 1.0f / (params2.w * 2.0f);
	output.texCoord = vsInput.texCoord;
	output.color = vsInput.color;
}

float getEllipseRadius(float x, float y, float2 radius)
{
	float phi = atan2(y, x);
	float sinp = sin(phi);
	float cosp = cos(phi);
	float a2 = radius.x * radius.x;
	float b2 = radius.y * radius.y;
	float ab = radius.x * radius.y;

	return ab / sqrt(a2 * (sinp * sinp) + b2 * (cosp * cosp));
}

float4 sinusInterpolate(float4 src, float4 dest, float pct)
{
	float sinval = sin(pct * 3.1415926 / 2.0f);
	return sinval * src + (1 - sinval) * dest;
}

// Returns:
// 0 -> top left
// 1 -> bottom left
// 2 -> top right
// 3 -> bottom right
// -2 -> "anti alias area"
// -1 -> inside the solid area
int getBorderQuad(float2 pos)
{
	if(pos.x <= horizRadius.x + 2 && pos.y <= vertRadius.x + 2)
		return 0;
	else if(pos.x <= horizRadius.z + 2 && (params2.y - pos.y) <= vertRadius.z + 2)
		return 1;
	else if((params2.x - pos.x) <= horizRadius.y + 2 && pos.y <= vertRadius.y + 2)
		return 2;
	else if((params2.x - pos.x) <= horizRadius.w + 2 && (params2.y - pos.y) <= vertRadius.w + 2)
		return 3;

	if(pos.x <= 2 || pos.y <= 2 || (params2.x - pos.x) <= 2 || (params2.y - pos.y) <= 2)
		return -2;

	return -1;
}

float4 getColorFill(float4 color, float2 pos, bool fill, float2 texCoord)
{
	int quad = getBorderQuad(pos);
	
	if(quad == -2)
	{
		float pct = 0.0f;
		if(pos.x <= 2)
			pct = (2 - pos.x) / 2.0f;
		else if(pos.x >= (params2.x - 2))
			pct = (pos.x - params2.x + 2) / 2.0f;
		else if(pos.y <= 2)
			pct = (2 - pos.y) / 2.0f;
		else if(pos.y >= (params2.y - 2))
			pct = (pos.y - params2.y + 2) / 2.0f;

		return sinusInterpolate(color, float4(color.rgb, 0), 1 - pct);
	}

	if(quad == -1)
	{
		if(fill)
			return color;

		float strokex = params1.x;
		float strokey = params1.y;

		float cy = 2 / params2.y;

		if((pos.x - 2) <= strokex)
			return color;
		if((texCoord.y - cy) <= strokey)
			return color;
		if((params2.x - 2 - pos.x) <= strokex)
			return color;
		if((1 - cy - texCoord.y) <= strokey)
			return color;

		float pct = 0.0f;
		if(pos.x <= 3)
			pct = (2 - pos.x + 2) / 2.0f;
		else if(pos.y <= 3)
			pct = (2 - pos.y + 2) / 2.0f;
		else if((params2.x - pos.x) <= 3)
			pct = (params2.x - 2 - pos.x) / 2.0f;
		else if((params2.y - pos.y) <= 3)
			pct = (params2.y - 2 - pos.y) / 2.0f;
		else
			return float4(color.rgb, 0);

		return sinusInterpolate(color, float4(color.rgb, 0), 1 - pct);
	}

	float2 midPoint;
	float2 midRadius;
	
	if(quad == 0)
	{
		midRadius.x = horizRadius.x;
		midRadius.y = vertRadius.x;
		midPoint = float2(midRadius.x + 2, midRadius.y + 2);
	}
	else if(quad == 1)
	{
		midRadius.x = horizRadius.z;
		midRadius.y = vertRadius.z;
		midPoint = float2(midRadius.x + 2, params2.y - midRadius.y - 2);
	}
	else if(quad == 2)
	{
		midRadius.x = horizRadius.y;
		midRadius.y = vertRadius.y;
		midPoint = float2(params2.x - midRadius.x - 2, midRadius.y + 2);
	}
	else if(quad == 3)
	{
		midRadius.x = horizRadius.w;
		midRadius.y = vertRadius.w;
		midPoint = float2(params2.x - midRadius.x - 2, params2.y - midRadius.y - 2);
	}

	float x = pos.x - midPoint.x;
	float y = pos.y - midPoint.y;

	float radius = getEllipseRadius(x, y, midRadius);
	float radiusCoord = sqrt(x * x + y * y);
	if(fill)
	{
		if(radiusCoord <= radius)
			color = color;
		else
		{
			float diff = radiusCoord - radius;
			if(diff > 2)
				color = float4(color.rgb, 0);
			else
				color = sinusInterpolate(color, float4(color.rgb, 0), 1.0f - diff / 2.0f);
		}

		return color;
	}
	else
	{
		if(radiusCoord <= radius)
		{
			if((radius - radiusCoord) <= params1.x)
				color = color;
			else
			{
				float diff = (radius - radiusCoord) - params1.x;
				if(diff > 0)
					color = float4(color.rgb, 0);
				else
					color = sinusInterpolate(color, float4(color.rgb, 0), 1.0f - diff / 2.0f);
			}
		}
		else
		{
			float diff = radiusCoord - radius;
			if(diff > 0)
				color = float4(color.rgb, 0);
			else
				color = sinusInterpolate(color, float4(color.rgb, 0), 1.0f - diff / 2.0f);
		}

		return color;
	}
}

void PixelMainFill(in PixelShaderInput input, out float4 color : COLOR0)
{
	color = getColorFill(input.color, float2(params2.x * input.texCoord.x, params2.y * input.texCoord.y), true, input.texCoord);
}

void PixelMainDraw(in PixelShaderInput input, out float4 color : COLOR0)
{
	color = getColorFill(input.color, float2(params2.x * input.texCoord.x, params2.y * input.texCoord.y), false, input.texCoord);
}