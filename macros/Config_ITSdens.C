// One can use the configuration macro in compiled mode by
// root [0] gSystem->Load("libgeant321");
// root [0] gSystem->SetIncludePath("-I$ROOTSYS/include -I$ALICE_ROOT/include\
//                   -I$ALICE_ROOT -I$ALICE/geant3/TGeant3");
// root [0] .x grun.C(1,"Config.C++")

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TPDGCode.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeant3TGeo.h>
#include "STEER/AliRunLoader.h"
#include "STEER/AliRun.h"
#include "STEER/AliConfig.h"
#include "PYTHIA6/AliDecayerPythia.h"
#include "EVGEN/AliGenCocktail.h"
#include "EVGEN/AliGenHIJINGpara.h"
#include "STEER/AliMagF.h"
#include "STRUCT/AliBODY.h"
#include "STRUCT/AliMAG.h"
#include "STRUCT/AliABSOv3.h"
#include "STRUCT/AliDIPOv3.h"
#include "STRUCT/AliHALLv3.h"
#include "STRUCT/AliFRAMEv2.h"
#include "STRUCT/AliSHILv3.h"
#include "STRUCT/AliPIPEv3.h"
#include "ITS/AliITSv11.h"
#include "TPC/AliTPCv2.h"
#include "TOF/AliTOFv6T0.h"
#include "HMPID/AliHMPIDv3.h"
#include "ZDC/AliZDCv4.h"
#include "TRD/AliTRDv1.h"
#include "FMD/AliFMDv1.h"
#include "MUON/AliMUONv1.h"
#include "PHOS/AliPHOSv1.h"
#include "PMD/AliPMDv1.h"
#include "T0/AliT0v1.h"
#include "EMCAL/AliEMCALv2.h"
#include "ACORDE/AliACORDEv1.h"
#include "VZERO/AliVZEROv7.h"
#endif

Float_t EtaToTheta(Float_t arg);
void    LoadPythia();


