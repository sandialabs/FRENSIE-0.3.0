//---------------------------------------------------------------------------//
//! 
//! \file   MonteCarlo_NuclearReactionType.cpp
//! \author Alex Robinson
//! \brief  Nuclear reaction type helper function definitions.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <stdexcept>
#include <sstream>

// FRENSIE Includes
#include "MonteCarlo_NuclearReactionType.hpp"
#include "Utility_ExceptionTestMacros.hpp"

namespace MonteCarlo{

// Convert an unsigned int to a NuclearReactionType
NuclearReactionType convertUnsignedToNuclearReactionType(
						      const unsigned reaction )
{
  switch( reaction )
  {
  case 1u: return N__TOTAL_REACTION;
  case 2u: return N__N_ELASTIC_REACTION;
  case 3u: return N__N_NON_ELASTIC_REACTION;
  case 4u: return N__N_INELASTIC_REACTION;
  case 5u: return N__ANYTHING_REACTION;
  case 11u: return N__2N_D_REACTION;
  case 16u: return N__2N_REACTION;
  case 17u: return N__3N_REACTION;
  case 18u: return N__TOTAL_FISSION_REACTION;
  case 19u: return N__FISSION_REACTION;
  case 20u: return N__N_FISSION_REACTION;
  case 21u: return N__2N_FISSION_REACTION;
  case 22u: return N__N_ALPHA_REACTION;
  case 23u: return N__N_3ALPHA_REACTION;
  case 24u: return N__2N_ALPHA_REACTION;
  case 25u: return N__3N_ALPHA_REACTION;
  case 27u: return N__TOTAL_ABSORPTION_REACTION;
  case 28u: return N__N_P_REACTION;
  case 29u: return N__N_2ALPHA_REACTION;
  case 30u: return N__2N_2ALPHA_REACTION;
  case 32u: return N__N_D_REACTION;
  case 33u: return N__N_T_REACTION;
  case 34u: return N__N_HE3_REACTION;
  case 35u: return N__N_D_2ALPHA_REACTION;
  case 36u: return N__N_T_2ALPHA_REACTION;
  case 37u: return N__4N_REACTION;
  case 38u: return N__3N_FISSION_REACTION;
  case 41u: return N__2N_P_REACTION;
  case 42u: return N__3N_P_REACTION;
  case 44u: return N__N_2P_REACTION;
  case 45u: return N__N_P_ALPHA_REACTION;
  case 51u: return N__N_EXCITED_STATE_1_REACTION;
  case 52u: return N__N_EXCITED_STATE_2_REACTION;
  case 53u: return N__N_EXCITED_STATE_3_REACTION;
  case 54u: return N__N_EXCITED_STATE_4_REACTION;
  case 55u: return N__N_EXCITED_STATE_5_REACTION;
  case 56u: return N__N_EXCITED_STATE_6_REACTION;
  case 57u: return N__N_EXCITED_STATE_7_REACTION;
  case 58u: return N__N_EXCITED_STATE_8_REACTION;
  case 59u: return N__N_EXCITED_STATE_9_REACTION;
  case 60u: return N__N_EXCITED_STATE_10_REACTION;
  case 61u: return N__N_EXCITED_STATE_11_REACTION;
  case 62u: return N__N_EXCITED_STATE_12_REACTION;
  case 63u: return N__N_EXCITED_STATE_13_REACTION;
  case 64u: return N__N_EXCITED_STATE_14_REACTION;
  case 65u: return N__N_EXCITED_STATE_15_REACTION;
  case 66u: return N__N_EXCITED_STATE_16_REACTION;
  case 67u: return N__N_EXCITED_STATE_17_REACTION;
  case 68u: return N__N_EXCITED_STATE_18_REACTION;
  case 69u: return N__N_EXCITED_STATE_19_REACTION;
  case 70u: return N__N_EXCITED_STATE_20_REACTION;
  case 71u: return N__N_EXCITED_STATE_21_REACTION;
  case 72u: return N__N_EXCITED_STATE_22_REACTION;
  case 73u: return N__N_EXCITED_STATE_23_REACTION;
  case 74u: return N__N_EXCITED_STATE_24_REACTION;
  case 75u: return N__N_EXCITED_STATE_25_REACTION;
  case 76u: return N__N_EXCITED_STATE_26_REACTION;
  case 77u: return N__N_EXCITED_STATE_27_REACTION;
  case 78u: return N__N_EXCITED_STATE_28_REACTION;
  case 79u: return N__N_EXCITED_STATE_29_REACTION;
  case 80u: return N__N_EXCITED_STATE_30_REACTION;
  case 81u: return N__N_EXCITED_STATE_31_REACTION;
  case 82u: return N__N_EXCITED_STATE_32_REACTION;
  case 83u: return N__N_EXCITED_STATE_33_REACTION;
  case 84u: return N__N_EXCITED_STATE_34_REACTION;
  case 85u: return N__N_EXCITED_STATE_35_REACTION;
  case 86u: return N__N_EXCITED_STATE_36_REACTION;
  case 87u: return N__N_EXCITED_STATE_37_REACTION;
  case 88u: return N__N_EXCITED_STATE_38_REACTION;
  case 89u: return N__N_EXCITED_STATE_39_REACTION;
  case 90u: return N__N_EXCITED_STATE_40_REACTION;
  case 91u: return N__N_CONTINUUM_REACTION;
  case 101u: return N__CAPTURE_REACTION;
  case 102u: return N__GAMMA_REACTION;
  case 103u: return N__P_REACTION;
  case 104u: return N__D_REACTION;
  case 105u: return N__T_REACTION;
  case 106u: return N__HE3_REACTION;
  case 107u: return N__ALPHA_REACTION;
  case 108u: return N__2ALPHA_REACTION;
  case 109u: return N__3ALPHA_REACTION;
  case 111u: return N__2P_REACTION;
  case 112u: return N__P_ALPHA_REACTION;
  case 113u: return N__T_2ALPHA_REACTION;
  case 114u: return N__D_2ALPHA_REACTION;
  case 115u: return N__P_D_REACTION;
  case 116u: return N__P_T_REACTION;
  case 117u: return N__D_ALPHA_REACTION;
  case 152u: return N__5N_REACTION;
  case 153u: return N__6N_REACTION;
  case 154u: return N__2N_T_REACTION;
  case 155u: return N__T_ALPHA_REACTION;
  case 156u: return N__4N_P_REACTION;
  case 157u: return N__3N_D_REACTION;
  case 158u: return N__N_D_ALPHA_REACTION;
  case 159u: return N__2N_P_ALPHA_REACTION;
  case 160u: return N__7N_REACTION;
  case 161u: return N__8N_REACTION;
  case 162u: return N__5N_P_REACTION;
  case 163u: return N__6N_P_REACTION;
  case 164u: return N__7N_P_REACTION;
  case 165u: return N__4N_ALPHA_REACTION;
  case 166u: return N__5N_ALPHA_REACTION;
  case 167u: return N__6N_ALPHA_REACTION;
  case 168u: return N__7N_ALPHA_REACTION;
  case 169u: return N__4N_D_REACTION;
  case 170u: return N__5N_D_REACTION;
  case 171u: return N__6N_D_REACTION;
  case 172u: return N__3N_T_REACTION;
  case 173u: return N__4N_T_REACTION;
  case 174u: return N__5N_T_REACTION;
  case 175u: return N__6N_T_REACTION;
  case 176u: return N__2N_HE3_REACTION;
  case 177u: return N__3N_HE3_REACTION;
  case 178u: return N__4N_HE3_REACTION;
  case 179u: return N__3N_2P_REACTION;
  case 180u: return N__3N_2ALPHA_REACTION;
  case 181u: return N__3N_P_ALPHA_REACTION;
  case 182u: return N__D_T_REACTION;
  case 183u: return N__N_P_D_REACTION;
  case 184u: return N__N_P_T_REACTION;
  case 185u: return N__N_D_T_REACTION;
  case 186u: return N__N_P_HE3_REACTION;
  case 187u: return N__N_D_HE3_REACTION;
  case 188u: return N__N_T_HE3_REACTION;
  case 189u: return N__N_T_ALPHA_REACTION;
  case 190u: return N__2N_2P_REACTION;
  case 191u: return N__P_HE3_REACTION;
  case 192u: return N__D_HE3_REACTION;
  case 193u: return N__HE3_ALPHA_REACTION;
  case 194u: return N__4N_2P_REACTION;
  case 195u: return N__4N_2ALPHA_REACTION;
  case 196u: return N__4N_P_ALPHA_REACTION;
  case 197u: return N__3P_REACTION;
  case 198u: return N__N_3P_REACTION;
  case 199u: return N__3N_2P_ALPHA_REACTION;
  case 200u: return N__5N_2P_REACTION;
  case 201u: return N__TOTAL_N_PRODUCTION;
  case 202u: return N__TOTAL_GAMMA_PRODUCTION;
  case 203u: return N__TOTAL_P_PRODUCTION;
  case 204u: return N__TOTAL_D_PRODUCTION;
  case 205u: return N__TOTAL_T_PRODUCTION;
  case 206u: return N__TOTAL_HE3_PRODUCTION;
  case 207u: return N__TOTAL_ALPHA_PRODUCTION;
  case 301u: return N__AVERAGE_HEATING;
  case 444u: return N__DPA;
  case 600u: return N__P_EXCITED_STATE_0_REACTION;
  case 601u: return N__P_EXCITED_STATE_1_REACTION;
  case 602u: return N__P_EXCITED_STATE_2_REACTION;
  case 603u: return N__P_EXCITED_STATE_3_REACTION;
  case 604u: return N__P_EXCITED_STATE_4_REACTION;
  case 605u: return N__P_EXCITED_STATE_5_REACTION;
  case 606u: return N__P_EXCITED_STATE_6_REACTION;
  case 607u: return N__P_EXCITED_STATE_7_REACTION;
  case 608u: return N__P_EXCITED_STATE_8_REACTION;
  case 609u: return N__P_EXCITED_STATE_9_REACTION;
  case 610u: return N__P_EXCITED_STATE_10_REACTION;
  case 611u: return N__P_EXCITED_STATE_11_REACTION;
  case 612u: return N__P_EXCITED_STATE_12_REACTION;
  case 613u: return N__P_EXCITED_STATE_13_REACTION;
  case 614u: return N__P_EXCITED_STATE_14_REACTION;
  case 615u: return N__P_EXCITED_STATE_15_REACTION;
  case 616u: return N__P_EXCITED_STATE_16_REACTION;
  case 617u: return N__P_EXCITED_STATE_17_REACTION;
  case 618u: return N__P_EXCITED_STATE_18_REACTION;
  case 619u: return N__P_EXCITED_STATE_19_REACTION;
  case 620u: return N__P_EXCITED_STATE_20_REACTION;
  case 621u: return N__P_EXCITED_STATE_21_REACTION;
  case 622u: return N__P_EXCITED_STATE_22_REACTION;
  case 623u: return N__P_EXCITED_STATE_23_REACTION;
  case 624u: return N__P_EXCITED_STATE_24_REACTION;
  case 625u: return N__P_EXCITED_STATE_25_REACTION;
  case 626u: return N__P_EXCITED_STATE_26_REACTION;
  case 627u: return N__P_EXCITED_STATE_27_REACTION;
  case 628u: return N__P_EXCITED_STATE_28_REACTION;
  case 629u: return N__P_EXCITED_STATE_29_REACTION;
  case 630u: return N__P_EXCITED_STATE_30_REACTION;
  case 631u: return N__P_EXCITED_STATE_31_REACTION;
  case 632u: return N__P_EXCITED_STATE_32_REACTION;
  case 633u: return N__P_EXCITED_STATE_33_REACTION;
  case 634u: return N__P_EXCITED_STATE_34_REACTION;
  case 635u: return N__P_EXCITED_STATE_35_REACTION;
  case 636u: return N__P_EXCITED_STATE_36_REACTION;
  case 637u: return N__P_EXCITED_STATE_37_REACTION;
  case 638u: return N__P_EXCITED_STATE_38_REACTION;
  case 639u: return N__P_EXCITED_STATE_39_REACTION;
  case 640u: return N__P_EXCITED_STATE_40_REACTION;
  case 641u: return N__P_EXCITED_STATE_41_REACTION;
  case 642u: return N__P_EXCITED_STATE_42_REACTION;
  case 643u: return N__P_EXCITED_STATE_43_REACTION;
  case 644u: return N__P_EXCITED_STATE_44_REACTION;
  case 645u: return N__P_EXCITED_STATE_45_REACTION;
  case 646u: return N__P_EXCITED_STATE_46_REACTION;
  case 647u: return N__P_EXCITED_STATE_47_REACTION;
  case 648u: return N__P_EXCITED_STATE_48_REACTION;
  case 649u: return N__P_CONTINUUM_REACTION;
  case 650u: return N__D_EXCITED_STATE_0_REACTION;
  case 651u: return N__D_EXCITED_STATE_1_REACTION;
  case 652u: return N__D_EXCITED_STATE_2_REACTION;
  case 653u: return N__D_EXCITED_STATE_3_REACTION;
  case 654u: return N__D_EXCITED_STATE_4_REACTION;
  case 655u: return N__D_EXCITED_STATE_5_REACTION;
  case 656u: return N__D_EXCITED_STATE_6_REACTION;
  case 657u: return N__D_EXCITED_STATE_7_REACTION;
  case 658u: return N__D_EXCITED_STATE_8_REACTION;
  case 659u: return N__D_EXCITED_STATE_9_REACTION;
  case 660u: return N__D_EXCITED_STATE_10_REACTION;
  case 661u: return N__D_EXCITED_STATE_11_REACTION;
  case 662u: return N__D_EXCITED_STATE_12_REACTION;
  case 663u: return N__D_EXCITED_STATE_13_REACTION;
  case 664u: return N__D_EXCITED_STATE_14_REACTION;
  case 665u: return N__D_EXCITED_STATE_15_REACTION;
  case 666u: return N__D_EXCITED_STATE_16_REACTION;
  case 667u: return N__D_EXCITED_STATE_17_REACTION;
  case 668u: return N__D_EXCITED_STATE_18_REACTION;
  case 669u: return N__D_EXCITED_STATE_19_REACTION;
  case 670u: return N__D_EXCITED_STATE_20_REACTION;
  case 671u: return N__D_EXCITED_STATE_21_REACTION;
  case 672u: return N__D_EXCITED_STATE_22_REACTION;
  case 673u: return N__D_EXCITED_STATE_23_REACTION;
  case 674u: return N__D_EXCITED_STATE_24_REACTION;
  case 675u: return N__D_EXCITED_STATE_25_REACTION;
  case 676u: return N__D_EXCITED_STATE_26_REACTION;
  case 677u: return N__D_EXCITED_STATE_27_REACTION;
  case 678u: return N__D_EXCITED_STATE_28_REACTION;
  case 679u: return N__D_EXCITED_STATE_29_REACTION;
  case 680u: return N__D_EXCITED_STATE_30_REACTION;
  case 681u: return N__D_EXCITED_STATE_31_REACTION;
  case 682u: return N__D_EXCITED_STATE_32_REACTION;
  case 683u: return N__D_EXCITED_STATE_33_REACTION;
  case 684u: return N__D_EXCITED_STATE_34_REACTION;
  case 685u: return N__D_EXCITED_STATE_35_REACTION;
  case 686u: return N__D_EXCITED_STATE_36_REACTION;
  case 687u: return N__D_EXCITED_STATE_37_REACTION;
  case 688u: return N__D_EXCITED_STATE_38_REACTION;
  case 689u: return N__D_EXCITED_STATE_39_REACTION;
  case 690u: return N__D_EXCITED_STATE_40_REACTION;
  case 691u: return N__D_EXCITED_STATE_41_REACTION;
  case 692u: return N__D_EXCITED_STATE_42_REACTION;
  case 693u: return N__D_EXCITED_STATE_43_REACTION;
  case 694u: return N__D_EXCITED_STATE_44_REACTION;
  case 695u: return N__D_EXCITED_STATE_45_REACTION;
  case 696u: return N__D_EXCITED_STATE_46_REACTION;
  case 697u: return N__D_EXCITED_STATE_47_REACTION;
  case 698u: return N__D_EXCITED_STATE_48_REACTION;
  case 699u: return N__D_CONTINUUM_REACTION;
  case 700u: return N__T_EXCITED_STATE_0_REACTION;
  case 701u: return N__T_EXCITED_STATE_1_REACTION;
  case 702u: return N__T_EXCITED_STATE_2_REACTION;
  case 703u: return N__T_EXCITED_STATE_3_REACTION;
  case 704u: return N__T_EXCITED_STATE_4_REACTION;
  case 705u: return N__T_EXCITED_STATE_5_REACTION;
  case 706u: return N__T_EXCITED_STATE_6_REACTION;
  case 707u: return N__T_EXCITED_STATE_7_REACTION;
  case 708u: return N__T_EXCITED_STATE_8_REACTION;
  case 709u: return N__T_EXCITED_STATE_9_REACTION;
  case 710u: return N__T_EXCITED_STATE_10_REACTION;
  case 711u: return N__T_EXCITED_STATE_11_REACTION;
  case 712u: return N__T_EXCITED_STATE_12_REACTION;
  case 713u: return N__T_EXCITED_STATE_13_REACTION;
  case 714u: return N__T_EXCITED_STATE_14_REACTION;
  case 715u: return N__T_EXCITED_STATE_15_REACTION;
  case 716u: return N__T_EXCITED_STATE_16_REACTION;
  case 717u: return N__T_EXCITED_STATE_17_REACTION;
  case 718u: return N__T_EXCITED_STATE_18_REACTION;
  case 719u: return N__T_EXCITED_STATE_19_REACTION;
  case 720u: return N__T_EXCITED_STATE_20_REACTION;
  case 721u: return N__T_EXCITED_STATE_21_REACTION;
  case 722u: return N__T_EXCITED_STATE_22_REACTION;
  case 723u: return N__T_EXCITED_STATE_23_REACTION;
  case 724u: return N__T_EXCITED_STATE_24_REACTION;
  case 725u: return N__T_EXCITED_STATE_25_REACTION;
  case 726u: return N__T_EXCITED_STATE_26_REACTION;
  case 727u: return N__T_EXCITED_STATE_27_REACTION;
  case 728u: return N__T_EXCITED_STATE_28_REACTION;
  case 729u: return N__T_EXCITED_STATE_29_REACTION;
  case 730u: return N__T_EXCITED_STATE_30_REACTION;
  case 731u: return N__T_EXCITED_STATE_31_REACTION;
  case 732u: return N__T_EXCITED_STATE_32_REACTION;
  case 733u: return N__T_EXCITED_STATE_33_REACTION;
  case 734u: return N__T_EXCITED_STATE_34_REACTION;
  case 735u: return N__T_EXCITED_STATE_35_REACTION;
  case 736u: return N__T_EXCITED_STATE_36_REACTION;
  case 737u: return N__T_EXCITED_STATE_37_REACTION;
  case 738u: return N__T_EXCITED_STATE_38_REACTION;
  case 739u: return N__T_EXCITED_STATE_39_REACTION;
  case 740u: return N__T_EXCITED_STATE_40_REACTION;
  case 741u: return N__T_EXCITED_STATE_41_REACTION;
  case 742u: return N__T_EXCITED_STATE_42_REACTION;
  case 743u: return N__T_EXCITED_STATE_43_REACTION;
  case 744u: return N__T_EXCITED_STATE_44_REACTION;
  case 745u: return N__T_EXCITED_STATE_45_REACTION;
  case 746u: return N__T_EXCITED_STATE_46_REACTION;
  case 747u: return N__T_EXCITED_STATE_47_REACTION;
  case 748u: return N__T_EXCITED_STATE_48_REACTION;
  case 749u: return N__T_CONTINUUM_REACTION;
  case 750u: return N__HE3_EXCITED_STATE_0_REACTION;
  case 751u: return N__HE3_EXCITED_STATE_1_REACTION;
  case 752u: return N__HE3_EXCITED_STATE_2_REACTION;
  case 753u: return N__HE3_EXCITED_STATE_3_REACTION;
  case 754u: return N__HE3_EXCITED_STATE_4_REACTION;
  case 755u: return N__HE3_EXCITED_STATE_5_REACTION;
  case 756u: return N__HE3_EXCITED_STATE_6_REACTION;
  case 757u: return N__HE3_EXCITED_STATE_7_REACTION;
  case 758u: return N__HE3_EXCITED_STATE_8_REACTION;
  case 759u: return N__HE3_EXCITED_STATE_9_REACTION;
  case 760u: return N__HE3_EXCITED_STATE_10_REACTION;
  case 761u: return N__HE3_EXCITED_STATE_11_REACTION;
  case 762u: return N__HE3_EXCITED_STATE_12_REACTION;
  case 763u: return N__HE3_EXCITED_STATE_13_REACTION;
  case 764u: return N__HE3_EXCITED_STATE_14_REACTION;
  case 765u: return N__HE3_EXCITED_STATE_15_REACTION;
  case 766u: return N__HE3_EXCITED_STATE_16_REACTION;
  case 767u: return N__HE3_EXCITED_STATE_17_REACTION;
  case 768u: return N__HE3_EXCITED_STATE_18_REACTION;
  case 769u: return N__HE3_EXCITED_STATE_19_REACTION;
  case 770u: return N__HE3_EXCITED_STATE_20_REACTION;
  case 771u: return N__HE3_EXCITED_STATE_21_REACTION;
  case 772u: return N__HE3_EXCITED_STATE_22_REACTION;
  case 773u: return N__HE3_EXCITED_STATE_23_REACTION;
  case 774u: return N__HE3_EXCITED_STATE_24_REACTION;
  case 775u: return N__HE3_EXCITED_STATE_25_REACTION;
  case 776u: return N__HE3_EXCITED_STATE_26_REACTION;
  case 777u: return N__HE3_EXCITED_STATE_27_REACTION;
  case 778u: return N__HE3_EXCITED_STATE_28_REACTION;
  case 779u: return N__HE3_EXCITED_STATE_29_REACTION;
  case 780u: return N__HE3_EXCITED_STATE_30_REACTION;
  case 781u: return N__HE3_EXCITED_STATE_31_REACTION;
  case 782u: return N__HE3_EXCITED_STATE_32_REACTION;
  case 783u: return N__HE3_EXCITED_STATE_33_REACTION;
  case 784u: return N__HE3_EXCITED_STATE_34_REACTION;
  case 785u: return N__HE3_EXCITED_STATE_35_REACTION;
  case 786u: return N__HE3_EXCITED_STATE_36_REACTION;
  case 787u: return N__HE3_EXCITED_STATE_37_REACTION;
  case 788u: return N__HE3_EXCITED_STATE_38_REACTION;
  case 789u: return N__HE3_EXCITED_STATE_39_REACTION;
  case 790u: return N__HE3_EXCITED_STATE_40_REACTION;
  case 791u: return N__HE3_EXCITED_STATE_41_REACTION;
  case 792u: return N__HE3_EXCITED_STATE_42_REACTION;
  case 793u: return N__HE3_EXCITED_STATE_43_REACTION;
  case 794u: return N__HE3_EXCITED_STATE_44_REACTION;
  case 795u: return N__HE3_EXCITED_STATE_45_REACTION;
  case 796u: return N__HE3_EXCITED_STATE_46_REACTION;
  case 797u: return N__HE3_EXCITED_STATE_47_REACTION;
  case 798u: return N__HE3_EXCITED_STATE_48_REACTION;
  case 799u: return N__HE3_CONTINUUM_REACTION;
  case 800u: return N__ALPHA_EXCITED_STATE_0_REACTION;
  case 801u: return N__ALPHA_EXCITED_STATE_1_REACTION;
  case 802u: return N__ALPHA_EXCITED_STATE_2_REACTION;
  case 803u: return N__ALPHA_EXCITED_STATE_3_REACTION;
  case 804u: return N__ALPHA_EXCITED_STATE_4_REACTION;
  case 805u: return N__ALPHA_EXCITED_STATE_5_REACTION;
  case 806u: return N__ALPHA_EXCITED_STATE_6_REACTION;
  case 807u: return N__ALPHA_EXCITED_STATE_7_REACTION;
  case 808u: return N__ALPHA_EXCITED_STATE_8_REACTION;
  case 809u: return N__ALPHA_EXCITED_STATE_9_REACTION;
  case 810u: return N__ALPHA_EXCITED_STATE_10_REACTION;
  case 811u: return N__ALPHA_EXCITED_STATE_11_REACTION;
  case 812u: return N__ALPHA_EXCITED_STATE_12_REACTION;
  case 813u: return N__ALPHA_EXCITED_STATE_13_REACTION;
  case 814u: return N__ALPHA_EXCITED_STATE_14_REACTION;
  case 815u: return N__ALPHA_EXCITED_STATE_15_REACTION;
  case 816u: return N__ALPHA_EXCITED_STATE_16_REACTION;
  case 817u: return N__ALPHA_EXCITED_STATE_17_REACTION;
  case 818u: return N__ALPHA_EXCITED_STATE_18_REACTION;
  case 819u: return N__ALPHA_EXCITED_STATE_19_REACTION;
  case 820u: return N__ALPHA_EXCITED_STATE_20_REACTION;
  case 821u: return N__ALPHA_EXCITED_STATE_21_REACTION;
  case 822u: return N__ALPHA_EXCITED_STATE_22_REACTION;
  case 823u: return N__ALPHA_EXCITED_STATE_23_REACTION;
  case 824u: return N__ALPHA_EXCITED_STATE_24_REACTION;
  case 825u: return N__ALPHA_EXCITED_STATE_25_REACTION;
  case 826u: return N__ALPHA_EXCITED_STATE_26_REACTION;
  case 827u: return N__ALPHA_EXCITED_STATE_27_REACTION;
  case 828u: return N__ALPHA_EXCITED_STATE_28_REACTION;
  case 829u: return N__ALPHA_EXCITED_STATE_29_REACTION;
  case 830u: return N__ALPHA_EXCITED_STATE_30_REACTION;
  case 831u: return N__ALPHA_EXCITED_STATE_31_REACTION;
  case 832u: return N__ALPHA_EXCITED_STATE_32_REACTION;
  case 833u: return N__ALPHA_EXCITED_STATE_33_REACTION;
  case 834u: return N__ALPHA_EXCITED_STATE_34_REACTION;
  case 835u: return N__ALPHA_EXCITED_STATE_35_REACTION;
  case 836u: return N__ALPHA_EXCITED_STATE_36_REACTION;
  case 837u: return N__ALPHA_EXCITED_STATE_37_REACTION;
  case 838u: return N__ALPHA_EXCITED_STATE_38_REACTION;
  case 839u: return N__ALPHA_EXCITED_STATE_39_REACTION;
  case 840u: return N__ALPHA_EXCITED_STATE_40_REACTION;
  case 841u: return N__ALPHA_EXCITED_STATE_41_REACTION;
  case 842u: return N__ALPHA_EXCITED_STATE_42_REACTION;
  case 843u: return N__ALPHA_EXCITED_STATE_43_REACTION;
  case 844u: return N__ALPHA_EXCITED_STATE_44_REACTION;
  case 845u: return N__ALPHA_EXCITED_STATE_45_REACTION;
  case 846u: return N__ALPHA_EXCITED_STATE_46_REACTION;
  case 847u: return N__ALPHA_EXCITED_STATE_47_REACTION;
  case 848u: return N__ALPHA_EXCITED_STATE_48_REACTION;
  case 849u: return N__ALPHA_CONTINUUM_REACTION;
  case 875u: return N__2N_EXCITED_STATE_0_REACTION;
  case 876u: return N__2N_EXCITED_STATE_1_REACTION;
  case 877u: return N__2N_EXCITED_STATE_2_REACTION;
  case 878u: return N__2N_EXCITED_STATE_3_REACTION;
  case 879u: return N__2N_EXCITED_STATE_4_REACTION;
  case 880u: return N__2N_EXCITED_STATE_5_REACTION;
  case 881u: return N__2N_EXCITED_STATE_6_REACTION;
  case 882u: return N__2N_EXCITED_STATE_7_REACTION;
  case 883u: return N__2N_EXCITED_STATE_8_REACTION;
  case 884u: return N__2N_EXCITED_STATE_9_REACTION;
  case 885u: return N__2N_EXCITED_STATE_10_REACTION;
  case 886u: return N__2N_EXCITED_STATE_11_REACTION;
  case 887u: return N__2N_EXCITED_STATE_12_REACTION;
  case 888u: return N__2N_EXCITED_STATE_13_REACTION;
  case 889u: return N__2N_EXCITED_STATE_14_REACTION;
  case 890u: return N__2N_EXCITED_STATE_15_REACTION;
  case 891u: return N__2N_CONTINUUM_REACTION;
  default:
    THROW_EXCEPTION( std::runtime_error,
		     "Error: ENDF reaction (MT #) " << reaction <<
		     " is not supported.\n" );
  }
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_NuclearReactionType.cpp
//---------------------------------------------------------------------------//
