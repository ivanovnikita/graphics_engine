#pragma once

#include "result.hpp"

#include "string_view"

namespace ge
{
    class Allocator final
    {
    public:
        using Bytes = size_t;

        class Ptr final
        {
            friend class Allocator;
        public:
            ~Ptr() noexcept;

            Ptr(Ptr&&) noexcept;
            Ptr& operator=(Ptr&&) noexcept;

            Ptr(const Ptr&) = delete;
            Ptr& operator=(const Ptr&) = delete;

            void swap(Ptr&) noexcept;

            void* get() const noexcept;

        private:
            explicit Ptr(void*, Allocator&) noexcept;

            void* ptr_;
            Allocator* allocator_;
        };

        explicit Allocator() noexcept;

        Allocator(const Allocator&) = delete;
        Allocator& operator=(const Allocator&) = delete;

        Allocator(Allocator&&) = delete;
        Allocator& operator=(Allocator&&) = delete;

        struct AllocationError final
        {
            Bytes required_size;
            static constexpr std::string_view error_message
            {
                "memory allocation failed"
            };
        };

        [[ nodiscard ]] Result<Ptr, Errors<AllocationError>> allocate(Bytes n) noexcept;

    private:
        void deallocate(void*);
    };
}
