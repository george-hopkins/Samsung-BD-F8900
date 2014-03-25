IP_ROOT := lwip_releases/1.3.2
IP_ROOT_CORE := $(IP_ROOT)/core
IP_ROOT_NETIF := $(IP_ROOT)/netif
IP_ROOT_INC := $(IP_ROOT)/include

MODULE_SRC += \
            csr_ip_handler.c \
            csr_ip_ether_sef.c \
            csr_ip_ifconfig_sef.c \
            csr_ip_socket_sef.c \
            csr_ip_internal_sef.c \
            csr_ip_internal_lib.c \
            csr_ip_util.c \
            $(IP_ROOT_CORE)/init.c \
            $(IP_ROOT_CORE)/dhcp.c \
            $(IP_ROOT_CORE)/mem.c \
            $(IP_ROOT_CORE)/memp.c \
            $(IP_ROOT_CORE)/netif.c \
            $(IP_ROOT_CORE)/pbuf.c \
            $(IP_ROOT_CORE)/raw.c \
            $(IP_ROOT_CORE)/stats.c \
            $(IP_ROOT_CORE)/sys.c \
            $(IP_ROOT_CORE)/tcp.c \
            $(IP_ROOT_CORE)/tcp_in.c \
            $(IP_ROOT_CORE)/tcp_out.c \
            $(IP_ROOT_CORE)/udp.c \
            $(IP_ROOT_CORE)/dns.c \
            $(IP_ROOT_CORE)/ipv4/autoip.c \
            $(IP_ROOT_CORE)/ipv4/icmp.c \
            $(IP_ROOT_CORE)/ipv4/igmp.c \
            $(IP_ROOT_CORE)/ipv4/inet.c \
            $(IP_ROOT_CORE)/ipv4/inet_chksum.c \
            $(IP_ROOT_CORE)/ipv4/ip.c \
            $(IP_ROOT_CORE)/ipv4/ip_addr.c \
            $(IP_ROOT_CORE)/ipv4/ip_frag.c \
            $(IP_ROOT_CORE)/ipv4/nat.c \
            $(IP_ROOT_NETIF)/etharp.c \
            $(IP_ROOT_NETIF)/loopif.c

INC += -I$(IP_ROOT_INC)/ipv4 -I$(IP_ROOT_INC)
