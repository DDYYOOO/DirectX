#pragma once
class SwapChain
{
private:
	//SwapChain	객체
	ComPtr<IDXGISwapChain> swapChain;

	//RTV(Render Taget View)
	//렌더 타겟 버퍼 배열(더블 버퍼링을 하기 위해 2개 사용)
	ComPtr<ID3D12Resource> rtvBuffer[2];
	//RTV를 위한 디스크립터 힙
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	//Render target view 디스크립터 핸들 배열
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[2];

	//현재 사용 중인 백 버퍼의 인덱스
	int backBufferIndex = 0;

public: // Get 함수들 정리
	//SwapChain 객체를 반환
	ComPtr<IDXGISwapChain> GetSwapChain() { return swapChain; }
	//주어진 인덱스에 해당하는 렌더 타켓 버퍼 반환
	ComPtr<ID3D12Resource> GetRednerTarget(int index) { return rtvBuffer[index]; }
	//현재 백 버퍼에 해당하는 렌더 타겟 버퍼를 반환
	ComPtr<ID3D12Resource> GetBackRTVBuffer() const { return rtvBuffer[backBufferIndex]; }
	//현재 백 버퍼의 렌더 타겟 뷰(RTV) 디스크립터 핸들을 반환
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() const { return rtvHandle[backBufferIndex]; }

public:
	//초기화 함수, 필요한 정보와 객체를 받아 스왑체인과 관련 리소스를 생성
	void Init(HWND hwnd, int width, int height, bool windowed, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);

	//화면에 렌더링된 내용을 표시, 백 버퍼와 프론트 버퍼를 교환
	void Present();
	//백 버퍼 인덱스를 스왑, 렌더링할 다음 버퍼를 선택
	void SwapIndex();
private:
	//스왑체인을 생성하는 내부 함수
	void CreateSwapChain(HWND hwnd, int width, int height, bool windowed, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	//렌더 타겟 뷰(RTV)를 생성하는 내부 함수
	void CreatRTV(ComPtr<ID3D12Device> device);
};

