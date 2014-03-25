/* Generated automatically by the program `genopinit'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "expr.h"
#include "optabs.h"
#include "reload.h"

void
init_all_optabs (void)
{
#ifdef FIXUNS_TRUNC_LIKE_FIX_TRUNC
  int i, j;
#endif

  if (HAVE_mulhisi3)
    set_widening_optab_handler (smul_widen_optab, SImode, HImode, CODE_FOR_mulhisi3);
  if (HAVE_maddhisi4)
    set_widening_optab_handler (smadd_widen_optab, SImode, HImode, CODE_FOR_maddhisi4);
  if (HAVE_maddhidi4)
    set_widening_optab_handler (smadd_widen_optab, DImode, HImode, CODE_FOR_maddhidi4);
  if (HAVE_divsi3)
    set_optab_handler (sdiv_optab, SImode, CODE_FOR_divsi3);
  if (HAVE_udivsi3)
    set_optab_handler (udiv_optab, SImode, CODE_FOR_udivsi3);
  if (HAVE_one_cmpldi2)
    set_optab_handler (one_cmpl_optab, DImode, CODE_FOR_one_cmpldi2);
  if (HAVE_zero_extendqidi2)
    set_convert_optab_handler (zext_optab, DImode, QImode, CODE_FOR_zero_extendqidi2);
  if (HAVE_zero_extendhidi2)
    set_convert_optab_handler (zext_optab, DImode, HImode, CODE_FOR_zero_extendhidi2);
  if (HAVE_zero_extendsidi2)
    set_convert_optab_handler (zext_optab, DImode, SImode, CODE_FOR_zero_extendsidi2);
  if (HAVE_extendqidi2)
    set_convert_optab_handler (sext_optab, DImode, QImode, CODE_FOR_extendqidi2);
  if (HAVE_extendhidi2)
    set_convert_optab_handler (sext_optab, DImode, HImode, CODE_FOR_extendhidi2);
  if (HAVE_extendsidi2)
    set_convert_optab_handler (sext_optab, DImode, SImode, CODE_FOR_extendsidi2);
  if (HAVE_clzsi2)
    set_optab_handler (clz_optab, SImode, CODE_FOR_clzsi2);
  if (HAVE_muldi3)
    set_optab_handler (smul_optab, DImode, CODE_FOR_muldi3);
  if (HAVE_floatdisf2)
    set_convert_optab_handler (sfloat_optab, SFmode, DImode, CODE_FOR_floatdisf2);
  if (HAVE_floatdidf2)
    set_convert_optab_handler (sfloat_optab, DFmode, DImode, CODE_FOR_floatdidf2);
  if (HAVE_smulv4hi3_highpart)
    set_optab_handler (smul_highpart_optab, V4HImode, CODE_FOR_smulv4hi3_highpart);
  if (HAVE_umulv4hi3_highpart)
    set_optab_handler (umul_highpart_optab, V4HImode, CODE_FOR_umulv4hi3_highpart);
  if (HAVE_extendhfsf2)
    set_convert_optab_handler (sext_optab, SFmode, HFmode, CODE_FOR_extendhfsf2);
  if (HAVE_truncsfhf2)
    set_convert_optab_handler (trunc_optab, HFmode, SFmode, CODE_FOR_truncsfhf2);
  if (HAVE_fixuns_truncsfsi2)
    set_convert_optab_handler (ufixtrunc_optab, SImode, SFmode, CODE_FOR_fixuns_truncsfsi2);
  if (HAVE_fixuns_truncdfsi2)
    set_convert_optab_handler (ufixtrunc_optab, SImode, DFmode, CODE_FOR_fixuns_truncdfsi2);
  if (HAVE_floatunssisf2)
    set_convert_optab_handler (ufloat_optab, SFmode, SImode, CODE_FOR_floatunssisf2);
  if (HAVE_floatunssidf2)
    set_convert_optab_handler (ufloat_optab, DFmode, SImode, CODE_FOR_floatunssidf2);
  if (HAVE_vec_extractv8qi)
    set_optab_handler (vec_extract_optab, V8QImode, CODE_FOR_vec_extractv8qi);
  if (HAVE_vec_extractv4hi)
    set_optab_handler (vec_extract_optab, V4HImode, CODE_FOR_vec_extractv4hi);
  if (HAVE_vec_extractv2si)
    set_optab_handler (vec_extract_optab, V2SImode, CODE_FOR_vec_extractv2si);
  if (HAVE_vec_extractv2sf)
    set_optab_handler (vec_extract_optab, V2SFmode, CODE_FOR_vec_extractv2sf);
  if (HAVE_vec_extractv16qi)
    set_optab_handler (vec_extract_optab, V16QImode, CODE_FOR_vec_extractv16qi);
  if (HAVE_vec_extractv8hi)
    set_optab_handler (vec_extract_optab, V8HImode, CODE_FOR_vec_extractv8hi);
  if (HAVE_vec_extractv4si)
    set_optab_handler (vec_extract_optab, V4SImode, CODE_FOR_vec_extractv4si);
  if (HAVE_vec_extractv4sf)
    set_optab_handler (vec_extract_optab, V4SFmode, CODE_FOR_vec_extractv4sf);
  if (HAVE_vec_extractv2di)
    set_optab_handler (vec_extract_optab, V2DImode, CODE_FOR_vec_extractv2di);
  if (HAVE_iorv8qi3)
    set_optab_handler (ior_optab, V8QImode, CODE_FOR_iorv8qi3);
  if (HAVE_iorv16qi3)
    set_optab_handler (ior_optab, V16QImode, CODE_FOR_iorv16qi3);
  if (HAVE_iorv4hi3)
    set_optab_handler (ior_optab, V4HImode, CODE_FOR_iorv4hi3);
  if (HAVE_iorv8hi3)
    set_optab_handler (ior_optab, V8HImode, CODE_FOR_iorv8hi3);
  if (HAVE_iorv2si3)
    set_optab_handler (ior_optab, V2SImode, CODE_FOR_iorv2si3);
  if (HAVE_iorv4si3)
    set_optab_handler (ior_optab, V4SImode, CODE_FOR_iorv4si3);
  if (HAVE_iorv2sf3)
    set_optab_handler (ior_optab, V2SFmode, CODE_FOR_iorv2sf3);
  if (HAVE_iorv4sf3)
    set_optab_handler (ior_optab, V4SFmode, CODE_FOR_iorv4sf3);
  if (HAVE_iorv2di3)
    set_optab_handler (ior_optab, V2DImode, CODE_FOR_iorv2di3);
  if (HAVE_andv8qi3)
    set_optab_handler (and_optab, V8QImode, CODE_FOR_andv8qi3);
  if (HAVE_andv16qi3)
    set_optab_handler (and_optab, V16QImode, CODE_FOR_andv16qi3);
  if (HAVE_andv4hi3)
    set_optab_handler (and_optab, V4HImode, CODE_FOR_andv4hi3);
  if (HAVE_andv8hi3)
    set_optab_handler (and_optab, V8HImode, CODE_FOR_andv8hi3);
  if (HAVE_andv2si3)
    set_optab_handler (and_optab, V2SImode, CODE_FOR_andv2si3);
  if (HAVE_andv4si3)
    set_optab_handler (and_optab, V4SImode, CODE_FOR_andv4si3);
  if (HAVE_andv2sf3)
    set_optab_handler (and_optab, V2SFmode, CODE_FOR_andv2sf3);
  if (HAVE_andv4sf3)
    set_optab_handler (and_optab, V4SFmode, CODE_FOR_andv4sf3);
  if (HAVE_andv2di3)
    set_optab_handler (and_optab, V2DImode, CODE_FOR_andv2di3);
  if (HAVE_xorv8qi3)
    set_optab_handler (xor_optab, V8QImode, CODE_FOR_xorv8qi3);
  if (HAVE_xorv16qi3)
    set_optab_handler (xor_optab, V16QImode, CODE_FOR_xorv16qi3);
  if (HAVE_xorv4hi3)
    set_optab_handler (xor_optab, V4HImode, CODE_FOR_xorv4hi3);
  if (HAVE_xorv8hi3)
    set_optab_handler (xor_optab, V8HImode, CODE_FOR_xorv8hi3);
  if (HAVE_xorv2si3)
    set_optab_handler (xor_optab, V2SImode, CODE_FOR_xorv2si3);
  if (HAVE_xorv4si3)
    set_optab_handler (xor_optab, V4SImode, CODE_FOR_xorv4si3);
  if (HAVE_xorv2sf3)
    set_optab_handler (xor_optab, V2SFmode, CODE_FOR_xorv2sf3);
  if (HAVE_xorv4sf3)
    set_optab_handler (xor_optab, V4SFmode, CODE_FOR_xorv4sf3);
  if (HAVE_xorv2di3)
    set_optab_handler (xor_optab, V2DImode, CODE_FOR_xorv2di3);
  if (HAVE_one_cmplv8qi2)
    set_optab_handler (one_cmpl_optab, V8QImode, CODE_FOR_one_cmplv8qi2);
  if (HAVE_one_cmplv16qi2)
    set_optab_handler (one_cmpl_optab, V16QImode, CODE_FOR_one_cmplv16qi2);
  if (HAVE_one_cmplv4hi2)
    set_optab_handler (one_cmpl_optab, V4HImode, CODE_FOR_one_cmplv4hi2);
  if (HAVE_one_cmplv8hi2)
    set_optab_handler (one_cmpl_optab, V8HImode, CODE_FOR_one_cmplv8hi2);
  if (HAVE_one_cmplv2si2)
    set_optab_handler (one_cmpl_optab, V2SImode, CODE_FOR_one_cmplv2si2);
  if (HAVE_one_cmplv4si2)
    set_optab_handler (one_cmpl_optab, V4SImode, CODE_FOR_one_cmplv4si2);
  if (HAVE_one_cmplv2sf2)
    set_optab_handler (one_cmpl_optab, V2SFmode, CODE_FOR_one_cmplv2sf2);
  if (HAVE_one_cmplv4sf2)
    set_optab_handler (one_cmpl_optab, V4SFmode, CODE_FOR_one_cmplv4sf2);
  if (HAVE_one_cmplv2di2)
    set_optab_handler (one_cmpl_optab, V2DImode, CODE_FOR_one_cmplv2di2);
  if (HAVE_absv8qi2)
    set_optab_handler (abs_optab, V8QImode, CODE_FOR_absv8qi2);
  if (HAVE_absv16qi2)
    set_optab_handler (abs_optab, V16QImode, CODE_FOR_absv16qi2);
  if (HAVE_absv4hi2)
    set_optab_handler (abs_optab, V4HImode, CODE_FOR_absv4hi2);
  if (HAVE_absv8hi2)
    set_optab_handler (abs_optab, V8HImode, CODE_FOR_absv8hi2);
  if (HAVE_absv2si2)
    set_optab_handler (abs_optab, V2SImode, CODE_FOR_absv2si2);
  if (HAVE_absv4si2)
    set_optab_handler (abs_optab, V4SImode, CODE_FOR_absv4si2);
  if (HAVE_absv2sf2)
    set_optab_handler (absv_optab, V2SFmode, CODE_FOR_absv2sf2),
    set_optab_handler (abs_optab, V2SFmode, CODE_FOR_absv2sf2);
  if (HAVE_absv4sf2)
    set_optab_handler (absv_optab, V4SFmode, CODE_FOR_absv4sf2),
    set_optab_handler (abs_optab, V4SFmode, CODE_FOR_absv4sf2);
  if (HAVE_negv8qi2)
    set_optab_handler (neg_optab, V8QImode, CODE_FOR_negv8qi2);
  if (HAVE_negv16qi2)
    set_optab_handler (neg_optab, V16QImode, CODE_FOR_negv16qi2);
  if (HAVE_negv4hi2)
    set_optab_handler (neg_optab, V4HImode, CODE_FOR_negv4hi2);
  if (HAVE_negv8hi2)
    set_optab_handler (neg_optab, V8HImode, CODE_FOR_negv8hi2);
  if (HAVE_negv2si2)
    set_optab_handler (neg_optab, V2SImode, CODE_FOR_negv2si2);
  if (HAVE_negv4si2)
    set_optab_handler (neg_optab, V4SImode, CODE_FOR_negv4si2);
  if (HAVE_negv2sf2)
    set_optab_handler (negv_optab, V2SFmode, CODE_FOR_negv2sf2),
    set_optab_handler (neg_optab, V2SFmode, CODE_FOR_negv2sf2);
  if (HAVE_negv4sf2)
    set_optab_handler (negv_optab, V4SFmode, CODE_FOR_negv4sf2),
    set_optab_handler (neg_optab, V4SFmode, CODE_FOR_negv4sf2);
  if (HAVE_vashlv8qi3)
    set_optab_handler (vashl_optab, V8QImode, CODE_FOR_vashlv8qi3);
  if (HAVE_vashlv16qi3)
    set_optab_handler (vashl_optab, V16QImode, CODE_FOR_vashlv16qi3);
  if (HAVE_vashlv4hi3)
    set_optab_handler (vashl_optab, V4HImode, CODE_FOR_vashlv4hi3);
  if (HAVE_vashlv8hi3)
    set_optab_handler (vashl_optab, V8HImode, CODE_FOR_vashlv8hi3);
  if (HAVE_vashlv2si3)
    set_optab_handler (vashl_optab, V2SImode, CODE_FOR_vashlv2si3);
  if (HAVE_vashlv4si3)
    set_optab_handler (vashl_optab, V4SImode, CODE_FOR_vashlv4si3);
  if (HAVE_widen_ssumv8qi3)
    set_optab_handler (ssum_widen_optab, V8QImode, CODE_FOR_widen_ssumv8qi3);
  if (HAVE_widen_ssumv4hi3)
    set_optab_handler (ssum_widen_optab, V4HImode, CODE_FOR_widen_ssumv4hi3);
  if (HAVE_widen_ssumv2si3)
    set_optab_handler (ssum_widen_optab, V2SImode, CODE_FOR_widen_ssumv2si3);
  if (HAVE_widen_usumv8qi3)
    set_optab_handler (usum_widen_optab, V8QImode, CODE_FOR_widen_usumv8qi3);
  if (HAVE_widen_usumv4hi3)
    set_optab_handler (usum_widen_optab, V4HImode, CODE_FOR_widen_usumv4hi3);
  if (HAVE_widen_usumv2si3)
    set_optab_handler (usum_widen_optab, V2SImode, CODE_FOR_widen_usumv2si3);
  if (HAVE_reduc_splus_v2di)
    set_optab_handler (reduc_splus_optab, V2DImode, CODE_FOR_reduc_splus_v2di);
  if (HAVE_clzv8qi2)
    set_optab_handler (clz_optab, V8QImode, CODE_FOR_clzv8qi2);
  if (HAVE_clzv16qi2)
    set_optab_handler (clz_optab, V16QImode, CODE_FOR_clzv16qi2);
  if (HAVE_clzv4hi2)
    set_optab_handler (clz_optab, V4HImode, CODE_FOR_clzv4hi2);
  if (HAVE_clzv8hi2)
    set_optab_handler (clz_optab, V8HImode, CODE_FOR_clzv8hi2);
  if (HAVE_clzv2si2)
    set_optab_handler (clz_optab, V2SImode, CODE_FOR_clzv2si2);
  if (HAVE_clzv4si2)
    set_optab_handler (clz_optab, V4SImode, CODE_FOR_clzv4si2);
  if (HAVE_popcountv8qi2)
    set_optab_handler (popcount_optab, V8QImode, CODE_FOR_popcountv8qi2);
  if (HAVE_popcountv16qi2)
    set_optab_handler (popcount_optab, V16QImode, CODE_FOR_popcountv16qi2);
  if (HAVE_vec_pack_trunc_v8hi)
    set_optab_handler (vec_pack_trunc_optab, V8HImode, CODE_FOR_vec_pack_trunc_v8hi);
  if (HAVE_vec_pack_trunc_v4si)
    set_optab_handler (vec_pack_trunc_optab, V4SImode, CODE_FOR_vec_pack_trunc_v4si);
  if (HAVE_vec_pack_trunc_v2di)
    set_optab_handler (vec_pack_trunc_optab, V2DImode, CODE_FOR_vec_pack_trunc_v2di);
  if (HAVE_adddi3)
    set_optab_handler (add_optab, DImode, CODE_FOR_adddi3);
  if (HAVE_addsi3)
    set_optab_handler (add_optab, SImode, CODE_FOR_addsi3);
  if (HAVE_addsf3)
    set_optab_handler (addv_optab, SFmode, CODE_FOR_addsf3),
    set_optab_handler (add_optab, SFmode, CODE_FOR_addsf3);
  if (HAVE_adddf3)
    set_optab_handler (addv_optab, DFmode, CODE_FOR_adddf3),
    set_optab_handler (add_optab, DFmode, CODE_FOR_adddf3);
  if (HAVE_subdi3)
    set_optab_handler (sub_optab, DImode, CODE_FOR_subdi3);
  if (HAVE_subsi3)
    set_optab_handler (sub_optab, SImode, CODE_FOR_subsi3);
  if (HAVE_subsf3)
    set_optab_handler (subv_optab, SFmode, CODE_FOR_subsf3),
    set_optab_handler (sub_optab, SFmode, CODE_FOR_subsf3);
  if (HAVE_subdf3)
    set_optab_handler (subv_optab, DFmode, CODE_FOR_subdf3),
    set_optab_handler (sub_optab, DFmode, CODE_FOR_subdf3);
  if (HAVE_mulsi3)
    set_optab_handler (smul_optab, SImode, CODE_FOR_mulsi3);
  if (HAVE_maddsidi4)
    set_widening_optab_handler (smadd_widen_optab, DImode, SImode, CODE_FOR_maddsidi4);
  if (HAVE_mulsidi3)
    set_widening_optab_handler (smul_widen_optab, DImode, SImode, CODE_FOR_mulsidi3);
  if (HAVE_umulsidi3)
    set_widening_optab_handler (umul_widen_optab, DImode, SImode, CODE_FOR_umulsidi3);
  if (HAVE_umaddsidi4)
    set_widening_optab_handler (umadd_widen_optab, DImode, SImode, CODE_FOR_umaddsidi4);
  if (HAVE_smulsi3_highpart)
    set_optab_handler (smul_highpart_optab, SImode, CODE_FOR_smulsi3_highpart);
  if (HAVE_umulsi3_highpart)
    set_optab_handler (umul_highpart_optab, SImode, CODE_FOR_umulsi3_highpart);
  if (HAVE_mulsf3)
    set_optab_handler (smulv_optab, SFmode, CODE_FOR_mulsf3),
    set_optab_handler (smul_optab, SFmode, CODE_FOR_mulsf3);
  if (HAVE_muldf3)
    set_optab_handler (smulv_optab, DFmode, CODE_FOR_muldf3),
    set_optab_handler (smul_optab, DFmode, CODE_FOR_muldf3);
  if (HAVE_divsf3)
    set_optab_handler (sdiv_optab, SFmode, CODE_FOR_divsf3);
  if (HAVE_divdf3)
    set_optab_handler (sdiv_optab, DFmode, CODE_FOR_divdf3);
  if (HAVE_modsf3)
    set_optab_handler (smod_optab, SFmode, CODE_FOR_modsf3);
  if (HAVE_moddf3)
    set_optab_handler (smod_optab, DFmode, CODE_FOR_moddf3);
  if (HAVE_anddi3)
    set_optab_handler (and_optab, DImode, CODE_FOR_anddi3);
  if (HAVE_andsi3)
    set_optab_handler (and_optab, SImode, CODE_FOR_andsi3);
  if (HAVE_iordi3)
    set_optab_handler (ior_optab, DImode, CODE_FOR_iordi3);
  if (HAVE_iorsi3)
    set_optab_handler (ior_optab, SImode, CODE_FOR_iorsi3);
  if (HAVE_xordi3)
    set_optab_handler (xor_optab, DImode, CODE_FOR_xordi3);
  if (HAVE_xorsi3)
    set_optab_handler (xor_optab, SImode, CODE_FOR_xorsi3);
  if (HAVE_smaxsi3)
    set_optab_handler (smax_optab, SImode, CODE_FOR_smaxsi3);
  if (HAVE_sminsi3)
    set_optab_handler (smin_optab, SImode, CODE_FOR_sminsi3);
  if (HAVE_umaxsi3)
    set_optab_handler (umax_optab, SImode, CODE_FOR_umaxsi3);
  if (HAVE_uminsi3)
    set_optab_handler (umin_optab, SImode, CODE_FOR_uminsi3);
  if (HAVE_ashldi3)
    set_optab_handler (ashl_optab, DImode, CODE_FOR_ashldi3);
  if (HAVE_ashlsi3)
    set_optab_handler (ashl_optab, SImode, CODE_FOR_ashlsi3);
  if (HAVE_ashrdi3)
    set_optab_handler (ashr_optab, DImode, CODE_FOR_ashrdi3);
  if (HAVE_ashrsi3)
    set_optab_handler (ashr_optab, SImode, CODE_FOR_ashrsi3);
  if (HAVE_lshrdi3)
    set_optab_handler (lshr_optab, DImode, CODE_FOR_lshrdi3);
  if (HAVE_lshrsi3)
    set_optab_handler (lshr_optab, SImode, CODE_FOR_lshrsi3);
  if (HAVE_rotlsi3)
    set_optab_handler (rotl_optab, SImode, CODE_FOR_rotlsi3);
  if (HAVE_rotrsi3)
    set_optab_handler (rotr_optab, SImode, CODE_FOR_rotrsi3);
  if (HAVE_negdi2)
    set_optab_handler (neg_optab, DImode, CODE_FOR_negdi2);
  if (HAVE_negsi2)
    set_optab_handler (neg_optab, SImode, CODE_FOR_negsi2);
  if (HAVE_negsf2)
    set_optab_handler (negv_optab, SFmode, CODE_FOR_negsf2),
    set_optab_handler (neg_optab, SFmode, CODE_FOR_negsf2);
  if (HAVE_negdf2)
    set_optab_handler (negv_optab, DFmode, CODE_FOR_negdf2),
    set_optab_handler (neg_optab, DFmode, CODE_FOR_negdf2);
  if (HAVE_abssi2)
    set_optab_handler (abs_optab, SImode, CODE_FOR_abssi2);
  if (HAVE_abssf2)
    set_optab_handler (absv_optab, SFmode, CODE_FOR_abssf2),
    set_optab_handler (abs_optab, SFmode, CODE_FOR_abssf2);
  if (HAVE_absdf2)
    set_optab_handler (absv_optab, DFmode, CODE_FOR_absdf2),
    set_optab_handler (abs_optab, DFmode, CODE_FOR_absdf2);
  if (HAVE_sqrtsf2)
    set_optab_handler (sqrt_optab, SFmode, CODE_FOR_sqrtsf2);
  if (HAVE_sqrtdf2)
    set_optab_handler (sqrt_optab, DFmode, CODE_FOR_sqrtdf2);
  if (HAVE_one_cmplsi2)
    set_optab_handler (one_cmpl_optab, SImode, CODE_FOR_one_cmplsi2);
  if (HAVE_floatsihf2)
    set_convert_optab_handler (sfloat_optab, HFmode, SImode, CODE_FOR_floatsihf2);
  if (HAVE_floatdihf2)
    set_convert_optab_handler (sfloat_optab, HFmode, DImode, CODE_FOR_floatdihf2);
  if (HAVE_floatsisf2)
    set_convert_optab_handler (sfloat_optab, SFmode, SImode, CODE_FOR_floatsisf2);
  if (HAVE_floatsidf2)
    set_convert_optab_handler (sfloat_optab, DFmode, SImode, CODE_FOR_floatsidf2);
  if (HAVE_fix_trunchfsi2)
    set_convert_optab_handler (sfixtrunc_optab, SImode, HFmode, CODE_FOR_fix_trunchfsi2);
  if (HAVE_fix_trunchfdi2)
    set_convert_optab_handler (sfixtrunc_optab, DImode, HFmode, CODE_FOR_fix_trunchfdi2);
  if (HAVE_fix_truncsfsi2)
    set_convert_optab_handler (sfixtrunc_optab, SImode, SFmode, CODE_FOR_fix_truncsfsi2);
  if (HAVE_fix_truncdfsi2)
    set_convert_optab_handler (sfixtrunc_optab, SImode, DFmode, CODE_FOR_fix_truncdfsi2);
  if (HAVE_truncdfsf2)
    set_convert_optab_handler (trunc_optab, SFmode, DFmode, CODE_FOR_truncdfsf2);
  if (HAVE_truncdfhf2)
    set_convert_optab_handler (trunc_optab, HFmode, DFmode, CODE_FOR_truncdfhf2);
  if (HAVE_zero_extendhisi2)
    set_convert_optab_handler (zext_optab, SImode, HImode, CODE_FOR_zero_extendhisi2);
  if (HAVE_zero_extendqisi2)
    set_convert_optab_handler (zext_optab, SImode, QImode, CODE_FOR_zero_extendqisi2);
  if (HAVE_extendhisi2)
    set_convert_optab_handler (sext_optab, SImode, HImode, CODE_FOR_extendhisi2);
  if (HAVE_extendqihi2)
    set_convert_optab_handler (sext_optab, HImode, QImode, CODE_FOR_extendqihi2);
  if (HAVE_extendqisi2)
    set_convert_optab_handler (sext_optab, SImode, QImode, CODE_FOR_extendqisi2);
  if (HAVE_extendsfdf2)
    set_convert_optab_handler (sext_optab, DFmode, SFmode, CODE_FOR_extendsfdf2);
  if (HAVE_extendhfdf2)
    set_convert_optab_handler (sext_optab, DFmode, HFmode, CODE_FOR_extendhfdf2);
  if (HAVE_movdi)
    set_optab_handler (mov_optab, DImode, CODE_FOR_movdi);
  if (HAVE_movsi)
    set_optab_handler (mov_optab, SImode, CODE_FOR_movsi);
  if (HAVE_movhi)
    set_optab_handler (mov_optab, HImode, CODE_FOR_movhi);
  if (HAVE_reload_outhi)
    set_direct_optab_handler (reload_out_optab, HImode, CODE_FOR_reload_outhi);
  if (HAVE_reload_inhi)
    set_direct_optab_handler (reload_in_optab, HImode, CODE_FOR_reload_inhi);
  if (HAVE_movqi)
    set_optab_handler (mov_optab, QImode, CODE_FOR_movqi);
  if (HAVE_movhf)
    set_optab_handler (mov_optab, HFmode, CODE_FOR_movhf);
  if (HAVE_movsf)
    set_optab_handler (mov_optab, SFmode, CODE_FOR_movsf);
  if (HAVE_movdf)
    set_optab_handler (mov_optab, DFmode, CODE_FOR_movdf);
  if (HAVE_reload_outdf)
    set_direct_optab_handler (reload_out_optab, DFmode, CODE_FOR_reload_outdf);
  if (HAVE_movxf)
    set_optab_handler (mov_optab, XFmode, CODE_FOR_movxf);
  if (HAVE_movmemqi)
    set_direct_optab_handler (movmem_optab, QImode, CODE_FOR_movmemqi);
  if (HAVE_cbranchsi4)
    set_optab_handler (cbranch_optab, SImode, CODE_FOR_cbranchsi4);
  if (HAVE_cbranchqi4)
    set_optab_handler (cbranch_optab, QImode, CODE_FOR_cbranchqi4);
  if (HAVE_cbranchsf4)
    set_optab_handler (cbranch_optab, SFmode, CODE_FOR_cbranchsf4);
  if (HAVE_cbranchdf4)
    set_optab_handler (cbranch_optab, DFmode, CODE_FOR_cbranchdf4);
  if (HAVE_cbranchdi4)
    set_optab_handler (cbranch_optab, DImode, CODE_FOR_cbranchdi4);
  if (HAVE_cstoresi4)
    set_optab_handler (cstore_optab, SImode, CODE_FOR_cstoresi4);
  if (HAVE_cstoresf4)
    set_optab_handler (cstore_optab, SFmode, CODE_FOR_cstoresf4);
  if (HAVE_cstoredf4)
    set_optab_handler (cstore_optab, DFmode, CODE_FOR_cstoredf4);
  if (HAVE_cstoredi4)
    set_optab_handler (cstore_optab, DImode, CODE_FOR_cstoredi4);
  if (HAVE_movsicc)
    set_direct_optab_handler (movcc_optab, SImode, CODE_FOR_movsicc);
  if (HAVE_movsfcc)
    set_direct_optab_handler (movcc_optab, SFmode, CODE_FOR_movsfcc);
  if (HAVE_movdfcc)
    set_direct_optab_handler (movcc_optab, DFmode, CODE_FOR_movdfcc);
  if (HAVE_ctzsi2)
    set_optab_handler (ctz_optab, SImode, CODE_FOR_ctzsi2);
  if (HAVE_bswapsi2)
    set_optab_handler (bswap_optab, SImode, CODE_FOR_bswapsi2);
  if (HAVE_movv2di)
    set_optab_handler (mov_optab, V2DImode, CODE_FOR_movv2di);
  if (HAVE_movv2si)
    set_optab_handler (mov_optab, V2SImode, CODE_FOR_movv2si);
  if (HAVE_movv4hi)
    set_optab_handler (mov_optab, V4HImode, CODE_FOR_movv4hi);
  if (HAVE_movv8qi)
    set_optab_handler (mov_optab, V8QImode, CODE_FOR_movv8qi);
  if (HAVE_movv2sf)
    set_optab_handler (mov_optab, V2SFmode, CODE_FOR_movv2sf);
  if (HAVE_movv4si)
    set_optab_handler (mov_optab, V4SImode, CODE_FOR_movv4si);
  if (HAVE_movv8hi)
    set_optab_handler (mov_optab, V8HImode, CODE_FOR_movv8hi);
  if (HAVE_movv16qi)
    set_optab_handler (mov_optab, V16QImode, CODE_FOR_movv16qi);
  if (HAVE_movv4sf)
    set_optab_handler (mov_optab, V4SFmode, CODE_FOR_movv4sf);
  if (HAVE_addv2di3)
    set_optab_handler (add_optab, V2DImode, CODE_FOR_addv2di3);
  if (HAVE_addv2si3)
    set_optab_handler (add_optab, V2SImode, CODE_FOR_addv2si3);
  if (HAVE_addv4hi3)
    set_optab_handler (add_optab, V4HImode, CODE_FOR_addv4hi3);
  if (HAVE_addv8qi3)
    set_optab_handler (add_optab, V8QImode, CODE_FOR_addv8qi3);
  if (HAVE_addv2sf3)
    set_optab_handler (addv_optab, V2SFmode, CODE_FOR_addv2sf3),
    set_optab_handler (add_optab, V2SFmode, CODE_FOR_addv2sf3);
  if (HAVE_addv4si3)
    set_optab_handler (add_optab, V4SImode, CODE_FOR_addv4si3);
  if (HAVE_addv8hi3)
    set_optab_handler (add_optab, V8HImode, CODE_FOR_addv8hi3);
  if (HAVE_addv16qi3)
    set_optab_handler (add_optab, V16QImode, CODE_FOR_addv16qi3);
  if (HAVE_addv4sf3)
    set_optab_handler (addv_optab, V4SFmode, CODE_FOR_addv4sf3),
    set_optab_handler (add_optab, V4SFmode, CODE_FOR_addv4sf3);
  if (HAVE_subv2di3)
    set_optab_handler (sub_optab, V2DImode, CODE_FOR_subv2di3);
  if (HAVE_subv2si3)
    set_optab_handler (sub_optab, V2SImode, CODE_FOR_subv2si3);
  if (HAVE_subv4hi3)
    set_optab_handler (sub_optab, V4HImode, CODE_FOR_subv4hi3);
  if (HAVE_subv8qi3)
    set_optab_handler (sub_optab, V8QImode, CODE_FOR_subv8qi3);
  if (HAVE_subv2sf3)
    set_optab_handler (subv_optab, V2SFmode, CODE_FOR_subv2sf3),
    set_optab_handler (sub_optab, V2SFmode, CODE_FOR_subv2sf3);
  if (HAVE_subv4si3)
    set_optab_handler (sub_optab, V4SImode, CODE_FOR_subv4si3);
  if (HAVE_subv8hi3)
    set_optab_handler (sub_optab, V8HImode, CODE_FOR_subv8hi3);
  if (HAVE_subv16qi3)
    set_optab_handler (sub_optab, V16QImode, CODE_FOR_subv16qi3);
  if (HAVE_subv4sf3)
    set_optab_handler (subv_optab, V4SFmode, CODE_FOR_subv4sf3),
    set_optab_handler (sub_optab, V4SFmode, CODE_FOR_subv4sf3);
  if (HAVE_mulv2si3)
    set_optab_handler (smul_optab, V2SImode, CODE_FOR_mulv2si3);
  if (HAVE_mulv4hi3)
    set_optab_handler (smul_optab, V4HImode, CODE_FOR_mulv4hi3);
  if (HAVE_mulv8qi3)
    set_optab_handler (smul_optab, V8QImode, CODE_FOR_mulv8qi3);
  if (HAVE_mulv2sf3)
    set_optab_handler (smulv_optab, V2SFmode, CODE_FOR_mulv2sf3),
    set_optab_handler (smul_optab, V2SFmode, CODE_FOR_mulv2sf3);
  if (HAVE_mulv4si3)
    set_optab_handler (smul_optab, V4SImode, CODE_FOR_mulv4si3);
  if (HAVE_mulv8hi3)
    set_optab_handler (smul_optab, V8HImode, CODE_FOR_mulv8hi3);
  if (HAVE_mulv16qi3)
    set_optab_handler (smul_optab, V16QImode, CODE_FOR_mulv16qi3);
  if (HAVE_mulv4sf3)
    set_optab_handler (smulv_optab, V4SFmode, CODE_FOR_mulv4sf3),
    set_optab_handler (smul_optab, V4SFmode, CODE_FOR_mulv4sf3);
  if (HAVE_sminv2si3)
    set_optab_handler (smin_optab, V2SImode, CODE_FOR_sminv2si3);
  if (HAVE_sminv4hi3)
    set_optab_handler (smin_optab, V4HImode, CODE_FOR_sminv4hi3);
  if (HAVE_sminv8qi3)
    set_optab_handler (smin_optab, V8QImode, CODE_FOR_sminv8qi3);
  if (HAVE_sminv2sf3)
    set_optab_handler (smin_optab, V2SFmode, CODE_FOR_sminv2sf3);
  if (HAVE_sminv4si3)
    set_optab_handler (smin_optab, V4SImode, CODE_FOR_sminv4si3);
  if (HAVE_sminv8hi3)
    set_optab_handler (smin_optab, V8HImode, CODE_FOR_sminv8hi3);
  if (HAVE_sminv16qi3)
    set_optab_handler (smin_optab, V16QImode, CODE_FOR_sminv16qi3);
  if (HAVE_sminv4sf3)
    set_optab_handler (smin_optab, V4SFmode, CODE_FOR_sminv4sf3);
  if (HAVE_uminv2si3)
    set_optab_handler (umin_optab, V2SImode, CODE_FOR_uminv2si3);
  if (HAVE_uminv4hi3)
    set_optab_handler (umin_optab, V4HImode, CODE_FOR_uminv4hi3);
  if (HAVE_uminv8qi3)
    set_optab_handler (umin_optab, V8QImode, CODE_FOR_uminv8qi3);
  if (HAVE_uminv4si3)
    set_optab_handler (umin_optab, V4SImode, CODE_FOR_uminv4si3);
  if (HAVE_uminv8hi3)
    set_optab_handler (umin_optab, V8HImode, CODE_FOR_uminv8hi3);
  if (HAVE_uminv16qi3)
    set_optab_handler (umin_optab, V16QImode, CODE_FOR_uminv16qi3);
  if (HAVE_smaxv2si3)
    set_optab_handler (smax_optab, V2SImode, CODE_FOR_smaxv2si3);
  if (HAVE_smaxv4hi3)
    set_optab_handler (smax_optab, V4HImode, CODE_FOR_smaxv4hi3);
  if (HAVE_smaxv8qi3)
    set_optab_handler (smax_optab, V8QImode, CODE_FOR_smaxv8qi3);
  if (HAVE_smaxv2sf3)
    set_optab_handler (smax_optab, V2SFmode, CODE_FOR_smaxv2sf3);
  if (HAVE_smaxv4si3)
    set_optab_handler (smax_optab, V4SImode, CODE_FOR_smaxv4si3);
  if (HAVE_smaxv8hi3)
    set_optab_handler (smax_optab, V8HImode, CODE_FOR_smaxv8hi3);
  if (HAVE_smaxv16qi3)
    set_optab_handler (smax_optab, V16QImode, CODE_FOR_smaxv16qi3);
  if (HAVE_smaxv4sf3)
    set_optab_handler (smax_optab, V4SFmode, CODE_FOR_smaxv4sf3);
  if (HAVE_umaxv2si3)
    set_optab_handler (umax_optab, V2SImode, CODE_FOR_umaxv2si3);
  if (HAVE_umaxv4hi3)
    set_optab_handler (umax_optab, V4HImode, CODE_FOR_umaxv4hi3);
  if (HAVE_umaxv8qi3)
    set_optab_handler (umax_optab, V8QImode, CODE_FOR_umaxv8qi3);
  if (HAVE_umaxv4si3)
    set_optab_handler (umax_optab, V4SImode, CODE_FOR_umaxv4si3);
  if (HAVE_umaxv8hi3)
    set_optab_handler (umax_optab, V8HImode, CODE_FOR_umaxv8hi3);
  if (HAVE_umaxv16qi3)
    set_optab_handler (umax_optab, V16QImode, CODE_FOR_umaxv16qi3);
  if (HAVE_movti)
    set_optab_handler (mov_optab, TImode, CODE_FOR_movti);
  if (HAVE_movei)
    set_optab_handler (mov_optab, EImode, CODE_FOR_movei);
  if (HAVE_movoi)
    set_optab_handler (mov_optab, OImode, CODE_FOR_movoi);
  if (HAVE_movci)
    set_optab_handler (mov_optab, CImode, CODE_FOR_movci);
  if (HAVE_movxi)
    set_optab_handler (mov_optab, XImode, CODE_FOR_movxi);
  if (HAVE_movmisalignv8qi)
    set_optab_handler (movmisalign_optab, V8QImode, CODE_FOR_movmisalignv8qi);
  if (HAVE_movmisalignv16qi)
    set_optab_handler (movmisalign_optab, V16QImode, CODE_FOR_movmisalignv16qi);
  if (HAVE_movmisalignv4hi)
    set_optab_handler (movmisalign_optab, V4HImode, CODE_FOR_movmisalignv4hi);
  if (HAVE_movmisalignv8hi)
    set_optab_handler (movmisalign_optab, V8HImode, CODE_FOR_movmisalignv8hi);
  if (HAVE_movmisalignv2si)
    set_optab_handler (movmisalign_optab, V2SImode, CODE_FOR_movmisalignv2si);
  if (HAVE_movmisalignv4si)
    set_optab_handler (movmisalign_optab, V4SImode, CODE_FOR_movmisalignv4si);
  if (HAVE_movmisalignv2sf)
    set_optab_handler (movmisalign_optab, V2SFmode, CODE_FOR_movmisalignv2sf);
  if (HAVE_movmisalignv4sf)
    set_optab_handler (movmisalign_optab, V4SFmode, CODE_FOR_movmisalignv4sf);
  if (HAVE_movmisaligndi)
    set_optab_handler (movmisalign_optab, DImode, CODE_FOR_movmisaligndi);
  if (HAVE_movmisalignv2di)
    set_optab_handler (movmisalign_optab, V2DImode, CODE_FOR_movmisalignv2di);
  if (HAVE_vec_setv8qi)
    set_optab_handler (vec_set_optab, V8QImode, CODE_FOR_vec_setv8qi);
  if (HAVE_vec_setv16qi)
    set_optab_handler (vec_set_optab, V16QImode, CODE_FOR_vec_setv16qi);
  if (HAVE_vec_setv4hi)
    set_optab_handler (vec_set_optab, V4HImode, CODE_FOR_vec_setv4hi);
  if (HAVE_vec_setv8hi)
    set_optab_handler (vec_set_optab, V8HImode, CODE_FOR_vec_setv8hi);
  if (HAVE_vec_setv2si)
    set_optab_handler (vec_set_optab, V2SImode, CODE_FOR_vec_setv2si);
  if (HAVE_vec_setv4si)
    set_optab_handler (vec_set_optab, V4SImode, CODE_FOR_vec_setv4si);
  if (HAVE_vec_setv2sf)
    set_optab_handler (vec_set_optab, V2SFmode, CODE_FOR_vec_setv2sf);
  if (HAVE_vec_setv4sf)
    set_optab_handler (vec_set_optab, V4SFmode, CODE_FOR_vec_setv4sf);
  if (HAVE_vec_setv2di)
    set_optab_handler (vec_set_optab, V2DImode, CODE_FOR_vec_setv2di);
  if (HAVE_vec_initv8qi)
    set_optab_handler (vec_init_optab, V8QImode, CODE_FOR_vec_initv8qi);
  if (HAVE_vec_initv16qi)
    set_optab_handler (vec_init_optab, V16QImode, CODE_FOR_vec_initv16qi);
  if (HAVE_vec_initv4hi)
    set_optab_handler (vec_init_optab, V4HImode, CODE_FOR_vec_initv4hi);
  if (HAVE_vec_initv8hi)
    set_optab_handler (vec_init_optab, V8HImode, CODE_FOR_vec_initv8hi);
  if (HAVE_vec_initv2si)
    set_optab_handler (vec_init_optab, V2SImode, CODE_FOR_vec_initv2si);
  if (HAVE_vec_initv4si)
    set_optab_handler (vec_init_optab, V4SImode, CODE_FOR_vec_initv4si);
  if (HAVE_vec_initv2sf)
    set_optab_handler (vec_init_optab, V2SFmode, CODE_FOR_vec_initv2sf);
  if (HAVE_vec_initv4sf)
    set_optab_handler (vec_init_optab, V4SFmode, CODE_FOR_vec_initv4sf);
  if (HAVE_vec_initv2di)
    set_optab_handler (vec_init_optab, V2DImode, CODE_FOR_vec_initv2di);
  if (HAVE_vashrv8qi3)
    set_optab_handler (vashr_optab, V8QImode, CODE_FOR_vashrv8qi3);
  if (HAVE_vashrv16qi3)
    set_optab_handler (vashr_optab, V16QImode, CODE_FOR_vashrv16qi3);
  if (HAVE_vashrv4hi3)
    set_optab_handler (vashr_optab, V4HImode, CODE_FOR_vashrv4hi3);
  if (HAVE_vashrv8hi3)
    set_optab_handler (vashr_optab, V8HImode, CODE_FOR_vashrv8hi3);
  if (HAVE_vashrv2si3)
    set_optab_handler (vashr_optab, V2SImode, CODE_FOR_vashrv2si3);
  if (HAVE_vashrv4si3)
    set_optab_handler (vashr_optab, V4SImode, CODE_FOR_vashrv4si3);
  if (HAVE_vlshrv8qi3)
    set_optab_handler (vlshr_optab, V8QImode, CODE_FOR_vlshrv8qi3);
  if (HAVE_vlshrv16qi3)
    set_optab_handler (vlshr_optab, V16QImode, CODE_FOR_vlshrv16qi3);
  if (HAVE_vlshrv4hi3)
    set_optab_handler (vlshr_optab, V4HImode, CODE_FOR_vlshrv4hi3);
  if (HAVE_vlshrv8hi3)
    set_optab_handler (vlshr_optab, V8HImode, CODE_FOR_vlshrv8hi3);
  if (HAVE_vlshrv2si3)
    set_optab_handler (vlshr_optab, V2SImode, CODE_FOR_vlshrv2si3);
  if (HAVE_vlshrv4si3)
    set_optab_handler (vlshr_optab, V4SImode, CODE_FOR_vlshrv4si3);
  if (HAVE_vec_shr_v8qi)
    set_optab_handler (vec_shr_optab, V8QImode, CODE_FOR_vec_shr_v8qi);
  if (HAVE_vec_shr_v16qi)
    set_optab_handler (vec_shr_optab, V16QImode, CODE_FOR_vec_shr_v16qi);
  if (HAVE_vec_shr_v4hi)
    set_optab_handler (vec_shr_optab, V4HImode, CODE_FOR_vec_shr_v4hi);
  if (HAVE_vec_shr_v8hi)
    set_optab_handler (vec_shr_optab, V8HImode, CODE_FOR_vec_shr_v8hi);
  if (HAVE_vec_shr_v2si)
    set_optab_handler (vec_shr_optab, V2SImode, CODE_FOR_vec_shr_v2si);
  if (HAVE_vec_shr_v4si)
    set_optab_handler (vec_shr_optab, V4SImode, CODE_FOR_vec_shr_v4si);
  if (HAVE_vec_shr_v2sf)
    set_optab_handler (vec_shr_optab, V2SFmode, CODE_FOR_vec_shr_v2sf);
  if (HAVE_vec_shr_v4sf)
    set_optab_handler (vec_shr_optab, V4SFmode, CODE_FOR_vec_shr_v4sf);
  if (HAVE_vec_shr_v2di)
    set_optab_handler (vec_shr_optab, V2DImode, CODE_FOR_vec_shr_v2di);
  if (HAVE_vec_shl_v8qi)
    set_optab_handler (vec_shl_optab, V8QImode, CODE_FOR_vec_shl_v8qi);
  if (HAVE_vec_shl_v16qi)
    set_optab_handler (vec_shl_optab, V16QImode, CODE_FOR_vec_shl_v16qi);
  if (HAVE_vec_shl_v4hi)
    set_optab_handler (vec_shl_optab, V4HImode, CODE_FOR_vec_shl_v4hi);
  if (HAVE_vec_shl_v8hi)
    set_optab_handler (vec_shl_optab, V8HImode, CODE_FOR_vec_shl_v8hi);
  if (HAVE_vec_shl_v2si)
    set_optab_handler (vec_shl_optab, V2SImode, CODE_FOR_vec_shl_v2si);
  if (HAVE_vec_shl_v4si)
    set_optab_handler (vec_shl_optab, V4SImode, CODE_FOR_vec_shl_v4si);
  if (HAVE_vec_shl_v2sf)
    set_optab_handler (vec_shl_optab, V2SFmode, CODE_FOR_vec_shl_v2sf);
  if (HAVE_vec_shl_v4sf)
    set_optab_handler (vec_shl_optab, V4SFmode, CODE_FOR_vec_shl_v4sf);
  if (HAVE_vec_shl_v2di)
    set_optab_handler (vec_shl_optab, V2DImode, CODE_FOR_vec_shl_v2di);
  if (HAVE_reduc_splus_v8qi)
    set_optab_handler (reduc_splus_optab, V8QImode, CODE_FOR_reduc_splus_v8qi);
  if (HAVE_reduc_splus_v4hi)
    set_optab_handler (reduc_splus_optab, V4HImode, CODE_FOR_reduc_splus_v4hi);
  if (HAVE_reduc_splus_v2si)
    set_optab_handler (reduc_splus_optab, V2SImode, CODE_FOR_reduc_splus_v2si);
  if (HAVE_reduc_splus_v2sf)
    set_optab_handler (reduc_splus_optab, V2SFmode, CODE_FOR_reduc_splus_v2sf);
  if (HAVE_reduc_splus_v16qi)
    set_optab_handler (reduc_splus_optab, V16QImode, CODE_FOR_reduc_splus_v16qi);
  if (HAVE_reduc_splus_v8hi)
    set_optab_handler (reduc_splus_optab, V8HImode, CODE_FOR_reduc_splus_v8hi);
  if (HAVE_reduc_splus_v4si)
    set_optab_handler (reduc_splus_optab, V4SImode, CODE_FOR_reduc_splus_v4si);
  if (HAVE_reduc_splus_v4sf)
    set_optab_handler (reduc_splus_optab, V4SFmode, CODE_FOR_reduc_splus_v4sf);
  if (HAVE_reduc_uplus_v8qi)
    set_optab_handler (reduc_uplus_optab, V8QImode, CODE_FOR_reduc_uplus_v8qi);
  if (HAVE_reduc_uplus_v16qi)
    set_optab_handler (reduc_uplus_optab, V16QImode, CODE_FOR_reduc_uplus_v16qi);
  if (HAVE_reduc_uplus_v4hi)
    set_optab_handler (reduc_uplus_optab, V4HImode, CODE_FOR_reduc_uplus_v4hi);
  if (HAVE_reduc_uplus_v8hi)
    set_optab_handler (reduc_uplus_optab, V8HImode, CODE_FOR_reduc_uplus_v8hi);
  if (HAVE_reduc_uplus_v2si)
    set_optab_handler (reduc_uplus_optab, V2SImode, CODE_FOR_reduc_uplus_v2si);
  if (HAVE_reduc_uplus_v4si)
    set_optab_handler (reduc_uplus_optab, V4SImode, CODE_FOR_reduc_uplus_v4si);
  if (HAVE_reduc_uplus_v2di)
    set_optab_handler (reduc_uplus_optab, V2DImode, CODE_FOR_reduc_uplus_v2di);
  if (HAVE_reduc_smin_v8qi)
    set_optab_handler (reduc_smin_optab, V8QImode, CODE_FOR_reduc_smin_v8qi);
  if (HAVE_reduc_smin_v4hi)
    set_optab_handler (reduc_smin_optab, V4HImode, CODE_FOR_reduc_smin_v4hi);
  if (HAVE_reduc_smin_v2si)
    set_optab_handler (reduc_smin_optab, V2SImode, CODE_FOR_reduc_smin_v2si);
  if (HAVE_reduc_smin_v2sf)
    set_optab_handler (reduc_smin_optab, V2SFmode, CODE_FOR_reduc_smin_v2sf);
  if (HAVE_reduc_smin_v16qi)
    set_optab_handler (reduc_smin_optab, V16QImode, CODE_FOR_reduc_smin_v16qi);
  if (HAVE_reduc_smin_v8hi)
    set_optab_handler (reduc_smin_optab, V8HImode, CODE_FOR_reduc_smin_v8hi);
  if (HAVE_reduc_smin_v4si)
    set_optab_handler (reduc_smin_optab, V4SImode, CODE_FOR_reduc_smin_v4si);
  if (HAVE_reduc_smin_v4sf)
    set_optab_handler (reduc_smin_optab, V4SFmode, CODE_FOR_reduc_smin_v4sf);
  if (HAVE_reduc_smax_v8qi)
    set_optab_handler (reduc_smax_optab, V8QImode, CODE_FOR_reduc_smax_v8qi);
  if (HAVE_reduc_smax_v4hi)
    set_optab_handler (reduc_smax_optab, V4HImode, CODE_FOR_reduc_smax_v4hi);
  if (HAVE_reduc_smax_v2si)
    set_optab_handler (reduc_smax_optab, V2SImode, CODE_FOR_reduc_smax_v2si);
  if (HAVE_reduc_smax_v2sf)
    set_optab_handler (reduc_smax_optab, V2SFmode, CODE_FOR_reduc_smax_v2sf);
  if (HAVE_reduc_smax_v16qi)
    set_optab_handler (reduc_smax_optab, V16QImode, CODE_FOR_reduc_smax_v16qi);
  if (HAVE_reduc_smax_v8hi)
    set_optab_handler (reduc_smax_optab, V8HImode, CODE_FOR_reduc_smax_v8hi);
  if (HAVE_reduc_smax_v4si)
    set_optab_handler (reduc_smax_optab, V4SImode, CODE_FOR_reduc_smax_v4si);
  if (HAVE_reduc_smax_v4sf)
    set_optab_handler (reduc_smax_optab, V4SFmode, CODE_FOR_reduc_smax_v4sf);
  if (HAVE_reduc_umin_v8qi)
    set_optab_handler (reduc_umin_optab, V8QImode, CODE_FOR_reduc_umin_v8qi);
  if (HAVE_reduc_umin_v4hi)
    set_optab_handler (reduc_umin_optab, V4HImode, CODE_FOR_reduc_umin_v4hi);
  if (HAVE_reduc_umin_v2si)
    set_optab_handler (reduc_umin_optab, V2SImode, CODE_FOR_reduc_umin_v2si);
  if (HAVE_reduc_umin_v16qi)
    set_optab_handler (reduc_umin_optab, V16QImode, CODE_FOR_reduc_umin_v16qi);
  if (HAVE_reduc_umin_v8hi)
    set_optab_handler (reduc_umin_optab, V8HImode, CODE_FOR_reduc_umin_v8hi);
  if (HAVE_reduc_umin_v4si)
    set_optab_handler (reduc_umin_optab, V4SImode, CODE_FOR_reduc_umin_v4si);
  if (HAVE_reduc_umax_v8qi)
    set_optab_handler (reduc_umax_optab, V8QImode, CODE_FOR_reduc_umax_v8qi);
  if (HAVE_reduc_umax_v4hi)
    set_optab_handler (reduc_umax_optab, V4HImode, CODE_FOR_reduc_umax_v4hi);
  if (HAVE_reduc_umax_v2si)
    set_optab_handler (reduc_umax_optab, V2SImode, CODE_FOR_reduc_umax_v2si);
  if (HAVE_reduc_umax_v16qi)
    set_optab_handler (reduc_umax_optab, V16QImode, CODE_FOR_reduc_umax_v16qi);
  if (HAVE_reduc_umax_v8hi)
    set_optab_handler (reduc_umax_optab, V8HImode, CODE_FOR_reduc_umax_v8hi);
  if (HAVE_reduc_umax_v4si)
    set_optab_handler (reduc_umax_optab, V4SImode, CODE_FOR_reduc_umax_v4si);
  if (HAVE_vcondv8qi)
    set_direct_optab_handler (vcond_optab, V8QImode, CODE_FOR_vcondv8qi);
  if (HAVE_vcondv16qi)
    set_direct_optab_handler (vcond_optab, V16QImode, CODE_FOR_vcondv16qi);
  if (HAVE_vcondv4hi)
    set_direct_optab_handler (vcond_optab, V4HImode, CODE_FOR_vcondv4hi);
  if (HAVE_vcondv8hi)
    set_direct_optab_handler (vcond_optab, V8HImode, CODE_FOR_vcondv8hi);
  if (HAVE_vcondv2si)
    set_direct_optab_handler (vcond_optab, V2SImode, CODE_FOR_vcondv2si);
  if (HAVE_vcondv4si)
    set_direct_optab_handler (vcond_optab, V4SImode, CODE_FOR_vcondv4si);
  if (HAVE_vcondv2sf)
    set_direct_optab_handler (vcond_optab, V2SFmode, CODE_FOR_vcondv2sf);
  if (HAVE_vcondv4sf)
    set_direct_optab_handler (vcond_optab, V4SFmode, CODE_FOR_vcondv4sf);
  if (HAVE_vconduv8qi)
    set_direct_optab_handler (vcondu_optab, V8QImode, CODE_FOR_vconduv8qi);
  if (HAVE_vconduv16qi)
    set_direct_optab_handler (vcondu_optab, V16QImode, CODE_FOR_vconduv16qi);
  if (HAVE_vconduv4hi)
    set_direct_optab_handler (vcondu_optab, V4HImode, CODE_FOR_vconduv4hi);
  if (HAVE_vconduv8hi)
    set_direct_optab_handler (vcondu_optab, V8HImode, CODE_FOR_vconduv8hi);
  if (HAVE_vconduv2si)
    set_direct_optab_handler (vcondu_optab, V2SImode, CODE_FOR_vconduv2si);
  if (HAVE_vconduv4si)
    set_direct_optab_handler (vcondu_optab, V4SImode, CODE_FOR_vconduv4si);
  if (HAVE_vec_load_lanesv8qiv8qi)
    set_convert_optab_handler (vec_load_lanes_optab, V8QImode, V8QImode, CODE_FOR_vec_load_lanesv8qiv8qi);
  if (HAVE_vec_load_lanesv16qiv16qi)
    set_convert_optab_handler (vec_load_lanes_optab, V16QImode, V16QImode, CODE_FOR_vec_load_lanesv16qiv16qi);
  if (HAVE_vec_load_lanesv4hiv4hi)
    set_convert_optab_handler (vec_load_lanes_optab, V4HImode, V4HImode, CODE_FOR_vec_load_lanesv4hiv4hi);
  if (HAVE_vec_load_lanesv8hiv8hi)
    set_convert_optab_handler (vec_load_lanes_optab, V8HImode, V8HImode, CODE_FOR_vec_load_lanesv8hiv8hi);
  if (HAVE_vec_load_lanesv2siv2si)
    set_convert_optab_handler (vec_load_lanes_optab, V2SImode, V2SImode, CODE_FOR_vec_load_lanesv2siv2si);
  if (HAVE_vec_load_lanesv4siv4si)
    set_convert_optab_handler (vec_load_lanes_optab, V4SImode, V4SImode, CODE_FOR_vec_load_lanesv4siv4si);
  if (HAVE_vec_load_lanesv2sfv2sf)
    set_convert_optab_handler (vec_load_lanes_optab, V2SFmode, V2SFmode, CODE_FOR_vec_load_lanesv2sfv2sf);
  if (HAVE_vec_load_lanesv4sfv4sf)
    set_convert_optab_handler (vec_load_lanes_optab, V4SFmode, V4SFmode, CODE_FOR_vec_load_lanesv4sfv4sf);
  if (HAVE_vec_load_lanesdidi)
    set_convert_optab_handler (vec_load_lanes_optab, DImode, DImode, CODE_FOR_vec_load_lanesdidi);
  if (HAVE_vec_load_lanesv2div2di)
    set_convert_optab_handler (vec_load_lanes_optab, V2DImode, V2DImode, CODE_FOR_vec_load_lanesv2div2di);
  if (HAVE_vec_store_lanesv8qiv8qi)
    set_convert_optab_handler (vec_store_lanes_optab, V8QImode, V8QImode, CODE_FOR_vec_store_lanesv8qiv8qi);
  if (HAVE_vec_store_lanesv16qiv16qi)
    set_convert_optab_handler (vec_store_lanes_optab, V16QImode, V16QImode, CODE_FOR_vec_store_lanesv16qiv16qi);
  if (HAVE_vec_store_lanesv4hiv4hi)
    set_convert_optab_handler (vec_store_lanes_optab, V4HImode, V4HImode, CODE_FOR_vec_store_lanesv4hiv4hi);
  if (HAVE_vec_store_lanesv8hiv8hi)
    set_convert_optab_handler (vec_store_lanes_optab, V8HImode, V8HImode, CODE_FOR_vec_store_lanesv8hiv8hi);
  if (HAVE_vec_store_lanesv2siv2si)
    set_convert_optab_handler (vec_store_lanes_optab, V2SImode, V2SImode, CODE_FOR_vec_store_lanesv2siv2si);
  if (HAVE_vec_store_lanesv4siv4si)
    set_convert_optab_handler (vec_store_lanes_optab, V4SImode, V4SImode, CODE_FOR_vec_store_lanesv4siv4si);
  if (HAVE_vec_store_lanesv2sfv2sf)
    set_convert_optab_handler (vec_store_lanes_optab, V2SFmode, V2SFmode, CODE_FOR_vec_store_lanesv2sfv2sf);
  if (HAVE_vec_store_lanesv4sfv4sf)
    set_convert_optab_handler (vec_store_lanes_optab, V4SFmode, V4SFmode, CODE_FOR_vec_store_lanesv4sfv4sf);
  if (HAVE_vec_store_lanesdidi)
    set_convert_optab_handler (vec_store_lanes_optab, DImode, DImode, CODE_FOR_vec_store_lanesdidi);
  if (HAVE_vec_store_lanesv2div2di)
    set_convert_optab_handler (vec_store_lanes_optab, V2DImode, V2DImode, CODE_FOR_vec_store_lanesv2div2di);
  if (HAVE_vec_load_lanestiv8qi)
    set_convert_optab_handler (vec_load_lanes_optab, TImode, V8QImode, CODE_FOR_vec_load_lanestiv8qi);
  if (HAVE_vec_load_lanestiv4hi)
    set_convert_optab_handler (vec_load_lanes_optab, TImode, V4HImode, CODE_FOR_vec_load_lanestiv4hi);
  if (HAVE_vec_load_lanestiv2si)
    set_convert_optab_handler (vec_load_lanes_optab, TImode, V2SImode, CODE_FOR_vec_load_lanestiv2si);
  if (HAVE_vec_load_lanestiv2sf)
    set_convert_optab_handler (vec_load_lanes_optab, TImode, V2SFmode, CODE_FOR_vec_load_lanestiv2sf);
  if (HAVE_vec_load_lanestidi)
    set_convert_optab_handler (vec_load_lanes_optab, TImode, DImode, CODE_FOR_vec_load_lanestidi);
  if (HAVE_vec_load_lanesoiv16qi)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, V16QImode, CODE_FOR_vec_load_lanesoiv16qi);
  if (HAVE_vec_load_lanesoiv8hi)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, V8HImode, CODE_FOR_vec_load_lanesoiv8hi);
  if (HAVE_vec_load_lanesoiv4si)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, V4SImode, CODE_FOR_vec_load_lanesoiv4si);
  if (HAVE_vec_load_lanesoiv4sf)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, V4SFmode, CODE_FOR_vec_load_lanesoiv4sf);
  if (HAVE_vec_store_lanestiv8qi)
    set_convert_optab_handler (vec_store_lanes_optab, TImode, V8QImode, CODE_FOR_vec_store_lanestiv8qi);
  if (HAVE_vec_store_lanestiv4hi)
    set_convert_optab_handler (vec_store_lanes_optab, TImode, V4HImode, CODE_FOR_vec_store_lanestiv4hi);
  if (HAVE_vec_store_lanestiv2si)
    set_convert_optab_handler (vec_store_lanes_optab, TImode, V2SImode, CODE_FOR_vec_store_lanestiv2si);
  if (HAVE_vec_store_lanestiv2sf)
    set_convert_optab_handler (vec_store_lanes_optab, TImode, V2SFmode, CODE_FOR_vec_store_lanestiv2sf);
  if (HAVE_vec_store_lanestidi)
    set_convert_optab_handler (vec_store_lanes_optab, TImode, DImode, CODE_FOR_vec_store_lanestidi);
  if (HAVE_vec_store_lanesoiv16qi)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, V16QImode, CODE_FOR_vec_store_lanesoiv16qi);
  if (HAVE_vec_store_lanesoiv8hi)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, V8HImode, CODE_FOR_vec_store_lanesoiv8hi);
  if (HAVE_vec_store_lanesoiv4si)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, V4SImode, CODE_FOR_vec_store_lanesoiv4si);
  if (HAVE_vec_store_lanesoiv4sf)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, V4SFmode, CODE_FOR_vec_store_lanesoiv4sf);
  if (HAVE_vec_load_laneseiv8qi)
    set_convert_optab_handler (vec_load_lanes_optab, EImode, V8QImode, CODE_FOR_vec_load_laneseiv8qi);
  if (HAVE_vec_load_laneseiv4hi)
    set_convert_optab_handler (vec_load_lanes_optab, EImode, V4HImode, CODE_FOR_vec_load_laneseiv4hi);
  if (HAVE_vec_load_laneseiv2si)
    set_convert_optab_handler (vec_load_lanes_optab, EImode, V2SImode, CODE_FOR_vec_load_laneseiv2si);
  if (HAVE_vec_load_laneseiv2sf)
    set_convert_optab_handler (vec_load_lanes_optab, EImode, V2SFmode, CODE_FOR_vec_load_laneseiv2sf);
  if (HAVE_vec_load_laneseidi)
    set_convert_optab_handler (vec_load_lanes_optab, EImode, DImode, CODE_FOR_vec_load_laneseidi);
  if (HAVE_vec_load_lanesciv16qi)
    set_convert_optab_handler (vec_load_lanes_optab, CImode, V16QImode, CODE_FOR_vec_load_lanesciv16qi);
  if (HAVE_vec_load_lanesciv8hi)
    set_convert_optab_handler (vec_load_lanes_optab, CImode, V8HImode, CODE_FOR_vec_load_lanesciv8hi);
  if (HAVE_vec_load_lanesciv4si)
    set_convert_optab_handler (vec_load_lanes_optab, CImode, V4SImode, CODE_FOR_vec_load_lanesciv4si);
  if (HAVE_vec_load_lanesciv4sf)
    set_convert_optab_handler (vec_load_lanes_optab, CImode, V4SFmode, CODE_FOR_vec_load_lanesciv4sf);
  if (HAVE_vec_store_laneseiv8qi)
    set_convert_optab_handler (vec_store_lanes_optab, EImode, V8QImode, CODE_FOR_vec_store_laneseiv8qi);
  if (HAVE_vec_store_laneseiv4hi)
    set_convert_optab_handler (vec_store_lanes_optab, EImode, V4HImode, CODE_FOR_vec_store_laneseiv4hi);
  if (HAVE_vec_store_laneseiv2si)
    set_convert_optab_handler (vec_store_lanes_optab, EImode, V2SImode, CODE_FOR_vec_store_laneseiv2si);
  if (HAVE_vec_store_laneseiv2sf)
    set_convert_optab_handler (vec_store_lanes_optab, EImode, V2SFmode, CODE_FOR_vec_store_laneseiv2sf);
  if (HAVE_vec_store_laneseidi)
    set_convert_optab_handler (vec_store_lanes_optab, EImode, DImode, CODE_FOR_vec_store_laneseidi);
  if (HAVE_vec_store_lanesciv16qi)
    set_convert_optab_handler (vec_store_lanes_optab, CImode, V16QImode, CODE_FOR_vec_store_lanesciv16qi);
  if (HAVE_vec_store_lanesciv8hi)
    set_convert_optab_handler (vec_store_lanes_optab, CImode, V8HImode, CODE_FOR_vec_store_lanesciv8hi);
  if (HAVE_vec_store_lanesciv4si)
    set_convert_optab_handler (vec_store_lanes_optab, CImode, V4SImode, CODE_FOR_vec_store_lanesciv4si);
  if (HAVE_vec_store_lanesciv4sf)
    set_convert_optab_handler (vec_store_lanes_optab, CImode, V4SFmode, CODE_FOR_vec_store_lanesciv4sf);
  if (HAVE_vec_load_lanesoiv8qi)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, V8QImode, CODE_FOR_vec_load_lanesoiv8qi);
  if (HAVE_vec_load_lanesoiv4hi)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, V4HImode, CODE_FOR_vec_load_lanesoiv4hi);
  if (HAVE_vec_load_lanesoiv2si)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, V2SImode, CODE_FOR_vec_load_lanesoiv2si);
  if (HAVE_vec_load_lanesoiv2sf)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, V2SFmode, CODE_FOR_vec_load_lanesoiv2sf);
  if (HAVE_vec_load_lanesoidi)
    set_convert_optab_handler (vec_load_lanes_optab, OImode, DImode, CODE_FOR_vec_load_lanesoidi);
  if (HAVE_vec_load_lanesxiv16qi)
    set_convert_optab_handler (vec_load_lanes_optab, XImode, V16QImode, CODE_FOR_vec_load_lanesxiv16qi);
  if (HAVE_vec_load_lanesxiv8hi)
    set_convert_optab_handler (vec_load_lanes_optab, XImode, V8HImode, CODE_FOR_vec_load_lanesxiv8hi);
  if (HAVE_vec_load_lanesxiv4si)
    set_convert_optab_handler (vec_load_lanes_optab, XImode, V4SImode, CODE_FOR_vec_load_lanesxiv4si);
  if (HAVE_vec_load_lanesxiv4sf)
    set_convert_optab_handler (vec_load_lanes_optab, XImode, V4SFmode, CODE_FOR_vec_load_lanesxiv4sf);
  if (HAVE_vec_store_lanesoiv8qi)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, V8QImode, CODE_FOR_vec_store_lanesoiv8qi);
  if (HAVE_vec_store_lanesoiv4hi)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, V4HImode, CODE_FOR_vec_store_lanesoiv4hi);
  if (HAVE_vec_store_lanesoiv2si)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, V2SImode, CODE_FOR_vec_store_lanesoiv2si);
  if (HAVE_vec_store_lanesoiv2sf)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, V2SFmode, CODE_FOR_vec_store_lanesoiv2sf);
  if (HAVE_vec_store_lanesoidi)
    set_convert_optab_handler (vec_store_lanes_optab, OImode, DImode, CODE_FOR_vec_store_lanesoidi);
  if (HAVE_vec_store_lanesxiv16qi)
    set_convert_optab_handler (vec_store_lanes_optab, XImode, V16QImode, CODE_FOR_vec_store_lanesxiv16qi);
  if (HAVE_vec_store_lanesxiv8hi)
    set_convert_optab_handler (vec_store_lanes_optab, XImode, V8HImode, CODE_FOR_vec_store_lanesxiv8hi);
  if (HAVE_vec_store_lanesxiv4si)
    set_convert_optab_handler (vec_store_lanes_optab, XImode, V4SImode, CODE_FOR_vec_store_lanesxiv4si);
  if (HAVE_vec_store_lanesxiv4sf)
    set_convert_optab_handler (vec_store_lanes_optab, XImode, V4SFmode, CODE_FOR_vec_store_lanesxiv4sf);
  if (HAVE_vec_unpacks_hi_v16qi)
    set_optab_handler (vec_unpacks_hi_optab, V16QImode, CODE_FOR_vec_unpacks_hi_v16qi);
  if (HAVE_vec_unpacku_hi_v16qi)
    set_optab_handler (vec_unpacku_hi_optab, V16QImode, CODE_FOR_vec_unpacku_hi_v16qi);
  if (HAVE_vec_unpacks_hi_v8hi)
    set_optab_handler (vec_unpacks_hi_optab, V8HImode, CODE_FOR_vec_unpacks_hi_v8hi);
  if (HAVE_vec_unpacku_hi_v8hi)
    set_optab_handler (vec_unpacku_hi_optab, V8HImode, CODE_FOR_vec_unpacku_hi_v8hi);
  if (HAVE_vec_unpacks_hi_v4si)
    set_optab_handler (vec_unpacks_hi_optab, V4SImode, CODE_FOR_vec_unpacks_hi_v4si);
  if (HAVE_vec_unpacku_hi_v4si)
    set_optab_handler (vec_unpacku_hi_optab, V4SImode, CODE_FOR_vec_unpacku_hi_v4si);
  if (HAVE_vec_unpacks_lo_v16qi)
    set_optab_handler (vec_unpacks_lo_optab, V16QImode, CODE_FOR_vec_unpacks_lo_v16qi);
  if (HAVE_vec_unpacku_lo_v16qi)
    set_optab_handler (vec_unpacku_lo_optab, V16QImode, CODE_FOR_vec_unpacku_lo_v16qi);
  if (HAVE_vec_unpacks_lo_v8hi)
    set_optab_handler (vec_unpacks_lo_optab, V8HImode, CODE_FOR_vec_unpacks_lo_v8hi);
  if (HAVE_vec_unpacku_lo_v8hi)
    set_optab_handler (vec_unpacku_lo_optab, V8HImode, CODE_FOR_vec_unpacku_lo_v8hi);
  if (HAVE_vec_unpacks_lo_v4si)
    set_optab_handler (vec_unpacks_lo_optab, V4SImode, CODE_FOR_vec_unpacks_lo_v4si);
  if (HAVE_vec_unpacku_lo_v4si)
    set_optab_handler (vec_unpacku_lo_optab, V4SImode, CODE_FOR_vec_unpacku_lo_v4si);
  if (HAVE_vec_widen_smult_lo_v16qi)
    set_optab_handler (vec_widen_smult_lo_optab, V16QImode, CODE_FOR_vec_widen_smult_lo_v16qi);
  if (HAVE_vec_widen_umult_lo_v16qi)
    set_optab_handler (vec_widen_umult_lo_optab, V16QImode, CODE_FOR_vec_widen_umult_lo_v16qi);
  if (HAVE_vec_widen_smult_lo_v8hi)
    set_optab_handler (vec_widen_smult_lo_optab, V8HImode, CODE_FOR_vec_widen_smult_lo_v8hi);
  if (HAVE_vec_widen_umult_lo_v8hi)
    set_optab_handler (vec_widen_umult_lo_optab, V8HImode, CODE_FOR_vec_widen_umult_lo_v8hi);
  if (HAVE_vec_widen_smult_lo_v4si)
    set_optab_handler (vec_widen_smult_lo_optab, V4SImode, CODE_FOR_vec_widen_smult_lo_v4si);
  if (HAVE_vec_widen_umult_lo_v4si)
    set_optab_handler (vec_widen_umult_lo_optab, V4SImode, CODE_FOR_vec_widen_umult_lo_v4si);
  if (HAVE_vec_widen_smult_hi_v16qi)
    set_optab_handler (vec_widen_smult_hi_optab, V16QImode, CODE_FOR_vec_widen_smult_hi_v16qi);
  if (HAVE_vec_widen_umult_hi_v16qi)
    set_optab_handler (vec_widen_umult_hi_optab, V16QImode, CODE_FOR_vec_widen_umult_hi_v16qi);
  if (HAVE_vec_widen_smult_hi_v8hi)
    set_optab_handler (vec_widen_smult_hi_optab, V8HImode, CODE_FOR_vec_widen_smult_hi_v8hi);
  if (HAVE_vec_widen_umult_hi_v8hi)
    set_optab_handler (vec_widen_umult_hi_optab, V8HImode, CODE_FOR_vec_widen_umult_hi_v8hi);
  if (HAVE_vec_widen_smult_hi_v4si)
    set_optab_handler (vec_widen_smult_hi_optab, V4SImode, CODE_FOR_vec_widen_smult_hi_v4si);
  if (HAVE_vec_widen_umult_hi_v4si)
    set_optab_handler (vec_widen_umult_hi_optab, V4SImode, CODE_FOR_vec_widen_umult_hi_v4si);
  if (HAVE_vec_widen_sshiftl_lo_v16qi)
    set_optab_handler (vec_widen_sshiftl_lo_optab, V16QImode, CODE_FOR_vec_widen_sshiftl_lo_v16qi);
  if (HAVE_vec_widen_ushiftl_lo_v16qi)
    set_optab_handler (vec_widen_ushiftl_lo_optab, V16QImode, CODE_FOR_vec_widen_ushiftl_lo_v16qi);
  if (HAVE_vec_widen_sshiftl_lo_v8hi)
    set_optab_handler (vec_widen_sshiftl_lo_optab, V8HImode, CODE_FOR_vec_widen_sshiftl_lo_v8hi);
  if (HAVE_vec_widen_ushiftl_lo_v8hi)
    set_optab_handler (vec_widen_ushiftl_lo_optab, V8HImode, CODE_FOR_vec_widen_ushiftl_lo_v8hi);
  if (HAVE_vec_widen_sshiftl_lo_v4si)
    set_optab_handler (vec_widen_sshiftl_lo_optab, V4SImode, CODE_FOR_vec_widen_sshiftl_lo_v4si);
  if (HAVE_vec_widen_ushiftl_lo_v4si)
    set_optab_handler (vec_widen_ushiftl_lo_optab, V4SImode, CODE_FOR_vec_widen_ushiftl_lo_v4si);
  if (HAVE_vec_widen_sshiftl_hi_v16qi)
    set_optab_handler (vec_widen_sshiftl_hi_optab, V16QImode, CODE_FOR_vec_widen_sshiftl_hi_v16qi);
  if (HAVE_vec_widen_ushiftl_hi_v16qi)
    set_optab_handler (vec_widen_ushiftl_hi_optab, V16QImode, CODE_FOR_vec_widen_ushiftl_hi_v16qi);
  if (HAVE_vec_widen_sshiftl_hi_v8hi)
    set_optab_handler (vec_widen_sshiftl_hi_optab, V8HImode, CODE_FOR_vec_widen_sshiftl_hi_v8hi);
  if (HAVE_vec_widen_ushiftl_hi_v8hi)
    set_optab_handler (vec_widen_ushiftl_hi_optab, V8HImode, CODE_FOR_vec_widen_ushiftl_hi_v8hi);
  if (HAVE_vec_widen_sshiftl_hi_v4si)
    set_optab_handler (vec_widen_sshiftl_hi_optab, V4SImode, CODE_FOR_vec_widen_sshiftl_hi_v4si);
  if (HAVE_vec_widen_ushiftl_hi_v4si)
    set_optab_handler (vec_widen_ushiftl_hi_optab, V4SImode, CODE_FOR_vec_widen_ushiftl_hi_v4si);
  if (HAVE_vec_unpacks_lo_v8qi)
    set_optab_handler (vec_unpacks_lo_optab, V8QImode, CODE_FOR_vec_unpacks_lo_v8qi);
  if (HAVE_vec_unpacku_lo_v8qi)
    set_optab_handler (vec_unpacku_lo_optab, V8QImode, CODE_FOR_vec_unpacku_lo_v8qi);
  if (HAVE_vec_unpacks_lo_v4hi)
    set_optab_handler (vec_unpacks_lo_optab, V4HImode, CODE_FOR_vec_unpacks_lo_v4hi);
  if (HAVE_vec_unpacku_lo_v4hi)
    set_optab_handler (vec_unpacku_lo_optab, V4HImode, CODE_FOR_vec_unpacku_lo_v4hi);
  if (HAVE_vec_unpacks_lo_v2si)
    set_optab_handler (vec_unpacks_lo_optab, V2SImode, CODE_FOR_vec_unpacks_lo_v2si);
  if (HAVE_vec_unpacku_lo_v2si)
    set_optab_handler (vec_unpacku_lo_optab, V2SImode, CODE_FOR_vec_unpacku_lo_v2si);
  if (HAVE_vec_unpacks_hi_v8qi)
    set_optab_handler (vec_unpacks_hi_optab, V8QImode, CODE_FOR_vec_unpacks_hi_v8qi);
  if (HAVE_vec_unpacku_hi_v8qi)
    set_optab_handler (vec_unpacku_hi_optab, V8QImode, CODE_FOR_vec_unpacku_hi_v8qi);
  if (HAVE_vec_unpacks_hi_v4hi)
    set_optab_handler (vec_unpacks_hi_optab, V4HImode, CODE_FOR_vec_unpacks_hi_v4hi);
  if (HAVE_vec_unpacku_hi_v4hi)
    set_optab_handler (vec_unpacku_hi_optab, V4HImode, CODE_FOR_vec_unpacku_hi_v4hi);
  if (HAVE_vec_unpacks_hi_v2si)
    set_optab_handler (vec_unpacks_hi_optab, V2SImode, CODE_FOR_vec_unpacks_hi_v2si);
  if (HAVE_vec_unpacku_hi_v2si)
    set_optab_handler (vec_unpacku_hi_optab, V2SImode, CODE_FOR_vec_unpacku_hi_v2si);
  if (HAVE_vec_widen_smult_hi_v8qi)
    set_optab_handler (vec_widen_smult_hi_optab, V8QImode, CODE_FOR_vec_widen_smult_hi_v8qi);
  if (HAVE_vec_widen_umult_hi_v8qi)
    set_optab_handler (vec_widen_umult_hi_optab, V8QImode, CODE_FOR_vec_widen_umult_hi_v8qi);
  if (HAVE_vec_widen_smult_hi_v4hi)
    set_optab_handler (vec_widen_smult_hi_optab, V4HImode, CODE_FOR_vec_widen_smult_hi_v4hi);
  if (HAVE_vec_widen_umult_hi_v4hi)
    set_optab_handler (vec_widen_umult_hi_optab, V4HImode, CODE_FOR_vec_widen_umult_hi_v4hi);
  if (HAVE_vec_widen_smult_hi_v2si)
    set_optab_handler (vec_widen_smult_hi_optab, V2SImode, CODE_FOR_vec_widen_smult_hi_v2si);
  if (HAVE_vec_widen_umult_hi_v2si)
    set_optab_handler (vec_widen_umult_hi_optab, V2SImode, CODE_FOR_vec_widen_umult_hi_v2si);
  if (HAVE_vec_widen_smult_lo_v8qi)
    set_optab_handler (vec_widen_smult_lo_optab, V8QImode, CODE_FOR_vec_widen_smult_lo_v8qi);
  if (HAVE_vec_widen_umult_lo_v8qi)
    set_optab_handler (vec_widen_umult_lo_optab, V8QImode, CODE_FOR_vec_widen_umult_lo_v8qi);
  if (HAVE_vec_widen_smult_lo_v4hi)
    set_optab_handler (vec_widen_smult_lo_optab, V4HImode, CODE_FOR_vec_widen_smult_lo_v4hi);
  if (HAVE_vec_widen_umult_lo_v4hi)
    set_optab_handler (vec_widen_umult_lo_optab, V4HImode, CODE_FOR_vec_widen_umult_lo_v4hi);
  if (HAVE_vec_widen_smult_lo_v2si)
    set_optab_handler (vec_widen_smult_lo_optab, V2SImode, CODE_FOR_vec_widen_smult_lo_v2si);
  if (HAVE_vec_widen_umult_lo_v2si)
    set_optab_handler (vec_widen_umult_lo_optab, V2SImode, CODE_FOR_vec_widen_umult_lo_v2si);
  if (HAVE_vec_widen_sshiftl_hi_v8qi)
    set_optab_handler (vec_widen_sshiftl_hi_optab, V8QImode, CODE_FOR_vec_widen_sshiftl_hi_v8qi);
  if (HAVE_vec_widen_ushiftl_hi_v8qi)
    set_optab_handler (vec_widen_ushiftl_hi_optab, V8QImode, CODE_FOR_vec_widen_ushiftl_hi_v8qi);
  if (HAVE_vec_widen_sshiftl_hi_v4hi)
    set_optab_handler (vec_widen_sshiftl_hi_optab, V4HImode, CODE_FOR_vec_widen_sshiftl_hi_v4hi);
  if (HAVE_vec_widen_ushiftl_hi_v4hi)
    set_optab_handler (vec_widen_ushiftl_hi_optab, V4HImode, CODE_FOR_vec_widen_ushiftl_hi_v4hi);
  if (HAVE_vec_widen_sshiftl_hi_v2si)
    set_optab_handler (vec_widen_sshiftl_hi_optab, V2SImode, CODE_FOR_vec_widen_sshiftl_hi_v2si);
  if (HAVE_vec_widen_ushiftl_hi_v2si)
    set_optab_handler (vec_widen_ushiftl_hi_optab, V2SImode, CODE_FOR_vec_widen_ushiftl_hi_v2si);
  if (HAVE_vec_widen_sshiftl_lo_v8qi)
    set_optab_handler (vec_widen_sshiftl_lo_optab, V8QImode, CODE_FOR_vec_widen_sshiftl_lo_v8qi);
  if (HAVE_vec_widen_ushiftl_lo_v8qi)
    set_optab_handler (vec_widen_ushiftl_lo_optab, V8QImode, CODE_FOR_vec_widen_ushiftl_lo_v8qi);
  if (HAVE_vec_widen_sshiftl_lo_v4hi)
    set_optab_handler (vec_widen_sshiftl_lo_optab, V4HImode, CODE_FOR_vec_widen_sshiftl_lo_v4hi);
  if (HAVE_vec_widen_ushiftl_lo_v4hi)
    set_optab_handler (vec_widen_ushiftl_lo_optab, V4HImode, CODE_FOR_vec_widen_ushiftl_lo_v4hi);
  if (HAVE_vec_widen_sshiftl_lo_v2si)
    set_optab_handler (vec_widen_sshiftl_lo_optab, V2SImode, CODE_FOR_vec_widen_sshiftl_lo_v2si);
  if (HAVE_vec_widen_ushiftl_lo_v2si)
    set_optab_handler (vec_widen_ushiftl_lo_optab, V2SImode, CODE_FOR_vec_widen_ushiftl_lo_v2si);
  if (HAVE_vec_pack_trunc_v4hi)
    set_optab_handler (vec_pack_trunc_optab, V4HImode, CODE_FOR_vec_pack_trunc_v4hi);
  if (HAVE_vec_pack_trunc_v2si)
    set_optab_handler (vec_pack_trunc_optab, V2SImode, CODE_FOR_vec_pack_trunc_v2si);
  if (HAVE_vec_pack_trunc_di)
    set_optab_handler (vec_pack_trunc_optab, DImode, CODE_FOR_vec_pack_trunc_di);
  if (HAVE_sync_compare_and_swapqi)
    set_direct_optab_handler (sync_compare_and_swap_optab, QImode, CODE_FOR_sync_compare_and_swapqi);
  if (HAVE_sync_compare_and_swaphi)
    set_direct_optab_handler (sync_compare_and_swap_optab, HImode, CODE_FOR_sync_compare_and_swaphi);
  if (HAVE_sync_compare_and_swapsi)
    set_direct_optab_handler (sync_compare_and_swap_optab, SImode, CODE_FOR_sync_compare_and_swapsi);
  if (HAVE_sync_compare_and_swapdi)
    set_direct_optab_handler (sync_compare_and_swap_optab, DImode, CODE_FOR_sync_compare_and_swapdi);
  if (HAVE_sync_lock_test_and_setqi)
    set_direct_optab_handler (sync_lock_test_and_set_optab, QImode, CODE_FOR_sync_lock_test_and_setqi);
  if (HAVE_sync_lock_test_and_sethi)
    set_direct_optab_handler (sync_lock_test_and_set_optab, HImode, CODE_FOR_sync_lock_test_and_sethi);
  if (HAVE_sync_lock_test_and_setsi)
    set_direct_optab_handler (sync_lock_test_and_set_optab, SImode, CODE_FOR_sync_lock_test_and_setsi);
  if (HAVE_sync_lock_test_and_setdi)
    set_direct_optab_handler (sync_lock_test_and_set_optab, DImode, CODE_FOR_sync_lock_test_and_setdi);
  if (HAVE_sync_addqi)
    set_direct_optab_handler (sync_add_optab, QImode, CODE_FOR_sync_addqi);
  if (HAVE_sync_subqi)
    set_direct_optab_handler (sync_sub_optab, QImode, CODE_FOR_sync_subqi);
  if (HAVE_sync_iorqi)
    set_direct_optab_handler (sync_ior_optab, QImode, CODE_FOR_sync_iorqi);
  if (HAVE_sync_xorqi)
    set_direct_optab_handler (sync_xor_optab, QImode, CODE_FOR_sync_xorqi);
  if (HAVE_sync_andqi)
    set_direct_optab_handler (sync_and_optab, QImode, CODE_FOR_sync_andqi);
  if (HAVE_sync_addhi)
    set_direct_optab_handler (sync_add_optab, HImode, CODE_FOR_sync_addhi);
  if (HAVE_sync_subhi)
    set_direct_optab_handler (sync_sub_optab, HImode, CODE_FOR_sync_subhi);
  if (HAVE_sync_iorhi)
    set_direct_optab_handler (sync_ior_optab, HImode, CODE_FOR_sync_iorhi);
  if (HAVE_sync_xorhi)
    set_direct_optab_handler (sync_xor_optab, HImode, CODE_FOR_sync_xorhi);
  if (HAVE_sync_andhi)
    set_direct_optab_handler (sync_and_optab, HImode, CODE_FOR_sync_andhi);
  if (HAVE_sync_addsi)
    set_direct_optab_handler (sync_add_optab, SImode, CODE_FOR_sync_addsi);
  if (HAVE_sync_subsi)
    set_direct_optab_handler (sync_sub_optab, SImode, CODE_FOR_sync_subsi);
  if (HAVE_sync_iorsi)
    set_direct_optab_handler (sync_ior_optab, SImode, CODE_FOR_sync_iorsi);
  if (HAVE_sync_xorsi)
    set_direct_optab_handler (sync_xor_optab, SImode, CODE_FOR_sync_xorsi);
  if (HAVE_sync_andsi)
    set_direct_optab_handler (sync_and_optab, SImode, CODE_FOR_sync_andsi);
  if (HAVE_sync_adddi)
    set_direct_optab_handler (sync_add_optab, DImode, CODE_FOR_sync_adddi);
  if (HAVE_sync_subdi)
    set_direct_optab_handler (sync_sub_optab, DImode, CODE_FOR_sync_subdi);
  if (HAVE_sync_iordi)
    set_direct_optab_handler (sync_ior_optab, DImode, CODE_FOR_sync_iordi);
  if (HAVE_sync_xordi)
    set_direct_optab_handler (sync_xor_optab, DImode, CODE_FOR_sync_xordi);
  if (HAVE_sync_anddi)
    set_direct_optab_handler (sync_and_optab, DImode, CODE_FOR_sync_anddi);
  if (HAVE_sync_nandqi)
    set_direct_optab_handler (sync_nand_optab, QImode, CODE_FOR_sync_nandqi);
  if (HAVE_sync_nandhi)
    set_direct_optab_handler (sync_nand_optab, HImode, CODE_FOR_sync_nandhi);
  if (HAVE_sync_nandsi)
    set_direct_optab_handler (sync_nand_optab, SImode, CODE_FOR_sync_nandsi);
  if (HAVE_sync_nanddi)
    set_direct_optab_handler (sync_nand_optab, DImode, CODE_FOR_sync_nanddi);
  if (HAVE_sync_new_addqi)
    set_direct_optab_handler (sync_new_add_optab, QImode, CODE_FOR_sync_new_addqi);
  if (HAVE_sync_new_subqi)
    set_direct_optab_handler (sync_new_sub_optab, QImode, CODE_FOR_sync_new_subqi);
  if (HAVE_sync_new_iorqi)
    set_direct_optab_handler (sync_new_ior_optab, QImode, CODE_FOR_sync_new_iorqi);
  if (HAVE_sync_new_xorqi)
    set_direct_optab_handler (sync_new_xor_optab, QImode, CODE_FOR_sync_new_xorqi);
  if (HAVE_sync_new_andqi)
    set_direct_optab_handler (sync_new_and_optab, QImode, CODE_FOR_sync_new_andqi);
  if (HAVE_sync_new_addhi)
    set_direct_optab_handler (sync_new_add_optab, HImode, CODE_FOR_sync_new_addhi);
  if (HAVE_sync_new_subhi)
    set_direct_optab_handler (sync_new_sub_optab, HImode, CODE_FOR_sync_new_subhi);
  if (HAVE_sync_new_iorhi)
    set_direct_optab_handler (sync_new_ior_optab, HImode, CODE_FOR_sync_new_iorhi);
  if (HAVE_sync_new_xorhi)
    set_direct_optab_handler (sync_new_xor_optab, HImode, CODE_FOR_sync_new_xorhi);
  if (HAVE_sync_new_andhi)
    set_direct_optab_handler (sync_new_and_optab, HImode, CODE_FOR_sync_new_andhi);
  if (HAVE_sync_new_addsi)
    set_direct_optab_handler (sync_new_add_optab, SImode, CODE_FOR_sync_new_addsi);
  if (HAVE_sync_new_subsi)
    set_direct_optab_handler (sync_new_sub_optab, SImode, CODE_FOR_sync_new_subsi);
  if (HAVE_sync_new_iorsi)
    set_direct_optab_handler (sync_new_ior_optab, SImode, CODE_FOR_sync_new_iorsi);
  if (HAVE_sync_new_xorsi)
    set_direct_optab_handler (sync_new_xor_optab, SImode, CODE_FOR_sync_new_xorsi);
  if (HAVE_sync_new_andsi)
    set_direct_optab_handler (sync_new_and_optab, SImode, CODE_FOR_sync_new_andsi);
  if (HAVE_sync_new_adddi)
    set_direct_optab_handler (sync_new_add_optab, DImode, CODE_FOR_sync_new_adddi);
  if (HAVE_sync_new_subdi)
    set_direct_optab_handler (sync_new_sub_optab, DImode, CODE_FOR_sync_new_subdi);
  if (HAVE_sync_new_iordi)
    set_direct_optab_handler (sync_new_ior_optab, DImode, CODE_FOR_sync_new_iordi);
  if (HAVE_sync_new_xordi)
    set_direct_optab_handler (sync_new_xor_optab, DImode, CODE_FOR_sync_new_xordi);
  if (HAVE_sync_new_anddi)
    set_direct_optab_handler (sync_new_and_optab, DImode, CODE_FOR_sync_new_anddi);
  if (HAVE_sync_new_nandqi)
    set_direct_optab_handler (sync_new_nand_optab, QImode, CODE_FOR_sync_new_nandqi);
  if (HAVE_sync_new_nandhi)
    set_direct_optab_handler (sync_new_nand_optab, HImode, CODE_FOR_sync_new_nandhi);
  if (HAVE_sync_new_nandsi)
    set_direct_optab_handler (sync_new_nand_optab, SImode, CODE_FOR_sync_new_nandsi);
  if (HAVE_sync_new_nanddi)
    set_direct_optab_handler (sync_new_nand_optab, DImode, CODE_FOR_sync_new_nanddi);
  if (HAVE_sync_old_addqi)
    set_direct_optab_handler (sync_old_add_optab, QImode, CODE_FOR_sync_old_addqi);
  if (HAVE_sync_old_subqi)
    set_direct_optab_handler (sync_old_sub_optab, QImode, CODE_FOR_sync_old_subqi);
  if (HAVE_sync_old_iorqi)
    set_direct_optab_handler (sync_old_ior_optab, QImode, CODE_FOR_sync_old_iorqi);
  if (HAVE_sync_old_xorqi)
    set_direct_optab_handler (sync_old_xor_optab, QImode, CODE_FOR_sync_old_xorqi);
  if (HAVE_sync_old_andqi)
    set_direct_optab_handler (sync_old_and_optab, QImode, CODE_FOR_sync_old_andqi);
  if (HAVE_sync_old_addhi)
    set_direct_optab_handler (sync_old_add_optab, HImode, CODE_FOR_sync_old_addhi);
  if (HAVE_sync_old_subhi)
    set_direct_optab_handler (sync_old_sub_optab, HImode, CODE_FOR_sync_old_subhi);
  if (HAVE_sync_old_iorhi)
    set_direct_optab_handler (sync_old_ior_optab, HImode, CODE_FOR_sync_old_iorhi);
  if (HAVE_sync_old_xorhi)
    set_direct_optab_handler (sync_old_xor_optab, HImode, CODE_FOR_sync_old_xorhi);
  if (HAVE_sync_old_andhi)
    set_direct_optab_handler (sync_old_and_optab, HImode, CODE_FOR_sync_old_andhi);
  if (HAVE_sync_old_addsi)
    set_direct_optab_handler (sync_old_add_optab, SImode, CODE_FOR_sync_old_addsi);
  if (HAVE_sync_old_subsi)
    set_direct_optab_handler (sync_old_sub_optab, SImode, CODE_FOR_sync_old_subsi);
  if (HAVE_sync_old_iorsi)
    set_direct_optab_handler (sync_old_ior_optab, SImode, CODE_FOR_sync_old_iorsi);
  if (HAVE_sync_old_xorsi)
    set_direct_optab_handler (sync_old_xor_optab, SImode, CODE_FOR_sync_old_xorsi);
  if (HAVE_sync_old_andsi)
    set_direct_optab_handler (sync_old_and_optab, SImode, CODE_FOR_sync_old_andsi);
  if (HAVE_sync_old_adddi)
    set_direct_optab_handler (sync_old_add_optab, DImode, CODE_FOR_sync_old_adddi);
  if (HAVE_sync_old_subdi)
    set_direct_optab_handler (sync_old_sub_optab, DImode, CODE_FOR_sync_old_subdi);
  if (HAVE_sync_old_iordi)
    set_direct_optab_handler (sync_old_ior_optab, DImode, CODE_FOR_sync_old_iordi);
  if (HAVE_sync_old_xordi)
    set_direct_optab_handler (sync_old_xor_optab, DImode, CODE_FOR_sync_old_xordi);
  if (HAVE_sync_old_anddi)
    set_direct_optab_handler (sync_old_and_optab, DImode, CODE_FOR_sync_old_anddi);
  if (HAVE_sync_old_nandqi)
    set_direct_optab_handler (sync_old_nand_optab, QImode, CODE_FOR_sync_old_nandqi);
  if (HAVE_sync_old_nandhi)
    set_direct_optab_handler (sync_old_nand_optab, HImode, CODE_FOR_sync_old_nandhi);
  if (HAVE_sync_old_nandsi)
    set_direct_optab_handler (sync_old_nand_optab, SImode, CODE_FOR_sync_old_nandsi);
  if (HAVE_sync_old_nanddi)
    set_direct_optab_handler (sync_old_nand_optab, DImode, CODE_FOR_sync_old_nanddi);
  if (HAVE_sync_lock_releasedi)
    set_direct_optab_handler (sync_lock_release_optab, DImode, CODE_FOR_sync_lock_releasedi);

#ifdef FIXUNS_TRUNC_LIKE_FIX_TRUNC
  /* This flag says the same insns that convert to a signed fixnum
     also convert validly to an unsigned one.  */
  for (i = 0; i < NUM_MACHINE_MODES; i++)
    for (j = 0; j < NUM_MACHINE_MODES; j++)
      set_convert_optab_handler
 	(ufixtrunc_optab, (enum machine_mode) i, (enum machine_mode) j,
	 convert_optab_handler (sfixtrunc_optab, (enum machine_mode) i,
						 (enum machine_mode) j));
#endif
}
