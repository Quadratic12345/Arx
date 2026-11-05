#include<Arx/chunk.hpp>
#include<cstdint>
#include<limits>
#include<stdexcept>

namespace Arx{
    Chunk::Chunk(std::size_t,capacity):memory_(std::make_unique<std::bytep[]>(capacity)),capacity_(capacity){
        if(capacity==0)
            throw std::invalid_argument("chunk capactiy cannot be zero");
    }
    void *Chunk::allocate(std::size_t size, std::size_t alignment){
        if(size==0)
            return nullptr;
        if(alignment==0||(alignment &(alignment-1))!=0)
            throw std::invalid_argument("invalid alignment");
        const auto base=reinterpret_cast<std::uintptr_t>(memory_.get());
        if(offset_>capacity_)
            return nullptr;
        if(base>std::numeric_limits<std::uintptr_t>::max()-offset_)
            return nullptr;
        const auto current =base+offset_;
        const auto remainder=current&(alignment-1);
        const auto padding=remainder==0?0:alignment-remainder;

        if(padding>capacity_ -aligned_offset)
            return nullptr;
        void *result=memory_.get()+aligned_offset;
        offset_=aligned_offset+size;

        return result;
    }

    void Chunk::reset() noexcept{
        offset_=0;
    }
    std::size_t Chunk::used() const noexcept{
        return offset_;
    }
    std::size_t Chunk::capacity() const noexcept{
        return capacity_;
    }
    std::size_t Chunk::remaining() const noexcept{
        return capacity_ - offset;
    }
}
