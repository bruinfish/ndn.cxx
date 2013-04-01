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

#ifndef CCNX_CERT_H
#define CCNX_CERT_H

#include "ccnx-common.h"
#include "ccnx-name.h"
#include "ccnx-pco.h"
#include "hash-helper.h"
#include <boost/shared_ptr.hpp>

namespace Ccnx {

class Cert
{
public:
  enum VALIDITY
  {
    NOT_YET_VALID,
    WITHIN_VALID_TIME_SPAN,
    EXPIRED,
    OTHER
  };

  Cert();
  Cert(const PcoPtr &keyObject, const PcoPtr &metaObject);
  ~Cert();

  void
  updateMeta(const PcoPtr &metaObject);

  Name
  name() { return m_name; }

  Bytes
  rawKeyBytes() { return m_rawKeyBytes; }

  Hash
  keyDigest() { return m_keyHash; }

  std::string
  realworldID() { return m_meta.realworldID; }

  std::string
  affilication() { return m_meta.affiliation; }

  ccn_pkey *
  pkey() { return m_pkey; }

  VALIDITY
  validity();

private:
  struct Meta
  {
    Meta(std::string id, std::string affi, time_t from, time_t to)
      : realworldID(id)
      , affiliation(affi)
      , validFrom(from)
      , validTo(to)
    {
    }
    std::string realworldID;
    std::string affiliation;
    time_t validFrom;
    time_t validTo;
  };

  Name m_name;
  Hash m_keyHash; // publisherPublicKeyHash
  Bytes m_rawKeyBytes;
  ccn_pkey *m_pkey;
  Meta m_meta;
};

typedef boost::shared_ptr<Cert> CertPtr;

}

#endif // CCNX_CERT_H
