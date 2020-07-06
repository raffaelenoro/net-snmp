Date: 22 Mar 2019 

Version: 1.0

Author: Raffaele Noro


Introduction
============
Given the requirement for a configuration mechanism of the Smart EQ VST, the goal was to rely on a text file named "smarteq.config" where an administrator can enable/disable features at will, without having to write actual code. The configuration file will resemble something like the following:

smarteq.config
analysisEnabled = 1
timeAnalysisEnabled = 0
...
To that end, I decided to experiment with SNMP (Simple Network Management Protocol). SNMP has historically been widely used in managing network equipment (routers, gateways, set-top-boxes), thanks in large part to the IETF standardization efforts and the availability of software, applications and expertise.

In the SNMP context, an SNMP Agent runs on the device being managed, while an SNMP Manager contacts the agent to perform the management task requested by the user (for example, to modify the routing table on a network device).

The message exchanges are facilitated by a MIB (Management Information Base) which is just a formal data model that regulates the type and values of the managed objects.

While SNMP is traditionally used for network devices, there isn't a particular reason preventing its use in a different field where management and configuration are required. I had some experience with SNMP implementations, so I found it suitable for this project as well. In particular the following aspects seemed appealing:

- The syntax and semantics of a MIB as data model for the system
- The availability of open-source tools and frameworks
- The ease of integration into the existing system
- In the next sections I will describe the steps taken to incorporate SNMP in the Smart EQ VST.

Defining a MIB
==============
The first step is to model the objects that we want to configure using a MIB file. For example, we want to enable or disable the real-time analysis feature:

This is a scalar (i.e., a single value) modeled by a Boolean in our MIB. Other scalars will model the enable/disable of the Time Analysis (another Boolean), the type of a Filter (an Enumeration), its Frequency (a Float), and so on.

The array of EQ filters is modeled by a table of rows in our MIB. Each row consists of a few scalars organized in columns.

In the end our DTS-TOOL-SMARTEQ-MIB file contains:

	DTS-TOOL-SMARTEQ-MIB.txt
	--
	-- Scalar configuration parameters
	--
	 
	 
	analysisEnabled OBJECT-TYPE
	   SYNTAX Boolean
	   MAX-ACCESS read-write
	   STATUS current
	   DESCRIPTION "Global enable of the Analyzer sub-component"
	   ::= { smarteq 3 }
	 
	 
	 
	--
	-- Table of EQ Filters
	--
	 
	eqFilterTable OBJECT-TYPE
	   SYNTAX SEQUENCE OF EqFilterEntry
	   MAX-ACCESS not-accessible
	   STATUS current
	   DESCRIPTION "Table of EQ Filters"
	   ::= { smarteq 99 }
	 
	eqFilterEntry OBJECT-TYPE
	   SYNTAX EqFilterEntry
	   MAX-ACCESS not-accessible
	   STATUS current
	   DESCRIPTION "EQ Filter"
	   INDEX { eqFilterIndex }
	   ::= { eqFilterTable 1 }
	 
	EqFilterEntry ::= SEQUENCE {
	   eqFilterIndex Unsigned32,
	   eqFilterFrequency Float,
	   eqFilterGain Float,
	   eqFilterQ Float,
	   eqFilterType INTEGER,
	   eqFilterStatus RowStatus
	}
	 
	eqFilterIndex OBJECT-TYPE
	   SYNTAX Unsigned32
	   MAX-ACCESS read-only
	   STATUS current
	   DESCRIPTION "Index of the filter"
	   ::= { eqFilterEntry 1 }
	 
	eqFilterFrequency OBJECT-TYPE
	   SYNTAX Float
	   MAX-ACCESS read-write
	   STATUS current
	   DESCRIPTION "Center frequency of the filter"
	   ::= { eqFilterEntry 2 }
	 
	eqFilterGain OBJECT-TYPE
	   SYNTAX Float
	   MAX-ACCESS read-write
	   STATUS current
	   DESCRIPTION "Gain of the filter"
	   ::= { eqFilterEntry 3 }
	 
	eqFilterQ OBJECT-TYPE
	   SYNTAX Float
	   MAX-ACCESS read-write
	   STATUS current
	   DESCRIPTION "Q factor of the filter"
	   ::= { eqFilterEntry 4 }
	 
	eqFilterType OBJECT-TYPE
	   SYNTAX INTEGER {
				 traditionalEQ(0),
				 peakingEQ(1),
				 lowShelf(2),
				 highShelf(3),
				 lowPass(4),
				 highPass(5),
				 bandPass(6),
				 allPass(7),
				 notch(8)
			  }
	   MAX-ACCESS read-write
	   STATUS current
	   DESCRIPTION "Type of the filter"
	   ::= { eqFilterEntry 5 }
	 
	eqFilterStatus OBJECT-TYPE
	   SYNTAX RowStatus
	   MAX-ACCESS read-create
	   STATUS current
	   DESCRIPTION "Status"
	   ::= { eqFilterEntry 9 }
   
