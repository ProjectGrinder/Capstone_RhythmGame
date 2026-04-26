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
}

Game::Battle::BulletRegistry Scene::init_bullet_graphic()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    std::vector<BulletGraphicMap> maps = {
        BulletGraphicMap(ColliderData(CIRCLE, 12), GraphicData( 0, 0, 12, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 12), GraphicData( 12, 0, 24, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 12), GraphicData( 24, 0, 36, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 12), GraphicData( 36, 0, 48, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 12), GraphicData( 48, 0, 60, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 12), GraphicData( 60, 0, 72, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 12), GraphicData( 72, 0, 84, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 12), GraphicData( 84, 0, 96, 12 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 0, 12, 17, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 18, 12, 35, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 36, 12, 53, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 54, 12, 71, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 72, 12, 89, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 90, 12, 107, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 108, 12, 125, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 126, 12, 143, 29 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 256, 332, 273, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 274, 332, 291, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 292, 332, 309, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 310, 332, 327, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 328, 332, 345, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 346, 332, 363, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 364, 332, 381, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 382, 332, 399, 349 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 0, 30, 19, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 20, 30, 39, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 40, 30, 59, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 60, 30, 79, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 80, 30, 99, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 100, 30, 119, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 120, 30, 139, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 140, 30, 159, 49 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 0, 50, 29, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 30, 50, 59, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 60, 50, 89, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 90, 50, 119, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 120, 50, 149, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 150, 50, 179, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 180, 50, 209, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 210, 50, 239, 79 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 256, 302, 285, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 286, 302, 315, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 316, 302, 345, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 346, 302, 375, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 376, 302, 405, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 406, 302, 435, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 436, 302, 465, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 466, 302, 495, 331 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 0, 80, 9, 100 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 10, 80, 19, 100 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 20, 80, 29, 100 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 30, 80, 39, 100 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 40, 80, 49, 100 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 50, 80, 59, 100 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 60, 80, 69, 100 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 70, 80, 79, 100 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 0, 100, 11, 118 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 12, 100, 23, 118 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 24, 100, 35, 118 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 36, 100, 47, 118 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 48, 100, 59, 118 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 60, 100, 71, 118 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 72, 100, 83, 118 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 84, 100, 95, 118 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 0, 118, 11, 138 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 12, 118, 23, 138 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 24, 118, 35, 138 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 36, 118, 47, 138 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 48, 118, 59, 138 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 60, 118, 71, 138 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 72, 118, 83, 138 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 84, 118, 95, 138 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 0, 138, 11, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 12, 138, 23, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 24, 138, 35, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 36, 138, 47, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 48, 138, 59, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 60, 138, 71, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 72, 138, 83, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 84, 138, 93, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 0, 158, 17, 186 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 18, 158, 35, 186 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 36, 158, 53, 186 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 54, 158, 71, 186 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 72, 158, 89, 186 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 90, 158, 107, 186 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 108, 158, 125, 186 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 126, 158, 143, 186 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 0, 186, 13, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 14, 186, 27, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 28, 186, 41, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 42, 186, 55, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 56, 186, 69, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 70, 186, 83, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 84, 186, 97, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 13), GraphicData( 98, 186, 111, 208 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 0, 209, 17, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 18, 209, 35, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 36, 209, 53, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 54, 209, 71, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 72, 209, 89, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 90, 209, 107, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 108, 209, 125, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 126, 209, 143, 224 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 0, 493, 17, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 18, 493, 35, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 36, 493, 53, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 54, 493, 71, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 72, 493, 89, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 90, 493, 107, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 108, 493, 125, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 15), GraphicData( 126, 493, 143, 508 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 0, 224, 16, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 16, 224, 32, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 32, 224, 48, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 48, 224, 64, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 64, 224, 80, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 80, 224, 96, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 96, 224, 112, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 16), GraphicData( 112, 224, 128, 242 )),
        BulletGraphicMap(ColliderData(CIRCLE, 18), GraphicData( 0, 242, 19, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 18), GraphicData( 20, 242, 39, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 18), GraphicData( 40, 242, 59, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 18), GraphicData( 60, 242, 79, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 18), GraphicData( 80, 242, 99, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 18), GraphicData( 100, 242, 119, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 18), GraphicData( 120, 242, 139, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 18), GraphicData( 140, 242, 159, 260 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 0, 260, 30, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 30, 260, 60, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 60, 260, 90, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 90, 260, 120, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 120, 260, 150, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 150, 260, 180, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 180, 260, 210, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 210, 260, 240, 290 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 0, 290, 27, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 28, 290, 55, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 56, 290, 83, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 84, 290, 111, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 112, 290, 139, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 140, 290, 167, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 168, 290, 195, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 196, 290, 223, 317 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 0, 318, 19, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 20, 318, 39, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 40, 318, 59, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 60, 318, 79, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 80, 318, 99, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 100, 318, 119, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 120, 318, 139, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 140, 318, 159, 338 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 0, 338, 32, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 32, 338, 64, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 64, 338, 96, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 96, 338, 128, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 128, 338, 160, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 160, 338, 192, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 192, 338, 224, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 31), GraphicData( 225, 339, 256, 370 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 0, 370, 22, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 22, 370, 44, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 44, 370, 66, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 66, 370, 88, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 88, 370, 110, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 110, 370, 132, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 132, 370, 154, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 154, 370, 176, 398 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 0, 398, 22, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 22, 398, 44, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 44, 398, 66, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 66, 398, 88, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 88, 398, 110, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 110, 398, 132, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 132, 398, 154, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 22), GraphicData( 154, 398, 176, 430 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 1, 431, 25, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 27, 431, 51, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 53, 431, 77, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 79, 431, 103, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 105, 431, 129, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 131, 431, 155, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 157, 431, 181, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 183, 431, 207, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 209, 431, 233, 459 )),
        BulletGraphicMap(ColliderData(CIRCLE, 14), GraphicData( 120, 460, 134, 492 )),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 320, 0, 384, 64 )),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 448, 64, 512, 128 )),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 320, 64, 384, 128 )),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 384, 0, 448, 64 )),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 384, 64, 448, 128 )),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 448, 0, 512, 64 )),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 256, 64, 320, 128 )),
        BulletGraphicMap(ColliderData(CIRCLE, 64), GraphicData( 256, 0, 320, 64 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 256, 128, 273, 145 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 274, 128, 291, 145 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 292, 128, 309, 145 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 310, 128, 327, 145 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 256, 146, 275, 165 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 276, 146, 295, 165 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 296, 146, 315, 165 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 316, 146, 335, 165 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 256, 166, 285, 195 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 286, 166, 315, 195 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 316, 166, 345, 195 )),
        BulletGraphicMap(ColliderData(CIRCLE, 29), GraphicData( 346, 166, 375, 195 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 256, 196, 265, 216 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 266, 196, 275, 216 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 276, 196, 285, 216 )),
        BulletGraphicMap(ColliderData(CIRCLE, 9), GraphicData( 286, 196, 295, 216 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 256, 216, 267, 234 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 268, 216, 279, 234 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 280, 216, 291, 234 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 292, 216, 303, 234 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 256, 234, 267, 254 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 268, 234, 279, 254 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 280, 234, 291, 254 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 292, 234, 303, 254 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 256, 254, 267, 274 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 268, 254, 279, 274 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 280, 254, 291, 274 )),
        BulletGraphicMap(ColliderData(CIRCLE, 11), GraphicData( 292, 254, 303, 274 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 256, 274, 273, 302 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 274, 274, 291, 302 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 292, 274, 309, 302 )),
        BulletGraphicMap(ColliderData(CIRCLE, 17), GraphicData( 310, 274, 327, 302 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 384, 128, 414, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 414, 128, 444, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 444, 128, 474, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 30), GraphicData( 474, 128, 504, 158 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 384, 158, 411, 185 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 412, 158, 439, 185 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 440, 158, 467, 185 )),
        BulletGraphicMap(ColliderData(CIRCLE, 27), GraphicData( 468, 158, 495, 185 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 384, 186, 403, 206 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 404, 186, 423, 206 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 424, 186, 443, 206 )),
        BulletGraphicMap(ColliderData(CIRCLE, 19), GraphicData( 444, 186, 463, 206 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 384, 206, 416, 238 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 416, 206, 448, 238 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 448, 206, 480, 238 )),
        BulletGraphicMap(ColliderData(CIRCLE, 32), GraphicData( 480, 206, 512, 238 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 385, 239, 409, 267 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 411, 239, 435, 267 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 437, 239, 461, 267 )),
        BulletGraphicMap(ColliderData(CIRCLE, 24), GraphicData( 463, 239, 487, 267 )),
        BulletGraphicMap(ColliderData(CIRCLE, 31), GraphicData( 480, 384, 511, 512 )),
        BulletGraphicMap(ColliderData(CIRCLE, 94), GraphicData( 385, 417, 479, 511 )),
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

        int left_padding = 5000;

        BulletLoader loader;
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5, 1,135, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.9f, 120 ,  100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.8f, 105, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.9f, 150, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5, 0.8f, 165, 100, 0));

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
            loader.CreateBullet(1000 + i*100, BulletData(0, 0, 100, (static_cast<float>(j) *45) + i*6,50,-45.f, 1000, (i*8 + j)%240));
        }
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
        Game::Battle::BulletHellState(),
        init_bullet_graphic(),
        create_bullet_data2(),
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
        Game::Physics::CircularCollider(48),
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
    tm->run_all();
    return (tm);
}

Scene::DemoBulletHell::ResourceManager Scene::DemoBulletHell::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoBulletHell Scene.");
    return ResourceManager();
}