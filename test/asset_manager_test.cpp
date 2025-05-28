#include "pch.h"
#include "system.h"
#include <string>

using System::AssetManager;

struct DummyAsset 
{
    std::string name;
    DummyAsset(const std::string& n) : name(n) {}
};

// Override the load method temporarily by specializing _AssetStore
namespace System 
{
    template <>
    std::shared_ptr<DummyAsset> AssetManager::_AssetStore<DummyAsset>::load(const std::string& path) 
    {
        auto asset = std::make_shared<DummyAsset>(path);
        _assets[path] = asset;
        return (asset);
    }
}

class AssetManagerTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        AssetManager::instance().clear_all();
    }

    void TearDown() override 
    {
        AssetManager::instance().clear_all();
    }
};

TEST_F(AssetManagerTest, get_asset_test) 
{
    auto asset = AssetManager::instance().get<DummyAsset>("test_asset");
    ASSERT_NE(asset, nullptr);
    EXPECT_EQ(asset->name, "test_asset");
}

TEST_F(AssetManagerTest, get_same_asset_test) 
{
    auto asset1 = AssetManager::instance().get<DummyAsset>("test_asset");
    auto asset2 = AssetManager::instance().get<DummyAsset>("test_asset");
    ASSERT_EQ(asset1, asset2);
}

TEST_F(AssetManagerTest, unload_asset_test) {
    auto asset = AssetManager::instance().get<DummyAsset>("temp_path");
    AssetManager::instance().unload<DummyAsset>("temp_path");

    EXPECT_FALSE(AssetManager::instance().is_loaded<DummyAsset>("temp_path"));
}



