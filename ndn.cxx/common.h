/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *                     Zhenkai Zhu
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Zhenkai Zhu <zhenkai@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_COMMON_H
#define NDN_COMMON_H

extern "C" {
#include <ccn/ccn.h>
#include <ccn/charbuf.h>
#include <ccn/keystore.h>
#include <ccn/uri.h>
#include <ccn/bloom.h>
#include <ccn/signing.h>
}
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/exception/all.hpp>
#include <boost/function.hpp>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <string.h>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/make_shared.hpp>

namespace ndn {
typedef std::vector<unsigned char> Bytes;
typedef std::vector<std::string>Comps;

typedef boost::shared_ptr<Bytes> BytesPtr;

inline
const unsigned char *
head(const Bytes &bytes)
{
  return &bytes[0];
}

inline
unsigned char *
head (Bytes &bytes)
{
  return &bytes[0];
}

// --- Bytes operations start ---
inline void
readRaw(Bytes &bytes, const unsigned char *src, size_t len)
{
  if (len > 0)
  {
    bytes.resize(len);
    memcpy (head (bytes), src, len);
  }
}

inline BytesPtr
readRawPtr (const unsigned char *src, size_t len)
{
  if (len > 0)
    {
      BytesPtr ret (new Bytes (len));
      memcpy (head (*ret), src, len);

      return ret;
    }
  else
    return BytesPtr ();
}

template<class Msg>
BytesPtr
serializeMsg(const Msg &msg)
{
  int size = msg.ByteSize ();
  BytesPtr bytes (new Bytes (size));
  msg.SerializeToArray (head(*bytes), size);
  return bytes;
}

template<class Msg>
boost::shared_ptr<Msg>
deserializeMsg (const Bytes &bytes)
{
  boost::shared_ptr<Msg> retval (new Msg ());
  if (!retval->ParseFromArray (head (bytes), bytes.size ()))
    {
      // to indicate an error
      return boost::shared_ptr<Msg> ();
    }
  return retval;
}

template<class Msg>
boost::shared_ptr<Msg>
deserializeMsg (const void *buf, size_t length)
{
  boost::shared_ptr<Msg> retval (new Msg ());
  if (!retval->ParseFromArray (buf, length))
    {
      // to indicate an error
      return boost::shared_ptr<Msg> ();
    }
  return retval;
}


template<class Msg>
BytesPtr
serializeGZipMsg(const Msg &msg)
{
  std::vector<char> bytes;   // Bytes couldn't work
  {
    boost::iostreams::filtering_ostream out;
    out.push(boost::iostreams::gzip_compressor()); // gzip filter
    out.push(boost::iostreams::back_inserter(bytes)); // back_inserter sink

    msg.SerializeToOstream(&out);
  }
  BytesPtr uBytes = boost::make_shared<Bytes>(bytes.size());
  memcpy(&(*uBytes)[0], &bytes[0], bytes.size());
  return uBytes;
}

template<class Msg>
boost::shared_ptr<Msg>
deserializeGZipMsg(const Bytes &bytes)
{
  std::vector<char> sBytes(bytes.size());
  memcpy(&sBytes[0], &bytes[0], bytes.size());
  boost::iostreams::filtering_istream in;
  in.push(boost::iostreams::gzip_decompressor()); // gzip filter
  in.push(boost::make_iterator_range(sBytes)); // source

  boost::shared_ptr<Msg> retval = boost::make_shared<Msg>();
  if (!retval->ParseFromIstream(&in))
    {
      // to indicate an error
      return boost::shared_ptr<Msg> ();
    }

  return retval;
}


// --- Bytes operations end ---

// Exceptions
typedef boost::error_info<struct tag_errmsg, std::string> error_info_str;

} // ndn
#endif // NDN_COMMON_H
