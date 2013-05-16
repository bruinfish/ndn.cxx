/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_SIGNATURE_SHA256_WITH_RSA_H
#define NDN_SIGNATURE_SHA256_WITH_RSA_H

#include "signature.h"

#include "ndn.cxx/fields/blob.h"
#include "ndn.cxx/fields/key-locator.h"

#include <string>

namespace ndn {

namespace signature {

/**
 * @brief Class providing operations to work with SHA256withRSA (OID: "2.16.840.1.101.3.4.2.1")
 */
class Sha256WithRsa : public virtual Signature
{
public:
  /**
   * @brief Virtual destructor
   */
  virtual
  ~Sha256WithRsa ();

  /**
   * @brief Get OID of the signature algorithm
   */
  inline const std::string &
  getDigestAlgorithm () const;

  inline Blob &
  getSignatureBits ();

  inline const Blob &
  getSignatureBits () const;

  inline void
  setSignatureBits (const Blob &signatureBits);

  inline Blob &
  getPublisherKeyDigest ();

  inline const Blob &
  getPublisherKeyDigest () const;

  inline void
  setPublisherKeyDigest (const Blob &publisherKeyDigest);

  inline KeyLocator &
  getKeyLocator ();

  inline const KeyLocator &
  getKeyLocator () const;

  inline void
  setKeyLocator (const KeyLocator &keyLocator);
  
private:
  static const std::string s_oid;

  Blob m_signatureBits;
  Blob m_publisherKeyDigest;
  KeyLocator m_keyLocator;
};

const std::string &
Sha256WithRsa::getDigestAlgorithm () const
{
  return s_oid;
}

inline Blob &
Sha256WithRsa::getSignatureBits ()
{
  return m_signatureBits;
}

inline const Blob &
Sha256WithRsa::getSignatureBits () const
{
  return m_signatureBits;
}

inline void
Sha256WithRsa::setSignatureBits (const Blob &signatureBits)
{
  m_signatureBits = signatureBits;
}

inline Blob &
Sha256WithRsa::getPublisherKeyDigest ()
{
  return m_publisherKeyDigest;
}

inline const Blob &
Sha256WithRsa::getPublisherKeyDigest () const
{
  return m_publisherKeyDigest;
}

inline void
Sha256WithRsa::setPublisherKeyDigest (const Blob &publisherKeyDigest)
{
  m_publisherKeyDigest = publisherKeyDigest;
}

inline KeyLocator &
Sha256WithRsa::getKeyLocator ()
{
  return m_keyLocator;
}

inline const KeyLocator &
Sha256WithRsa::getKeyLocator () const
{
  return m_keyLocator;
}

inline void
Sha256WithRsa::setKeyLocator (const KeyLocator &keyLocator)
{
  m_keyLocator = keyLocator;
}

} // signature
} // ndn

#endif // NDN_EXCLUDE_H
