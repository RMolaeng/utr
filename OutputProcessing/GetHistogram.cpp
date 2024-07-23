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


//////Modified by Refilwe-18 July 2024////////////////////

#include <argp.h>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::map;

// Program documentation.
static char doc[] = "Create histograms of energy depositions in detectors from a list of events stored among multiple ROOT files";
// Description of the accepted/required arguments
static char args_doc[] = ""; // No arguments, only options!

// The options argp understands
static struct argp_option options[] = {
    {"tree", 't', "TREENAME", 0, "Name of tree composing the list of events to process (default: utr)"},
    {"pattern1", 'p', "PATTERN1", 0, "First string files must contain to be processed (default: utr)"},
    {"pattern2", 'q', "PATTERN2", 0, "Second string files must contain to be processed (default: .root)"},
    {"inputdir", 'd', "INPUTDIR", 0, "Directory to search for input files matching the patterns (default: current working directory '.' )"},
    {"filename", 'o', "OUTPUTFILENAME", 0, "Output file name, file will be overwritten! (default: {PATTERN1}_hist.root with a trailing '_t' in PATTERN1 dropped)"},
    {"outputdir", 'O', "OUTPUTDIR", 0, "Directory in which the output files will be written (default: same as INPUTDIR)"},
    {"binning", 'b', "BINNING", 0, "Size of bins in the histogram in keV (default: 1 keV)"},
    {"maxenergy", 'e', "EMAX", 0, "Maximum energy displayed in histogram in MeV (rounded up to match BINNING) (default: 10 MeV)"},
    {"showbin", 'B', "BIN", 0, "Number of energy bin whose value should be displayed, -1 to disable (default: -1)"},
    {"maxid", 'n', "MAXID", 0, "Highest detection volume ID (default: 12). 'getHistogram' only processes energy depositions in detectors with integer volume ID numbers from 0 to MAXID (MAXID is included)."},
    {"multiplicity", 'm', "MULTIPLICITY", 0, "Particle multiplicity, sum energy depositions for each detector among MULTIPLICITY events (default: 1)"},
    {"addback", 'a', 0, 0, "Add back energy depositions that occurred in a single event to the detector first listed in the event (usually this is the first one hit) (default: Off)"},
    {"silent", 's', 0, 0, "Silent mode (does not silence -B option) (default: Off"},
    {0, 0, 0, 0, 0}};

// Used by main to communicate with parse_opt
struct arguments {
    string tree = "utr";
    string p1 = "utr";
    string p2 = ".root";
    string inputDir = ".";
    string outputFilename = "";
    string outputDir = "";
    double binning = 1. / 1000.;
    double eMax = 10.;
    int binToPrint = -1;
    unsigned int nhistograms = 12 + 1; // Default value for MAXID of 12 and +1 (histograms 0 to 12)
    unsigned int multiplicity = 1;
    bool addback = false;
    bool verbose = true;
};

