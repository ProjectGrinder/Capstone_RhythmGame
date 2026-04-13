#include <cstdlib>
#include "scene.h"
#include "system/intent_storage.h"

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
        System::Render::IntentStorage::next_frame();

        std::visit(
                []<typename ManagerPtr>(ManagerPtr &manager)
                {
                    using ManagerPtrType = std::decay_t<ManagerPtr>;

                    if constexpr (std::is_same_v<ManagerPtrType, std::monostate>)
                    {
                        LOG_WARNING("No scene loaded during update");
                    }
                    else
                    {
                        if (manager)
                        {
                            manager->run_all();
                        }
                        else
                        {
                            LOG_WARNING("Current scene manager is null during update");
                        }
                    }
                },
                instance()._current_manager);

        // if _next_scene_template is not monostate, it is to be changed to.
        std::visit([]<typename S>([[maybe_unused]] S &&scene){
            if constexpr (!std::is_same_v<std::decay_t<S>, std::monostate>)
            {
                change_scene<std::decay_t<S>>();
                instance()._next_scene_template = std::monostate{};
            }
        }, instance()._next_scene_template);
    }

    SceneManager::~SceneManager()
    {
        LOG_INFO("Cleaning up SceneManager...");
        _current_scene_template = std::monostate{};
        _current_manager = std::monostate{};
    }
} // namespace Scene