#include "CamElSfProvider.h"


#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"

CamElSfProvider::CamElSfProvider(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), m_elSfTools(this)
{

    declareProperty("ElSfSelections", m_elSfSelections = {"reco", "ID", "iso"}, "Vector of electron scale factors considered, possible values are: reco, ID, iso, trigger, chargeFlip");

    declareProperty("ElSfTools", m_elSfTools, "Array of electron scale factor tools");

    declareProperty("ElSfCorrModel", m_elSfCorrModel = "SIMPLIFIED", "Electron scale factor correlation model");
    declareProperty("ElSfNtoys", m_elSfNtoys  = 40, "Electron scale factor number of toys");

#if ROOTCORE_RELEASE_SERIES >= 25
    declareProperty("ElSfMapFilePath", m_elSfMapFilePath = "", "Electron scale factor tool map file path .. leave blank to use default");
#else
    declareProperty("ElSfMapFilePath", m_elSfMapFilePath = "ElectronEfficiencyCorrection/2015_2016/rel20.7/Moriond_February2017_v1/map0.txt", "Electron scale factor tool map file path ... leave blank to use default");
#endif
    declareProperty("ElSfIdKey", m_elSfIdKey = "LooseBLayer", "Electron scale factor ID tool operating point");
    declareProperty("ElSfIsoKey", m_elSfIsoKey = "FixedCutLoose", "Electron scale factor Iso tool working point");
    declareProperty("ElSfTriggerKey", m_elSfTriggerKey = "", "Electron scale factor Trigger tool trigger name");
    declareProperty("ElSfChargeIdKey", m_elSfChIdKey = "Tight", "Electron charge id tool working point");
    /*
    declareProperty("ElSfCorrectionFileReco", m_eleCorrFileReco = {"ElectronEfficiencyCorrection/2015_2016/rel20.7/Moriond_February2017_v1/offline/efficiencySF.offline.RecoTrk.root"}, "Electron scale factor correction file reco");
    declareProperty("ElSfCorrectionFileID", m_eleCorrFileID = {"ElectronEfficiencyCorrection/2015_2016/rel20.7/Moriond_February2017_v1/offline/efficiencySF.offline.LooseAndBLayerLLH_d0z0_v11.root"}, "Electron scale factor correction file ID");
    declareProperty("ElSfCorrectionFileIso", m_eleCorrFileIso = {"ElectronEfficiencyCorrection/2015_2016/rel20.7/Moriond_February2017_v1/isolation/efficiencySF.Isolation.LooseAndBLayerLLH_d0z0_v11_isolFixedCutLoose.root"}, "Electron scale factor correction file isolation");
    declareProperty("ElSfCorrectionFileTrigger", m_eleCorrFileTrig = {"ElectronEfficiencyCorrection/efficiencySF.AnyElectronTrigger.LooseLH.2015.13TeV.rel20p0.25ns.v01.root"}, "Electron scale factor correction file trigger");
    */

}

// Destructor
CamElSfProvider::~CamElSfProvider()
{}

