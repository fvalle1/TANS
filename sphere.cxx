//
// Created by Filippo Valle on 23/10/2018.
//

#include "TRandomPlus.h"
#include "TH3D.h"
#include "TCanvas.h"

void sphere(){
    auto rnd = new TRandomPlus();
    delete gRandom;
    gRandom = rnd;
    double coords[2];

    auto plot = new TH3D("plot", "Sphere", 100, -1.1, 1.1, 100, -1.1, 1.1, 100, -1.1, 1.1);
    plot->SetMarkerColor(kCyan-3);
    for (int i = 0; i < 100000; ++i) {
        rnd->GetBySphere(coords);
        double theta = coords[0];
        double phi = coords[1];

        double x = TMath::Sin(theta) * TMath::Cos(phi);
        double y = TMath::Sin(theta) * TMath::Sin(phi);
        double z = TMath::Cos(theta);
        plot->Fill(x, y, z);
    }

    new TCanvas("canvas", "sphere", 400, 400);
    plot->Draw();
}