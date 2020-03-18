#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

void unfold_MC()
{

    TString eta[] = {
        "0.0",
        "0.5",
        "1.0",
        "1.5",
        "2.0",
        "2.5"};

    TString x[] = {"0", "1", "2", "3", "4"};
    int bin_counter = (sizeof(eta) / sizeof(*eta));

    Double_t w = 1500;
    Double_t h = 1000;

    TCanvas *c1 = new TCanvas("c1", "c1", w, h);
    c1->SetLogx();
    c1->SetLogy();
    // c1->SetTopMargin(0.055);
    // c1->SetBottomMargin(2);
    // c1->SetLeftMargin(0.055);
    // c1->SetRightMargin(0.018);

    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    TLegend *leg = new TLegend(0.80, 0.76, 0.93, 0.92, NULL, "brNDC");
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
    leg->SetTextSize(0.045);

    TLatex *t1 = new TLatex();
    t1->SetNDC();
    t1->SetTextFont(42);
    t1->SetTextSize(0.04);
    t1->SetTextAlign(11);

    TFile *Unfolded_file = new TFile("NoReg_0.0_yMC/unfoldedSpectra-Mjj-Tau_0.0.root");
    TFile *Gen_file = new TFile("output-MC-1.root");

    ///// Histos booking //////

    TH1D *Unfolded_level[5];
    TH1D *Gen_level[5];
    TH1D *Ratio;

    for (int i = 0; i < bin_counter - 1; i++)
    { //

        Gen_level[i] = (TH1D *)Gen_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i + 1] + "/mc" + "/hdjmass_half_gen");
        Unfolded_level[i] = (TH1D *)Unfolded_file->Get("HistoOutput" + x[i] + "bin");

        c1->cd();

        TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
        pad1->SetBottomMargin(0.012);
        pad1->Draw();
        pad1->SetLogy();
        pad1->SetLogx();
        pad1->SetTopMargin(0.055);
        pad1->SetBottomMargin(0.011);
        pad1->SetLeftMargin(0.066);
        pad1->SetRightMargin(0.018);
        pad1->cd();

        // Y axis mcSpectrum[i] plot settings
        Gen_level[i]->GetYaxis()->SetTitle("A.U.");
        Gen_level[i]->GetYaxis()->SetTitleSize(30);
        Gen_level[i]->GetYaxis()->SetTitleFont(43);
        Gen_level[i]->GetYaxis()->SetTitleOffset(1.15);
        Gen_level[i]->GetYaxis()->SetLabelFont(43);
        Gen_level[i]->GetYaxis()->SetLabelSize(30);

        // X axis mcSpectrum[i] plot settings
        Gen_level[i]->GetXaxis()->SetLabelSize(0.);
        Gen_level[i]->GetXaxis()->SetRange(3, Gen_level[i]->GetNbinsX());

        Gen_level[i]->SetLineColor(kBlue);
        Gen_level[i]->SetLineWidth(2);

        Gen_level[i]->Draw("e1");

        Unfolded_level[i]->GetXaxis()->SetRange(3, Gen_level[i]->GetNbinsX());
        Unfolded_level[i]->SetLineColor(2);
        Unfolded_level[i]->SetLineWidth(2);
        Unfolded_level[i]->Draw("e1 same");

        leg->AddEntry(Gen_level[i], "Gen", "l");
        leg->AddEntry(Unfolded_level[i], "Unfolded", "l");
        leg->AddEntry("", eta[i] + " < y_{max} < " + eta[i + 1], "");
        leg->Draw();

        // TLatex lt;
        // lt.SetNDC();
        // lt.SetTextSize(0.03);
        // lt.DrawLatex(0.77, 0.72, eta[i] + " < #eta < " + eta[i + 1]);
        // lt.SetTextAlign(12);

        t1->SetTextSize(0.035);
        t1->DrawLatex(0.055, 0.96, "#bf{CMS} #font[12]{Work in Progress 2016}");

        //* Create another latex and set style
        TLatex *t2 = new TLatex();
        t2->SetNDC();
        t2->SetTextFont(42);
        t2->SetTextSize(0.04);
        t2->SetTextAlign(11);

        t2->DrawLatex(0.840, 0.96, "P8 Tune M1 (2016)");

        Ratio = (TH1D *)Gen_level[i]->Clone("Ratio");

        pad1->RedrawAxis();

        c1->cd();

        TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
        pad2->SetTopMargin(0.012);
        pad2->SetLogx();
        pad2->SetGridy();
        // pad2->SetTopMargin(0.055);
        pad2->SetBottomMargin(0.28);
        pad2->SetLeftMargin(0.066);
        pad2->SetRightMargin(0.018);
        pad2->Draw();
        pad2->cd(); // pad2 becomes the current pad

        Ratio->Divide(Unfolded_level[i]);

        //Cosmetics
        Ratio->SetMinimum(0.5); // Define Y ..
        Ratio->SetMaximum(1.4); // .. range
        Ratio->SetStats(0);     // No statistics on lower plot

        Ratio->SetMarkerColor(kBlue);
        Ratio->SetLineColor(kBlue);
        Ratio->SetLineWidth(2);

        Ratio->GetYaxis()->SetTitle("Gen / Unfolded ");
        Ratio->GetYaxis()->CenterTitle();
        Ratio->GetYaxis()->SetNdivisions(505);
        Ratio->GetYaxis()->SetTitleSize(30);
        Ratio->GetYaxis()->SetTitleFont(43);
        Ratio->GetYaxis()->SetTitleOffset(1.15);
        Ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        Ratio->GetYaxis()->SetLabelSize(30);

        Ratio->GetXaxis()->SetTitle("M_{jj} (GeV)");
        Ratio->GetXaxis()->SetTitleSize(30);
        Ratio->GetXaxis()->SetTitleFont(43);
        Ratio->GetXaxis()->SetTitleOffset(4.3);
        Ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        Ratio->GetXaxis()->SetLabelSize(30);
        // Ratio->GetXaxis()->SetMoreLogLabels();
        // Ratio->GetXaxis()->SetNoExponent();
        Ratio->GetXaxis()->SetRange(3, Ratio->GetNbinsX());

        Ratio->Draw("E1");
        pad2->RedrawAxis();
        // gPad->RedrawAxis("g");

        c1->SaveAs("MC_unfolding_" + eta[i] + "-" + eta[i + 1] + ".png");
        leg->Clear();
        c1->Update();

    } //i

} //main
