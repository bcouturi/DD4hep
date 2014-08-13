// $Id: LCDD.h 1117 2014-04-25 08:07:22Z markus.frank@cern.ch $
//====================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------
//
//  Author     : M.Frank
//
//====================================================================
// Framework include files

#define __DD4HEP_DDEVE_EXCLUSIVE__

// C/C++ include files
#include <vector>
#include "Math/Vector3D.h"
#include "Math/VectorUtil.h"
namespace DD4hep { namespace Geometry {
    typedef ROOT::Math::XYZVector Position;
  }
}
class G4Step;
class G4StepPoint;
#include "DDG4/Geant4Data.h"
#include "DDEve/DDEveEventData.h"


#ifdef __DD4HEP_DDEVE_EXCLUSIVE__

/*
 *   DD4hep namespace declaration
 */
namespace DD4hep {

  /*
   *   Simulation namespace declaration
   */
  namespace Simulation {
    #define NO_CALL {      throw "This function shoule never ever be called!";    }
    /// Default constructor
    inline SimpleRun::SimpleRun() {    }
    /// Default destructor
    inline SimpleRun::~SimpleRun() {    }
    /// Default constructor
    inline SimpleEvent::SimpleEvent() {    }
    /// Default destructor
    inline SimpleEvent::~SimpleEvent() {    }

    /// Default constructor
    inline Particle::Particle()   {     }
    /// Copy constructor
    inline Particle::Particle(const Particle&)   {  NO_CALL   }
    /// Default destructor
    inline Particle::~Particle()   {     }
    /// Remove daughter from set
    inline void Particle::removeDaughter(int)   {   NO_CALL  }

    /// Default constructor
    inline SimpleHit::SimpleHit()   {    }
    /// Default destructor
    inline  SimpleHit::~SimpleHit()  {    }
    /// Extract the MC contribution for a given hit from the step information
    inline SimpleHit::Contribution SimpleHit::extractContribution(G4Step*) { return Contribution(); }
    /// Default constructor
    inline SimpleTracker::Hit::Hit()   {    }
    /// Initializing constructor
    inline SimpleTracker::Hit::Hit(int, int, double, double)   {}
    /// Default destructor
    inline SimpleTracker::Hit::~Hit()  {    }
    /// Assignment operator
    inline SimpleTracker::Hit& SimpleTracker::Hit::operator=(const Hit&)   { return *this; }
    /// Clear hit content
    inline SimpleTracker::Hit& SimpleTracker::Hit::clear()    { return *this; }
    /// Store Geant4 point and step information into tracker hit structure.
    inline SimpleTracker::Hit& SimpleTracker::Hit::storePoint(G4Step*, G4StepPoint*)  { return *this;}
    /// Default constructor
    inline SimpleCalorimeter::Hit::Hit()   {    }
    /// Initializing constructor
    inline SimpleCalorimeter::Hit::Hit(const Position&)  {}
    /// Default destructor
    inline SimpleCalorimeter::Hit::~Hit()   {    }
  }
}

// CINT configuration
#if defined(__MAKECINT__)

//#pragma link C++ class Position+;
//#pragma link C++ class Direction+;
//#pragma link C++ class std::set<int>+;
#pragma link C++ class DD4hep::Simulation::SimpleRun+;
#pragma link C++ class DD4hep::Simulation::SimpleEvent+;

#pragma link C++ class DD4hep::Simulation::Particle+;
#pragma link C++ class std::vector<DD4hep::Simulation::Particle*>+;

#pragma link C++ class DD4hep::Simulation::SimpleHit+;
#pragma link C++ class std::vector<DD4hep::Simulation::SimpleHit*>+;
#pragma link C++ class DD4hep::Simulation::SimpleHit::Contribution+;
#pragma link C++ class DD4hep::Simulation::SimpleHit::Contributions+;
#pragma link C++ class DD4hep::Simulation::SimpleTracker+;
#pragma link C++ class DD4hep::Simulation::SimpleTracker::Hit+;
#pragma link C++ class std::vector<DD4hep::Simulation::SimpleTracker::Hit*>+;
#pragma link C++ class DD4hep::Simulation::SimpleCalorimeter+;
#pragma link C++ class DD4hep::Simulation::SimpleCalorimeter::Hit+;
#pragma link C++ class std::vector<DD4hep::Simulation::SimpleCalorimeter::Hit*>+;
#else
#include <typeinfo>
#include "TROOT.h"
#include "TClass.h"

using namespace std;
using namespace DD4hep;
using namespace DD4hep::Simulation;

namespace {
  template <typename T> T* _fill(SimpleHit* ptr, DDEveHit* target)   {
    T* s = dynamic_cast<T*>(ptr);
    if ( s )   {
      Geometry::Position* p = &s->position;
      target->x = p->X();
      target->y = p->Y();
      target->z = p->Z();
      target->deposit = s->energyDeposit;
      return s;
    }
    return 0;
  }

  void* _convertHitFunc(void* source, DDEveHit* target)  {
    if (source )  {
      static TClass* cl_calo = gROOT->GetClass(typeid(SimpleCalorimeter::Hit));
      static TClass* cl_tracker = gROOT->GetClass(typeid(SimpleTracker::Hit));
      //static TClass* cl_particles = gROOT->GetClass(typeid(Particle));

      void* result = 0;
      SimpleHit* hit = (SimpleHit*)source;
      const std::type_info& type = typeid(*hit);
      TClass* cl = gROOT->GetClass(type);
      if ( cl == cl_tracker && (result=_fill<SimpleTracker::Hit>(hit,target)) ) return result;
      if ( cl == cl_calo && (result=_fill<SimpleCalorimeter::Hit>(hit,target)) ) return result;
    }
    return 0;
  }
  union FCN  {
    void* v;
    void* (*f)(void*, DDEveHit*);
    FCN(void* (*ff)(void*, DDEveHit*)) { f=ff; }
  };
}

static void* _convertHit(const char*)  {
  return FCN(_convertHitFunc).v;
}

#include "DD4hep/Factories.h"
using namespace DD4hep::Geometry;
DECLARE_CONSTRUCTOR(DDEve_DDG4HitAccess,_convertHit)
#endif
#endif