// Function to parse a single option
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = (struct arguments *)state->input;

    switch (key) {
        case 't':
            arguments->tree = arg;
            break;
        case 'p':
            arguments->p1 = arg;
            break;
        case 'q':
            arguments->p2 = arg;
            break;
        case 'd':
            arguments->inputDir = arg;
            break;
        case 'o':
            arguments->outputFilename = arg;
            break;
        case 'O':
            arguments->outputDir = arg;
            break;
        case 'b':
            arguments->binning = atof(arg) / 1000.;
            break;
        case 'e':
            arguments->eMax = atof(arg);
            break;
        case 'B':
            arguments->binToPrint = atoi(arg);
            break;
        case 'n':
            arguments->nhistograms = (unsigned int)atoi(arg) + 1;
            break; // = MAXID + 1 (histograms 0 to MAXID)
        case 'm':
            arguments->multiplicity = (unsigned int)atoi(arg);
            break;
        case 'a':
            arguments->addback = true;
            break;
        case 's':
            arguments->verbose = false;
            break;
        case ARGP_KEY_ARG:
            cerr << "> Error: getHistogram takes only options and no arguments!" << endl;
            argp_usage(state);
            break;
        case ARGP_KEY_END:
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[]) {
    struct arguments arguments;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // If no outputDir was given, use the same as inputDir
    if (arguments.outputDir == "") {
        arguments.outputDir = arguments.inputDir;
    }

    // If no outputFilename was given, create an outputFilename based on pattern1 with "_hist.root" appended
    if (arguments.outputFilename == "") {
        if (arguments.p1.size() >= 2 && arguments.p1.compare(arguments.p1.size() - 2, 2, "_t") == 0) {
            arguments.outputFilename = arguments.p1.substr(0, arguments.p1.size() - 2) + "_hist.root";
        } else {
            arguments.outputFilename = arguments.p1 + "_hist.root";
        }
    }

    if (arguments.verbose) {
        cout << "#############################################" << endl;
        cout << "> getHistogram" << endl;
        cout << "> TREENAME     : " << arguments.tree << endl;
        cout << "> FILES        : "
             << "*" << arguments.p1 << "*" << arguments.p2 << "*" << endl;
        cout << "> INPUTDIR     : " << arguments.inputDir << endl;
        cout << "> OUTPUTFILE   : " << arguments.outputFilename << endl;
        cout << "> OUTPUTDIR    : " << arguments.outputDir << endl;
        cout << "> BINNING      : " << arguments.binning * 1000 << " keV" << endl;
        cout << "> EMAX         : " << arguments.eMax << " MeV" << endl;
        cout << "> MAXID        : " << arguments.nhistograms - 1 << endl;
        cout << "> MULTIPLICITY : " << arguments.multiplicity << endl;
        if (arguments.binToPrint != -1) {
            cout << "> BIN          : " << arguments.binToPrint << endl;
        }
        cout << "> ADDBACK      : ";
        if (arguments.addback) {
            cout << "TRUE" << endl;
        } else {
            cout << "FALSE" << endl;
        }
        cout << "#############################################" << endl;
    }


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

    // Find all files in the input directory that contain pattern1 and pattern2, and connect them to a TChain
    if (!opendir(arguments.inputDir.c_str())) {
        cerr << "> ERROR: Supplied INPUTDIR is not a valid directory! Aborting..." << endl;
        exit(1);
    }
    if (!opendir(arguments.outputDir.c_str())) {
        cerr << "> ERROR: Supplied OUTPUTDIR is not a valid directory! Aborting..." << endl;
        exit(1);
    }

    // Prepare the input chain
    TChain chain(arguments.tree.c_str());
    TSystemDirectory dir(arguments.inputDir.c_str(), arguments.inputDir.c_str());
    TList *files = dir.GetListOfFiles();
    if (files) {
        TSystemFile *file;
        TString fname;
        TIter next(files);
        while ((file = (TSystemFile *)next())) {
            fname = file->GetName();
            if (!file->IsDirectory() && fname.Contains(arguments.p1.c_str()) && fname.Contains(arguments.p2.c_str())) {
                chain.Add(Form("%s/%s", arguments.inputDir.c_str(), fname.Data()));
            }
        }
    }
 
    //preparing bins for histograms
 
  const double emin = 0 - arguments.binning / 2; // Minimum energy of histograms in MeV: bin centered around 0
  const int nbins = (int)ceil((arguments.eMax - emin) / arguments.binning);// Number of bins in the histograms: Chosen so that the end of the last bin using the given binning is greater or equal to the given maximum energy
  const double eMax = emin + nbins * arguments.binning; // Maximum energy of histograms in MeV: Choosen so that it matches the given binning

  if (arguments.verbose && eMax != arguments.eMax) {
    cout << "> Rounded up EMAX from " << arguments.eMax << " MeV to " << eMax << " MeV in order to match the requested BINNING of " << arguments.binning << " MeV" << endl;
  }
    // Create the histograms
    vector<TH1D *> histograms;
    vector<TH1D *> addbackHistograms;
    vector<TH1D *> cloverHistograms;
    for (unsigned int i = 0; i < arguments.nhistograms; ++i) {
        histograms.push_back(new TH1D(Form("hist%d", i), Form("Energy histogram for detector ID %d", i), nbins, emin, eMax));
    }

    cloverHistograms.resize(5); // Resize the vector to hold nClover elements
    cloverHistograms[0] = new TH1D("he_s1_cal","S1: energy_cal", nbins, emin, eMax);
    cloverHistograms[1] = new TH1D("he_s2_cal","S2: energy_cal", nbins, emin, eMax);
    cloverHistograms[2] = new TH1D("he_s3_cal","S3: energy_cal", nbins, emin, eMax);
    cloverHistograms[3] = new TH1D("he_s4_cal","S4: energy_cal", nbins, emin, eMax);
    cloverHistograms[4] = new TH1D("he_s5_cal","S5: energy_cal", nbins, emin, eMax);

      if (arguments.addback) {
    addbackHistograms.resize(5); // Resize the vector to hold nClover elements
    addbackHistograms[0] = new TH1D("he_c1_cal","C1: energy_cal", nbins, emin, eMax);
    addbackHistograms[1] = new TH1D("he_c2_cal","C2: energy_cal", nbins, emin, eMax);
    addbackHistograms[2] = new TH1D("he_c3_cal","C3: energy_cal", nbins, emin, eMax);
    addbackHistograms[3] = new TH1D("he_c4_cal","C4: energy_cal", nbins, emin, eMax);
    addbackHistograms[4] = new TH1D("he_c5_cal","C5: energy_cal", nbins, emin, eMax);
}


    // Read energy depositions from the input chain and fill the histograms
    TTreeReader reader(&chain);
    TTreeReaderValue<double> id(reader, "volume");
    TTreeReaderValue<double> edep(reader, "edep");
    TTreeReaderValue<double> event(reader, "event");//event number is relevant for addback
  
    // Map to accumulate total energy depositions per event and group
    std::map<double, std::map<int, double>> eventGroupEdep;

   
    while (reader.Next()) {
        double eventID = *event;
        double volumeID = *id;
        double energyDeposition = *edep;
/////////////////////Fill singles-Refilwe//////////////////////////////////////////
   if (*id >= 0 && static_cast<size_t>(*id) < arguments.nhistograms) {
    histograms[static_cast<std::vector<TH1D*>::size_type>(*id)]->Fill(*edep);
    }
/////////////////Fill the sum for singles-Refilwe/////////////////////////////
    for (int n=0; n<nClover; ++n){
         for (int m=0; m<4; ++m){
            if (*id == clo_id[n][m]){
          cloverHistograms[static_cast<size_t>(n)]->Fill(*edep);
          }
      }}
////////////////////Fill the sum for addback-Refilwe///////////////////////////

    if (arguments.addback) {

      if(volumeID >= 8 && volumeID <= 27){
          int groupID;
            if (volumeID >= 8 && volumeID <= 11) groupID = 0;
            else if (volumeID >= 12 && volumeID <= 15) groupID = 1;
            else if (volumeID >= 16 && volumeID <= 19) groupID = 2;
            else if (volumeID >= 20 && volumeID <= 23) groupID = 3;
            else if (volumeID >= 24 && volumeID <= 27) groupID = 4;
 // Ensure groupID is valid
                if (groupID < 0 || groupID > 4) {
                    std::cerr << "Invalid groupID: " << groupID << "\n";
                    continue;
                }

                // Accumulate energy deposition for each event and group
                eventGroupEdep[eventID][groupID] += energyDeposition;

                // Ensure the histogram for this groupID exists
                if (static_cast<size_t>(groupID) < addbackHistograms.size()) {
                    addbackHistograms[static_cast<size_t>(groupID)]->Fill(eventGroupEdep[eventID][groupID]);
                } else {
                    std::cerr << "Histogram for groupID " << groupID << " does not exist.\n";
        }
         }}}

    // Save histograms to output file
    TFile outputFile(Form("%s/%s", arguments.outputDir.c_str(), arguments.outputFilename.c_str()), "RECREATE");
    for (auto &hist : histograms) {
        hist->Write();
    }

    for (size_t i = 0; i < cloverHistograms.size(); ++i) {
        cloverHistograms[i]->Write();
    } 

  if (arguments.addback) {
    for (size_t i = 0; i < addbackHistograms.size(); ++i) {
        addbackHistograms[i]->Write();
    }
}
    outputFile.Close();

    // Display specific bin value if requested
    if (arguments.binToPrint != -1) {
        cout << "Value of bin " << arguments.binToPrint << " is: " << histograms[0]->GetBinContent(arguments.binToPrint) << endl;
    }

    return 0;
}
