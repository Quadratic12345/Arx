#include "Arx/arena.hpp"
#include "Arx/checkpoint.hpp"
namespace Arx{
    ArenaMark Arena::mark() const noexcept{
        return ArenaMark{
            this,current_chunk,chunk_[current_chunk]->used(),generation_,0
        };
    }
    bool Arena::rewind(const ArenaMark& checkpoint) noexcept{
        if(checkpoint.owner!=this){
            return false;
        }

        if(checkpoint.generation!=generation_){
            return false;
        }
        if(checkpoint.chunk_index>=chunks_.size()){
            return false;
        }
        Chunk& target=*chunks_[checkpoint.chunk_index];

        if(checkpoint.offset>target.used()){
            return false;
        }
        for(std::size_t i=checkpoint.chunk_index+1;i<chunks_.size();i++){
            chunks_[i]->reset();
        }

        if(!target.rewind_to(checkpoint.offset)){
            return false;
        }

        current_chunk_=checkpoint.chunk_index;
        total_used_=0;
        for(const auto& chunk: chunks_){
            total_used_+=chunk->used();
        }
        return true;
    }
}
