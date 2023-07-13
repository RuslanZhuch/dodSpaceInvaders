#include "RenderExecutor.h"

#include <memory>

namespace Game::ExecutionBlock
{

    void Render::initImpl() noexcept
    {
        this->rendererContext.renderer = std::make_unique<Game::GameRenderer>(
            600,
            800,
            "Generated space invaders"
        );
    }

    void Render::updateImpl(float dt) noexcept
    {
        sf::Event event;

        auto& window{ this->rendererContext.renderer->getWindow() };
        while (window.pollEvent(event))
        {
//            Dod::BufferUtils::populate(this->applicationContext.commands, 1, event.type == sf::Event::Closed);
        }

        window.clear();

        window.display();
    }

}
