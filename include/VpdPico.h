//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb 19 23:45:02 2016 by ROOT version 6.04/00
// from TTree VpdPico/V0 Pico Dst
// found on file: ntuple_574.root
//////////////////////////////////////////////////////////

#ifndef VpdPico_h
#define VpdPico_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "DataAdapter.h"

// Header file for the classes stored in the TTree if any.
#include "TObject.h"

class VpdPico : public DataAdapter {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
 //StPicoEvent     *event;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Int_t           mxqTdcEast[16];
   Int_t           mxqTdcWest[16];
   Int_t           mxqAdcEast[16];
   Int_t           mxqAdcWest[16];
   Int_t           bbqTdcEast[16];
   Int_t           bbqTdcWest[16];
   Int_t           bbqAdcEast[16];
   Int_t           bbqAdcWest[16];
   Int_t           bbqEarliestEast;
   Int_t           bbqEarliestWest;
   Int_t           mxqEarliestEast;
   Int_t           mxqEarliestWest;
   Int_t           runId;
   Float_t         vpdZ;
   Double_t        vtx_mX1;
   Double_t        vtx_mX2;
   Double_t        vtx_mX3;
   Int_t           grefmult;
   Int_t           refmult;

   // List of branches
   TBranch        *b_event_fUniqueID;   //!
   TBranch        *b_event_fBits;   //!
   TBranch        *b_event_mxqTdcEast;   //!
   TBranch        *b_event_mxqTdcWest;   //!
   TBranch        *b_event_mxqAdcEast;   //!
   TBranch        *b_event_mxqAdcWest;   //!
   TBranch        *b_event_bbqTdcEast;   //!
   TBranch        *b_event_bbqTdcWest;   //!
   TBranch        *b_event_bbqAdcEast;   //!
   TBranch        *b_event_bbqAdcWest;   //!
   TBranch        *b_event_bbqEarliestEast;   //!
   TBranch        *b_event_bbqEarliestWest;   //!
   TBranch        *b_event_mxqEarliestEast;   //!
   TBranch        *b_event_mxqEarliestWest;   //!
   TBranch        *b_event_runId;   //!
   TBranch        *b_event_vpdZ;   //!
   TBranch        *b_event_vtx_mX1;   //!
   TBranch        *b_event_vtx_mX2;   //!
   TBranch        *b_event_vtx_mX3;   //!
   TBranch        *b_event_grefmult;   //!
   TBranch        *b_event_refmult;   //!

   VpdPico(TTree *tree=0);
   virtual ~VpdPico();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual int gRefMult(){
      return grefmult;
   }


   virtual int tdc( string side = "east", string crate="bbq", int tube = 0 ){
      if ( "east" == side || "East" == side || "EAST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return bbqTdcEast[ tube ];
         } else {
            return mxqTdcEast[ tube ];
         }
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return bbqTdcWest[ tube ];
         } else {
            return mxqTdcWest[ tube ];
         }
      }
      return 0;
   }
   virtual int adc( string side = "east", string crate="bbq", int tube = 0 ) { 
      if ( "east" == side || "East" == side || "EAST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return bbqAdcEast[ tube ];
         } else {
            return mxqAdcEast[ tube ];
         }
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return bbqAdcWest[ tube ];
         } else {
            return mxqAdcWest[ tube ];
         }
      }
      return 0;
   }

   virtual Float_t vpdVz() {
      return vpdZ;
   }

   virtual Float_t vX() {
      return vtx_mX1;
   }
   virtual Float_t vY() {
      return vtx_mX2;
   }
   virtual Float_t vZ() {
      return vtx_mX3;
   }

   virtual Int_t runNumber(){
      return runId;
   }
   virtual Int_t eventNumber(){
      return 0;
   }


};

#endif