void Config()
{
    // ThetaRange is (0., 180.). It was (0.28,179.72) 7/12/00 09:00
    // Theta range given through pseudorapidity limits 22/6/2001

    // Set Random Number seed
  //gRandom->SetSeed(123456); // Set 0 to use the current time
  
  AliLog::Message(AliLog::kInfo, Form("Seed for random number generation = %d",gRandom->GetSeed()), "Config.C", "Config.C", "Config()","Config.C", __LINE__);

  // Load Pythia libraries
  LoadPythia();
  // Libraries required by geant321
#if defined(__CINT__)
    gSystem->Load("libgeant321");
#endif

    new     TGeant3TGeo("C++ Interface to Geant3");

    AliRunLoader* rl=0x0;

    AliLog::Message(AliLog::kInfo, "Creating Run Loader", "Config.C", "Config.C", "Config()"," Config.C", __LINE__);

    rl = AliRunLoader::Open("galice.root",
			    AliConfig::GetDefaultEventFolderName(),
			    "recreate");
    if (rl == 0x0)
      {
	gAlice->Fatal("Config.C","Can not instatiate the Run Loader");
	return;
      }
    rl->SetCompressionLevel(2);
    rl->SetNumberOfEventsPerFile(3);
    gAlice->SetRunLoader(rl);
    
    // gAlice->SetGeometryFromFile("geometry.root");

    // Uncomment if you want to load geometry from OCDB!   >>>>
/*    
    if(!AliCDBManager::Instance()->IsDefaultStorageSet()){
	 cout << "#####################################################" << endl;
	 cout << "#                                                   #" << endl;
	 cout << "#     WARNING: CDB DEFAULT STORAGE NOT SET !!!      #" << endl;
	 cout << "#     SETTING IT TO local://$ALICE_ROOT/OCDB !!!         #" << endl;
	 cout << "#                                                   #" << endl;
	 cout << "#####################################################" << endl;
          
         AliCDBManager::Instance()->SetDefaultStorage("local://$ALICE_ROOT/OCDB");
    }
    
    if(AliCDBManager::Instance()->GetRun() < 0){
	 cout << "#####################################################" << endl;
	 cout << "#                                                   #" << endl;
	 cout << "#     WARNING: RUN NUMBER NOT SET !!!               #" << endl;
	 cout << "#     SETTING IT TO 0 !!!                           #" << endl;
	 cout << "#                                                   #" << endl;
	 cout << "#####################################################" << endl;
          
         AliCDBManager::Instance()->SetRun(0);
    }
    gAlice->SetGeometryFromCDB();
*/
    // Uncomment if you want to load geometry from OCDB!   <<<<

    // Set the trigger configuration
    AliSimulation::Instance()->SetTriggerConfig("Pb-Pb");
    cout<<"Trigger configuration is set to  Pb-Pb"<<endl;

    //
    // Set External decayer
    TVirtualMCDecayer *decayer = new AliDecayerPythia();

    decayer->SetForceDecay(kAll);
    decayer->Init();
    gMC->SetExternalDecayer(decayer);
    //=======================================================================
    // ************* STEERING parameters FOR ALICE SIMULATION **************
    // --- Specify event type to be tracked through the ALICE setup
    // --- All positions are in cm, angles in degrees, and P and E in GeV


    gMC->SetProcess("DCAY",1);
    gMC->SetProcess("PAIR",1);
    gMC->SetProcess("COMP",1);
    gMC->SetProcess("PHOT",1);
    gMC->SetProcess("PFIS",0);
    gMC->SetProcess("DRAY",0);
    gMC->SetProcess("ANNI",1);
    gMC->SetProcess("BREM",1);
    gMC->SetProcess("MUNU",1);
    gMC->SetProcess("CKOV",1);
    gMC->SetProcess("HADR",1);
    gMC->SetProcess("LOSS",2);
    gMC->SetProcess("MULS",1);
    gMC->SetProcess("RAYL",1);

    Float_t cut = 1.e-3;        // 1MeV cut by default
    Float_t tofmax = 1.e10;

    gMC->SetCut("CUTGAM", cut);
    gMC->SetCut("CUTELE", cut);
    gMC->SetCut("CUTNEU", cut);
    gMC->SetCut("CUTHAD", cut);
    gMC->SetCut("CUTMUO", cut);
    gMC->SetCut("BCUTE",  cut); 
    gMC->SetCut("BCUTM",  cut); 
    gMC->SetCut("DCUTE",  cut); 
    gMC->SetCut("DCUTM",  cut); 
    gMC->SetCut("PPCUTM", cut);
    gMC->SetCut("TOFMAX", tofmax); 


    int     nParticles = 100;
    if (gSystem->Getenv("CONFIG_NPARTICLES"))
    {
        nParticles = atoi(gSystem->Getenv("CONFIG_NPARTICLES"));
    }

    AliGenCocktail *gener = new AliGenCocktail();
    gener->SetPhiRange(0, 360);
    // Set pseudorapidity range from -8 to 8.
    Float_t thmin = EtaToTheta(8);   // theta min. <---> eta max
    Float_t thmax = EtaToTheta(-8);  // theta max. <---> eta min 
    gener->SetThetaRange(thmin,thmax);
    gener->SetOrigin(0, 0, 0);  //vertex position
    gener->SetSigma(0, 0, 0);   //Sigma in (X,Y,Z) (cm) on IP position

    AliGenHIJINGpara *hijingparam = new AliGenHIJINGpara(nParticles);
    hijingparam->SetMomentumRange(0.2, 999);
    gener->AddGenerator(hijingparam,"HIJING PARAM",1);

//    AliGenBox *genbox = new AliGenBox(nParticles);
//    genbox->SetPart(kGamma);
//    genbox->SetPtRange(0.3, 10.00);
//    gener->AddGenerator(genbox,"GENBOX GAMMA for PHOS",1);
    gener->Init();


    // 
    // Activate this line if you want the vertex smearing to happen
    // track by track
    //
    //gener->SetVertexSmear(perTrack); 
    // Field (L3 0.4 T)
    TGeoGlobalMagField::Instance()->SetField(new AliMagF("Maps","Maps", -1., -1., AliMagF::k5kG));

    Int_t   iABSO   = 0;
    Int_t   iDIPO   = 0;
    Int_t   iFMD    = 0;
    Int_t   iFRAME  = 1;
    Int_t   iHALL   = 0;
    Int_t   iITS    = 1;
    Int_t   iMAG    = 1;
    Int_t   iMUON   = 0;
    Int_t   iPHOS   = 0;
    Int_t   iPIPE   = 1;
    Int_t   iPMD    = 0;
    Int_t   iHMPID  = 0;
    Int_t   iSHIL   = 0;
    Int_t   iT0     = 0;
    Int_t   iTOF    = 0;
    Int_t   iTPC    = 0;
    Int_t   iTRD    = 0;
    Int_t   iZDC    = 0;
    Int_t   iEMCAL  = 0;
    Int_t   iACORDE = 0;
    Int_t   iVZERO  = 0;
    rl->CdGAFile();
    //=================== Alice BODY parameters =============================
    AliBODY *BODY = new AliBODY("BODY", "Alice envelop");

    if (iMAG)
    {
        //=================== MAG parameters ============================
        // --- Start with Magnet since detector layouts may be depending ---
        // --- on the selected Magnet dimensions ---
        AliMAG *MAG = new AliMAG("MAG", "Magnet");
    }


    if (iABSO)
    {
        //=================== ABSO parameters ============================
        AliABSO *ABSO = new AliABSOv3("ABSO", "Muon Absorber");
    }

    if (iDIPO)
    {
        //=================== DIPO parameters ============================

        AliDIPO *DIPO = new AliDIPOv3("DIPO", "Dipole version 3");
    }

    if (iHALL)
    {
        //=================== HALL parameters ============================

        AliHALL *HALL = new AliHALLv3("HALL", "Alice Hall");
    }


    if (iFRAME)
    {
        //=================== FRAME parameters ============================

        AliFRAMEv2 *FRAME = new AliFRAMEv2("FRAME", "Space Frame");
	FRAME->SetHoles(1);
    }

    if (iSHIL)
    {
        //=================== SHIL parameters ============================

        AliSHIL *SHIL = new AliSHILv3("SHIL", "Shielding Version 3");
    }


    if (iPIPE)
    {
        //=================== PIPE parameters ============================

        AliPIPE *PIPE = new AliPIPEv3("PIPE", "Beam Pipe");
    }
 
    if (iITS)
    {
        //=================== ITS parameters ============================

	AliITSv11 *ITS  = new AliITSv11("ITS","ITS v11");
	ITS->SetDensityFactor(AliITSv11::kSPDSiChip, 1.35);// SPD Silicon chip
	ITS->SetDensityFactor(AliITSv11::kSPDSiSens, 1.35);// SPD Silicon sensor
	ITS->SetDensityFactor(AliITSv11::kSPDAlBus, 1.35);// SPD Aluminum Bus
	ITS->SetDensityFactor(AliITSv11::kSPDCoolPipes, 1.35);// SPD Phynox
	ITS->SetDensityFactor(AliITSv11::kSDDSiAll, 1.08);// SDD Silicon chip+sensor
    }

    if (iTPC)
    {
        //============================ TPC parameters ===================
        AliTPC *TPC = new AliTPCv2("TPC", "Default");
    }


    if (iTOF) {
        //=================== TOF parameters ============================
	AliTOF *TOF = new AliTOFv6T0("TOF", "normal TOF");
    }


    if (iHMPID)
    {
        //=================== HMPID parameters ===========================
        AliHMPID *HMPID = new AliHMPIDv3("HMPID", "normal HMPID");

    }


    if (iZDC)
    {
        //=================== ZDC parameters ============================

        AliZDC *ZDC = new AliZDCv4("ZDC", "normal ZDC");
    }

    if (iTRD)
    {
        //=================== TRD parameters ============================

        AliTRD *TRD = new AliTRDv1("TRD", "TRD slow simulator");
    }

    if (iFMD)
    {
        //=================== FMD parameters ============================
	AliFMD *FMD = new AliFMDv1("FMD", "normal FMD");
   }

    if (iMUON)
    {
        //=================== MUON parameters ===========================
        // New MUONv1 version (geometry defined via builders)
        AliMUON *MUON = new AliMUONv1("MUON", "default");
    }
    //=================== PHOS parameters ===========================

    if (iPHOS)
    {
        AliPHOS *PHOS = new AliPHOSv1("PHOS", "IHEP");
    }


    if (iPMD)
    {
        //=================== PMD parameters ============================
        AliPMD *PMD = new AliPMDv1("PMD", "normal PMD");
    }

    if (iT0)
    {
        //=================== T0 parameters ============================
        AliT0 *T0 = new AliT0v1("T0", "T0 Detector");
    }

    if (iEMCAL)
    {
        //=================== EMCAL parameters ============================
        AliEMCAL *EMCAL = new AliEMCALv2("EMCAL", "EMCAL_COMPLETE12SMV1");
    }

     if (iACORDE)
    {
        //=================== ACORDE parameters ============================
        AliACORDE *ACORDE = new AliACORDEv1("ACORDE", "normal ACORDE");
    }

     if (iVZERO)
    {
        //=================== VZERO parameters ============================
        AliVZERO *VZERO = new AliVZEROv7("VZERO", "normal VZERO");
    }

     AliLog::Message(AliLog::kInfo, "End of Config", "Config.C", "Config.C", "Config()"," Config.C", __LINE__);

}

Float_t EtaToTheta(Float_t arg){
  return (180./TMath::Pi())*2.*atan(exp(-arg));
}


void LoadPythia()
{
    // Load Pythia related libraries
    gSystem->Load("liblhapdf");      // Parton density functions
    gSystem->Load("libEGPythia6");   // TGenerator interface
    gSystem->Load("libpythia6");     // Pythia
    gSystem->Load("libAliPythia6");  // ALICE specific implementations
}
