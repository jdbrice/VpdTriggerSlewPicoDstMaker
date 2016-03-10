#define vpdPico_cxx
#include "vpdPico.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void VpdPico::Loop()
{
//   In a ROOT session, you can do:
//      root> .L vpdPico.C
//      root> vpdPico t
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


VpdPico::VpdPico(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ntuple_574.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ntuple_574.root");
      }
      f->GetObject("VpdPico",tree);

   }
   Init(tree);
}

VpdPico::~VpdPico()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t VpdPico::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t VpdPico::LoadTree(Long64_t entry)
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

void VpdPico::Init(TTree *tree)
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

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_event_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_event_fBits);
   fChain->SetBranchAddress("triggerIds", &triggerIds, &b_event_triggerIds);
   fChain->SetBranchAddress("mxqTdcEast[16]", mxqTdcEast, &b_event_mxqTdcEast);
   fChain->SetBranchAddress("mxqTdcWest[16]", mxqTdcWest, &b_event_mxqTdcWest);
   fChain->SetBranchAddress("mxqAdcEast[16]", mxqAdcEast, &b_event_mxqAdcEast);
   fChain->SetBranchAddress("mxqAdcWest[16]", mxqAdcWest, &b_event_mxqAdcWest);
   fChain->SetBranchAddress("bbqTdcEast[16]", bbqTdcEast, &b_event_bbqTdcEast);
   fChain->SetBranchAddress("bbqTdcWest[16]", bbqTdcWest, &b_event_bbqTdcWest);
   fChain->SetBranchAddress("bbqAdcEast[16]", bbqAdcEast, &b_event_bbqAdcEast);
   fChain->SetBranchAddress("bbqAdcWest[16]", bbqAdcWest, &b_event_bbqAdcWest);
   fChain->SetBranchAddress("bbqEarliestEast", &bbqEarliestEast, &b_event_bbqEarliestEast);
   fChain->SetBranchAddress("bbqEarliestWest", &bbqEarliestWest, &b_event_bbqEarliestWest);
   fChain->SetBranchAddress("mxqEarliestEast", &mxqEarliestEast, &b_event_mxqEarliestEast);
   fChain->SetBranchAddress("mxqEarliestWest", &mxqEarliestWest, &b_event_mxqEarliestWest);
   fChain->SetBranchAddress("bbqSumTacEast", &bbqSumTacEast, &b_event_bbqSumTacEast);
   fChain->SetBranchAddress("bbqSumTacWest", &bbqSumTacWest, &b_event_bbqSumTacWest);
   fChain->SetBranchAddress("bbqNGoodHitEast", &bbqNGoodHitEast, &b_event_bbqNGoodHitEast);
   fChain->SetBranchAddress("bbqNGoodHitWest", &bbqNGoodHitWest, &b_event_bbqNGoodHitWest);
   fChain->SetBranchAddress("runId", &runId, &b_event_runId);
   fChain->SetBranchAddress("vpdZ", &vpdZ, &b_event_vpdZ);
   fChain->SetBranchAddress("vtx.mX1", &vtx_mX1, &b_event_vtx_mX1);
   fChain->SetBranchAddress("vtx.mX2", &vtx_mX2, &b_event_vtx_mX2);
   fChain->SetBranchAddress("vtx.mX3", &vtx_mX3, &b_event_vtx_mX3);
   fChain->SetBranchAddress("grefmult", &grefmult, &b_event_grefmult);
   fChain->SetBranchAddress("refmult", &refmult, &b_event_refmult);
   Notify();
}

Bool_t VpdPico::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void VpdPico::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t VpdPico::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}