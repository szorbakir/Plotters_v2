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
TString x[] = {"0", "1", "2", "3", "4"};

constexpr size_t trgCounter = (sizeof(jt) / sizeof(*jt));
constexpr size_t etaCounter = (sizeof(eta) / sizeof(*eta));

//* Canvas width and height
constexpr double w = 1500.;
constexpr double h = 1000.;

TH1 *jetTrgMassHist[10];
TH1 *unfoldedHist[10];
TH1 *genHists[10];

// TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2016}";
// TString latexRight = "35.9 fb^{-1} (13 TeV)";
TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2017}";
TString latexRight = "41.5 fb^{-1} (13 TeV)";
// TString latexLeft = "#bf{CMS} #font[12]{Work in Progress - 2018}";
// TString latexRight = "59.7 fb^{-1} (13 TeV)";

// Summary hist will be saved with this name:
// TString storedHistoName = "pngs/2016_unfolded_summary.png";
TString storedHistoName = "pngs/2017_unfolded_summary.png";

// Gen spectrum file
// TString genFile = "Meeting_unfolding/DATA_unfold_2016/output-MC-2a.root";
TString genFile = "Meeting_unfolding/test/DATA_unfold_2017/output-MC-2a.root";

// Fetch histograms from this file:
// TString unfoldedRoot = "Meeting_unfolding/DATA_unfold_2016/NoReg_0.0_yDATA/unfoldedSpectra-Mjj-Tau_0.0.root";
TString unfoldedRoot = "Meeting_unfolding/test/DATA_unfold_2017/NoReg_0.0_yDATA/unfoldedSpectra-Mjj-Tau_0.0.root";

// TString tune = "Pythia8 M1";
TString tune = "Pythia8 Tune CP5";

