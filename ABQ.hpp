#pragma once

#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"

// Technically bad, but size_t isn't likely to conflict with any client code.
using std::size_t;

template<typename T>
class ABQ : public QueueInterface<T>{

    size_t capacity_;
    size_t curr_size_;
    T* array_;
    static constexpr size_t scale_factor_ = 2;

public:
    // Constructors + Big 5
    ABQ() : capacity_(1), curr_size_(0) {
        array_ = new T[capacity_];
    }
    explicit ABQ(const size_t capacity) : capacity_(capacity), curr_size_(0) {
        array_ = new T[capacity_];
    }
    ABQ(const ABQ& other) : capacity_(other.capacity_), curr_size_(other.curr_size_) {
        this->array_ = new T[capacity_];
        
        for (size_t i = 0; i < curr_size_; i++) {
            this->array_[i] = other.array_[i];
        }
    }
    ABQ& operator=(const ABQ& rhs) {
        if (this == &rhs) return *this;

        T* temp = new T[rhs.capacity_];

        delete[] this->array_;

        this->array_ = temp;
        for (size_t i = 0; i < rhs.curr_size_; i++) {
            this->array_[i] = rhs.array_[i];
        }

        this->capacity_ = rhs.capacity_;
        this->curr_size_ = rhs.curr_size_;

        return *this;
    }
    ABQ(ABQ&& other) noexcept : array_(other.array_), capacity_(other.capacity_), curr_size_(other.curr_size_) {
        other.array_ = nullptr;
        other.capacity_ = 0;
        other.curr_size_ = 0;
    }
    ABQ& operator=(ABQ&& rhs) noexcept {
        if (this == &rhs) return *this;

        delete[] this->array_;

        this->array_ = rhs.array_;
        this->capacity_ = rhs.capacity_;
        this->curr_size_ = rhs.curr_size_;

        rhs.array_ = nullptr;
        rhs.capacity_ = 0;
        rhs.curr_size_ = 0;

        return *this;
    }
    ~ABQ() noexcept {
        delete[] array_;
        array_ = nullptr;
        capacity_ = 0;
        curr_size_ = 0;
    }

    // Getters
    [[nodiscard]] size_t getSize() const noexcept override {
        return curr_size_;
    }
    [[nodiscard]] size_t getMaxCapacity() const noexcept {
        return capacity_;
    }
    [[nodiscard]] T* getData() const noexcept {
        return array_;
    }

    // Insertion
    void enqueue(const T& data) override {
        if (curr_size_ == capacity_) {
            capacity_ *= scale_factor_;

            T* new_array_ = new T[capacity_];
            for (size_t i = 0; i < curr_size_; i++) {
                new_array_[i] = array_[i];
            }

            delete[] array_;
            array_ = new_array_;
        }

        array_[curr_size_] = data;
        curr_size_++;
    };

    // Access
    T peek() const override {
        if (curr_size_ == 0) throw std::runtime_error("");
        return array_[0];
    }

    // Deletion
    T dequeue() override {
        if (curr_size_ == 0) throw std::runtime_error("");
        
        T val = array_[0];

        T* newArray = new T[capacity_];
        for (size_t i = 1; i < curr_size_; i++) {
            newArray[i - 1] = array_[i];
        }

        delete[] array_;
        array_ = newArray;
        curr_size_--;

        if (curr_size_ <= capacity_ / 4) capacity_ /= 2;

        return val;
    }

};
