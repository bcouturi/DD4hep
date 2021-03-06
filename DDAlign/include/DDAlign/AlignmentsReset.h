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
#ifndef DD4HEP_DDALIGN_ALIGNMENTRESET_H
#define DD4HEP_DDALIGN_ALIGNMENTRESET_H

// Framework includes
#include "DD4hep/Alignments.h"
#include "DD4hep/Conditions.h"
#include "DD4hep/Detector.h"
#include "DD4hep/Printout.h"

/// Namespace for the AIDA detector description toolkit
namespace DD4hep {

  /// Namespace for the geometry part of the AIDA detector description toolkit
  namespace Conditions {
    // Forward declarations
    class ConditionsSlice;
  }
  
  /// Namespace for the geometry part of the AIDA detector description toolkit
  namespace Alignments {
  
    // Forward declarations
    class AlignmentsUpdateCall;

    /// Reset alignment dependencies from conditions
    /**
     *
     *   \author  M.Frank
     *   \version 1.0
     *   \date    31/03/2016
     *   \ingroup DD4HEP_DDALIGN
     */
    class AlignmentsReset : public DetElement::Processor {
    public:
      /// Shortcut the ConditionsSlice type
      typedef Conditions::ConditionsSlice Slice;
    public:
      /// Reference to the alignment manager object
      Slice&                 slice;
      /// The callback to be registered for the update mechanism
      AlignmentsUpdateCall*  updateCall;
      /// Extension property to construct the name of the alignment condition
      std::string            extension;
      /// Name of the alignment alias for the detector elements alignment object
      std::string            alias;
      /// Flag if an alias to the real alignment object should be registered
      bool                   haveAlias;
      /// Print level
      PrintLevel             printLevel;
      
      /// Initializing constructor
      AlignmentsReset(Slice& slice, AlignmentsUpdateCall* call);
      /// Default destructor
      virtual ~AlignmentsReset();
      /// Callback to output conditions information
      virtual int processElement(DetElement de);
      /// Overloadable: call to construct the alignment conditions name.
      /**
       *   Specialize for user defined implementation.
       *
       *   Default implementation returns "cond.name()+extension".
       *   Please note, that the corrsponding implementation of
       *   'AlignmentsForward::construct_name' must match
       */
      virtual std::string construct_name(DetElement de, Conditions::Condition cond) const;
    };
  }       /* End namespace Alignments              */
}         /* End namespace DD4hep                  */
#endif    /* DD4HEP_DDALIGN_ALIGNMENTRESET_H       */
