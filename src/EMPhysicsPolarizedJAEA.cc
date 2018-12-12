/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

// Physics list largely based on G4EmLivermorePolarizedPhysics in Geant4 version 10.5
// Modified to include elastic scattering processes implemented by
// Omer and Hajima (see references below)
//
// ------------------------------------------------------------------------------------------
// Authors:
// M. Omer and R. Hajima  on   17 October 2016
// contact:
// omer.mohamed@jaea.go.jp and hajima.ryoichi@qst.go.jp
// Publication Information:
// 1- M. Omer, R. Hajima, Including Delbrück scattering in Geant4,
// Nucl. Instrum. Methods Phys. Res. Sect. B, vol. 405, 2017, pp. 43-49.,
// https://doi.org/10.1016/j.nimb.2017.05.028
// 2- M. Omer, R. Hajima, Geant4 physics process for elastic scattering of gamma-rays,
// JAEA Technical Report 2018-007, 2018.
// https://doi.org/10.11484/jaea-data-code-2018-007
// ------------------------------------------------------------------------------------------

#include "EMPhysicsPolarizedJAEA.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

// *** Processes and models

// gamma
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePolarizedPhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermorePolarizedComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermorePolarizedGammaConversionModel.hh"

//#include "G4RayleighScattering.hh" 
//#include "G4LivermorePolarizedRayleighModel.hh"
#include "G4JAEAElasticScattering.hh"
#include "G4JAEAElasticScatteringModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4PhotoElectricAngularGeneratorPolarized.hh"

#include "G4PEEffectFluoModel.hh"
#include "G4KleinNishinaModel.hh"

// e+-
#include "G4eMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"
#include "G4Generator2BS.hh"
#include "G4SeltzerBergerModel.hh"

// e+
#include "G4eplusAnnihilation.hh"

// mu+-
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4MuBremsstrahlungModel.hh"
#include "G4MuPairProductionModel.hh"
#include "G4hBremsstrahlungModel.hh"
#include "G4hPairProductionModel.hh"

// hadrons
#include "G4hMultipleScattering.hh"
#include "G4MscStepLimitType.hh"

#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
#include "G4ePairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4alphaIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

// msc models
#include "G4UrbanMscModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"

// interfaces
#include "G4LossTableManager.hh"
#include "G4EmParameters.hh"
#include "G4UAtomicDeexcitation.hh"

// particles
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

