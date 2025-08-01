#pragma once

#include <cstddef>
#include <iterator>
#include <cassert>
#include <initializer_list>
#include <type_traits>
#include <memory>
#include <new>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace dsacpp
{

template<typename T>
class vector {
public:
   /**
    * Iterators
    */
   
   class iterator {
   public:
      using value_type        = const T;
      using reference         = const T&;
      using pointer           = const T*;
      using difference_type   = std::ptrdiff_t;
      using iterator_category = std::random_access_iterator_tag;

      iterator() : ptr_{ nullptr } { }
      explicit iterator(pointer p) : ptr_{ p } { }

      // dereference and pointer access
      reference operator*() const noexcept { return *ptr_; }
      pointer operator->() const noexcept { return ptr_; }

      // increment/decrement
      iterator& operator++() noexcept { ++ptr_; return *this; }
      iterator operator++(int) noexcept { iterator tmp = *this; ++(*this); return tmp; }
      iterator& operator--() noexcept { --ptr_; return *this; }
      iterator operator--(int) noexcept { iterator tmp = *this; --(*this); return tmp; }

      // arithmetic
      iterator& operator+=(difference_type n) noexcept { ptr_ += n; return *this; }
      iterator& operator-=(difference_type n) noexcept { ptr_ -= n; return *this; }
      iterator operator+(difference_type n) const noexcept { iterator tmp{ ptr_ }; tmp += n; return tmp; }
      iterator operator-(difference_type n) const noexcept { iterator tmp{ ptr_ }; tmp -= n; return tmp; }
      difference_type operator-(const iterator& other) const noexcept { return ptr_ - other.ptr_; }

      // comparisons
      bool operator==(const iterator& other) const noexcept { return ptr_ == other.ptr_; }
      bool operator!=(const iterator& other) const noexcept { return ptr_ != other.ptr_; }
      bool operator<(const iterator& other) const noexcept { return ptr_ < other.ptr_; }
      bool operator>(const iterator& other) const noexcept { return ptr_ > other.ptr_; }
      bool operator<=(const iterator& other) const noexcept { return ptr_ <= other.ptr_; }
      bool operator>=(const iterator& other) const noexcept { return ptr_ >= other.ptr_; }

   private:
      pointer ptr_;
   };

   class const_iterator {
   public:
      using value_type        = const T;
      using reference         = const T&;
      using pointer           = const T*;
      using difference_type   = std::ptrdiff_t;
      using iterator_category = std::random_access_iterator_tag;

      const_iterator() : ptr_{ nullptr } { }
      explicit const_iterator(pointer p) : ptr_{ p } { }

      // dereference and pointer access
      reference operator*()  const noexcept { return *ptr_; }
      pointer operator->() const noexcept { return ptr_; }

      // increment/decrement
      const_iterator& operator++() noexcept { ++ptr_; return *this; }
      const_iterator operator++(int) noexcept { auto tmp = *this; ++ptr_; return tmp; }
      const_iterator& operator--() noexcept { --ptr_; return *this; }
      const_iterator operator--(int) noexcept { auto tmp = *this; --ptr_; return tmp; }

      // arithmetic
      const_iterator& operator+=(difference_type n) noexcept { ptr_ += n; return *this; }
      const_iterator  operator+ (difference_type n) const noexcept { return const_iterator{ ptr_ + n}; }
      const_iterator& operator-=(difference_type n) noexcept { ptr_ -= n; return *this; }
      const_iterator  operator- (difference_type n) const noexcept { return const_iterator{ ptr_ - n }; }
      difference_type operator-(const const_iterator& other) const noexcept { return ptr_ - other.ptr_; }

      // comparisons
      bool operator==(const const_iterator& other) const noexcept { return ptr_ == other.ptr_; }
      bool operator!=(const const_iterator& other) const noexcept { return ptr_ != other.ptr_; }
      bool operator<(const const_iterator& other) const noexcept { return ptr_ <  other.ptr_; }
      bool operator>(const const_iterator& other) const noexcept { return ptr_ >  other.ptr_; }
      bool operator<=(const const_iterator& other) const noexcept { return ptr_ <= other.ptr_; }
      bool operator>=(const const_iterator& other) const noexcept { return ptr_ >= other.ptr_; }

   private:
      pointer ptr_;
   };

   /**
    * Type declarations
    */
   
   using value_type              = T;
   using size_type               = std::size_t;
   using reference               = T&;
   using rvalue_reference        = T&&;
   using pointer                 = T*;
   using const_value_type        = const T;
   using const_reference         = const T&;
   using const_pointer           = const T*;
   using difference_type         = std::ptrdiff_t;

   using iterator                = iterator;
   using const_iterator          = const_iterator;
   using reverse_iterator        = std::reverse_iterator<iterator>;
   using const_reverse_iterator  = std::reverse_iterator<const_iterator>;

   /**
    * Constructors & Destructor
    */

   vector() noexcept;
   
   explicit vector(size_type count, const_reference value = T());

   vector(const vector& other) noexcept(std::is_nothrow_copy_constructible<T>::value);

   vector(vector&& other) noexcept;

   vector(std::initializer_list<value_type> init_list);

   // SNIFAE
   template<
      typename iter,
      typename = std::enable_if_t<
                  std::is_base_of<
                  std::input_iterator_tag,
                  typename std::iterator_traits<iter>::iterator_category
               >::value
      >
   >
   vector(iter begin, iter end);

   ~vector() noexcept;

   /**
    * Getters
    */

   reference at(size_type index);
   const_reference at(size_type index) const;
   
   reference front();
   const_reference front() const;

   reference back();
   const_reference back() const;

   /**
    * Setters
    */

   void clear() noexcept(std::is_nothrow_destructible<T>::value);

   void push_back(const_reference value);
   void push_back(rvalue_reference value);

   template<typename... Args>
   void emplace_back(Args&&... args);

   void pop_back();

   void resize(size_type new_size);
   void resize(size_type new_size, const_reference value);

   void swap(vector& other) noexcept;

   /**
    * Metadata
    */
   
   size_type size() const noexcept;

   size_type capacity() const noexcept;

   bool empty() const noexcept;

   void shrink_to_fit();

   void reserve(size_type new_capacity);

   void reallocate(size_type new_capacity);

   /**
    * Operators
    */

   vector& operator=(const vector& other);
   vector& operator=(vector&& other);
   vector& operator=(std::initializer_list<value_type> init_list);

   reference operator[](size_type index);
   const_reference operator[](size_type index) const;
   
   /**
    * Iterators
    */

   iterator begin() noexcept;
   const_iterator begin() const noexcept;
   reverse_iterator rbegin() noexcept;
   const_reverse_iterator rbegin() const noexcept;

   iterator end() noexcept;
   const_iterator end() const noexcept;
   reverse_iterator rend() noexcept;
   const_reverse_iterator rend() const noexcept;

private:

   static constexpr size_t VECTOR_DEFAULT_CAPACITY = 16;
   static constexpr u_int16_t VECTOR_GROWTH_RATE = 2;

   size_type size_ = 0;
   size_type capacity_ = 0;
   pointer data_ = nullptr;

   void _clear() noexcept;

   // multiply capacity by growth rate
   void _resize();
};

template<typename T>
vector<T>::vector() noexcept { }

template<typename T>
vector<T>::vector(size_type count, const_reference value) :
   size_{ count },
   capacity_{ count },
   data_{ capacity_ ? static_cast<pointer>(::operator new(capacity_ * sizeof(T))) : nullptr }
{ std::uninitialized_fill_n(data_, size_, value); }

template<typename T>
vector<T>::vector(const vector<T>& other) noexcept(std::is_nothrow_copy_constructible<T>::value) :
   size_{ other.size_ },
   capacity_{ other.capacity_ },
   data_{ capacity_ ? static_cast<pointer>(::operator new(capacity_ * sizeof(T))) : nullptr }
{ std::uninitialized_copy_n(other.data_, size_, data_); }

template<typename T>
vector<T>::vector(vector<T>&& other) noexcept :
   size_{ other.size_ },
   capacity_{ other.capacity_ },
   data_{ other.data_ }
{ other.size_ = 0; other.capacity_ = 0; other.data_ = nullptr; }

template<typename T>
vector<T>::vector(std::initializer_list<value_type> init_list) :
   size_{ init_list.size() },
   capacity_{ init_list.size() },
   data_{ capacity_ ? static_cast<pointer>(::operator new(capacity_ * sizeof(T))) : nullptr }
{ std::uninitialized_copy(init_list.begin(), init_list.end(), data_); }

template<typename T>
template<typename iter, typename>
vector<T>::vector(iter begin, iter end) {
   auto dist = std::distance(begin, end);
   size_ = static_cast<size_type>(dist);
   capacity_ = static_cast<size_type>(dist);
   data_ = capacity_ ? static_cast<pointer>(::operator new(capacity_ * sizeof(T))) : nullptr;
   std::uninitialized_copy(begin, end, data_);
}

template<typename T>
vector<T>::~vector() noexcept {
   _clear();
}

template<typename T>
typename vector<T>::reference vector<T>::at(size_type index) {
   if (index >= size_) {
      std::ostringstream oss;
      oss << "vector::at index" << index << " out of range" << "(size=" << size_ << ")";
      throw std::out_of_range(oss.str());
   }
   return data_[index];
}

template<typename T>
typename vector<T>::const_reference vector<T>::at(size_type index) const {
   if (index >= size_) {
      std::ostringstream oss;
      oss << "vector::at index" << index << " out of range" << "(size=" << size_ << ")";
      throw std::out_of_range(oss.str());
   }
   return data_[index];
}

template<typename T>
typename vector<T>::reference vector<T>::front() {
   return at(0);
}

template<typename T>
typename vector<T>::const_reference vector<T>::front() const {
   return at(0);
}

template<typename T>
typename vector<T>::reference vector<T>::back() {
   return at(size_ - 1);
}

template<typename T>
typename vector<T>::const_reference vector<T>::back() const {
   return at(size_ - 1);
}

template<typename T>
void vector<T>::clear() noexcept(std::is_nothrow_destructible<T>::value) {
   for (size_t i = 0; i < size_; i++) {
      data_[i].~T();
   }
   size_ = 0;
}

template<typename T>
void vector<T>::push_back(const_reference value) {
   if (size_ >= capacity_) {
      _resize();
   }
   new (data_ + size_++) T(value);
}

template<typename T>
void vector<T>::push_back(rvalue_reference value) {
   if (size_ >= capacity_) {
      _resize();
   }
   new (data_ + size_++) T(std::move(value)); 
}

template<typename T>
template<typename... Args>
void vector<T>::emplace_back(Args&&... args) {
   if (size_ == capacity_) _resize();
   new (data_ + size_++) T(std::forward<Args>(args)...);
}

template<typename T>
void vector<T>::pop_back() {
   if (size_ == 0) {
      throw std::out_of_range("vector::pop_back empty vector");
   }
   data_[--size_].~T();
}

template<typename T>
void vector<T>::resize(size_type new_size) {
   if (new_size == size_) {
      return;
   } else if (new_size < size_) {
      for (size_t i = new_size; i < size_; i++) {
         data_[i].~T();
      }
   } else {
      while (new_size > capacity_) {
         _resize();
      }
      for (size_t i = size_; i < new_size; i++) {
         new (data_ + i) T();
      }
   }
   size_ = new_size;
}

template<typename T>
void vector<T>::resize(size_type new_size, const_reference value) {
   if (new_size == size_) {
      return;
   } else if (new_size < size_) {
      for (size_t i = new_size; i < size_; i++) {
         data_[i].~T();
      }
   } else {
      while (new_size > capacity_) {
         _resize();
      }
      for (size_t i = size_; i < new_size; i++) {
         new (data_ + i) T(value);
      }
   }
   size_ = new_size;
}

template<typename T>
void vector<T>::swap(vector<T>& other) noexcept {
   std::swap(data_, other.data_);
   std::swap(size_, other.size_);
   std::swap(capacity_, other.capacity_);
}

template<typename T>
typename vector<T>::size_type vector<T>::size() const noexcept {
   return size_;
}

template<typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
   return capacity_;
}

