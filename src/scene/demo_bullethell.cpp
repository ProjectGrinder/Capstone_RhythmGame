#include "audio.h"
#include "scene.h"
#include "system.h"

Scene::DemoBulletHell Scene::DemoBulletHell::instance()
{
    static DemoBulletHell instance;
    return (instance);
}

std::shared_ptr<Scene::DemoBulletHell::TaskManager> Scene::DemoBulletHell::init()
{
    auto tm = std::make_shared<TaskManager>();
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
        create_bullet_data(),
        create_pattern_container(),
        Game::Audio::init_sounds(),
        Game::Rhythm::KeyInput(),
        Game::BulletHell::Input()
        );


    auto hit_sound = load_audio("audio/fishdam1", "player_hit");
    AudioCache *out = nullptr;
    load_audio_if_not_exist((AssetsRecord *) hit_sound, &out);
    tm->create_entity<Game::BulletHell::Player,
    Position, Rotation,
    Game::Physics::Scale,
    Velocity,
    Acceleration,
    AngularVelocity, Game::Physics::CircularCollider>(
        {}, {}, {}, {}, {}, {}, {}, {}
    );
    tm->run_all();
    return (tm);
}



std::vector<Scene::DemoBulletHell::ComponentTuple> Scene::DemoBulletHell::exit()
{
    LOG_INFO("Exiting DemoBulletHell Scene.");
    return {};
}