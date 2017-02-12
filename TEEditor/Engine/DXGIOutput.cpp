#include "stdafx.h"
#include "DXGIOutput.h"

namespace TEE
{
	DXGIOutput::DXGIOutput(Microsoft::WRL::ComPtr<IDXGIOutput> pOutput)
	{
		m_pOutput = pOutput;
	}
	//--------------------------------------------------------------------------------
	DXGIOutput::~DXGIOutput()
	{
	}
	//--------------------------------------------------------------------------------
}

