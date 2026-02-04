#include <cstdlib>
#include "scene.h"

namespace Scene
{
    /*
     * Very Unsafe Please Fix This
     * Didn't Protect Against Use Before Initialization and After Cleanup
     */
    typedef void *SceneManagerHandler;
    extern "C" SceneManagerHandler get_scene_manager();

    SceneManager &SceneManager::instance()
    {
        auto *instance = static_cast<SceneManager *>(get_scene_manager());
        if (instance == nullptr)
        {
            LOG_ERROR("SceneManager used before initialization or after cleanup");
            std::abort();
        }
        return (*instance);
    }

    void SceneManager::update()
    {
        std::visit(
                []<typename S>([[maybe_unused]] S &&scene)
                {
                    if constexpr (!std::is_same_v<std::decay_t<S>, std::monostate>)
                    {
                        using SceneType = std::decay_t<S>;
                        auto &manager = std::any_cast<typename SceneType::TaskManager &>(instance()._current_manager);
                        manager.run_all();
                    }
                    else
                    {
                        LOG_WARNING("No scene loaded during update");
                    }
                },
                instance()._current_scene_template);
    }

    SceneManager::~SceneManager()
    {
        LOG_INFO("Cleaning up SceneManager...");
        _current_manager.reset();
        std::visit(
                []<typename S>(S &&scene)
                {
                    if constexpr (!std::is_same_v<std::decay_t<S>, std::monostate>)
                    {
                        scene.exit();
                    }
                },
                _current_scene_template);
    }
} // namespace Scene
