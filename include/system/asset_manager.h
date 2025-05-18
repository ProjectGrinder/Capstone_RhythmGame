#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>
#include "assets.h"
#include "utils.h"

namespace System
{
    class AssetManager
    {
    private:
        class _BaseAssetStore
        {
        public:
            virtual void clear() = 0;
            virtual ~_BaseAssetStore() = default;
        };

        template <AssetType Asset>
        class _AssetStore : public _BaseAssetStore
        {
        private:
            std::map<std::string, std::weak_ptr<Asset>> _assets;
        public:
            std::shared_ptr<Asset> load(const std::string& path)
            {
                // TODO: Compute absolute path relative to executable for loading
                std::string filepath = path;

                std::shared_ptr<Asset> asset = Asset::load(filepath);

                _assets[path] = asset;
                return asset;
            }

            void unload(const std::string& path)
            {
                auto iterator = _assets.find(path);
                if (iterator != _assets.end())
                {
                    std::shared_ptr<Asset> asset = iterator->second.lock();
                    if (asset)
                    {
                        LOG_DEBUG
                        (
                            "WARNING: Unloading asset {} while it is in use",
                            path
                        );
                    }
                    _assets.erase(iterator);
                }
            }

            std::shared_ptr<Asset> get(const std::string& path)
            {
                auto iterator = _assets.find(path);
                if (iterator != _assets.end())
                {
                    std::shared_ptr<Asset> asset = iterator->second.lock();
                    if (asset)
                    {
                        return (asset);
                    }
                }
                return (load(path));
            }

            void clear() override
            {
                _assets.clear();
            }

            ~_AssetStore() override
            {
                _assets.clear();
            }
        };

        static AssetManager _instance;
        std::unordered_map<std::type_index, std::unique_ptr<_BaseAssetStore>> _stores;
        AssetManager() {};
    public:
        static AssetManager& instance();

        template <AssetType Asset>
        std::shared_ptr<Asset> get(const std::string& path)
        {
            auto type_index = std::type_index(typeid(Asset));
            auto iterator = _stores.find(type_index);
            if (iterator != _stores.end())
            {
                auto store = static_cast<_AssetStore<Asset>*>(iterator->second.get());
                return (store->get(path));
            }
            else
            {
                auto store = std::make_unique<_AssetStore<Asset>>();
                auto asset = store->get(path);
                _stores[type_index] = std::move(store);
                return (asset);
            }
        }

        template <AssetType Asset>
        void unload(const std::string& path)
        {
            auto type_index = std::type_index(typeid(Asset));
            auto iterator = _stores.find(type_index);
            if (iterator != _stores.end())
            {
                auto store = static_cast<_AssetStore<Asset>*>(iterator->second.get());
                store->_assets.erase(path);
            }
        }

        template <AssetType Asset>
        bool is_loaded(const std::string& path)
        {
            auto type_index = std::type_index(typeid(Asset));
            auto iterator = _stores.find(type_index);
            if (iterator != _stores.end())
            {
                auto store = static_cast<_AssetStore<Asset>*>(iterator->second.get());
                return (store->_assets.find(path) != store->_assets.end());
            }
            return (false);
        }

        template <AssetType Asset>
        void clear()
        {
            auto type_index = std::type_index(typeid(Asset));
            auto iterator = _stores.find(type_index);
            if (iterator != _stores.end())
            {
                auto store = static_cast<_AssetStore<Asset>*>(iterator->second.get());
                store->clear();
            }
        }

        void clear_all()
        {
            for (auto& store : _stores)
            {
                store.second->clear();
            }
        }
    };
}