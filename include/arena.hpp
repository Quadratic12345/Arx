#pragma once
#include "chunk.hpp"
#include<algorithm>
#include<cstddef>
#include<memory>
#include<vector>

namespace Arx{
    class Arena{
        public:
            explicit Arena(std::size_t chunk_size=64*1024);
            Arena(const Arena&)=delete;
            Arena& operator=(const Arena&)=delete;
            void *allocate(
                std::size_t size,
            std::size_t alignment=alignof(std::max_align_t)
            );
            void reset() noexcept;
            std::size_t bytes_used() const noexcept;
            std::size_t bytes_reserved() const noexcept;

        private:
            std::vector<std::unique_ptr<Chunk>> chunks_;
            std::size_t chunk_size_;
            std::size_t current_chunk_=0;
            std::size_t total_used_=0;
            std::size_t peak_used_=0;

            Chunk& add_chunk(std::size_t minimum_capacity);
    };
}
