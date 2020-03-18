//* This code will do:
//*   - Find turn on points for jet triggers with respect to mass variable and store them
//*   - Cut zero bias and jet trigger hists using turn on points
//*   - Store cutted hist in a root file

//* Abbreviations:
//* trg -> trigger, ref -> reference, ZB -> zero bias

#include <iostream>
using namespace std;

//* ROOT header files
#include <TH1.h>
#include <TF1.h>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TLatex.h>

//* Function prototypes
TH1 *getClonedHist(TString pathToFile, TString pathToHist);
TF1 *defineFitFunction(int jetTrgThreshold, double ParameterForFirstRangeValue, double lastRangeValue, double maxEta, double curveParameter);
void storeTurnOnPoints();
void cutTrgHists();
void storeHist(TFile *file, TH1 *massHist, TString histName);
void cutZBHists();

//* Global variables definitions
bool debug = false;
bool drawTurnOnHists = true;
bool cutHists = true;

//* Triggers and etas
TString jt[] = {"0", "40", "60", "80", "140", "200", "260", "320", "400", "450"};
TString eta[] = {"0.0", "0.5", "1.0", "1.5", "2.0", "2.5"};

constexpr size_t trgCounter = (sizeof(jt) / sizeof(*jt));
constexpr size_t etaCounter = (sizeof(eta) / sizeof(*eta));

//* Canvas width and height
constexpr double w = 1500.;
constexpr double h = 1000.;

double turn_on[etaCounter][trgCounter - 1];

TCanvas *c1 = new TCanvas("c1", "c1", w, h);
TCanvas *c2 = new TCanvas("c2", "c2", w, h);
TCanvas *c3 = new TCanvas("c3", "c3", w, h);

//* Store hist in this file
TFile *storageFile = new TFile("triggerMassHists2016.root", "RECREATE");
// TFile *storageFile = new TFile("triggerMassHists2017.root", "RECREATE");
// TFile *storageFile = new TFile("triggerMassHists2018.root", "RECREATE");

//* Path to DATA file to be constructed
TString pathToDataFile = "datas/2016/DATA/cleaned/output-DATA-Run2016-2a.root";
// TString pathToDataFile = "datas/2017/DATA/std_noVeto_JECunc/output-DATA-Run2017-2a.root";
// TString pathToDataFile = "datas/2018/DATA/STD+ECALVETO/output-DATA-Run2018-2a.root";

TString year = "2016";
// TString year = "2017";
// TString year = "2018";

