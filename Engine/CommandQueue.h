#pragma once
class SwapChain;

class CommandQueue
{
private:
	//Direct3D 12 커맨드 큐
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//Direct3D 12 커맨드 allocator
	ComPtr<ID3D12CommandAllocator> cmdAlloc;
	//Direct3D 12 그래픽 커맨드 리스트
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//동기화 객체인 팬스
	ComPtr<ID3D12Fence> fence;

	//fence 값, CPU와 GPU간의 동기화를 위해 사용
	int fenceValue = 0;

	//fence 이벤트 핸들
	HANDLE fenceEvent = INVALID_HANDLE_VALUE;

	//SwapChain 객체에 대한 스마트 포인터
	shared_ptr<SwapChain> swapChain;

public: //Get 함수들
	//커맨드 큐 객체를 반환하는 함수
	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return cmdQueue; }
	//그래픽 커맨드 리스트 객체를 반환하는 함수
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return cmdList; }

public:
	//소멸자
	~CommandQueue();

	//CommandQueue 초기화 함수
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);

	//동기화를 대기하는 함수
	void WaitSync();

	//랜더링 시작을 위한 설정 함수
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);

	//렌더링 끝내고 명령을 제출하는 함수
	void RenderEnd();

};

