/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Zhenkai Zhu <zhenkai@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "name.h"

#include "ndn.cxx/error.h"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/algorithm/string.hpp>

#include <ctype.h>

using namespace std;

namespace ndn
{

///////////////////////////////////////////////////////////////////////////////
//                              CONSTRUCTORS                                 //
///////////////////////////////////////////////////////////////////////////////

Name::Name ()
{
}

Name::Name (const string &uri)
{ 
  string::const_iterator i = uri.begin ();
  string::const_iterator end = uri.end ();

  string::const_iterator firstSlash = std::find (i, end, '/');
  if (firstSlash == end)
    {
      BOOST_THROW_EXCEPTION (error::Name ()
                             << error::msg ("Name should include at least one slash (did you forget to specify initial /?)"));
    }

  if (firstSlash != i)
    {
      string schema (i, firstSlash);
      if (*schema.rbegin () != ':')
        {
          BOOST_THROW_EXCEPTION (error::Name ()
                                 << error::msg ("First component of the name does not start with a slash (did you forget to specify initial /?)"));
        }

      i = firstSlash;

      if (!boost::iequals (schema, "ccnx:") &&
          !boost::iequals (schema, "ndn:"))
        {
          BOOST_THROW_EXCEPTION (error::Name ()
                                 << error::msg ("URI schema is not supported (only ccnx: or ndn: is allowed)")
                                 << error::msg (schema));
        }
    }

  string::const_iterator secondSlash = i;
  secondSlash ++;
  if (secondSlash != end && *secondSlash == '/')
    {
      // The authority component (the part after the initial "//" in the familiar http and ftp URI schemes) is present,
      // but it is not relevant to NDN name.
      // skipping it
      secondSlash ++;
      i = std::find (secondSlash, end, '/');
    }
  
  if (i == end)
    {
      BOOST_THROW_EXCEPTION (error::Name ()
                             << error::msg ("Invalid URI")
                             << error::msg (uri));
    }

  while (i != end)
    {
      // skip any extra slashes
      while (i != end && *i == '/')
        {
          i ++;
        }
      if (i == end)
        break;
      
      string::const_iterator endOfComponent = std::find (i, end, '/');
      append (name::Component (i, endOfComponent));

      i = endOfComponent;
    }
}

Name::Name (const Name &other)
{
  m_comps = other.m_comps;
}

Name::Name (const unsigned char *data, const ccn_indexbuf *comps)
{
  for (unsigned int i = 0; i < comps->n - 1; i++)
  {
    const unsigned char *compPtr;
    size_t size;
    ccn_name_comp_get(data, comps, i, &compPtr, &size);

    append (name::Component (compPtr, size));
  }
}

Name::Name (const void *buf, const size_t length)
{
  ccn_indexbuf *idx = ccn_indexbuf_create();
  const ccn_charbuf namebuf = { length, length, const_cast<unsigned char *> (reinterpret_cast<const unsigned char *> (buf)) };
  ccn_name_split (&namebuf, idx);

  const unsigned char *compPtr = NULL;
  size_t size = 0;
  int i = 0;
  while (ccn_name_comp_get(namebuf.buf, idx, i, &compPtr, &size) == 0)
    {
      append (name::Component (compPtr, size));
      i++;
    }
  ccn_indexbuf_destroy(&idx);
}

Name::Name (const Charbuf &buf)
{
  ccn_indexbuf *idx = ccn_indexbuf_create();
  ccn_name_split (buf.getBuf (), idx);

  const unsigned char *compPtr = NULL;
  size_t size = 0;
  int i = 0;
  while (ccn_name_comp_get(buf.getBuf ()->buf, idx, i, &compPtr, &size) == 0)
    {
      append (name::Component (compPtr, size));
      i++;
    }
  ccn_indexbuf_destroy(&idx);
}

Name::Name (const ccn_charbuf *buf)
{
  ccn_indexbuf *idx = ccn_indexbuf_create();
  ccn_name_split (buf, idx);

  const unsigned char *compPtr = NULL;
  size_t size = 0;
  int i = 0;
  while (ccn_name_comp_get(buf->buf, idx, i, &compPtr, &size) == 0)
    {
      append (name::Component (compPtr, size));
      i++;
    }
  ccn_indexbuf_destroy(&idx);
}

Name &
Name::operator= (const Name &other)
{
  m_comps = other.m_comps;
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//                                SETTERS                                    //
///////////////////////////////////////////////////////////////////////////////

Name &
Name::appendVersion (uint64_t version/* = Name::nversion*/)
{
  if (version != Name::nversion)
    return appendNumberWithMarker (version, 0xFD);
  else
    {
      boost::posix_time::time_duration now (boost::posix_time::microsec_clock::universal_time () -
                                            boost::posix_time::ptime (boost::gregorian::date (1970, boost::gregorian::Jan, 1)));
      version = (now.total_seconds () << 12) | (0xFFF & (now.fractional_seconds () / 244 /*( 1000,000 microseconds / 4096.0 resolution = last 12 bits)*/));
      return appendNumberWithMarker (version, 0xFD);
    }
}


///////////////////////////////////////////////////////////////////////////////
//                                GETTERS                                    //
///////////////////////////////////////////////////////////////////////////////

const name::Component &
Name::get (int index) const
{
  if (index < 0)
    {
      index = size () - (-index);
    }

  if (static_cast<unsigned int> (index) >= size ())
    {
      BOOST_THROW_EXCEPTION (error::Name ()
                             << error::msg ("Index out of range")
                             << error::pos (index));
    }
  return m_comps [index];
}

name::Component &
Name::get (int index)
{
  if (index < 0)
    {
      index = size () - (-index);
    }

  if (static_cast<unsigned int> (index) >= size())
    {
      BOOST_THROW_EXCEPTION (error::Name ()
                             << error::msg ("Index out of range")
                             << error::pos (index));
    }
  return m_comps [index];
}


/////
///// Static helpers to convert name component to appropriate value
/////


Name
Name::getSubName (size_t pos/* = 0*/, size_t len/* = Name::npos*/) const
{
  Name retval;

  if (len == npos)
    {
      len = size () - pos;
    }

  if (pos + len > size ())
    {
      BOOST_THROW_EXCEPTION (error::Name ()
                             << error::msg ("getSubName parameter out of range")
                             << error::pos (pos)
                             << error::pos (len));
    }

  for (size_t i = pos; i < pos + len; i++)
    {
      retval.append (get (i));
    }

  return retval;
}

Name
Name::operator+ (const Name &name) const
{
  Name newName;
  newName
    .append (*this)
    .append (name);

  return newName;
}

std::string
Name::toUri () const
{
  ostringstream os;
  toUri (os);
  return os.str ();
}

void
Name::toUri (std::ostream &os) const
{
  for (Name::const_iterator comp = begin (); comp != end (); comp++)
    {
      os << "/";
      comp->toUri (os);
    }
  if (size () == 0)
    os << "/";
}

// ostream &
// operator << (ostream &os, const Name &name)
// {
//   for (Name::const_iterator comp = name.begin (); comp != name.end (); comp++)
//     {
//       os << "/" << *comp;
//     }
//   if (name.size () == 0)
//     os << "/";
//   return os;
// }

int
Name::compare (const Name &name) const
{
  Name::const_iterator i = this->begin ();
  Name::const_iterator j = name.begin ();

  for (; i != this->end () && j != name.end (); i++, j++)
    {
      int res = i->compare (*j);
      if (res == 0)
        continue;
      else
        return res;
    }

  if (i == this->end () && j == name.end ())
    return 0; // prefixes are equal

  return (i == this->end ()) ? -1 : +1;
}

} // ndn