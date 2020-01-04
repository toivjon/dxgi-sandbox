#include <dxgi1_6.h>
#include <iostream>
#include <wrl/client.h> // ComPtr
#include <comdef.h>		// _com_error

#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL; // ComPtr

// a utility to easily catch failed HRESULTS.
inline void check_hresult(HRESULT result) {
	if (FAILED(result)) {
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
//
// # Features
// Here is a list of features that are currently available with DXGI factories.
//
//	 1. Enumerate and track changes in the list of available display adapters.
//		- EnumAdapterByGpuPreference
//		- EnumAdapterByLuid
//		- EnumAdapters
//		- EnumAdapters1
//		- EnumWarpAdapter
//		- RegisterAdaptersChangedEvent
//		- UnregisterAdaptersChangedEvent
//   2. Create swap chains.
//		- CreateSwapChain
//		- CreateSwapChainForComposition
//		- CreateSwapChainForCoreWindow
//		- CreateSwapChainForHwnd
//   3. Associate window with DXGI.
//		- GetWindowAssociation
//		- MakeWindowAssociation
//   4. Register occlusion event listeners.
//      - RegisterOcclusionStatusEvent
//		- RegisterOcclusionStatusWindow
//		- UnregisterOcclusionStatus
//   5. Query and trace state about the stereographic mode support.
//		- IsWindowedStereoEnabled
//		- RegisterStereoStatusWindow
//		- UnregisterStereoStatus
//   6. Create a software display adapter from an external DLL.
//		- CreateSoftwareAdapter
//	 7. Check the supported features (currenlty only tearing support).
//		- CheckFeatureSupport
//   8. Get the LUID of the adapter owning the given shared resource.
//		- GetSharedResourceAdapterLuid
//	 9. Get the flag that was used when the DXGI factory was created.
//		- GetCreationFlags
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

	// ==========================================================================
	// additions in the IDXGIFactory1
	// 
	// EnumAdapters1	: Enumerate all display adapters in the system like the
	//                    former EnumAdapters function. This version however, has
	//                    more former ordering of the results like following.
	//
	//                    * First adapter is the adapter which shows desktop.
	//                    * Then enumeration lists adapters with outputs.
	//                    * Finally enumeration lists adapters without outputs.
	// IsCurrent		: Tells whether there's been changed with the adapters
	//                    after the factory was created. In such cases, it's most
	//                    recommended that factory will be recreated and adapters
	//                    would get enumerated again to get up-to-date set.
	// ==========================================================================

	printf("enumeration up-to-date? %s\n", (factory->IsCurrent() ? "yes": "no"));
	
	// ==========================================================================
	// additions in the IDXGIFactory2
	//
	// CreateSwapChainForComposition	: Create a new swap chain for the target
	//                                    DirectComposition or XAML framework to
	//                                    device or the command queue if Direct3D
	//                                    12 is being used. Note that these swap
	//                                    chains allow only sequential flip swap
	//                                    effects (DXGI_SWAP_CHAIN_DESC1).
	// CreateSwapChainForCoreWindow		: Create a new swap chain for the target
	//                                    UWP CoreWindow to device or the command
	//									  queue if Direct3D 12 is being used.
	// CreateSwapChainForHwnd			: Create a new swap chain for the target
	//									  HWND handle to device or the command
	//									  queue if Direct3D 12 is being used.
	// GetSharedResourceAdapterLuid		: Identify the LUID of the adapter which
	//									  has a permission to open the resource.
	// IsWindowedStereoEnabled			: Check if the usage of the stereoscopic
	//									  graphics mode is allowed by the system.
	// RegisterOcclusionStatusEvent		: Register the application to receive OS
	//                                    event from the OS when application is
	//                                    occluded (hidden or otherwise unseen).
	// RegisterOcclusionStatusWindow	: Register the application window to get
	//									  OS event when application is occluded
	//									  (hidden or otherwise unseen).
	// RegisterStereoStatusWindow		: Register the application window the get
	//									  notification events about changes of the
	//									  stereo status.
	// UnregisterOcclusionStatus		: Unregister window or application from
	//									  receiving events about the occlusions.
	// UnregisterStereoStatus			: Unregister window or application from
	//									  receiving events of the stereo status.
	// ==========================================================================

	printf("stereographics supported? %d\n", factory->IsWindowedStereoEnabled());

	// ==========================================================================
	// additions in the IDXGIFactory3
	//
	// GetCreationFlags	: Get the flags provided for the DXGI factory during the
	//					  construction phase (CreateDXGIFactory2). At the moment
	//					  only supported flag is DXGI_CREATE_FACTORY_DEBUG.
	// ==========================================================================

	printf("factory creation flags: %d\n", factory->GetCreationFlags());

	// ==========================================================================
	// additions in the IDXGIFactory4
	//
	// EnumWarpAdapter		: Get the adapter that can be used build a Direct3D
	//						  device that uses a WARP renderer. WARP renderers are
	//						  quite heavy and should be only used in development.
	//						  Note that Xbox One requires one to use WARP device
	//						  when in the developer mode if Direct3D 12 is used.
	// EnumAdapterByLuid	: Get the adapter that has the provided LUID. Used in
	//						  pair with ID3D12Device::GetAdapterLuid if needed.
	// ==========================================================================

	check_hresult(factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter)));
	DXGI_ADAPTER_DESC adapterDesc;
	check_hresult(adapter->GetDesc(&adapterDesc));
	printf("WARP adapter details:\n");
	printf("system-mem  : %d\n", adapterDesc.DedicatedSystemMemory);
	printf("video-mem   : %d\n", adapterDesc.DedicatedVideoMemory);
	printf("shared-mem  : %d\n", adapterDesc.SharedSystemMemory);

	// ==========================================================================
	// additions in the IDXGIFactory5
	//
	// CheckFeatureSupport	: Check what kind of hardware features are available.
	//						  Currently enumeration contains only following item:
	//						  DXGI_FEATURE_PRESENT_ALLOW_TEARING, which is there
	//						  if hardware supports variable refresh rate display.
	// ==========================================================================

	BOOL allowTearing = FALSE;
	check_hresult(factory->CheckFeatureSupport(
		DXGI_FEATURE_PRESENT_ALLOW_TEARING,
		&allowTearing,
		sizeof(allowTearing)
	));
	printf("tearing supported: %s\n", (allowTearing ? "yes" : "no"));

	// ==========================================================================
	// additions in the IDXGIFactory6
	//
	// EnumAdapterByGpuPreference	: Enumerate and order adapters based on the
	//								  given GPU preference. There are possible
	//							      ordering options where to select.
	//
	//								  DXGI_CPU_PREFERENCE_UNSPECIFIED
	//								  DXGI_CPU_PREFERENCE_MINIMUM_POWER
	//								  DXGI_CPU_PREFERENCE_HIGH_PERFORMANCE
	// ==========================================================================

	for (auto i = 0;
		factory->EnumAdapterByGpuPreference(
			i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		printf("Adapter %d device-id: %d\n", i, desc.DeviceId);
	}

	// ==========================================================================
	// additions in the IDXGIFactory7
	// 
	// RegisterAdaptersChangedEvent	  : Register application to get notification
	//									of changes in the adapter enumeration.
	//									When this happens, we should build our
	//									DXGI factory again to renew enumeration.
	// UnregisterAdaptersChangedEvent : Unregister application to receive events
	//								    about the changes in adapter enumeration.
	// ==========================================================================

	return 0;
}