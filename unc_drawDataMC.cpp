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
TString dataSpectrumFile = "massSpectrums2016.root";
TString dataSpectrumFile_up = "massSpectrums2016_up.root";
TString dataSpectrumFile_down = "massSpectrums2016_down.root";

// TString dataSpectrumFile = "massSpectrums2017.root";
// TString dataSpectrumFile_up = "massSpectrums2017_up.root";
// TString dataSpectrumFile_down = "massSpectrums2017_down.root";

// TString dataSpectrumFile = "massSpectrums2018.root";
// TString dataSpectrumFile_up = "massSpectrums2018_up.root";
// TString dataSpectrumFile_down = "massSpectrums2018_down.root";

// Source root for MC
TString mcSpectrumFile = "datas/2016/MC/newXSEC/STD+GEN+JER_INVESTIGATION/NOMINAL/output-MC-2a.root";
// TString mcSpectrumFile = "datas/2017/MC/STD+GEN/output-MC-2a.root";
// TString mcSpectrumFile = "datas/2018/MC/STD+GEN/output-MC-2a.root";

void unc_drawDataMC()
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

    Color_t BackgroundColor = 10;
    gStyle->SetCanvasColor(BackgroundColor);
    gStyle->SetTitleFillColor(BackgroundColor);
    gStyle->SetStatColor(BackgroundColor);
    c1->SetFillColor(BackgroundColor);

    for (size_t i = 0; i < 5; i++)
    { //etaCounter - 1

        TString etaName = (TString)(eta[i] + " < |y| < " + eta[i + 1]);

        cout << "norm" << endl;
        //* Data spectrum
        TString pathToDataSpectrum = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
        dataSpectrum[i] = getClonedHist(dataSpectrumFile, pathToDataSpectrum);

        cout << "up" << endl;
        //* Data spectrum up
        TString pathToDataSpectrum_up = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
        dataSpectrum_up[i] = getClonedHist(dataSpectrumFile_up, pathToDataSpectrum_up);

        cout << "down" << endl;
        //* Data spectrum down
        TString pathToDataSpectrum_down = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
        dataSpectrum_down[i] = getClonedHist(dataSpectrumFile_down, pathToDataSpectrum_down);

        cout << "finished" << endl;
        // MC spectrum
        // TString pathToMCSpectrum = "massSpectrum_" + eta[i] + "-" + eta[i + 1];
        // mcSpectrum[i] = getClonedHist(mcSpectrumFile, pathToMCSpectrum);

        // // MC spectrum
        TString pathToMCSpectrum = "Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/mc/hdjmass";
        mcSpectrum[i] = getClonedHist(mcSpectrumFile, pathToMCSpectrum);

        mcSpectrum_up[i] = (TH1 *)mcSpectrum[i]->Clone("mcSpectrum_up");
        mcSpectrum_down[i] = (TH1 *)mcSpectrum[i]->Clone("mcSpectrum_down");

        auto firstBin = dataSpectrum[i]->FindFirstBinAbove();
        auto lastBin = dataSpectrum[i]->FindLastBinAbove();

        auto scale = dataSpectrum[i]->Integral(firstBin, lastBin) / mcSpectrum[i]->Integral(firstBin, lastBin);
        mcSpectrum[i]->Scale(scale);

        auto scale_up = dataSpectrum_up[i]->Integral(firstBin, lastBin) / mcSpectrum_up[i]->Integral(firstBin, lastBin);
        mcSpectrum_up[i]->Scale(scale);

        auto scale_down = dataSpectrum_down[i]->Integral(firstBin, lastBin) / mcSpectrum_down[i]->Integral(firstBin, lastBin);
        mcSpectrum_down[i]->Scale(scale);

        ratioHist[i] = (TH1 *)mcSpectrum[i]->Clone("ratioHist");
        ratioHist_up[i] = (TH1 *)mcSpectrum_up[i]->Clone("ratioHist_up");
        ratioHist_down[i] = (TH1 *)mcSpectrum_down[i]->Clone("ratioHist_down");

        // ratioHist[i]->Divide(dataSpectrum[i]);

        // ratioHist[i]->SetXTitle("M_{ij} (GeV)");
        // ratioHist[i]->SetYTitle("MC/Data");
        // ratioHist[i]->GetYaxis()->CenterTitle();
        // ratioHist[i]->GetYaxis()->SetRangeUser(0.6, 1.3);
        // ratioHist[i]->GetXaxis()->SetMoreLogLabels();
        // ratioHist[i]->GetXaxis()->SetNoExponent();

        // ratioHist[i]->Draw("E1");

        //cout << mcSpectrum[i]->FindFirstBinAbove() << " " << mcSpectrum[i]->FindLastBinAbove() << endl;
        // cout << mcSpectrum[i]->GetBinCenter(mcSpectrum[i]->FindFirstBinAbove()) << " " << mcSpectrum[i]->GetBinCenter(mcSpectrum[i]->FindLastBinAbove()) << endl;

        // Upper plot will be in pad1
        TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
        pad1->SetBottomMargin(0.015); // Upper and lower plot are joined
        pad1->SetLogx();
        pad1->SetLogy();
        pad1->Draw(); // Draw the upper pad: pad1
        pad1->cd();   // pad1 becomes the current pad
        pad1->SetFillColor(BackgroundColor);
        dataSpectrum_up[i]->SetStats(0); // No statistics on upper plot

        dataSpectrum_down[i]->SetFillColor(BackgroundColor);
        dataSpectrum_up[i]->SetFillColor(kYellow);

        dataSpectrum_down[i]->SetLineColor(kWhite);
        dataSpectrum_down[i]->SetMarkerColor(kWhite);

        dataSpectrum_up[i]->SetLineColor(kWhite);
        dataSpectrum_up[i]->SetMarkerColor(kWhite);

        dataSpectrum_up[i]->Draw("hist ");
        dataSpectrum_down[i]->Draw("same hist ");
        mcSpectrum[i]->Draw("same");   // Draw mcSpectrum[i]
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

        mcSpectrum[i]->SetLineColor(kRed + 1);
        mcSpectrum[i]->SetLineWidth(2);
        mcSpectrum[i]->SetMarkerColor(kRed);

        // Y axis mcSpectrum[i] plot settings
        dataSpectrum_up[i]->GetYaxis()->SetTitle("A.U.");
        dataSpectrum_up[i]->GetYaxis()->SetTitleSize(25);
        dataSpectrum_up[i]->GetYaxis()->SetTitleFont(43);
        dataSpectrum_up[i]->GetYaxis()->SetTitleOffset(1.55);
        dataSpectrum_up[i]->GetYaxis()->SetLabelFont(43);
        dataSpectrum_up[i]->GetYaxis()->SetLabelSize(20);

        // X axis mcSpectrum[i] plot settings
        dataSpectrum_up[i]->GetXaxis()->SetRange(firstBin, lastBin);
        dataSpectrum_up[i]->GetXaxis()->SetLabelSize(0.);

        // mcSpectrum[i] settings
        mcSpectrum[i]->SetLineColor(kRed);
        //mcSpectrum[i]->SetLineWidth(2);

        // dataSpectrum[i] settings
        dataSpectrum[i]->SetLineColor(kBlack);
        //dataSpectrum[i]->SetLineWidth(2);
        dataSpectrum[i]->SetMarkerColor(kBlack);
        dataSpectrum[i]->SetMarkerStyle(kFullSquare);

        pad1->RedrawAxis();

        c1->cd(); // Go back to the main canvas before defining pad2
        TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
        pad2->SetTopMargin(0.015);
        pad2->SetBottomMargin(0.2);
        pad2->SetGridy();
        pad2->SetLogx();
        pad2->Draw();
        pad2->SetFillColor(BackgroundColor);
        pad2->cd(); // pad2 becomes the current pad

        ratioHist_down[i]->SetMinimum(0.4); // Define Y ..
        ratioHist_down[i]->SetMaximum(1.7); // .. range
        ratioHist_down[i]->Sumw2();
        ratioHist_down[i]->SetStats(0); // No statistics on lower plot

        ratioHist_down[i]->Divide(dataSpectrum_down[i]);
        ratioHist_up[i]->Divide(dataSpectrum_up[i]);
        ratioHist[i]->Divide(dataSpectrum[i]);

        ratioHist_up[i]->SetFillColor(BackgroundColor);
        ratioHist_down[i]->SetFillColor(kYellow);

        ratioHist_down[i]->Draw("hist ");    // Draw the ratio plot
        ratioHist_up[i]->Draw("same hist "); // Draw the ratio plot
        ratioHist[i]->Draw("same ep");       // Draw the ratio plot

        ratioHist[i]->SetLineColor(kBlack);
        //ratioHist[i]->SetLineWidth(2);
        ratioHist[i]->SetMarkerStyle(kFullSquare);
        ratioHist[i]->SetMarkerColor(kBlack);

        // Ratio plot (ratioHist[i]) settings
        ratioHist_down[i]->SetTitle(""); // Remove the ratio title

        // Y axis ratio plot settings
        ratioHist_down[i]->GetYaxis()->SetTitle("MC/DATA ");
        ratioHist_down[i]->GetYaxis()->CenterTitle();
        ratioHist_down[i]->GetYaxis()->SetNdivisions(209);
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
