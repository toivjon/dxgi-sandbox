#include <dxgi1_6.h>
#include <iostream>
#include <wrl/client.h> // ComPtr
#include <comdef.h>		// _com_error

#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL; // ComPtr

// a utility to easily catch failed HRESULTS.
inline void check_hresult(HRESULT result) {
	if (FAILED(0)) {
		throw _com_error(result);
	}
}

// ============================================================================
// # Creation
// There are currently three different versions of the CreateDXGIFactory method.
//
//   1. CreateDXGIFactory   : Creates DXGI 1.0 factory.
//   2. CreateDXGIFactory1  : Creates DXGI 1.1 factory.
//   3. CreateDXGIFactory2  : Creates DXGI 1.3 factory which has debug option.
//
// # Versions
// Each factory version also inherits all features from the parent version. At
// the moment, there are the following versions of the DXGI factory available.
//
//   IDXGIFactory  : Minimum OS is not specified.
//   IDXGIFactory1 : Minimum OS is Windows 7.
//   IDXGIFactory2 : Minimum OS is Windows 7 with platform update.
//   IDXGIFactory3 : Minimum OS is Windows 8.1.
//   IDXGIFactory4 : Minimum OS is not specified (Windows 8.1?)
//   IDXGIFactory5 : Minimum OS is not specified (Windows 8.1?)
//   IDXGIFactory6 : Minimum OS is Windows 10 (ver. 1803)
//   IDXGIFactory7 : Minimum OS is Windows 10 (ver. 1809)
//
// # Device Enumerations
// DXGI factory enumerates devices when the factory is created and NOT when the
// enumeration function is actually being called. Therefore it is important to
// (re)create a new factory after the set of system devices has been changed.
// ============================================================================

int main() {
	// create a new DXGI factory and apply debug flag in debug builds.
	ComPtr<IDXGIFactory7> factory;
	UINT flags = (UINT)0;
	#if defined(_DEBUG)
	flags = DXGI_CREATE_FACTORY_DEBUG;
	#endif
	check_hresult(CreateDXGIFactory2(flags, IID_PPV_ARGS(&factory)));

	// ==========================================================================
	// functions in the IDXGIFactory
	//
	//  CreateSoftwareAdapter : Allows the use of software adapter DLL. This is
	//                          used to assign custom device driver & emulation.
	//                          Implementing own SW-driver is NOT RECOMMENDED.
	//  CreateSwapChain       : Create a new swap chain for the target device
	//                          or the command queue if the Direct3D 12 is used.
	//                          Starting from D3D 11.1 usage is NOT RECOMMENDED.
	//  EnumAdapters          : Enumerate all display adapters in the system.
	//                          First adapter is the adapter that shows desktop.
	//                          Note that the factory enumerates the adapter set
	//                          when factory is created. If adapters change, a
	//                          new factory must be created to get current set.
	//  GetWindowAssociation  : Get a handle to the window that is currently
	//                          being resized on the window/fullscreen toggle.
	//  MakeWindowAssociation : Set the handle of the window to be monitored by
	//                          the DXGI for window specific graphics events.
	//                          Additional flags can be provided for association.
	//
	//                          DXGI_MWA_NO_WINDOW_CHANGES : Ignore all.
	//                          DXGI_MWA_NO_ALT_ENTER      : Ignore ALT+ENTER.
	//                          DXGI_MWA_NO_PRINT_SCREEN   : Ignore print-screen.
	// ==========================================================================

	// a simple adapter enumeration example.
	ComPtr<IDXGIAdapter> adapter;
	for (auto i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++) {
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		printf("Adapter %d device-id: %d\n", i, desc.DeviceId);
	}

	// additions in the IDXGIFactory1
	// TODO dxgiFactory->EnumAdapters1
	// TODO dxgiFactory->IsCurrent

	// additions in the IDXGIFactory2
	// TODO dxgiFactory->CreateSwapChainForComposition
	// TODO dxgiFactory->CreateSwapChainForCoreWindow
	// TODO dxgiFactory->CreateSwapChainForHwnd
	// TODO dxgiFactory->GetSharedResourceAdapterLuid
	// TODO dxgiFactory->IsWindowedStereoEnabled
	// TODO dxgiFactory->RegisterOcclusionStatusEvent
	// TODO dxgiFactory->RegisterOcclusionStatusWindow
	// TODO dxgiFactory->RegisterStereoStatusWindow
	// TODO dxgiFactory->UnregisterOcclusionStatus
	// TODO dxgiFactory->UnregisterStereoStatus

	// additions in the IDXGIFactory3
	// TOOD dxgiFactory->GetCreationFlags

	// additions in the IDXGIFactory4
	// TODO dxgiFactory->EnumWarpAdapter
	// TODO dxgiFactory->EnumAdapterByLuid

	// additions in the IDXGIFactory5
	// TODO dxgiFactory->CheckFeatureSupport

	// additions in the IDXGIFactory6
	// TODO dxgiFactory->EnumAdapterByGpuPreference

	// additions in the IDXGIFactory7
	// TOOD dxgiFactory->RegisterAdaptersChangedEvent
	// TODO dxgiFactory->UnregisterAdaptersChangedEvent

	return 0;
}