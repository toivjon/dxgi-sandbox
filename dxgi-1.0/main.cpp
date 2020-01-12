#include <wrl/client.h> // ComPtr
#include <comdef.h>		// _com_error
#include <string>
#include <vector>

#include "window.h"

#include <dxgi.h>
#include <d3d10.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d10.lib")

#define CHECK_BIT(x, bit) (x & bit) != 0

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

// a utility to convert DXGI_MODE_ROTATION into a descriptive string.
inline const char* rotationString(DXGI_MODE_ROTATION rotation) {
	switch (rotation) {
	case DXGI_MODE_ROTATION_IDENTITY:
		return "identity";
	case DXGI_MODE_ROTATION_ROTATE180:
		return "rotate-180";
	case DXGI_MODE_ROTATION_ROTATE270:
		return "rotate-270";
	case DXGI_MODE_ROTATION_ROTATE90:
		return "rotate-90";
	case DXGI_MODE_ROTATION_UNSPECIFIED:
		return "unspecified";
	default:
		return "unknown";
	}
}

// a utility to convert RECT into a descriptive string.
inline std::string rectString(RECT& rect) {
	std::string str = "{";
	str += "top: " + std::to_string(rect.top);
	str += ", right: " + std::to_string(rect.right);
	str += ", bottom: " + std::to_string(rect.bottom);
	str += ", left: " + std::to_string(rect.left);
	str += "}";
	return str;
}

// a utility to convert DWORD into a boolean string.
inline const char* boolString(DWORD value) {
	return value == 0 ? "false" : "true";
}

// a utility to convert BOOL into a boolean string.
inline const char* boolString(BOOL value) {
	return value == 0 ? "false" : "true";
}

// a utility to get string presentation of DXGI_MODE_SCALING.
inline const char* scalingString(DXGI_MODE_SCALING mode) {
	switch (mode) {
	case DXGI_MODE_SCALING_CENTERED:
		return "centered";
	case DXGI_MODE_SCALING_STRETCHED:
		return "stretched";
	case DXGI_MODE_SCALING_UNSPECIFIED:
		return "unspecified";
	default:
		return "unknown";
	}
}

// a utility to get string presentation of DXGI_MODE_SCANLINE_ORDER.
inline const char* scanlineOrderingString(DXGI_MODE_SCANLINE_ORDER mode) {
	switch (mode) {
	case DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST:
		return "lower-field-first";
	case DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE:
		return "progressive";
	case DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED:
		return "unspecified";
	case DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST:
		return "upper-field-first";
	default:
		return "unknown";
	}
}

// a utility to get string presentation of DXGI_RESIDENCY.
inline const char* residencyString(DXGI_RESIDENCY residency) {
	switch (residency) {
	case DXGI_RESIDENCY_EVICTED_TO_DISK:
		return "evicted-to-disk";
	case DXGI_RESIDENCY_FULLY_RESIDENT:
		return "fully-resident";
	case DXGI_RESIDENCY_RESIDENT_IN_SHARED_MEMORY:
		return "in-shared-memory";
	default:
		return "unknown";
	}
}

