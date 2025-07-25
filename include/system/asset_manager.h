#pragma once
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include "utils.h"

namespace System
{

    template<typename T>
        class AssetStore
    {
    private:
        std::map<std::string, std::weak_ptr<T>> _assets;

    public:
        std::shared_ptr<T> load(const std::string &path)
        {
            // TODO: Load the asset from the filesystem
            // Nack get over here
            T asset;
            return asset;
        }

        void unload(const std::string &path)
        {
            auto iterator = _assets.find(path);
            if (iterator != _assets.end())
            {
                if (std::shared_ptr<T> asset = iterator->second.lock())
                {
                    LOG_DEBUG("WARNING: Unloading asset {} while it is in use", path);
                }
                _assets.erase(iterator);
            }
        }

        std::shared_ptr<T> get(const std::string &path)
        {
            auto iterator = _assets.find(path);
            if (iterator != _assets.end())
            {
                std::shared_ptr<T> asset = iterator->second.lock();
                if (asset)
                {
                    return (asset);
                }
            }
            return (load(path));
        }

        [[nodiscard]] bool contains(const std::string &path) const
        {
            return _assets.contains(path);
        }

        void clear()
        {
            _assets.clear();
        }

        ~AssetStore()
        {
            _assets.clear();
        }
    };

    template<typename... Assets>
    class AssetManager
    {
        std::tuple<AssetStore<Assets>...> _stores;

    public:

        template<typename T>
        std::shared_ptr<T> get(const std::string &path)
        {
            auto &store = std::get<AssetStore<T>>(_stores);
            return store.get(path);
        }

        template<typename T>
        void unload(const std::string &path)
        {
            auto &store = std::get<AssetStore<T>>(_stores);
            store.unload(path);
        }

        template<typename T>
        bool is_loaded(const std::string &path)
        {
            auto &store = std::get<AssetStore<T>>(_stores);
            return store.contains(path);
        }

        template<typename T>
        void clear()
        {
            auto &store = std::get<AssetStore<T>>(_stores);
            store.clear();
        }

        void clear_all()
        {
            (std::get<AssetStore<Assets>>(_stores).clear(), ...);
        }
    };
} // namespace System
