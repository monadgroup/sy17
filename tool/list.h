#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>
#include <cmath>

template <typename T>
class List {
public:
    typedef typename std::size_t size_type;
private:
    size_type _size = 0;
    size_type _capacity = 0;
    T* _data = nullptr;

    static T* allocate(size_type size) {
        return static_cast<T*>(malloc(sizeof(T) * size));
    }

    static void copyRange(const T* begin, const T* end, T* dest) {
        while (begin != end) {
            new((void*)dest) T(*begin);
            ++begin;
            ++dest;
        }
    }

    static void deleteRange(T* begin, T* end) {
        while (begin != end) {
            begin->~T();
            ++begin;
        }
    }

    void doCopy(List<T> const &r) {
        _size = r._size;
        _capacity = r._capacity;
        _data = allocate(_capacity);
        copyRange(r.begin(), r.end(), _data);
    }

    void doMove(List<T> &r) {
        _size = r._size;
        _capacity = r._capacity;
        _data = r._data;

        r._size = 0;
        r._capacity = 0;
        r._data = nullptr;
    }

public:
    List(int initialSize = 0) {
        if (initialSize == 0) return;
        _capacity = initialSize;
        _data = allocate(_capacity);
    }

    List(List<T> const &r) { doCopy(r); }
    List(List<T> &&r) noexcept { doMove(r); }
    List<T> &operator=(List<T> const &r) { doCopy(r); return *this; }
    List<T> &operator=(List<T> &&r) noexcept { doMove(r); return *this; }

    ~List() {
        deleteRange(_data, _data + _size);
        free(_data);
    }

    size_type size() const { return _size; }

    void push_back(const T& value) {
        if (_size == _capacity) reserve(_capacity ? _capacity * 2 : 1);

        new((void*)(_data + _size)) T(value);
        ++_size;
    }

    T& operator[](size_type index) { return _data[index]; }
    const T& operator[](size_type index) const { return _data[index]; }

    T* begin() const { return _data; }
    T* end() const { return _data + _size; }

    void reverse() {
        auto halfLen = _size / 2;
        for (size_type i = 0; i < halfLen; i++) {
            auto endI = _size - i - 1;
            auto temp = _data[i];
            _data[i] = _data[endI];
            _data[endI] = temp;
        }
    }

    void reserve(int size) {
        auto newCapacity = _size + size;
        if (newCapacity <= _capacity) return;

        T* newData = allocate(newCapacity);
        copyRange(_data, _data + _size, newData);
        deleteRange(_data, _data + _size);
        free(_data);
        _data = newData;
        _capacity = newCapacity;
    }

    void concat(const List<T> &otherList) {
        // todo: optimize this
        for (T &itm : otherList) {
            push_back(itm);
        }
    }
};