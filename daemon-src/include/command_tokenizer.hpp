/**
 * @file command_tokenizer.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_COMMAND_TOKENIZER__H__
#define __WAPSTART_SWORDFISH_COMMAND_TOKENIZER__H__
//-------------------------------------------------------------------------------------------------
#include <string>
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  class CommandTokenizer {
  public:
    typedef CommandTokenizer          class_type;
    typedef std::size_t               size_type;
    /**
     *
     */
    class iterator {
    public:
      typedef iterator                  class_type;
      typedef std::forward_iterator_tag iterator_category;
      typedef ptrdiff_t                 difference_type;
      typedef std::string               value_type;
      typedef std::string::size_type    size_type; 
      typedef const value_type&         reference;
      typedef const value_type*         pointer;
      iterator();
      iterator(std::string &remainder);
      reference operator *() const ;
      pointer   operator ->() const;
      class_type &operator ++(); 
      bool operator ==(const class_type &r) const;
      bool operator !=(const class_type &r) const;
    private:
      value_type *remainder_;
      value_type  token_;
      size_type   offset_;
    };
    /**
     *
     */
    CommandTokenizer();
    /**
     *
     */
    void append(const std::string &buff);
    /**
     *
     */
    iterator begin();
    /**
     *
     */
    iterator end();
    /**
     *
     */
    const std::string &remainder() const;
  private:
    std::string remainder_;
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_COMMAND_TOKENIZER__H__
//-------------------------------------------------------------------------------------------------

