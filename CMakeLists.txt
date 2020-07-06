cmake_minimum_required ( VERSION 3.2.0 )

project(pps-snmp-lib)

include_directories(${CMAKE_CURRENT_LIST_DIR}/dts-agent
                    ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent
                    ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup
                    ${CMAKE_CURRENT_LIST_DIR}/net-snmp/include
                    ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib
                    ${CMAKE_CURRENT_LIST_DIR}/net-snmp/win32)

file(GLOB SNMPSOURCES ${CMAKE_CURRENT_LIST_DIR}/dts-agent/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/dts-agent/*.cpp
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/helpers/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/agentx/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/mibII/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/snmpv3/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/target/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/util_funcs/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/utilities/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/*.c
                      ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/transports/*.c)

list(REMOVE_ITEM SNMPSOURCES ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/snmp_perl.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/helpers/table_generic.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/mibII/ipCidrRouteTable.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/mibII/ipCidrRouteTable_access.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/mibII/ipv6.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/mibII/kernel_linux.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/mibII/kernel_netbsd.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/mibII/kernel_sysctl.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/util_funcs/Exit.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/agent/mibgroup/util_funcs/get_pid_from_inode.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/dir_utils.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/snmpksm.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/snmptsm.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/strtol.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/strtoul.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/transports/snmpAAL5PVCDomain.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/transports/snmpIPXDomain.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/transports/snmpSSHDomain.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/transports/snmpSTDDomain.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/transports/snmpTLSBaseDomain.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/transports/snmpTLSTCPDomain.c
                             ${CMAKE_CURRENT_LIST_DIR}/net-snmp/snmplib/transports/snmpUnixDomain.c)

add_library(pps-snmp-lib STATIC ${SNMPSOURCES})
