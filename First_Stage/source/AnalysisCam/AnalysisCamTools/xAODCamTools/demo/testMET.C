{
  POOL::TEvent evt(POOL::TEvent::kClassAccess); 
  evt.readFrom("$ASG_TEST_FILE_MC"); 

  evt.getEntry(0);

  CamEvent e;
  CamObject* myMet = e.getObject("CamMETProvider/myMet");
  
  
  std::cout << "Calibrated MET (x,y) = (" << myMet->Px() << "," << myMet->Py() << ")" << std::endl;
  
  

}
