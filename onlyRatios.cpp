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

// Size of the eta:
constexpr size_t etaCounter = (sizeof(eta) / sizeof(*eta));

//* Canvas width and height
constexpr double w = 1400.;
constexpr double h = 250.;

// Choose the correct information settings for latex depending on the year:
TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2016}";
TString latexRight = "35.9 fb^{-1} (13 TeV)";
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

// Source root for DATA
TString dataSpectrumFile = "massSpectrums2016.root";
TString dataSpectrumFile_up = "massSpectrums2016_up.root";
TString dataSpectrumFile_down = "massSpectrums2016_down.root";
// TString dataSpectrumFile = "massSpectrums2017.root";
// TString dataSpectrumFile = "massSpectrums2018.root";

// Source root for MC
TString mcSpectrumFile = "datas/2016/MC/newXSEC/STD+GEN+JER_INVESTIGATION/NOMINAL/output-MC-2a.root";
TString mcSpectrumFile_up = "datas/2016/MC/newXSEC/STD+GEN+JER_INVESTIGATION/UP_Variation/output-MC-2a.root";
TString mcSpectrumFile_down = "datas/2016/MC/newXSEC/STD+GEN+JER_INVESTIGATION/DOWN_Variation/output-MC-2a.root";
// TString mcSpectrumFile = "datas/2017/MC/STD+GEN/output-MC-2a.root";
// TString mcSpectrumFile = "datas/2018/MC/STD+GEN/output-MC-2a.root";

//* Store hist in this file
TFile *storageFile = new TFile("uncHists.root", "RECREATE");

