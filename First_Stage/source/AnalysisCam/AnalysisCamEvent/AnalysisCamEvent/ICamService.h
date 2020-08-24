#ifndef _ICAMSERVICE_
#define _ICAMSERVICE_


//----------------------------------------------------------
//object definition interface
//----------------------------------------------------------
#include "GaudiKernel/IService.h"

class CamObject;
class CamDeque;
class CamEvent;
class CamVector;

static const InterfaceID IID_ICamService( "ICamService", 1, 0 );

class ICamService : virtual public IService {

   public:
      static const InterfaceID& interfaceID() { return IID_ICamService; };

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


      virtual void* get(std::string name="")=0;

};

#endif