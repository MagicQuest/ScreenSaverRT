#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace DX = DirectX;

#define SafeRelease(x) if(x != nullptr) x->Release()//; x = nullptr

void GetRealError(HRESULT shit, const wchar_t* lol) {
	wchar_t* message;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, shit, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&message, 0, NULL);
	MessageBoxW(NULL, message, lol, MB_OK | MB_ICONERROR);
	LocalFree(message);
}

struct Vertex {
	float x;
	float y;
	float r;
	float g;
	float b;
};

class Direct3D {
public:
	ID3D11Device* device = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* renderTarget = nullptr;
	ID3DBlob* blob = nullptr;
	int width, height;
	//ID3D11DepthStencilView* DSV = nullptr;
	//const Vertex vertices[6]{
	//	//{1.0f, 0.0f},
	//	//{1.0f, 1.0f},
	//	//{0.0f, 1.0f},
	//	//{0.0f, 0.0f},
	//	//{1.0f, 0.0f},
	//	//
	//	//{0.0f, 0.5f},
	//	//{0.5f, -0.5f},
	//	//{-0.5f, -0.5f},
	//	//{0.0f, 0.5f},
	//	{0.0f,0.0f},
	//	{1.0f,0.0f},
	//	{1.0f,-1.0f},
	//
	//	{1.0f,-1.0f},
	//	{0.0f, -1.0f},
	//	{0.0f, 0.0f},
	//
	//	//{1.0F,1.0F},
	//	//{1.0F,-1.0F},
	//	//{-1.0F,-1.0F},
	//	//{-1.0F,1.0F},
	//	//{1.0F,1.0F},
	//};
	Direct3D(HWND window) {//, bool& worked) {
		DXGI_SWAP_CHAIN_DESC sd{};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = window;//(HWND)696969421;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

#ifdef _DEBUG
		HRESULT shit = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context);
#else
		HRESULT shit = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context);
