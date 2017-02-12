#include "stdafx.h"
#include "RenderSystemDX11.h"
#include "DXGIAdapter.h"
#include "Log.h"

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "DXGI.lib" )

using Microsoft::WRL::ComPtr;

namespace TEE
{
	RenderSystemDX11* RenderSystemDX11::m_spRenderer = 0;

	RenderSystemDX11::RenderSystemDX11()
	{
		if (m_spRenderer == 0)
			m_spRenderer = this;

		//	m_pDevice = 0;
		//	m_pDebugger = 0;
		m_driverType = D3D_DRIVER_TYPE_NULL;

		m_FeatureLevel = D3D_FEATURE_LEVEL_9_1; // Initialize this to only support 9.1...
	}

	RenderSystemDX11::~RenderSystemDX11()
	{

	}

	RenderSystemDX11* RenderSystemDX11::Get()
	{
		return(m_spRenderer);
	}

	bool RenderSystemDX11::Initialize(D3D_DRIVER_TYPE DriverType, D3D_FEATURE_LEVEL FeatureLevel)
	{

		HRESULT hr = S_OK;

		// Create a factory to enumerate all of the hardware in the system.

		ComPtr<IDXGIFactory1> pFactory;
		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));


		// Enumerate all of the adapters in the current system.  This includes all
		// adapters, even the ones that don't support the ID3D11Device interface.

		ComPtr<IDXGIAdapter1> pCurrentAdapter;
		std::vector<DXGIAdapter> vAdapters;

		while (pFactory->EnumAdapters1(static_cast<UINT>(vAdapters.size()), pCurrentAdapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			vAdapters.push_back(pCurrentAdapter);

			DXGI_ADAPTER_DESC1 desc;
			pCurrentAdapter->GetDesc1(&desc);

			Log::Get().Write(desc.Description);
		}

		// Specify debug
		UINT CreateDeviceFlags = 0;
#ifdef _DEBUG
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		DeviceContextComPtr pContext;

		D3D_FEATURE_LEVEL level[] = { FeatureLevel };
		D3D_FEATURE_LEVEL CreatedLevel;

		//遍历所有的adapters知道创建成功。这对多adapter系统有用，内建的GPU可能不能适配最新的特性levels。
		// If it isn't a hardware device, then we simply use nullptr for the adapter with
		// the appropriate driver type.  See the following page for more details on device 
		// creation: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476082%28v=vs.85%29.aspx

		if (DriverType == D3D_DRIVER_TYPE_HARDWARE)
		{
			for (auto pAdapter : vAdapters)
			{
				hr = D3D11CreateDevice(
					pAdapter.m_pAdapter.Get(),
					D3D_DRIVER_TYPE_UNKNOWN,
					nullptr,
					CreateDeviceFlags,
					level,
					1,
					D3D11_SDK_VERSION,
					m_pDevice.GetAddressOf(),
					&CreatedLevel,
					pContext.GetAddressOf());

				if (hr == S_OK)
					break;
			}
		}
		else
		{
			hr = D3D11CreateDevice(
				nullptr,
				DriverType,
				nullptr,
				CreateDeviceFlags,
				level,
				1,
				D3D11_SDK_VERSION,
				m_pDevice.GetAddressOf(),
				&CreatedLevel,
				pContext.GetAddressOf());
		}

		if (FAILED(hr))
			return false;

		// Get the debugger interface from the device.

		hr = m_pDevice.CopyTo(m_pDebugger.GetAddressOf());

		if (FAILED(hr))
		{
			Log::Get().Write(L"Unable to acquire the ID3D11Debug interface from the device!");
		}

		// Grab a copy of the feature level for use by the rest of the rendering system.

		m_FeatureLevel = m_pDevice->GetFeatureLevel();

		// Create a query object to be used to gather statistics on the pipeline.

		D3D11_QUERY_DESC queryDesc;
		queryDesc.Query = D3D11_QUERY_PIPELINE_STATISTICS;
		queryDesc.MiscFlags = 0;

		D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS Options;
		m_pDevice->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &Options, sizeof(Options));
		if (Options.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x)
			Log::Get().Write(L"Device supports compute shaders plus raw and structured buffers via shader 4.x");


		D3D11_FEATURE_DATA_THREADING ThreadingOptions;
		m_pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING, &ThreadingOptions, sizeof(ThreadingOptions));

		// TODO: Enumerate all of the formats and quality levels available for the given format.
		//       It may be beneficial to allow this query from the user instead of enumerating
		//       all possible formats...
		UINT NumQuality;
		HRESULT hr1 = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &NumQuality);


		return(true);
	}
}

