#define TrgEvent_cxx
#include "TrgEvent.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>


TrgEvent::TrgEvent(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/Users/danielbrandenburg/bnl/local/data/Run16/vpd_trigger/run17042030.2.dat.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/Users/danielbrandenburg/bnl/local/data/Run16/vpd_trigger/run17042030.2.dat.root");
      }
      f->GetObject("TrgEvent",tree);

   }
   Init(tree);
}

TrgEvent::~TrgEvent()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TrgEvent::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TrgEvent::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void TrgEvent::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("npre", &npre, &b_npre);
   fChain->SetBranchAddress("npost", &npost, &b_npost);
   fChain->SetBranchAddress("nsum", &nsum, &b_nsum);
   fChain->SetBranchAddress("Vpd", Vpd, &b_Vpd);
   fChain->SetBranchAddress("VpdHi", VpdHi, &b_VpdHi);
   fChain->SetBranchAddress("bbqEastVpd", bbqEastVpd, &b_bbqEastVpd);
   fChain->SetBranchAddress("bbqWestVpd", bbqWestVpd, &b_bbqWestVpd);
   fChain->SetBranchAddress("fastBbqEastVpd", fastBbqEastVpd, &b_fastBbqEastVpd);
   fChain->SetBranchAddress("fastBbqWestVpd", fastBbqWestVpd, &b_fastBbqWestVpd);
   fChain->SetBranchAddress("mxqEastVpd", mxqEastVpd, &b_mxqEastVpd);
   fChain->SetBranchAddress("mxqWestVpd", mxqWestVpd, &b_mxqWestVpd);
   fChain->SetBranchAddress("fastMxqEastVpd", fastMxqEastVpd, &b_fastMxqEastVpd);
   fChain->SetBranchAddress("fastMxqWestVpd", fastMxqWestVpd, &b_fastMxqWestVpd);
   fChain->SetBranchAddress("tofMult", tofMult, &b_tofMult);
   fChain->SetBranchAddress("tofTrayMult", tofTrayMult, &b_tofTrayMult);
   fChain->SetBranchAddress("MT001", MT001, &b_MT001);
   fChain->SetBranchAddress("MT002", MT002, &b_MT002);
   fChain->SetBranchAddress("MT003", MT003, &b_MT003);
   fChain->SetBranchAddress("MT004", MT004, &b_MT004);
   fChain->SetBranchAddress("MT005", MT005, &b_MT005);
   fChain->SetBranchAddress("MT006", MT006, &b_MT006);
   fChain->SetBranchAddress("MT007", MT007, &b_MT007);
   fChain->SetBranchAddress("MT008", MT008, &b_MT008);
   fChain->SetBranchAddress("MtdDsm", MtdDsm, &b_MtdDsm);
   fChain->SetBranchAddress("DsmTF201Ch", DsmTF201Ch, &b_DsmTF201Ch);
   fChain->SetBranchAddress("lastDsmBit", lastDsmBit, &b_lastDsmBit);
   Notify();
}

Bool_t TrgEvent::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TrgEvent::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TrgEvent::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void TrgEvent::Loop()
{
//   In a ROOT session, you can do:
//      root> .L TrgEvent.C
//      root> TrgEvent t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}
