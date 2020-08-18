#include "allocator.h"

#include <cassert>
#include <cstdlib>

namespace ge
{
    Allocator::Ptr::Ptr
    (
        void* ptr,
        Allocator& allocator
    ) noexcept
        : ptr_{ptr}
        , allocator_{&allocator}
    {
        assert(ptr_ != nullptr);
    }

    Allocator::Ptr::~Ptr() noexcept
    {
        if (ptr_ != nullptr)
        {
            allocator_->deallocate(ptr_);
        }
    }

    Allocator::Ptr::Ptr(Ptr&& other) noexcept
        : ptr_{other.ptr_}
        , allocator_{other.allocator_}
    {
        other.ptr_ = nullptr;
    }

    auto Allocator::Ptr::operator=(Ptr&& other) noexcept -> Ptr&
    {
        if (this != &other)
        {
            Ptr{std::move(other)}.swap(*this);
        }

        return *this;
    }

    void Allocator::Ptr::swap(Ptr& other) noexcept
    {
        std::swap(ptr_, other.ptr_);
        std::swap(allocator_, other.allocator_);
    }

    void* Allocator::Ptr::get() const noexcept
    {
        return ptr_;
    }

    Allocator::Allocator() noexcept = default;

    auto Allocator::allocate(Bytes n) noexcept -> Result<Ptr, Errors<AllocationError>>
    {
        void* const result = std::malloc(n);

        if (result == nullptr)
        {
            return AllocationError{.required_size = n};
        }

        return Ptr(result, *this);
    }

    void Allocator::deallocate(void* ptr)
    {
        std::free(ptr);
    }
}
