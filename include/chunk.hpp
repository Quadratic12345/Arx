#pragma once
#include <cstddef>
#include<memory>


// the chunk is responsible for acquiring a block of memory
// it remembers hwo much memory it owns how much memory can be consumed
namespace Arx{
   class Chunk{
       public:
           explicit Chunk(std::size_t, capacity);
           ~Chunk()=default;

           Chunk(const Chunk&)=delete;
           Chunk& operator=(const Chunk&)=delete;

           void allocate(std::size_t size, std::size_t alignment);

           void reset() noexcept;
           std::size_t used() const noexcept;
           std::size_t capacity() const noexcept;
           std::size_t remaining() const noexcept;
       private:
           std::unique_ptr<std::byte[]> memory_;
           std::size_t capacity_;
           std::size_t offset_=0;
   };
}
