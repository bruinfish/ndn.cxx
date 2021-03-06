/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Yingdi Yu
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Yingdi Yu <yingdi@cs.ucla.edu>
 */

#include "exception.h"

using namespace std;

namespace ndn
{

namespace der
{
  DerException::DerException(const string & errMsg) throw()
    : m_errMsg(errMsg)
  {
  }

  DerException::~DerException() throw()
  {
  }
  
}//der

}//ndn
