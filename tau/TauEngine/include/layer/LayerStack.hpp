#pragma once

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

#include <DLL.hpp>
#include <layer/ILayer.hpp>

class TAU_DLL LayerStack final
{
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

    std::vector<ILayer*>::iterator begin() noexcept { return _layers.begin(); }
    std::vector<ILayer*>::iterator   end() noexcept { return _layers.end(); }

    std::vector<ILayer*>::iterator beginOverlay() noexcept { return _overlayLayers.begin(); }
    std::vector<ILayer*>::iterator   endOverlay() noexcept { return _overlayLayers.end(); }
};
