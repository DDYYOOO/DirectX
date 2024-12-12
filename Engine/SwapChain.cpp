#include "pch.h"
#include "SwapChain.h"

//SwapChain 초기화
void SwapChain::Init(HWND hwnd, int width, int height, bool windowed, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	//스왑 체인 생성
	CreateSwapChain(hwnd, width, height, windowed, dxgi, cmdQueue);
	//렌더 타겟 뷰(RTV) 생성
	CreatRTV(device);
}

//현재 백 버퍼의 내용을 화면에 출력하는 함수
void SwapChain::Present()
{
	//Vsync를 사용 하지 않고 화면 출력
	swapChain->Present(0,0);
}

void SwapChain::SwapIndex()
{
	//백 버퍼 인덱스를 순환하여 해당 버퍼 인덱스를 교환
	backBufferIndex = (backBufferIndex + 1) % 2;
}


void SwapChain::CreateSwapChain(HWND hwnd, int width, int height, bool windowed, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	//기존 SwapChain 리셋
	swapChain.Reset();

	//스왑 체인 설명 구조체 초기화
	DXGI_SWAP_CHAIN_DESC sd;

	//버퍼 폭 설정
	sd.BufferDesc.Width = static_cast<unsigned int>(width);
	//버퍼의 높이 설정
	sd.BufferDesc.Height = static_cast<unsigned int>(height);

	//화면 재생률(분자)
	sd.BufferDesc.RefreshRate.Numerator = 60;
	//화면 재생률(분모)
	sd.BufferDesc.RefreshRate.Denominator = 1;


	//버퍼 포멧 설정
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//스캔라인 순서
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	 //스케일 모드 설정
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//멀티 샘플링 수 설정(1이면 비활성화)
	sd.SampleDesc.Count = 1;
	//멀티 샘플링 설정
	sd.SampleDesc.Quality = 0;

	//버퍼 사용 방법 설정
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//버퍼 수 설정(더블 버퍼)
	sd.BufferCount = 2;

	//출력 창 핸드 설정
	sd.OutputWindow = hwnd;

	//창모드 설정
	sd.Windowed = windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 스왑 효과 설정
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 모드 전환 허용 설정


	//스왑체인 생성
	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &swapChain);

	//각 버퍼를 tryBuffer 배열에 저장
	for (int i = 0; i < 2; i++)
	{
		swapChain->GetBuffer(i, IID_PPV_ARGS(&rtvBuffer[i]));
	}

}

//렌더 타겟 뷰(RTV)를 생성 하는 함수
void SwapChain::CreatRTV(ComPtr<ID3D12Device> device)
{
	//RTV 디스크립터 힙의 크기 계산
	int rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//RTV 디스크립터 구조체 초기화
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;

	//디스크립터 힙 타입 설정
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	//디스크립터 수 설정(더블 버퍼링)
	rtvDesc.NumDescriptors = 2;
	//디스크립터 힙 플레그 설정
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//노드 마스크 설정(단일 어뎁터의 경우 0)
	rtvDesc.NodeMask = 0;

	//RTV 디스크립터 힙 생성
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&rtvHeap));

	//RTV 디스크립터 힙의 시작 핸들 가져오기
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = rtvHeap->GetCPUDescriptorHandleForHeapStart();

	//각 버퍼에 대한 RTV 생성
	for (int i = 0; i < 2; i++)
	{
		//RTV 핸들 설정
		rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i + rtvHeapSize);

		//RTV 생성
		device->CreateRenderTargetView(rtvBuffer[i].Get(), nullptr, rtvHandle[i]);
	}

}