StatusCode CamElSfProvider::initialize() { 
    //ATH_MSG_INFO("Init");
    
    // configuration checks
    if (m_elSfSelections.empty()) {
        ATH_MSG_FATAL("Electron scale factors is empty");
        return StatusCode::FAILURE;
    }
    
    std::map<std::string, int> IdKeyCutMap = {{"Tight", 0}, {"Medium", 1}, {"LooseBLayer", 2}};
    m_elSfIdCut = IdKeyCutMap.at(m_elSfIdKey);
    //std::cout << "IdKey: " << m_elSfIdKey << " cut #: " << m_elSfIdCut;

    // need to set output levels
    for (auto& sfType : m_elSfSelections) {
        ToolHandle<IAsgElectronEfficiencyCorrectionTool> tool("", this);
        if (sfType == "reco") {
            tool.setTypeAndName("AsgElectronEfficiencyCorrectionTool/" + sfType); 
            CHECK( AAH::setProperty( tool, "RecoKey", "Reconstruction") );
            m_elSfTools.push_back(tool.typeAndName());
        }
        if (sfType == "ID") {
            tool.setTypeAndName("AsgElectronEfficiencyCorrectionTool/" + sfType); 
            CHECK( AAH::setProperty( tool, "IdKey", m_elSfIdKey) );
            m_elSfTools.push_back(tool.typeAndName());
        }
        if (sfType == "iso") {
            tool.setTypeAndName("AsgElectronEfficiencyCorrectionTool/" + sfType); 
            CHECK( AAH::setProperty( tool, "IsoKey", m_elSfIsoKey) );
            CHECK( AAH::setProperty( tool, "IdKey", m_elSfIdKey) );
            m_elSfTools.push_back(tool.typeAndName());
        }
        if (sfType == "trigger") {
            tool.setTypeAndName("AsgElectronEfficiencyCorrectionTool/" + sfType); 
            CHECK( AAH::setProperty( tool, "TriggerKey", m_elSfTriggerKey) );
            CHECK( AAH::setProperty( tool, "IdKey", m_elSfIdKey) );
            CHECK( AAH::setProperty( tool, "IsoKey", m_elSfIsoKey) );
            m_elSfTools.push_back(tool.typeAndName());
        }
        if(!tool.empty() && !m_elSfMapFilePath.empty()) CHECK( AAH::setProperty( tool, "MapFilePath", m_elSfMapFilePath) );
        
        if (sfType == "chargeFlip") {
            tool.setTypeAndName("CP::ElectronChargeEfficiencyCorrectionTool/" + sfType);
             std::string tmpIDWP = m_elSfIdKey;
            if (tmpIDWP!="Medium" && tmpIDWP!="Tight") {
              ATH_MSG_WARNING( "** Only MediumLLH & TightLLH ID WP are supported for ChargeID SFs at the moment. Falling back to MediumLLH, be aware! **");
              tmpIDWP = "Medium";
            }
            std::string tmpIsoWP = m_elSfIsoKey;
            if (tmpIsoWP != "Gradient" && tmpIsoWP != "FixedCutTight") {
              ATH_MSG_WARNING( "** Only Gradient & FixedCutTight Iso WP are supported for ChargeID SFs at the moment. Falling back to Gradient, be aware! **");
              tmpIsoWP = "Gradient";
            }
            if (tmpIsoWP == "Gradient") tmpIsoWP="isolGradient";  // <3 egamma
            
            std::string tmpChID("");
            if(!m_elSfChIdKey.empty()){
              ATH_MSG_WARNING( "** Only one ID+ISO+CFT configuration supported for ChargeID SFs at the moment (Medium_FixedCutTightIso_CFTMedium) . Falling back to that, be aware! **");
              tmpIDWP  = "Medium";
              tmpIsoWP = "FixedCutTightIso";
              tmpChID  = "_CFTMedium";
            }
            
            std::string chfFile("ElectronEfficiencyCorrection/2015_2016/rel20.7/Moriond_February2017_v1/charge_misID/ChargeCorrectionSF."+tmpIDWP+"_"+tmpIsoWP+tmpChID+".root");//only supported for now!
            CHECK(AAH::setProperty(tool, "CorrectionFileName", chfFile));
            
            m_elSfTools.push_back(tool.typeAndName());
            
        }
        
        if(!tool.empty() && msgLevel()>=MSG::INFO) CHECK( AAH::setProperty( tool, "OutputLevel", MSG::WARNING) ); //because tools are too noisy!
        
        if(tool.empty()) {
          ATH_MSG_ERROR("Unrecognised scale factor type: " << sfType);
          return StatusCode::FAILURE;
        }
        
    }

    CamTool::DataType dType = getDataType(); // enum DataType { DATA, FULLSIM, FASTSIM }
    int dataTypeCode = 0; //data
    if(dType==CamTool::FULLSIM) dataTypeCode=1;
    else if(dType==CamTool::FASTSIM) dataTypeCode=3; //fast sim ... why isnt this 2!??

    for (auto& sfTool : m_elSfTools) {
        CHECK( AAH::setProperty( sfTool, "ForceDataType", dataTypeCode) ); //expects value from enum DataType { Data = 0, Full = 1, FastShower = 2, Fast = 3, True = 4 }
        if(sfTool.name()=="chargeFlip") continue; //properties not defined for charge flip tool
        CHECK( AAH::setProperty( sfTool, "CorrelationModel", m_elSfCorrModel ) );
        CHECK( AAH::setProperty( sfTool, "NumberOfToys", m_elSfNtoys ) );
    }
 
  //register systematics of tools
    for (auto& sfTool : m_elSfTools) {
        registerSystematics( &*sfTool ); 
    }

  return StatusCode::SUCCESS;

}
StatusCode CamElSfProvider::finalize() { 

   return StatusCode::SUCCESS;

}

StatusCode CamElSfProvider::apply(CamDeque* elDeque, std::string& label, const std::string& option) {
    if(label.empty()) label="scaleFactor";

    //usage: els_select->applyTool("CamElSfProvider");
    
    auto elSysts = this->getSystematics(); // the recommended systematicset containing variations
    bool onlyNominal(option.find("n")!=std::string::npos); //if true, only does nominal 
    bool append(option.find("a")!=std::string::npos); //if true, will multiply existing scale factor with new ones
    CP::SystematicSet tmpSet;
    
    // if css is empty, i.e. no kin variation, we do all SF (i.e. nominal + variations)
    // if css > 0, assign css to tmpSet. applySystematicVariation(tmpSet) will find no affected systematics. only do nominal SF.
    if(currentSystematicSet().size() > 0) {
      tmpSet = currentSystematicSet();
      onlyNominal = true;
    }

    for(auto el = elDeque->begin(); el != elDeque->end(); ++el) {

        //std::cout << "elSfProvider Pt: " << (*el)->Pt() << std::endl;;

        const xAOD::Electron* elAuxElement = (*el)->ae<xAOD::Electron>();
        
        // nominal SF
        
        float sfN( ( append && (*el)->contains<float>(label,CamObject::MAP)) ? (*el)->get<float>(label) : 1. ); //will add to existing scale factors
        float sfNCopy(sfN);
        
    
        for (auto& sfTool : m_elSfTools) {
            double res(1.);
            sfTool->applySystematicVariation(tmpSet).ignore();
            if(sfTool->getEfficiencyScaleFactor(*elAuxElement, res)==CP::CorrectionCode::Ok) sfN *= res;
        }

        (*el)->set(label) = sfN;

        if(onlyNominal) continue; //go to nex

        // do variations on SF as well ( only if css is empty (i.e. no kin var) )
        for (auto sv : elSysts) {
            tmpSet.insert(sv);
            float sf( ( append && (*el)->contains<float>(label+ sv.name(),CamObject::MAP)) ? (*el)->get<float>(label+ sv.name()) : sfNCopy );
            for (auto& sfTool : m_elSfTools) {
                double res(1.);
                sfTool->applySystematicVariation(tmpSet).ignore(); // warning correction codes concerning unaffected systematics will be suppressed
                if(sfTool->getEfficiencyScaleFactor(*elAuxElement, res)==CP::CorrectionCode::Ok) sf *= res;
            }

            (*el)->set(label + sv.name()) = sf;
            tmpSet = CP::SystematicSet(); //tmpSet.clear(); ...CLEARING DOESNT CLEAR THE SET PROPERTIES!!! ARRRRRRGGHHH
        }
    }

    return StatusCode::SUCCESS;
    
}
