#ifndef SMARTEQ_UTILS_H
#define SMARTEQ_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

	int SnmpDaemonMain(int, char**);
	void StopSnmpAgent(void);

	const char* get_tool_version();

	const int get_snmp_enabled();

	const int get_analysis_enabled();
	int set_analysis_enabled(int enable);

	const int get_time_analysis_enabled();
	int set_time_analysis_enabled(int enable);

	const int get_save_wav_files_enabled();
	int set_save_wav_files_enabled(int enable);

	int get_number_microphones();

	const int get_microphone_selected();
	int set_microphone_selected(int selected);

	void get_eq_filter_defaults(double* frequency, double* gain, double* q, int* type);
	int get_number_eq_filters();
	int get_eq_filter_parameters(int index, double* frequency, double* gain, double* q, int* type);
	int set_eq_filter_parameters(int index, double frequency, double gain, double q, int type);
	int add_eq_filter();
	int remove_eq_filter(int index);

#ifdef __cplusplus
}
#endif

#endif /* SMARTEQ_UTILS_H */
