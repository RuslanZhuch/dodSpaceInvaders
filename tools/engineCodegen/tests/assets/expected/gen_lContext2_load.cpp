
[[nodiscard]] Data Data::load(Dod::MemPool& pool, int32_t& header) noexcept
{

    Data data;

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/local/lContext2.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 4) };

    if (!inputDataOpt.has_value())
    {
        return data;
    }

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(data.var1, loadingDataArray, 0);
    Engine::ContextUtils::loadVariable(data.var2, loadingDataArray, 1);

    Engine::ContextUtils::loadBuffer(data.dbvar0, loadingDataArray, 2, pool, header);
    Engine::ContextUtils::loadBuffer(data.dbvar1, loadingDataArray, 3, pool, header);

    return data;

}
