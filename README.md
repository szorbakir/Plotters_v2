# Plotters_v2

constructMassSpectrum.cpp —> Add trigger histograms and create Mass Spectrum. (If you want to run this code, first you need to run constructTriggerMassHists.cpp)

constructTriggerMassHists.cpp —> Clean trigger histograms and store them into a root file.

unc_constructMassSpectrum.cpp —> Add trigger histograms and create Mass Spectrum for JEC up and down variations. (If you want to run this code, first you need to run unc_constructTriggerMassHists.cpp)

unc_constructTriggerMassHists.cpp —> Clean trigger histograms or JEC up and down variations and store them into a root file.

If you want to run the plotters below you need create mass spectrums with the scripts above...

allUnc_onlyRatios.cpp —> draw only MC vs DATA ratios with uncertainties.

allUnc_drawDataMC.cpp —> draw MC vs DATA with uncertainties.

allUnc_drawDataSummary.cpp —> draw Summary plot with uncertainties.

drawDataMC.cpp —> Draw MC vs DATA for mass spectrum without uncertainties.

drawDataMCPt.cpp —> Draw MC vs DATA for pt spectrum without uncertainties.

drawDataSummary.cpp —> Draw Summary plot without uncertainties
