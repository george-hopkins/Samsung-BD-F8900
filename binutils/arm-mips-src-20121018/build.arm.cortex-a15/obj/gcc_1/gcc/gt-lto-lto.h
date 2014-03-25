/* Type information for lto/lto.c.
   Copyright (C) 2004, 2007, 2009 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* This file is machine generated.  Do not edit.  */

void
gt_ggc_mx_VEC_ltrans_partition_gc (void *x_p)
{
  struct VEC_ltrans_partition_gc * const x = (struct VEC_ltrans_partition_gc *)x_p;
  if (ggc_test_and_set_mark (x))
    {
      {
        size_t i0;
        size_t l0 = (size_t)(((*x).base).num);
        for (i0 = 0; i0 != l0; i0++) {
          gt_ggc_m_20ltrans_partition_def ((*x).base.vec[i0]);
        }
      }
    }
}

void
gt_ggc_mx_ltrans_partition_def (void *x_p)
{
  struct ltrans_partition_def * const x = (struct ltrans_partition_def *)x_p;
  if (ggc_test_and_set_mark (x))
    {
      gt_ggc_m_19cgraph_node_set_def ((*x).cgraph_set);
      gt_ggc_m_20varpool_node_set_def ((*x).varpool_set);
    }
}

void
gt_pch_nx_VEC_ltrans_partition_gc (void *x_p)
{
  struct VEC_ltrans_partition_gc * const x = (struct VEC_ltrans_partition_gc *)x_p;
  if (gt_pch_note_object (x, x, gt_pch_p_23VEC_ltrans_partition_gc, gt_ggc_e_23VEC_ltrans_partition_gc))
    {
      {
        size_t i0;
        size_t l0 = (size_t)(((*x).base).num);
        for (i0 = 0; i0 != l0; i0++) {
          gt_pch_n_20ltrans_partition_def ((*x).base.vec[i0]);
        }
      }
    }
}

void
gt_pch_nx_ltrans_partition_def (void *x_p)
{
  struct ltrans_partition_def * const x = (struct ltrans_partition_def *)x_p;
  if (gt_pch_note_object (x, x, gt_pch_p_20ltrans_partition_def, gt_ggc_e_20ltrans_partition_def))
    {
      gt_pch_n_19cgraph_node_set_def ((*x).cgraph_set);
      gt_pch_n_20varpool_node_set_def ((*x).varpool_set);
    }
}

void
gt_pch_p_23VEC_ltrans_partition_gc (ATTRIBUTE_UNUSED void *this_obj,
	void *x_p,
	ATTRIBUTE_UNUSED gt_pointer_operator op,
	ATTRIBUTE_UNUSED void *cookie)
{
  struct VEC_ltrans_partition_gc * const x ATTRIBUTE_UNUSED = (struct VEC_ltrans_partition_gc *)x_p;
  {
    size_t i0;
    size_t l0 = (size_t)(((*x).base).num);
    for (i0 = 0; i0 != l0; i0++) {
      if ((void *)(x) == this_obj)
        op (&((*x).base.vec[i0]), cookie);
    }
  }
}

void
gt_pch_p_20ltrans_partition_def (ATTRIBUTE_UNUSED void *this_obj,
	void *x_p,
	ATTRIBUTE_UNUSED gt_pointer_operator op,
	ATTRIBUTE_UNUSED void *cookie)
{
  struct ltrans_partition_def * const x ATTRIBUTE_UNUSED = (struct ltrans_partition_def *)x_p;
  if ((void *)(x) == this_obj)
    op (&((*x).cgraph_set), cookie);
  if ((void *)(x) == this_obj)
    op (&((*x).varpool_set), cookie);
}

/* GC roots.  */

static void gt_ggc_ma_real_file_decl_data (void *);
static void
gt_ggc_ma_real_file_decl_data (ATTRIBUTE_UNUSED void *x_p)
{
  if (real_file_decl_data != NULL) {
    size_t i0;
    for (i0 = 0; i0 != (size_t)(real_file_count + 1); i0++) {
      gt_ggc_m_18lto_file_decl_data (real_file_decl_data[i0]);
    }
    ggc_mark (real_file_decl_data);
  }
}

static void gt_pch_pa_real_file_decl_data
    (void *, void *, gt_pointer_operator, void *);
