//IF YOU RECOMPILE A SHADER YOU MUST PUT THE NEW .CSO INTO THE RESOURCE FILE (sorry)

//cbuffer CBuf {
//	//int size;
//	float colors;
//	float x;
//	float y;
//	float z;
//	vector<int, 4> circles[1000];
//};

//float4 main(float4 pos : SV_POSITION) : SV_TARGET
float4 main(float3 color : Color) : SV_TARGET
{
	//for(int i = 0; i < 1000; i++) {
	//	int left = circles[i][0];
	//	int top = circles[i][1];
	//	int right = left + 30;
	//	int bottom = top + 30;
	//	if(pos.x > left && pos.x < right && pos.y > top && pos.y < bottom) {		
	//		return float4(colors, top/4.0f/255.0f, left/6.0f/255.0f, 1.0f);
	//		//return float4(colors, top/1280.0f, left/720.0f, 1.0f);
	//	}
	//}
	//return float4(0.0f,0.0f,0.0f,0.0f); //float4(pos.x/1920.0f, pos.y/1080.0f, 0.0f, 1.0f);
	return float4(color,1.0f);//float4(1.0f,1.0f,1.0f,1.0f);
}