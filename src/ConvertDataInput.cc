/*
 * ConvertDataInput.cc
 *
 *  Created on: Jun 18, 2013
 *      Author: valentinknuenz
 */

#include "../interface/NRQCDglobalfitObject.h"
#include "../src/NRQCDglobalfitObject.cc"
#include "../interface/GlobalVar.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

//rootincludes
#include "TFile.h"
#include "TH1.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TSystem.h"


using namespace NRQCDvars;







void LoadData(Int_t nState, Int_t MeasurementID, Int_t nExp, Char_t *DataID);

int main(int argc, char** argv) {

  	Char_t *DataID = "Default";

  	for( int i=0;i < argc; ++i ) {
		if(std::string(argv[i]).find("DataID") != std::string::npos) {char* DataIDchar = argv[i]; char* DataIDchar2 = strtok (DataIDchar, "="); DataID = DataIDchar2; cout<<"DataID = "<<DataID<<endl;}
	}

	for(int iState=0;iState<NRQCDvars::nStates;iState++){
		for(int iMeasurementID=0;iMeasurementID<NRQCDvars::nMeasurementIDs;iMeasurementID++){
			for(int iExperiment=0;iExperiment<NRQCDvars::nExperiments;iExperiment++){
				cout<<"Load data"<<endl;
				LoadData(iState, iMeasurementID, iExperiment, DataID);
			}
		}
	}


	//LoadData(0, 0, 0, DataID);

	   return 0;

}


