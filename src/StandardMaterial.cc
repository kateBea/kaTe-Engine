/**
* StandardMaterial.cc
* Created by kate on 6/30/23.
* */


// C++ Standard Library
#include <string>
#include <string_view>
#include <memory>

// Project Headers
#include "Renderer/Material/Material.hh"
#include "Renderer/Material/StandardMaterial.hh"
#include "Tools/Common.hh"

namespace kaTe {

    StandardMaterial::StandardMaterial(std::string_view name)
        :   Material{ name, Type::STANDARD }
    {

    }

    auto StandardMaterial::Use() -> void {

    }
}