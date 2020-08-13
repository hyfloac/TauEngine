#include "Renderer.hpp"

RetCode Renderer::dxgiFactorySetupError(const HRESULT res) const noexcept
{
    switch(res)
    {
        case DXGI_ERROR_INVALID_CALL:
            MessageBoxW(NULL, L"Invalid call to CreateDXGIFactory1.", NULL, MB_OK | MB_ICONERROR);
            break;
        case DXGI_ERROR_UNSUPPORTED:
            MessageBoxW(NULL, L"Call to CreateDXGIFactory1 invoked unsupported functionality.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_UNEXPECTED:
        default:
            MessageBoxW(NULL, L"Unknown error while creating DXGI Factory.", NULL, MB_OK | MB_ICONERROR);
            break;
    }

    return RC_ErrorReported;
}

RetCode Renderer::deviceSetupError(const HRESULT res) const noexcept
{
    switch(res)
    {
        case D3D12_ERROR_ADAPTER_NOT_FOUND:
            MessageBoxW(NULL, L"Adapter could not be found while creating Direct3D12 Device.", NULL, MB_OK | MB_ICONERROR);
            break;
        case DXGI_ERROR_INVALID_CALL:
            MessageBoxW(NULL, L"Invalid call to D3D12CreateDevice.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_FAIL:
            MessageBoxW(NULL, L"D3D12CreateDevice returned E_FAIL. This may be because the device was created with the debug layer was enabled, but the layer is not installed in the system, this would indicate that the app is running on non-development system.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_INVALIDARG:
            MessageBoxW(NULL, L"Invalid argument passed to D3D12CreateDevice.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_OUTOFMEMORY:
            MessageBoxW(NULL, L"Driver ran out memory while creating Direct3D12 Device.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_NOTIMPL:
            MessageBoxW(NULL, L"D3D12CreateDevice was not implemented.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_UNEXPECTED:
        default:
            MessageBoxW(NULL, L"Unknown error while creating Direct3D12 Device.", NULL, MB_OK | MB_ICONERROR);
            break;
    }

    return RC_ErrorReported;
}

RetCode Renderer::commandQueueSetupError(const HRESULT res) const noexcept
{
    switch(res)
    {
        case DXGI_ERROR_INVALID_CALL:
            MessageBoxW(NULL, L"Invalid call to ID3D12Device::CreateCommandQueue.", L"Error: Command Queue", MB_OK | MB_ICONERROR);
            break;
        case E_FAIL:
            MessageBoxW(NULL, L"ID3D12Device::CreateCommandQueue returned E_FAIL.", L"Error: Command Queue", MB_OK | MB_ICONERROR);
            break;
        case E_INVALIDARG:
            MessageBoxW(NULL, L"Invalid argument passed to ID3D12Device::CreateCommandQueue.", L"Error: Command Queue", MB_OK | MB_ICONERROR);
            break;
        case E_OUTOFMEMORY:
            MessageBoxW(NULL, L"Driver ran out memory while creating Command Queue.", L"Error: Command Queue", MB_OK | MB_ICONERROR);
            break;
        case E_NOTIMPL:
            MessageBoxW(NULL, L"ID3D12Device::CreateCommandQueue was not implemented.", L"Error: Command Queue", MB_OK | MB_ICONERROR);
            break;
        case E_UNEXPECTED:
        default:
            MessageBoxW(NULL, L"Unknown error while creating Command Queue.", L"Error: Command Queue", MB_OK | MB_ICONERROR);
            break;
    }

    return RC_ErrorReported;
}

RetCode Renderer::uploadQueueSetupError(const HRESULT res) const noexcept
{
    switch(res)
    {
        case DXGI_ERROR_INVALID_CALL:
            MessageBoxW(NULL, L"Invalid call to ID3D12Device::CreateCommandQueue.", L"Error: Upload Queue", MB_OK | MB_ICONERROR);
            break;
        case E_FAIL:
            MessageBoxW(NULL, L"ID3D12Device::CreateCommandQueue returned E_FAIL.", L"Error: Upload Queue", MB_OK | MB_ICONERROR);
            break;
        case E_INVALIDARG:
            MessageBoxW(NULL, L"Invalid argument passed to ID3D12Device::CreateCommandQueue.", L"Error: Upload Queue", MB_OK | MB_ICONERROR);
            break;
        case E_OUTOFMEMORY:
            MessageBoxW(NULL, L"Driver ran out memory while creating Upload Queue.", L"Error: Upload Queue", MB_OK | MB_ICONERROR);
            break;
        case E_NOTIMPL:
            MessageBoxW(NULL, L"ID3D12Device::CreateCommandQueue was not implemented.", L"Error: Upload Queue", MB_OK | MB_ICONERROR);
            break;
        case E_UNEXPECTED:
        default:
            MessageBoxW(NULL, L"Unknown error while creating Upload Queue.", L"Error: Upload Queue", MB_OK | MB_ICONERROR);
            break;
    }

    return RC_ErrorReported;
}

RetCode Renderer::swapChainSetupError(const HRESULT res) const noexcept
{
    switch(res)
    {
        case DXGI_ERROR_INVALID_CALL:
            MessageBoxW(NULL, L"Invalid call to IDXGIFactory2::CreateSwapChainForHwnd.", NULL, MB_OK | MB_ICONERROR);
            break;
        case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:
            MessageBoxW(NULL, L"Swap Chain is currently unavailable.", NULL, MB_OK | MB_ICONERROR);
            break;
        case DXGI_ERROR_SDK_COMPONENT_MISSING:
            MessageBoxW(NULL, L"A required SDK component for IDXGIFactory2::CreateSwapChainForHwnd is missing.", NULL, MB_OK | MB_ICONERROR);
            break;
        case DXGI_ERROR_UNSUPPORTED:
            MessageBoxW(NULL, L"IDXGIFactory2::CreateSwapChainForHwnd was unsupported.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_FAIL:
            MessageBoxW(NULL, L"IDXGIFactory2::CreateSwapChainForHwnd returned E_FAIL.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_INVALIDARG:
            MessageBoxW(NULL, L"Invalid argument passed to IDXGIFactory2::CreateSwapChainForHwnd.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_OUTOFMEMORY:
            MessageBoxW(NULL, L"Driver ran out memory while creating Swap Chain.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_NOTIMPL:
            MessageBoxW(NULL, L"IDXGIFactory2::CreateSwapChainForHwnd was not implemented.", NULL, MB_OK | MB_ICONERROR);
            break;
        case E_UNEXPECTED:
        default:
            MessageBoxW(NULL, L"Unknown error while creating Swap Chain.", NULL, MB_OK | MB_ICONERROR);
            break;
    }

    return RC_ErrorReported;
}