void constructTriggerMassHists()
{

  //* This function will store turn-on points for jet trigger with respect to mass variable
  storeTurnOnPoints();

  if (cutHists)
  {
    //* This function will cut zero bias hists
    cutZBHists();

    //* This function will cut jet trigger hists
    cutTrgHists();
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

//* This function will find turn-on points for each jet triggers.
void storeTurnOnPoints()
{
  TH1 *massHist[10];
  TH1 *refMassHist[10];
  TF1 *fitFunction[10];

  for (size_t i = 0; i < 5; i++)
  { //etaCounter - 1
    for (size_t j = 0; j < trgCounter - 1; j++)
    { // trgCounter - 1

      //* Create TString variables for fetching histograms:
      TString pathToRefMassHist = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/jt" + jt[j] + "/hdjmass";
      TString PathToMassHist = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/jt" + jt[j + 1] + "/hdjmass";

      //* Fetch histograms:
      refMassHist[j] = getClonedHist(pathToDataFile, pathToRefMassHist);
      massHist[j] = getClonedHist(pathToDataFile, PathToMassHist);

      //* Default parameters for fit function but most likely they'll be modified for each jet triggers:
      //* Fit will be applied by given range
      //*   first range value -> 2ptcosh(maxEta) * parameterForFirstRangeValue
      //*   last range value -> lastRangeValue
      //* curve parameter -> will change the curviness of the fit.
      //* Curve paramater is used to get a better fit result for some jet triggers
      double parameterForFirstRangeValue = 1.1;
      double lastRangeValue = 2000;
      double maxEta = atoi(eta[i + 1]);
      double curveParameter = 20;

      if (i == 0 && j == 0) // 0.0 - 0.5 - jt40
      {
        parameterForFirstRangeValue = 1.7;
        lastRangeValue = 1000;
        maxEta = atoi(eta[i + 1]);
      }

      if (i == 0 && j == 1) // 0.0 - 0.5 - jt60
      {
        parameterForFirstRangeValue = 1.7;
        lastRangeValue = 1000;
        maxEta = atoi(eta[i + 1]);
      }

      if (i == 0 && j == 2) // 0.0 - 0.5 - jt80
      {
        parameterForFirstRangeValue = 1.7;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if (i == 0 && j == 3) // 0.0 - 0.5 - jt140
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if (i == 0 && j == 4) // 0.0 - 0.5 - jt200
      {
        parameterForFirstRangeValue = 1.85;
        lastRangeValue = 2500;
        maxEta = atoi(eta[i + 1]);
      }

      if (i == 0 && j == 5) // 0.0 - 0.5 - jt260
      {
        parameterForFirstRangeValue = 1.7;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if (i == 0 && j == 6) // 0.0 - 0.5 - jt320
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if (i == 0 && j == 7) // 0.0 - 0.5 - jt400
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 0) && j == 8) // 0.0 - 0.5 - jt450
      {
        parameterForFirstRangeValue = 1.9;
        lastRangeValue = 4000;
        maxEta = atoi(eta[i + 1]);
      }

      //-------------------------------------------------------------------------------------------//

      if ((i == 1) && j == 0) // 0.5 - 1.0 - jt40
      {
        parameterForFirstRangeValue = 2.2;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 1) && j == 1) // 0.5 - 1.0 - jt60
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 1) && j == 2) // 0.5 - 1.0 - jt80
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 1) && j == 3) // 0.5 - 1.0 - jt140
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 1) && j == 4) // 0.5 - 1.0 - jt200
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 1) && j == 5) // 0.5 - 1.0 - jt260
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 1) && j == 6) // 0.5 - 1.0 - jt320
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 1) && j == 7) // 0.5 - 1.0 - jt400
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 4000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 1) && j == 8) // 0.5 - 1.0 - jt450
      {
        parameterForFirstRangeValue = 1.8;
        lastRangeValue = 4000;
        maxEta = atoi(eta[i + 1]);
      }

      //-------------------------------------------------------------------------------------------//

      if ((i == 2) && j == 0) // 1.0 - 1.5 - jt40
      {
        parameterForFirstRangeValue = 3.5;
        lastRangeValue = 1000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 2) && j == 1) // 1.0 - 1.5 - jt60
      {
        parameterForFirstRangeValue = 2.0;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 2) && j == 2) // 1.0 - 1.5 - jt80
      {
        parameterForFirstRangeValue = 2.0;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 2) && j == 3) // 1.0 - 1.5 - jt140
      {
        parameterForFirstRangeValue = 2.0;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 2) && j == 4) // 1.0 - 1.5 - jt200
      {
        parameterForFirstRangeValue = 2.0;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 2) && j == 5) // 1.0 - 1.5 - jt260
      {
        parameterForFirstRangeValue = 2.0;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 2) && j == 6) // 1.0 - 1.5 - jt320
      {
        parameterForFirstRangeValue = 2.0;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 2) && j == 7) // 1.0 - 1.5 - jt400
      {
        parameterForFirstRangeValue = 2.0;
        lastRangeValue = 4000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 2) && j == 8) // 1.0 - 1.5 - jt450
      {
        parameterForFirstRangeValue = 2.5;
        lastRangeValue = 4000;
        maxEta = atoi(eta[i + 1]);
      }

      //-------------------------------------------------------------------------------------------//

      if ((i == 3) && j == 0) // 1.5 - 2.0 - jt40
      {
        parameterForFirstRangeValue = 5.5;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 3) && j == 1) // 1.5 - 2.0 - jt60
      {
        parameterForFirstRangeValue = 2.7;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 3) && j == 2) // 1.5 - 2.0 - jt80
      {
        parameterForFirstRangeValue = 2.9;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 3) && j == 3) // 1.5 - 2.0 - jt140
      {
        parameterForFirstRangeValue = 3.5;
        lastRangeValue = 2000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 3) && j == 4) // 1.5 - 2.0 - jt200
      {
        parameterForFirstRangeValue = 4.0;
        lastRangeValue = 3500;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 3) && j == 5) // 1.5 - 2.0 - jt260
      {
        parameterForFirstRangeValue = 4.0;
        lastRangeValue = 3500;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 3) && j == 6) // 1.5 - 2.0 - jt320
      {
        parameterForFirstRangeValue = 4.5;
        lastRangeValue = 4000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 3) && j == 7) // 1.5 - 2.0 - jt400
      {
        parameterForFirstRangeValue = 4.5;
        lastRangeValue = 6000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 3) && j == 8) // 1.5 - 2.0 - jt450
      {
        parameterForFirstRangeValue = 4.5;
        lastRangeValue = 7000;
        maxEta = atoi(eta[i + 1]);
      }

      //-------------------------------------------------------------------------------------------//
      if ((i == 4) && j == 0) // 2.0 - 2.5 - jt40
      {
        parameterForFirstRangeValue = 4.5;
        lastRangeValue = 1000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 4) && j == 1) // 2.0 - 2.5 - jt60
      {
        parameterForFirstRangeValue = 6;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 4) && j == 2) // 2.0 - 2.5 - jt80
      {
        parameterForFirstRangeValue = 5.5;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 4) && j == 3) // 2.0 - 2.5 - jt140
      {
        parameterForFirstRangeValue = 5.5;
        lastRangeValue = 3000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 4) && j == 4) // 2.0 - 2.5 - jt200
      {
        parameterForFirstRangeValue = 5.5;
        lastRangeValue = 4500;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 4) && j == 5) // 2.0 - 2.5 - jt260
      {
        parameterForFirstRangeValue = 6.0;
        lastRangeValue = 5500;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 4) && j == 6) // 2.0 - 2.5 - jt320
      {
        parameterForFirstRangeValue = 6.5;
        lastRangeValue = 6000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 4) && j == 7) // 2.0 - 2.5 - jt400
      {
        parameterForFirstRangeValue = 6.5;
        lastRangeValue = 8000;
        maxEta = atoi(eta[i + 1]);
      }

      if ((i == 4) && j == 8) // 2.0 - 2.5 - jt450
      {
        parameterForFirstRangeValue = 6.5;
        lastRangeValue = 8000;
        maxEta = atoi(eta[i + 1]);
      }
      //-------------------------------------------------------------------------------------------//

      //* Efficiency hist
      massHist[j]->Divide(refMassHist[j]);

      //* Define with function with several parameters and fit efficiency hist, atoi(jt[j + 1]) -> pt
      fitFunction[j] = defineFitFunction(atoi(jt[j + 1]), parameterForFirstRangeValue, lastRangeValue, maxEta, curveParameter);
      fitFunction[j]->Draw();
      massHist[j]->Fit(fitFunction[j], "R");
      // massHist[j]->Draw();

      //* Get turn-on points and store them in a global variable for later use
      turn_on[i][j] = fitFunction[j]->GetX(0.99);

      if (drawTurnOnHists)
      {
        c1->cd();
        massHist[j]->GetYaxis()->SetRangeUser(0., 1.3);
        massHist[j]->GetXaxis()->SetTitle("M_{ij} (GeV)");
        massHist[j]->GetYaxis()->SetTitle("Efficiency");
        massHist[j]->GetXaxis()->SetTitleOffset(1.2);
        massHist[j]->Draw();

        if (j == 8 || j == 7)
        {
          massHist[j]->GetXaxis()->SetRangeUser(0., 8000.);
        }
        else if (j == 6 || j == 5 || j == 4)
        {
          massHist[j]->GetXaxis()->SetRangeUser(0., 6000.);
        }
        else if (j == 0)
        {
          massHist[j]->GetXaxis()->SetRangeUser(0., 1000.);
        }
        else
        {
          massHist[j]->GetXaxis()->SetRangeUser(0., 4000.);
        }

        //Lines from point to each axis
        TLine *lineHorizontal = new TLine(0., 1., 4000., 1.);
        lineHorizontal->SetLineColor(kBlack);
        lineHorizontal->SetLineStyle(2);
        lineHorizontal->SetLineWidth(1);
        lineHorizontal->Draw();

        TLine *lineVertical = new TLine(fitFunction[j]->GetX(0.99), 0., fitFunction[j]->GetX(0.99), 1.);
        lineVertical->SetLineColor(kBlack);
        lineVertical->SetLineStyle(2);
        lineVertical->SetLineWidth(1);
        lineVertical->Draw();

        TLatex latex;
        latex.SetTextSize(0.025);
        latex.DrawLatex(1320.114, 0.216371, year);
        latex.DrawLatex(1320.114, 0.296371, Form("TurnOn Point = %f", fitFunction[j]->GetX(0.99)));
        latex.DrawLatex(1320.114, 0.371371, Form("Run2, %s" + jt[j + 1], "jt"));
        latex.DrawLatex(1320.114, 0.441533, Form(eta[i] + "%s" + eta[i + 1], " < #||{#eta} < "));

        c1->SaveAs("pngs/" + year + "_eta_" + eta[i] + "-" + eta[i + 1] + "_jt" + jt[j + 1] + "_.png");
      }
    } // end of trgCounter loop
  }   // end of etaCounter loop
}

