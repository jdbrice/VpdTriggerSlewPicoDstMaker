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

   static const int NSUM = 11;

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          eventNumber;
   UInt_t          npre;
   UInt_t          npost;
   UInt_t          nsum;
   UShort_t        Vpd[NSUM][64];   //[nsum]
   UShort_t        VpdHi[NSUM][64];   //[nsum]
   UShort_t        bbqEastVpd[NSUM][32];   //[nsum]
   UShort_t        bbqWestVpd[NSUM][32];   //[nsum]
   UShort_t        fastBbqEastVpd[NSUM];   //[nsum]
   UShort_t        fastBbqWestVpd[NSUM];   //[nsum]
   UShort_t        sumBbqEastVpd[NSUM];   //[nsum]
   UShort_t        sumBbqWestVpd[NSUM];   //[nsum]
   UShort_t        sumADCBbqEastVpd[NSUM];   //[nsum]
   UShort_t        sumADCBbqWestVpd[NSUM];   //[nsum]
   UShort_t        nGoodBbqEastVpd[NSUM];   //[nsum]
   UShort_t        nGoodBbqWestVpd[NSUM];   //[nsum]
   UShort_t        bbcEastVpdNHits[NSUM];   //[nsum]
   UShort_t        bbcWestVpdNHits[NSUM];   //[nsum]
   UShort_t        bbcEastVpdADCsum[NSUM];   //[nsum]
   UShort_t        bbcWestVpdADCsum[NSUM];   //[nsum]
   UShort_t        bbcEastVpdTACsum[NSUM];   //[nsum]
   UShort_t        bbcWestVpdTACsum[NSUM];   //[nsum]
   UShort_t        mxqEastVpd[NSUM][32];   //[nsum]
   UShort_t        mxqWestVpd[NSUM][32];   //[nsum]
   UShort_t        fastMxqEastVpd[NSUM];   //[nsum]
   UShort_t        fastMxqWestVpd[NSUM];   //[nsum]
   UShort_t        tofMult[NSUM];   //[nsum]
   UShort_t        tofTrayMult[NSUM][120];   //[nsum]
   UShort_t        MT001[NSUM][32];   //[nsum]
   UShort_t        MT002[NSUM][32];   //[nsum]
   UShort_t        MT003[NSUM][32];   //[nsum]
   UShort_t        MT004[NSUM][32];   //[nsum]
   UShort_t        MT005[NSUM][32];   //[nsum]
   UShort_t        MT006[NSUM][32];   //[nsum]
   UShort_t        MT007[NSUM][32];   //[nsum]
   UShort_t        MT008[NSUM][32];   //[nsum]
   UChar_t         MtdDsm[NSUM][32];   //[nsum]
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
   TBranch        *b_bbcEastVpdNHits;   //!
   TBranch        *b_bbcWestVpdNHits;   //!
   TBranch        *b_bbcEastVpdADCsum;   //!
   TBranch        *b_bbcWestVpdADCsum;   //!
   TBranch        *b_bbcEastVpdTACsum;   //!
   TBranch        *b_bbcWestVpdTACsum;   //!
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



   virtual int runNumber(){
      return fChain->GetTreeNumber();
   }
   virtual int sumTAC( string side = "east" ){
      if ( "east" == side || "East" == side || "EAST" == side ){
         return bbcEastVpdTACsum[ iPrePost ];
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         return bbcWestVpdTACsum[ iPrePost ];  
      }
      return 0;
   }

   virtual int sumADC( string side = "east" ){
      if ( "east" == side || "East" == side || "EAST" == side ){
         return bbcEastVpdADCsum[ iPrePost ];
      }
      if ( "west" == side || "West" == side || "WEST" == side ){
         return bbcWestVpdADCsum[ iPrePost ];  
      }
      return 0;
   }

   virtual int nGood( string side = "east" ) {
      if ( "west" == side || "West" == side || "WEST" == side ){
         return bbcWestVpdNHits[ iPrePost ];
      } else if ( "east" == side || "East" == side || "EAST" == side ){
         return bbcEastVpdNHits[ iPrePost ];
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

