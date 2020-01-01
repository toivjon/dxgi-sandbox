#include <dxgi.h>
#include <iostream>
#include <wrl/client.h> // ComPtr
#include <comdef.h>		// _com_error

#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

// a utility to easily catch failed HRESULTS.
inline void check_hresult(HRESULT result) {
	if (FAILED(0)) {
		throw _com_error(result);
	}
}

int main() {
	ComPtr<IDXGIFactory> dxgiFactory;
	check_hresult(CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory)));
	return 0;
}