void LoadData(Int_t nState, Int_t MeasurementID, Int_t nExp, Char_t *DataID){
//Data yet to be added:
// Cross sections: CMS7/CMS8 UPS_NS
// CMS polarization measurements
// Cross section ratios
// Feed-down fractions

	  printf("loading %s %s measurement from %s\n",NRQCDvars::StateName[nState], NRQCDvars::MeasurementIDName[MeasurementID], NRQCDvars::ExpName[nExp]);




	//'local' variables (Allocations modified by Joao)
	Int_t const nMaxRapBins = 6;
	Int_t *npTBins = newCVector< Int_t > (nMaxRapBins);
	Int_t nRapBins;
	Int_t const kNbPolScenario = 3; //[0]..unpol, [1]...long(HX), [2]...transv(HX)
	Char_t *polSc[kNbPolScenario] = {"unpolarized", "#lambda_{#theta}(HX) = -1", "#lambda_{#theta}(HX) = +1"};
	Float_t *rapMin = newCVector< Float_t > (nMaxRapBins);
	Float_t *rapMax = newCVector< Float_t > (nMaxRapBins);
	Float_t **pTMean = newCMatrix< Float_t > (nMaxRapBins, 100);
	Float_t **pTMin = newCMatrix< Float_t > (nMaxRapBins, 100);
	Float_t **pTMax = newCMatrix< Float_t > (nMaxRapBins, 100);

	TGraphAsymmErrors *gSigma[kNbPolScenario][nMaxRapBins];
	TGraphAsymmErrors *gSigma_syst[kNbPolScenario][nMaxRapBins];
	TGraphAsymmErrors *gSigma_tot[kNbPolScenario][nMaxRapBins];
	TGraphAsymmErrors *gSigma_Mult[kNbPolScenario][nMaxRapBins];
	TGraphAsymmErrors *gSigma_syst_Mult[kNbPolScenario][nMaxRapBins];
	TGraphAsymmErrors *gSigma_tot_Mult[kNbPolScenario][nMaxRapBins];


	bool isMeasurementAvailable=false;







	  //////////////////////////////////////
	  //////   Define Input File   /////////
	  //////////////////////////////////////


	  FILE *fIn;
	  Int_t maxPTPoints[nMaxRapBins];

	  // CrossSection Measurements:::

	 if(MeasurementID==0){ // Modified by Joao
		 switch( nExp ){
		 case NRQCDvars::LHCb:
			 switch( nState ){
			 case NRQCDvars::PSI_1S:
				 fIn = fopen("HEPDATA/CrossSections/LHCb_promptJpsi_EPJC71_2011_1645.txt", "read");
				 nRapBins=5;
				 maxPTPoints[0] = 14; maxPTPoints[1] = 14; maxPTPoints[2] = 14; maxPTPoints[3] = 13; maxPTPoints[4] = 11;
				 npTBins[0] = 14; npTBins[1] = 14; npTBins[2] = 14;
				 npTBins[3] = 13; npTBins[4] = 11;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::PSI_2S:
				 fIn = fopen("HEPDATA/CrossSections/LHCb_promptPsiPrime_EPJC72_2012_2100.txt", "read");
				 nRapBins=1;
				 maxPTPoints[0] = 12;
				 npTBins[0] = 12;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::UPS_1S:
				 fIn = fopen("HEPDATA/CrossSections/LHCb_Ups1S_EPJC72_2012_2025.txt", "read");
				 nRapBins=5;
				 maxPTPoints[0] = 15; maxPTPoints[1] = 15; maxPTPoints[2] = 15; maxPTPoints[3] = 15; maxPTPoints[4] = 15;
				 npTBins[0] = 15; npTBins[1] = 15; npTBins[2] = 15;
				 npTBins[3] = 15; npTBins[4] = 15;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::UPS_2S:
				 fIn = fopen("HEPDATA/CrossSections/LHCb_Ups2S_EPJC72_2012_2025.txt", "read");
				 nRapBins=5;
				 maxPTPoints[0] = 15; maxPTPoints[1] = 15; maxPTPoints[2] = 15; maxPTPoints[3] = 12; maxPTPoints[4] = 10;
				 npTBins[0] = 15; npTBins[1] = 15; npTBins[2] = 15;
				 npTBins[3] = 12; npTBins[4] = 10;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::UPS_3S:
				 fIn = fopen("HEPDATA/CrossSections/LHCb_Ups3S_EPJC72_2012_2025.txt", "read");
				 nRapBins=5;
				 maxPTPoints[0] = 15; maxPTPoints[1] = 15; maxPTPoints[2] = 15; maxPTPoints[3] = 12; maxPTPoints[4] = 9;
				 npTBins[0] = 15; npTBins[1] = 15; npTBins[2] = 15;
				 npTBins[3] = 12; npTBins[4] = 9;
				 isMeasurementAvailable=true;
				 break;
			 default:
				 cerr << "Error: Unknown state for LHCb. Execution stop!" << endl;
				 exit(2);
			 }
			 break;
		 case NRQCDvars::CMS:
			 switch( nState ){
			 case NRQCDvars::PSI_1S:
				 fIn = fopen("HEPDATA/CrossSections/CMS_promptJpsi_JHEP02_2012_011.txt", "read");
				 nRapBins=5;
				 maxPTPoints[0] = 10; maxPTPoints[1] = 6; maxPTPoints[2] = 11; maxPTPoints[3] = 11; maxPTPoints[4] = 6;
				 npTBins[0] = 10; npTBins[1] = 6; npTBins[2] = 11;
				 npTBins[3] = 11; npTBins[4] = 6;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::PSI_2S:
				 fIn = fopen("HEPDATA/CrossSections/CMS_psiPrime_JHEP02_2012_011.txt", "read");
				 nRapBins=3;
				 maxPTPoints[0] = 9; maxPTPoints[1] = 7; maxPTPoints[2] = 7;
				 npTBins[0] = 9; npTBins[1] = 7; npTBins[2] = 7;
				 isMeasurementAvailable=true;
				 break;
			 default:
				 cerr << "Error: Unknown state for CMS. Execution stop!" << endl;
				 exit(2);
			 }
			 break;
		 case NRQCDvars::ATLAS:
			 switch( nState ){
			 case NRQCDvars::PSI_1S:
				 fIn = fopen("HEPDATA/CrossSections/ATLAS_promptJpsi_NPB850_2011_387.txt", "read");
				 nRapBins=4;
				 maxPTPoints[0] = 14; maxPTPoints[1] = 18; maxPTPoints[2] = 19; maxPTPoints[3] = 13;
				 npTBins[0] = 14; npTBins[1] = 18;
				 npTBins[2] = 19; npTBins[3] = 13;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::UPS_1S:
				 fIn = fopen("HEPDATA/CrossSections/ATLAS_Ups1S_arXiv1211_7255.txt", "read");
				 nRapBins=2;
				 maxPTPoints[0] = 50; maxPTPoints[1] = 50;
				 npTBins[0] = 50; npTBins[1] = 50;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::UPS_2S:
				 fIn = fopen("HEPDATA/CrossSections/ATLAS_Ups2S_arXiv1211_7255.txt", "read");
				 nRapBins=2;
				 maxPTPoints[0] = 25; maxPTPoints[1] = 24;
				 npTBins[0] = 25; npTBins[1] = 24;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::UPS_3S:
				 fIn = fopen("HEPDATA/CrossSections/ATLAS_Ups3S_arXiv1211_7255.txt", "read");
				 nRapBins=2;
				 maxPTPoints[0] = 25; maxPTPoints[1] = 24;
				 npTBins[0] = 25; npTBins[1] = 25;
				 isMeasurementAvailable=true;
				 break;
			 default:
				 cerr << "Error: Unknown state for ATLAS. Execution stop!" << endl;
				 exit(2);
			 }
			 break;
		 case NRQCDvars::CDF:
			 switch( nState ){
			 case NRQCDvars::PSI_1S:
				 fIn = fopen("HEPDATA/CrossSections/CDF_promptJPsi_PRD71_2005_032001.txt", "read");
				 nRapBins=1;
				 maxPTPoints[0] = 26;
				 npTBins[0] = 26;
				 isMeasurementAvailable=true;
				 break;
			 case NRQCDvars::PSI_2S:
				 fIn = fopen("HEPDATA/CrossSections/CDF_promptPsiP_PRD80_2009_031103.txt", "read");
				 nRapBins=1;
				 maxPTPoints[0] = 25;
				 npTBins[0] = 25;
				 isMeasurementAvailable=true;
				 break;
			 default:
				 cerr << "Error: Unknown state for CDF. Execution stop!" << endl;
				 exit(2);
			 }
			 break;
		 case NRQCDvars::ALICE:
			 switch( nState ){
			 case NRQCDvars::PSI_1S:
				 fIn = fopen("HEPDATA/CrossSections/ALICE_promptJpsi_JHEP11_2012_065_polUnc.txt", "read");
				 nRapBins=1;
				 maxPTPoints[0] = 4;
				 npTBins[0] = 4;
				 isMeasurementAvailable=true;
				 break;
			 default:
				 cerr << "Error: Unknown state for ALICE. Execution stop!" << endl;
				 exit(2);
			 }
			 break;
	  }

		 isMeasurementAvailable=false;
	  if (isMeasurementAvailable){


	  Char_t line[1000];
	  Char_t name[100];
	  float* pT = newCVector< Float_t > (100);
	  float* pTBinsForHisto = newCVector< Float_t > (100);
	  float** sigma = newCMatrix< Float_t > (kNbPolScenario, 100);
	  float* errX = newCVector< Float_t > (100);
	  float* errX_syst = newCVector< Float_t > (100);
	  float** errStatPos = newCMatrix< Float_t > (kNbPolScenario, 100);
	  float** errSystPos = newCMatrix< Float_t > (kNbPolScenario, 100);
	  float** errSystNeg = newCMatrix< Float_t > (kNbPolScenario, 100);
	  float** errStatNeg = newCMatrix< Float_t > (kNbPolScenario, 100);
	  float* errPolPos = newCVector< Float_t > (100);
	  float* errPolNeg = newCVector< Float_t > (100);
	  float** errTotPos = newCMatrix< Float_t > (kNbPolScenario, 100);
	  float** errTotNeg = newCMatrix< Float_t > (kNbPolScenario, 100);
	  fgets(line, sizeof(line), fIn); //comment
	  fgets(line, sizeof(line), fIn); //comment
	  fgets(line, sizeof(line), fIn); //comment
	  Double_t relChangeNeg, relChangePos, scaleFac = 1.;
	  for(int iRap = 0; iRap < nRapBins; iRap++){

	    for(int iP = 0; iP < maxPTPoints[iRap]; iP++){
	      fgets(line, sizeof(line), fIn);
	      sscanf(line, "%f %f %f %f %f %f %f %f %f %f %f %f",
		     &rapMin[iRap], &rapMax[iRap],
		     &pTMean[iRap][iP], &pTMin[iRap][iP], &pTMax[iRap][iP],
		     &sigma[0][iP], &errStatPos[0][iP], &errStatNeg[0][iP],
		     &errSystPos[0][iP], &errSystNeg[0][iP],
		     &errPolPos[iP], &errPolNeg[iP]);

	      //printf("iRap %d, iP %d, pTMin %1.3f, pTMax %1.3f, sigma %1.3f, polPos %1.3f, polNeg %1.3f\n",
		  //   iRap, iP, pTMin[iRap][iP], pTMax[iRap][iP], sigma[0][iP], errPolPos[iP], errPolNeg[iP]);

	      Double_t deltaRap = 2*fabs(rapMax[iRap] - rapMin[iRap]);
	      Double_t deltaPT = pTMax[iRap][iP] - pTMin[iRap][iP];

	      pT[iP] = pTMin[iRap][iP] + 0.5*(pTMax[iRap][iP] - pTMin[iRap][iP]); //use the bin centre, since we will be fitting with a histogram!
	      pTBinsForHisto[iP] = pTMin[iRap][iP];

	      errStatNeg[0][iP] = fabs(errStatNeg[0][iP]);
	      errSystNeg[0][iP] = fabs(errSystNeg[0][iP]);

	      if(nState == NRQCDvars::PSI_1S){
		if(nExp == NRQCDvars::LHCb || nExp == NRQCDvars::ALICE)
		  scaleFac = 1 / 0.0593; //BR used by LHCb
		else if(nExp == NRQCDvars::CDF)
		  scaleFac = 2*0.6; //correct for the rap-interval
	      }
	      else if(nState == NRQCDvars::PSI_2S){
		if(nExp == NRQCDvars::CMS)
		  scaleFac = 0.0077; //to compare with LHCb we need to correct for the BR
		else if(nExp == NRQCDvars::LHCb)
		  scaleFac = (4.5 - 2.); //data not normalized to deltaY
		if(nExp == NRQCDvars::CDF)
		  scaleFac = 0.0077 * 2*0.6 * 1000; //correct for mumu BR (for comparison to LHCb) and the rap-interval
	      }
	      else if(nState == NRQCDvars::UPS_1S || nState == NRQCDvars::UPS_2S || nState == NRQCDvars::UPS_3S){
		if(nExp == NRQCDvars::CMS) //normalize to the bin-width (pT and y)
		  scaleFac = (deltaRap * deltaPT);
		else if(nExp == NRQCDvars::LHCb)
		  scaleFac = 1000.; //pb --> nb
		else if(nExp == NRQCDvars::ATLAS)
		  scaleFac = 1e6; //fb --> nb
	      }
	      sigma[0][iP] /= scaleFac;
	      errStatPos[0][iP] /= scaleFac;
	      errSystPos[0][iP] /= scaleFac;
	      errPolPos[iP] /= scaleFac;
	      errStatNeg[0][iP] /= scaleFac;
	      errSystNeg[0][iP] /= scaleFac;
	      errPolNeg[iP] /= scaleFac;

	      //printf("sigma after scaling up = %1.3e\n",  sigma[0][iP]);
	      // errX[iP] = 0.5; errX_syst[iP] = 0.5;
	      errX[iP] = 0.0; errX_syst[iP] = 0.1;
	      errTotPos[0][iP] = sqrt(pow(errStatPos[0][iP],2) + pow(errSystPos[0][iP],2));
	      errTotNeg[0][iP] = sqrt(pow(errStatNeg[0][iP],2) + pow(errSystNeg[0][iP],2));

	      //prepare the other polarization scenarios:
	      if(nState == NRQCDvars::PSI_1S || nState == NRQCDvars::PSI_2S || nExp == NRQCDvars::LHCb || nExp == NRQCDvars::ATLAS || nExp == NRQCDvars::CDF){//gives absolute uncertainties
		sigma[1][iP] = sigma[0][iP] - fabs(errPolNeg[iP]); //Long(HX)
		sigma[2][iP] = sigma[0][iP] + fabs(errPolPos[iP]); //Transv(HX)
		relChangeNeg = sigma[1][iP] / sigma[0][iP];//Long
		relChangePos = sigma[2][iP] / sigma[0][iP];//Trans
		// printf("long. pol leads to a %1.3f changed polarization, while transv. pol leads to a %1.3f changed polarization\n",
		//        relChangeNeg, relChangePos);
	      }
	      else if((nState == NRQCDvars::UPS_1S || nState ==  NRQCDvars::UPS_2S || nState == NRQCDvars::UPS_3S) && (nExp == NRQCDvars::CMS)){//gives relative uncertainties
		relChangePos = errPolPos[iP] * 0.01 + 1.;//Trans; transform the rel uncertainty [%] into a scaling factor
		relChangeNeg = errPolNeg[iP] * 0.01 + 1.;//Long; transform the rel uncertainty [%] into a scaling factor

		sigma[2][iP] = sigma[0][iP] * relChangePos; //Transv(HX)
		sigma[1][iP] = sigma[0][iP] * relChangeNeg; //Long(HX)

		// printf("transv. pol leads to a %1.3f changed polarization, while long. pol leads to a %1.3f changed polarization\n",
		//        relChangePos, relChangeNeg);
	      }
	      else{
		printf("no instructions how to propagate the pol. uncertainties! exiting\n");
		exit(0);
	      }
	      errStatPos[1][iP] = errStatPos[0][iP] * relChangeNeg;
	      errStatNeg[1][iP] = errStatNeg[0][iP] * relChangeNeg;
	      errSystPos[1][iP] = errSystPos[0][iP] * relChangeNeg;
	      errSystNeg[1][iP] = errSystNeg[0][iP] * relChangeNeg;
	      errStatPos[2][iP] = errStatPos[0][iP] * relChangePos;
	      errStatNeg[2][iP] = errStatNeg[0][iP] * relChangePos;
	      errSystPos[2][iP] = errSystPos[0][iP] * relChangePos;
	      errSystNeg[2][iP] = errSystNeg[0][iP] * relChangePos;
	      for(int iPol = 1; iPol < kNbPolScenario; iPol++){
		errTotPos[iPol][iP] = sqrt(errStatPos[iPol][iP]*errStatPos[iPol][iP] + errSystPos[iPol][iP]*errSystPos[1][iP]);
		errTotNeg[iPol][iP] = sqrt(errStatNeg[iPol][iP]*errStatNeg[iPol][iP] + errSystNeg[iPol][iP]*errSystNeg[1][iP]);
	      }

	      // printf("exp %s, rap %d, pT %d: sigma(long_HX) = %1.3e, statXsyst_pos = %1.3e, statXsyst_neg = %1.3e\n",
	      // 	     ExpName[nExp], iRap, iP, sigma[1][iP], errTotPos[1][iP], errTotNeg[1][iP]);

	    }//loop over all pT bins inside a given rapidity bin


	    fgets(line, sizeof(line), fIn); //empty line
	    pTBinsForHisto[maxPTPoints[iRap]] = pTMax[iRap][maxPTPoints[iRap]-1];


	    for(int iPol = 0; iPol < kNbPolScenario; iPol++){
	      gSigma[iPol][iRap] = new TGraphAsymmErrors(maxPTPoints[iRap], pTMean[iRap], sigma[iPol], errX, errX, errStatNeg[iPol], errStatPos[iPol]);
	      gSigma_syst[iPol][iRap] = new TGraphAsymmErrors(maxPTPoints[iRap], pTMean[iRap], sigma[iPol], errX_syst, errX_syst, errSystNeg[iPol], errSystPos[iPol]);
	      gSigma_tot[iPol][iRap] = new TGraphAsymmErrors(maxPTPoints[iRap], pTMean[iRap], sigma[iPol], errX_syst, errX_syst, errTotNeg[iPol], errTotPos[iPol]);
	      gSigma_Mult[iPol][iRap] = new TGraphAsymmErrors(maxPTPoints[iRap], pTMean[iRap], sigma[iPol], errX, errX, errStatNeg[iPol], errStatPos[iPol]);
	      gSigma_syst_Mult[iPol][iRap] = new TGraphAsymmErrors(maxPTPoints[iRap], pTMean[iRap], sigma[iPol], errX_syst, errX_syst, errSystNeg[iPol], errSystNeg[iPol]);
	      gSigma_tot_Mult[iPol][iRap] = new TGraphAsymmErrors(maxPTPoints[iRap], pTMean[iRap], sigma[iPol], errX_syst, errX_syst, errTotNeg[iPol], errTotPos[iPol]);
	    }//polarization

	  }//loop over rapidity bins

	  }
	 }


	 /// Add here the code for all other MeasurementIDs




	 if(isMeasurementAvailable){//Fill NRQCDglobalfitObjects

	  for(int iRap = 0; iRap < nRapBins; iRap++){

	    for(int iP = 0; iP < maxPTPoints[iRap]; iP++){

	    	//setData() here

	    	NRQCDglobalfitObject *DataObject = new NRQCDglobalfitObject();

	    	double Bufferx;
	    	double Buffer_CentralValue;
	    	double Buffer_LongHX;
	    	double Buffer_TransHX;
	    	gSigma[0][iRap]->GetPoint(iP, Bufferx, Buffer_CentralValue);
	    	gSigma[1][iRap]->GetPoint(iP, Bufferx, Buffer_LongHX);
	    	gSigma[2][iRap]->GetPoint(iP, Bufferx, Buffer_TransHX);

	    	if(NRQCDvars::debug) cout<<"setData variables"<<endl;


	    	int setData_nState=nState;
	    	int setData_nStateRatioDenom=999;
	    	int setData_nExp=nExp;
	    	double setData_CentralValue=Buffer_CentralValue;
	    	double setData_ErrStatPos=gSigma[0][iRap]->GetErrorYhigh(iP);
	    	double setData_ErrStatNeg=gSigma[0][iRap]->GetErrorYlow(iP);
	    	double setData_ErrSystPos=gSigma_syst[0][iRap]->GetErrorYhigh(iP);
	    	double setData_ErrSystNeg=gSigma_syst[0][iRap]->GetErrorYlow(iP);
	    	double setData_ErrTotPos=gSigma_tot[0][iRap]->GetErrorYhigh(iP);
	    	double setData_ErrTotNeg=gSigma_tot[0][iRap]->GetErrorYlow(iP);
	    	double setData_ErrGlobalPos=999;
	    	double setData_ErrGlobalNeg=999;
	    	double setData_yMin=rapMin[iRap];
	    	double setData_yMax=rapMax[iRap];
	    	double setData_yMean=(rapMin[iRap]+rapMax[iRap])/2.;
	    	double setData_pTMin=pTMin[iRap][iP];
	    	double setData_pTMax=pTMax[iRap][iP];
	    	double setData_pTMean=pTMean[iRap][iP];
	    	vector<double> setData_PolCorrParams (2); setData_PolCorrParams[0]=Buffer_LongHX; setData_PolCorrParams[1]=Buffer_TransHX;
	    	int setData_MeasurementID=MeasurementID;
	    	string setData_ObjectID="teststring";
	    	bool setData_isDataValid=true;

	    	if(NRQCDvars::debug) cout<<"setData"<<endl;
	    	DataObject->setData(setData_nState, setData_nStateRatioDenom, setData_CentralValue,  setData_ErrStatPos, setData_ErrStatNeg, setData_ErrSystPos,
	    			  setData_ErrSystNeg, setData_ErrTotPos, setData_ErrTotNeg, setData_ErrGlobalPos, setData_ErrGlobalNeg,
	    			  setData_yMin, setData_yMax, setData_yMean, setData_pTMin, setData_pTMax, setData_pTMean,
	    			  setData_PolCorrParams, setData_MeasurementID, setData_nExp, setData_ObjectID, setData_isDataValid);

	    	DataObject->setInvalidModel(nStates, NRQCDvars::nColorChannels, NRQCDvars::nModelSystematicScales);


	    	if(NRQCDvars::debug)DataObject->Dump(nStates, true, false);



//	    	if(iRap==0&&iP==0){




	    char outname[200];
	    char inname[200];
	    char predirname[200];
	    char dirname[200];
	    sprintf(predirname,"DataID");
	    gSystem->mkdir(predirname);
	    sprintf(dirname,"%s/%s",predirname,DataID);
	    gSystem->mkdir(dirname);
	    sprintf(outname,"%s/ConvertedData_%s_%s_%s_rap%d_pT%d.txt",dirname, StateName[nState],  MeasurementIDName[MeasurementID],  ExpName[nExp], iRap, iP);


	    ofstream out;
	    out.open(outname);//, std::ofstream::app);

    	out << *DataObject;

	    out.close();


//	    	}

	    }

	  }



	 }


	 else cout<<"--->  Measurement not (yet) available"<<endl;


}