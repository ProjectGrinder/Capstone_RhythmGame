#include "scene.h"

namespace Scene
{
    SceneManager &SceneManager::instance()
    {
        static SceneManager instance;
        return (instance);
    }

    void SceneManager::init(System::Renderer::VertexGeneratorQueue<System::Config::VertexQueueSize> *queue)
    {
        instance()._generator_queue = queue;
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

                        if (instance()._generator_queue == nullptr)
                        {
                            LOG_DEBUG(
                                    "Warn: Vertex queue generator didn't get initialize. Please check if you have "
                                    "called init "
                                    "function");
                            return;
                        }
                        instance()._generator_queue->write(manager.get_rm());
                    }
                    else
                    {
                        LOG_DEBUG("Warn: No scene loaded during update");
                    }
                },
                instance()._current_scene_template);
    }

    void SceneManager::clean_up()
    {
        LOG_DEBUG("Info: Cleaning up...");
        auto &scene_template = instance()._current_scene_template;
        instance()._current_manager.reset();
        std::visit(
                []<typename S>(S &&scene)
                {
                    if constexpr (!std::is_same_v<std::decay_t<S>, std::monostate>)
                    {
                        scene.Exit();
                    }
                },
                scene_template);
    }
} // namespace Scene
