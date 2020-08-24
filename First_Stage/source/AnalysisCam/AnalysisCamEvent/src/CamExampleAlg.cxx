// AnalysisCamEvent includes
#include "CamExampleAlg.h"



CamExampleAlg::CamExampleAlg( const std::string& name, ISvcLocator* pSvcLocator ) : CamAlgorithm( name, pSvcLocator ){

  //declareProperty( "Property", m_nProperty ); //example property declaration

}


CamExampleAlg::~CamExampleAlg() {}


StatusCode CamExampleAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");

  return StatusCode::SUCCESS;
}

StatusCode CamExampleAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");

  return StatusCode::SUCCESS;
}

StatusCode CamExampleAlg::beginInputFile() {  
  ATH_MSG_INFO ("BeginInputFile  " << name() << "...");

  return StatusCode::SUCCESS;
}

StatusCode CamExampleAlg::firstExecute() {
  ATH_MSG_INFO ("firstExecute  " << name() << "...");

  return StatusCode::SUCCESS;
}

float myFunction(CamObject &c) {
   std::cout << "in myFunction. property is " << c.get("myProperty") << std::endl;
   return c.get("myProperty")*2;
}

float myArgFunction(CamObject& c,va_list args) {
   std::string arg1 = va_arg(args,char*);
   int arg2 = va_arg(args,int);
   std::cout << "in myArgFunction. arg1=" << arg1 << " arg2=" << arg2 << std::endl;
   return c.get("myProperty");
}

void CamExampleAlg::myMemberFunction(CamObject&) {
   std::cout << "in Member function of " << name() << std::endl;
}

StatusCode CamExampleAlg::execute() {  
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

   evt.set("myFunction") = std::function<int(CamObject&)>(&myFunction);    // you can attach functions with the std::function object. Signature must be:
                                                                           //  () ,  (CamObject&) , (CamObject&, va_list)

   evt.SetPtEtaPhiM(3,4,5,6);
   evt.set("myPt") = &CamObject::Pt;
   std::cout << " myPt = " << evt.get("myPt") << std::endl;

   evt.set("myFunction") = CamObject::Function(&myFunction);               //myFunction must be signature: static [type] myFunction(CamObject& c)
                                                                           //                          OR: static [type] myFunction(CamObject& c,va_list args)
   evt.get<float>("myFunction");                                           //evaluates the function, the CamObject passes itself to the function 

   evt.set("myArgFunction") = CamObject::Function(&myArgFunction);         //attach function with arguments 
   evt.get<float>("myArgFunction","myArg",2);                              //call function with arguments WARNING: NO COMPILE TIME CHECK FOR CORRECT ARG # OR TYPE 

   evt.set("myMemberFunction") = CamObject::Function(&CamExampleAlg::myMemberFunction,this); //attaches a member function
   evt.call("myMemberFunction");                                           // call the member function: have to use 'call' because this example member function is a void type

   evt.set("myLambda") = std::function<int(CamObject&)>([](CamObject&){    //this syntax is a little nasty but shows how to attach a lambda function!
      std::cout << "in a lambda!" << std::endl; return 1; });
   evt.get<int>("myLambda");
   evt.set("newLambda") = CamObject::Function([](CamObject&){ 
            std::cout << "in next lambda!" << std::endl;});
   evt.call("newLambda");

   ///D3PD Variable access 
   ///---------------------------------
   if(evt.contains<uint>("mc_channel_number"))                             //example of checking if CamObject can access a property. MUST MATCH EXACT TYPE
      evt.get<uint>("mc_channel_number");                                  //gets a property out of the input file!

   CamVector* el = evt.getVector("el");                                    //constructs a CamVector with el_n CamObjects in it
   el->all_call( [](CamObject& c){                                         //'calling' a lambda ('call' means the lambda must return void)
      c.SetPtEtaPhiM(c.get("pt"),c.get("eta"),c.get("phi"),c.get("m"));}   // that gets applied to all CamObjects in the collection
   );
   el->all_print("tlv");                                                   //print TLorentzVector properties of all the CamObjects

   CamVector mu("mu_staco_");                                              //direct constructon of CamVector, argument is the 'prefix'
   mu.resize("n");                                                         //resizes according to mu_staco_n branch value

   std::vector<double> mu_pt = mu.all_get<double>("pt");                  //creates a vector of all the pt 
   std::vector<float> el_tlv_eta = el->all_get<float>(&CamObject::Eta);   //creates a vector of floats by calling the Eta() method with each
   std::vector<float> same_again = el->all_get<float>( [](CamObject& c){  //does same as line above, but uses a lambda .. note type *must* match
                                                return float(c.Eta());});

   mu.all_set("myString") = "hello";                                      //sets a property in ever CamObject in the vector
   mu.all_call([](CamObject& c) { 
      std::cout << c.get<std::string>("myString") <<","; });              //another example of a lambda .. going to print "myString" many times 
   std::cout << std::endl;

   mu.set("myTopProperty") = 6;                                           //can set properties of the collections, they're camobjects too!

   ///Writing to TTree
   ///--------------------------
   mu.applyService("CamWriter/myWriter","mu_");                            //will write all properties of CamObjects into vectors, 
                                                                           //and properties of collection into non-vectors. Prefixes all with "mu_"
   el->applyService("CamWriter/myWriter","el_");                           //similar for this collection
   evt.applyService("CamWriter/myWriter");                                 //finally writes out the evt CamObject properties

   return StatusCode::SUCCESS;
}

