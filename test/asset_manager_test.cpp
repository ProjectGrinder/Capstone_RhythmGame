#include "pch.h"
#include "system.h"
#include <string>
#include <utility>

struct DummyAsset 
{
    std::string name;
    explicit DummyAsset(std::string  n) : name(std::move(n)) {}
};

struct SecondDummyAsset
{
    std::string name;
    int value;
    explicit SecondDummyAsset(std::string  n) : name(std::move(n)), value(0) {}
};

using TestAssetManager = System::AssetManager<DummyAsset, SecondDummyAsset>;

// Override the load method temporarily by specializing _AssetStore
namespace System 
{
    template <>
    std::shared_ptr<DummyAsset> AssetStore<DummyAsset>::load(const std::string& path)
    {
        auto asset = std::make_shared<DummyAsset>(path);
        _assets[path] = asset;
        return (asset);
    }

    template <>
    std::shared_ptr<SecondDummyAsset> AssetStore<SecondDummyAsset>::load(const std::string& path)
    {
        auto asset = std::make_shared<SecondDummyAsset>(path);
        _assets[path] = asset;
        return (asset);
    }
}

TEST(AssetManager, get_asset_test)
{
    TestAssetManager instance;
    auto asset = instance.get<DummyAsset>("test_asset");
    ASSERT_NE(asset, nullptr);
    EXPECT_EQ(asset->name, "test_asset");
}

TEST(AssetManager, get_same_asset_test)
{
    TestAssetManager instance;
    auto asset1 = instance.get<DummyAsset>("test_asset");
    auto asset2 = instance.get<DummyAsset>("test_asset");
    ASSERT_EQ(asset1, asset2);
}

TEST(AssetManager, get_different_asset_test)
{
    TestAssetManager instance;
    auto asset1 = instance.get<DummyAsset>("asset_one");
    auto asset2 = instance.get<SecondDummyAsset>("asset_two");
    
    ASSERT_NE(asset1, nullptr);
    ASSERT_NE(asset2, nullptr);
    EXPECT_EQ(asset1->name, "asset_one");
    EXPECT_EQ(asset2->name, "asset_two");
}

TEST(AssetManager, get_two_asset_types_test)
{
    TestAssetManager instance;
    auto asset1 = instance.get<DummyAsset>("asset_one");
    auto asset2 = instance.get<SecondDummyAsset>("asset_two");
    ASSERT_NE(asset1, nullptr);
    ASSERT_NE(asset2, nullptr);
    EXPECT_EQ(asset1->name, "asset_one");
	EXPECT_EQ(asset2->name, "asset_two");
}

TEST(AssetManager, unload_asset_test)
{
    TestAssetManager instance;
    auto asset = instance.get<DummyAsset>("temp_path");
    instance.unload<DummyAsset>("temp_path");

    EXPECT_FALSE(instance.is_loaded<DummyAsset>("temp_path"));
}

TEST(AssetManager, is_loaded_test)
{
    TestAssetManager instance;
    instance.get<DummyAsset>("check_path");
    EXPECT_TRUE(instance.is_loaded<DummyAsset>("check_path"));
}

TEST(AssetManager, nonexistence_is_not_loaded_test)
{
    TestAssetManager instance;
    EXPECT_FALSE(instance.is_loaded<DummyAsset>("nonexistent_asset_path"));
}

TEST(AssetManager, clear_assetstore_test)
{
    TestAssetManager instance;
    instance.get<DummyAsset>("clear_path");
    instance.clear<DummyAsset>();
    
	EXPECT_FALSE(instance.is_loaded<DummyAsset>("clear_path"));
}

TEST(AssetManager, clear_assetstore_ignores_other_stores_test)
{
    TestAssetManager instance;
    instance.get<DummyAsset>("dummy_path");
    instance.get<SecondDummyAsset>("second_dummy_path");
    
    instance.clear<DummyAsset>();

    EXPECT_FALSE(instance.is_loaded<DummyAsset>("dummy_path"));
	EXPECT_TRUE(instance.is_loaded<SecondDummyAsset>("second_dummy_path"));
}

TEST(AssetManager, clear_all_assets_test)
{
    TestAssetManager instance;
    instance.get<DummyAsset>("clear_path");
    instance.clear_all();
    
    EXPECT_FALSE(instance.is_loaded<DummyAsset>("clear_path"));
}

