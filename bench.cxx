//
// Created by Filippo Valle on 03/10/2018.
//

#include <TH1D.h>
#include <TMultiGraph.h>
#include <TLegend.h>

void bench(){
    int max = 30;

    auto leg = new TLegend();
    auto icc = new TH1D("icc", "icc", 10, 0, max);
    icc->Fill(9.44);
    icc->Fill(8.11);
    icc->Fill(8.02);
    icc->Fill(7.99);
    icc->Fill(10.79);
    icc->SetLineColor(kGreen);
    leg->AddEntry(icc, "icc");

    auto clangompO3 = new TH1D("clangompO3", "clangompO3", 10, 0, max);
    clangompO3->Fill(7.00325);
    clangompO3->Fill(12.65);
    clangompO3->Fill(11.67);
    clangompO3->Fill(8.37);
    clangompO3->Fill(12.011);
    clangompO3->SetLineColor(kBlue);
    leg->AddEntry(clangompO3, "clangomp -O3");

    auto clangomp = new TH1D("clangomp", "clangomp", 10, 0, max);
    clangomp->Fill(22.12);
    clangomp->Fill(26.31);
    clangomp->Fill(44.50);
    clangomp->Fill(28.32);
    clangomp->Fill(34.01);
    clangomp->SetLineColor(kRed);
    leg->AddEntry(clangomp, "clangomp");

    auto root = new TH1D("root", "root", 10, 0, max);
    root->Fill(17.21);
    root->Fill(26.17);
    root->Fill(19.02);
    root->Fill(20.03);
    root->Fill(22.28);
    root->SetLineColor(kCyan);
    leg->AddEntry(root, "root");

    icc->Draw();
    root->Draw("SAME");
    clangomp->Draw("SAME");
    clangompO3->Draw("SAME");
    leg->Draw("same");
}