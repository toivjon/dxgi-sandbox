#pragma once

#include <wrl/client.h> // ComPtr
#include <comdef.h>		// _com_error

// a utility to easily catch failed HRESULTS.
inline void check_hresult(HRESULT result) {
	if (FAILED(result)) {
		throw _com_error(result);
	}
}

// a utility to count COM object references.
inline UINT countRefs(Microsoft::WRL::ComPtr<IUnknown> object) {
	object->AddRef();
	return object->Release();
}

// a utility to create a new GUID.
inline GUID createGUID() {
	GUID guid;
	check_hresult(CoCreateGuid(&guid));
	return guid;
}