static void gt_pch_pa_real_file_decl_data (ATTRIBUTE_UNUSED void *this_obj,
      ATTRIBUTE_UNUSED void *x_p,
      ATTRIBUTE_UNUSED gt_pointer_operator op,
      ATTRIBUTE_UNUSED void * cookie)
{
  if (real_file_decl_data != NULL) {
    size_t i0;
    for (i0 = 0; i0 != (size_t)(real_file_count + 1); i0++) {
      if ((void *)(real_file_decl_data) == this_obj)
        op (&(real_file_decl_data[i0]), cookie);
    }
    if ((void *)(&real_file_decl_data) == this_obj)
      op (&(real_file_decl_data), cookie);
  }
}

static void gt_pch_na_real_file_decl_data (void *);
static void
gt_pch_na_real_file_decl_data (ATTRIBUTE_UNUSED void *x_p)
{
  if (real_file_decl_data != NULL) {
    size_t i1;
    for (i1 = 0; i1 != (size_t)(real_file_count + 1); i1++) {
      gt_pch_n_18lto_file_decl_data (real_file_decl_data[i1]);
    }
    gt_pch_note_object (real_file_decl_data, &real_file_decl_data, gt_pch_pa_real_file_decl_data, gt_types_enum_last);
  }
}

static void gt_ggc_ma_all_file_decl_data (void *);
static void
gt_ggc_ma_all_file_decl_data (ATTRIBUTE_UNUSED void *x_p)
{
  if (all_file_decl_data != NULL) {
    size_t i0;
    for (i0 = 0; i0 != (size_t)(lto_stats.num_input_files + 1); i0++) {
      gt_ggc_m_18lto_file_decl_data (all_file_decl_data[i0]);
    }
    ggc_mark (all_file_decl_data);
  }
}

static void gt_pch_pa_all_file_decl_data
    (void *, void *, gt_pointer_operator, void *);
static void gt_pch_pa_all_file_decl_data (ATTRIBUTE_UNUSED void *this_obj,
      ATTRIBUTE_UNUSED void *x_p,
      ATTRIBUTE_UNUSED gt_pointer_operator op,
      ATTRIBUTE_UNUSED void * cookie)
{
  if (all_file_decl_data != NULL) {
    size_t i0;
    for (i0 = 0; i0 != (size_t)(lto_stats.num_input_files + 1); i0++) {
      if ((void *)(all_file_decl_data) == this_obj)
        op (&(all_file_decl_data[i0]), cookie);
    }
    if ((void *)(&all_file_decl_data) == this_obj)
      op (&(all_file_decl_data), cookie);
  }
}

static void gt_pch_na_all_file_decl_data (void *);
static void
gt_pch_na_all_file_decl_data (ATTRIBUTE_UNUSED void *x_p)
{
  if (all_file_decl_data != NULL) {
    size_t i1;
    for (i1 = 0; i1 != (size_t)(lto_stats.num_input_files + 1); i1++) {
      gt_pch_n_18lto_file_decl_data (all_file_decl_data[i1]);
    }
    gt_pch_note_object (all_file_decl_data, &all_file_decl_data, gt_pch_pa_all_file_decl_data, gt_types_enum_last);
  }
}

EXPORTED_CONST struct ggc_root_tab gt_ggc_r_gt_lto_lto_h[] = {
  {
    &lto_eh_personality_decl,
    1,
    sizeof (lto_eh_personality_decl),
    &gt_ggc_mx_tree_node,
    &gt_pch_nx_tree_node
  },
  {
    &real_file_decl_data,
    1,
    sizeof (real_file_decl_data),
    &gt_ggc_ma_real_file_decl_data,
    &gt_pch_na_real_file_decl_data
  },
  {
    &all_file_decl_data,
    1,
    sizeof (all_file_decl_data),
    &gt_ggc_ma_all_file_decl_data,
    &gt_pch_na_all_file_decl_data
  },
  {
    &ltrans_partitions,
    1,
    sizeof (ltrans_partitions),
    &gt_ggc_mx_VEC_ltrans_partition_gc,
    &gt_pch_nx_VEC_ltrans_partition_gc
  },
  {
    &first_personality_decl,
    1,
    sizeof (first_personality_decl),
    &gt_ggc_mx_tree_node,
    &gt_pch_nx_tree_node
  },
  LAST_GGC_ROOT_TAB
};

