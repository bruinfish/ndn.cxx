/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef _CCNB_PARSER_NON_NEGATIVE_INTEGER_VISITOR_H_
#define _CCNB_PARSER_NON_NEGATIVE_INTEGER_VISITOR_H_

#include "no-argu-depth-first-visitor.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

/**
 * \ingroup ccnx-ccnb
 * \brief Visitor to obtain non-negative integer value from UDATA block
 *
 * Will return empty boost::any() if called on anything except UDATA block
 */
class NonNegativeIntegerVisitor : public NoArguDepthFirstVisitor
{
public:
  virtual boost::any visit (Blob &n); ///< Throws an exception if BLOB object is encountered
  virtual boost::any visit (Udata &n);
};

} // CcnbParser
} // wire

NDN_NAMESPACE_END

#endif // _CCNB_PARSER_NON_NEGATIVE_INTEGER_VISITOR_H_