template<typename T>
bool vector<T>::empty() const noexcept {
   return size_ == 0;
}

template<typename T>
void vector<T>::shrink_to_fit() {
   if (capacity_ == size_) return;
   pointer new_data = size_ ? static_cast<pointer>(::operator new(size_ * sizeof(T))) : nullptr;
   for (size_t i = 0; i < size_; i++) {
      new (new_data + i) T(std::move(data_[i]));
      data_[i].~T();
   }
   ::operator delete(data_);
   data_ = new_data;
   capacity_ = size_;
}

template<typename T>
void vector<T>::reserve(size_type new_capacity) {
   if (new_capacity <= capacity_) return;
   pointer new_data = new_capacity ? static_cast<pointer>(::operator new(new_capacity * sizeof(T))) : nullptr;
   for (size_t i = 0; i < size_; i++) {
      new (new_data + i) T(std::move(data_[i]));
      data_[i].~T();
   }
   ::operator delete(data_);
   data_ = new_data;
   capacity_ = new_capacity;
}

template<typename T>
void vector<T>::reallocate(size_type new_capacity) {
   if (new_capacity == capacity_) return;
   pointer new_data = new_capacity ? static_cast<pointer>(::operator new(new_capacity * sizeof(T))) : nullptr;
   for (size_t i = 0; i < size_; i++) {
      new (new_data + i) T(std::move(data_[i]));
      data_[i].~T();
   }
   ::operator delete(data_);
   data_ = new_data;
   capacity_ = new_capacity;
}

