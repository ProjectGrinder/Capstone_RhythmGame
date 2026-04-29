#include "scene.h"
#include "system.h"

void init_graphics(const std::shared_ptr<Scene::DemoBulletHell::TaskManager>& tm)
{
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1280, .scaleY = 720, .rotation = 0});

    InputAttributeDescription sprite_vs_input_attributes[] = {
        InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
        InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12},
        InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 20}};

    InputAttributeDescription sprite_ps_input_attributes[] = {
        InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
        InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16},
        InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 24}};

    load_vertex_shader("shaders/vs/sprite.cso", "sprite_vs", sprite_vs_input_attributes, 3);

    load_pixel_shader("shaders/ps/sprite.cso", "sprite_ps", sprite_ps_input_attributes, 3);

    load_sprite("img/Default_Shot.dds", "bullet_sprite", 512, 512);
    load_sprite("img/test.dds", "test", 500, 500);

    load_sprite("img/Square.dds", "Square", 20, 20);
    load_sprite("img/Square20px.dds", "Square20px", 20, 20);
}

Game::Battle::BulletRegistry Scene::init_bullet_graphic()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    std::vector<BulletGraphicMap> maps = {
        BulletGraphicMap(ColliderData(CIRCLE, 6), GraphicData( 0, 0, 12, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 6), GraphicData( 12, 0, 24, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 6), GraphicData( 24, 0, 36, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 6), GraphicData( 36, 0, 48, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 6), GraphicData( 48, 0, 60, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 6), GraphicData( 60, 0, 72, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 6), GraphicData( 72, 0, 84, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 6), GraphicData( 84, 0, 96, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 0, 12, 17, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 18, 12, 35, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 36, 12, 53, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 54, 12, 71, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 72, 12, 89, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 90, 12, 107, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 108, 12, 125, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 126, 12, 143, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 256, 332, 273, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 274, 332, 291, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 292, 332, 309, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 310, 332, 327, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 328, 332, 345, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 346, 332, 363, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 364, 332, 381, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 382, 332, 399, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 0, 30, 19, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 20, 30, 39, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 40, 30, 59, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 60, 30, 79, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 80, 30, 99, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 100, 30, 119, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 120, 30, 139, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 140, 30, 159, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 0, 50, 29, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 30, 50, 59, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 60, 50, 89, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 90, 50, 119, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 120, 50, 149, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 150, 50, 179, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 180, 50, 209, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 210, 50, 239, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 256, 302, 285, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 286, 302, 315, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 316, 302, 345, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 346, 302, 375, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 376, 302, 405, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 406, 302, 435, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 436, 302, 465, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 466, 302, 495, 331 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 4), GraphicData( 0, 80, 9, 100 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 4), GraphicData( 10, 80, 19, 100 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 4), GraphicData( 20, 80, 29, 100 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 4), GraphicData( 30, 80, 39, 100 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 4), GraphicData( 40, 80, 49, 100 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 4), GraphicData( 50, 80, 59, 100 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 4), GraphicData( 60, 80, 69, 100 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 4), GraphicData( 70, 80, 79, 100 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 0, 100, 11, 118 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 12, 100, 23, 118 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 24, 100, 35, 118 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 36, 100, 47, 118 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 48, 100, 59, 118 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 60, 100, 71, 118 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 72, 100, 83, 118 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 84, 100, 95, 118 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 0, 118, 11, 138 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 12, 118, 23, 138 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 24, 118, 35, 138 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 36, 118, 47, 138 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 48, 118, 59, 138 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 60, 118, 71, 138 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 72, 118, 83, 138 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 84, 118, 95, 138 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 0, 138, 11, 158 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 12, 138, 23, 158 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 24, 138, 35, 158 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 36, 138, 47, 158 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 48, 138, 59, 158 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 60, 138, 71, 158 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 72, 138, 83, 158 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 5), GraphicData( 84, 138, 93, 158 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 8), GraphicData( 0, 158, 17, 186 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 8), GraphicData( 18, 158, 35, 186 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 8), GraphicData( 36, 158, 53, 186 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 8), GraphicData( 54, 158, 71, 186 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 8), GraphicData( 72, 158, 89, 186 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 8), GraphicData( 90, 158, 107, 186 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 8), GraphicData( 108, 158, 125, 186 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 8), GraphicData( 126, 158, 143, 186 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 6), GraphicData( 0, 186, 13, 208 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 6), GraphicData( 14, 186, 27, 208 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 6), GraphicData( 28, 186, 41, 208 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 6), GraphicData( 42, 186, 55, 208 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 6), GraphicData( 56, 186, 69, 208 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 6), GraphicData( 70, 186, 83, 208 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 6), GraphicData( 84, 186, 97, 208 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 6), GraphicData( 98, 186, 111, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 0, 209, 17, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 18, 209, 35, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 36, 209, 53, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 54, 209, 71, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 72, 209, 89, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 90, 209, 107, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 108, 209, 125, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 126, 209, 143, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 0, 493, 17, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 18, 493, 35, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 36, 493, 53, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 54, 493, 71, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 72, 493, 89, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 90, 493, 107, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 108, 493, 125, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 7), GraphicData( 126, 493, 143, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 0, 224, 16, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 16, 224, 32, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 32, 224, 48, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 48, 224, 64, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 64, 224, 80, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 80, 224, 96, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 96, 224, 112, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 8), GraphicData( 112, 224, 128, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 0, 242, 19, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 20, 242, 39, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 40, 242, 59, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 60, 242, 79, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 80, 242, 99, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 100, 242, 119, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 120, 242, 139, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 140, 242, 159, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 0, 260, 30, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 30, 260, 60, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 60, 260, 90, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 90, 260, 120, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 120, 260, 150, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 150, 260, 180, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 180, 260, 210, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 210, 260, 240, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 0, 290, 27, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 28, 290, 55, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 56, 290, 83, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 84, 290, 111, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 112, 290, 139, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 140, 290, 167, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 168, 290, 195, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 196, 290, 223, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 0, 318, 19, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 20, 318, 39, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 40, 318, 59, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 60, 318, 79, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 80, 318, 99, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 100, 318, 119, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 120, 318, 139, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 140, 318, 159, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 0, 338, 32, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 32, 338, 64, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 64, 338, 96, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 96, 338, 128, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 128, 338, 160, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 160, 338, 192, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 192, 338, 224, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 225, 339, 256, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 0, 370, 22, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 22, 370, 44, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 44, 370, 66, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 66, 370, 88, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 88, 370, 110, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 110, 370, 132, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 132, 370, 154, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 154, 370, 176, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 0, 398, 22, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 22, 398, 44, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 44, 398, 66, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 66, 398, 88, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 88, 398, 110, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 110, 398, 132, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 132, 398, 154, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 154, 398, 176, 430 )),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 1, 431, 25, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 27, 431, 51, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 53, 431, 77, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 79, 431, 103, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 105, 431, 129, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 131, 431, 155, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 157, 431, 181, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 183, 431, 207, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(RECTANGLE, 12, 14), GraphicData( 209, 431, 233, 459 ), SpecialBulletData(Laser,100,200), 1,999,1500),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 320, 0, 384, 64 ), SpecialBulletData(Booming,3,200), 1,999,1500),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 448, 64, 512, 128 ), SpecialBulletData(Booming,3,200), 1,999,1500),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 320, 64, 384, 128 ), SpecialBulletData(Booming,3,200), 1,999,1500),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 384, 0, 448, 64 ), SpecialBulletData(Booming,3,200), 1,999,1500),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 384, 64, 448, 128 ), SpecialBulletData(Booming,3,200), 1,999,1500),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 448, 0, 512, 64 ), SpecialBulletData(Booming,3,200), 1,999,1500),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 256, 64, 320, 128 ), SpecialBulletData(Booming,3,200), 1,999,1500),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 256, 0, 320, 64 ), SpecialBulletData(Booming,3,200), 1,999,1500),
    };
    return {BulletRegistry(maps)};
}

