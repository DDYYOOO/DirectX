#pragma once
class Device
{
private:
	//디버그 컨트롤러, DirectX 12 디버깅 기능을 관리
	ComPtr<ID3D12Debug> debugController;
	//DXGI 팩토리, 화면 출력 및 그래픽 카드 관리기능 제공
	ComPtr<IDXGIFactory> dxgiFactory;
	//D3D12 디바이스, 그래픽 리소스 생성을 담당
	ComPtr<ID3D12Device> device;

public:
	//DXGI 팩토리 객체를 검색(당야한 DXGI 객체 생성에 사용)
	ComPtr<IDXGIFactory> GetDXGI() { return  dxgiFactory; }

	//D3D12 디바이스 객체를 검색(그래픽 리소스 생성에 사용) 
	ComPtr<ID3D12Device> GetDevice() { return device; }

public:
	//장치 및 관련 구성 요소 초기화
	void Init();
};

