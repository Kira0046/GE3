#include "Sprite.hlsli"

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

VSOutput main(float4 pos:POSITION, float2 uv : TEXCOORD) {
	VSOutput output;
	//output.svpos = pos;
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}