#include <iostream>
using namespace std;

//* ROOT header files
#include <TH1.h>
#include <TF1.h>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TRatioPlot.h>
#include <TPad.h>
#include <TGaxis.h>

//* Fuction prototype
TH1 *getClonedHist(TString pathToFile, TString pathToHist);
void storeHist(TFile *file, TH1 *massHist, TString histName);

TString eta[] = {"0.0", "0.5", "1.0", "1.5", "2.0", "2.5"};
TString x[] = {"0", "1", "2", "3", "4"};

// Size of the eta:
constexpr size_t etaCounter = (sizeof(eta) / sizeof(*eta));

//* Canvas width and height
constexpr double w = 1400.;
constexpr double h = 250.;

// Choose the correct information settings for latex depending on the year:
// TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2016}";
// TString latexRight = "35.9 fb^{-1} (13 TeV)";
// TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2017}";
// TString latexRight = "41.5 fb^{-1} (13 TeV)";
// TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2018}";
// TString latexRight = "59.7 fb^{-1} (13 TeV)";

// Chose the year you're running
TString year = "2016";
// TString year = "2017";
// TString year = "2018";

// Choose UE Tune for MC
TString tune = "P8M1";
// TString tune = "P8CP5";

// Fetch histograms from this file:
TString unfoldedRoot = "Meeting_unfolding/DATA_unfold_2016/NoReg_0.0_yDATA/unfoldedSpectra-Mjj-Tau_0.0.root";
// TString unfoldedRoot = "Meeting_unfolding/test/DATA_unfold_2017/NoReg_0.0_yDATA/unfoldedSpectra-Mjj-Tau_0.0.root";

//Gen spectrum file
TString genFile = "Meeting_unfolding/DATA_unfold_2016/output-MC-2a.root";
// TString genFile = "Meeting_unfolding/test/DATA_unfold_2017/output-MC-2a.root";

void unfolded_onlyRatios()
{

  //* Create a latex and set style
  TLatex *t1 = new TLatex();
  t1->SetNDC();
  t1->SetTextFont(42);
  t1->SetTextSize(0.10);
  t1->SetTextAlign(11);

  //* Create another latex and set style
  TLatex *t2 = new TLatex();
  t2->SetNDC();
  t2->SetTextFont(42);
  t2->SetTextSize(0.08);
  t2->SetTextAlign(11);

  //* Disable stat box for all histograms
  gStyle->SetOptStat(0);

  //* Create canvas and set style
  TCanvas *c1 = new TCanvas("c1", "c1", w, h);

  TH1 *unfoldedSpectrum[5];
  TH1 *genSpectrum[5];
  TH1 *ratioHist[5];

  int ListMarkerStyle[] = {20, 21, 22, 23, 29};

  for (size_t i = 0; i < 5; i++)
  { //etaCounter - 1

    TString etaName = (TString)(eta[i] + " < |y| < " + eta[i + 1]);

    //* Unfolded histograms
    TString UnfoldedHistPath = "HistoOutput" + x[i] + "bin";
    unfoldedSpectrum[i] = getClonedHist(unfoldedRoot, UnfoldedHistPath);

    //* MC histograms
    TString pathToGenHists = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/mc/hdjmass_half_gen";
    genSpectrum[i] = getClonedHist(genFile, pathToGenHists);

    auto firstBin = unfoldedSpectrum[i]->FindFirstBinAbove();
    auto lastBin = unfoldedSpectrum[i]->FindLastBinAbove();

    // // 2017
    // auto scale = unfoldedSpectrum[i]->Integral(2, lastBin) / genSpectrum[i]->Integral(2, lastBin);
    // genSpectrum[i]->Scale(scale);

    // 2016
    auto scale = unfoldedSpectrum[i]->Integral(4, lastBin) / genSpectrum[i]->Integral(4, lastBin);
    genSpectrum[i]->Scale(scale);

    // cout << scale << endl;

    ratioHist[i] = (TH1 *)genSpectrum[i]->Clone("ratioHist");

    c1->cd(); // Go back to the main canvas before defining pad2
    // TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    c1->SetTopMargin(0.15);
    c1->SetBottomMargin(0.2);
    c1->SetLeftMargin(0.055);
    c1->SetRightMargin(0.005);
    c1->SetGridy();
    c1->SetLogx();
    c1->Draw();
    c1->cd(); // pad2 becomes the current pad

    ratioHist[i]->SetMinimum(0.5); // Define Y ..
    ratioHist[i]->SetMaximum(1.4); // .. range
    ratioHist[i]->Sumw2();
    ratioHist[i]->SetStats(0); // No statistics on lower plot

    ratioHist[i]->Divide(unfoldedSpectrum[i]);

    ratioHist[i]->Draw(); // Draw the ratio plot

    gPad->RedrawAxis("g");

    // Ratio plot (ratioHist[i]) settings
    ratioHist[i]->SetTitle(""); // Remove the ratio title

    // Y axis ratio plot settings
    ratioHist[i]->GetYaxis()->SetTitle("Gen/Unfolded");
    ratioHist[i]->GetYaxis()->CenterTitle();
    ratioHist[i]->GetYaxis()->SetNdivisions(505);
    ratioHist[i]->GetYaxis()->SetTitleSize(25);
    ratioHist[i]->GetYaxis()->SetTitleFont(43);
    ratioHist[i]->GetYaxis()->SetTitleOffset(0.25);
    ratioHist[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratioHist[i]->GetYaxis()->SetLabelSize(20);

    // X axis ratio plot settings
    ratioHist[i]->GetXaxis()->SetTitle("M_{jj} (GeV)");
    ratioHist[i]->GetXaxis()->SetTitleSize(25);
    ratioHist[i]->GetXaxis()->SetTitleFont(43);
    // ratioHist[i]->GetXaxis()->SetNdivisions(505);
    ratioHist[i]->GetXaxis()->SetTitleOffset(0.87);
    ratioHist[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratioHist[i]->GetXaxis()->SetLabelSize(20);
    ratioHist[i]->GetXaxis()->SetMoreLogLabels();
    ratioHist[i]->GetXaxis()->SetNoExponent();
    ratioHist[i]->GetXaxis()->SetRange(3, ratioHist[i]->GetNbinsX());

    ratioHist[i]->SetLineColor(kBlack);
    //ratioHist[i]->SetLineWidth(2);
    ratioHist[i]->SetMarkerStyle(ListMarkerStyle[i]);
    ratioHist[i]->SetMarkerColor(kBlack);

    // Set size for markers
    if (i == 0)
      ratioHist[i]->SetMarkerSize(1.2);
    if (i == 1)
      ratioHist[i]->SetMarkerSize(1.2);
    if (i == 2)
      ratioHist[i]->SetMarkerSize(1.4);
    if (i == 3)
      ratioHist[i]->SetMarkerSize(1.5);
    if (i == 4)
      ratioHist[i]->SetMarkerSize(2.0);

    //* Draw latex onto pre-drawn hist
    t1->DrawLatex(0.904, 0.88, eta[i] + " < |y| < " + eta[i + 1]);
    // t2->DrawLatex(0.78, 0.82, latexRight);

    gPad->RedrawAxis("g");
    c1->SaveAs("pngs/" + year + "_deneme_unfolded_" + eta[i] + "-" + eta[i + 1] + ".png");

    // c1->Delete();
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
