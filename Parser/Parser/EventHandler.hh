/** @file EventHandler.hh
    @brief Defines the EventHandler module controller
    @author bloer
    @ingroup modules
*/

#ifndef EVENTHANDLER_h
#define EVENTHANDLER_h

#include "ParameterList.hh"
#include "Event.hh"
#include "RunDB.hh"
#include <string>
#include <vector>
#ifndef SINGLETHREAD
#include "boost/thread.hpp"
#include "boost/thread/barrier.hpp"
#include "boost/thread/shared_mutex.hpp"
#endif

class BaseModule;

/** @class EventHandler
    @brief Master class which controls processing events by enabled modules
    @ingroup modules
*/
class EventHandler : public ParameterList{
private:
  ///EventHandler is singleton, so no public constructor
  EventHandler();
  ///EventHandler is singleton, so no public constructor
  EventHandler(const EventHandler& right);
  /// Copy constructor private
  EventHandler& operator=(const EventHandler& right);

public:
  /// Get the global singleton pointer
  static EventHandler* GetInstance();
  /// Destructor
  ~EventHandler();
  
  /// This should be the only way modules are instantiated
  template<class Module> 
  Module* AddModule(const std::string& name=Module::GetDefaultName());
  /// Get an already registered module; return null if not found
  template<class Module>
  Module* GetModule(const std::string& name=Module::GetDefaultName());
  
  /// Add all of the processing modules, but not writers or viewers
  int AddCommonModules(); 
  
  //public interface functions
  //return 0 if no errors
  /// Initialize all registered modules
  int Initialize();
  /// Create an Event and process it with all enabled modules
  int Process(RawEventPtr raw);
  /// Force the caller to wait until all modules have finished processing
  void WaitForModules();
  /// Finalize all registered and enabled modules
  int Finalize();
  
  /// Get a pointer to the current event being processed
  EventPtr GetCurrentEvent(){ return _current_event; }
  /// Get the list of all defined modules const-ly
  const std::vector<BaseModule*>* GetListOfModules() const{ return &_modules; }
  /// Get the list of all defined modules
  std::vector<BaseModule*>* GetListOfModules(){ return &_modules;}
  /// Set the ID number of this run
  void SetRunID(int id){ run_id = id; }
  /// Set the ID number of this run based on a filename
  int SetRunIDFromFilename(const std::string& filename);
  /// Get the ID number of this run
  int GetRunID(){ return run_id; }
  /// Get the database info about the run
  RunDB::runinfo* GetRunInfo(){ return &_dbinfo; }
  
  /// Set whether to load calibration info from database at initialize
  void AllowDatabaseAccess(bool setval){ _access_database=setval; }
  /// Check whether we are supposed to fail on bad calibration
  bool GetFailOnBadCal() const { return _fail_on_bad_cal;}
private:
  std::vector<BaseModule*> _modules;
  EventPtr _current_event;
  bool _is_initialized;  ///< are the modules initialized?
  int run_id;
  RunDB::runinfo _dbinfo; ///< info about the run from the database
  bool _access_database; ///< do we query the database for missing info?
  bool _fail_on_bad_cal; ///< Fail to initialize if no calibration data found
  //functions needed by modules in case threading is enabled
#ifndef SINGLETHREAD
private:
  std::vector<boost::shared_ptr<boost::thread> > _threads;
#endif

};

#include "BaseModule.hh"

template<class Module> inline 
Module* EventHandler::AddModule(const std::string& name)
{
  for(std::vector<BaseModule*>::iterator it = _modules.begin(); 
      it != _modules.end(); it++){
    if( (*it)->GetName() == name && dynamic_cast<Module*>(*it) )
      return dynamic_cast<Module*>( *it );
  }
  //if we get here, this module doesn't exist yet, so add it
  BaseModule* newmod = new Module;
  newmod->SetName(name);
  _modules.push_back(newmod);
  RegisterParameter(name,*newmod);
  return dynamic_cast<Module*>( newmod );
}

template<class Module> inline 
Module* EventHandler::GetModule(const std::string& name)
{
  for(std::vector<BaseModule*>::iterator it = _modules.begin(); 
      it != _modules.end(); it++){
    if( (*it)->GetName() == name && dynamic_cast<Module*>(*it) )
      return dynamic_cast<Module*>( *it );
  }
  //if we get here, this module doesn't exist yet
  return 0;
}


#endif
  
