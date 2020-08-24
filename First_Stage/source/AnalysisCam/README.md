Introduction to CamEvent
========================

CamEvent is an EDM suite that helps you do analysis, whether you working with xAOD or flat ntuples.
The core objects in this suite are:

CamObject - the basic EDM object, it has arbitrary properties you can ```set``` and ```get```<br>
CamVector - a vector of CamObjects<br>
CamDeque - a vector (actually a deque, but the difference isn't important) of pointers to CamObjects<br>
CamEvent - a special CamObject that helps you create the above objects<br>

These EDM objects know how to interact with ```CamTools``` and ```CamServices``` (the distinction is not too important).

The most important example is the ```CamWriter``` which is a ```CamService``` that can be used to write out the EDM objects as flat ROOT TTrees (ntuples).

Example in athena - 21.2.16 onwards
============================

An example configuration for reading calibrated and selected electrons and muons can be found at [GeneralCamTools/share/configs/example.opts](AnalysisCamTools/GeneralCamTools/share/configs/example.opts)

In the ```firstExecute``` method, you should load the configuration file like this (note the lack of 'share'): 
```
CamEvent::readOptions("GeneralCamTools/configs/example.opts");
```

If you want to write out ntuples, you can configure CamWriters at this stage too:

```
CamEvent::configureWriter("MyWriter","/MYSTREAM/MyTree"); //writer name = MyWriter, output tree = MyTree, output stream = MYSTREAM
```

Then in the ```execute``` method you can do something like this:

```
  //Obtain the selections like this:
  CamEvent e;
  CamDeque* els = e.getDeque("CamEleSelector/MyElectrons");
  CamDeque* mus = e.getDeque("CamMuonSelector/MyMuons");
  
  els->applyService("CamWriter/MyWriter","el_"); //writes out electrons with el_ prefix on branch names
  mus->applyService("CamWriter/MyWriter","mu_"); //writes out muons with mu_prefix on branch names
  
```


For dealing with systematics, in the ```firstExecute``` method you can obtain a set of systematics that affect a given object/collection:

```
  //list of systematics affecting electrons can be obtained with:
  CP::SystematicSet elSysts = CamEvent::getSystematics("CamEleSelector/MyElectrons");
```  

Then you can loop over these systematics in the ```execute``` method and obtain systematic variations like this:


```
  for(auto& syst : elSysts) {
    CamDeque* els = e.getDeque("CamEleSelector/MyElectrons",syst);
    CamDeque* mus = e.getDeque("CamMuonSelector/MyMuons",syst); //this will be the nominal muons in each case
    ...
    
  }
```

Note that CamEvent is smart enough that when it detects ```MyMuons``` is not affected by the systematic you will get back the nominal collection.


Exploring xAOD interactively
============================

You can use CamEvent to explore an xAOD. Here's an example from the ROOT prompt:

```
POOL::TEvent evt(POOL::TEvent::kPOOLAccess); //slowest read mode but most robust
evt.readFrom("$ASG_TEST_FILE_MC"); //read the test file in the release

evt.getEntry(0);
CamEvent ce;

ce.print(); //prints the event properties (properties of the xAOD::EventInfo)
ce.print("vectors"); //prints the list of vectors you can retrieve
CamVector* jets = ce.getVector("AntiKt4EMTopoJets"); //retrieve jets
jets->all_print("tlv_pt"); //print the pTs of the jets

evt.getEntry(1); //move to next event
ce.reinit(); //must reinitialize CamEvent, will cause the vectors collection to be deleted

jets = ce.getVector("AntiKt4EMTopoJets"); //retrieve jets in this event
jets->all_print("tlv_pt"); //print the pTs of the jets


```

Using CamWriters to record CamObjects
=====================================

The most basic way to work with a CamWriter is to simply apply a writer to the object you want to write out


```
ce.set("myProperty") = 5;
ce.applyService("CamWriter/myWriter","","fillAfterApply"); 
```

The above will create a file called ```CAMTUP.root``` containing a tree called ```CamTree```. It would contain an integer branch called ```myProperty```. 