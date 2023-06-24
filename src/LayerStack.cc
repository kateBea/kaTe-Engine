/**
 * LayerStack.cc
 * Created by kate on 5/26/23.
 * */

// C++ Standard Library
#include <algorithm>

// Project headers
#include <Core/Logger.hh>
#include <Core/Layers/LayerStack.hh>

namespace kaTe {
    auto LayerStack::addLayer(std::shared_ptr<Layer> layer) -> void {
        m_Layers.emplace(m_Layers.begin() + m_LayerIndex, layer);
        ++m_LayerIndex;
    }

    auto LayerStack::addOverlay(std::shared_ptr<Layer> overlay) -> void {
        m_Layers.emplace_back(overlay);
    }

    auto LayerStack::popLayer(std::shared_ptr<Layer> layer) -> void {
        auto targetIt{ std::find(m_Layers.begin(), m_Layers.end(), layer) };
        if (targetIt != m_Layers.end()) {
            m_Layers.erase(targetIt);
            --m_LayerIndex;
        }

    }

    auto LayerStack::popOverlay(std::shared_ptr<Layer> overlay) -> void {
        auto targetIt{ std::find(m_Layers.begin(), m_Layers.end(), overlay) };
        if (targetIt != m_Layers.end())
            m_Layers.erase(targetIt);

    }

    auto LayerStack::Init() -> void {
        KATE_CORE_LOGGER_INFO("Layer Stack initialization");
    }

}   // END NAMESPACE kT