//* This function will define "fit function" and returns it.
TF1 *defineFitFunction(int jetTrgThreshold, double ParameterForFirstRangeValue, double lastRangeValue, double maxEta, double curveParameter = 2)
{
  TF1 *function;

  //* Defining "error function"
  function = new TF1("", "0.5*(1+TMath::Erf((x-[0])/[1]))", jetTrgThreshold * ParameterForFirstRangeValue, lastRangeValue);

  //* Set error function parameters.
  function->SetParameters(2 * jetTrgThreshold * cosh(maxEta), jetTrgThreshold * curveParameter * 0.1);

  return function;
}

//* This function cuts zero bias hists
void cutZBHists()
{
  TH1 *ZBMassHist[10];
  for (size_t i = 0; i < 5; i++)
  {
    TString pathToHist = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/jt" + jt[0] + "/hdjmass";

    //* Fetch histograms:
    ZBMassHist[i] = getClonedHist(pathToDataFile, pathToHist);

    // ZBMassHist[i]->Draw();

    //* Find maximum bin
    int maxBin = ZBMassHist[i]->GetMaximumBin();

    auto numberOfBins = ZBMassHist[i]->GetNbinsX();

    //* Cut histogram if bin value is greater than jet trigger 40 turn-on
    //* or if a there is any bin lower than maximum bin
    for (size_t k = 1; k <= numberOfBins; k++)
    {
      auto XaxisValue = ZBMassHist[i]->GetBinCenter(k);

      if (XaxisValue > turn_on[i][0] || (k < maxBin))
      {

        ZBMassHist[i]->SetBinContent(k, 0);
        ZBMassHist[i]->SetBinError(k, 0);
      }
    } // end of numberOfBins loop

    //* Define a name and store the hist
    TString histName = "mass_" + eta[i] + "-" + eta[i + 1] + "_jt" + jt[0];
    storeHist(storageFile, ZBMassHist[i], histName);
  } // end of etaCounter loop
}

