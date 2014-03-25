The HID host demo application is only supported on HCI-builds due to the fact 
that HID devices may need SDP-records larger than supported by RFCOMM-build.
gcc ss_demo_test.c -o ./ss_demo_test -L ./ -l csr_bt_ss_demo_app -l csr_socket -l csr_panic -l csr_pmem -l pthread