void onlyRatios()
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

  TH1 *dataSpectrum[5];
  TH1 *dataSpectrum_up[5];
  TH1 *dataSpectrum_down[5];

  TH1 *mcSpectrum[5];
  TH1 *mcSpectrum_up[5];
  TH1 *mcSpectrum_down[5];

  TH1 *ratioHist[5];
  TH1 *ratioHist_up[5];
  TH1 *ratioHist_down[5];

  TH1 *totalUnc_up[5];
  TH1 *totalUnc_down[5];

  Color_t BackgroundColor = 10;
  gStyle->SetCanvasColor(BackgroundColor);
  gStyle->SetTitleFillColor(BackgroundColor);
  gStyle->SetStatColor(BackgroundColor);
  c1->SetFillColor(BackgroundColor);

  int ListMarkerStyle[] = {20, 21, 22, 23, 29};

  for (size_t i = 0; i < 5; i++)
  { //etaCounter - 1

    TString etaName = (TString)(eta[i] + " < |y| < " + eta[i + 1]);

    //* Data spectrum
    TString pathToDataSpectrum = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
    dataSpectrum[i] = getClonedHist(dataSpectrumFile, pathToDataSpectrum);

    //* Data spectrum up
    TString pathToDataSpectrum_up = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
    dataSpectrum_up[i] = getClonedHist(dataSpectrumFile_up, pathToDataSpectrum_up);

    //* Data spectrum down
    TString pathToDataSpectrum_down = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
    dataSpectrum_down[i] = getClonedHist(dataSpectrumFile_down, pathToDataSpectrum_down);

    // MC spectrum
    TString pathToMCSpectrum = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/mc/hdjmass";
    mcSpectrum[i] = getClonedHist(mcSpectrumFile, pathToMCSpectrum);

    // MC spectrum up
    TString pathToMCSpectrum_up = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/mc/hdjmass";
    mcSpectrum_up[i] = getClonedHist(mcSpectrumFile_up, pathToMCSpectrum_up);

    // MC spectrum down
    TString pathToMCSpectrum_down = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/mc/hdjmass";
    mcSpectrum_down[i] = getClonedHist(mcSpectrumFile_down, pathToMCSpectrum_down);

    auto firstBin = dataSpectrum[i]->FindFirstBinAbove();
    auto lastBin = dataSpectrum[i]->FindLastBinAbove();

    auto scale = dataSpectrum[i]->Integral(firstBin, lastBin) / mcSpectrum[i]->Integral(firstBin, lastBin);
    mcSpectrum[i]->Scale(scale);

    auto scale_up = dataSpectrum_up[i]->Integral(firstBin, lastBin) / mcSpectrum_up[i]->Integral(firstBin, lastBin);
    mcSpectrum_up[i]->Scale(scale_up);

    auto scale_down = dataSpectrum_down[i]->Integral(firstBin, lastBin) / mcSpectrum_down[i]->Integral(firstBin, lastBin);
    mcSpectrum_down[i]->Scale(scale_down);

    // cout << scale << endl;
    // cout << scale_down << endl;
    // cout << scale_up << endl;

    // For calculation of total uncertainties
    totalUnc_up[i] = (TH1 *)dataSpectrum[i]->Clone("totalUnc_up");
    totalUnc_down[i] = (TH1 *)dataSpectrum[i]->Clone("totalUnc_down");

    ratioHist[i] = (TH1 *)mcSpectrum[i]->Clone("ratioHist");
    ratioHist_up[i] = (TH1 *)mcSpectrum[i]->Clone("ratioHist_up");
    ratioHist_down[i] = (TH1 *)mcSpectrum[i]->Clone("ratioHist_down");

    // Calculation of total uncertainty band
    auto nBins = dataSpectrum[i]->GetNbinsX();
    // cout << nBins << endl;

    for (size_t k = 0; k <= nBins; k++)
    {
      auto check_bins = dataSpectrum[i]->GetBinContent(k);

      if (check_bins)
      {
        auto data = dataSpectrum[i]->GetBinContent(k);

        // JEC uncertainties
        auto jec_up = abs(dataSpectrum_up[i]->GetBinContent(k) - data);
        auto jec_down = abs(data - dataSpectrum_down[i]->GetBinContent(k));

        auto lumi_unc = 0;
        // Corresponding lumi uncertainties
        if (year == "2016")
        {
          lumi_unc = data * (2.5 / 100);
        }
        else if (year == "2017")
        {
          lumi_unc = data * (2.3 / 100);
        }
        else
        {
          lumi_unc = data * (2.5 / 100);
        }

        // MC uncertainties
        auto mc = mcSpectrum[i]->GetBinContent(k);

        // JER uncertainties
        auto jer_up = abs(mcSpectrum_up[i]->GetBinContent(k) - mc);
        auto jer_down = abs(mc - mcSpectrum_down[i]->GetBinContent(k));

        // Check if jer_up is smaller than jer_down
        if (mcSpectrum_up[i]->GetBinContent(k) <= mcSpectrum_down[i]->GetBinContent(k))
        {
          cout << "JER up: " << mcSpectrum_up[i]->GetBinContent(k) << endl;
          cout << "JER down: " << mcSpectrum_down[i]->GetBinContent(k) << endl;
        }

        auto total_data_unc_up = sqrt(pow(jec_up, 2) + pow(lumi_unc, 2));
        auto total_data_unc_down = sqrt(pow(jec_down, 2) + pow(lumi_unc, 2));

        auto total_unc_up = sqrt(pow(total_data_unc_up, 2) + pow(jer_up, 2));
        auto total_unc_down = sqrt(pow(total_data_unc_down, 2) + pow(jer_down, 2));

        totalUnc_up[i]->SetBinContent(k, data + total_unc_up);
        totalUnc_down[i]->SetBinContent(k, data - total_unc_down);

        // Check if totalUnc_up is smaller than totalUnc_down
        if (totalUnc_up[i]->GetBinContent(k) <= totalUnc_down[i]->GetBinContent(k))
        {
          cout << "Total unc up: " << totalUnc_up[i]->GetBinContent(k) << endl;
          cout << "Total unc down: " << totalUnc_down[i]->GetBinContent(k) << endl;
        }
      }
    }

    //* Define a name and store the hist
    TString histName_up = "totalUnc_up_" + eta[i] + "-" + eta[i + 1];
    TString histName_down = "totalUnc_down_" + eta[i] + "-" + eta[i + 1];

    storeHist(storageFile, totalUnc_up[i], histName_up);
    storeHist(storageFile, totalUnc_down[i], histName_down);

    c1->cd(); // Go back to the main canvas before defining pad2
    // TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    c1->SetTopMargin(0.055);
    c1->SetBottomMargin(0.2);
    c1->SetLeftMargin(0.055);
    c1->SetRightMargin(0.005);
    c1->SetGridy();
    c1->SetLogx();
    c1->Draw();
    c1->cd(); // pad2 becomes the current pad

    ratioHist_down[i]->SetMinimum(0.5); // Define Y ..
    ratioHist_down[i]->SetMaximum(1.4); // .. range
    ratioHist_down[i]->Sumw2();
    ratioHist_down[i]->SetStats(0); // No statistics on lower plot

    ratioHist[i]->Divide(dataSpectrum[i]);
    ratioHist_up[i]->Divide(totalUnc_up[i]);
    ratioHist_down[i]->Divide(totalUnc_down[i]);

    ratioHist_up[i]->SetFillColor(BackgroundColor);
    ratioHist_down[i]->SetFillColor(kYellow);

    ratioHist_up[i]->SetLineColor(kYellow);
    ratioHist_down[i]->SetLineColor(kYellow);

    ratioHist_down[i]->Draw("hist ");    // Draw the ratio plot
    ratioHist_up[i]->Draw("same hist "); // Draw the ratio plot
    ratioHist[i]->Draw("same ep");       // Draw the ratio plot

    gPad->RedrawAxis("g");

    // Ratio plot (ratioHist[i]) settings
    ratioHist_down[i]->SetTitle(""); // Remove the ratio title

    // Y axis ratio plot settings
    ratioHist_down[i]->GetYaxis()->SetTitle("MC/DATA ");
    ratioHist_down[i]->GetYaxis()->CenterTitle();
    ratioHist_down[i]->GetYaxis()->SetNdivisions(505);
    ratioHist_down[i]->GetYaxis()->SetTitleSize(25);
    ratioHist_down[i]->GetYaxis()->SetTitleFont(43);
    ratioHist_down[i]->GetYaxis()->SetTitleOffset(0.25);
    ratioHist_down[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratioHist_down[i]->GetYaxis()->SetLabelSize(20);

    // X axis ratio plot settings
    ratioHist_down[i]->GetXaxis()->SetTitle("M_{jj} (GeV)");
    ratioHist_down[i]->GetXaxis()->SetTitleSize(25);
    ratioHist_down[i]->GetXaxis()->SetTitleFont(43);
    // ratioHist_down[i]->GetXaxis()->SetNdivisions(505);
    ratioHist_down[i]->GetXaxis()->SetTitleOffset(0.87);
    ratioHist_down[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratioHist_down[i]->GetXaxis()->SetLabelSize(20);
    ratioHist_down[i]->GetXaxis()->SetMoreLogLabels();
    ratioHist_down[i]->GetXaxis()->SetNoExponent();
    ratioHist_down[i]->GetXaxis()->SetRangeUser(mcSpectrum[i]->GetBinCenter(firstBin), mcSpectrum[i]->GetBinCenter(lastBin));

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
      ratioHist[i]->SetMarkerSize(1.6);

    //* Draw latex onto pre-drawn hist
    t1->DrawLatex(0.1, 0.82, eta[i] + " < |y| < " + eta[i + 1]);
    // t2->DrawLatex(0.78, 0.82, latexRight);

    gPad->RedrawAxis("g");
    c1->SaveAs("pngs/" + year + "deneme" + eta[i] + "-" + eta[i + 1] + ".png");

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
