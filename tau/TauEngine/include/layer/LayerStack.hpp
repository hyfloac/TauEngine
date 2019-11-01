#pragma once

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

#include <DLL.hpp>
#include <layer/ILayer.hpp>

class TAU_DLL LayerStack final
{
public:
    template<typename _Iter>
    struct IteratorContainer
    {
        using iter = std::vector<ILayer*>::iterator;

        _Iter _begin;
        _Iter _end;

        _Iter begin() noexcept { return _begin; }
        _Iter end() noexcept { return _end; }
    };
private:
    std::vector<ILayer*> _layers;
    std::vector<ILayer*> _overlayLayers;
public:
    inline LayerStack() noexcept = default;

    ~LayerStack() noexcept;

    LayerStack(const LayerStack& copy) noexcept = delete;
    LayerStack(LayerStack&& move) noexcept = delete;

    LayerStack& operator=(const LayerStack& copy) noexcept = delete;
    LayerStack& operator=(LayerStack&& move) noexcept = delete;

    void   pushLayer(ILayer* layer) noexcept;
    void removeLayer(ILayer* layer) noexcept;

    void   pushOverlay(ILayer* layer) noexcept;
    void removeOverlay(ILayer* layer) noexcept;

    IteratorContainer<std::vector<ILayer*>::iterator> layers()   noexcept { return { _layers.begin(), _layers.end() }; }
    IteratorContainer<std::vector<ILayer*>::iterator> overlays() noexcept { return { _overlayLayers.begin(), _overlayLayers.end() }; }

    IteratorContainer<std::vector<ILayer*>::reverse_iterator> rlayers()   noexcept { return { _layers.rbegin(), _layers.rend() }; }
    IteratorContainer<std::vector<ILayer*>::reverse_iterator> roverlays() noexcept { return { _overlayLayers.rbegin(), _overlayLayers.rend() }; }
};
