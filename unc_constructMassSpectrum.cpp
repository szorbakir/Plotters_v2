//* This program construct mass spectrums by combining trigger mass hists.

#include <iostream>
using namespace std;

//* ROOT header files
#include <TH1.h>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>

//* Function prototypes
TH1 *getClonedHist(TString pathToFile, TString pathToHist);
void storeHist(TFile *file, TH1 *massHist, TString histName);

//* Triggers and etas
TString jt[] = {"0", "40", "60", "80", "140", "200", "260", "320", "400", "450"};
TString eta[] = {"0.0", "0.5", "1.0", "1.5", "2.0", "2.5"};

constexpr size_t trgCounter = (sizeof(jt) / sizeof(*jt));
constexpr size_t etaCounter = (sizeof(eta) / sizeof(*eta));

//* Canvas width and height
constexpr double w = 1500.;
constexpr double h = 1000.;

// TString uncType = "up";
TString uncType = "down";

TString year = "2016";
// TString year = "2017";
// TString year = "2018";

TFile *storageFile;
TString sourceRoot;

TH1 *jetTrgMassHist[10];
TH1 *baseMassHist[10];

void unc_constructMassSpectrum()
{
    if (year == "2016")
    {
        if (uncType == "up")
        {
            storageFile = new TFile("massSpectrums2016_up.root", "RECREATE");
            sourceRoot = "triggerMassHists2016_up.root";
        }
        else
        {
            storageFile = new TFile("massSpectrums2016_down.root", "RECREATE");
            sourceRoot = "triggerMassHists2016_down.root";
        }
    }
    else if (year == "2017")
    {
        if (uncType == "up")
        {
            storageFile = new TFile("massSpectrums2017_up.root", "RECREATE");
            sourceRoot = "triggerMassHists2017_up.root";
        }
        else
        {
            storageFile = new TFile("massSpectrums2017_down.root", "RECREATE");
            sourceRoot = "triggerMassHists2017_down.root";
        }
    }
    else
    {
        if (uncType == "up")
        {
            storageFile = new TFile("massSpectrums2018_up.root", "RECREATE");
            sourceRoot = "triggerMassHists2018_up.root";
        }
        else
        {
            storageFile = new TFile("massSpectrums2018_down.root", "RECREATE");
            sourceRoot = "triggerMassHists2018_down.root";
        }
    }

    for (size_t i = 0; i < etaCounter - 1; i++)
    { //etaCounter - 1

        //* Zerobias(jt0) histograms
        TString pathToBaseMassHist = "mass_" + eta[i] + "-" + eta[i + 1] + "_jt" + jt[0];

        //* Fetch histograms:
        baseMassHist[i] = getClonedHist(sourceRoot, pathToBaseMassHist);

        for (size_t j = 0; j < trgCounter - 1; j++)
        { // trgCounter - 1

            //* Trigger histograms
            TString jetTrgMassHistPath = "mass_" + eta[i] + "-" + eta[i + 1] + "_jt" + jt[j + 1];

            //* Fetch histograms
            jetTrgMassHist[j] = getClonedHist(sourceRoot, jetTrgMassHistPath);

            //* Construct mass spectrum by adding each trigger hist to the Zero Bias one
            baseMassHist[i]->Add(jetTrgMassHist[j]);
        }

        //* Store constructed mass spectrums
        TString histName = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
        storeHist(storageFile, baseMassHist[i], histName);
    }
}

//* This function will fetch a histogram in the given file.
//* For integration purposes this function clones the requested histogram and then returns it.
TH1 *getClonedHist(TString pathToFile, TString pathToHist)
{

    //* Create histograms and a file to be used:
    TH1 *originalHist, *clonedHist;
    TFile *file = new TFile(pathToFile);

    //* Fetch the histogram:
    originalHist = (TH1 *)file->Get(pathToHist);

    //* If histogram fetched properly clone and return.
    //* If not print error message and exit program immediately :
    if (originalHist)
    {

        clonedHist = (TH1 *)originalHist->Clone("clonedHist");
        return clonedHist;
    }
    else
    {
        cerr << "Histogram cannot be fetched properly!" << endl;
        exit(1);
    }
}

//* This function opens pre-created root file and write hist in it.
void storeHist(TFile *file, TH1 *massHist, TString histName)
{
    file->ReOpen("UPDATE");
    massHist->Write(histName);
}