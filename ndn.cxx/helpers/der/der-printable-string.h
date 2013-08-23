/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Yingdi Yu
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Yingdi Yu <yingdi@cs.ucla.edu>
 */

#ifndef NDN_DER_PRINTABLE_STRING_H
#define NDN_DER_PRINTABLE_STRING_H

#include "der-byte-string.h"

namespace ndn
{

namespace der
{
  class DerPrintableString : public DerByteString
  {
  public:
    DerPrintableString(const string & str);

    DerPrintableString(const Blob & blob);

    DerPrintableString(InputIterator &start);

    virtual 
    ~DerPrintableString();

    virtual void accept(VoidNoArguVisitor & visitor)               {        visitor.visit(*this);        }
    virtual void accept(VoidVisitor & visitor, boost::any param)   {        visitor.visit(*this, param); }
    virtual boost::any accept(NoArguVisitor & visitor)             { return visitor.visit(*this);        }
    virtual boost::any accept(Visitor & visitor, boost::any param) { return visitor.visit(*this, param); }
  };

}//der

}//ndn
#endif
