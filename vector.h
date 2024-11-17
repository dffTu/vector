#include <compare>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>

template <typename ValueType>
class Vector {
public:
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;

    class Iterator {
        ValueType* pointer_ = nullptr;

    public:
        explicit Iterator(ValueType* pointer) {
            pointer_ = pointer;
        }
        Iterator() = default;

        ValueType& operator*() const {
            return *pointer_;
        }
        ValueType* operator->() const {
            return pointer_;
        }

        Iterator& operator=(Iterator other) {
            pointer_ = other.pointer_;
            return *this;
        }

        Iterator& operator++() {
            ++pointer_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++pointer_;
            return temp;
        }

        Iterator& operator--() {
            --pointer_;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --pointer_;
            return temp;
        }

        Iterator operator+(DifferenceType shift) {
            Iterator temp = *this;
            temp.pointer_ += shift;
            return temp;
        }

        Iterator operator-(DifferenceType shift) {
            Iterator temp = *this;
            temp.pointer_ -= shift;
            return temp;
        }

        DifferenceType operator-(Iterator other) {
            return pointer_ - other.pointer_;
        }

        Iterator& operator+=(DifferenceType shift) {
            pointer_ += shift;
            return *this;
        }

        Iterator& operator-=(DifferenceType shift) {
            pointer_ -= shift;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return pointer_ == other.pointer_;
        }
        bool operator!=(const Iterator& other) const {
            return pointer_ != other.pointer_;
        }
        std::strong_ordering operator<=>(const Iterator& other) const {
            if (pointer_ < other.pointer_) {
                return std::strong_ordering::less;
            } else if (pointer_ == other.pointer_) {
                return std::strong_ordering::equivalent;
            } else {
                return std::strong_ordering::greater;
            }
        }
    };

    Vector() = default;

    explicit Vector(size_t size) {
        size_ = size;
        ChangeCapacity();
        begin_ = new ValueType[capacity_]{};
    }

    Vector(size_t size, const ValueType& value) {
        size_ = size;
        ChangeCapacity();
        begin_ = (ValueType*) calloc(capacity_, sizeof(ValueType));
        for (size_t i = 0; i < size_; ++i) {
            new(begin_ + i) ValueType(value);
        }
    }

    Vector(const Iterator& begin, const Iterator& end) {
        size_ = end - begin;
        ChangeCapacity();
        begin_ = (ValueType*) calloc(capacity_, sizeof(ValueType));
        for (size_t i = 0; i < size_; ++i) {
            new(begin_ + i) ValueType(*(begin + i));
        }
    }

    Vector(const Vector& other) {
        size_ = other.size_;
        ChangeCapacity();
        begin_ = (ValueType*) calloc(capacity_, sizeof(ValueType));
        for (size_t i = 0; i < size_; ++i) {
            new(begin_ + i) ValueType(other.begin_[i]);
        }
    }

    Vector(Vector&& other) {
        size = other.size_;
        ChangeCapacity();
        std::swap(begin_, other.begin_);
    }

    Vector(std::initializer_list<ValueType> list) {
        size_ = list.size();
        ChangeCapacity();
        begin_ = (ValueType*) calloc(capacity_, sizeof(ValueType));
        size_t i = 0;
        for (const ValueType& value : list) {
            new(begin_ + i) ValueType(value);
            ++i;
        }
    }

    Vector& operator=(const Vector& other) {
        Vector temp_vector(other);
        Swap(temp_vector);
        return *this;
    }

    Vector& operator=(Vector&& other) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        std::swap(begin_, other.begin_);
        return *this;
    }

    Vector& operator=(std::initializer_list<ValueType> list) {
        size_ = list.size_;
        ChangeCapacity();
        begin_ = (ValueType*) calloc(capacity_, sizeof(ValueType));
        size_t i = 0;
        for (const ValueType& value : list) {
            new(begin_ + i) ValueType(value);
            ++i;
        }
        return *this;
    }

    void ReallocateArray() {
        if (begin_ == nullptr) {
            begin_ = new ValueType[capacity_];
        } else {
            ValueType* ptr = (ValueType*) calloc(capacity_, sizeof(ValueType));
            for (size_t i = 0; i < size_ - 1; ++i) {
                new (ptr + i) ValueType(begin_[i]);
            }
            std::swap(ptr, begin_);
            delete[] ptr;
        }
    }

    void ChangeCapacity() {
        if (capacity_ >= size_) {
            return;
        }
        while (capacity_ < size_) {
            if (capacity_ == 0) {
                capacity_ = 1;
            } else {
                capacity_ *= 2;
            }
        }
    }

    ~Vector() {
        delete[] begin_;
    }

    SizeType Size() const {
        return size_;
    }

    SizeType Capacity() const {
        return capacity_;
    }

    const ValueType* Data() const {
        return begin_;
    }

    ValueType& operator[](size_t position) {
        return begin_[position];
    }

    ValueType operator[](size_t position) const {
        return begin_[position];
    }

    bool operator==(const Vector& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (begin_[i] != other.begin_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Vector& other) const {
        return !(operator==(other));
    }
    std::strong_ordering operator<=>(const Vector& other) const {
        for (size_t i = 0; i < size_ && i < other.size_; ++i) {
            if (begin_[i] < other.begin_[i]) {
                return std::strong_ordering::less;
            } else if (begin_[i] > other.begin_[i]) {
                return std::strong_ordering::greater;
            }
        }
        if (size_ > other.size_) {
            return std::strong_ordering::greater;
        } else if (size_ == other.size_) {
            return std::strong_ordering::equivalent;
        } else {
            return std::strong_ordering::less;
        }
    }

    void Reserve(SizeType new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }
        capacity_ = new_capacity;
        ReallocateArray();
    }

    void Clear() {
        size_ = 0;
    }

    void PushBack(const ValueType& new_element) {
        ++size_;
        size_t capacity_tmp = capacity_;
        ChangeCapacity();
        if (capacity_tmp != capacity_) {
            ReallocateArray();
        }

        begin_[size_ - 1] = new_element;
    }

    void PopBack() {
        --size_;
    }

    void Swap(Vector& other) {
        std::swap(begin_, other.begin_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    Iterator Begin() {
        return Iterator(begin_);
    }
    Iterator End() {
        return Iterator(begin_ + size_);
    }

    Iterator begin() {
        return Iterator(begin_);
    }
    Iterator end() {
        return Iterator(begin_ + size_);
    }

private:
    ValueType* begin_ = nullptr;
    SizeType size_ = 0;
    SizeType capacity_ = 0;
};

