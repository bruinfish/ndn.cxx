/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef _CCNB_PARSER_TIMESTAMP_VISITOR_H_
#define _CCNB_PARSER_TIMESTAMP_VISITOR_H_

#include "no-argu-depth-first-visitor.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

/**
 * \ingroup ccnx-ccnb
 * \brief Visitor to obtain timestamp value from BLOB block
 *
 * Will return empty boost::any() if called on anything except BLOB block
 */
class TimestampVisitor : public NoArguDepthFirstVisitor
{
public:
  virtual boost::any visit (Blob &n); 
  virtual boost::any/*Time*/ visit (Udata &n); ///< Throws parsing error if UDATA object is encountered
};

} // CcnbParser
} // wire

NDN_NAMESPACE_END

#endif // _CCNB_PARSER_TIMESTAMP_VISITOR_H_
