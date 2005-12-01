/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.67 $ of : mfd-interface.m2c,v $
 *
 * $Id$
 */
/** @defgroup interface: Routines to interface to Net-SNMP
 *
 * \warning This code should not be modified, called directly,
 *          or used to interpret functionality. It is subject to
 *          change at any time.
 * 
 * @{
 */
/*
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 * ***                                                               ***
 * ***  NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE  ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THIS FILE DOES NOT CONTAIN ANY USER EDITABLE CODE.      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THE GENERATED CODE IS INTERNAL IMPLEMENTATION, AND      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***    IS SUBJECT TO CHANGE WITHOUT WARNING IN FUTURE RELEASES.   ***
 * ***                                                               ***
 * ***                                                               ***
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 */
#ifndef IPV6INTERFACETABLE_INTERFACE_H
#define IPV6INTERFACETABLE_INTERFACE_H

#ifdef __cplusplus
extern          "C" {
#endif


#include "ipv6InterfaceTable.h"


    /*
     ********************************************************************
     * Table declarations
     */

    /*
     * PUBLIC interface initialization routine 
     */
    void
     
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        _ipv6InterfaceTable_initialize_interface
        (ipv6InterfaceTable_registration * user_ctx, u_long flags);
    void
     
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        _ipv6InterfaceTable_shutdown_interface
        (ipv6InterfaceTable_registration * user_ctx);

         
         
         
         
         
         
        ipv6InterfaceTable_registration
        * ipv6InterfaceTable_registration_get(void);

         
         
         
         
         
         
        ipv6InterfaceTable_registration
        * ipv6InterfaceTable_registration_set
        (ipv6InterfaceTable_registration * newreg);

    netsnmp_container *ipv6InterfaceTable_container_get(void);
    int             ipv6InterfaceTable_container_size(void);

    u_int           ipv6InterfaceTable_dirty_get(void);
    void            ipv6InterfaceTable_dirty_set(u_int status);

         
         
         
         
         
         
        ipv6InterfaceTable_rowreq_ctx
        * ipv6InterfaceTable_allocate_rowreq_ctx(void *);
    void
     
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        ipv6InterfaceTable_release_rowreq_ctx(ipv6InterfaceTable_rowreq_ctx
                                              * rowreq_ctx);

    int             ipv6InterfaceTable_index_to_oid(netsnmp_index *
                                                    oid_idx,
                                                    ipv6InterfaceTable_mib_index
                                                    * mib_idx);
    int             ipv6InterfaceTable_index_from_oid(netsnmp_index *
                                                      oid_idx,
                                                      ipv6InterfaceTable_mib_index
                                                      * mib_idx);

    /*
     * access to certain internals. use with caution!
     */
    void
     
        
        
        
        
        
        
        
        
        
             ipv6InterfaceTable_valid_columns_set(netsnmp_column_info *vc);

    /*
     */
    void            ipv6InterfaceTable_lastChange_set(u_long uptime);

#ifdef __cplusplus
}
#endif
#endif                          /* IPV6INTERFACETABLE_INTERFACE_H */
