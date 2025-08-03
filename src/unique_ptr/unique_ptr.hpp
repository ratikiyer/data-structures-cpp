#pragma once

#include <type_traits>
#include <stdexcept>
#include <utility>

namespace dsacpp
{

template<typename T>
struct default_delete {
   void operator()(T* ptr) {
      delete ptr;
   }
};

template<typename T>
struct default_delete<T[]> {
   void operator()(T* ptr) {
      delete[] ptr;
   }
};

template<typename T, typename D = default_delete<T>>
class unique_ptr {
public:
   /**
    * Type declarations
    */
   
   using pointer        = T*;
   using element_type   = T;
   using deleter_type   = D;

   /**
    * Constructors & Destructor
    */
   
   unique_ptr();

   unique_ptr(pointer ptr);

   unique_ptr(const unique_ptr& other) = delete;

   unique_ptr(unique_ptr&& other) noexcept(std::is_nothrow_move_constructible<T>::value);

   ~unique_ptr() noexcept;

   /**
    * Modifiers
    */
   
   pointer release() noexcept;

   void reset(pointer new_ptr = nullptr);

   void swap(unique_ptr& other) noexcept;

   /**
    * Observers
    */
   
   pointer get() const noexcept;

   deleter_type& get_deleter() noexcept;

   const deleter_type& get_deleter() const noexcept;

   bool is_owning() const noexcept;

   /**
    * Operators
    */
   
   unique_ptr& operator=(const unique_ptr& other) = delete;

   unique_ptr& operator=(unique_ptr&& other) noexcept(std::is_nothrow_move_constructible<T>::value);

   element_type& operator*() const;

   pointer operator->() const noexcept;

   explicit operator bool() const noexcept;

private:
   pointer ptr_;
   deleter_type del_;
};

template<typename T, typename D>
class unique_ptr<T[], D> {
public:
   /**
    * Type declarations
    */
   
   using pointer        = T*;
   using element_type   = T;
   using deleter_type   = D;

   /**
    * Constructors & Destructor
    */
   
   unique_ptr();

   unique_ptr(pointer ptr);

   unique_ptr(const unique_ptr& other) = delete;

   unique_ptr(unique_ptr&& other) noexcept(std::is_nothrow_move_constructible<T>::value);

   ~unique_ptr() noexcept;

   /**
    * Modifiers
    */
   
   pointer release() noexcept;

   void reset(pointer new_ptr = nullptr);

   void swap(unique_ptr& other) noexcept;

   /**
    * Observers
    */
   
   pointer get() const noexcept;

   deleter_type& get_deleter() noexcept;

   const deleter_type& get_deleter() const noexcept;

   bool is_owning() const noexcept;

   /**
    * Operators
    */
   
   unique_ptr& operator=(const unique_ptr& other) = delete;

   unique_ptr& operator=(unique_ptr&& other) noexcept(std::is_nothrow_move_constructible<T>::value);

   element_type& operator[](size_t index) const;

