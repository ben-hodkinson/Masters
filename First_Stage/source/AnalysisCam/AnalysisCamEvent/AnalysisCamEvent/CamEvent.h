#ifndef ANALYSISCAM_CAMEVENT_H
#define ANALYSISCAM_CAMEVENT_H

#include "CamObject.h"
#include "CamDeque.h"
#include "CamVector.h"

#include "xAODEventInfo/EventInfo.h"

#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/ISystematicsTool.h"

#include "AthAnalysisBaseComps/AthAnalysisHelper.h"

class AthAlgorithm;class AthAlgTool;

//CamEvent object can have parents, but the event() method stops here

class CamEvent: public CamObject {

   public:
      static StatusCode readOptions(const std::string& options) {
        return AAH::joSvc->readOptions(options,"$JOBOPTSEARCHPATH");
      }
      
      static StatusCode configureWriter(const std::string& writerName, const std::string& treeName, const std::vector<std::string>& branches = {}) {
        if(writerName.find("__")!=std::string::npos) {
          //services with __ in their name have troubles, so we will auto-sanitize this ...
          std::string sanitizedWriterName(writerName);
          std::string::size_type n = 0;
          while( ( n=sanitizedWriterName.find("__",n) ) != std::string::npos ) {
            sanitizedWriterName.replace(n,2,"_");
            n += 1;
          }
          return configureWriter(sanitizedWriterName,treeName,branches);
        }
        if( AAH::setProperty( writerName , "TreeName" , treeName ).isFailure() ) return StatusCode::FAILURE;
        if( AAH::setProperty( writerName , "Branches", branches ).isFailure() ) return StatusCode::FAILURE;
        return StatusCode::SUCCESS;
      }

      using CamObject::operator=;

      static CP::SystematicSet getSystematics(const std::string& toolName);

      static bool isSimulation(CamObject& c); //helper method for CamEvent that use xAOD::EventInfo

      static bool AutoCreateObjects; //says whether objects/deques should be made in get calls
      CamEvent();
      CamEvent(AthAlgorithm* caller);
      CamEvent(AthAlgTool* caller);
      ~CamEvent();

      virtual CamEvent* event() { return this; }

      void clear();

      bool hasVector(const std::string& name);
      bool hasDeque(const std::string& name);
      bool hasObject(const std::string& name);

      CamVector* createVector(const std::string& name,const xAOD::IParticleContainer* cont=0);
      CamDeque* createDeque(const std::string& name, const xAOD::IParticleContainer* cont=0);
      CamObject* createObject(const std::string& name);

      CamVector* addVector(CamVector *vector,std::string name="");
      CamDeque* addDeque(CamDeque *deque,std::string name="");
      CamObject* addObject(CamObject *object,std::string name="");

      CamVector* getVector(const std::string& name);
      CamDeque* getDeque(const std::string& name);
      CamObject* getObject(const std::string& name);
      
      //methods for working with CamTools that provide objects/collections, potentially with a systematic variation
      CamVector* getVector(const std::string& toolName, const CP::SystematicSet& syst);
      CamDeque* getDeque(const std::string& toolName, const CP::SystematicSet& syst);
      
      CamVector* getVector(const std::string& toolName, const CP::SystematicVariation& syst) { return getVector(toolName, CP::SystematicSet({syst})); }
      CamDeque* getDeque(const std::string& toolName, const CP::SystematicVariation& syst) { return getDeque(toolName, CP::SystematicSet({syst})); }

      //special accessor for MET objects
      CamObject* getMETObject(const std::string& containerName, const std::string& term="Final");

      void changeName(CamObject* obj, const std::string& newName); ///change the name of something in the event

      const std::string& getObjectName(CamObject* obj); ///find the name of this object

      virtual std::string applyTool(const std::string& toolName,std::string label="",const std::string& option="");
      virtual std::string applyService(const std::string& toolName,std::string label="",const std::string& option="");

      //typecast operators for CamEvent -> xAOD::EventInfo .. warning: potentially unsafe!!!
      operator const xAOD::EventInfo& () { return *(static_cast<const xAOD::EventInfo*>(m_xAODParticle)); }
      operator const xAOD::EventInfo* () { return (static_cast<const xAOD::EventInfo*>(m_xAODParticle)); }

      std::map<std::string,CamObject*>* getObjectMap() { return &m_objects; }
   protected:
      //std::map<std::string,CamDeque> m_deques;

      std::map<std::string,CamObject*> m_objects;
      
   public:
      void print( const std::string& propsToShow="", bool callDynamic=true );

      void reinit(); //does a clear and tries to re-retrieve the EventInfo

        ClassDef(CamEvent,1)
};
CLASS_DEF( CamEvent , 115756999 , 1 )


#endif
