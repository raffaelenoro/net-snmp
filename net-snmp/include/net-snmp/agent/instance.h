/* testhandler.h */

/* The instance helper is designed to simplify the task of adding simple
 * instances to the mib tree.
 */

/* GETNEXTs are auto-converted to a GET.
 * non-valid GETs are dropped.
 * The client can assume that if you're called for a GET, it shouldn't
 * have to check the oid at all.  Just answer.
 */

int register_instance(netsnmp_handler_registration *reginfo);
int register_read_only_instance(netsnmp_handler_registration *reginfo);

#define INSTANCE_HANDLER_NAME "instance"

netsnmp_mib_handler *get_instance_handler(void);
   
int register_read_only_ulong_instance(const char *name,
                                      oid *reg_oid, size_t reg_oid_len,
                                      u_long *it, Netsnmp_Node_Handler *subhandler);
int register_ulong_instance(const char *name, oid *reg_oid, size_t reg_oid_len,
                            u_long *it, Netsnmp_Node_Handler *subhandler);
int register_read_only_counter32_instance(const char *name,
                                          oid *reg_oid, size_t reg_oid_len,
                                          u_long *it, Netsnmp_Node_Handler *subhandler);
int register_read_only_long_instance(const char *name,
                                     oid *reg_oid, size_t reg_oid_len,
                                     long *it, Netsnmp_Node_Handler *subhandler);
int register_long_instance(const char *name, oid *reg_oid, size_t reg_oid_len,
                           long *it, Netsnmp_Node_Handler *subhandler);

int register_int_instance(const char *name, oid *reg_oid, size_t reg_oid_len,
                          int *it, Netsnmp_Node_Handler *subhandler);


Netsnmp_Node_Handler instance_helper_handler;
Netsnmp_Node_Handler instance_ulong_handler;
Netsnmp_Node_Handler instance_long_handler;
Netsnmp_Node_Handler instance_int_handler;
Netsnmp_Node_Handler instance_counter32_handler;
