/**
 * Material.cc
 * Created by kate on 6/30/23.
 * */

// C++ Standard Library
#include <string>
#include <string_view>
#include <memory>

// Project Headers
#include <Tools/Common.hh>
#include <Renderer/Material/Material.hh>

namespace kaTe {

    auto Material::Create(Type matType) -> std::shared_ptr<Material> {
        switch(matType) {
                // TODO:
            case Type::STANDARD: return nullptr;
        }
    }
}
