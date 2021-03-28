#include <dxgi.h>
#include <d3d11.h>
#include <iostream>
#include <Windows.h>
#include <winrt/base.h>
#include <locale>
#include <codecvt>
#include <sstream>

#include "DXGIUtils.h"
#include "MathUtils.hpp"
#include "GraphicsInfoWriter.hpp"
#include "TestCases.hpp"

static void iterateAdapters();

static GraphicsInfoWriter* writer;

int main(int argCount, char* args[])
{
    writer = new GraphicsInfoWriter(L"GraphicsResults.json");

    iterateAdapters();
    
    delete writer;

    return 0;
}

static void writeDeviceInfo(const winrt::com_ptr<IDXGIAdapter>& adapter, UINT adapterIndex);
static void checkDevice(const winrt::com_ptr<IDXGIAdapter>& adapter, UINT adapterIndex);

static void testTexture2D(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context, UINT adapterIndex);
static void testTexture3D(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context, UINT adapterIndex);

static void testTexture2D(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context, const D3D11_TEXTURE2D_DESC& desc, UINT adapterIndex);
static void testTexture3D(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context, const D3D11_TEXTURE3D_DESC& desc, UINT adapterIndex);

static void iterateAdapters()
{
    winrt::com_ptr<IDXGIFactory> dxgiFactory;
    const HRESULT res = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

    if(FAILED(res))
    { return; }
    
    winrt::com_ptr<IDXGIAdapter> dxgiAdapter;

    writer->beginArray("gpus");
    {
        for(UINT i = 0; dxgiFactory->EnumAdapters(i, dxgiAdapter.put()) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            writeDeviceInfo(dxgiAdapter, i);
            dxgiAdapter = nullptr;
        }
    }
    writer->endArray();

    writer->beginArray("alignments");
    {
        for(UINT i = 0; dxgiFactory->EnumAdapters(i, dxgiAdapter.put()) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            checkDevice(dxgiAdapter, i);
            dxgiAdapter = nullptr;
        }
    }
    writer->endArray();
}

static void writeDeviceInfo(const winrt::com_ptr<IDXGIAdapter>& adapter, const UINT adapterIndex)
{
#ifdef _DEBUG
    const UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
    const UINT flags = 0;
#endif

    const D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };

    winrt::com_ptr<ID3D11Device> device;
    winrt::com_ptr<ID3D11DeviceContext> context;
    const HRESULT res = D3D11CreateDevice(
        adapter.get(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        flags,
        levels,
        1,
        D3D11_SDK_VERSION,
        device.put(),
        nullptr,
        context.put()
    );

    if(FAILED(res))
    { return; }

    DXGI_ADAPTER_DESC desc;
    adapter->GetDesc(&desc);

    writer->beginObject();
    {
        using convert_type = ::std::codecvt_utf8<wchar_t>;
        ::std::wstring_convert<convert_type, wchar_t> converter;

        const ::std::string deviceDescription = converter.to_bytes(desc.Description);
        writer->writeString("desc", deviceDescription);

        ::std::stringstream idStream;
        idStream << std::hex << desc.VendorId << ':' << desc.DeviceId;

        writer->writeString("id", idStream.str());
        writer->writeInt("index", adapterIndex);
        writer->writeInt("ded-vid-mem", desc.DedicatedVideoMemory);
        writer->writeInt("ded-sys-mem", desc.DedicatedSystemMemory);
        writer->writeInt("shared-sys-mem", desc.SharedSystemMemory);
    }
    writer->endObject();
}

static void checkDevice(const winrt::com_ptr<IDXGIAdapter>& adapter, UINT adapterIndex)
{
#ifdef _DEBUG
    const UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
    const UINT flags = 0;
#endif

    const D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };

    winrt::com_ptr<ID3D11Device> device;
    winrt::com_ptr<ID3D11DeviceContext> context;
    const HRESULT res = D3D11CreateDevice(
        adapter.get(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        flags,
        levels,
        1,
        D3D11_SDK_VERSION,
        device.put(),
        nullptr,
        context.put()
    );

    if(FAILED(res))
    { return; }

    DXGI_ADAPTER_DESC desc;
    adapter->GetDesc(&desc);
    
    // ::std::wcout << L"Device: " << desc.Description << ::std::endl;
    // ::std::wcout << L"ID: " << std::hex << desc.VendorId << L":" << desc.DeviceId << std::endl;
    //
    // ::std::wcout << std::dec << std::endl;

    testTexture2D(device, context, adapterIndex);
    testTexture3D(device, context, adapterIndex);

    // ::std::wcout << L"=======================================" << std::endl;
    // ::std::wcout << std::endl;
}

static void testTexture2D(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context, const UINT adapterIndex)
{
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = 1;
    desc.Height = 1;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    
    for(int i = 0; i < Texture2DTestCount; ++i)
    {
        desc.Width = Texture2DTests[i].width;
        desc.Height = Texture2DTests[i].height;

        for(int j = 0; j < TestFormatCount; ++j)
        {
            if(isBlockCompressed(TestFormats[j]))
            {
                if(desc.Width % 4 != 0 || desc.Height % 4 != 0)
                { continue; }
            }

            desc.Format = TestFormats[j];
            
            testTexture2D(device, context, desc, adapterIndex);
        }

        context->Flush();
        ::std::this_thread::sleep_for(::std::chrono::milliseconds(25));
    }
}

