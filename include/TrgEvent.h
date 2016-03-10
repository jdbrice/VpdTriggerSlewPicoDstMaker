//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb 12 09:38:39 2016 by ROOT version 6.04/00
// from TTree TrgEvent/TrgEvent
// found on file: /Users/danielbrandenburg/bnl/local/data/Run16/vpd_trigger/run17042030.2.dat.root
//////////////////////////////////////////////////////////

#ifndef TrgEvent_h
#define TrgEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "DataAdapter.h"


// Header file for the classes stored in the TTree if any.

class TrgEvent : public DataAdapter{
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          eventNumber;
   UInt_t          npre;
   UInt_t          npost;
   UInt_t          nsum;
   UShort_t        Vpd[1][64];   //[nsum]
   UShort_t        VpdHi[1][64];   //[nsum]
   UShort_t        bbqEastVpd[1][32];   //[nsum]
   UShort_t        bbqWestVpd[1][32];   //[nsum]
   UShort_t        fastBbqEastVpd[1];   //[nsum]
   UShort_t        fastBbqWestVpd[1];   //[nsum]
   UShort_t        sumBbqEastVpd[1];   //[nsum]
   UShort_t        sumBbqWestVpd[1];   //[nsum]
   UShort_t        sumADCBbqEastVpd[1];   //[nsum]
   UShort_t        sumADCBbqWestVpd[1];   //[nsum]
   UShort_t        nGoodBbqEastVpd[1];   //[nsum]
   UShort_t        nGoodBbqWestVpd[1];   //[nsum]
   UShort_t        mxqEastVpd[1][32];   //[nsum]
   UShort_t        mxqWestVpd[1][32];   //[nsum]
   UShort_t        fastMxqEastVpd[1];   //[nsum]
   UShort_t        fastMxqWestVpd[1];   //[nsum]
   UShort_t        tofMult[1];   //[nsum]
   UShort_t        tofTrayMult[1][120];   //[nsum]
   UShort_t        MT001[1][32];   //[nsum]
   UShort_t        MT002[1][32];   //[nsum]
   UShort_t        MT003[1][32];   //[nsum]
   UShort_t        MT004[1][32];   //[nsum]
   UShort_t        MT005[1][32];   //[nsum]
   UShort_t        MT006[1][32];   //[nsum]
   UShort_t        MT007[1][32];   //[nsum]
   UShort_t        MT008[1][32];   //[nsum]
   UChar_t         MtdDsm[1][32];   //[nsum]
   UShort_t        DsmTF201Ch[8];
   UShort_t        lastDsmBit[8];

   // List of branches
   TBranch        *b_eventNumber;   //!
   TBranch        *b_npre;   //!
   TBranch        *b_npost;   //!
   TBranch        *b_nsum;   //!
   TBranch        *b_Vpd;   //!
   TBranch        *b_VpdHi;   //!
   TBranch        *b_bbqEastVpd;   //!
   TBranch        *b_bbqWestVpd;   //!
   TBranch        *b_fastBbqEastVpd;   //!
   TBranch        *b_fastBbqWestVpd;   //!
   TBranch        *b_sumBbqEastVpd;   //!
   TBranch        *b_sumBbqWestVpd;   //!
   TBranch        *b_sumADCBbqEastVpd;   //!
   TBranch        *b_sumADCBbqWestVpd;   //!
   TBranch        *b_nGoodBbqEastVpd;   //!
   TBranch        *b_nGoodBbqWestVpd;   //!
   TBranch        *b_mxqEastVpd;   //!
   TBranch        *b_mxqWestVpd;   //!
   TBranch        *b_fastMxqEastVpd;   //!
   TBranch        *b_fastMxqWestVpd;   //!
   TBranch        *b_tofMult;   //!
   TBranch        *b_tofTrayMult;   //!
   TBranch        *b_MT001;   //!
   TBranch        *b_MT002;   //!
   TBranch        *b_MT003;   //!
   TBranch        *b_MT004;   //!
   TBranch        *b_MT005;   //!
   TBranch        *b_MT006;   //!
   TBranch        *b_MT007;   //!
   TBranch        *b_MT008;   //!
   TBranch        *b_MtdDsm;   //!
   TBranch        *b_DsmTF201Ch;   //!
   TBranch        *b_lastDsmBit;   //!

   TrgEvent(TTree *tree=0);
   virtual ~TrgEvent();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);


   virtual int sumTAC( string side = "east" ){
      if ( "east" == side || "East" == side || "EAST" == side ){
         return sumBbqEastVpd[ iPrePost ];
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         return sumBbqWestVpd[ iPrePost ];  
      }
      return 0;
   }

   virtual int sumADC( string side = "east" ){
      if ( "east" == side || "East" == side || "EAST" == side ){
         return sumADCBbqEastVpd[ iPrePost ];
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         return sumADCBbqWestVpd[ iPrePost ];  
      }
      return 0;
   }

   virtual int nGood( string side = "east" ) {
      if ( "west" == side || "West" == side || "WEST" == side ){
         return nGoodBbqWestVpd[ iPrePost ];
      } else if ( "east" == side || "East" == side || "EAST" == side ){
         return nGoodBbqEastVpd[ iPrePost ];
      }
      return 0;
   }

   virtual int tdc( string side = "east", string crate="bbq", int tube = 0 ) { 
      
      int iTDC = tube + tube / 4 * 4 + 4;

      if ( "east" == side || "East" == side || "EAST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return bbqEastVpd[iPrePost][ iTDC ];
         } else {
            return mxqEastVpd[iPrePost][ iTDC ];
         }
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return bbqWestVpd[iPrePost][ iTDC ];
         } else {
            return mxqWestVpd[iPrePost][ iTDC ];
         }
      }
      return 0;
   }
   virtual int adc( string side = "east", string crate="bbq", int tube = 0 ) {
      int iADC = tube + tube / 4 * 4;
      if ( "east" == side || "East" == side || "EAST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return bbqEastVpd[iPrePost][ iADC ];
         } else {
            return mxqEastVpd[iPrePost][ iADC ];
         }
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return bbqWestVpd[iPrePost][ iADC ];
         } else {
            return mxqWestVpd[iPrePost][ iADC ];
         }
      }
      return 0;
   }

   virtual int fastTdc( string side = "east", string crate="bbq" ) {
      if ( "east" == side || "East" == side || "EAST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return fastMxqEastVpd[iPrePost];
         } else {
            return fastMxqEastVpd[iPrePost];
         }
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){
            return fastBbqWestVpd[iPrePost];
         } else {
            return fastMxqWestVpd[iPrePost];
         }
      }
      return 0;
   }

   virtual Int_t trigger(){
      return 0;
   }


};

#endif

