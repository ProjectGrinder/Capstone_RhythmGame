#pragma once
#include <concepts>
#include <string>

template <typename T>
concept AssetType = requires(const std::string& path) {
    { 
        T::load(path) 
    };
};