   explicit operator bool() const noexcept;

private:
   pointer ptr_;
   deleter_type del_;
};

// implementation
template<typename T, typename D>
unique_ptr<T, D>::unique_ptr() :
   ptr_{ nullptr },
   del_{  }
{ }

template<typename T, typename D>
unique_ptr<T, D>::unique_ptr(pointer ptr) : 
   ptr_{ ptr },
   del_{  }
{ }

template<typename T, typename D>
unique_ptr<T, D>::unique_ptr(unique_ptr<T, D>&& other) noexcept(std::is_nothrow_move_constructible<T>::value) : 
   ptr_{ other.ptr_ },
   del_{ std::move(other.del_) }
{ other.ptr_ = nullptr; }

template<typename T, typename D>
unique_ptr<T, D>::~unique_ptr() noexcept {
   del_(ptr_);
}

template<typename T, typename D>
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::release() noexcept {
   pointer tmp = ptr_;
   ptr_ = nullptr;
   return tmp;
}

template<typename T, typename D>
void unique_ptr<T, D>::reset(pointer new_ptr) {
   if (ptr_) {
      del_(ptr_);
   }
   ptr_ = new_ptr;
}

template<typename T, typename D>
void unique_ptr<T, D>::swap(unique_ptr<T, D>& other) noexcept {
   std::swap(ptr_, other.ptr_);
   std::swap(del_, other.del_);
}

template<typename T, typename D>
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::get() const noexcept {
   return ptr_;
}

template<typename T, typename D>
typename unique_ptr<T, D>::deleter_type& unique_ptr<T, D>::get_deleter() noexcept {
   return del_;
}

template<typename T, typename D>
const typename unique_ptr<T, D>::deleter_type& unique_ptr<T, D>::get_deleter() const noexcept {
   return del_;
}

template<typename T, typename D>
bool unique_ptr<T, D>::is_owning() const noexcept {
   return ptr_ != nullptr;
}

template<typename T, typename D>
unique_ptr<T, D>& unique_ptr<T, D>::operator=(unique_ptr<T, D>&& other) noexcept(std::is_nothrow_move_constructible<T>::value) {
   if (this == &other) {
      return *this;
   }
   reset();
   ptr_ = other.ptr_;
   del_ = std::move(other.del_);
   other.ptr_ = nullptr;
   return *this;
}

template<typename T, typename D>
typename unique_ptr<T, D>::element_type& unique_ptr<T, D>::operator*() const {
   if (ptr_) {
      return *ptr_;
   } else {
      throw std::logic_error("unique_ptr::operator* attempting to dereference nullptr");
   }
}

template<typename T, typename D>
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::operator->() const noexcept {
   return ptr_;
}

template<typename T, typename D>
unique_ptr<T, D>::operator bool() const noexcept {
   return ptr_ != nullptr;
}

// template specialization for arrays
template<typename T, typename D>
unique_ptr<T[], D>::unique_ptr() :
   ptr_{ nullptr },
   del_{  }
{ }

template<typename T, typename D>
unique_ptr<T[], D>::unique_ptr(pointer ptr) : 
   ptr_{ ptr },
   del_{  }
{ }

template<typename T, typename D>
unique_ptr<T[], D>::unique_ptr(unique_ptr<T[], D>&& other) noexcept(std::is_nothrow_move_constructible<T>::value) : 
   ptr_{ other.ptr_ },
   del_{ other.del_ }
{ }

template<typename T, typename D>
unique_ptr<T[], D>::~unique_ptr() noexcept {
   del_(ptr_);
}

template<typename T, typename D>
typename unique_ptr<T[], D>::pointer unique_ptr<T[], D>::release() noexcept {
   pointer tmp = ptr_;
   ptr_ = nullptr;
   return tmp;
}

template<typename T, typename D>
void unique_ptr<T[], D>::reset(pointer new_ptr) {
   if (ptr_) {
      del_(ptr_);
   }
   ptr_ = new_ptr;
}

template<typename T, typename D>
void unique_ptr<T[], D>::swap(unique_ptr<T[], D>& other) noexcept {
   std::swap(ptr_, other.ptr_);
   std::swap(del_, other.del_);
}

template<typename T, typename D>
typename unique_ptr<T[], D>::pointer unique_ptr<T[], D>::get() const noexcept {
   return ptr_;
}

template<typename T, typename D>
typename unique_ptr<T[], D>::deleter_type& unique_ptr<T[], D>::get_deleter() noexcept {
   return del_;
}

template<typename T, typename D>
const typename unique_ptr<T[], D>::deleter_type& unique_ptr<T[], D>::get_deleter() const noexcept {
   return del_;
}

template<typename T, typename D>
bool unique_ptr<T[], D>::is_owning() const noexcept {
   return ptr_ != nullptr;
}

template<typename T, typename D>
unique_ptr<T[], D>& unique_ptr<T[], D>::operator=(unique_ptr<T[], D>&& other) noexcept(std::is_nothrow_move_constructible<T>::value) {
   if (this == &other) {
      return *this;
   }
   reset();
   ptr_ = other.ptr_;
   del_ = std::move(other.del_);
   other.ptr_ = nullptr;
   return *this;
}

template<typename T, typename D>
typename unique_ptr<T[], D>::element_type& unique_ptr<T[], D>::operator[](size_t index) const {
   if (ptr_) {
      return ptr_[index];
   } else {
      throw std::logic_error("unique_ptr::operator* attempting to dereference nullptr");
   }
}

template<typename T, typename D>
unique_ptr<T[], D>::operator bool() const noexcept {
   return ptr_ != nullptr;
}

} // namespace dsacpp