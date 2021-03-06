//==========================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================

// Framework includes
#include "DD4hep/Printout.h"
#include "DD4hep/DetConditions.h"
#include "DD4hep/ConditionsPrinter.h"
#include "DD4hep/objects/ConditionsInterna.h"

using namespace DD4hep;
using namespace DD4hep::Conditions;

/// Initializing constructor
ConditionsPrinter::ConditionsPrinter(UserPool* p, const std::string& pref, int flg)
  : ConditionsProcessor(p), name("Condition"), prefix(pref), printLevel(INFO), m_flag(flg)
{
}

/// Initializing constructor
ConditionsPrinter::ConditionsPrinter(const std::string& pref, int flg)
  : ConditionsProcessor(0), name("Condition"), prefix(pref), printLevel(INFO), m_flag(flg)
{
}

/// Callback to output conditions information
int ConditionsPrinter::operator()(Condition cond)    {
  if ( cond.isValid() )   {
    std::string repr = cond.str(m_flag);
    if ( repr.length() > 100 )
      printout(this->printLevel,name,
               "++ %s%s", prefix.c_str(), repr.c_str());
    else
      printout(this->printLevel,name,"++ %s%s [%p]",
               prefix.c_str(), repr.c_str(), cond.ptr());
    const OpaqueData& data = cond.data();
    std::string values = data.str();
    if ( values.length() > 132 ) values = values.substr(0,130)+"...";
    std::string new_prefix = prefix;
    new_prefix.assign(prefix.length(),' ');
    printout(this->printLevel,name,"++ %s \tPath:%s Key:%16llX Type:%s",
             new_prefix.c_str(), cond.name(), cond.key(), data.dataType().c_str());
    printout(this->printLevel,name,"++ %s \tData:%s", new_prefix.c_str(), values.c_str());
  }
  return 1;
}

/// Container callback for object processing
int ConditionsPrinter::operator()(Container container)   {
  if ( m_pool )  {
    printout(this->printLevel,name,"++ Conditions of DE %s [%d entries]",
             container->detector.path().c_str(), int(container.keys().size()));
    for(const auto& k : container.keys() )  {
      Condition c = container.get(k.first,*m_pool);
      std::string nam = c.name();
      std::string cn = nam.substr(nam.find('#')+1);
      Condition::key_type key = ConditionKey::hashCode(cn);
      printout(this->printLevel,name,"++ %s %s %s [%16llX] -> %s [%16llX]",
               prefix.c_str(), "Condition:", cn.c_str(), key==k.first ? key : k.first,
               c.name(), k.second.first);
      (*this)(c);
    }
    return 1;
  }
  except(name,"Cannot dump conditions container without user-pool.");
  return 0;
}
