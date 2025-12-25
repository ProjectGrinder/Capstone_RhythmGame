#include "pch.h"
#include "system.h"
#include <string>

extern "C"
{
    typedef uint32_t assets_id;

    typedef enum
    {
        FLOAT32BITS,
        FLOAT16BITS,
        FLOAT8BITS,

        UINT32BITS,
        UINT16BITS,
        UINT8BITS,
    } InputType;

    typedef struct
    {
        const char *semantic;
        InputType type;
        size_t offset;
    } InputAttributeDescription;

    assets_id load_sprite(const char *path, const char *name, size_t width, size_t height);
    assets_id load_vertex_shader(const char *path, const char *name, InputAttributeDescription *attributes, size_t count);
    assets_id load_pixel_shader(const char *path, const char *name, InputAttributeDescription *attributes, size_t count);

    void free_assets(assets_id id);

    void assets_cleanup(void);
}

InputAttributeDescription dummyVS[] = {
    {"position",FLOAT32BITS,0},
    {"normal",FLOAT32BITS,4},
    {"texCoord",FLOAT32BITS,8}
};
InputAttributeDescription dummyPS[] = {
    {"fragColor",FLOAT32BITS,0},
};

TEST(AssetManager, get_null_assets)
{
    assets_id id = get_assets_id("testNull");
    EXPECT_EQ(id, -1);
}

TEST(AssetManager, load_sprite)
{
    assets_id idImg = load_sprite("./test.png","testImg",500,500);
    EXPECT_NE(idImg, -1);
    assets_id id = get_assets_id("testImg");
    EXPECT_NE(id, -1);
    EXPECT_EQ(id, idImg);
}

TEST(AssetManager, load_vertex_shader)
{
    assets_id idVS = load_vertex_shader("./shaders/vs/rainbow.hlsl","testVS",dummyVS, sizeof(dummyVS)/sizeof(dummyVS[0]));
    EXPECT_NE(idVS, -1);
    assets_id id = get_assets_id("testVS");
    EXPECT_EQ(id, idVS);
}

TEST(AssetManager, load_pixel_shader)
{
    assets_id idPS = load_pixel_shader("./shaders/ps/rainbow.hlsl","testPS",dummyPS, sizeof(dummyPS)/sizeof(dummyPS[0]));
    EXPECT_NE(idPS, -1);
    assets_id id = get_assets_id("testPS");
    EXPECT_EQ(id, idPS);
}

TEST(AssetManager, free_assets)
{
    assets_id idImg = load_sprite("./test.png","testImg",500,500);
    EXPECT_NE(idImg, -1);
    free_assets(idImg);
    idImg = get_assets_id("testImg");
    //EXPECT_EQ(get_assets_id("testImg"), -1);
}

TEST(AssetManager, assets_cleanup)
{
    load_sprite("./test.png","testImg",500,500);
    load_vertex_shader("./shaders/vs/rainbow.hlsl","testVS",dummyVS, sizeof(dummyVS)/sizeof(dummyVS[0]));
    load_pixel_shader("./shaders/ps/rainbow.hlsl","testPS",dummyPS, sizeof(dummyPS)/sizeof(dummyPS[0]));
    load_sprite("./test.png","testImg2",500,500);

    assets_cleanup();
    EXPECT_EQ(get_assets_id("testImg"), -1);
    EXPECT_EQ(get_assets_id("testVS"), -1);
    EXPECT_EQ(get_assets_id("testPS"), -1);
    EXPECT_EQ(get_assets_id("testImg2"), -1);
}