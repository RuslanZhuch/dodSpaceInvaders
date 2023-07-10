class Test
{
public:

private:

public:
    Context::Type1::Data local1Context;
    Context::Type2::Data local2Context;
    Context::Type2::Data local3Context;
    Context::Type1::Data target1Context;
    Context::Type3::Data target2Context;
    Context::Type3::Data target3Context;
    const Dod::SharedContext::Controller<Context::Scontext1::Data>* shared1Context{ nullptr };
    const Dod::SharedContext::Controller<Context::Scontext1::Data>* shared2Context{ nullptr };
    const Dod::SharedContext::Controller<Context::Scontext2::Data>* shared3Context{ nullptr };
    const Dod::SharedContext::Controller<Context::Scontext2::Data>* shared4Context{ nullptr };
};
