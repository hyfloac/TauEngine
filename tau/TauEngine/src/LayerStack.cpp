#include <layer/LayerStack.hpp>

LayerStack::~LayerStack() noexcept
{
    for(ILayer* layer : _layers)
    {
        layer->onDetach();
        delete layer;
    }

    for(ILayer* overlay : _overlayLayers)
    {
        overlay->onDetach();
        delete overlay;
    }
}

void LayerStack::pushLayer(ILayer* layer) noexcept
{
    _layers.emplace_back(layer);
    layer->onAttach();
}

void LayerStack::removeLayer(ILayer* layer) noexcept
{
    const std::vector<ILayer*>::iterator it = std::find(_layers.begin(), _layers.end(), layer);
    if(it != _layers.end())
    {
        layer->onDetach();
        _layers.erase(it);
        delete layer;
    }
}

void LayerStack::pushOverlay(ILayer* layer) noexcept
{
    _overlayLayers.emplace_back(layer);
    layer->onAttach();
}

void LayerStack::removeOverlay(ILayer* layer) noexcept
{
    const std::vector<ILayer*>::iterator it = std::find(_overlayLayers.begin(), _overlayLayers.end(), layer);
    if(it != _layers.end())
    {
        layer->onDetach();
        _overlayLayers.erase(it);
        delete layer;
    }
}
