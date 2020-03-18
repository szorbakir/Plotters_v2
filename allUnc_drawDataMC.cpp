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
constexpr double w = 1600.;
constexpr double h = 1000.;

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
TString mcSpectrumFile = "datas/2016/MC/newXSEC/cleaned/nominal/output-MC-2a.root";
TString mcSpectrumFile_up = "datas/2016/MC/newXSEC/cleaned/up/output-MC-2a.root";
TString mcSpectrumFile_down = "datas/2016/MC/newXSEC/cleaned/down/output-MC-2a.rootM";
// TString mcSpectrumFile = "datas/2017/MC/STD+GEN/output-MC-2a.root";
// TString mcSpectrumFile = "datas/2018/MC/STD+GEN/output-MC-2a.root";

//* Store hist in this file
TFile *storageFile = new TFile("uncHists.root", "RECREATE");

void allUnc_drawDataMC()
{

  //* Create a latex and set style
  TLatex *t1 = new TLatex();
  t1->SetNDC();
  t1->SetTextFont(42);
  t1->SetTextSize(0.04);
  t1->SetTextAlign(11);

  //* Create another latex and set style
  TLatex *t2 = new TLatex();
  t2->SetNDC();
  t2->SetTextFont(42);
  t2->SetTextSize(0.04);
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

    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0.015); // Upper and lower plot are joined
    pad1->SetLogx();
    pad1->SetLogy();
    pad1->SetFillColor(BackgroundColor); // Vertical grid
    pad1->Draw();                        // Draw the upper pad: pad1
    pad1->cd();                          // pad1 becomes the current pad
    totalUnc_up[i]->SetStats(0);         // No statistics on upper plot

    totalUnc_down[i]->SetFillColor(BackgroundColor);
    totalUnc_up[i]->SetFillColor(kYellow);

    totalUnc_down[i]->SetLineColor(kWhite);
    totalUnc_down[i]->SetMarkerColor(kWhite);

    totalUnc_up[i]->SetLineColor(kWhite);
    totalUnc_up[i]->SetMarkerColor(kWhite);

    totalUnc_up[i]->Draw("hist ");
    totalUnc_down[i]->Draw("same hist ");
    mcSpectrum[i]->Draw("same");   // Draw mcSpectrum[i]
    dataSpectrum[i]->Draw("same"); // Draw mcSpectrum[i] on top of mcSpectrum[i]

    // Create legend and set style
    TLegend *leg = new TLegend(0.70, 0.60, 0.90, 0.87);
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);

    leg->AddEntry(dataSpectrum[i], "DATA (" + year + ")", "lp");
    leg->AddEntry(mcSpectrum[i], "MC (" + tune + ")", "l");
    leg->AddEntry(totalUnc_up[i], "JEC+JER+Lumi Unc.", "lf");
    //leg->AddEntry("", "", "");
    leg->AddEntry("", "Anti-k_{T} R=0.4", "");
    leg->AddEntry("", "PF+CHS", "");
    leg->AddEntry("", etaName, "");

    leg->Draw();

    //* Draw latex onto pre-drawn hist
    t1->DrawLatex(0.1, 0.92, latexLeft);
    t2->DrawLatex(0.78, 0.92, latexRight);

    // Y axis mcSpectrum[i] plot settings
    totalUnc_up[i]->GetYaxis()->SetTitle("A.U.");
    totalUnc_up[i]->GetYaxis()->SetTitleSize(25);
    totalUnc_up[i]->GetYaxis()->SetTitleFont(43);
    totalUnc_up[i]->GetYaxis()->SetTitleOffset(1.55);
    totalUnc_up[i]->GetYaxis()->SetLabelFont(43);
    totalUnc_up[i]->GetYaxis()->SetLabelSize(20);

    // X axis mcSpectrum[i] plot settings
    totalUnc_up[i]->GetXaxis()->SetRange(firstBin, lastBin);
    totalUnc_up[i]->GetXaxis()->SetLabelSize(0.);

    // dataSpectrum[i] settings
    dataSpectrum[i]->SetLineColor(kBlack);
    dataSpectrum[i]->SetMarkerColor(kBlack);
    dataSpectrum[i]->SetMarkerStyle(kFullSquare);

    // mcSpectrum[i] settings
    mcSpectrum[i]->SetLineColor(kRed + 1);
    mcSpectrum[i]->SetLineWidth(2);
    mcSpectrum[i]->SetMarkerColor(kRed);

    pad1->RedrawAxis();

    // ------------------------------------------------------------------------

    c1->cd(); // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0.015);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridy();
    pad2->SetLogx();
    pad2->Draw();
    pad2->cd(); // pad2 becomes the current pad

    ratioHist_down[i]->SetMinimum(0.5); // Define Y ..
    ratioHist_down[i]->SetMaximum(1.4); // .. range
    ratioHist_down[i]->Sumw2();
    ratioHist_down[i]->SetStats(0); // No statistics on lower plot

    ratioHist[i]->Divide(dataSpectrum[i]);
    ratioHist_up[i]->Divide(totalUnc_up[i]);
    ratioHist_down[i]->Divide(totalUnc_down[i]);

    ratioHist_up[i]->SetFillColor(BackgroundColor);
    ratioHist_down[i]->SetFillColor(kYellow);

    ratioHist_down[i]->Draw("hist ");    // Draw the ratio plot
    ratioHist_up[i]->Draw("same hist "); // Draw the ratio plot
    ratioHist[i]->Draw("same ep");       // Draw the ratio plot

    // Ratio plot (ratioHist[i]) settings
    ratioHist_down[i]->SetTitle(""); // Remove the ratio title

    // Y axis ratio plot settings
    ratioHist_down[i]->GetYaxis()->SetTitle("MC/DATA ");
    ratioHist_down[i]->GetYaxis()->CenterTitle();
    ratioHist_down[i]->GetYaxis()->SetNdivisions(505);
    ratioHist_down[i]->GetYaxis()->SetTitleSize(25);
    ratioHist_down[i]->GetYaxis()->SetTitleFont(43);
    ratioHist_down[i]->GetYaxis()->SetTitleOffset(1.55);
    ratioHist_down[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratioHist_down[i]->GetYaxis()->SetLabelSize(20);

    // X axis ratio plot settings
    ratioHist_down[i]->GetXaxis()->SetTitle("M_{jj} (GeV)");
    ratioHist_down[i]->GetXaxis()->SetTitleSize(25);
    ratioHist_down[i]->GetXaxis()->SetTitleFont(43);
    ratioHist_down[i]->GetXaxis()->SetTitleOffset(4.);
    ratioHist_down[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratioHist_down[i]->GetXaxis()->SetLabelSize(20);
    ratioHist_down[i]->GetXaxis()->SetMoreLogLabels();
    ratioHist_down[i]->GetXaxis()->SetNoExponent();
    ratioHist_down[i]->GetXaxis()->SetRangeUser(mcSpectrum[i]->GetBinCenter(firstBin), mcSpectrum[i]->GetBinCenter(lastBin));

    ratioHist[i]->SetLineColor(kBlack);
    //ratioHist[i]->SetLineWidth(2);
    ratioHist[i]->SetMarkerStyle(kFullSquare);
    ratioHist[i]->SetMarkerColor(kBlack);

    gPad->RedrawAxis("g");
    c1->SaveAs("pngs/" + year + "_massratio_" + eta[i] + "-" + eta[i + 1] + ".png");

    leg->Delete();
    pad1->Delete();
    pad2->Delete();
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
