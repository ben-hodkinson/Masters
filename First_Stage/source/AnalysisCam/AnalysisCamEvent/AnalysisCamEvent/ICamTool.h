#ifndef _ICAMTOOL_
#define _ICAMTOOL_


//----------------------------------------------------------
//object definition interface
//----------------------------------------------------------
#include "GaudiKernel/IAlgTool.h"

#include "PATInterfaces/SystematicSet.h"

class CamObject;
class CamDeque;
class CamEvent;
class CamVector;

static const InterfaceID IID_ICamTool( "ICamTool", 1, 0 );

class ICamTool : virtual public IAlgTool {

   public:
      static const InterfaceID& interfaceID() { return IID_ICamTool; };

      //label should usually be modified to match the name of the object this thing creates in the CamEvent
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option)=0;
      virtual StatusCode apply(CamDeque* deque, std::string& label, const std::string& option)=0;
      virtual StatusCode apply(CamObject* object, std::string& label, const std::string& option)=0;
      virtual StatusCode apply(CamVector* vector, std::string& label, const std::string& option)=0;

      //these ones are applies with no effective return info, other than the StatusCode
      virtual StatusCode apply(CamEvent* event, const std::string& option)=0;
      virtual StatusCode apply(CamDeque* deque, const std::string& option)=0;
      virtual StatusCode apply(CamObject* object, const std::string& option)=0;
      virtual StatusCode apply(CamVector* vector, const std::string& option)=0;


      virtual void* get(const std::string& name="")=0;


      virtual void applySystematicSet(const CP::SystematicSet& syst)=0;
      //returns the systematics registered to this CamTool during its initialization
      virtual const CP::SystematicSet& getSystematics() const = 0;

};

#endif