class Test
{
public:

private:

public:
    Game::Context::Type1::Local local1Context;
    Game::Context::Type2::Local local2Context;
    Game::Context::Type2::Local local3Context;
    Game::Context::Type1::Shared target1Context;
    Game::Context::Type3::Shared target2Context;
    Game::Context::Type3::Shared target3Context;
    const Dod::SharedContext::Controller<Game::Context::Type4::Shared>* shared1Context{ nullptr };
    const Dod::SharedContext::Controller<Game::Context::Type4::Shared>* shared2Context{ nullptr };
    const Dod::SharedContext::Controller<Game::Context::Type5::Shared>* shared3Context{ nullptr };
};
