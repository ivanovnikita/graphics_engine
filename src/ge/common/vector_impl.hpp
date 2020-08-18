#pragma once

#include "vector.hpp"
#include "construct.hpp"

namespace ge
{
    template <typename T>
    Vector<T>::Vector() noexcept
        : storage_{none}
        , capacity_{0}
        , size_{0}
    {
    }

    template <typename T>
    Vector<T>::Vector
    (
        Allocator::Ptr storage,
        const size_t capacity
    ) noexcept
        : storage_{std::move(storage)}
        , capacity_{capacity}
        , size_{0}
    {
    }

    template <typename T>
    Vector<T>::~Vector() noexcept
        requires
            std::is_nothrow_destructible_v<T>
    {
        free_all_memory();
    }

    template <typename T>
    Vector<T>::Vector(Vector&& other) noexcept
        : storage_{std::move(other.storage_)}
        , capacity_{other.capacity_}
        , size_{other.size_}
    {
        other.capacity_ = 0;
        other.size_ = 0;
        assert(other.storage_.is_none());
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
    {
        if (this != &other)
        {
            Vector<T>{std::move(other)}.swap(*this);
        }

        return *this;
    }

    template <typename T>
    void Vector<T>::swap(Vector& other) noexcept
    {
        std::swap(storage_, other.storage_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
    }

    template <typename T>
    [[ nodiscard ]] Result
    <
        Vector<T>,
        Errors<Allocator::AllocationError>
    > Vector<T>::create
    (
        Allocator& allocator,
        const size_t capacity
    ) noexcept
    {
        return allocator.allocate(capacity * sizeof(T))
            .then
            (
                [&capacity] (Allocator::Ptr&& memory) noexcept
                {
                    return Vector<T>(std::move(memory), capacity);
                }
            );
    }

    template <typename T>
    [[ nodiscard ]] Result
    <
        Vector<T>,
        Errors
        <
            Allocator::AllocationError,
            CopyFromError<T>
        >
    > Vector<T>::copy_from
    (
        Allocator& allocator,
        const Vector& other
    ) noexcept
        requires
            std::is_nothrow_move_constructible_v<T> and
            (CopyableFrom<T> or std::is_nothrow_copy_constructible_v<T>)
    {
        using R = Result
        <
            Vector<T>,
            Errors
            <
                Allocator::AllocationError,
                CopyFromError<T>
            >
        >;

        if (other.size() == 0)
        {
            return Vector<T>{};
        }

        return allocator.allocate(other.size() * sizeof(T))
            .then
            (
                [&other] (const Allocator::Ptr&& storage) noexcept -> R
                {
                    Vector<T> result{std::move(storage), other.size()};

                    if constexpr (std::is_nothrow_copy_constructible_v<T>)
                    {
                        other.for_each
                        (
                            [&result] (const T& v) noexcept
                            {
                                [[ maybe_unused ]] auto error = result.push_back_to_reserved(v);
                                assert(error.is_none());
                            }
                        );
                    }
                    else if constexpr (CopyableFrom<T>)
                    {
                        Option<CopyFromError<T>> copy_error;
                        other.for_each_until
                        (
                            [&result, &copy_error] (const T& v) noexcept
                            {
                                return T::copy_from(v)
                                    .match
                                    (
                                        [&result] (T&& new_value) noexcept
                                        {
                                            [[ maybe_unused ]] auto error = result.push_back_to_reserved
                                            (
                                                std::move(new_value)
                                            );
                                            assert(error.is_none());
                                            return true;
                                        },
                                        [&copy_error] (auto&& e) noexcept
                                        {
                                            copy_error = std::move(e);
                                            return false;
                                        }
                                    );
                            }
                        );
                    }

                    assert(result.size() == other.size());
                    return result;
                }
            );
    }

    template <typename T>
    [[ nodiscard ]] auto Vector<T>::operator[](const size_t ind) noexcept
        -> Result
        <
            Ref<T>,
            Errors<IndexError>
        >
    {
        using R = Result<Ref<T>, Errors<IndexError>>;

        return storage_.match
        (
            [this, &ind] (const Allocator::Ptr& storage) noexcept -> R
            {
                if (ind >= size_)
                {
                    return IndexError
                    {
                        .requested_index = ind,
                        .current_size = size_
                    };
                }

                T* memory = reinterpret_cast<T*>(storage.get());
                return memory[ind];
            },
            [&ind] () noexcept -> R
            {
                return IndexError
                {
                    .requested_index = ind,
                    .current_size = 0
                };
            }
        );
    }

    template <typename T>
    [[ nodiscard ]] auto Vector<T>::operator[](const size_t ind) const noexcept
        -> Result
        <
            Ref<const T>,
            Errors<IndexError>
        >
    {
        using R = Result<Ref<const T>, Errors<IndexError>>;

        return storage_.match
        (
            [this, &ind] (const Allocator::Ptr& storage) noexcept -> R
            {
                if (ind >= size_)
                {
                    return IndexError
                    {
                        .requested_index = ind,
                        .current_size = size_
                    };
                }

                T* memory = reinterpret_cast<T*>(storage.get());
                return memory[ind];
            },
            [&ind] () noexcept -> R
            {
                return IndexError
                {
                    .requested_index = ind,
                    .current_size = 0
                };
            }
        );
    }

    template <typename T>
    template <size_t N>
    [[ nodiscard ]] auto Vector<T>::view(const size_t ind_from) noexcept
        -> Result
        <
            View<T, N>,
            Errors<ViewError>
        >
    {
        using R = Result<View<T, N>, Errors<ViewError>>;

        return storage_.match
        (
            [this, &ind_from] (const Allocator::Ptr& storage) noexcept -> R
            {
                T* memory = reinterpret_cast<T*>(storage.get());
                return View<T, N>::create
                (
                    memory,
                    memory + size_,
                    memory + ind_from,
                    memory + ind_from + N
                ).on_error
                (
                    [this, ind_from] (auto&& /*errors*/) noexcept
                    {
                        return IndexError
                        {
                            .requested_index = ind_from + N - 1,
                            .current_size = size_
                        };
                    }
                );
            },
            [&ind_from] () noexcept -> R
            {
                return IndexError
                {
                    .requested_index = ind_from,
                    .current_size = 0
                };
            }
        );
    }

    template <typename T>
    template <size_t N>
    [[ nodiscard ]] auto Vector<T>::view(const size_t ind_from) const noexcept
        -> Result
        <
            View<const T, N>,
            Errors<ViewError>
        >
    {
        using R = Result<View<const T, N>, Errors<ViewError>>;

        return storage_.match
        (
            [this, &ind_from] (const Allocator::Ptr& storage) noexcept -> R
            {
                T* memory = reinterpret_cast<T*>(storage.get());
                return View<T, N>::create
                (
                    memory,
                    memory + size_,
                    memory + ind_from,
                    memory + ind_from + N
                ).on_error
                (
                    [this, ind_from] (auto&& /*errors*/) noexcept
                    {
                        return IndexError
                        {
                            .requested_index = ind_from + N - 1,
                            .current_size = size_
                        };
                    }
                );
            },
            [&ind_from] () noexcept -> R
            {
                return IndexError
                {
                    .requested_index = ind_from,
                    .current_size = 0
                };
            }
        );
    }

    template <typename T>
    [[ nodiscard ]] Option<Errors<Allocator::AllocationError>> Vector<T>::reserve
    (
        Allocator& allocator,
        const size_t count
    ) noexcept
        requires
            std::is_nothrow_move_constructible_v<T>
    {
        if (capacity_ >= count)
        {
            return none;
        }

        return allocator.allocate(count * sizeof(T))
            .then
            (
                [this, &count] (Allocator::Ptr&& new_storage) noexcept
                {
                    storage_.match_some
                    (
                        [this, &new_storage] (const Allocator::Ptr& old_storage) noexcept
                        {
                            T* old_memory = reinterpret_cast<T*>(old_storage.get());
                            T* new_memory = reinterpret_cast<T*>(new_storage.get());

                            for (size_t i = 0; i < size_; ++i)
                            {
                                construct_at<T>(&new_memory[i], std::move(old_memory[i]));
                                std::destroy_at(&old_memory[i]);
                            }
                        }
                    );

                    storage_ = std::move(new_storage);
                    capacity_ = count;
                }
            );
    }

    template <typename T>
    [[ nodiscard ]] Option
    <
        Errors<Allocator::AllocationError>
    > Vector<T>::reserve_for_new_one(Allocator& allocator) noexcept
        requires
            std::is_nothrow_move_constructible_v<T>
    {
        if (size_ + 1 <= capacity_)
        {
            return none;
        }

        return reserve(allocator, 2 * size_);
    }

    template <typename T>
    [[ nodiscard ]] Option
    <
        Errors<Allocator::AllocationError>
    > Vector<T>::push_back
    (
        Allocator& allocator,
        T v
    ) noexcept
        requires
            std::is_nothrow_move_constructible_v<T>
    {
        auto alloc_error = reserve_for_new_one(allocator);

        alloc_error.match_none
        (
            [this, &v] () noexcept
            {
                storage_.match
                (
                    [this, &v] (const Allocator::Ptr& ptr) noexcept
                    {
                        T* memory = reinterpret_cast<T*>(ptr.get());
                        construct_at<T>(&memory[size_], std::move(v));
                        ++size_;
                    },
                    [] () noexcept
                    {
                        assert(false);
                    }
                );
            }
        );

        assert(size_ <= capacity_);

        return alloc_error;
    }

    template <typename T>
    template <typename... Args>
    [[ nodiscard ]] Option
    <
        Errors<Allocator::AllocationError>
    > Vector<T>::construct_at_end
    (
        Allocator& allocator,
        Args&&... args
    ) noexcept
        requires
            std::is_nothrow_constructible_v<T, Args...>
    {
        auto alloc_error = reserve_for_new_one(allocator);

        alloc_error.match_none
        (
            [this, &args...] () noexcept
            {
                storage_.match
                (
                    [this, &args...] (const Allocator::Ptr& ptr) noexcept
                    {
                        T* memory = reinterpret_cast<T*>(ptr.get());
                        construct_at<T>(&memory[size_], std::move(args)...);
                        ++size_;
                    },
                    [] () noexcept
                    {
                        assert(false);
                    }
                );
            }
        );

        assert(size_ <= capacity_);

        return alloc_error;
    }

    template <typename T>
    [[ nodiscard ]] auto Vector<T>::push_back_to_reserved(T v) noexcept
        -> Option<Errors<PushToReservedError>>
        requires
            std::is_nothrow_move_constructible_v<T>
    {
        if (size_ + 1 > capacity_)
        {
            return Errors<PushToReservedError>
            {
                PushToReservedError
                {
                    .reserved_space = capacity_,
                    .required_space = size_ + 1
                }
            };
        }

        storage_.match
        (
            [this, &v] (const Allocator::Ptr& ptr) noexcept
            {
                T* memory = reinterpret_cast<T*>(ptr.get());
                construct_at<T>(&memory[size_], std::move(v));
                ++size_;
            },
            [] () noexcept
            {
                assert(false);
            }
        );

        assert(size_ <= capacity_);

        return none;
    }

    template <typename T>
    template <typename... Args>
    [[ nodiscard ]] auto Vector<T>::construct_at_reserved_end(Args&&... args) noexcept
        -> Option<Errors<PushToReservedError>>
        requires
            std::is_nothrow_constructible_v<T, Args...>
    {
        if (size_ + 1 > capacity_)
        {
            return Errors<PushToReservedError>
            {
                PushToReservedError
                {
                    .reserved_space = capacity_,
                    .required_space = size_ + 1
                }
            };
        }

        storage_.match
        (
            [this, &args...] (const Allocator::Ptr& ptr) noexcept
            {
                T* memory = reinterpret_cast<T*>(ptr.get());
                construct_at<T>(&memory[size_], std::move(args)...);
                ++size_;
            },
            [] () noexcept
            {
                assert(false);
            }
        );

        assert(size_ <= capacity_);

        return none;
    }

    template <typename T>
    void Vector<T>::destroy_all_elements() noexcept
    {
        storage_.match_some
        (
            [this] (const Allocator::Ptr& storage) noexcept
            {
                T* memory = reinterpret_cast<T*>(storage.get());

                for (size_t i = 0; i < size_; ++i)
                {
                    std::destroy_at(&memory[i]);
                }

                size_ = 0;
            }
        );
    }

    template <typename T>
    void Vector<T>::free_all_memory() noexcept
    {
        destroy_all_elements();
        storage_ = none;
        capacity_ = 0;
    }

    template <typename T>
    size_t Vector<T>::size() const noexcept
    {
        return size_;
    }

    template <typename T>
    size_t Vector<T>::capacity() const noexcept
    {
        return capacity_;
    }

    template <typename T>
    template <typename F>
        requires
            std::is_nothrow_invocable_v<F, T&> and
            std::is_same_v<std::invoke_result_t<F, T&>, void>
    constexpr void Vector<T>::for_each(F&& func) noexcept
    {
        storage_.match_some
        (
            [this, &func] (const Allocator::Ptr& storage) noexcept
            {
                T* memory = reinterpret_cast<T*>(storage.get());
                for (size_t i = 0; i < size_; ++i)
                {
                    func(memory[i]);
                }
            }
        );
    }

    template <typename T>
    template <typename F>
        requires
            std::is_nothrow_invocable_v<F, const T&> and
            std::is_same_v<std::invoke_result_t<F, const T&>, void>
    constexpr void Vector<T>::for_each(F&& func) const noexcept
    {
        storage_.match_some
        (
            [this, &func] (const Allocator::Ptr& storage) noexcept
            {
                const T* memory = reinterpret_cast<const T*>(storage.get());
                for (size_t i = 0; i < size_; ++i)
                {
                    func(memory[i]);
                }
            }
        );
    }

    template <typename T>
    template <typename F>
        requires
            std::is_nothrow_invocable_v<F, T&> and
            std::is_same_v<std::invoke_result_t<F, T&>, bool>
    constexpr void Vector<T>::for_each_until(F&& func) noexcept
    {
        storage_.match_some
        (
            [this, &func] (const Allocator::Ptr& storage) noexcept
            {
                T* memory = reinterpret_cast<T*>(storage.get());
                for (size_t i = 0; i < size_; ++i)
                {
                    if (not func(memory[i]))
                    {
                        break;
                    }
                }
            }
        );
    }

    template <typename T>
    template <typename F>
        requires
            std::is_nothrow_invocable_v<F, const T&> and
            std::is_same_v<std::invoke_result_t<F, const T&>, bool>
    constexpr void Vector<T>::for_each_until(F&& func) noexcept
    {
        storage_.match_some
        (
            [this, &func] (const Allocator::Ptr& storage) noexcept
            {
                const T* memory = reinterpret_cast<const T*>(storage.get());
                for (size_t i = 0; i < size_; ++i)
                {
                    if (not func(memory[i]))
                    {
                        break;
                    }
                }
            }
        );
    }
}
