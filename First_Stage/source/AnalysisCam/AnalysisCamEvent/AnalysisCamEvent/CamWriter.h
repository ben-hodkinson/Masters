
#ifndef _CAMWRITER_
#define _CAMWRITER_

// includes
#include "AnalysisCamEvent/CamService.h"
#include "GaudiKernel/IIncidentSvc.h"

#include <typeinfo>
#include <TString.h>
#include <TTree.h>
#include <TLeaf.h>

/*
template <typename T> struct has_iterator {
    template <typename U> static char test(typename U::iterator* x);
    template <typename U> static long test(U* x);
    static const bool value = sizeof(test<T>(0)) == 1;
};*/

///templated object to separate things that are storegate-writable (have ClassID_traits) and those that don't
template<typename W, bool B> class SGObject {
   public: 
      static bool record(ServiceHandle<StoreGateSvc>&, const std::string&, W*) {
         //not writable so do nothing
         return true;
      }
      static void* newObject() { 
         if(std::is_arithmetic<W>::value) { return new W(); }
         return 0; 
      }
      static void deleteObject(void* obj) {
         if(!obj) return;
         if(std::is_arithmetic<W>::value) { delete static_cast<W*>(obj); }
      }
};

template<typename W> class SGObject<W,true> { //this specialization only gets called for things that can be written to storegate!
   public: 
      static bool record(ServiceHandle<StoreGateSvc>& sg, const std::string& key, W* pointer) {
         //attempt to record it 
         if(sg->record(pointer,key).isFailure()) return false;
         if(sg->setConst(pointer).isFailure()) return false;
         return true;
      }
      static void* newObject() { return new W(); } //assumes all writable things have a default constructor!!
      static void deleteObject(void* obj) {
         if(!obj) return;
         delete static_cast<W*>(obj);
      }
};



class TLorentzVector;

class TBranch;

class CamWriter : virtual public CamService, virtual public IIncidentListener {

  public:
      using CamService::apply;


      typedef TBranch* (CamWriter::*branchMakerMethod)(TString,TString,TString);
      static std::map<TString,branchMakerMethod> branchMakerMethods;

      template<typename W> static void registerType() {
         //demangle the type 

         TString type = System::typeinfoName(typeid(W));
         type.ReplaceAll("std::","");
         type.ReplaceAll(" ","");
         type.ReplaceAll("unsigned","unsigned ");
         type.ReplaceAll("longlong","long long");
         TString vectorType = TString::Format("vector<%s>",type.Data());
         TString vectorType2 = TString::Format("vector<%s,allocator<%s>>",type.Data(),type.Data());
         TString vectorvectorType = TString::Format("vector<vector<%s>>",type.Data());
         TString vectorvectorType2 = TString::Format("vector<vector<%s>,allocator<vector<%s>>>",type.Data(),type.Data());
         //register it, and it's vector version

         branchMakerMethods[type] = &CamWriter::setupCamBranch<W>;
         branchMakerMethods[vectorType] = &CamWriter::setupCamBranch<std::vector<W>>;
         branchMakerMethods[vectorType2] = &CamWriter::setupCamBranch<std::vector<W>>;
         branchMakerMethods[vectorvectorType] = &CamWriter::setupCamBranch<std::vector<std::vector<W>>>;
         branchMakerMethods[vectorvectorType2] = &CamWriter::setupCamBranch<std::vector<std::vector<W>>>;
         
      }

      // Constructors, destructor
      //CamWriter(const std::string& type, const std::string& name,const IInterface* pParent);
      CamWriter(const std::string& name, ISvcLocator *svcLoc);
   
      virtual ~CamWriter();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      void handle(const Incident& incident);

      virtual StatusCode sysInitialize();
      virtual StatusCode initialize();
      virtual StatusCode finalize();

      virtual StatusCode apply(CamObject* object,std::string& label, const std::string& option);
      virtual StatusCode apply(CamDeque* deque,std::string& label, const std::string& option);
      virtual StatusCode apply(CamEvent* event,std::string& label, const std::string& option);
      //virtual StatusCode apply(CamVector* vector, std::string& label, std::string option);

