/* Type information for gimple.c.
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
gt_ggc_mx_gimple_type_leader_entry_s (void *x_p)
{
  struct gimple_type_leader_entry_s * const x = (struct gimple_type_leader_entry_s *)x_p;
  if (ggc_test_and_set_mark (x))
    {
      gt_ggc_m_9tree_node ((*x).type);
      gt_ggc_m_9tree_node ((*x).leader);
    }
}

void
gt_pch_nx_gimple_type_leader_entry_s (void *x_p)
{
  struct gimple_type_leader_entry_s * const x = (struct gimple_type_leader_entry_s *)x_p;
  if (gt_pch_note_object (x, x, gt_pch_p_26gimple_type_leader_entry_s, gt_ggc_e_26gimple_type_leader_entry_s))
    {
      gt_pch_n_9tree_node ((*x).type);
      gt_pch_n_9tree_node ((*x).leader);
    }
}

void
gt_pch_p_26gimple_type_leader_entry_s (ATTRIBUTE_UNUSED void *this_obj,
	void *x_p,
	ATTRIBUTE_UNUSED gt_pointer_operator op,
	ATTRIBUTE_UNUSED void *cookie)
{
  struct gimple_type_leader_entry_s * const x ATTRIBUTE_UNUSED = (struct gimple_type_leader_entry_s *)x_p;
  if ((void *)(x) == this_obj)
    op (&((*x).type), cookie);
  if ((void *)(x) == this_obj)
    op (&((*x).leader), cookie);
}

/* GC roots.  */

static void gt_ggc_ma_gimple_type_leader (void *);
static void
gt_ggc_ma_gimple_type_leader (ATTRIBUTE_UNUSED void *x_p)
{
  if (gimple_type_leader != NULL) {
    size_t i0;
    for (i0 = 0; i0 != (size_t)(GIMPLE_TYPE_LEADER_SIZE); i0++) {
      gt_ggc_m_9tree_node (gimple_type_leader[i0].type);
      gt_ggc_m_9tree_node (gimple_type_leader[i0].leader);
    }
    ggc_mark (gimple_type_leader);
  }
}

static void gt_pch_pa_gimple_type_leader
    (void *, void *, gt_pointer_operator, void *);
static void gt_pch_pa_gimple_type_leader (ATTRIBUTE_UNUSED void *this_obj,
      ATTRIBUTE_UNUSED void *x_p,
      ATTRIBUTE_UNUSED gt_pointer_operator op,
      ATTRIBUTE_UNUSED void * cookie)
{
  if (gimple_type_leader != NULL) {
    size_t i0;
    for (i0 = 0; i0 != (size_t)(GIMPLE_TYPE_LEADER_SIZE); i0++) {
      if ((void *)(gimple_type_leader) == this_obj)
        op (&(gimple_type_leader[i0].type), cookie);
      if ((void *)(gimple_type_leader) == this_obj)
        op (&(gimple_type_leader[i0].leader), cookie);
    }
    if ((void *)(&gimple_type_leader) == this_obj)
      op (&(gimple_type_leader), cookie);
  }
}

static void gt_pch_na_gimple_type_leader (void *);
static void
gt_pch_na_gimple_type_leader (ATTRIBUTE_UNUSED void *x_p)
{
  if (gimple_type_leader != NULL) {
    size_t i1;
    for (i1 = 0; i1 != (size_t)(GIMPLE_TYPE_LEADER_SIZE); i1++) {
      gt_pch_n_9tree_node (gimple_type_leader[i1].type);
      gt_pch_n_9tree_node (gimple_type_leader[i1].leader);
    }
    gt_pch_note_object (gimple_type_leader, &gimple_type_leader, gt_pch_pa_gimple_type_leader, gt_types_enum_last);
  }
}

EXPORTED_CONST struct ggc_root_tab gt_ggc_r_gt_gimple_h[] = {
  {
    &gimple_type_leader,
    1,
    sizeof (gimple_type_leader),
    &gt_ggc_ma_gimple_type_leader,
    &gt_pch_na_gimple_type_leader
  },
  LAST_GGC_ROOT_TAB
};

EXPORTED_CONST struct ggc_root_tab gt_ggc_rd_gt_gimple_h[] = {
  { &gimple_seq_cache, 1, sizeof (gimple_seq_cache), NULL, NULL },
  LAST_GGC_ROOT_TAB
};

EXPORTED_CONST struct ggc_cache_tab gt_ggc_rc_gt_gimple_h[] = {
  {
    &canonical_type_hash_cache,
    1,
    sizeof (canonical_type_hash_cache),
    &gt_ggc_mx_tree_int_map,
    &gt_pch_nx_tree_int_map,
    &tree_int_map_marked_p
  },
  {
    &type_hash_cache,
    1,
    sizeof (type_hash_cache),
    &gt_ggc_mx_tree_int_map,
    &gt_pch_nx_tree_int_map,
    &tree_int_map_marked_p
  },
  {
    &gimple_canonical_types,
    1,
    sizeof (gimple_canonical_types),
    &gt_ggc_mx_tree_node,
    &gt_pch_nx_tree_node,
    &ggc_marked_p
  },
  {
    &gimple_types,
    1,
    sizeof (gimple_types),
    &gt_ggc_mx_tree_node,
    &gt_pch_nx_tree_node,
    &ggc_marked_p
  },
  LAST_GGC_CACHE_TAB
};

EXPORTED_CONST struct ggc_root_tab gt_pch_rc_gt_gimple_h[] = {
  {
    &canonical_type_hash_cache,
    1,
    sizeof (canonical_type_hash_cache),
    &gt_ggc_m_P12tree_int_map4htab,
    &gt_pch_n_P12tree_int_map4htab
  },
  {
    &type_hash_cache,
    1,
    sizeof (type_hash_cache),
    &gt_ggc_m_P12tree_int_map4htab,
    &gt_pch_n_P12tree_int_map4htab
  },
  {
    &gimple_canonical_types,
    1,
    sizeof (gimple_canonical_types),
    &gt_ggc_m_P9tree_node4htab,
    &gt_pch_n_P9tree_node4htab
  },
  {
    &gimple_types,
    1,
    sizeof (gimple_types),
    &gt_ggc_m_P9tree_node4htab,
    &gt_pch_n_P9tree_node4htab
  },
  LAST_GGC_ROOT_TAB
};