The syntax of a MIB is required to follow the Abstract Syntax Notation One (ASN.1), hence the structure of the fields and the various keywords.

Implementing the SNMP Agent
===========================
The SNMP Agent is the body of code that runs on the system to be managed. A well-supported, off-the-shelf framework for implementing our own agent is the Net-SNMP, which provides the majority of the infrastructure (connectivity, transport, authentication, validation of requests, etc.) and lets the developer handle the finer details of each request (the glue logic that is system-specific).

The agent runs in its own thread of execution, therefore completely isolated from the audio data path and the UI rendering.

In our case we need to provide the handler code for the objects defined in our MIB: Net-SNMP contains a utility called mib2c for creating a stub handler by parsing the MIB itself. We use it in the following way:

	$ env MIBS="+DTS-TOOL-SMARTEQ-MIB" mib2c -c mib2c.scalar.conf smarteq
	writing to smarteq.h
	writing to smarteq.c
	running indent on smarteq.h
	running indent on smarteq.c
	
The generated stub smarteq.c needs to be fine-tuned to properly handle the requests:

	smarteq.c
	/** Initializes the smarteq module */
	void
	init_smarteq(void)
	{
		const oid analysisEnabled_oid[] = { 1,3,6,1,4,1,5220,2,101,3 };
	 
	  DEBUGMSGTL(("smarteq", "Initializing\n"));
	 
		netsnmp_register_scalar(
			netsnmp_create_handler_registration("analysisEnabled", handle_analysisEnabled,
								   analysisEnabled_oid, OID_LENGTH(analysisEnabled_oid),
								   HANDLER_CAN_RWRITE
			));
	}
	 
	int
	handle_analysisEnabled(netsnmp_mib_handler *handler,
							  netsnmp_handler_registration *reginfo,
							  netsnmp_agent_request_info   *reqinfo,
							  netsnmp_request_info         *requests)
	{
		int ret;
		/* We are never called for a GETNEXT if it's registered as a
		   "instance", as it's "magically" handled for us.  */
	 
		/* a instance handler also only hands us one request at a time, so
		   we don't need to loop over a list of requests; we'll only get one. */
	 
	/**/const int enable = get_analysis_enabled();
	 
		switch(reqinfo->mode) {
	 
			case MODE_GET:
				snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
	/**/                                 &enable,
	/**/                                 sizeof(enable));
				break;
	 
			/*
			 * SET REQUEST
			 *
			 * multiple states in the transaction.  See:
			 * http://www.net-snmp.org/tutorial-5/toolkit/mib_module/set-actions.jpg
			 */
			case MODE_SET_RESERVE1:
					/* or you could use netsnmp_check_vb_type_and_size instead */
				ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
				if ( ret != SNMP_ERR_NOERROR ) {
					netsnmp_set_request_error(reqinfo, requests, ret );
				}
				break;
	 
			case MODE_SET_RESERVE2:
	/**/        ret = netsnmp_check_vb_size(requests->requestvb, sizeof(enable));
	/**/        if ( ret != SNMP_ERR_NOERROR ) {
					netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
				}
				break;
	 
			case MODE_SET_FREE:
				/* XXX: free resources allocated in RESERVE1 and/or
				   RESERVE2.  Something failed somewhere, and the states
				   below won't be called. */
				break;
	 
			case MODE_SET_ACTION:
	/**/        requests->requestvb->data = (void*)enable;
	/**/        requests->requestvb->dataFreeHook = freeHook;
	/**/        if (set_analysis_enabled(*requests->requestvb->val.integer)) {
					netsnmp_set_request_error(reqinfo, requests, /* some error */);
				}
				break;
	 
			case MODE_SET_COMMIT:
				/* XXX: delete temporary storage */
	/**/        if (set_analysis_enabled(enabled)) {
					/* try _really_really_ hard to never get to this point */
					netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
				}
				break;
	 
			case MODE_SET_UNDO:
				/* XXX: UNDO and return to previous value for the object */
	/**/       if (set_analysis_enabled((int)requests->requestvb->data)) {
					/* try _really_really_ hard to never get to this point */
					netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_UNDOFAILED);
				}
				break;
	 
			default:
				/* we should never get here, so this is a really bad error */
				snmp_log(LOG_ERR, "unknown mode (%d) in handle_analysisEnabled\n", reqinfo->mode );
				return SNMP_ERR_GENERR;
		}
	 
		return SNMP_ERR_NOERROR;
	}

