/**
 * @file command_tokenizer.cpp
 * @author  Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include "command_tokenizer.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  CommandTokenizer::iterator::iterator(): remainder_(NULL), offset_(std::string::npos)
  {
  }
  //-----------------------------------------------------------------------------------------------
  CommandTokenizer::iterator::iterator(std::string &remainder): 
    remainder_(&remainder), offset_(remainder_->find("\r\n"))
  {
    if(offset_ != std::string::npos) {
      token_.assign(remainder_->begin(), remainder_->begin() + offset_);
      offset_ += 2;
    }
    else remainder_ = NULL;
  }
  //-----------------------------------------------------------------------------------------------
  CommandTokenizer::iterator::reference CommandTokenizer::iterator::operator *() const
  {
    return token_;
  }
  //-----------------------------------------------------------------------------------------------
  CommandTokenizer::iterator::pointer CommandTokenizer::iterator::operator ->() const 
  {
    return &token_;
  }
  //-----------------------------------------------------------------------------------------------
  bool CommandTokenizer::iterator::operator ==(const class_type &r) const 
  {
    return remainder_ == r.remainder_ && offset_ == r.offset_;
  }
  //-----------------------------------------------------------------------------------------------
  bool CommandTokenizer::iterator::operator !=(const class_type &r) const 
  {
    return !this->operator ==(r);
  }
  //-----------------------------------------------------------------------------------------------
  CommandTokenizer::iterator::class_type &CommandTokenizer::iterator::operator ++()
  {
    if(remainder_) {
      size_type x = remainder_->find("\r\n", offset_);

      if(x != std::string::npos) {
        token_.assign(remainder_->begin() + offset_, remainder_->begin() + x);
        offset_ = x + 2;
      }
      else {
        remainder_->assign(remainder_->begin() + offset_, remainder_->end());
        offset_ = x;
        remainder_ = NULL;
        token_.clear();
      }
    }
    return *this;
  }
  //-----------------------------------------------------------------------------------------------
  CommandTokenizer::CommandTokenizer() {}
  //-----------------------------------------------------------------------------------------------
  void CommandTokenizer::append(const std::string &buff)
  {
    remainder_ += buff;
  }
  //-----------------------------------------------------------------------------------------------
  CommandTokenizer::iterator CommandTokenizer::begin() 
  {
    return iterator(remainder_);
  }
  //-----------------------------------------------------------------------------------------------
  CommandTokenizer::iterator CommandTokenizer::end() 
  {
    return iterator();
  }
  //-----------------------------------------------------------------------------------------------
  const std::string &CommandTokenizer::remainder() const 
  {
    return remainder_;
  }
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

