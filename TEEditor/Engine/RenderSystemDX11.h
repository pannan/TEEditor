#pragma once

namespace TEE
{
	typedef Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContextComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11Query> QueryComPtr;

	typedef Microsoft::WRL::ComPtr<ID3D11Buffer> BufferComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11Texture1D> Texture1DComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2DComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11Texture3D> Texture3DComPtr;

	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceViewComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilViewComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetViewComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> UnorderedAccessViewComPtr;

	typedef Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStateComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilStateComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStateComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerStateComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayoutComPtr;

	class RenderSystemDX11
	{
	public:

		RenderSystemDX11();
		~RenderSystemDX11();


		static RenderSystemDX11* Get();


		D3D_FEATURE_LEVEL GetAvailableFeatureLevel(D3D_DRIVER_TYPE DriverType);
		D3D_FEATURE_LEVEL GetCurrentFeatureLevel();


		UINT64 GetAvailableVideoMemory();



		bool Initialize(D3D_DRIVER_TYPE DriverType, D3D_FEATURE_LEVEL FeatureLevel);
		void Shutdown();


		void Present(HWND hWnd = 0, int SwapChain = -1, UINT SyncInterval = 0, UINT PresentFlags = 0);

	protected:

		// The main API interfaces used in the renderer.
		Microsoft::WRL::ComPtr<ID3D11Device>		m_pDevice;
		Microsoft::WRL::ComPtr<ID3D11Debug>		m_pDebugger;
		D3D_DRIVER_TYPE										m_driverType;

		static RenderSystemDX11*							m_spRenderer;

		D3D_FEATURE_LEVEL									m_FeatureLevel;
	private:
	};
}