#include<Arx/arena.hpp>
#include<Algorithm>
#include<stdexcept>

namespace Arx{
    Arena::Arena(std::size_t chunk_size):chunk_size_(chunk_size){
        if(chunk_size==0)
            throw std::invalid_argument("the chunk size is invalid");
        add_chunk(chunk_size);
    }
    Chunk& Arena::add_chunk(std::size_t minimum_capacity){
        const std::size_t capacity=std::max(chunk_size_t,minimum_capacity);
        chunks_.push_back(std::make_unique<Chunk>(capacity));
        return *chunks_.back();
    }
    void *Arena::allocate(std::size_t size,std::size_t alignment){
        if(size==0)
            return nullptr;

        if(void *ptr=chunks_[current_chunk_]->allocate(size,alignment)){
            total_used_+=size;
            peak_used_=std::max(peak_used_,total_used_);
            return ptr;
        }
        for(std::size_t i=current_chunk_+1;i<chunks_.size();i++){
            if(void *ptr=chunks_[i]->allocate(size,alignment)){
                current_chunk_=i;
                total_used_+=size;
               peak_used_=std::max(peak_used_,total_used_);
              return ptr;
            }
        }
        if(size>static_cast<std::size_t>(-1)-alignment)
            throw std::bad_alloc();
        Chunk& chunk=add_chunk(size+alignment);
        current_chunk_=chunks_.size()-1;
        void *ptr=chunk.allocate(size,alignment);
        if(!ptr)
            throw std::bad_alloc();
        total_used_+=size;
        peak_used_=std::max(peak_used_,total_used_);
        return ptr;
    }

    void Arena::reset()noexcept{
        for(auto& chunk: chunks_)
        chunk->reset();
        current_chunk_=0;
        total_used_=0;
    }
    std::size_t Arena::bytes_used() const noexcept{
        return total_used_;
    }
    std::size_t Arena::bytes_reserved() const noexcept{
        std::size_t total=0;
        for(const auto& chunk:chunks_)
        total+=chunk->capacity();
        return total;
    }
}
