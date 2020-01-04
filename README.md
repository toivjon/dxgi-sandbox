# dxgi-sandbox
This sandbox is dedicated for my personal study about how the Microsoft DirectX Graphics Infrastructure (DXGI) works.

DXGI is used by the following DirectX versions:
* DirectX 10
* DirectX 11
* DirectX 12

## DXGI Versions

| Version | Header    | Minimum Windows Platform    | Descriptiom
| ------- | --------- | --------------------------- | ----------------- |
|   1.0	  | dxgi.h    | Windows Vista     		    | The base version. | 
|   1.1	  | dxgi.h    | Windows Vista (SP2) 		| <ul><li>A</li><li>B</li></ul> |
|   1.2	  | dxgi1_2.h | Windows 7 (Platform Update) | |
|   1.3	  | dxgi1_3.h | Windows 8.1                 | |
|   1.4	  | dxgi1_4.h | Windows 10                  | |
|   1.5	  | dxgi1_5.h | Windows 10                  | |
|   1.6   | dxgi1_6.h | Windows 10                  | |

DXGI contains the following components:
* IDXGIAdapter
* IDXGIDecodeSwapChain
* IDXGIDevice
* IDXGIDeviceSubObject
* IDXGIDisplayControl
* IDXGIFactory
* IDXGIFactoryMedia
* IDXGIKeyedMutex
* IDXGIObject
* IDXGIOutput
* IDXGIOutputDuplication
* IDXGIResource
* IDXGISurface
* IDXGISwapChain
* IDXGISwapChainMedia

## Sources

[DXGI Documentation](https://docs.microsoft.com/fi-fi/windows/win32/direct3ddxgi/dx-graphics-dxgi)
