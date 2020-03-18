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
TString dataSpectrumFile = "datas/2016/DATA/std_veto_JECunc/output-DATA-Run2016-2b.root";
// TString dataSpectrumFile = "massSpectrums2017_halfbin.root";
// TString dataSpectrumFile = "massSpectrums2018.root";

// Source root for MC
TString mcSpectrumFile = "datas/2016/MC/newXSEC/STD+GEN+JER_INVESTIGATION/NOMINAL/output-MC-2b.root";
// TString mcSpectrumFile = "datas/2017/MC/newXSEC/std+gen+jer/output-MC-2a.root";
// TString mcSpectrumFile = "datas/2018/MC/STD+GEN/output-MC-2a.root";

void drawDataMCPt()
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
  TH1 *mcSpectrum[5];
  TH1 *ratioHist[5];

  for (size_t i = 0; i < 5; i++)
  { //etaCounter - 1

    TString etaName = (TString)(eta[i] + " < |y| < " + eta[i + 1]);

    //* Data spectrum
    TString pathToRefMassHist = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/hdjpt_leading";
    dataSpectrum[i] = getClonedHist(dataSpectrumFile, pathToRefMassHist);

    // MC spectrum
    // TString pathToMCSpectrum = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
    // mcSpectrum[i] = getClonedHist(mcSpectrumFile, pathToMCSpectrum);

    // // MC spectrum
    TString pathToMCSpectrum = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/hdjpt_leading";
    mcSpectrum[i] = getClonedHist(mcSpectrumFile, pathToMCSpectrum);

    auto firstBin = dataSpectrum[i]->FindFirstBinAbove();
    auto lastBin = dataSpectrum[i]->FindLastBinAbove();

    auto scale = dataSpectrum[i]->Integral(firstBin, lastBin) / mcSpectrum[i]->Integral(firstBin, lastBin);
    mcSpectrum[i]->Scale(scale);

    cout << scale << endl;

    ratioHist[i] = (TH1 *)mcSpectrum[i]->Clone("ratioHist");

    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0.015); // Upper and lower plot are joined
    pad1->SetLogx();
    pad1->SetLogy();               // Vertical grid
    pad1->Draw();                  // Draw the upper pad: pad1
    pad1->cd();                    // pad1 becomes the current pad
    mcSpectrum[i]->SetStats(0);    // No statistics on upper plot
    mcSpectrum[i]->Draw();         // Draw mcSpectrum[i]
    dataSpectrum[i]->Draw("same"); // Draw mcSpectrum[i] on top of mcSpectrum[i]

    // Create legend and set style
    TLegend *leg = new TLegend(0.73, 0.64, 0.85, 0.87);
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);

    leg->AddEntry(dataSpectrum[i], "DATA (" + year + ")", "lp");
    leg->AddEntry(mcSpectrum[i], "MC (" + tune + ")", "l");
    //leg->AddEntry("", "", "");
    leg->AddEntry("", "Anti-k_{T} R=0.4", "");
    leg->AddEntry("", "PF+CHS", "");
    leg->AddEntry("", etaName, "");

    leg->Draw();

    //* Draw latex onto pre-drawn hist
    t1->DrawLatex(0.1, 0.92, latexLeft);
    t2->DrawLatex(0.78, 0.92, latexRight);

    c1->cd(); // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0.015);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridy();
    pad2->SetLogx();
    pad2->Draw();
    pad2->cd(); // pad2 becomes the current pad

    ratioHist[i]->SetMinimum(0.5); // Define Y ..
    ratioHist[i]->SetMaximum(1.4); // .. range
    ratioHist[i]->Sumw2();
    ratioHist[i]->SetStats(0); // No statistics on lower plot
    ratioHist[i]->Divide(dataSpectrum[i]);
    ratioHist[i]->Draw("ep"); // Draw the ratio plot

    mcSpectrum[i]->SetLineColor(kRed + 1);
    mcSpectrum[i]->SetLineWidth(2);
    mcSpectrum[i]->SetMarkerColor(kRed);

    // Y axis mcSpectrum[i] plot settings
    mcSpectrum[i]->GetYaxis()->SetTitle("A.U.");
    mcSpectrum[i]->GetYaxis()->SetTitleSize(25);
    mcSpectrum[i]->GetYaxis()->SetTitleFont(43);
    mcSpectrum[i]->GetYaxis()->SetTitleOffset(1.55);
    mcSpectrum[i]->GetYaxis()->SetLabelFont(43);
    mcSpectrum[i]->GetYaxis()->SetLabelSize(20);

    // X axis mcSpectrum[i] plot settings
    mcSpectrum[i]->GetXaxis()->SetRange(firstBin, lastBin);
    mcSpectrum[i]->GetXaxis()->SetLabelSize(0.);

    // mcSpectrum[i] settings
    mcSpectrum[i]->SetLineColor(kRed);
    //mcSpectrum[i]->SetLineWidth(2);

    // dataSpectrum[i] settings
    dataSpectrum[i]->SetLineColor(kBlack);
    //dataSpectrum[i]->SetLineWidth(2);
    dataSpectrum[i]->SetMarkerColor(kBlack);
    dataSpectrum[i]->SetMarkerStyle(kFullSquare);

    // Ratio plot (ratioHist[i]) settings
    ratioHist[i]->SetTitle(""); // Remove the ratio title
    ratioHist[i]->SetLineColor(kBlack);
    //ratioHist[i]->SetLineWidth(2);
    ratioHist[i]->SetMarkerStyle(kFullSquare);
    ratioHist[i]->SetMarkerColor(kBlack);

    // Y axis ratio plot settings
    ratioHist[i]->GetYaxis()->SetTitle("MC/DATA");
    ratioHist[i]->GetYaxis()->CenterTitle();
    ratioHist[i]->GetYaxis()->SetNdivisions(505);
    ratioHist[i]->GetYaxis()->SetTitleSize(25);
    ratioHist[i]->GetYaxis()->SetTitleFont(43);
    ratioHist[i]->GetYaxis()->SetTitleOffset(1.55);
    ratioHist[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratioHist[i]->GetYaxis()->SetLabelSize(20);

    // X axis ratio plot settings
    ratioHist[i]->GetXaxis()->SetTitle("p_{T} (GeV)");
    ratioHist[i]->GetXaxis()->SetTitleSize(25);
    ratioHist[i]->GetXaxis()->SetTitleFont(43);
    ratioHist[i]->GetXaxis()->SetTitleOffset(3.95);
    ratioHist[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratioHist[i]->GetXaxis()->SetLabelSize(20);
    ratioHist[i]->GetXaxis()->SetMoreLogLabels();
    ratioHist[i]->GetXaxis()->SetNoExponent();
    ratioHist[i]->GetXaxis()->SetRangeUser(mcSpectrum[i]->GetBinCenter(firstBin), mcSpectrum[i]->GetBinCenter(lastBin));

    gPad->RedrawAxis();
    c1->SaveAs("pngs/" + year + "_hdjpt_leading" + eta[i] + "-" + eta[i + 1] + ".png");

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
