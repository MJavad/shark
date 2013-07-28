/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

float4 g_extent : register(c0);
float4 g_horizontalRadius : register(c1);
float4 g_verticalRadius : register(c2);
float4 g_stroke : register(c3);

float getEllipseRadius(float x, float y, float2 radius) {
	float phi = atan2(y, x);
	float sinp = sin(phi);
	float cosp = cos(phi);
	float a2 = radius.x * radius.x;
	float b2 = radius.y * radius.y;
	float ab = radius.x * radius.y;

	return ab / sqrt(a2 * (sinp * sinp) + b2 * (cosp * cosp));
}

float4 sinusInterpolate(float4 src, float4 dst, float pct) {
	float sinval = sin(pct * 3.1415926 / 2.0f);
	return sinval * dst + (1 - sinval) * src;
}

// Returns:
// 0 -> top left
// 1 -> bottom left
// 2 -> top right
// 3 -> bottom right
// -2 -> "anti alias area"
// -1 -> inside the solid area
int getBorderQuad(float2 pos) {
	if (pos.x <= g_horizontalRadius.x + 2 && pos.y <= g_verticalRadius.x + 2)
		return 0;
	else if (pos.x <= g_horizontalRadius.z + 2 && (g_extent.y - pos.y) <= g_verticalRadius.z + 2)
		return 1;
	else if ((g_extent.x - pos.x) <= g_horizontalRadius.y + 2 && pos.y <= g_verticalRadius.y + 2)
		return 2;
	else if ((g_extent.x - pos.x) <= g_horizontalRadius.w + 2 && (g_extent.y - pos.y) <= g_verticalRadius.w + 2)
		return 3;

	if (pos.x <= 2 || pos.y <= 2 || (g_extent.x - pos.x) <= 2 || (g_extent.y - pos.y) <= 2)
		return -2;

	return -1;
}

float4 getColorFill(float4 color, float2 pos, float2 texCoord) {
	// the current quad (where we are)
	int quad = getBorderQuad(pos);

	// solid area...
	if (quad == -1) {
		float cy = 2 / g_extent.y;
		if ((pos.x - 2) <= g_stroke.x ||
			(texCoord.y - cy) <= g_stroke.y ||
			(g_extent.x - 2 - pos.x) <= g_stroke.x ||
			(1 - cy - texCoord.y) <= g_stroke.y)
			return color;

		// get antialias percentage...
		float pct = 0.0f;
		if (pos.x <= 3)
			pct = (2 - pos.x + 2) / 2.0f;
		else if (pos.y <= 3)
			pct = (2 - pos.y + 2) / 2.0f;
		else if ((g_extent.x - pos.x) <= 3)
			pct = (g_extent.x - 2 - pos.x) / 2.0f;
		else if ((g_extent.y - pos.y) <= 3)
			pct = (g_extent.y - 2 - pos.y) / 2.0f;
		else
			return float4(color.rgb, 0);

		return sinusInterpolate(color, float4(color.rgb, 0), pct);
	}
	
	// anti alias area...
	if (quad == -2) {
		float pct = 0.0f;
		if (pos.x <= 2)
			pct = (2 - pos.x) / 2.0f;
		else if (pos.x >= (g_extent.x - 2))
			pct = (pos.x - g_extent.x + 2) / 2.0f;
		else if (pos.y <= 2)
			pct = (2 - pos.y) / 2.0f;
		else if (pos.y >= (g_extent.y - 2))
			pct = (pos.y - g_extent.y + 2) / 2.0f;

		return sinusInterpolate(color, float4(color.rgb, 0), pct);
	}

	// the radius for the current quad and circle center
	float2 midPoint, midRadius;
	if (quad == 0) {
		midRadius.x = g_horizontalRadius.x;
		midRadius.y = g_verticalRadius.x;
		midPoint = float2(midRadius.x + 2, midRadius.y + 2);
	}
	else if (quad == 1) {
		midRadius.x = g_horizontalRadius.z;
		midRadius.y = g_verticalRadius.z;
		midPoint = float2(midRadius.x + 2, g_extent.y - midRadius.y - 2);
	}
	else if (quad == 2) {
		midRadius.x = g_horizontalRadius.y;
		midRadius.y = g_verticalRadius.y;
		midPoint = float2(g_extent.x - midRadius.x - 2, midRadius.y + 2);
	}
	else if (quad == 3) {
		midRadius.x = g_horizontalRadius.w;
		midRadius.y = g_verticalRadius.w;
		midPoint = float2(g_extent.x - midRadius.x - 2, g_extent.y - midRadius.y - 2);
	}

	float x = pos.x - midPoint.x;
	float y = pos.y - midPoint.y;

	float radius = getEllipseRadius(x, y, midRadius);
	float radiusCoord = sqrt(x * x + y * y);
	float diff = radiusCoord - radius;

	// inside the circle?
	if (radiusCoord <= radius) {
		if((radius - radiusCoord) <= g_stroke.x)
			return color;

		diff = (radius - radiusCoord) - g_stroke.x;
	}

	// outer corner anti alias...
	if (diff > 0.0f)
		return float4(color.rgb, 0);

	return sinusInterpolate(color, float4(color.rgb, 0), diff / 2.0f);
}

struct PixelShaderInput {
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
	float4 color : TEXCOORD1;
};

// shader entry point
void PixelMain(in PixelShaderInput input, out float4 color : COLOR0) {
	color = getColorFill(input.color, float2(g_extent.x * input.texCoord.x, g_extent.y * input.texCoord.y), input.texCoord);
}