// a utility to get string presentation of DXGI_USAGE.
inline std::string usageString(DXGI_USAGE usage) {
	std::string result;
	if (CHECK_BIT(usage, DXGI_USAGE_BACK_BUFFER)) {
		result += "[back-buffer]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_DISCARD_ON_PRESENT)) {
		result += "[discard-on-present]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_READ_ONLY)) {
		result += "[read-only]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_RENDER_TARGET_OUTPUT)) {
		result += "[render-target-output]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_SHADER_INPUT)) {
		result += "[shared-input]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_SHARED)) {
		result += "[shared]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_UNORDERED_ACCESS)) {
		result += "[unordered-access]";
	}
	return result;
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

// ============================================================================
// IDXGIOutput
//
//   - GetDesc						-- Get information about the output
//   - GetFrameStatistics			-- Get information about rendered frames
//	 - GetGammaControlCapabilities	-- Get information about gamma controls
//   - ReleaseOwnership				-- [WARNING] Release the target output
//   - TakeOwnership				-- [WARNING] Captures the target output
//	 - GetGammaControl				-- Get the definitions for gamma
//	 - SetGammaControl				-- Set the definitions for gamma
//	 - GetDisplaySurface			-- Get the display surface
//	 - SetDisplaySurface			-- [WARNING] Set the display surface
//	 - WaitForVBlank				-- Wait for the next vertical blank
//	 - FindClosestMatchingMode		-- Find closest mode for desired mode
//	 - GetDisplayModeList			-- Find the list of modes
//
// Note that some additional information can be gathered by querying the output
// information with GetMonitorInfo with the DXGI_OUTPUT_DESC.HMONITOR handle.
// Most of the information is however already present in the DXGI_OUTPUT_DESC.
//
// Note that the TakeOwnership and RelaseOwnership are not typically used with
// an application that uses a swap chain to present rendering. DXGI knows how
// to automatically perform capture and release when swap chains are used. If
// still called manually, the application may have unpredictable behavior.
//
// [WARNING] Following methods can be only used when ouput is in fullscreen.
//
//		GetGammaControlCapabilities
//		GetGammaControl
//		SetGammaControl
//		GetDisplaySurface
//		SetDisplaySurface
//
// Note that SetDisplaySurface is not manually used with an application which
// uses swap chain for presenting. DXGI knows how to automatically use them.
// ============================================================================
void testOutput(ComPtr<IDXGIOutput> output) {
	// get and print information about the output.
	DXGI_OUTPUT_DESC desc;
	check_hresult(output->GetDesc(&desc));
	printf("==============================================================\n");
	printf("name:          %ls\n", desc.DeviceName);
	printf("hasDesktop:    %s\n", boolString(desc.AttachedToDesktop));
	printf("rotation:      %s\n", rotationString(desc.Rotation));
	printf("desktopCoords: %s\n", rectString(desc.DesktopCoordinates).c_str());

	// get additional details from the HMONITOR pointer.
	MONITORINFOEX info = {};
	info.cbSize = sizeof(MONITORINFOEX);
	if (GetMonitorInfo(desc.Monitor, &info) != 0) {
		printf("device:        %ls\n", info.szDevice);
		printf("monitorCoords: %s\n", rectString(info.rcMonitor).c_str());
		printf("workCoords:    %s\n", rectString(info.rcWork).c_str());
		printf("isPrimary:     %s\n", boolString(info.dwFlags & MONITORINFOF_PRIMARY));
	}

	// wait until the output makes next vertical blank call.
	check_hresult(output->WaitForVBlank());

	// enumerate the available display modes for the target format.
	UINT modeCount = 0;
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	check_hresult(output->GetDisplayModeList(format, 0, &modeCount, nullptr));
	std::vector<DXGI_MODE_DESC> modes(modeCount);
	check_hresult(output->GetDisplayModeList(format, 0, &modeCount, &modes[0]));
	printf("display modes for format R8G8B8A8_UNORM:\n");
	for (auto mode : modes) {
		printf("  %dx%d\t\t%d/%d\tscaling: %s\t\tscanline-ordering: %s\n",
			mode.Width, mode.Height,
			mode.RefreshRate.Numerator, mode.RefreshRate.Denominator,
			scalingString(mode.Scaling),
			scanlineOrderingString(mode.ScanlineOrdering)
			);
	}

	// a utility to find the closest matching display mode for a desired mode.
	DXGI_MODE_DESC desiredMode;
	DXGI_MODE_DESC closestMode;
	desiredMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desiredMode.Width = 800;
	desiredMode.Height = 600;
	desiredMode.Scaling = DXGI_MODE_SCALING_CENTERED;
	desiredMode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	check_hresult(output->FindClosestMatchingMode(&desiredMode, &closestMode, nullptr));
	printf("found the following closest matching mode for R8G8B8A8 UNORM 800 x 600:\n");
	printf("  %dx%d\t\t%d/%d\tscaling: %s\t\tscanline-ordering: %s\n",
		closestMode.Width, closestMode.Height,
		closestMode.RefreshRate.Numerator, closestMode.RefreshRate.Denominator,
		scalingString(closestMode.Scaling),
		scanlineOrderingString(closestMode.ScanlineOrdering)
	);

	// get the gamma control settings (only when fullscreen).
	/* these can be only managed when output is in fullscreen mode
	DXGI_GAMMA_CONTROL gammaControl;
	check_hresult(output->GetGammaControl(&gammaControl));
	check_hresult(output->SetGammaControl(&gammaControl));
	*/

	// get information about recently rendered frames.
	/* check how this works.... now it just returns error
	DXGI_FRAME_STATISTICS stats;
	check_hresult(output->GetFrameStatistics(&stats));
	printf("stats.presentCount:        %d\n", stats.PresentCount);
	printf("stats.presentRefreshCount: %d\n", stats.PresentRefreshCount);
	printf("stats.syncGPUTime:		   %lld\n", stats.SyncGPUTime.QuadPart);
	printf("stats.syncQPCTime:		   %lld\n", stats.SyncQPCTime.QuadPart);
	printf("stats.syncRefreshCount:    %d\n", stats.SyncRefreshCount);
	*/

	// get details about the output's gamma control capabilities (only when fullscreen).
	/* these can be only queried when output is in fullscreen mode
	DXGI_GAMMA_CONTROL_CAPABILITIES caps;
	check_hresult(output->GetGammaControlCapabilities(&caps));
	printf("gammaCaps.maxConvertedValue:     %0.2f\n", caps.MaxConvertedValue);
	printf("gammaCaps.minConvertedValue:     %0.2f\n", caps.MinConvertedValue);
	printf("gammaCaps.numGammaControlPoints: %d\n", caps.NumGammaControlPoints);
	printf("gammaCaps.scaleAndOffsetSupport: %s\n", boolString(caps.ScaleAndOffsetSupported));
	*/

	// these can be only used when output is in fullscreen mode
	// output->GetDisplaySurfaceData();
	// output->SetDisplaySurface

	// [WARNING] not testing the following methods as they have unpredictable results.
	// output->TakeOwnership()
	// output->ReleaseOwnership()
}

// ============================================================================
// IDXGIAdapter
//
//   - EnumOutputs			 -- Enumerate attached outputs (e.g. monitors)
//   - GetDesc				 -- Get information about the adapter
//   - CheckInterfaceSupport -- Check if adapter supports target D3D 10 device
//
// Note that CheckInterfaceSupport only works when checking againts Direct3D 10
// device interfaces (e.g. D3D10Device). If used with Direct3D 11 or later, this
// function will return DXGI_ERROR_UNSUPPORTED (see the documentation remarks).
// ============================================================================
void testAdapter(ComPtr<IDXGIAdapter> adapter) {
	// get and print information about the adapter.
	DXGI_ADAPTER_DESC desc;
	check_hresult(adapter->GetDesc(&desc));
	printf("==============================================================\n");
	printf("description:   %ls\n", desc.Description);
	printf("vendor-id:     %d\n", desc.VendorId);
	printf("device-id:     %d\n", desc.DeviceId);
	printf("sub-sys-id:    %d\n", desc.SubSysId);
	printf("revision:      %d\n", desc.Revision);
	printf("video-memory:  %u\n", desc.DedicatedVideoMemory);
	printf("system-memory: %u\n", desc.DedicatedSystemMemory);
	printf("shared-memory: %u\n", desc.SharedSystemMemory);
	printf("luid:          %d:%d\n", desc.AdapterLuid.HighPart, desc.AdapterLuid.HighPart);

	// check whether the adapter supports Direct3D 10 and get the driver version.
	LARGE_INTEGER version;
	check_hresult(adapter->CheckInterfaceSupport(__uuidof(ID3D10Device), &version));
	printf("D3D-10 driver: %d.%d\n", version.HighPart, version.LowPart);

	// iterate over the enumerated outputs.
	ComPtr<IDXGIOutput> output;
	for (auto i = 0u; adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND; i++) {
		testOutput(output);
	}
}

// ============================================================================
// IDXGIDevice
//
//	 - GetAdapter				-- Get the wrapped adapter
//	 - CreateSurface			-- [WARNING] create surface
//	 - QueryResourceResidency	-- Get info how target resources reside
//	 - SetGPUThreadPriority		-- Set the GPU thread priority
//	 - GetGPUThreadPriority		-- Get the GPU thread priority
//
// [WARNING] CreateSurface function should not be used manually. DXGI will know
// how to use it automatically whenever there is a possible need to use it.
//
// GPU priorities vary from -7 to 7, where 0 presents the normal priority. Note
// that this function should be only used when application is fully profiled to
// ensure that it behaves as intended. Otherwise, poor render speed can occur.
//
// QueryResourceResidency can be used to determine how and where the target
// resources are being currently located. These IDXGIResources can be found
// by querying the D3D generated resource with the QueryInterface function.
// ============================================================================
void testDevice(ComPtr<IDXGIDevice> device, ComPtr<IDXGIResource> resource) {
	// get a reference to the wrapped DXGI adapter interface.
	ComPtr<IDXGIAdapter> adapter;
	check_hresult(device->GetAdapter(&adapter));
	printf("device-adapter found: %s\n", (adapter ? "yes" : "no"));

	// get and set the GPU thread priority.
	auto gpuPriority = 0;
	check_hresult(device->GetGPUThreadPriority(&gpuPriority));
	printf("device-gpu-priority:  %d\n", gpuPriority);
	check_hresult(device->SetGPUThreadPriority(gpuPriority));

	// get the residence status of the target resources.
	IUnknown* resources = { resource.Get() };
	DXGI_RESIDENCY residencies[1];
	check_hresult(device->QueryResourceResidency(&resources, residencies, 1));
	printf("resource-residency: %s\n", residencyString(residencies[0]));
}

// ============================================================================
// IDXGIResource
//
//	 - GetSharedHandle		-- Get the handle to shared resource
//	 - GetUsage				-- Get the expected resource usage
//	 - SetEvictionPolicy	-- Set the memory eviction policy
//	 - GetEvictionPolicy	-- Get the current memory eviction policy
//
// Starting from Direct3D 11.1 GetSharedHandle should not be used anymore. The
// IDXGIResource1::CreatedSharedHandle should be used to get new shared handle.
//
// Note that eviction value can vary between DXGI_RRESOURCE_PRIORITY_MINIMUM to
// DXGI_RESOURCE_EVICTION_PRIORITY_MAXIMUM. There are some existing enumeration
// values defined, but values other than enumerations are used when approriate.
// ============================================================================
void testResource(ComPtr<IDXGIResource> resource) {
	// get the handle to shared resource.
	HANDLE handle;
	check_hresult(resource->GetSharedHandle(&handle));
	printf("hasSharedHandle:  %s\n", (handle ? "yes": "no"));

	// get the expected resource usage.
	DXGI_USAGE usage;
	check_hresult(resource->GetUsage(&usage));
	printf("usage:            %s\n", usageString(usage).c_str());

	// get and set the memory eviction priority.
	UINT evictionPriority;
	check_hresult(resource->GetEvictionPriority(&evictionPriority));
	printf("evictionPriority: %d\n", evictionPriority);
	check_hresult(resource->SetEvictionPriority(evictionPriority));
}

void enumerateAdaptersAndOutputs() {
	// note that adapters are actually enumerated when the factory is created.
	ComPtr<IDXGIFactory> factory;
	check_hresult(CreateDXGIFactory(IID_PPV_ARGS(&factory)));

	// iterate over the enumerated adapters.
	ComPtr<IDXGIAdapter> adapter;
	for (auto i = 0u; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++) {
		testAdapter(adapter);
	}
}


int main() {
	enumerateAdaptersAndOutputs();

	/*
	Window window(WINDOW_WIDTH, WINDOW_HEIGHT);
	auto factory = createFactory();
	ComPtr<IDXGIAdapter> adapter;
	check_hresult(factory->EnumAdapters(0, &adapter));
	*/

	// Hmm... we actually seem to need D3D device.
	ComPtr<ID3D10Device> d3dDevice;
	check_hresult(D3D10CreateDevice(
		0,
		D3D10_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		D3D10_SDK_VERSION,
		&d3dDevice)
	);

	// Hmm... we actually also need a resource for our tests.
	D3D10_TEXTURE2D_DESC desc = {};
	desc.Width = WINDOW_WIDTH;
	desc.Height = WINDOW_HEIGHT;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	ComPtr<ID3D10Texture2D> texture;
	check_hresult(d3dDevice->CreateTexture2D(&desc, NULL, &texture));
	ComPtr<IDXGIResource> resource;
	check_hresult(texture->QueryInterface(IID_PPV_ARGS(&resource)));
	ComPtr<IDXGIDevice> device;

	check_hresult(d3dDevice->QueryInterface(IID_PPV_ARGS(&device)));
	testDevice(device, resource);
	testResource(resource);

	/*
	// testDXGIObject(adapter);
	auto swapchain = testDXGIFactory(window, d3dDevice);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		check_hresult(swapchain->Present(0, 0));
	}
	*/
	return 0;
}