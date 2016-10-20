#include <QCoreApplication>

#include "Reader.hh"
#include "ConfigHandler.hh"
#include "CommandSwitchFunctions.hh"
#include "V172X_Params.hh"
#include "ProcessedPlotter.hh"
#include "SumChannels.hh"
#include "V172X_Event.hh"
#include "EventHandler.hh"
#include "Message.hh"
#include "RootGraphix.hh"

#include "TFile.h"
#include "TCanvas.h"

#include <fstream>
#include <string>
#include <exception>
#include <stdlib.h>




/** @file daqview.cc
    @brief Main file for daqview, displays a single event from raw data
    @author bloer
*/



using namespace std;
/// Allows user to save a displayed canvas to a ROOT file
void SaveCanvas(const TCanvas* c, const char* name)
{
  TFile fout("daqview.root","UPDATE");
  if(!fout.IsOpen() || fout.IsZombie()){
    Message(ERROR)<<"Unable to open file daqview.root to save canvas!\n";
  }
  if(c)
    c->Write(name);
}



int main(int argc, char** argv)
{
  ConfigHandler* config = ConfigHandler::GetInstance();
  config->SetProgramUsageString("daqview [options] <filename> [<event>]");
  EventHandler* modules = EventHandler::GetInstance();
  modules->AddCommonModules();
  ProcessedPlotter* plotter = modules->AddModule<ProcessedPlotter>();
  RootGraphix* graphix = modules->AddModule<RootGraphix>();

  string list_filename="";
  config->AddCommandSwitch('l',"event-list",
               "Read the events to display from <file>",
               CommandSwitch::DefaultRead<string>(list_filename),
               "file");

  try{
    config->ProcessCommandLine(argc,argv);
  }
  catch(std::exception& e){
    Message(EXCEPTION)<<"While processing command line: "<<e.what()<<std::endl;
    config->PrintSwitches(true);
  }
  if(argc != 3 && argc != 2){
    Message(ERROR)<<"Incorrect number of arguments: "<<argc<<std::endl;
    config->PrintSwitches(true);
  }
  ifstream eventlist;
  bool use_elist = false;
  if(list_filename != ""){
    eventlist.open(list_filename.c_str());
    if(!eventlist.is_open()){
      Message(ERROR)<<"Unable to open event-list file "<<list_filename<<endl;
      return -1;
    }
    use_elist = true;
  }


  //plotter refresh must be set to 0 to make sure it updates
  graphix->refresh_time = 0;
  graphix->process_every_event = true;
  std::string fname(argv[1]);
  Reader reader(fname.c_str());
  if(!reader.IsOk())
    return 1;
  int next_evt = 0;
  if (argc == 3 ) next_evt = atoi(argv[2]);
  else if(use_elist) eventlist>>next_evt;

  if(modules->Initialize()){
    Message(ERROR)<<"Unable to initialize all modules.\n";
    return 1;
  }
  while(reader.IsOk() && !reader.eof()){
    RawEventPtr raw = reader.GetEventWithID(next_evt);
    if(!raw){
      Message(ERROR)<<"Problem encountered reading event "<<next_evt<<std::endl;
      return 1;
    }
    else{
      Message(DEBUG)<<"Processing event...\n";
      modules->Process(raw);
      modules->WaitForModules();
      if( plotter->GetLastProcessReturn()){
    //the plotter failed some cut; try the next one
    next_evt++;
    continue;
      }
      Message(DEBUG)<<"Done!\n";
      Message(INFO)<<"Current event has file index "<<reader.GetCurrentIndex()
           <<" and ID "<<raw->GetID()<<"\n";

    }
    int this_evt = next_evt;
    if(use_elist){
      if( !(eventlist>>next_evt)){
    Message(INFO)<<"Reached end event-list file.\n";
    next_evt = this_evt+1;
      }
    }
    else
      next_evt = this_evt+1;

    Message(INFO)<<"Enter the next event to view; <enter> for "<<next_evt
         <<", s to save this canvas, q or -1 to quit\n";
    std::string line;
    getline(std::cin, line);
    if( line == "" )
      {}
    else if(line == "q" || line == "Q" || line == "-1"){
      Message(DEBUG)<<"Quitting\n";
      break;
    }
    else if(line == "s" || line == "S"){
      Message(INFO)<<"Saving canvas to file daqview.root.\n";
      char name[30];
      sprintf(name, "Event%d",next_evt);
      SaveCanvas(plotter->GetCanvas(),name);
    }
    else{
      next_evt = atoi(line.c_str());
    }
  }
  modules->Finalize();
  return 0;
}











//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    return a.exec();
//}
