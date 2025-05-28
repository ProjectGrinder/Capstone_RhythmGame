#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>
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

        template <typename T>
        class _AssetStore : public _BaseAssetStore
        {
        private:
            std::map<std::string, std::weak_ptr<T>> _assets;
        public:
            std::shared_ptr<T> load(const std::string& path)
            {
                // TODO: Load the asset from the filesystem
                // Nack get over here
                T asset;
                return asset;
            }

            void unload(const std::string& path)
            {
                auto iterator = _assets.find(path);
                if (iterator != _assets.end())
                {
                    std::shared_ptr<T> asset = iterator->second.lock();
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

            std::shared_ptr<T> get(const std::string& path)
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

            bool contains(const std::string& path) const
            {
                return _assets.find(path) != _assets.end();
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

        template <typename T>
        std::shared_ptr<T> get(const std::string& path)
        {
            auto type_index = std::type_index(typeid(T));
            auto iterator = _stores.find(type_index);
            if (iterator != _stores.end())
            {
                auto store = static_cast<_AssetStore<T>*>(iterator->second.get());
                return (store->get(path));
            }
            else
            {
                auto store = std::make_unique<_AssetStore<T>>();
                auto asset = store->get(path);
                _stores[type_index] = std::move(store);
                return (asset);
            }
        }

        template <typename T>
        void unload(const std::string& path)
        {
            auto type_index = std::type_index(typeid(T));
            auto iterator = _stores.find(type_index);
            if (iterator != _stores.end())
            {
                auto store = static_cast<_AssetStore<T>*>(iterator->second.get());
                store->unload(path);
            }
        }

        template <typename T>
        bool is_loaded(const std::string& path)
        {
            auto type_index = std::type_index(typeid(T));
            auto iterator = _stores.find(type_index);
            if (iterator != _stores.end())
            {
                auto store = static_cast<_AssetStore<T>*>(iterator->second.get());
                return (store->contains(path));
            }
            return (false);
        }

        template <typename T>
        void clear()
        {
            auto type_index = std::type_index(typeid(T));
            auto iterator = _stores.find(type_index);
            if (iterator != _stores.end())
            {
                auto store = static_cast<_AssetStore<T>*>(iterator->second.get());
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