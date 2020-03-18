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

//* Fuction prototype
TH1 *getClonedHist(TString pathToFile, TString pathToHist);

TString jt[] = {"0", "40", "60", "80", "140", "200", "260", "320", "400", "450"};
TString eta[] = {"0.0", "0.5", "1.0", "1.5", "2.0", "2.5"};

constexpr size_t trgCounter = (sizeof(jt) / sizeof(*jt));
constexpr size_t etaCounter = (sizeof(eta) / sizeof(*eta));

//* Canvas width and height
constexpr double w = 1500.;
constexpr double h = 1000.;

TH1 *jetTrgMassHist[10];
TH1 *baseMassHist[10];
TH1 *MCHists[10];

TH1 *totalUnc_up[10];
TH1 *totalUnc_down[10];

// TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2016}";
// TString latexRight = "35.9 fb^{-1} (13 TeV)";
TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2017}";
TString latexRight = "41.5 fb^{-1} (13 TeV)";
// TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2018}";
// TString latexRight = "59.7 fb^{-1} (13 TeV)";

//Summary hist will be saved with this name:
// TString storedHistoName = "pngs/2016_summary.png";
TString storedHistoName = "pngs/2017_summary.png";
// TString storedHistoName = "pngs/2018_summary.png";

//MC spectrum file
// TString MCFile = "datas/2016/MC/newXSEC/STD+GEN+JER_INVESTIGATION/NOMINAL/output-MC-2a.root";
TString MCFile = "datas/2017/MC/newXSEC/STD+GEN+JER_INVESTIGATION/NOMINAL/output-MC-2a.root";
// TString MCFile = "datas/2018/MC/STD+GEN/output-MC-2a.root";

//Up and down histogram file
TString totalUncFile = "uncHists.root";

// Fetch histograms from this file:
// TString sourceRoot = "triggerMassHists2016.root";
TString sourceRoot = "triggerMassHists2017.root";
// TString sourceRoot = "triggerMassHists2018.root";

// TString tune = "Pythia8 Tune M1";
TString tune = "Pythia8 Tune CP5";