   private:
      struct CamBranch {
         TBranch* tBranch;
         std::function<void()> clear; //function pointer to how to 'clear' this branch ... simplest case is just zero the address
         std::function<bool(CamObject&,const std::string&,bool,bool)> fillFromCamObject; //method that extracts value for this branch using a string key (may not be same as branch name) and assigns to myObject
         std::function<bool(CamDeque&,const std::string&,bool,bool)> fillFromCamDeque; //extracts value from camdeque, probably via the all_get method, returning true if successful 
         std::function<void()> deleteObject; //called at cleanup time!
         void** voidAddress; //the address of the void* address .. should always equal branch.GetAddress()
         bool recordingToStoregate; //indicates if must 'copy' the contents into voidAddress. If not recording, we can just temporarily direct
         bool isBasicType;
         std::string branchType; //the branchType string 
         int vectorDoubleOrFloat; //0 = neither, 1=vector double, 2=vector float
         bool doNotFill; //should we bypass the fill 
      };

      void fillTree();

      template<typename W> void addBranchFromStoregate(std::string branchT) {
         std::vector<std::string> keys;
         evtStore()->keys<W>(keys);
         //check if its a good branch and add it if it is
         std::vector<std::string>::iterator end = keys.end();
         for(std::vector<std::string>::iterator it=keys.begin();it!=end;++it) {
            if(isGoodBranch(*it)) { addBranch(*it,branchT); }
         }
      }

      TBranch* addBranch(std::string branchName, std::string branchT);
      bool isGoodBranch(TString branchName);
      int wildcmp(const char *wild, const char *string);

      std::vector<bool>& getTLVFormatVec(std::string& label, const std::string& option);


      void FillDoubleVector(CamBranch* b, const std::string& branchName,double (TLorentzVector::*funcPtr)() const,double scaleFactor, CamDeque* deque, bool createIfNotExisting=true);

      void AddTLV(std::string branchName,TLorentzVector* value);
      void AddDouble(CamBranch* b, const std::string&,double value);


