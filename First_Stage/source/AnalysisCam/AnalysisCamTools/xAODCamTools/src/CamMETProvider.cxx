#include "CamMETProvider.h"


#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"

CamMETProvider::CamMETProvider(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), m_metMaker("",this)
{
   declareProperty("MuonCollection",m_muonCollection="", "Name of the muons collection");
   declareProperty("MuonSelection",m_muonSelection="", "optional check for char/bool flag on objects in collection");

   declareProperty("EleCollection",m_eleCollection="", "Name of the electron collection");
   declareProperty("EleSelection",m_eleSelection="", "optional check for char/bool flag on objects in collection");
   
   declareProperty("TauCollection",m_tauCollection="", "Name of the tau collection");
   declareProperty("TauSelection",m_tauSelection="", "optional check for char/bool flag on objects in collection");

   declareProperty("JetCollection",m_jetCollection="CamJetProvider/CalibratedJets","Name of the jet collection");
   

   declareProperty("InputMETContainer",m_inputMETContainer="MET_Core_","Input MET Container ... leave default if unsure");
   declareProperty("InputMETMap",m_inputMETMap="METAssoc_","Input MET Map ... leave default if unsure");

}

// Destructor
CamMETProvider::~CamMETProvider()
{}

StatusCode CamMETProvider::initialize() { 

  m_metMaker.setTypeAndName("met::METMaker/metMaker");
  
  if(m_muonCollection.empty()) {
    //disable MuonJetEMScale feature of MET ..
    CHECK( AAH::setProperty( m_metMaker, "DoSetMuonJetEMScale", false ) );
  }
  
  //determine the jet collection type from the name ...
  std::string author = m_jetCollection;
  while(author.find("CamJetProvider")==0||author.find("CamJetSelector")==0) {
    //determine author from tool properties ... keep iterating until hit raw collection
    ToolHandle<ICamTool> tool(author);
    author = dynamic_cast<AlgTool*>(&*tool)->getProperty("InputCollection").toString();
  } 
  
  author = author.erase(author.find("Jets"),4);
  
  ATH_MSG_DEBUG("Detected jet type: " << author);
  
  m_inputMETContainer += author;
  m_inputMETMap += author;
  
  if(author.find("PFlow")!=std::string::npos) {
    CHECK( AAH::setProperty( m_metMaker, "DoPFlow", true ) );
  }
  
  CHECK( m_metMaker.retrieve() );

  return StatusCode::SUCCESS;

}
StatusCode CamMETProvider::finalize() { 

  

   return StatusCode::SUCCESS;

}

StatusCode CamMETProvider::execute() {
  m_eventLoaded = false;
  
  return StatusCode::SUCCESS;
}

StatusCode CamMETProvider::addHardObject(CamEvent* evt, const std::string& collection, const std::string& selection, const std::string& metPart, xAOD::Type::ObjectType type, xAOD::MissingETContainer* metCont) {
  if(collection.empty()) return StatusCode::SUCCESS;
  
  
  CamDeque* ps = evt->getDeque(collection,  currentSystematicSet()); //obtain collection under the current systematic variation 
  ConstDataVector<xAOD::IParticleContainer> ips(SG::VIEW_ELEMENTS);
      
  for(auto p : *ps) {
    if(!selection.empty() && !p->is(selection)) continue; //applies the optional selection
    ips.push_back( p->ae<xAOD::IParticle>() );
  }
  
  CHECK( m_metMaker->rebuildMET(metPart,type,metCont,ips.asDataVector(),m_metMap) );
  
  return StatusCode::SUCCESS;

}


StatusCode CamMETProvider::apply(CamEvent* evt, std::string& label, const std::string& option) {

  if(label.empty()) {
    label = name().substr(8,100);
  }
  
  if(evtStore()->contains<xAOD::MissingETContainer>(label)) {
  
    const xAOD::MissingETContainer* met = 0;
    CHECK(evtStore()->retrieve(met,label));
    //create object and record ...
    CamObject* metObj = new CamObject(*(*met)["Final"]);
    metObj->SetPx(metObj->get("mpx"));metObj->SetPy(metObj->get("mpy"));
    evt->addObject(metObj,label+"Final");
  
    return StatusCode::SUCCESS;
  }

  if(!m_eventLoaded) {
    CHECK( evtStore()->retrieve( m_metCont , m_inputMETContainer ) );
    CHECK( evtStore()->retrieve( m_metMap , m_inputMETMap ) );
    m_eventLoaded=true;
  }

  m_metMap->resetObjSelectionFlags();

  xAOD::MissingETContainer* met = new xAOD::MissingETContainer;
  xAOD::MissingETAuxContainer* metaux = new xAOD::MissingETAuxContainer;
  met->setStore( metaux );
  
  CHECK( evtStore()->record(met,label) );
  CHECK( evtStore()->record(metaux,label+"Aux."));
  

  CHECK( addHardObject(evt,m_eleCollection,m_eleSelection,"RefEle",xAOD::Type::Electron,met) );
  CHECK( addHardObject(evt,m_muonCollection,m_muonSelection,"RefMuon",xAOD::Type::Muon,met) );
  CHECK( addHardObject(evt,m_tauCollection,m_tauSelection,"RefTau",xAOD::Type::Tau,met) );
  
  //now rebuild the jet and the soft term
  
  const xAOD::JetContainer* jets = dynamic_cast<const xAOD::JetContainer*>(evt->getDeque(m_jetCollection, currentSystematicSet())->container());
  
  CHECK( m_metMaker->rebuildJetMET("RefJet","PVSoftTrk",met, jets, m_metCont, m_metMap, true /*FIXME choose if do jvt cut? */) );
  
  CHECK( m_metMaker->buildMETSum("Final",met,(*met)["PVSoftTrk"]->source()) );
  
  

  //create object and record ...
  CamObject* metObj = new CamObject(*(*met)["Final"]);
  metObj->SetPx(metObj->get("mpx"));metObj->SetPy(metObj->get("mpy"));
  evt->addObject(metObj,label+"Final");

  label = label+"Final";
  
  return StatusCode::SUCCESS;
    
}