//
#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
#include "G4EmModelActivator.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(EMPhysicsPolarizedJAEA);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EMPhysicsPolarizedJAEA::EMPhysicsPolarizedJAEA(G4int ver, 
							     const G4String&)
  : G4VPhysicsConstructor("G4EmLivermorePolarized"), verbose(ver)
{
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetVerbose(verbose);
  param->SetMinEnergy(100*eV);
  param->SetLowestElectronEnergy(100*eV);
  param->SetNumberOfBinsPerDecade(20);
  param->ActivateAngularGeneratorForIonisation(true);
  param->SetStepFunction(0.2, 10*um);
  param->SetStepFunctionMuHad(0.1, 10*um);
  param->SetUseMottCorrection(true);  
  param->SetMscStepLimitType(fUseSafetyPlus);
  param->SetMscSkin(3);            
  param->SetMscRangeFactor(0.2);   
  param->SetMuHadLateralDisplacement(true);
  param->SetFluo(true);
  //param->SetAugerCascade(true);
  SetPhysicsType(bElectromagnetic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EMPhysicsPolarizedJAEA::~EMPhysicsPolarizedJAEA()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EMPhysicsPolarizedJAEA::ConstructParticle()
{
  // gamma
  G4Gamma::Gamma();

  // leptons
  G4Electron::Electron();
  G4Positron::Positron();
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();

  // mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();

  // baryons
  G4Proton::Proton();
  G4AntiProton::AntiProton();

  // ions
  G4Deuteron::Deuteron();
  G4Triton::Triton();
  G4He3::He3();
  G4Alpha::Alpha();
  G4GenericIon::GenericIonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EMPhysicsPolarizedJAEA::ConstructProcess()
{
  if(verbose > 1) {
    G4cout << "### " << GetPhysicsName() << " Construct Processes " << G4endl;
  }
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  G4LossTableManager* man = G4LossTableManager::Instance();

  // muon & hadron bremsstrahlung and pair production
  G4MuBremsstrahlung* mub = new G4MuBremsstrahlung();
  G4MuPairProduction* mup = new G4MuPairProduction();
  G4hBremsstrahlung* pib = new G4hBremsstrahlung();
  G4hPairProduction* pip = new G4hPairProduction();
  G4hBremsstrahlung* kb = new G4hBremsstrahlung();
  G4hPairProduction* kp = new G4hPairProduction();
  G4hBremsstrahlung* pb = new G4hBremsstrahlung();
  G4hPairProduction* pp = new G4hPairProduction();
  G4ePairProduction* ee = new G4ePairProduction();

  // muon & hadron multiple scattering
  G4MuMultipleScattering* mumsc = new G4MuMultipleScattering();
  mumsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* muss = new G4CoulombScattering();
  
  G4hMultipleScattering* pimsc = new G4hMultipleScattering();
  pimsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* piss = new G4CoulombScattering();

  G4hMultipleScattering* kmsc = new G4hMultipleScattering();
  kmsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* kss = new G4CoulombScattering();
  
  G4hMultipleScattering* hmsc = new G4hMultipleScattering("ionmsc");

  // high energy limit for e+- scattering models
  G4double highEnergyLimit = G4EmParameters::Instance()->MscEnergyLimit();
  G4double livEnergyLimit = 1*GeV;

  // nuclear stopping
  G4NuclearStopping* pnuc = new G4NuclearStopping();
  G4NuclearStopping* inuc = new G4NuclearStopping();

  // Add Livermore EM Processes
  G4ParticleTable* table = G4ParticleTable::GetParticleTable();
  for(const auto& particleName : partList.PartNames()) {
    G4ParticleDefinition* particle = table->FindParticle(particleName);
    if (!particle) { continue; }
    if (particleName == "gamma") {

      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4VEmModel* mod = new G4LivermorePhotoElectricModel();
      mod->SetAngularDistribution(new G4PhotoElectricAngularGeneratorPolarized());
      thePhotoElectricEffect->SetEmModel(mod);

      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      theComptonScattering->SetEmModel(new G4KleinNishinaModel());
      G4VEmModel* comptLiv = new G4LivermorePolarizedComptonModel();
      comptLiv->SetHighEnergyLimit(livEnergyLimit);
      theComptonScattering->AddEmModel(0, comptLiv);

      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4VEmModel* convLiv = new G4LivermorePolarizedGammaConversionModel();
      convLiv->SetHighEnergyLimit(livEnergyLimit);
      theGammaConversion->AddEmModel(0, convLiv);

      //G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      //theRayleigh->SetEmModel(new G4LivermorePolarizedRayleighModel());

      G4JAEAElasticScattering* theElasticScattering = new G4JAEAElasticScattering();
      theElasticScattering->SetEmModel(new G4JAEAElasticScatteringModel() );

      ph->RegisterProcess(thePhotoElectricEffect, particle);
      ph->RegisterProcess(theComptonScattering, particle);
      ph->RegisterProcess(theGammaConversion, particle);
      //ph->RegisterProcess(theRayleigh, particle);
      ph->RegisterProcess(theElasticScattering, particle);

    } else if (particleName == "e-") {

      // multiple and single scattering
      G4eMultipleScattering* msc = new G4eMultipleScattering;
      msc->SetStepLimitType(fUseDistanceToBoundary);
      G4GoudsmitSaundersonMscModel* msc1 = new G4GoudsmitSaundersonMscModel();
      G4WentzelVIModel* msc2 = new G4WentzelVIModel();
      msc1->SetHighEnergyLimit(highEnergyLimit);
      msc2->SetLowEnergyLimit(highEnergyLimit);
      msc->SetEmModel(msc1);
      msc->SetEmModel(msc2);

      G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
      G4CoulombScattering* ss = new G4CoulombScattering();
      ss->SetEmModel(ssm); 
      ss->SetMinKinEnergy(highEnergyLimit);
      ssm->SetLowEnergyLimit(highEnergyLimit);
      ssm->SetActivationLowEnergyLimit(highEnergyLimit);
      
      // Ionisation
      G4eIonisation* eioni = new G4eIonisation();
      G4LivermoreIonisationModel* theIoniLivermore = new
        G4LivermoreIonisationModel();
      theIoniLivermore->SetHighEnergyLimit(0.1*MeV); 
      eioni->AddEmModel(0, theIoniLivermore, new G4UniversalFluctuation() );
      
      // Bremsstrahlung from standard
      G4eBremsstrahlung* brem = new G4eBremsstrahlung();
      G4SeltzerBergerModel* br1 = new G4SeltzerBergerModel();
      G4eBremsstrahlungRelModel* br2 = new G4eBremsstrahlungRelModel();
      br1->SetAngularDistribution(new G4Generator2BS());
      br2->SetAngularDistribution(new G4Generator2BS());
      brem->SetEmModel(br1);
      brem->SetEmModel(br2);
      br1->SetHighEnergyLimit(GeV);

      // register processes
      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(eioni, particle);
      ph->RegisterProcess(brem, particle);
      ph->RegisterProcess(ee, particle);
      ph->RegisterProcess(ss, particle);

    } else if (particleName == "e+") {

      // multiple scattering
      G4eMultipleScattering* msc = new G4eMultipleScattering;
      msc->SetStepLimitType(fUseDistanceToBoundary);
      G4GoudsmitSaundersonMscModel* msc1 = new G4GoudsmitSaundersonMscModel();
      G4WentzelVIModel* msc2 = new G4WentzelVIModel();
      msc1->SetHighEnergyLimit(highEnergyLimit);
      msc2->SetLowEnergyLimit(highEnergyLimit);
      msc->SetEmModel(msc1);
      msc->SetEmModel(msc2);

      G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
      G4CoulombScattering* ss = new G4CoulombScattering();
      ss->SetEmModel(ssm); 
      ss->SetMinKinEnergy(highEnergyLimit);
      ssm->SetLowEnergyLimit(highEnergyLimit);
      ssm->SetActivationLowEnergyLimit(highEnergyLimit);

      // Ionisation
      G4eIonisation* eioni = new G4eIonisation();

      // Bremsstrahlung from standard
      G4eBremsstrahlung* brem = new G4eBremsstrahlung();
      G4SeltzerBergerModel* br1 = new G4SeltzerBergerModel();
      G4eBremsstrahlungRelModel* br2 = new G4eBremsstrahlungRelModel();
      br1->SetAngularDistribution(new G4Generator2BS());
      br2->SetAngularDistribution(new G4Generator2BS());
      brem->SetEmModel(br1);
      brem->SetEmModel(br2);
      br1->SetHighEnergyLimit(GeV);

      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(eioni, particle);
      ph->RegisterProcess(brem, particle);
      ph->RegisterProcess(ee, particle);
      ph->RegisterProcess(new G4eplusAnnihilation(), particle);
      ph->RegisterProcess(ss, particle);

    } else if (particleName == "mu+" ||
               particleName == "mu-"    ) {

      G4MuIonisation* muIoni = new G4MuIonisation();

      ph->RegisterProcess(mumsc, particle);
      ph->RegisterProcess(muIoni, particle);
      ph->RegisterProcess(mub, particle);
      ph->RegisterProcess(mup, particle);
      ph->RegisterProcess(muss, particle);

    } else if (particleName == "alpha" ||
               particleName == "He3" ) {
      
      G4hMultipleScattering* msc = new G4hMultipleScattering();
      G4ionIonisation* ionIoni = new G4ionIonisation();

      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(ionIoni, particle);
      ph->RegisterProcess(pnuc, particle);

    } else if (particleName == "GenericIon") {
      
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 1*um);

      ph->RegisterProcess(hmsc, particle);
      ph->RegisterProcess(ionIoni, particle);
      ph->RegisterProcess(inuc, particle);

    } else if (particleName == "pi+" ||
               particleName == "pi-" ) {

      G4hIonisation* hIoni = new G4hIonisation();

      ph->RegisterProcess(pimsc, particle);
      ph->RegisterProcess(hIoni, particle);
      ph->RegisterProcess(pib, particle);
      ph->RegisterProcess(pip, particle);
      ph->RegisterProcess(piss, particle);

    } else if (particleName == "kaon+" ||
               particleName == "kaon-" ) {

      G4hIonisation* hIoni = new G4hIonisation();

      ph->RegisterProcess(kmsc, particle);
      ph->RegisterProcess(hIoni, particle);
      ph->RegisterProcess(kb, particle);
      ph->RegisterProcess(kp, particle);
      ph->RegisterProcess(kss, particle);

    } else if (particleName == "proton" ||
	       particleName == "anti_proton") {

      G4hMultipleScattering* pmsc = new G4hMultipleScattering();
      pmsc->SetEmModel(new G4WentzelVIModel());
      G4hIonisation* hIoni = new G4hIonisation();

      ph->RegisterProcess(pmsc, particle);
      ph->RegisterProcess(hIoni, particle);
      ph->RegisterProcess(pb, particle);
      ph->RegisterProcess(pp, particle);
      ph->RegisterProcess(new G4CoulombScattering(), particle);
      ph->RegisterProcess(pnuc, particle);

    } else if (particleName == "B+" ||
	       particleName == "B-" ||
	       particleName == "D+" ||
	       particleName == "D-" ||
	       particleName == "Ds+" ||
	       particleName == "Ds-" ||
               particleName == "anti_He3" ||
               particleName == "anti_alpha" ||
               particleName == "anti_deuteron" ||
               particleName == "anti_lambda_c+" ||
               particleName == "anti_omega-" ||
               particleName == "anti_sigma_c+" ||
               particleName == "anti_sigma_c++" ||
               particleName == "anti_sigma+" ||
               particleName == "anti_sigma-" ||
               particleName == "anti_triton" ||
               particleName == "anti_xi_c+" ||
               particleName == "anti_xi-" ||
               particleName == "deuteron" ||
	       particleName == "lambda_c+" ||
               particleName == "omega-" ||
               particleName == "sigma_c+" ||
               particleName == "sigma_c++" ||
               particleName == "sigma+" ||
               particleName == "sigma-" ||
               particleName == "tau+" ||
               particleName == "tau-" ||
               particleName == "triton" ||
               particleName == "xi_c+" ||
               particleName == "xi-" ) {
      ph->RegisterProcess(hmsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
      ph->RegisterProcess(pnuc, particle);
    }
  }
    
  // Nuclear stopping
  pnuc->SetMaxKinEnergy(MeV);
  inuc->SetMaxKinEnergy(MeV);

  // Deexcitation
  //
  G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  man->SetAtomDeexcitation(de);

  G4EmModelActivator mact(GetPhysicsName());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
