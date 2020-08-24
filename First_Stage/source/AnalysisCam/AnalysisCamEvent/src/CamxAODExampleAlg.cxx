// AnalysisCamEvent includes
#include "CamxAODExampleAlg.h"

CamxAODExampleAlg::CamxAODExampleAlg( const std::string& name, ISvcLocator* pSvcLocator ) : CamAlgorithm( name, pSvcLocator ){

  //declareProperty( "Property", m_nProperty ); //example property declaration

}


CamxAODExampleAlg::~CamxAODExampleAlg() {}


StatusCode CamxAODExampleAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");

  return StatusCode::SUCCESS;
}

StatusCode CamxAODExampleAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");

  return StatusCode::SUCCESS;
}

StatusCode CamxAODExampleAlg::beginInputFile() {  
  ATH_MSG_INFO ("BeginInputFile  " << name() << "...");
  ATH_MSG_INFO ("Current file is " << currentFile()->GetName());
  return StatusCode::SUCCESS;
}


void CamxAODExampleAlg::myMemberFunction(CamObject&) {
   std::cout << "in Member function of " << name() << std::endl;
}

void myFunc(const xAOD::EventInfo& ev) {
   std::cout << ev.runNumber() << std::endl;
}


StatusCode CamxAODExampleAlg::execute() {  
   ATH_MSG_DEBUG ("Executing " << name() << "...");

   CamEvent evt;



   ///BASIC SET AND GET BEHAVIOUR  
   ///---------------------------------
   evt.set("myProperty") = 5;                                              //set a property 
   std::cout << "type is: " << evt.type("myProperty") << std::endl;        //accessing a 'type' of a property. returns "" if unavailable 
   evt.get("myProperty");                                                  //gets copy of property as a double (double is the default!)
                                                                           //the lazy heirachy (only works for 'get copy') is: bool -> unsigned int -> int -> float -> double 
   evt.get<int>("myProperty");                                             //gets copy of property as int 
   int& myInt = evt.get<int&>("myProperty");                               //gets reference to the property 
   myInt = 4;                                                              //can change the property in-situ 
   //evt.get<float&>("myProperty"); //GIVES STACKTRACE

   evt.set("myMemberFunction") = CamObject::Function(&CamxAODExampleAlg::myMemberFunction,this); //attaches a member function
   evt.call("myMemberFunction");                                           // call the member function: have to use 'call' because this example member function is a void type


   CamObject::Documentation=true;                                          //enable documenting ... don't leave this on, it slows things down 
   evt.set("documentatedProp","set by CamxAODExampleAlg::execute") = 4;    //example of doing a 'documented' property.  
   evt.print();                                                            //look for documentatedProp with it's documentation line 
   CamObject::Documentation=false;    

   evt.set("myLambda") = std::function<int(CamObject&)>([](CamObject&){    //this syntax is a little nasty but shows how to attach a lambda function!
      std::cout << "in a lambda!" << std::endl; return 1; });
   evt.get<int>("myLambda");
   evt.set("newLambda") = CamObject::Function([](CamObject&){ 
            std::cout << "in next lambda!" << std::endl;});
   evt.call("newLambda");

   ///xAOD object decoration access 
   ///---------------------------------
   evt.print();                                                            //evt knows all of EventInfo's properties. print them to see!
   ATH_MSG_INFO(" mcChannelNumber = " << evt.get<uint>("mcChannelNumber")  //and here's a couple of examples of reading them!
          << "beamPosY=" << evt.get<float>("beamPosY"));

   myFunc(evt);                                                            //you can even pass the CamEvent to any function that takes a
                                                                           //const xAOD::EventInfo&, const xAOD::EventInfo*

   evt.auxdecor<int>("myDecoration") = 4;                                  //can use auxdecor method to decorate the underlying auxelement
   ATH_MSG_INFO(" myDecoration = " << evt.auxdata<int>("myDecoration")     //two ways to access aux information (info stored in the auxstore)
                           << " or " << evt.get<int>("myDecoration"));

                                                                           //Now we'lll create a CamVector from a particle collection:
   CamVector* els = evt.getVector("ElectronCollection");                   //You can create a CamVector from any IParticleContainer type


   els->sort_Pt(); els->all_print("tlv");                                  //sort by Pt property of TLV
   els->sort<float>("eta"); els->all_print("tlv");                         //sort by an arbitrary property

   for(auto& el : *els) {
      ATH_MSG_INFO("truthType is: " << el.get<int>("truthType"));          //example of reading a property loaded from the underlying IParticle
      el.set("charge") = el.get<float>("charge");
   }

   els->all_call( CamObject::Function([](CamObject& c) {                    //example of using a lambda to do the same as the loop above 
      std::cout << "lambda truthType is: " << c.get<int>("truthType")       //Note: the CamObject::Function wrapping is unnecessary in this case (only necessary for attaching lambdafunction properties or using member methods)
            << std::endl; }) );


   CamObject* met = evt.getMETObject("MET_RefFinal","Final");               //helper function in CamEvent to get hold of a met term 
                                                                            //the first string is the name of the MissingETContainer 
                                                                            //the second string is the name of the term (see Run2xAODMissingET twiki)
   met->print();

   ///Writing out to ntuple (example of using a service)
   ///---------------------------------
   els->applyService("CamWriter/myWriter","el_");
   evt.applyService("CamWriter/myWriter");                                 //writes out via the myWriter instance of CamWriter - see jobo for config

   ///Using a Tool 
   ///---------------------------------
   evt.applyTool("CamMetaData/CamMetaData"); //attaches functions to evt called 'meta_string", 'meta_int' and 'meta_double' 
                                                         //see content of metadata.tsv for loaded metadata
   ATH_MSG_INFO("SampleName = " << evt.get<std::string>("meta_string","SampleName") << " Nevt=" << evt.get<int>("meta_int","Nevt") );



   ///Composite objects
   ///---------------------------------
   CamObject obj1; obj1.set("myProperty") = 1;
   CamObject obj2; obj2.set("myProperty") = 4;

   CamObject combinedObj = obj1 + obj2;

   auto propVec = combinedObj.get<std::vector<int>>("myProperty");         //returns a vector of the myProperty values... aka {1,4}

   combinedObj.addChildAlias(0,"obj1"); combinedObj.addChildAlias(1,"obj2");  //you can 'name' the children ....
   int obj1_prop = combinedObj.get<int>("obj1_myProperty");                   //... and then retrieve their properties like this!
   ATH_MSG_INFO("expected = 1, actual = " << obj1_prop);

   return StatusCode::SUCCESS;
}

