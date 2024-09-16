#include <argp.h>
#include <iostream>
#include <stdlib.h>

#include <TChain.h>
#include <TF2.h>
#include <TFile.h>
#include <TH2.h>
#include <TMath.h>
#include <TROOT.h>
#include <TSystemDirectory.h>

#include "AngularDistribution.hh"
#include <vector>  // Needed for std::vector



static char doc[] = "AngularDistributionGenerator_Test";
static char args_doc[] = "Create histogram of momentum distribution in theta and phi";

struct arguments {
  const char *tree;
  const char *p1;
  const char *p2;
  const char *outputfilename;
  bool is_unpolarized;
  bool fit;

  arguments() : tree("utr"), p1("utr"), p2(".root"), outputfilename("hist.root"), is_unpolarized(false), fit(false){};
};

static struct argp_option options[] = {
    {0, 't', "TREENAME", 0, "Name of tree"},
    {0, 'p', "PATTERN1", 0, "File name pattern 1"},
    {0, 'q', "PATTERN2", 0, "File name pattern 2"},
    {0, 'o', "OUTPUTFILENAME", 0, "Output file name"},
    {0, 'u', 0, 0, "Assume unpolarized excitation"},
    {0, 'f', 0, 0, "Fit a function to the simulation result"},
    {0, 0, 0, 0, 0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

  struct arguments *args = (struct arguments *)state->input;

  switch (key) {
    case ARGP_KEY_ARG:
      break;
    case 't':
      args->tree = arg;
      break;
    case 'p':
      args->p1 = arg;
      break;
    case 'q':
      args->p2 = arg;
      break;
    case 'o':
      args->outputfilename = arg;
      break;
    case 'u':
      args->is_unpolarized = true;
      break;
    case 'f':
      args->fit = true;
      break;
    case ARGP_KEY_END:
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

using namespace std;

// Function object used in the ROOT fit
// In order to define the same angular distribution that was used in utr, it makes use of the AngularDistribution class
class W_Function {
  public:
  W_Function(bool unp) : is_unpolarized(false) {
    //
    //	START OF USER-DEFINED OUTPUT
    //

    nstates = 3;

    states[0] = 0.;
    states[1] = -1.;
    states[2] = 2.;

    for (int i = 0; i < 4; ++i)
      alt_states[i] = states[i];
    alt_states[1] = -states[1];

    mix[0] = 0.;
    mix[1] = 0.;

    is_unpolarized = unp;

    //
    //	END OF USER-DEFINED OUTPUT
    //
  };

  Double_t operator()(Double_t *x, Double_t *par) {
    if (is_unpolarized) {
      return par[0] * sin(x[0]) * (angdist.AngDist(x[0], x[1], states, nstates, mix) + angdist.AngDist(x[0], x[1], alt_states, nstates, mix));
    }
    return par[0] * sin(x[0]) * angdist.AngDist(x[0], x[1], states, nstates, mix);
  }

  double states[4];
  double alt_states[4];
  double mix[3];
  int nstates;
  bool is_unpolarized;

  AngularDistribution angdist;
};

int main(int argc, char *argv[]) {

  const Double_t pi = TMath::Pi();
  const Double_t two_pi = TMath::Pi() * 2.;

  struct arguments args;
  argp_parse(&argp, argc, argv, 0, 0, &args);

  cout << "#############################################" << endl;
  cout << "> AngularDistributionGenerator" << endl;
  cout << "> TREENAME     : " << args.tree << endl;
  cout << "> FILES        : "
       << "*" << args.p1 << "*" << args.p2 << "*" << endl;
  cout << "> OUTPUTFILE   : " << args.outputfilename << endl;
  cout << "> FIT          : " << args.fit << endl;
  cout << "> UNPOLARIZED  : " << args.is_unpolarized << endl;
  cout << "#############################################" << endl;

  // Find all files in the current directory that contain pattern1 and pattern1 and connect them to a TChain
  TSystemDirectory dir(".", ".");

  TList *files = dir.GetListOfFiles();
  TChain utr("utr");

  cout << "> Joining all files that contain '" << args.p1 << "' and '" << args.p2 << "':" << endl;

  if (files) {
    TSystemFile *file;
    TString fname;

    TIter next(files);
    file = (TSystemFile *)next();
    while (file) {
      fname = file->GetName();
      if (!file->IsDirectory() && fname.Contains(args.p1) && fname.Contains(args.p2)) {
        cout << fname << endl;
        utr.Add(fname);
      }
      file = (TSystemFile *)next();
    }
  }

  // Create histogram

  Int_t nbins_phi = 50;
  Int_t nbins_theta = 50;
  Double_t phi_low = 0.;
  Double_t phi_up = two_pi;
  Double_t theta_low = 0.;
  Double_t theta_up = pi;

  //////////////////Define my clovers for addback//////////////////////
  int  nClover = 5;
  int clo_id[5][4];
  std::vector<double> clov_energy(static_cast<size_t>(nClover), 0.0);
 
  clo_id[0][0] = 8;
	clo_id[0][1] = 9;
	clo_id[0][2] = 10;
	clo_id[0][3] = 11;
	
	clo_id[1][0] = 12;
	clo_id[1][1] = 13;
	clo_id[1][2] = 14;
	clo_id[1][3] = 15;

	clo_id[2][0] = 16;
	clo_id[2][1] = 17;
	clo_id[2][2] = 18;
	clo_id[2][3] = 19;
	
	clo_id[3][0] = 20;
	clo_id[3][1] = 21;
	clo_id[3][2] = 22;
	clo_id[3][3] = 23;
	
	clo_id[4][0] = 24;
	clo_id[4][1] = 25;
	clo_id[4][2] = 26;
	clo_id[4][3] = 27;

  ////////////////////////////////////////////////////////////////////



  // Declare the histograms vector
   std::vector<TH2F*> histograms;
   std::vector<TH2F*> cloverHistograms;
  //TH2F *hist = new TH2F("hist", "Momentum distribution in (theta, phi)", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up);
   for (unsigned int i = 0; i < 28; ++i) {
        histograms.push_back(new  TH2F(Form("hist%d", i), Form("Momentum distribution in (theta, phi) ID %d", i), nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up));
    }
    cloverHistograms.resize(5); // Resize the vector to hold nClover elements
    cloverHistograms[0] = new TH2F("ang_s1","S1: angular_dist", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up);
    cloverHistograms[1] = new TH2F("ang_s2","S2: angular_dist", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up);
    cloverHistograms[2] = new TH2F("ang_s3","S3: angular_dist", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up);
    cloverHistograms[3] = new TH2F("ang_s4","S4: angular_dist", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up);
    cloverHistograms[4] = new TH2F("ang_s5","S5: angular_dist", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up);

  // Fill histogram from TBranch in TChain with user-defined conditions
  // Define variables and read their values from the tree using the GetEntry() method
  Double_t momentum_x, momentum_y, momentum_z, detector_id;

  utr.SetBranchAddress("vx", &momentum_x);
  utr.SetBranchAddress("vy", &momentum_y);
  utr.SetBranchAddress("vz", &momentum_z);
  utr.SetBranchAddress("volume", &detector_id);



  for (int i = 0; i < utr.GetEntries(); i++) {
    utr.GetEntry(i);

    // Need to distinguish between different sectors of the unit circle when filling the histogram
 
    // Initialize x, y, z with momentum components
    Double_t x = momentum_x, y = momentum_y, z = momentum_z;
    Double_t theta, phi;

    // Calculate theta and phi
    //calculateThetaPhi(x, y, z, theta, phi);
    theta = acos(z / sqrt(x * x + y * y + z * z));
    if (x >= 0. && y < 0.) {
        phi = 2. * TMath::Pi() + atan(y / x);
    } else if (x >= 0. && y >= 0.) {
        phi = atan(y / x);
    } else {
        phi = TMath::Pi() + atan(y / x);
    }


      if (detector_id >= 0 && static_cast<size_t>(detector_id) < histograms.size()) {
        histograms[static_cast<std::vector<TH2F*>::size_type>(detector_id)]->Fill(theta, phi);
    }

      for (int n=0; n<nClover; ++n){
         for (int m=0; m<4; ++m){
            if (detector_id == clo_id[n][m]){
          cloverHistograms[static_cast<size_t>(n)]->Fill(theta, phi);
          }
      }}
  }

  // Create output ROOT file and write histograms
    TFile *of = new TFile(args.outputfilename, "RECREATE");
    for (auto &hist : histograms) {
        hist->Write();
    }

    for (size_t i = 0; i < cloverHistograms.size(); ++i) {
        cloverHistograms[i]->Write();
    } 
    of->Close();

    std::cout << "> Created output file " << args.outputfilename << std::endl;

    return 0;
}