//* This function cuts jet trigger hists by using turn-on points.
void cutTrgHists()
{
  TH1 *jetTrgMassHist[10];

  for (size_t i = 0; i < 5; i++)
  { //etaCounter - 1
    for (size_t j = 0; j < trgCounter - 1; j++)
    { // trgCounter - 1

      TString PathToJetTrgMassHist = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/jt" + jt[j + 1] + "/hdjmass";

      //* Fetch histograms:
      jetTrgMassHist[j] = getClonedHist(pathToDataFile, PathToJetTrgMassHist);

      auto numberOfBins = jetTrgMassHist[j]->GetNbinsX();

      for (size_t k = 1; k <= numberOfBins; k++)
      {
        auto XaxisValue = jetTrgMassHist[j]->GetBinCenter(k);

        //* Cut left side and right side of hist bz using turn-on points.
        if (XaxisValue < turn_on[i][j] || (j < 8 && XaxisValue >= turn_on[i][j + 1]))
        {
          jetTrgMassHist[j]->SetBinContent(k, 0);
          jetTrgMassHist[j]->SetBinError(k, 0);
        }

      } // end of numberOfBins loop

      //* Define a name and store the hist
      TString histName = "mass_" + eta[i] + "-" + eta[i + 1] + "_jt" + jt[j + 1];

      storeHist(storageFile, jetTrgMassHist[j], histName);

      // c2->cd();
      // jetTrgMassHist[j]->Draw();
      // c2->SaveAs("pngs/mass_" + eta[i] + "-" + eta[i + 1] + "_jt" + jt[j + 1] + "_.png");
    } // end of trgCounter loop
  }   // end of etaCounter loop
}

//* This function opens pre-created root file and write hist in it.
void storeHist(TFile *file, TH1 *massHist, TString histName)
{

  file->ReOpen("UPDATE");
  massHist->Write(histName);
}