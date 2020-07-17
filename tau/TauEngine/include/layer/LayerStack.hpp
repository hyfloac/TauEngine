#pragma once

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

#include "DLL.hpp"
#include "Layer.hpp"

class TAU_DLL LayerStack final
{
    DEFAULT_CONSTRUCT_PU(LayerStack);
    DELETE_CM(LayerStack);
public:
    template<typename _Iter>
    struct IteratorContainer
    {
        _Iter _begin;
        _Iter _end;

        _Iter begin() noexcept { return _begin; }
        _Iter end() noexcept { return _end; }
    };
private:
    std::vector<ILayer*> _layers;
    std::vector<ILayer*> _overlayLayers;
public:
    ~LayerStack() noexcept;

    template<typename _T, typename... _Args>
    _T* emplaceLayer(_Args&&... args) noexcept
    {
        _T* const layer = new(::std::nothrow) _T(::std::forward<_Args>(args)...);
        _layers.emplace_back(layer);
        layer->onAttach();
        return layer;
    }

    template<typename _T, typename... _Args>
    _T* emplaceOverlay(_Args&&... args) noexcept
    {
        _T* const overlay = new(::std::nothrow) _T(::std::forward<_Args>(args)...);
        _overlayLayers.emplace_back(overlay);
        overlay->onAttach();
        return overlay;
    }

    void   pushLayer(ILayer* layer) noexcept;
    void removeLayer(ILayer* layer) noexcept;

    void   pushOverlay(ILayer* layer) noexcept;
    void removeOverlay(ILayer* layer) noexcept;

    IteratorContainer<std::vector<ILayer*>::iterator> layers()   noexcept { return { _layers.begin(), _layers.end() }; }
    IteratorContainer<std::vector<ILayer*>::iterator> overlays() noexcept { return { _overlayLayers.begin(), _overlayLayers.end() }; }

    IteratorContainer<std::vector<ILayer*>::reverse_iterator> rlayers()   noexcept { return { _layers.rbegin(), _layers.rend() }; }
    IteratorContainer<std::vector<ILayer*>::reverse_iterator> roverlays() noexcept { return { _overlayLayers.rbegin(), _overlayLayers.rend() }; }
};