void allUnc_drawDataSummary()
{
  //* Disable stat box for all histograms
  gStyle->SetOptStat(0);

  //* Create canvas and set style
  TCanvas *c1 = new TCanvas("c1", "c1", w, h);
  c1->SetLogx();
  c1->SetLogy();
  c1->SetTickx(1);
  c1->SetTicky(1);
  c1->SetTopMargin(0.055);
  c1->SetBottomMargin(0.099);
  c1->SetLeftMargin(0.077);
  c1->SetRightMargin(0.018);

  //* Colors and marker styles
  int ListColors[] = {840, 632, 401, 800, 416, 600, 28, 616, 814};
  //int ListColors[] = {2, 3, 4, 41, 6, 7, 28, 30, 9};
  int ListMarkerStyle[] = {20, 21, 22, 23, 29};
  int baseColor = 1;        // Black
  int baseMarkerStyle = 20; // Circle

  //* Create a latex and set style
  TLatex *t1 = new TLatex();
  t1->SetNDC();
  t1->SetTextFont(42);
  t1->SetTextSize(0.03);
  t1->SetTextAlign(11);

  //* Create another latex and set style
  TLatex *t2 = new TLatex();
  t2->SetNDC();
  t2->SetTextFont(42);
  t2->SetTextSize(0.03);
  t2->SetTextAlign(11);

  //* Create legend and set style
  TLegend *leg = new TLegend(0.65, 0.68, 0.75, 0.92);
  leg->SetBorderSize(0);
  leg->SetLineColor(0);
  leg->SetLineStyle(1);
  leg->SetLineWidth(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->SetTextSize(0.02);

  //* Create another legend and set style
  TLegend *leg2 = new TLegend(0.77, 0.63, 0.89, 0.92);
  leg2->SetBorderSize(0);
  leg2->SetLineColor(0);
  leg2->SetLineStyle(1);
  leg2->SetLineWidth(0);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(1001);
  leg2->SetTextSize(0.025);

  Color_t BackgroundColor = 10;
  gStyle->SetCanvasColor(BackgroundColor);
  gStyle->SetTitleFillColor(BackgroundColor);
  gStyle->SetStatColor(BackgroundColor);
  c1->SetFillColor(BackgroundColor);

  for (size_t i = 0; i < etaCounter - 1; i++)
  { //etaCounter - 1

    //For scaling MC
    double integralDATA = 0.;
    double integralMC = 0.;

    //* Zero Bias histograms
    TString baseMassHistPath = "mass_" + eta[i] + "-" + eta[i + 1] + "_jt" + jt[0];

    //* Fetch histograms:
    baseMassHist[i] = getClonedHist(sourceRoot, baseMassHistPath);

    integralDATA = integralDATA + baseMassHist[i]->Integral();

    //* Scale each eta region for displying properly all spectrums in one canvas
    if (i == 0)
      baseMassHist[i]->Scale(1e8);
    if (i == 1)
      baseMassHist[i]->Scale(1e6);
    if (i == 2)
      baseMassHist[i]->Scale(1e4);
    if (i == 3)
      baseMassHist[i]->Scale(1e2);
    if (i == 4)
      baseMassHist[i]->Scale(1e0);

    //* Up and down histograms
    TString pathToUpUncHists = "totalUnc_up_" + eta[i] + "-" + eta[i + 1];
    TString pathToDownUncHists = "totalUnc_down_" + eta[i] + "-" + eta[i + 1];

    //* Fetch Up and down histograms
    totalUnc_up[i] = getClonedHist(totalUncFile, pathToUpUncHists);
    totalUnc_down[i] = getClonedHist(totalUncFile, pathToDownUncHists);

    if (i == 0)
    {
      totalUnc_up[i]->Scale(1e8);
      totalUnc_down[i]->Scale(1e8);
    }
    if (i == 1)
    {
      totalUnc_up[i]->Scale(1e6);
      totalUnc_down[i]->Scale(1e6);
    }
    if (i == 2)
    {
      totalUnc_up[i]->Scale(1e4);
      totalUnc_down[i]->Scale(1e4);
    }
    if (i == 3)
    {
      totalUnc_up[i]->Scale(1e2);
      totalUnc_down[i]->Scale(1e2);
    }
    if (i == 4)
    {
      totalUnc_up[i]->Scale(1e0);
      totalUnc_down[i]->Scale(1e0);
    }

    c1->cd();

    //* All hist will be drawn onto Zero Bias histogram in first eta region by using "same" option
    if (i == 0)
    {

      totalUnc_up[i]->Draw("hist");
      totalUnc_down[i]->Draw("same hist");

      baseMassHist[i]->Draw("same");
      //* Settin some styles
      totalUnc_up[i]->SetMinimum(1e-4);
      totalUnc_up[i]->SetMaximum(1e20);
      //* Adding hist info to the legend
      leg->AddEntry(baseMassHist[i], "Zero Bias", "p");
    }
    else
    {
      totalUnc_up[i]->Draw("same hist");
      totalUnc_down[i]->Draw("same hist");
      baseMassHist[i]->Draw("same");
    }

    totalUnc_down[i]->SetFillColor(BackgroundColor);
    totalUnc_up[i]->SetFillColor(kYellow);
    totalUnc_up[i]->SetLineColor(kYellow);

    totalUnc_down[i]->SetLineColor(kWhite);
    totalUnc_down[i]->SetMarkerColor(kWhite);

    // Set size for markers
    if (i == 0)
      baseMassHist[i]->SetMarkerSize(1.1);
    if (i == 1)
      baseMassHist[i]->SetMarkerSize(1.1);
    if (i == 2)
      baseMassHist[i]->SetMarkerSize(1.3);
    if (i == 3)
      baseMassHist[i]->SetMarkerSize(1.4);
    if (i == 4)
      baseMassHist[i]->SetMarkerSize(1.5);

    for (size_t j = 0; j < trgCounter - 1; j++)
    { // trgCounter - 1

      //* Create TString variables for fetching all jet trigger hists
      TString jetTrgMassHistPath = "mass_" + eta[i] + "-" + eta[i + 1] + "_jt" + jt[j + 1];

      //* Fetch hists:
      jetTrgMassHist[j] = getClonedHist(sourceRoot, jetTrgMassHistPath);

      integralDATA = integralDATA + jetTrgMassHist[j]->Integral();

      //* Scale each eta region for displying properly all spectrums in one canvas
      if (i == 0)
        jetTrgMassHist[j]->Scale(1e8);
      if (i == 1)
        jetTrgMassHist[j]->Scale(1e6);
      if (i == 2)
        jetTrgMassHist[j]->Scale(1e4);
      if (i == 3)
        jetTrgMassHist[j]->Scale(1e2);
      if (i == 4)
        jetTrgMassHist[j]->Scale(1e0);

      //* Draw jet histograms onto each eta region's Zero Bias hist
      jetTrgMassHist[j]->Draw("same");

      //* Set some styles
      jetTrgMassHist[j]->SetMarkerColor(ListColors[j]);
      jetTrgMassHist[j]->SetLineColor(ListColors[j]);
      jetTrgMassHist[j]->SetMarkerStyle(ListMarkerStyle[i]);

      // Set size for markers
      if (i == 0)
        jetTrgMassHist[j]->SetMarkerSize(1.2);
      if (i == 1)
        jetTrgMassHist[j]->SetMarkerSize(1.2);
      if (i == 2)
        jetTrgMassHist[j]->SetMarkerSize(1.4);
      if (i == 3)
        jetTrgMassHist[j]->SetMarkerSize(1.5);
      if (i == 4)
        jetTrgMassHist[j]->SetMarkerSize(2.0);

      //* Adding hist info to the legend. Condition is used to prevent multiple adding proccess
      //* for jet trigger hists
      //leg->AddEntry(baseMassHist[i], "Zero Bias", "p");
      if (i == 0)
      {
        leg->AddEntry(jetTrgMassHist[j], "HLT_PFJet" + jt[j + 1], "p");
      }
    }

    //* MC histograms
    TString pathToMCHists = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/mc/hdjmass";

    //* Fetch MC histograms:
    MCHists[i] = getClonedHist(MCFile, pathToMCHists);

    integralMC = MCHists[i]->Integral();
    MCHists[i]->Scale(integralDATA / integralMC);

    if (i == 0)
    {
      MCHists[i]->Scale(1e8);
    }
    if (i == 1)
    {
      MCHists[i]->Scale(1e6);
    }
    if (i == 2)
    {
      MCHists[i]->Scale(1e4);
    }
    if (i == 3)
    {
      MCHists[i]->Scale(1e2);
    }
    if (i == 4)
    {
      MCHists[i]->Scale(1e0);
    }

    MCHists[i]->SetMarkerColor(kRed);
    MCHists[i]->SetLineColor(kRed);

    MCHists[i]->Draw(" same");

    //* Set some styles
    baseMassHist[i]->SetMarkerColor(baseColor);
    baseMassHist[i]->SetLineColor(baseColor);
    baseMassHist[i]->SetMarkerStyle(ListMarkerStyle[i]);

    totalUnc_up[i]->GetXaxis()->SetTitle("M_{jj} (GeV)");
    totalUnc_up[i]->GetYaxis()->SetTitle("A.U.");
    totalUnc_up[i]->GetYaxis()->SetTitle("A.U.");
    totalUnc_up[i]->GetXaxis()->SetTitleOffset(1.1);
    totalUnc_up[i]->GetYaxis()->SetTitleOffset(1.1);
  }

  //* Adding one time infos to the legend
  leg2->AddEntry(baseMassHist[0], "0.0 < #||{y} < 0.5 (#times 10^{8})", "lp");
  leg2->AddEntry(baseMassHist[1], "0.5 < #||{y} < 1.0 (#times 10^{6})", "lp");
  leg2->AddEntry(baseMassHist[2], "1.0 < #||{y} < 1.5 (#times 10^{4})", "lp");
  leg2->AddEntry(baseMassHist[3], "1.5 < #||{y} < 2.0 (#times 10^{2})", "lp");
  leg2->AddEntry(baseMassHist[4], "2.0 < #||{y} < 2.5 (#times 10^{0})", "lp");
  leg2->AddEntry(MCHists[0], tune, "l");
  leg2->AddEntry(totalUnc_up[0], "JEC+JER+Lumi Unc.", "f");
  leg2->AddEntry("", "", "");
  leg2->AddEntry("", "Anti-k_{T} R=0.4", "");
  leg2->AddEntry("", "PF+CHS", "");
  leg2->AddEntry("", "Detector Level", "");

  //* Draw legends onto pre-drawn hist
  leg2->Draw();
  leg->Draw();

  //* Draw latex onto pre-drawn hist
  t1->DrawLatex(0.075, 0.96, latexLeft);
  t2->DrawLatex(0.850, 0.96, latexRight);

  gPad->RedrawAxis();

  c1->SaveAs(storedHistoName);
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
