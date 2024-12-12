#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h" // 호출

CommandQueue::~CommandQueue()
{
	//팬스 이벤트 핸들을 닫아서 리소스를 해제
	CloseHandle(fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	//SwapChain을 연결
	this->swapChain = swapChain;

	//커맨드 큐 description 구조체 초기화
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // 커맨드 리스트 타입 설정
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // 플래그 설정

	//커맨드 큐 생성
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));

	//커맨드 allocator 생성
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));

	//커맨드 리스트 생성
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	//커맨드 리스트를 닫음. 재설정을 위해
	cmdList->Close();

	//팬스 객체 생성
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	//팬스 이벤트 생성
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

//CPU와 GPU간의 동기화를 대기하는 함수
void CommandQueue::WaitSync()
{
	//팬스 값 증가
	fenceValue++;

	//GPU에 팬스 신호 보내기
	cmdQueue->Signal(fence.Get(), fenceValue);
											   
	//팬스 값이 완료됮 않으면 이벤트 대기
	if (fence->GetCompletedValue() < fenceValue)
	{
		//팬스 값이 완료되었을 때 이벤트 설정
		fence->SetEventOnCompletion(fenceValue, fenceEvent);

		//이벤트 대기
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

//렌더링 시작하기 위한 설정 함수
void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	//커맨드 alloc를 재설정
	cmdAlloc->Reset();

	//커맨드 리스트 재설정
	cmdList->Reset(cmdAlloc.Get(), nullptr);

	//리소스 베리어를 설정하여 버퍼 상태를 Present에서 Render Target으로 전환
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,	  //from
		D3D12_RESOURCE_STATE_RENDER_TARGET);//to

	//리소스 베리어 설정
	cmdList->ResourceBarrier(1, &barrier);

	//뷰포트 설정
	cmdList->RSSetViewports(1, vp);

	//시저(Rect) 설정
	cmdList->RSSetScissorRects(1, rect);

	//백 버퍼의 렌더 타겟 뷰를 가져옴
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = swapChain->GetBackRTV();

	//렌더 타겟 뷰를 클리어
	cmdList->ClearRenderTargetView(backBufferView, Colors::Aqua, 0, nullptr);

	//렌더 타겟 설정
	cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);

}

void CommandQueue::RenderEnd()
{

	//리소스 베리어를 설정하여 버퍼 상태를 Render Target에서 Present으로 전환
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,	//from
		D3D12_RESOURCE_STATE_PRESENT); //to

	//리소스 베리어 설정
	cmdList->ResourceBarrier(1, &barrier);

	//커맨드 리스트를 닫음
	cmdList->Close();

	//커맨드 리스트로 커맨드 리스트 배열 생성 
	ID3D12CommandList* cmdListArr[] = { cmdList.Get() };

	//커맨드 리스트를 실행
	cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	//스왑 체인을 통해 화면에 출력
	swapChain->Present();

	//동기화 대기
	WaitSync();

	//백 버퍼 인덱스 교환
	swapChain->SwapIndex();
	
}