Most of the code is boiler-plate, the code lines inserted by the developer are marked with '/**/'. 

The next change is to create a thread that runs and listens to management requests (enable/disable the analysis, etc.). I made the following changes to the startup code of the VST:

	CustomProcessor.cpp
	struct SnmpThread : public Thread
	{
		SnmpThread() : Thread("snmpThread") {}
		~SnmpThread() { StopSnmpAgent(); }
		void run()
		{
			String fileName = File::getCurrentWorkingDirectory().getChildFile("smarteq.config").getFullPathName();
	 
			init_config(fileName);            /* this configures the tool from the parameters in "smarteq.config" */
	 
			if (globalConfig.snmpdEnabled)
			{
				char *argv[] = {
					"", /* "-Dall", "-Lf", "snmpd.log", */
					"-C",
					"--rocommunity=public",   /* these are the credentials for reading/getting parameters */
					"--rwcommunity=private",  /* these are the credentials for writing/setting parameters */
					"UDP:11161"               /* this is the protocol and port that we will listen */
				};
				int nargs = sizeof(argv) / sizeof(argv[0]);
	 
				SnmpDaemonMain(nargs, argv);  /* starts the daemon and loops (listens) forever */
			}
		}
	};
	static SnmpThread* pSnmpThread;
	 
	...
	 
	customProcessor::customProcessor()
	{
	 
		...
	 
	/**/pSnmpThread = new SnmpThread();
	/**/pSnmpThread->startThread();
	}

Finally the running SNMP agent has to be instructed to include some MIB modules, including our own. This happens in the files mib_module_includes.h and mib_module_inits.h:

	mib_module_includes.h
	/* This file is automatically generated by configure.  Do not modify by hand. */
	#include "smarteq.h"
	#include "eqFilterTable.h"
	#include "mibgroup/mibII/snmp_mib.h"
	#include "mibgroup/mibII/system_mib.h"
	#include "mibgroup/mibII/sysORTable.h"
	#include "mibgroup/mibII/vacm_vars.h"
	#include "mibgroup/mibII/vacm_context.h"
	
	mib_module_inits.h
	/* This file is automatically generated by configure.  Do not modify by hand. */
	if (should_init("smarteq")) init_smarteq();
	if (should_init("eqFilterTable")) init_eqFilterTable();
	if (should_init("snmp_mib")) init_snmp_mib();
	if (should_init("system_mib")) init_system_mib();
	if (should_init("sysORTable")) init_sysORTable();
	if (should_init("vacm_vars")) init_vacm_vars();
	if (should_init("vacm_context")) init_vacm_context();
	
Managing the SNMP Agent
=======================
For the manager there is no need to write any new code as numerous free and commercial SNMP managers exist (due to the fact that MIB syntax and semantics are standardized). In Linux/Cygwin I use the SNMP command-line tools from the snmpcmd family (snmpget, snmpset, snmpwalk, etc), for example to disable the analysis I use:

	$ snmpget -v 2c -c public -M +/path/to/dts-mibs -m ALL localhost:11161 DTS-TOOL-SMARTEQ-MIB::analysisEnabled.0
	DTS-TOOL-SMARTEQ-MIB::analysisEnabled.0 = INTEGER: true(1)
	 
	 
	$ snmpset -v 2c -c private -M +/path/to/dts-mibs -m ALL localhost:11161 DTS-TOOL-SMARTEQ-MIB::analysisEnabled.0 i 0
	DTS-TOOL-SMARTEQ-MIB::analysisEnabled.0 = INTEGER: false(0)
	
There are also graphical SNMP managers like MibBrowser (or tkmib):


Agent initialization
====================
The whole purpose of incorporating the SNMP Agent is to be able to create and replay a desired configuration to tailor the VST for a specific usage.

This can be attained as follows:

- Start the VST
- Use the SNMP Manager to enable/disable any number of features
- Walk the SNMP Agent and store the result in a file called smarteq.config

	$ snmpwalk -v 2c -c public -M +/path/to/dts-mibs -m ALL -O eQ localhost:11161 DTS-TOOL-SMARTEQ-MIB::smarteq > smarteq.config

- Restart the VST

The content (and format) of the resulting configuration file is the following:

	smarteq.config
	DTS-TOOL-SMARTEQ-MIB::toolVersion.0 = 2.2.0.0
	DTS-TOOL-SMARTEQ-MIB::snmpEnabled.0 = 1
	DTS-TOOL-SMARTEQ-MIB::analysisEnabled.0 = 1
	DTS-TOOL-SMARTEQ-MIB::timeAnalysisEnabled.0 = 1
	
This can also be manually edited (especially for turning off the SNMP Agent in production), provided that the format of each line is preserved.