template<typename T>
vector<T>& vector<T>::operator=(const vector<T>& other) {
   if (this == &other) return *this;
   _clear();
   size_ = other.size_;
   capacity_ = other.capacity_;
   data_ = capacity_ ? static_cast<pointer>(::operator new(capacity_ * sizeof(T))) : nullptr;
   for (size_t i = 0; i < size_; i++) {
      new (data_ + i) T(other.data_[i]);
   }
   return *this;
}

template<typename T>
vector<T>& vector<T>::operator=(vector<T>&& other) {
   if (this == &other) return *this;
   _clear();
   size_ = other.size_;
   capacity_ = other.capacity_;
   data_ = other.data_;
   other.size_ = 0;
   other.capacity_ = 0;
   other.data_ = nullptr;
   return *this;
}

template<typename T>
vector<T>& vector<T>::operator=(std::initializer_list<value_type> init_list) {
   _clear();
   size_ = init_list.size();
   capacity_ = init_list.size();
   data_ = capacity_ ? static_cast<pointer>(::operator new(capacity_ * sizeof(T))) : nullptr;
   std::uninitialized_copy_n(init_list.begin(), init_list.end(), data_);
   return *this;
}

template<typename T>
typename vector<T>::reference vector<T>::operator[](size_type index) {
   return at(index);
}

