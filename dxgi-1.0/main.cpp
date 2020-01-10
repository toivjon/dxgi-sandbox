#include <wrl/client.h> // ComPtr
#include <comdef.h>		// _com_error
#include <string>

#include <dxgi.h>

#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL; // ComPtr

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

int main() {
	auto factory = createFactory();
	ComPtr<IDXGIAdapter> adapter;
	check_hresult(factory->EnumAdapters(0, &adapter));

	testDXGIObject(adapter);
	
	return 0;
}