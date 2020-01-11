#include <wrl/client.h> // ComPtr
#include <comdef.h>		// _com_error
#include <string>

#include "window.h"

#include <dxgi.h>
#include <d3d10.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d10.lib")

using namespace Microsoft::WRL; // ComPtr

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// a utility to easily catch failed HRESULTS.
inline void check_hresult(HRESULT result) {
	if (FAILED(result)) {
		throw _com_error(result);
	}
}

// a utility to create a new GUID.
inline GUID createGUID() {
	GUID guid;
	check_hresult(CoCreateGuid(&guid));
	return guid;
}

// build a new DXGI factory instance.
ComPtr<IDXGIFactory> createFactory() {
	ComPtr<IDXGIFactory> factory;
	check_hresult(CreateDXGIFactory(IID_PPV_ARGS(&factory)));
	return factory;
}

// a utility to count COM object references.
UINT countRefs(ComPtr<IUnknown> object) {
	object->AddRef();
	return object->Release();
}

// ============================================================================
// IDXGIObject
//
// A base DXGI object type extended by all DXGI objects.
//
// Has following functions:
//   - SetPrivateData			-- Set custom data
//   - GetPrivateData			-- Get custom data or IUnknown-interface data
//   - SetPrivateDataInterface	-- Set IUnknown-interface data
//   - GetParent				-- Get reference to parent
//
// SetPrivateData can be also used to assign a special name for the object to
// help object debugging with WKPDID_D3DDebugObjectName (in D3Dcommon.h).
// GetPrivateData with GUID_DeviceType can be used to query the device type
// from the display adapter object. Do not use it to set device type manually!
//
// Note that IUnknown-interface queried with GetPrivateData should be freed
// manually with the Release function as COM reference get incremented when its
// assigned to the target IDXGIObject with the SetPrivateDataInterface and each
// time it gets queried with the GetPrivateData.
//
// Note that GetParent may fail if trying to query parent of an object which
// does not support them e.g. try what gets thrown with IDXGIFactory ;)
// ============================================================================
void testDXGIObject(ComPtr<IDXGIObject> object) {
	// assign a custom data into the target DXGI object.
	auto data = "foobar";
	auto guid = createGUID();
	check_hresult(object->SetPrivateData(guid, strlen(data), data));

	// retrieve a custom data from a DXGI object.
	auto size = 128u;
	char buffer[128];
	check_hresult(object->GetPrivateData(guid, &size, buffer));
	printf("data: %s\n", std::string(buffer, size).c_str());

	// assign a IUnknown-derived interface into the target DXGI object.
	auto object2 = createFactory();
	auto guid2 = createGUID();
	printf("object2 refs before attachment: %d\n", countRefs(object2));
	check_hresult(object->SetPrivateDataInterface(guid2, object2.Get()));
	printf("object2 refs after attachment: %d\n", countRefs(object2));

	// retrieve the interface we just put into the target DXGI object.
	IUnknown* item;
	auto itemSize = sizeof(IUnknown); 
	check_hresult(object->GetPrivateData(guid2, &itemSize, &item));
	printf("same interface: %d\n", (item == object2.Get()));
	printf("object2 refs after getting: %d\n", countRefs(object2));
	item->Release();

	// get a reference to the parent of the target DXGI object.
	ComPtr<IDXGIFactory> parent;
	check_hresult(object->GetParent(IID_PPV_ARGS(&parent)));
	printf("parent DXGIFactory refCount: %d\n", countRefs(parent));
}

// ============================================================================
// IDXGIFactory
//
// The main object to build different kinds of DXGI objects.
//
// Has following functions:
//	 - CreateSoftwareAdapter	-- Create a custom software DXGI adapter
//	 - CreateSwapChain			-- Create a swap chain
//	 - EnumAdapters				-- Enumerate display adapters
//	 - GetWindowAssociation		-- Return the associated window HWND
//	 - MakeWindowAssociation	-- Specify DXGI window association flags
//
// DXGI allows one to specify how DXGI is listening for the events from the
// specified window. Typical association allows user to use ALT+ENTER to toggle
// fullscreen window mode and PRINT SCREEN to capture screenshot. This default
// behavior can be changed by using the following flags in the association.
//
//	DXGI_MWA_NO_WINDOW_CHANGES	-- DXGI will not listen message queue at all.
//  DXGI_MWA_NO_ALT_ENTER		-- DXGI will not respond to ALT-ENTER.
//  DXGI_MWA_NO_PRINT_SCREEN	-- DXGI will not respond to PRINT SCREEN.
//
// Note that second call to MakeWindowAssociation makes DXGI to stop listening
// the previously associated window. NOTE that association parameters should be
// given to a window which is already attached to DXGI e.g. with swap chain.
//
// Note that GetWindowAssociation returns null even when theres an associaton.
//
// Note that for some reason window association has no effect in Windows 10.
// ============================================================================
ComPtr<IDXGISwapChain> testDXGIFactory(Window& window, ComPtr<ID3D10Device> d3dDevice) {
	ComPtr<IDXGIDevice> dd;
	check_hresult(d3dDevice->QueryInterface(IID_PPV_ARGS(&dd)));

	ComPtr<IDXGIAdapter> da;
	check_hresult(dd->GetParent(IID_PPV_ARGS(&da)));

	ComPtr<IDXGIFactory> factory;
	check_hresult(da->GetParent(IID_PPV_ARGS(&factory)));

	// enumerate the system's available display adapters.
	UINT index = 0;
	ComPtr<IDXGIAdapter> adapter;
	while (factory->EnumAdapters(index, &adapter) != DXGI_ERROR_NOT_FOUND) {
		// ... do something with the adapter ...
		index++;
	}

	// create a swap chain by associating our window to target device.
	ComPtr<IDXGISwapChain> swapChain;
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 2;
	desc.BufferDesc.Width = WINDOW_WIDTH;
	desc.BufferDesc.Height = WINDOW_HEIGHT;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = window.hwnd();
	desc.SampleDesc.Quality = 1;
	desc.SampleDesc.Count = 1;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;
	desc.Windowed = true;
	check_hresult(factory->CreateSwapChain(d3dDevice.Get(), &desc, &swapChain));

	// define how DXGI will monitor window message queue.
	// auto flags = DXGI_MWA_NO_ALT_ENTER;
	// check_hresult(factory->MakeWindowAssociation(window.hwnd(), flags));

	check_hresult(factory->MakeWindowAssociation(window.hwnd(), DXGI_MWA_NO_ALT_ENTER));

	// factory->GetWindowAssociation
	HWND hwnd;
	check_hresult(factory->GetWindowAssociation(&hwnd));
	printf("found hwnd: %s\n", (hwnd != nullptr ? "yes" : "no"));

	// ... factory->CreateSoftwareAdapter is being skipped.
	return swapChain;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Window window(WINDOW_WIDTH, WINDOW_HEIGHT);
	auto factory = createFactory();
	ComPtr<IDXGIAdapter> adapter;
	check_hresult(factory->EnumAdapters(0, &adapter));

	// Hmm... we actually seem to need D3D device.
	ComPtr<ID3D10Device> d3dDevice;
	check_hresult(D3D10CreateDevice(
		adapter.Get(),
		D3D10_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		D3D10_SDK_VERSION,
		&d3dDevice)
	);

	// testDXGIObject(adapter);
	auto swapchain = testDXGIFactory(window, d3dDevice);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		check_hresult(swapchain->Present(0, 0));
	}

	return 0;
}