      template<typename W> TBranch* setupCamBranch(TString branchName,TString branchTypeString, TString branchAlias="") {
          TString outputBranchName = (branchAlias.Length()>0)?branchAlias:branchName;

         
         if(m_camBranches.find(branchName)!=m_camBranches.end()) {
            ATH_MSG_ERROR("Branch " << branchName << " already defined as type: " << m_camBranches[branchName].tBranch->GetClassName() << "(" << m_camBranches[branchName].tBranch->GetLeaf(branchName)->GetTypeName() << ")");
            return 0;
         }

         //create new CamBranch 
         CamBranch& myBranch = m_camBranches[branchName];
         myBranch.branchType = branchTypeString.Data();

         if(branchTypeString=="bool") branchTypeString =outputBranchName+"/O";
         else if(branchTypeString=="unsigned int") branchTypeString =outputBranchName+"/i";
         else if(branchTypeString=="int") branchTypeString =outputBranchName+"/I";
         else if(branchTypeString=="float") branchTypeString =outputBranchName+"/F";
         else if(branchTypeString=="double") branchTypeString =outputBranchName+"/D";
         else if(branchTypeString=="long") branchTypeString=outputBranchName+"/L";
         else if(branchTypeString=="unsigned long") branchTypeString=outputBranchName+"/l";
         else if(branchTypeString=="unsigned long long") branchTypeString=outputBranchName+"/l";
         else if(branchTypeString=="char") branchTypeString=outputBranchName+"/B";
         else if(branchTypeString=="unsigned char") branchTypeString=outputBranchName+"/b";


         std::string type = System::typeinfoName(typeid(W));
         ATH_MSG_DEBUG("Creating branch " << branchName << " with ROOT type " << myBranch.branchType << " and C++ type " << type);

         m_voidStars.push_back(SGObject<W,(ClassID_traits<W>::s_version)>::newObject()); //pushes a zero if not storegate writable (the zero still has an address though)
         void** addressOfVoidStar = &(m_voidStars.back()); //this is the address of the void*
         myBranch.voidAddress = addressOfVoidStar; 
         myBranch.deleteObject = std::bind(SGObject<W,(ClassID_traits<W>::s_version)>::deleteObject,*(myBranch.voidAddress));
         myBranch.tBranch=0;
         myBranch.tBranch = (branchTypeString.Contains("/")) ? m_tree->Branch(outputBranchName,*addressOfVoidStar,branchTypeString): //basic types want the pointer to basic type, not address of a pointer
                  m_tree->Branch(outputBranchName,branchTypeString,addressOfVoidStar,32000,0);

         
         myBranch.doNotFill = m_doNotFill;//signals never to actually call fill .. done when not writing to a tree after all
        //std::cout << "address=" << myBranch.voidAddress << std::endl;

         if(myBranch.tBranch==0) {
            //bad branch type 
            ATH_MSG_ERROR("Unable to create branch " << branchName << " of type:" << myBranch.branchType << ". Please add this to blacklist");
            m_camBranches.erase(branchName);
            throw 10;
            return 0;
         }

         myBranch.isBasicType = std::is_arithmetic<W>::value; //have to do a proper 'copy' always!


         //std::cout << "isBasicType = " << myBranch.isBasicType << std::endl;

         //setup a clearing method
         if(!(ClassID_traits<W>::s_version) && !myBranch.isBasicType) { //not storegate writable or basic type, so the voidAddress is just an address ... so we use the simpleClearMethod
            myBranch.clear = std::bind(simpleClearMethod,myBranch.voidAddress);
         } else { 
            myBranch.clear = std::bind(camdequefiller<W>::clearMethod,myBranch.voidAddress);
/*
            //decide from branchTypeString if it's a vector type branch or not ... vector type branches begin with vector or std::vector 
            if(branchTypeString.BeginsWith("std::vector<") || branchTypeString.BeginsWith("vector<")) {
               
            } else {
               myBranch.clear = std::bind(defaultConstructorClearMethod<W>,myBranch.voidAddress);
            }
*/
         }

         //decide if vector double or vector float or neither here as well
         //used when populating the tlv branches
         if(branchTypeString=="vector<double>" || branchTypeString=="vector<double,allocator<double>>") myBranch.vectorDoubleOrFloat=1;
         else if(branchTypeString=="vector<float>" || branchTypeString=="vector<double,allocator<float>>") myBranch.vectorDoubleOrFloat=2;
         else myBranch.vectorDoubleOrFloat=0;

         //setup assignment methods ...  
         myBranch.fillFromCamObject = std::bind(fillFromCamObjectMethod<W>,myBranch,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
         myBranch.fillFromCamDeque = std::bind(camdequefiller<W>::fillFromCamDequeMethod,myBranch,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);

         if(myBranch.tBranch) {
            if(!m_doNotFill) {
               for(long i=0;i<nEntries;i++) {
                  if(myBranch.tBranch->Fill()<=0) {
                     ATH_MSG_FATAL("Fill of " << myBranch.tBranch->GetName() << " wrote no or negative bytes! Disk space issue?"); throw 10; return 0;
                  } 
               }
            }
            if(m_writeToStoregate) {
               W* tmp = static_cast<W*>(*(myBranch.voidAddress) ); //will be ok for the storegate writeables!
               SGObject<W,(ClassID_traits<W>::s_version)>::record(evtStore(),outputBranchName.Data(),tmp);
               if(ClassID_traits<W>::s_version) myBranch.recordingToStoregate=true;
            }
            myBranch.tBranch->ResetBit(3333); //we use this bit to monitor if the branch is ever filled
         }
         return myBranch.tBranch;
      }


      static void simpleClearMethod(void** address) { *address = 0; } //use just to zero the address
      //template<typename W> static void defaultConstructorClearMethod(void** address)
      //template<typename W> static void clearMethodClearMethod(void** address) {}

      template <typename T> struct camdequefiller {
         static bool fillFromCamDequeMethod(CamBranch&,CamDeque&,const std::string&,bool,bool) { return false; }
         static void clearMethod(void** address)  { *(static_cast<T*>(*address)) = T(); } //use on objects that can be 'assigned' from default constructor ... hopefully move constructor defined
      };

      //vector specific versions of these methods
      template <typename W> struct camdequefiller<std::vector<W> > {
        static bool fillFromCamDequeMethod(CamBranch& cBranch,CamDeque& deque,const std::string& key,bool checkMap,bool checkStoregate) {
            bool isValid(false);
            std::vector<W>&& myVec = deque.all_valid_get<W>(key,isValid,checkMap,checkStoregate);
            if(isValid) CamWriter::branchFiller<std::vector<W>>(cBranch,std::move(myVec));
            return isValid;
         }
         static void clearMethod(void** address) { (static_cast<std::vector<W>*>(*address))->clear();}
      };
/*
      template <typename W> static typename std::enable_if<!has_iterator<W>::value, bool>::type fillFromCamDequeMethod(CamBranch&,CamDeque&,const std::string&,bool,bool) {
         return false;
      }

      template<typename W> static typename std::enable_if<has_iterator<W>::value, bool>::type fillFromCamDequeMethod(CamBranch& cBranch, CamDeque& deque, const std::string& key, bool checkMap, bool checkStoregate) {
         bool isValid(false);
         typedef typename W::value_type WvalueType;
         W&& myVec = deque.all_valid_get<WvalueType>(key,isValid,checkMap,checkStoregate);
      }
*/
      template<typename W> static bool fillFromCamObjectMethod(CamBranch& cBranch, CamObject& obj, const std::string& key, bool checkMap, bool checkStoregate) {
         bool isValid(false);
         if(cBranch.tBranch->TestBit(kDoNotProcess)) return isValid;
         W&& myObject = obj.valid_get<W>(key,isValid,checkMap,checkStoregate); 
         if(isValid) branchFiller<W>(cBranch,std::move(myObject));
         return isValid;
      }

      //often T and W will be the same type, but just for cases off allowing a type variation (e.g. allow float to be set from a double)
     template<typename T, typename W> static void branchFiller(CamBranch& cBranch, W&& myObject) {
         if(cBranch.tBranch->TestBit(kDoNotProcess)) return;
         if(cBranch.recordingToStoregate || cBranch.isBasicType) { //must make a true copy
            void* tmpAddr = *(cBranch.voidAddress);
            T* obj = static_cast<T*>( tmpAddr );
            *obj = std::move(myObject); //moves it in instead of copying  can do with our rvalue reference to a temporary copy
            if(!cBranch.doNotFill) {
               if(cBranch.tBranch->Fill()<=0) { 
                  std::cout << "Fill of " << cBranch.tBranch->GetName() << " wrote no (or negative) bytes! Disk space issue?" << std::endl;; 
                  throw 10;
               }
            }
         } else if(!cBranch.doNotFill) { //only do this writing when actually writing to branches
            //can just point our address that this reference, and write from that!
            void* tmp = cBranch.tBranch->GetAddress();
            void* tmp2 = &myObject;
            cBranch.tBranch->SetAddress(&tmp2); //the branch address is now the address of tmp2 .. will need to swap that back
            if(cBranch.tBranch->Fill()<=0) { 
               std::cout << "Fill of " << cBranch.tBranch->GetName() << " wrote no (or negative) bytes! Disk space issue?" << std::endl; 
               throw 10;
            }
            cBranch.tBranch->SetAddress(tmp); //put the address back to the address of the void* in the deque
         }
         cBranch.tBranch->SetStatus(0);
         cBranch.tBranch->SetBit(3333); //indicates this branch has been filled
     }

      std::map<TString, CamBranch> m_camBranches;
      std::deque<void*> m_voidStars; //use a deque to hold the addresses because it's very important that the addresses of these addresses never change ... TBranch always looks at same location


      bool m_doNotFill;
      bool m_ownsTree;

      double m_tlvScaleFactor;

      long nEntries;
      bool m_predefinedBranches;
      std::string m_tlvFormat;
      std::string m_tlvPrefix;
      std::string m_streamName;
      bool m_fillEveryEvent;
      bool m_eventWritesObjects;
      std::vector<std::string> m_blacklist;
      std::vector<std::string> m_whitelist;
      bool m_writeToStoregate;
      std::vector<std::string> m_treeConfig;

      bool m_eventHasStuff; //signal if an apply has been called on this event

      TTree* m_tree;

      std::map<std::string,bool> m_badBranches; //cache of the rejected branches



      std::map<std::string,std::vector<bool> > m_options_tlvFormat;




      std::map<TString,bool> m_nonZeroDequeFilled; //monitors when the deques "_n" branch is filled with a nonzero value

};





#endif