void unfolded_drawDataSummary()
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
  c1->SetRightMargin(0.005);

  //* Colors and marker styles
  int ListColors[] = {840, 632, 814, 800, 416, 600, 28, 616, 401};
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
  TLegend *leg = new TLegend(0.63, 0.83, 0.75, 0.93);
  leg->SetBorderSize(0);
  leg->SetLineColor(0);
  leg->SetLineStyle(1);
  leg->SetLineWidth(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->SetTextSize(0.025);

  //* Create another legend and set style
  TLegend *leg2 = new TLegend(0.79, 0.65, 0.91, 0.92);
  leg2->SetBorderSize(0);
  leg2->SetLineColor(0);
  leg2->SetLineStyle(1);
  leg2->SetLineWidth(0);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(1001);
  leg2->SetTextSize(0.025);

  for (size_t i = 0; i < etaCounter - 1; i++)
  { //etaCounter - 1

    //For scaling MC
    double integralUnfolded = 0.;
    double integralGen = 0.;

    //* Unfolded histograms
    TString UnfoldedHistPath = "HistoOutput" + x[i] + "bin";
    unfoldedHist[i] = getClonedHist(unfoldedRoot, UnfoldedHistPath);

    // 2016
    integralUnfolded = integralUnfolded + unfoldedHist[i]->Integral(4, unfoldedHist[i]->GetNbinsX());

    // 2017
    // integralUnfolded = integralUnfolded + unfoldedHist[i]->Integral(2, unfoldedHist[i]->GetNbinsX());

    //* MC histograms
    TString pathToGenHists = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/mc/hdjmass_half_gen";

    //* Fetch MC histograms:
    genHists[i] = getClonedHist(genFile, pathToGenHists);

    //* Scale each eta region for displying properly all spectrums in one canvas
    if (i == 0)
      unfoldedHist[i]->Scale(1e8);
    if (i == 1)
      unfoldedHist[i]->Scale(1e6);
    if (i == 2)
      unfoldedHist[i]->Scale(1e4);
    if (i == 3)
      unfoldedHist[i]->Scale(1e2);
    if (i == 4)
      unfoldedHist[i]->Scale(1e0);

    c1->cd();

    genHists[i]->SetMarkerColor(kRed);
    genHists[i]->SetLineColor(kRed);
    // genHists[i]->GetXaxis()->SetRange(3, genHists[i]->GetNbinsX());

    //* All hist will be drawn onto Zero Bias histogram in first eta region by using "same" option
    if (i == 0)
    {
      unfoldedHist[i]->Draw();
      //* Settin some styles

      unfoldedHist[i]->SetMinimum(1e-4);
      unfoldedHist[i]->SetMaximum(1e20);
      // unfoldedHist[i]->GetXaxis()->SetRange(3, unfoldedHist[i]->GetNbinsX());
      //* Adding hist info to the legend
      leg->AddEntry(unfoldedHist[i], "Unfolded Spectrum", "l");
      leg->AddEntry(genHists[0], tune, "l");
    }
    else
    {
      unfoldedHist[i]->Draw("same");
      // unfoldedHist[i]->GetXaxis()->SetRange(3, unfoldedHist[i]->GetNbinsX());
    }

    // Set size for markers
    if (i == 0)
      unfoldedHist[i]->SetMarkerSize(1.2);
    if (i == 1)
      unfoldedHist[i]->SetMarkerSize(1.2);
    if (i == 2)
      unfoldedHist[i]->SetMarkerSize(1.4);
    if (i == 3)
      unfoldedHist[i]->SetMarkerSize(1.5);
    if (i == 4)
      unfoldedHist[i]->SetMarkerSize(1.8);

    // Normalization
    // 2016
    integralGen = genHists[i]->Integral(4, genHists[i]->GetNbinsX());
    // 2017
    // integralGen = genHists[i]->Integral(4, genHists[i]->GetNbinsX());
    genHists[i]->Scale(integralUnfolded / integralGen);

    if (i == 0)
      genHists[i]->Scale(1e8);
    if (i == 1)
      genHists[i]->Scale(1e6);
    if (i == 2)
      genHists[i]->Scale(1e4);
    if (i == 3)
      genHists[i]->Scale(1e2);
    if (i == 4)
      genHists[i]->Scale(1e0);

    genHists[i]->SetMarkerColor(kRed);
    genHists[i]->SetLineColor(kRed);

    genHists[i]->Draw("same");

    //* Set some styles
    unfoldedHist[i]->SetTitle("");
    unfoldedHist[i]->SetMarkerColor(baseColor);
    unfoldedHist[i]->SetLineColor(baseColor);
    unfoldedHist[i]->SetMarkerStyle(ListMarkerStyle[i]);
    unfoldedHist[i]->GetXaxis()->SetTitle("M_{jj} (GeV)");
    unfoldedHist[i]->GetYaxis()->SetTitle("A.U.");
    unfoldedHist[i]->GetYaxis()->SetTitle("A.U.");
    unfoldedHist[i]->GetXaxis()->SetTitleOffset(1.1);
    unfoldedHist[i]->GetYaxis()->SetTitleOffset(1.1);
  }

  //* Adding one time infos to the legend
  leg2->AddEntry(unfoldedHist[0], "0.0 < #||{y} < 0.5 (#times 10^{8})", "lp");
  leg2->AddEntry(unfoldedHist[1], "0.5 < #||{y} < 1.0 (#times 10^{6})", "lp");
  leg2->AddEntry(unfoldedHist[2], "1.0 < #||{y} < 1.5 (#times 10^{4})", "lp");
  leg2->AddEntry(unfoldedHist[3], "1.5 < #||{y} < 2.0 (#times 10^{2})", "lp");
  leg2->AddEntry(unfoldedHist[4], "2.0 < #||{y} < 2.5 (#times 10^{0})", "lp");
  // leg2->AddEntry("", "", "");
  leg2->AddEntry("", "Anti-k_{T} R=0.4", "");
  leg2->AddEntry("", "PF+CHS", "");
  leg2->AddEntry("", "Unfolded Level", "");

  //* Draw legends onto pre-drawn hist
  leg2->Draw();
  leg->Draw();

  //* Draw latex onto pre-drawn hist
  t1->DrawLatex(0.075, 0.96, latexLeft);
  t2->DrawLatex(0.860, 0.96, latexRight);

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
