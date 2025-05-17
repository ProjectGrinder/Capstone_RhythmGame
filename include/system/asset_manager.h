#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>

namespace System
{
    class AssetManager
    {
    private:
        class _BaseAssetStore
        {
        public:
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
                // TODO:
                // load the asset from the path
                // store the asset in the map
                // create a shared_ptr from the asset and return
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
    };
}