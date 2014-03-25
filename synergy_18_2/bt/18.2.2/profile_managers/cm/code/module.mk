CM_LIB_SRC += \
	csr_bt_cm_dm_acl_detach_handler.c \
	csr_bt_cm_dm_afh_handler.c \
	csr_bt_cm_dm_arrival_handler.c \
	csr_bt_cm_dm_cache_params_handler.c \
	csr_bt_cm_dm_connectable_handler.c \
	csr_bt_cm_dm_dut_handler.c \
	csr_bt_cm_dm_extended_inquiry_response.c \
	csr_bt_cm_dm_features_handler.c \
	csr_bt_cm_dm_get_link_quality_handler.c \
	csr_bt_cm_dm_iac_handler.c \
	csr_bt_cm_dm_inquiry_handler.c \
	csr_bt_cm_dm_maintenance_handler.c \
	csr_bt_cm_dm_mode_change_handler.c \
	csr_bt_cm_dm_provider.c \
	csr_bt_cm_dm_read_clock_handler.c \
	csr_bt_cm_dm_read_failed_contact_counter.c \
	csr_bt_cm_dm_read_local_bd_addr_handler.c \
	csr_bt_cm_dm_read_local_name_handler.c \
	csr_bt_cm_dm_read_remote_name_handler.c \
	csr_bt_cm_dm_read_remote_version_handler.c \
	csr_bt_cm_dm_read_local_version_handler.c \
	csr_bt_cm_dm_read_rssi_handler.c \
	csr_bt_cm_dm_read_tx_power_level_handler.c \
	csr_bt_cm_dm_sc_handler.c \
	csr_bt_cm_dm_sc_lib.c \
	csr_bt_cm_dm_sc_ssp_handler.c \
	csr_bt_cm_dm_sc_ssp_lib.c \
	csr_bt_cm_dm_sco_handler.c \
	csr_bt_cm_dm_set_event_filter_handler.c \
	csr_bt_cm_dm_set_local_name_handler.c \
	csr_bt_cm_dm_sniff_sub_rate_handler.c \
	csr_bt_cm_dm_switch_role_handler.c \
	csr_bt_cm_dm_write_auto_flush_timeout.c \
	csr_bt_cm_dm_write_inquiry_handler.c \
	csr_bt_cm_dm_write_link_super_visiontimeout.c \
	csr_bt_cm_dm_write_lp_settings_handler.c \
	csr_bt_cm_dm_write_page_handler.c \
	csr_bt_cm_dm_write_page_to_handler.c \
	csr_bt_cm_bccmd_arrival_handler.c \
	csr_bt_cm_events_handler.c \
	csr_bt_cm_main.c \
	csr_bt_cm_maintenance_handler.c \
	csr_bt_cm_l2cap_conftab.c \
	csr_bt_cm_private_lib.c \
	csr_bt_cm_profile_provider.c \
	csr_bt_cm_sdc_arrival_handler.c \
	csr_bt_cm_sdc_handler.c \
	csr_bt_cm_service_manager_provider.c \
	csr_bt_cm_callback_q.c \
	csr_bt_cm_free_down.c \
	csr_bt_cm_dm_encryption_key_size.c

CM_API_SRC += \
	csr_bt_cm_lib.c \
	csr_bt_cm_free_up.c \
	csr_bt_cm_free_hc_impl.c \
	csr_bt_cm_free_handcoded.c

# BNEP support begin
CM_LIB_SRC += \
	csr_bt_cm_bnep_arrival_handler.c \
	csr_bt_cm_bnep_connect_handler.c \
	csr_bt_cm_bnep_data_handler.c \
	csr_bt_cm_bnep_disconnect_handler.c \
	csr_bt_cm_bnep_maintenance_handler.c \
	csr_bt_cm_bnep_register_handler.c 
# BNEP support end

# L2CAP support begin
ifeq ($(CSR_AMP_ENABLE),1)
CM_LIB_SRC += csr_bt_cm_l2cap_amp_handler.c
endif
CM_LIB_SRC += \
	csr_bt_cm_l2cap_arrival_handler.c \
	csr_bt_cm_l2cap_config_handler.c \
	csr_bt_cm_l2cap_connect_handler.c \
	csr_bt_cm_l2cap_data_handler.c \
	csr_bt_cm_l2cap_disconnect_handler.c \
	csr_bt_cm_l2cap_maintenance_handler.c \
	csr_bt_cm_l2cap_register_handler.c \
	csr_bt_cm_l2cap_connless_handler.c
# L2CAP support end

# LE support begin
ifeq ($(CSR_BT_LE_ENABLE),1)
CM_LIB_SRC += csr_bt_cm_le_handler.c
endif

# RFCOMM support begin
CM_LIB_SRC += csr_bt_cm_rfc_sco_handler.c
ifeq ($(CSR_AMP_ENABLE),1)
CM_LIB_SRC += csr_bt_cm_rfc_amp_handler.c
endif
CM_LIB_SRC += \
	csr_bt_cm_rfc_accept_connect_handler.c \
	csr_bt_cm_rfc_arrival_handler.c \
	csr_bt_cm_rfc_connect_handler.c \
	csr_bt_cm_rfc_control_handler.c \
	csr_bt_cm_rfc_data_handler.c \
	csr_bt_cm_rfc_maintenance_handler.c\
	csr_bt_cm_rfc_port_neg_handler.c \
	csr_bt_cm_rfc_register_handler.c \
	csr_bt_cm_rfc_release_handler.c
# RFCOMM support end

# Common AMP support begin
ifeq ($(CSR_AMP_ENABLE),1)
CM_LIB_SRC += csr_bt_cm_common_amp.c
endif
# Common AMP support end

LIB_SRC += $(CM_LIB_SRC)
API_SRC += $(CM_API_SRC)

PM_LIB_OBJ += $(addprefix cm/code/,$(CM_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix cm/code/,$(CM_API_SRC:.c=.o))