template<typename T>
typename vector<T>::const_reference vector<T>::operator[](size_type index) const {
   return at(index);
}

template<typename T>
typename vector<T>::iterator vector<T>::begin() noexcept {
   return iterator{ data_ };
}

template<typename T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
   return const_iterator{ data_ };
}

template<typename T>
typename vector<T>::reverse_iterator vector<T>::rbegin() noexcept {
   return std::reverse_iterator<iterator>{ end() };
}

template<typename T>
typename vector<T>::const_reverse_iterator vector<T>::rbegin() const noexcept {
   return std::reverse_iterator<const_iterator>{ end() };
}

template<typename T>
typename vector<T>::iterator vector<T>::end() noexcept {
   return iterator{ data_ + size_ };
}

template<typename T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
   return const_iterator{ data_ + size_ };
}

template<typename T>
typename vector<T>::reverse_iterator vector<T>::rend() noexcept {
   return std::reverse_iterator<iterator>{ begin() };
}

template<typename T>
typename vector<T>::const_reverse_iterator vector<T>::rend() const noexcept {
   return std::reverse_iterator<const_iterator>{ begin() };
}

template<typename T>
void vector<T>::_clear() noexcept {
   for (size_t i = 0; i < size_; i++) {
      data_[i].~T();
   }
   ::operator delete(data_);
   size_ = 0;
   capacity_ = 0;
}

template<typename T>
void vector<T>::_resize()  {
   capacity_ = capacity_ ? VECTOR_GROWTH_RATE * capacity_ : VECTOR_DEFAULT_CAPACITY;
   pointer new_data = capacity_ ? static_cast<pointer>(::operator new(capacity_ * sizeof(T))) : nullptr;
   for (size_t i = 0; i < size_; i++) {
      new (new_data + i) T(data_[i]);
      data_[i].~T();
   }
   ::operator delete(data_);
   data_ = new_data;
}

} // namespace dsacpp