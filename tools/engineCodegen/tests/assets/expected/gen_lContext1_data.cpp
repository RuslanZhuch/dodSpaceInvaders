struct Data
{
    [[nodiscard]] static Data load(Dod::MemPool& pool, int32_t& header) noexcept;
    void reset() noexcept;
    void merge(const Data& other) noexcept;

    int32_t var1{ 1 };
    float var2{ 1.1 };
    CustomData1 var3;
    Dod::DBBuffer<float> dbvar1;
    Dod::DBBuffer<int64_t> dbvar2;
};
