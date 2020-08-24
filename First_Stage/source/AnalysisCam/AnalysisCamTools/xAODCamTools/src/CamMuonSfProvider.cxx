#include "CamMuonSfProvider.h"


#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"

CamMuonSfProvider::CamMuonSfProvider(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), m_muonSfTools(this)
{

    declareProperty("MuonSfSelections", m_muonSfSelections = {"reco", "iso", "TTVA"}, "Vector of muon scale factors to consider");

    declareProperty("MuonSfRecoWp", m_muonSfRecoWp = "Loose", "Muon scale factor Reco working point");
    
    declareProperty("MuonSfIsoWp", m_muonSfIsoWp = "FixedCutLooseIso", "Muon scale factor Iso working point");

    declareProperty("MuonSfTrigMuonQual", m_muonSfTrigMuonQual = "Loose", "Muon scale factor Trigger muon quality working point");
    declareProperty("MuonSfTrigYear", m_muonSfTrigYear = "", "Muon scale factor Trigger dataset year");
    declareProperty("MuonSfTrigMc", m_muonSfTrigMc = "", "Muon scale factor trigger MC campaign");

}

// Destructor
CamMuonSfProvider::~CamMuonSfProvider()
{}

StatusCode CamMuonSfProvider::initialize() { 
    //ATH_MSG_INFO("Init");
  
    if (m_muonSfSelections.empty()) {
        ATH_MSG_FATAL("Muon scale factors is empty");
        return StatusCode::FAILURE;
    }
    
    
    // need to set output levels
    for (auto& sfType : m_muonSfSelections) {
        ToolHandle<CP::IMuonEfficiencyScaleFactors> tool("", this);
        if (sfType == "reco") {
            tool.setTypeAndName("CP::MuonEfficiencyScaleFactors/" + sfType); 
            CHECK( AAH::setProperty( tool, "WorkingPoint", m_muonSfRecoWp) );
            m_muonSfTools.push_back(tool.typeAndName());
        }
        if (sfType == "TTVA") {
            tool.setTypeAndName("CP::MuonEfficiencyScaleFactors/" + sfType); 
            CHECK( AAH::setProperty( tool, "WorkingPoint", "TTVA") );
            m_muonSfTools.push_back(tool.typeAndName());
        }
        if (sfType == "iso") {
            tool.setTypeAndName("CP::MuonEfficiencyScaleFactors/" + sfType); 
            CHECK( AAH::setProperty( tool, "WorkingPoint", m_muonSfIsoWp) );
            m_muonSfTools.push_back(tool.typeAndName());
        }
        if (sfType == "trigger") {
            tool.setTypeAndName("CP::MuonEfficiencyScaleFactors/" + sfType); 
            CHECK( AAH::setProperty( tool, "MuonQuality", m_muonSfTrigMuonQual) );
            CHECK( AAH::setProperty( tool, "Year", m_muonSfTrigYear) );
            CHECK( AAH::setProperty( tool, "MC", m_muonSfTrigMc) );
            m_muonSfTools.push_back(tool.typeAndName());
        }
        
        if(!tool.empty() && msgLevel()>=MSG::INFO) CHECK( AAH::setProperty( tool, "OutputLevel", MSG::WARNING) ); //because tools are too noisy!
        
    }

  //register systematics of tools
    for (auto& sfTool : m_muonSfTools) {
        registerSystematics( &*sfTool ); 
    }

  return StatusCode::SUCCESS;

}

StatusCode CamMuonSfProvider::finalize() { 

   return StatusCode::SUCCESS;

}

StatusCode CamMuonSfProvider::apply(CamDeque* muDeque, std::string& label, const std::string& option) {
    if(label.empty()) label="scaleFactor";
    //usage: mus_select->applyTool("CamMuonSfProvider");
    
    
    
     auto muSysts = this->getSystematics(); // the recommended systematicset containing variations
    bool onlyNominal(option.find("n")!=std::string::npos);
    bool append(option.find("a")!=std::string::npos);
    CP::SystematicSet tmpSet;
    
    // if css is empty, i.e. no kin variation, we do all SF (i.e. nominal + variations)
    // if css > 0, assign css to tmpSet. applySystematicVariation(tmpSet) will find no affected systematics. only do nominal SF.
    if(currentSystematicSet().size() > 0) {
      tmpSet = currentSystematicSet();
      onlyNominal = true;
    }

    for(auto mu = muDeque->begin(); mu != muDeque->end(); ++mu) {

        //std::cout << "elSfProvider Pt: " << (*el)->Pt() << std::endl;;

        const xAOD::Muon* muAuxElement = (*mu)->ae<xAOD::Muon>();
        
        // nominal SF
        float sfN( ( append && (*mu)->contains<float>(label,CamObject::MAP)) ? (*mu)->get<float>(label) : 1. ); //will add to existing scale factors
        float sfNCopy(sfN);

            //std::cout << "passed cut: " << (*el)->get<int>("quality") << std::endl;
            for (auto& sfTool : m_muonSfTools) {
                float res(1.);
                sfTool->applySystematicVariation(tmpSet).ignore();
                sfTool->getEfficiencyScaleFactor(*muAuxElement, res).ignore();
                sfN *= res;
            }


        (*mu)->set(label) = sfN;


        if(onlyNominal) continue; //go to nex

        // do variations on SF as well ( only if css is empty (i.e. no kin var) )
        for (auto sv : muSysts) {
            tmpSet.insert(sv);
            float sf( ( append && (*mu)->contains<float>(label+ sv.name(),CamObject::MAP)) ? (*mu)->get<float>(label+ sv.name()) : sfNCopy );

                //std::cout << "passed cut: " << (*el)->get<int>("quality") << std::endl;
                for (auto& sfTool : m_muonSfTools) {
                    float res(1.);
                    sfTool->applySystematicVariation(tmpSet).ignore(); // warning correction codes concerning unaffected systematics will be suppressed
                    sfTool->getEfficiencyScaleFactor(*muAuxElement, res).ignore();
                    sf *= res;
                }
            //else { std::cout << "did not pass cut: " << (*el)->get<int>("quality") << std::endl; };


            (*mu)->set(label + sv.name()) = sf;
            tmpSet = CP::SystematicSet(); //tmpSet.clear(); ...CLEARING DOESNT CLEAR THE SET PROPERTIES!!! ARRRRRRGGHHH
        }
    }
    
    
    
    return StatusCode::SUCCESS;
    
}
