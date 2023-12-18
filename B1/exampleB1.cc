#include "DetectorConstruction.hh"

#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"

#include "G4SteppingVerbose.hh"

#include "G4UImanager.hh"

#include "Shielding.hh"

#include "G4VisExecutive.hh"

#include "G4UIExecutive.hh"

#include "Randomize.hh"

using namespace B1;

int main(int argc, char ** argv) {
  G4UIExecutive * ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }


  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager

  auto * runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  runManager -> SetUserInitialization(new DetectorConstruction());

  // Physics list
  G4VModularPhysicsList * physicsList = new Shielding;
  physicsList -> SetVerboseLevel(1);
  runManager -> SetUserInitialization(physicsList);

  // User action initialization
  runManager -> SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  //
  G4VisManager * visManager = new G4VisExecutive;
  visManager -> Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager * UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  if (!ui) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager -> ApplyCommand(command + fileName);
  } else {
    // interactive mode
    UImanager -> ApplyCommand("/control/execute init_vis.mac");
    ui -> SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted

  delete visManager;
  delete runManager;
}

