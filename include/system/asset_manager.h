#pragma once
#include <vector>
#include <map>
#include <memory>
#include <string>
#include "utils.h"

namespace System
{
    template<typename T>
    class AssetStore
    {
    private:
        std::vector<std::string> _paths;
        std::vector<std::weak_ptr<T>> _assets;
        std::vector<size_t> _index_to_id;
        std::vector<size_t> _id_to_index;

        [[nodiscard]] size_t find_path_index(const std::string& path) const {
            const auto it = std::ranges::find(_paths, path);
            return it != _paths.end() ? std::distance(_paths.begin(), it) : std::string::npos;
        }

    public:
        std::shared_ptr<T> load(const std::string& path)
        {
            // TODO: Load the asset from the filesystem
            // Nack get over here
            T asset();
            const size_t new_id = _paths.size();

            _paths.push_back(path);
            _assets.emplace_back();  // Empty weak_ptr
            _index_to_id.push_back(new_id);
            _id_to_index.push_back(new_id);

            return std::make_shared<T>(std::move(asset));
        }

        void unload(const std::string& path)
        {
            size_t index = find_path_index(path);
            if (index != std::string::npos)
            {
                if (std::shared_ptr<T> asset = _assets[index].lock())
                {
                    LOG_WARNING("Unloading asset %s while it is in use", path.c_str());
                }

                // Remove the asset
                const size_t id = _index_to_id[index];
                size_t last_index = _paths.size() - 1;

                // If this isn't the last element, move the last element to this position
                if (index < last_index)
                {
                    _paths[index] = std::move(_paths[last_index]);
                    _assets[index] = std::move(_assets[last_index]);
                    _index_to_id[index] = _index_to_id[last_index];
                    _id_to_index[_index_to_id[index]] = index;
                }

                // Remove the last elements
                _paths.pop_back();
                _assets.pop_back();
                _index_to_id.pop_back();
                _id_to_index[id] = std::string::npos;
            }
        }

        std::shared_ptr<T> get(const std::string& path)
        {
            size_t index = find_path_index(path);
            if (index != std::string::npos)
            {
                if (std::shared_ptr<T> asset = _assets[index].lock())
                {
                    return asset;
                }
            }
            return load(path);
        }

        [[nodiscard]] bool contains(const std::string& path) const
        {
            return find_path_index(path) != std::string::npos;
        }

        void clear()
        {
            _paths.clear();
            _assets.clear();
            _index_to_id.clear();
            _id_to_index.clear();
        }

        ~AssetStore()
        {
            clear();
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
