/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/*
$Log$
Revision 1.2  2001/10/15 08:15:51  morsch
Event vertex and vertex truncation setting moved into AliMC.

Revision 1.1  2001/07/13 10:56:00  morsch
AliGenMC base class for AliGenParam and AliGenPythia commonalities.

*/

#include "AliGenMC.h"
#include "AliPDG.h"
#include <TParticle.h>

 ClassImp(AliGenMC)

AliGenMC::AliGenMC()
                 :AliGenerator()
{
// Default Constructor
    SetCutOnChild();
    SetChildMomentumRange();
    SetChildPtRange();
    SetChildPhiRange();
    SetChildThetaRange(); 
    SetChildYRange(); 
}

AliGenMC::AliGenMC(Int_t npart)
                 :AliGenerator(npart)
{
//  Constructor
    SetCutOnChild();
    SetChildMomentumRange();
    SetChildPtRange();
    SetChildPhiRange();
    SetChildThetaRange();
    SetChildYRange(); 
// 
    fParentSelect.Set(8);
    fChildSelect.Set(8);
    for (Int_t i=0; i<8; i++) fParentSelect[i]=fChildSelect[i]=0;
    
}

AliGenMC::AliGenMC(const AliGenMC & mc)
{
// copy constructor
}

AliGenMC::~AliGenMC()
{
// Destructor
}

void AliGenMC::Init()
{
//
//  Initialization
    switch (fForceDecay) {
    case kSemiElectronic:
    case kDiElectron:
    case kBJpsiDiElectron:
    case kBPsiPrimeDiElectron:
	fChildSelect[0]=kElectron;	
	break;
    case kSemiMuonic:
    case kDiMuon:
    case kBJpsiDiMuon:
    case kBPsiPrimeDiMuon:
    case kPiToMu:
    case kKaToMu:
	fChildSelect[0]=kMuonMinus;
	break;
    case kHadronicD:
	fChildSelect[0]=kPiPlus;
	fChildSelect[1]=kKPlus;
	break;
    case kAll:
    case kNoDecay:
	break;
    }
}


Bool_t AliGenMC::ParentSelected(Int_t ip)
{
// True if particle is in list of parent particles to be selected
    for (Int_t i=0; i<8; i++)
    {
	if (fParentSelect[i]==ip) return kTRUE;
    }
    return kFALSE;
}

Bool_t AliGenMC::ChildSelected(Int_t ip)
{
// True if particle is in list of decay products to be selected
    for (Int_t i=0; i<5; i++)
    {
	if (fChildSelect[i]==ip) return kTRUE;
    }
    return kFALSE;
}

Bool_t AliGenMC::KinematicSelection(TParticle *particle, Int_t flag)
{
// Perform kinematic selection
    Float_t px    = particle->Px();
    Float_t py    = particle->Py();
    Float_t pz    = particle->Pz();
    Float_t  e    = particle->Energy();
    Float_t pt    = particle->Pt();
    Float_t p     = particle->P();
    Float_t theta = particle->Theta();
    Float_t phi   = Float_t(TMath::ATan2(Double_t(py),Double_t(px)));
    Float_t y;
    
    if ( (e-pz)<=0 || (e+pz)<=0 ) {
	return kFALSE;
    } else {
      y = 0.5*TMath::Log((e+pz)/(e-pz));
    }
    
    if (flag == 0) {
//
// Primary particle cuts
//
//  transverse momentum cut    
	if (pt > fPtMax || pt < fPtMin) {
//	    printf("\n failed pt cut %f %f %f \n",pt,fPtMin,fPtMax);
	    return kFALSE;
	}
//
// momentum cut
	if (p > fPMax || p < fPMin) {
//	    printf("\n failed p cut %f %f %f \n",p,fPMin,fPMax);
	    return kFALSE;
	}
//
// theta cut
	if (theta > fThetaMax || theta < fThetaMin) {
//	    printf("\n failed theta cut %f %f %f \n",theta,fThetaMin,fThetaMax);
	    return kFALSE;
	}
//
// rapidity cut
	if (y > fYMax || y < fYMin) {
//	    printf("\n failed y cut %f %f %f \n",y,fYMin,fYMax);
	    return kFALSE;
	}
//
// phi cut
	if (phi > fPhiMax || phi < fPhiMin) {
//	    printf("\n failed phi cut %f %f %f \n",phi,fPhiMin,fPhiMax);
	    return kFALSE;
	}
    } else {
//
// Decay product cuts
//
//  transverse momentum cut    
	if (pt > fChildPtMax || pt < fChildPtMin) {
//	    printf("\n failed pt cut %f %f %f \n",pt,fChildPtMin,fChildPtMax);
	    return kFALSE;
	}
//
// momentum cut
	if (p > fChildPMax || p < fChildPMin) {
//	    printf("\n failed p cut %f %f %f \n",p,fChildPMin,fChildPMax);
	    return kFALSE;
	}
//
// theta cut
	if (theta > fChildThetaMax || theta < fChildThetaMin) {
//	    printf("\n failed theta cut %f %f %f \n",theta,fChildThetaMin,fChildThetaMax);
	    return kFALSE;
	}
//
// rapidity cut
	if (y > fChildYMax || y < fChildYMin) {
//	    printf("\n failed y cut %f %f %f \n",y,fChildYMin,fChildYMax);
	    return kFALSE;
	}
//
// phi cut
	if (phi > fChildPhiMax || phi < fChildPhiMin) {
//	    printf("\n failed phi cut %f %f %f \n",phi,fChildPhiMin,fChildPhiMax);
	    return kFALSE;
	}
    }
    
    

    return kTRUE;
}

Int_t AliGenMC::CheckPDGCode(Int_t pdgcode)
{
//
//  If the particle is in a diffractive state, then take action accordingly
  switch (pdgcode) {
  case 91:
    return 92;
  case 110:
    //rho_diff0 -- difficult to translate, return rho0
    return 113;
  case 210:
    //pi_diffr+ -- change to pi+
    return 211;
  case 220:
    //omega_di0 -- change to omega0
    return 223;
  case 330:
    //phi_diff0 -- return phi0
    return 333;
  case 440:
    //J/psi_di0 -- return J/psi
    return 443;
  case 2110:
    //n_diffr -- return neutron
    return 2112;
  case 2210:
    //p_diffr+ -- return proton
    return 2212;
  }
  //non diffractive state -- return code unchanged
  return pdgcode;
}
	  
AliGenMC& AliGenMC::operator=(const  AliGenMC& rhs)
{
// Assignment operator
    return *this;
}

