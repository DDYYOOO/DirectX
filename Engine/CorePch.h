#pragma once
#include <Windows.h> 

//Helper 추가
#include "d3dx12.h"
#include <d3d12.h>


#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>


using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

//Helper Library 추가
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")
