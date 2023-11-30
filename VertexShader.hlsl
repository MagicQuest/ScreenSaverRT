//IF YOU RECOMPILE A SHADER YOU MUST PUT THE NEW .CSO INTO THE RESOURCE FILE (sorry)

struct VSOut {
	float3 color : Color;
	float4 pos : SV_POSITION;
};

//cbuffer CBuf {
//	matrix transform;
//};

VSOut main( float2 pos : Position, float3 color : Color)
//float4 main(float2 pos : Position) : SV_POSITION
{
	VSOut vso;
	vso.pos = float4(pos.x,pos.y,0.0f,1.0f);//mul(float4(pos, 1.0f), transform);
	vso.color = color;
	return vso;
	//return mul(float4(pos,1.0f), transform);
	//return float4(pos.x,pos.y, 0.0f, 1.0f);
}