#endif
		if (FAILED(shit)) {
			//MessageBoxA(NULL, "FAILED TO CREATE DEVICE OR SWAP CHAIN", "HELP", MB_OK | MB_ICONERROR);
			GetRealError(shit, L"FAILED TO CREATE DEVICE OR SWAP CHAIN");
			//worked = false;
			return;
		}

		ID3D11Resource* backBuffer = nullptr;
		shit = swapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&backBuffer);
		if (FAILED(shit)) {
			//MessageBoxA(NULL, "faield  to get swap chain buffer", "HELP, ", MB_OK | MB_ICONERROR);
			GetRealError(shit, L"faield  to get swap chain buffer");
			//worked = false;
			return;
		}
		shit = device->CreateRenderTargetView(backBuffer, nullptr, &renderTarget);
		if (FAILED(shit)) {
			//MessageBoxA(NULL, "afiled to create reder target viw", "HELP, ", MB_OK | MB_ICONERROR);
			GetRealError(shit, L"afiled to create reder target viw");
			//worked = false;
			return;
		}
		backBuffer->Release();

		//D3D11_DEPTH_STENCIL_DESC dsDesc{};
		//dsDesc.DepthEnable = true;
		//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		//ID3D11DepthStencilState* DSState;
		//shit = device->CreateDepthStencilState(&dsDesc, &DSState);
		//if (FAILED(shit)) {
		//	GetRealError(shit, L"faiiled to  hgrraCREATE DEPTH SETENCIL STATE!");
		//}
		//context->OMSetDepthStencilState(DSState, 1u);

		//ID3D11Texture2D* depthStencil;
		//D3D11_TEXTURE2D_DESC descDepth{};
		//descDepth.Width = 1280;
		//descDepth.Height = 720;
		//descDepth.MipLevels = 1u;
		//descDepth.ArraySize = 1u;
		//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		//descDepth.SampleDesc.Count = 1u;
		//descDepth.SampleDesc.Quality = 0u;
		//descDepth.Usage = D3D11_USAGE_DEFAULT;
		//descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//shit = device->CreateTexture2D(&descDepth, nullptr, &depthStencil);
		//if (FAILED(shit)) {
		//	GetRealError(shit, L"GRAHHHH CREATE DEPTH TEXTURE 2D");
		//}
		//
		//D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
		//descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		//descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		//descDSV.Texture2D.MipSlice = 0u;
		//shit = device->CreateDepthStencilView(depthStencil, &descDSV, &DSV);
		//
		context->OMSetRenderTargets(1u, &renderTarget, nullptr);// DSV);

		ID3D11PixelShader* pixelShader;

		//https://stackoverflow.com/questions/20978889/including-filesdll-txt-etc-in-exe-application-in-c
		//aw idk if it works like that (wait)
		//ID3DBlob* b;
		//D3DCreateBlob(10000, &b);
		//b->GetBufferPointer() //so do i just write to this???? (it might be go time)

		//shit = D3DReadFileToBlob(L"PixelShader.cso", &blob);
		HRSRC hpsInfo = FindResource(NULL, MAKEINTRESOURCE(101), L"cso"); //ps for pixelshader
		if (hpsInfo == NULL) {
			GetRealError(0, L"FindResource failed for 101 (pixelshader.cso)");
			//worked = false;
			return;
		}
		HGLOBAL hps = LoadResource(NULL, hpsInfo);
		LPVOID lppsData = LockResource(hps);

		DWORD dpsSize = SizeofResource(NULL, hpsInfo);

		shit = D3DCreateBlob(dpsSize, &blob);
		memcpy(blob->GetBufferPointer(), lppsData, dpsSize);
		if (FAILED(shit)) {
			GetRealError(shit, L"FAILED TO READ pxieskahders -> MAKE BLOB");
			//worked = false;
			return;
		}

		shit = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
		if (FAILED(shit)) {
			GetRealError(shit, L"Create VPixelexe shader");
			//worked = false;
			return;
		}

		context->PSSetShader(pixelShader, nullptr, 0u);
		blob->Release();

		ID3D11VertexShader* vertexShader;

		//shit = D3DReadFileToBlob(L"VertexShader.cso", &blob);
		HRSRC hvsInfo = FindResource(NULL, MAKEINTRESOURCE(102), L"cso"); //vs for pixelshader
		if (hvsInfo == NULL) {	
			GetRealError(0, L"FindResource failed for 102 (vertexshader.cso)");
			//worked = false;
			return;
		}
		HGLOBAL hvs = LoadResource(NULL, hvsInfo);
		LPVOID lpvsData = LockResource(hvs);

		DWORD dvsSize = SizeofResource(NULL, hvsInfo);

		shit = D3DCreateBlob(dvsSize, &blob);
		memcpy(blob->GetBufferPointer(), lpvsData, dvsSize);
		if (FAILED(shit)) {
			GetRealError(shit, L"FAILED TO READ -> MAKE BLOB");
			//worked = false;
			return;
		}

		shit = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
		if (FAILED(shit)) {
			GetRealError(shit, L"Create Vertexe shader");
			//worked = false;
			return;
		}

		context->VSSetShader(vertexShader, nullptr, 0u);

		//context->OMSetRenderTargets(1u, &renderTarget, nullptr);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //https://gamedev.stackexchange.com/questions/98283/how-do-i-draw-a-full-screen-quad-in-directx-11

		////noexcept;
		//depthStencil->Release();
		//DSState->Release();
		RECT rc; GetWindowRect(window, &rc);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
		D3D11_VIEWPORT vp;
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1u, &vp);
		//MessageBoxA(NULL, ("w: "+std::to_string(width)+" h: "+std::to_string(height)).c_str(), "HE", MB_OK);
	}

	~Direct3D() {
		SafeRelease(renderTarget);
		SafeRelease(context);
		SafeRelease(swapChain);
		SafeRelease(device);
		SafeRelease(blob);
	}

	void Present() {
		HRESULT hr;
		if (FAILED(hr = swapChain->Present(1u, 0u))) {
			if (hr == DXGI_ERROR_DEVICE_REMOVED) {
				//const char* shit = "HELLP -> ";
				//LPTSTR message;
				//FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &message, 0, NULL);
				//strcat(message, shit);
				//GetRealError(hr, L"DEVICE REMOVED");
				GetRealError(device->GetDeviceRemovedReason(), L"DEVICE REMOVED");
				//MessageBoxA(NULL, "HELPP -> ", "HDEVICE RMEOVBNIENTGV", MB_ABORTRETRYIGNORE | MB_ICONERROR);
			}
		}
	}

	void Clear(float red, float green, float blue, float alpha = 1.0F) {
		const float color[]{ red,green,blue,alpha };
		context->ClearRenderTargetView(renderTarget, color);
		//context->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void DrawShit() {
		static bool failed = false;
		Vertex* vertices = new Vertex[MAX_CIRCLES*6]; //uhhh stop allocating on stack?
		for (int i = 0; i < MAX_CIRCLES*6; i += 6) {
			//https://www.desmos.com/calculator/jbu1yh3o70 (i be using desmos alot i need to get sponsored)
			float x = (static_cast<float>(cb2data.circles[i/6].left) / (width / 2.0f)) - 1.0f;//I_RpcGetBufferWithObject;
			float y = 1.0f - (static_cast<float>(cb2data.circles[i/6].top) / (height / 2.0f));
			
			//https://www.google.com/search?q=directx11+vertex+coordinates&tbm=isch&ved=2ahUKEwiooJP01eeCAxWQxMkDHcPbCroQ2-cCegQIABAA&oq=directx11+vertex+coordinates&gs_lcp=CgNpbWcQAzoECCMQJ1ClHFiNJ2C9J2gAcAB4AYAB1gGIAbwIkgEGMTEuMS4xmAEAoAEBqgELZ3dzLXdpei1pbWfAAQE&sclient=img&ei=AmBmZaiXOZCJp84Pw7er0As&bih=923&biw=1920#imgrc=9q46qNfqipyQcM&imgdii=PhWCUuXQ-W-h-M
			vertices[i] = { x,y,cb2data.colors, cb2data.circles[i / 6].top / 4.0f / 255.0f , cb2data.circles[i / 6].left / 6.0f / 255.0f };
			vertices[i + 1] = { x + (60.0f / width),y,cb2data.colors, cb2data.circles[i / 6].top / 4.0f / 255.0f , cb2data.circles[i / 6].left / 6.0f / 255.0f };
			vertices[i + 2] = { x + (60.0f / width),y - (60.0f / height),cb2data.colors, cb2data.circles[i / 6].top / 4.0f / 255.0f , cb2data.circles[i / 6].left / 6.0f / 255.0f };
			
			vertices[i + 3] = { x + (60.0f / width),y - (60.0f / height),cb2data.colors, cb2data.circles[i / 6].top / 4.0f / 255.0f , cb2data.circles[i / 6].left / 6.0f / 255.0f };
			vertices[i + 4] = { x,y - (60.0f / height),cb2data.colors, cb2data.circles[i / 6].top / 4.0f / 255.0f , cb2data.circles[i / 6].left / 6.0f / 255.0f };
			vertices[i+5] = {x,y,cb2data.colors, cb2data.circles[i / 6].top / 4.0f / 255.0f , cb2data.circles[i / 6].left / 6.0f / 255.0f };
		}
		//int j = 0;
		//for (int i = 0; i < 6000; i++) {
		//	float x = (static_cast<float>(cb2data.circles[j].left) / (width/2.0f))-1;//I_RpcGetBufferWithObject;
		//	float y = 1-(static_cast<float>(cb2data.circles[j].top) / (height/2.0f));
		//	vertices[i] = {x,y};
		//	if (i % 6 == 0) {
		//		j++;
		//	}
		//}
		//vertices[0].g = 1.0;
		ID3D11Buffer* vertexBuffer;
		D3D11_BUFFER_DESC bd{};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = (MAX_CIRCLES * 6)*sizeof(Vertex);//sizeof(vertices);
		bd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA srd{};
		srd.pSysMem = vertices;
		HRESULT shit = device->CreateBuffer(&bd, &srd, &vertexBuffer);
		if (FAILED(shit)) {
			GetRealError(shit, L"FAILED CREATING BUFFER");
			failed = true;
			return;
		}

		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		context->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset);

		ID3D11InputLayout* layout;
		D3D11_INPUT_ELEMENT_DESC ied[]{
			{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"Color",0,DXGI_FORMAT_R32G32B32_FLOAT,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		shit = device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &layout);

		context->IASetInputLayout(layout);

		//const unsigned short indices[]{
		//	0,2,1, 2,3,1,
		//	1,3,5, 3,7,5,
		//	2,6,3, 3,6,7,
		//	4,5,7, 4,7,6,
		//	0,4,2, 2,4,6,
		//	0,1,4, 1,5,4,
		//};
		//ID3D11Buffer* indexBuffer;
		//D3D11_BUFFER_DESC ibd = {};
		//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//ibd.Usage = D3D11_USAGE_DEFAULT;
		//ibd.CPUAccessFlags = 0u;
		//ibd.MiscFlags = 0u;
		//ibd.ByteWidth = sizeof(indices);
		//ibd.StructureByteStride = sizeof(unsigned short);
		//D3D11_SUBRESOURCE_DATA isd{};
		//isd.pSysMem = indices;
		//
		//shit = device->CreateBuffer(&ibd, &isd, &indexBuffer);
		//if (FAILED(shit)) {
		//	GetRealError(shit, L"FAILED CREATING INDICIES BUFFER");
		//	return;
		//}
		//
		//context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);






		//ID3D11Buffer* constantBuffer2;
		//D3D11_BUFFER_DESC cbd2 = {};
		//cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//cbd2.Usage = D3D11_USAGE_DEFAULT;
		//cbd2.CPUAccessFlags = 0u;
		//cbd2.MiscFlags = 0u;
		//cbd2.ByteWidth = sizeof(cb2data);
		//cbd2.StructureByteStride = 0u;
		//D3D11_SUBRESOURCE_DATA csd2{};
		//csd2.pSysMem = &cb2data;
		//
		//shit = device->CreateBuffer(&cbd2, &csd2, &constantBuffer2);
		//if (FAILED(shit)) {
		//	GetRealError(shit, L"FAILED CREATING CONSTANT BUFFER 2");
		//	return;
		//}
		//
		//context->PSSetConstantBuffers(0u, 1u, &constantBuffer2);

		//ID3DBlob* blob;
		
		context->Draw(MAX_CIRCLES * 6, 0u); //haha i actually wanted to know if the compiler would calculate this at compile time and according to (https://godbolt.org/) the assembly says yes!
		//context->Draw((UINT)std::size(vertices), 0u);
		//context->Draw(4u, 0u);//(UINT)std::size(vertices), 0u);
		//context->DrawIndexed((UINT)std::size(indices), 0u, 0u);

		layout->Release();
		vertexBuffer->Release();
		delete[] vertices;
		//indexBuffer->Release();
		//constantBuffer->Release();
		//constantBuffer2->Release();
		//vertexShader->Release();
		//pixelShader->Release();
		//blob->Release();
	}
};