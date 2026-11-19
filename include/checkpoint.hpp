#include<cstdint>
namespace Arx{
    class Arena;

    struct ArenaMark{
        const Arena* owner=nullptr;
        std::size_t chunk_index=0;

        std::size_t offset=0;
        std::uint64_t generation=0;
        std::uint64_t id=0;
    };
}
