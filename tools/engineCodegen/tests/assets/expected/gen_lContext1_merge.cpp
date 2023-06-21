
void Data::merge(const Shared& other) noexcept
{
    Dod::BufferUtils::append(this->dbvar1, Dod::BufferUtils::createImFromBuffer(other.dbvar1));
    Dod::BufferUtils::append(this->dbvar2, Dod::BufferUtils::createImFromBuffer(other.dbvar2));
}
