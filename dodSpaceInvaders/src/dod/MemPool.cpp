#include "MemPool.h"

Dod::MemPool::MemPool(MemTypes::capacity_t capacityInBytes) noexcept
{
	this->data.resize(static_cast<size_t>(capacityInBytes));
	this->dataBegin = this->data.data();
	this->dataEnd = this->data.data() + this->data.size();
}