Game::Battle::PatternContainer Scene::create_pattern_container()
{
    using namespace Game::Battle;
    const std::vector<PatternStep> demo_step = {
        PatternStep(3000, OP_ADD, 2, 15),  // 3s Rot+15
        PatternStep(3000, OP_ADD, 2, -15), // 3s Rot-15
        PatternStep(3000, OP_SET, 1, 3),   // 3s Vel=3
};
    const std::vector<PatternSequence> demo_pattern = {
        PatternSequence(false),
        PatternSequence(false, 0, 2),
        PatternSequence(false, 1, 2),
        PatternSequence(true, 0, 2),
        PatternSequence(true, 1, 2),
    };
    auto demo_pattern_container = PatternContainer(demo_step,demo_pattern);
    return { PatternContainer(demo_pattern_container) };
}

Game::Battle::BulletLoader Scene::create_bullet_data()
    {
        using namespace Game::Battle;
        using namespace Game::Physics;

        int left_padding = 2500;
        Math::Point boss_pos = {0,240};

        BulletLoader loader;
        loader.CreateBullet(left_padding, BulletData(5, 5, 1,135, 100, 0));
        loader.CreateBullet(left_padding + 200, BulletData(5, 5,0.9f, 120 ,  100, 0));
        loader.CreateBullet(left_padding + 400, BulletData(5, 5,0.8f, 105, 100, 0));
        loader.CreateBullet(left_padding + 600, BulletData(5, 5,0.9f, 150, 100, 0));
        loader.CreateBullet(left_padding + 800, BulletData(5, 5, 0.8f, 165, 100, 0));

        loader.CreateBullet(left_padding + 3000, BulletData(10, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 3500, BulletData(20, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 4000, BulletData(30, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 4500, BulletData(40, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 5000, BulletData(50, 5, 1,135, 1, 0, 1));

        loader.CreateBullet(left_padding + 3000, BulletData(50, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 3500, BulletData(40, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 4000, BulletData(30, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 4500, BulletData(20, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 5000, BulletData(10, 5, 1,135, 2, 0, 1));

        loader.CreateBullet(left_padding + 7000, BulletData(25, 25, 0, 3));
        loader.CreateBullet(left_padding + 7500, BulletData(50, 50, 0, 3));
        loader.CreateBullet(left_padding + 8000, BulletData(75, 75, 0, 3));

        loader.CreateBullet(left_padding + 10000, BulletData(150, 25, 0, 3));
        loader.CreateBullet(left_padding + 10500, BulletData(125, 50, 0, 3));
        loader.CreateBullet(left_padding + 11000, BulletData(100, 75, 0, 3));

        loader.CreateBullet(left_padding + 12000, BulletData(25, 25, 150, 4));
        loader.CreateBullet(left_padding + 12500, BulletData(50, 50, 150, 4));
        loader.CreateBullet(left_padding + 13000, BulletData(75, 75, 150, 4));

        loader.CreateBullet(left_padding + 14000, BulletData(150, 25, 150, 4));
        loader.CreateBullet(left_padding + 14500, BulletData(125, 50, 150, 4));
        loader.CreateBullet(left_padding + 15000, BulletData(100, 75, 150, 4));
        return loader;
    }

Game::Battle::BulletLoader Scene::create_bullet_data2()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;
    for (int i=0;i<500;i++)
    {
        for (int j=0;j<8;j++)
        {
            loader.CreateBullet(1000 + i*100, BulletData(0, 0, 100, (static_cast<float>(j) *45) + i*6,50,-45.f, 0, (i*8 + j)%175));
        }
    }

    return loader;
}

Game::Battle::BulletLoader Scene::create_bullet_data_boom_test()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;
    for (int i=0;i<20;i++)
    {
        for (int j=0;j<4;j++)
        {
            loader.CreateBullet(1000 + i*3000 + j*100, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
        }
    }

    return loader;
}

Game::Battle::BulletLoader Scene::create_bullet_data_laser_test()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;
    for (int i=0;i<20;i++)
    {
        loader.CreateBullet(1000 + i*3000, BulletData(rand_float(-500,500), -300, 0, rand_float(-135,-45), 1000, 176));
        loader.CreateBullet(1000 + i*3000 + 250, BulletData(-500, rand_float(-300,300), 0, rand_float(-45,45), 1000, 177));
        loader.CreateBullet(1000 + i*3000 + 500, BulletData(rand_float(-500,500), 300, 0, rand_float(45,135), 0, 1000, 178));
        loader.CreateBullet(1000 + i*3000 + 750, BulletData(500, rand_float(-300,300), 0, rand_float(-225,-135), 1000, 179));
    }

    return loader;
}

Game::Battle::BulletLoader Scene::create_bullet_collision_test()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;

    for (int i=0;i<500;i++)
    {
        for (int j=0;j<8;j++)
        {
            loader.CreateBullet(1000 + i*250 + j*50, BulletData(0, 0, 100, (static_cast<float>(j) *45) + i*6,50,-45.f, 0, (i*8 + j)%175));
        }
    }

    for (int i=0;i<8;i++)
    {
        for (int j=0;j<4;j++)
        {
            loader.CreateBullet(127000 + i*2000 + j*100, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
        }
    }

    for (int i=0;i<10;i++)
    {
        loader.CreateBullet(150000 + i*3000, BulletData(rand_float(-500,500), -300, 0, rand_float(-135,-45), 1000, 176));
        loader.CreateBullet(150000 + i*3000 + 250, BulletData(-500, rand_float(-300,300), 0, rand_float(-45,45), 1000, 177));
        loader.CreateBullet(150000 + i*3000 + 500, BulletData(rand_float(-500,500), 300, 0, rand_float(45,135), 0, 1000, 178));
        loader.CreateBullet(150000 + i*3000 + 750, BulletData(500, rand_float(-300,300), 0, rand_float(-225,-135), 1000, 179));
    }

    return loader;
}

Scene::DemoBulletHell Scene::DemoBulletHell::instance()
{
    static DemoBulletHell instance;
    return (instance);
}

std::shared_ptr<Scene::DemoBulletHell::TaskManager> Scene::DemoBulletHell::init()
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1280, .scaleY = 720, .rotation = 0});

    init_graphics(tm);

    // Create and configure BattleState
    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::BulletHellState,
    Game::Battle::BulletRegistry,
    Game::Battle::BulletLoader,
    Game::Battle::PatternContainer,
    Game::Audio::SoundRegistry,
    Game::Rhythm::KeyInput, Game::BulletHell::Input>
    (
        Game::Battle::BattleState(100, 100, Game::Battle::Difficulty()),
        Game::Battle::BulletHellState(10),
        init_bullet_graphic(),
        create_bullet_collision_test(),
        create_pattern_container(),
        Game::Audio::init_sounds(),
        Game::Rhythm::KeyInput(),
        Game::BulletHell::Input()
        );


    auto hit_sound = load_audio("audio/fishdam1", "player_hit");
    AudioCache *out = nullptr;
    load_audio_if_not_exist((AssetsRecord *) hit_sound, &out);
    tm->create_entity<Game::BulletHell::Player,
    Game::Render::Transform,
    Rotation,
    Velocity,
    Acceleration,
    AngularVelocity, Game::Physics::CircularCollider, Game::Render::Sprite, Game::Render::Material>(
        {}, Game::Render::Transform(0,-240), {}, {}, {},{},
        Game::Physics::CircularCollider(24),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("test")), .pos = {{-24, 24, 0}, {24, 24, 0}, {24, -24, 0}, {-24, -24, 0}}, .layer = 1},
        Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))}
    );
    auto font = load_font("fonts/Klub04TT-Normal.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");
    tm->create_entity(
           Game::Test::FpsCounter{},
           Game::Render::Text{.font = font, .text = "0"},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{-600, 300, 0}, 0, 0, 0});
    tm->create_entity(
           Game::Test::BulletCounter{},
           Game::Render::Text{.font = font, .text = "0"},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{400, 300, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Test::LifeText{},
           Game::Render::Text{.font = font, .text = "0"},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{-600, 200, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Test::GrazeText{},
           Game::Render::Text{.font = font, .text = "0"},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{400, 200, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::HpBarMax{},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square20px")), .pos = {{-55, 12.5, 0}, {55, 12.5, 0}, {55, -12.5, 0}, {-55, -12.5, 0}}, .layer = 6},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, 300, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::HpBar{},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-50, 10, 0}, {50, 10, 0}, {50, -10, 0}, {-50, -10, 0}}, .color = {0.2f,1,0.2f}, .layer = 5},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, 300, 0}, 0, 0, 0});
    tm->run_all();
    return (tm);
}

Scene::DemoBulletHell::ResourceManager Scene::DemoBulletHell::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoBulletHell Scene.");
    return ResourceManager();
}