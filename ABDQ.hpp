#pragma once

#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"
#include <utility>

template <typename T>
class ABDQ : public DequeInterface<T> {
private:
    T* data_;                 // underlying dynamic array
    std::size_t capacity_;    // total allocated capacity
    std::size_t size_;        // number of stored elements
    std::size_t front_;       // index of front element
    std::size_t back_;        // index after the last element (circular)

    static constexpr std::size_t SCALE_FACTOR = 2;

public:
    // Big 5
    ABDQ() : capacity_(1), size_(0) , front_(0), back_(0) {
        data_ = new T[capacity_];
    }
    explicit ABDQ(const size_t capacity) : capacity_(capacity), size_(0) , front_(0), back_(0) {
        data_ = new T[capacity_];
    }
    ABDQ(const ABDQ& other) : capacity_(other.capacity_), size_(other.size_), front_(0), back_(0) {
        this->data_ = new T[capacity_];
        
        for (size_t i = 0; i < size_; i++) {
            this->data_[i] = other.data_[(other.front_ + i) % other.capacity_];
        }
        this->front_ = 0;
        this->back_ = size_;
    }
    ABDQ& operator=(const ABDQ& rhs) {
        if (this == &rhs) return *this;

        T* temp = new T[rhs.capacity_];

        delete[] this->data_;

        this->data_ = temp;
        for (size_t i = 0; i < rhs.size_; i++) {
            this->data_[i] = rhs.data_[(rhs.front_ + i) % rhs.capacity_];
        }

        this->capacity_ = rhs.capacity_;
        this->size_ = rhs.size_;
        this->front_ = 0;
        this->back_ = size_;

        return *this;
    }
    ABDQ(ABDQ&& other) noexcept : data_(other.data_), capacity_(other.capacity_), size_(other.size_) , front_(other.front_), back_(other.back_) {
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
        other.front_ = 0;
        other.back_ = 0;
    }
    ABDQ& operator=(ABDQ&& rhs) noexcept {
        if (this == &rhs) return *this;

        delete[] this->data_;

        this->data_ = rhs.data_;
        this->capacity_ = rhs.capacity_;
        this->size_ = rhs.size_;
        this->front_ = rhs.front_;
        this->back_ = rhs.back_;

        rhs.data_ = nullptr;
        rhs.capacity_ = 0;
        rhs.size_ = 0;
        rhs.front_ = 0;
        rhs.back_ = 0;

        return *this;
    }
    ~ABDQ() noexcept {
        delete[] data_;
        data_ = nullptr;
        capacity_ = 0;
        size_ = 0;
        front_ = 0;
        back_ = 0;
    }


    // Insertion
    void pushFront(const T& item) override {
        if (size_ == capacity_) {
            size_t oldCapacity_ = capacity_;
            capacity_ *= SCALE_FACTOR;

            T* new_data_ = new T[capacity_];
            for (size_t i = 0; i < size_; i++) {
                new_data_[i] = data_[(front_ + i) % oldCapacity_];
            }

            delete[] data_;
            data_ = new_data_;
            front_ = 0;
            back_ = size_;
        }

        front_ = (front_ - 1 + capacity_) % capacity_;
        data_[front_] = item;
        size_++;
    }
    void pushBack(const T& item) override {
        if (size_ == capacity_) {
            size_t oldCapacity_ = capacity_;
            capacity_ *= SCALE_FACTOR;

            T* new_data_ = new T[capacity_];
            for (size_t i = 0; i < size_; i++) {
                new_data_[i] = data_[(front_ + i) % oldCapacity_];
            }

            delete[] data_;
            data_ = new_data_;
            front_ = 0;
            back_ = size_;
        }

        data_[back_] = item;
        back_ = (back_ + 1) % capacity_;
        size_++;
    }

    // Deletion
    T popFront() override {
        if (size_ == 0) throw std::runtime_error("");

        T val = data_[front_];
        front_ = (front_ + 1) % capacity_;
        size_--;

        if (size_ <= capacity_ / 4) capacity_ /= 2;

        return val;
    }
    T popBack() override {
        if (size_ == 0) throw std::runtime_error("");
        
        back_ = (back_ - 1 + capacity_) % capacity_;
        T val = data_[back_];
        size_--;

        if (size_ <= capacity_ / 4) capacity_ /= 2;

        return val;
    }

    // Access
    const T& front() const override {
        if (size_ == 0) throw std::runtime_error("");
        return data_[front_];
    }
    const T& back() const override {
        if (size_ == 0) throw std::runtime_error("");
        return data_[(back_ - 1 + capacity_) % capacity_];
    }

    // Getters
    std::size_t getSize() const noexcept override {
        return size_;
    }

};
