#include "pch.h"
#include "system.h"
#include <string>

using System::AssetManager;

struct DummyAsset 
{
    std::string name;
    DummyAsset(const std::string& n) : name(n) {}
};

struct SecondDummyAsset
{
    std::string name;
    int value;
    SecondDummyAsset(const std::string& n) : name(n), value(0) {}
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

    template <>
    std::shared_ptr<SecondDummyAsset> AssetManager::_AssetStore<SecondDummyAsset>::load(const std::string& path)
    {
        auto asset = std::make_shared<SecondDummyAsset>(path);
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

TEST_F(AssetManagerTest, get_different_asset_test) 
{
    auto asset1 = AssetManager::instance().get<DummyAsset>("asset_one");
    auto asset2 = AssetManager::instance().get<SecondDummyAsset>("asset_two");
    
    ASSERT_NE(asset1, nullptr);
    ASSERT_NE(asset2, nullptr);
    EXPECT_EQ(asset1->name, "asset_one");
    EXPECT_EQ(asset2->name, "asset_two");
}

TEST_F(AssetManagerTest, get_two_asset_types_test)
{
    auto asset1 = AssetManager::instance().get<DummyAsset>("asset_one");
    auto asset2 = AssetManager::instance().get<SecondDummyAsset>("asset_two");
    ASSERT_NE(asset1, nullptr);
    ASSERT_NE(asset2, nullptr);
    EXPECT_EQ(asset1->name, "asset_one");
	EXPECT_EQ(asset2->name, "asset_two");
}

TEST_F(AssetManagerTest, unload_asset_test) 
{
    auto asset = AssetManager::instance().get<DummyAsset>("temp_path");
    AssetManager::instance().unload<DummyAsset>("temp_path");

    EXPECT_FALSE(AssetManager::instance().is_loaded<DummyAsset>("temp_path"));
}

TEST_F(AssetManagerTest, is_loaded_test)
{
    AssetManager::instance().get<DummyAsset>("check_path");
    EXPECT_TRUE(AssetManager::instance().is_loaded<DummyAsset>("check_path"));
}

TEST_F(AssetManagerTest, nonexistence_is_not_loaded_test)
{
    EXPECT_FALSE(AssetManager::instance().is_loaded<DummyAsset>("nonexistent_asset_path"));
}

TEST_F(AssetManagerTest, clear_assetstore_test)
{
    AssetManager::instance().get<DummyAsset>("clear_path");
    AssetManager::instance().clear<DummyAsset>();
    
	EXPECT_FALSE(AssetManager::instance().is_loaded<DummyAsset>("clear_path"));
}

TEST_F(AssetManagerTest, clear_assetstore_ignores_other_stores_test)
{
    AssetManager::instance().get<DummyAsset>("dummy_path");
    AssetManager::instance().get<SecondDummyAsset>("second_dummy_path");
    
    AssetManager::instance().clear<DummyAsset>();
    
    EXPECT_FALSE(AssetManager::instance().is_loaded<DummyAsset>("dummy_path"));
	EXPECT_TRUE(AssetManager::instance().is_loaded<SecondDummyAsset>("second_dummy_path"));
}

TEST_F(AssetManagerTest, clear_all_assets_test) 
{
    AssetManager::instance().get<DummyAsset>("clear_path");
    AssetManager::instance().clear_all();
    
    EXPECT_FALSE(AssetManager::instance().is_loaded<DummyAsset>("clear_path"));
}

