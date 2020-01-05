# dxgi-sandbox
This sandbox is dedicated for my personal study about how the Microsoft DirectX Graphics Infrastructure (DXGI) works.

DXGI is used by the following DirectX versions:
* DirectX 10
* DirectX 11
* DirectX 12

## DXGI Versions

| Version | Header    | Minimum Windows Platform    | Version Improvements |
| ------- | --------- | --------------------------- | -------------------- |
|   1.0	  | dxgi.h    | Windows Vista     		    | The base version. | 
|   1.1	  | dxgi.h    | Windows Vista (SP2) 		| - Support for synchronized shared surfaces<br/> - Support for high color<br/> - Support for maximum frame latency control<br/> - Support for adapter enumeration ordering<br/> - Support for BGRA<br/> |
|   1.2	  | dxgi1_2.h | Windows 7 (Platform Update) | - Support for stereo swap chain<br/> - Support for flip-model swap chain<br/> - Support for desktop duplication<br/> - Support for 16 bpp formats<br/> - Support for debugging APIs<br/> - Optimization of the use of video memory<br/> - Optimization for presentations |
|   1.3	  | dxgi1_3.h | Windows 8.1                 | - Support for DXGI adapter memory trim<br/> - Support for multi-plane overlays<br/> - Support for overlapping and scaled swap chains<br/> - Support for selecting backbuffer subregion for swap chain<br/> - Support for lower-latency swap chain presentation |
|   1.4	  | dxgi1_4.h | Windows 10                  | - Support for cheaper adapter enumeration<br/> - Support for video memory budget tracking<br/> - Support for invariant backbuffer identity<br/> - Support for alternate frame rendering (AFR)<br/> - Fullscreen state no longer owns the display |
|   1.5	  | dxgi1_5.h | Windows 10                  | - Support for Hich Dynamic Range (HDR)<br/> - Support for Wide Color Gamut (WCG)<br/> - Support for flexible and performant output duplication<br/> - Support for offering and reclaiming resources |
|   1.6   | dxgi1_6.h | Windows 10                  | - Support for listening adapter enumeration change events<br/> - Support for adapter enumeration ordering prefence<br/> - Support for checking adapter synchronization object features<br/> - Support for detecting HDR display detection |

## DXGI Interfaces

![alt text](https://github.com/toivjon/dxgi-sandbox/blob/master/images/dxgi-interfaces.svg "DXGI Interfaces")

| Interface Name         | Since | Description |
| ---------------------- | ----- | ----------- |
| IUnknown               | 1.0   | The root interface for all COM interfaces. |
| IDXGISwapChainMedia    | 1.3   | Represents an access to request a seamless change to specific refresh rate. |
| IDXGIFactoryMedia      | 1.3   | Represents an access to build and DirectComposition surface swap chains. |
| IDXGIObject            | 1.0   | The base interface for most DXGI objects. |
| IDXGIDecodeSwapChain   | 1.3   | Represents a swap chain used to show video data on DirectComposition surface. |
| IDXGIDisplayControl    | 1.2   | Represents stereoscopic 3D display behavior and status. |
| IDXGIFactory           | 1.0   | A factory object used to build DXGI objects. |
| IDXGIAdapter 		     | 1.0   | Represents a display subsystem containing GPUs, DACs and video memory. |
| IDXGIDevice 		     | 1.0   | Represents an abstraction of an DXGI object that produce image data. |
| IDXGIDeviceSubObject 	 | 1.0   | An abstraction for all objects that are tied to a DXGI device. |
| IDXGIOutput            | 1.0   | Represents an adapter output such as monitor or television. |
| IDXGIOutputDuplication | 1.2   | Represents an access to acquire and manipulate duplicated desktop images. |
| IDXGISurface           | 1.0   | Represents an 2D image-data section in memory. |
| IDXGIKeyedMutex        | 1.1   | Represents a mutex used to synchronized access to a shared resource. |
| IDXGISwapChain         | 1.0   | Represents a swap chain for presenting surfaces to an output. |
| IDXGIResource          | 1.0   | Represents a shareable resource within a memory. |

## Sources

[DXGI Documentation](https://docs.microsoft.com/fi-fi/windows/win32/direct3ddxgi/dx-graphics-dxgi)

[IUnknown Documentation](https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nn-unknwn-iunknown)