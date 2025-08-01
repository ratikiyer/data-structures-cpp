#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <map>

namespace dsacpp
{

template<
   typename Token,
   typename Data        = void,
   typename Traits      = std::char_traits<Token>,
   typename Allocator   = std::allocator<Data>
>
class trie {
public:

   /**
    * Type Declarations
    */

   using token_type        = Token;
   using key_type          = std::basic_string<Token, Traits>;
   using data_type         = Data;
   using traits_type       = Traits;
   using allocator_type    = Allocator;
   using size_type         = size_t;
   using difference_type   = std::ptrdiff_t;
   
   using value_type        = std::conditional_t<
      std::is_void<data_type>::value,
      key_type,
      std::pair<const key_type, data_type>
   >;


   class iterator {

   };

   class const_iterator {

   };
   
   /*
    * Constructors
   */

   explicit trie(const allocator_type& alloc = Allocator()) noexcept;

   trie(const trie& other);

   trie(trie&& other);

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
   trie(iter begin, iter end, const allocator_type& alloc = Allocator());

   ~trie() noexcept;

   /**
    * Iterators
    */
   
   iterator begin() noexcept;
   const_iterator begin() const noexcept;
   iterator end() noexcept;
   const_iterator end() const noexcept;

   /**
    * Modifiers
    */
   std::pair<iterator, bool> insert(const key_type& key, const data_type& data);

   iterator find(const key_type& key);
   const_iterator find(const key_type& key) const;

   size_type erase(const key_type& key);

   data_type& at(const key_type& key);

   /**
    * Prefix operations
    */
   
   iterator lower_bound(const key_type& prefix);

   iterator upper_bound(const key_type& prefix);

   std::pair<iterator, iterator> equal_range(const key_type& prefix);

   /**
    * Getters
    */
   
   size_type size() const noexcept;
   bool empty() const noexcept;
   void clear() noexcept;

   /**
    * Operators
    */
   
   data_type& operator[](const key_type& key);

   trie& operator=(const trie& other);

   trie& operator=(trie&& other);

private:

   struct Node {
      std::conditional_t<
         std::is_void<data_type>::value,
         bool,
         data_type
      > data_or_flag;
      bool is_terminal = std::is_void<data_type>::value;
      std::map<token_type, Node*> children;

      Node(const allocator_type& alloc);

      ~Node() noexcept;
   };

   using node_allocator_type = typename std::allocator_traits<allocator_type>::
      template rebind_alloc<Node>;
   
   Node* root_ = nullptr;
   size_type size_ = 0;
   allocator_type alloc_;
   node_allocator_type node_alloc_;

   Node* _create_node();
   void _destroy_node();

   Node* _find_node(const key_type& key);
   void _clear_node(Node* node);
};

}