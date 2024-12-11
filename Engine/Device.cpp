#include "pch.h"
#include "Device.h"

void Device::Init()
{
	//디버그 모드가 활성화 되어 있을 경우
#if _DEBUG
	//디버그 인터페이스를 요청하여 debugControoler에 저장
	D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
	//디버그 레이어를 활성화하여 개발 중 오류를 쉽게 찾을 수 있게 함
	debugController->EnableDebugLayer();
#endif

	//DXGI 팩토리 생성해서 dxgiFactory에 연결
	//DXGI 팩토리는 다양한 DXGI 객체를 생성하는데 사용
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	//DirectX 12 디바이스 생성, &device에 저장
	//이 디바이스는 Direct3D 기능을 사용하여 그래픽 리소스를 생성하고 관리하는데 필수 요소
	//nullptr : 기본 어뎁터 사용 하겠다는 의미
    //D3D_FEATURE_LEVEL_11_0 : 최소 지원해야 할 기능 레벨
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

}