static void testTexture3D(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context, const UINT adapterIndex)
{
    D3D11_TEXTURE3D_DESC desc;
    desc.Width = 1;
    desc.Height = 1;
    desc.Depth = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    
    for(int i = 0; i < Texture3DTestCount; ++i)
    {
        desc.Width = Texture3DTests[i].width;
        desc.Height = Texture3DTests[i].height;
        desc.Depth = Texture3DTests[i].depth;

        for(int j = 0; j < TestFormatCount; ++j)
        {
            if(isDepthStencil(TestFormats[j]) || isBlockCompressed(TestFormats[j]))
            {
                continue;
            }

            desc.Format = TestFormats[j];
            
            testTexture3D(device, context, desc, adapterIndex);
            
            if(j == TestFormatCount % 8)
            {
                context->Flush();
                ::std::this_thread::sleep_for(::std::chrono::milliseconds(15));
            }
        }

        context->Flush();
        ::std::this_thread::sleep_for(::std::chrono::milliseconds(25));
    }
}

static void testTexture2D(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context, const D3D11_TEXTURE2D_DESC& desc, const UINT adapterIndex)
{
    winrt::com_ptr<ID3D11Texture2D> texture2D;
    HRESULT res = device->CreateTexture2D(
        &desc,
        nullptr,
        texture2D.put()
    );

    if(FAILED(res))
    { return; }

    D3D11_MAPPED_SUBRESOURCE mapping;
    res = context->Map(
        texture2D.get(), 
        0, 
        D3D11_MAP_WRITE_DISCARD, 
        0, 
        &mapping);

    if(FAILED(res))
    { return; }

    context->Unmap(texture2D.get(), 0);

    writer->beginObject();
    {
        writer->writeInt("adapter", adapterIndex);
        writer->writeString("type", "Texture2D");
        writer->writeInt("width", desc.Width);
        writer->writeInt("height", desc.Height);
        writer->writeString("format", getDxgiFormatNameA(desc.Format));
        writer->writeInt("row-pitch", mapping.RowPitch);
        writer->writeInt("row-alignment", (1 << _ctz(mapping.RowPitch)));
    }
    writer->endObject();

    // ::std::wcout << L"Texture2D: " << desc.Width << L"x" << desc.Height << std::endl;
    // ::std::wcout << L"Format: " << getDxgiFormatNameW(desc.Format) << std::endl;
    // ::std::wcout << L"Row Pitch: " << mapping.RowPitch << std::endl;
    // ::std::wcout << L"Row Alignment: " << (1 << _ctz(mapping.RowPitch)) << ::std::endl;
    // ::std::wcout << std::endl;
}

static void testTexture3D(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context, const D3D11_TEXTURE3D_DESC& desc, const UINT adapterIndex)
{
    winrt::com_ptr<ID3D11Texture3D> texture3D;
    HRESULT res = device->CreateTexture3D(
        &desc,
        nullptr,
        texture3D.put()
    );

    if(FAILED(res))
    { return; }

    D3D11_MAPPED_SUBRESOURCE mapping;
    res = context->Map(
        texture3D.get(), 
        0, 
        D3D11_MAP_WRITE_DISCARD, 
        0, 
        &mapping);

    if(FAILED(res))
    { return; }
    
    context->Unmap(texture3D.get(), 0);

    writer->beginObject();
    {
        writer->writeInt("adapter", adapterIndex);
        writer->writeString("type", "Texture3D");
        writer->writeInt("width", desc.Width);
        writer->writeInt("height", desc.Height);
        writer->writeInt("depth", desc.Depth);
        writer->writeString("format", getDxgiFormatNameA(desc.Format));
        writer->writeInt("row-pitch", mapping.RowPitch);
        writer->writeInt("depth-pitch", mapping.DepthPitch);
        writer->writeInt("row-alignment", (1 << _ctz(mapping.RowPitch)));
        writer->writeInt("depth-alignment", (1 << _ctz(mapping.DepthPitch)));
    }
    writer->endObject();

    // ::std::wcout << L"Texture3D: " << desc.Width << L"x" << desc.Height << L"x" << desc.Depth << std::endl;
    // ::std::wcout << L"Format: " << getDxgiFormatNameW(desc.Format) << std::endl;
    // ::std::wcout << L"Row Pitch: " << mapping.RowPitch << std::endl;
    // ::std::wcout << L"Depth Pitch: " << mapping.DepthPitch << std::endl;
    // ::std::wcout << L"Row Alignment: " << (1 << _ctz(mapping.RowPitch)) << ::std::endl;
    // ::std::wcout << L"Depth Alignment: " << (1 << _ctz(mapping.DepthPitch)) << ::std::endl;
    // ::std::wcout << std::endl;
}
