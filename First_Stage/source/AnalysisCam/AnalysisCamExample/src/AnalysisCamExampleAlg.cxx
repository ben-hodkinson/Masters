// AnalysisCamExample includes
#include "AnalysisCamExampleAlg.h"


StatusCode AnalysisCamExampleAlg::firstExecute() {
  
  //read config file with object selections ...
  //The example in GeneralCamTools is for electrons (MyElectrons) and muons (MyMuons)
  CamEvent::readOptions("GeneralCamTools/configs/example.opts");
  
  //configure a writer for the nominal selection 
  //first arg is writer name, second arg is tree name (including stream)
  //optional third arg is specifying branches to create (rather than infer from objects)
  CamEvent::configureWriter("nominal","/MYSTREAM/nominal" /*, {"el_tlv_pt/vector<double>","el_isIsolFixedCutLoose/vector<bool>"}*/ ); 
 
  
  //get set of systematics affecting MyElectrons 
  m_elSysts = CamEvent::getSystematics("CamEleSelector/MyElectrons");
  
  //get set of systematics affecting MyMuons 
  m_muSysts = CamEvent::getSystematics("CamMuonSelector/MyMuons");
  
  //create writers for each systematic ..
  for(auto& syst : m_elSysts) {
    CamEvent::configureWriter(syst.name(),"/MYSTREAM/" + syst.name()); 
    ATH_MSG_INFO("Configured electron syst : " << syst.name() );
  }
  for(auto& syst : m_muSysts) {
    CamEvent::configureWriter(syst.name(),"/MYSTREAM/" + syst.name());
    ATH_MSG_INFO("Configured muon syst : " << syst.name() );
  }
  
  
  
  return StatusCode::SUCCESS;
}


StatusCode AnalysisCamExampleAlg::execute() {  

  CamEvent e;
  
  //do the nominal selection  
  CamDeque* els = e.getDeque("CamEleSelector/MyElectrons");
  CamDeque* mus = e.getDeque("CamMuonSelector/MyMuons");
  
  //we can add arbitrary properties
  //here we add an integer property to each electron
  //will get output to el_myProperty branch (of type: vector<int>)
  int i=0;
  for(auto& el : *els) {
    el->set("myProperty") = i;
    i++;
  }
  
  
  //write out the nominal electrons and muons
  els->applyService("CamWriter/nominal","el_");
  mus->applyService("CamWriter/nominal","mu_");
  
  
  //do the systematic variations
  for(auto& syst : m_elSysts) {
    CamDeque* els = e.getDeque("CamEleSelector/MyElectrons",syst);
    CamDeque* mus = e.getDeque("CamMuonSelector/MyMuons",syst); //this will be the nominal muons in each case
    els->applyService("CamWriter/"+syst.name(),"el_");
    mus->applyService("CamWriter/"+syst.name(),"mu_");
  }
  
  for(auto& syst : m_muSysts) {
    CamDeque* els = e.getDeque("CamEleSelector/MyElectrons",syst);  //this will be the nominal electrons in each case
    CamDeque* mus = e.getDeque("CamMuonSelector/MyMuons",syst);
    els->applyService("CamWriter/"+syst.name(),"el_");
    mus->applyService("CamWriter/"+syst.name(),"mu_");
  }
  
  
  
  
  
  return StatusCode::SUCCESS;
}
