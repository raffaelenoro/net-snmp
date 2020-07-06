#include "CustomEditor.h"

#include "smarteq_utils.h"

static MainCurveComponent* get_curve_component()
{
	customProcessor* processor = customProcessor::getCurrentProcessor();
	MainCurveComponent* component = nullptr;

	if (processor)
	{
		component = dynamic_cast<MainCurveComponent*>(processor->getCurveComponent());
	}

	return component;
}

const char *get_tool_version()
{
	return globalConfig.version;
}

const int get_snmp_enabled()
{
	return globalConfig.snmpdEnabled;
}

const int get_analysis_enabled()
{
	return globalConfig.analysisEnabled;
}

int set_analysis_enabled(int enable)
{
	MainCurveComponent* curveComponent = get_curve_component();
	ButtonsComponent* buttonsComponent;

	globalConfig.analysisEnabled = (enable != 0);

	if (curveComponent && (buttonsComponent = curveComponent->getButtonsComponent()))
	{
		if ( !globalConfig.analysisEnabled || !buttonsComponent->isAnalysisOn() )
		{
			buttonsComponent->stopAnalysis();
		}
	}

	return 0;
}

const int get_time_analysis_enabled()
{
	return globalConfig.timeAnalysisEnabled;
}

int set_time_analysis_enabled(int enable)
{
	MainCurveComponent* curveComponent = get_curve_component();
	ButtonsComponent* buttonsComponent;

	globalConfig.timeAnalysisEnabled = (enable != 0);

	if (curveComponent && (buttonsComponent = curveComponent->getButtonsComponent()))
	{
		if (buttonsComponent->isAnalysisOn())
		{
			curveComponent->stopTimeAnalysis();
		}
	}

	return 0;
}

const int get_save_wav_files_enabled()
{
	return globalConfig.saveWavFilesEnabled;
}

int set_save_wav_files_enabled(int enable)
{
	globalConfig.saveWavFilesEnabled = (enable != 0);

	return 0;
}

int get_number_microphones()
{
	return globalConfig.numberOfMicrophones;
}

const int get_microphone_selected()
{
	return globalConfig.microphoneSelected;
}

int set_microphone_selected(int selected)
{
	MainCurveComponent* curveComponent = get_curve_component();
	ButtonsComponent* buttonsComponent;

	if ( (selected < globalConfig.numberOfMicrophones) && (selected != globalConfig.microphoneSelected) )
	{
		globalConfig.microphoneSelected = selected;

		if (curveComponent && (buttonsComponent = curveComponent->getButtonsComponent()))
		{
			buttonsComponent->toggleMicrophone();
		}
	}

	return 0;
}

void get_eq_filter_defaults(double* frequency, double* gain, double* q, int* type)
{
	*frequency = dtsFilterData::default_frequency;
	*gain = dtsFilterData::default_gain;
	*q = dtsFilterData::default_q;
	*type = dtsFilterData::default_type;
}

int get_number_eq_filters()
{
	MainCurveComponent* curveComponent = get_curve_component();
	ButtonsComponent* buttonsComponent;
	dtsCurveFittingLib* fitting;
	int number_eq_filters = 0;

	if ( curveComponent&& (buttonsComponent = curveComponent->getButtonsComponent()) )
	{
		fitting = curveComponent->getCurrentFilterProfile(curveComponent->getSampleRate(), buttonsComponent->isLeft());

		number_eq_filters = fitting->getNumberOfFilters();
	}

	return number_eq_filters;
}

int get_eq_filter_parameters(int index, double* frequency, double* gain, double* q, int* type)
{
	MainCurveComponent* curveComponent = get_curve_component();
	ButtonsComponent* buttonsComponent;
	dtsCurveFittingLib* fitting;
	dtsFilterTypes filterType;
	int rc = -1;

	if ( curveComponent && (buttonsComponent = curveComponent->getButtonsComponent()) )
	{
		fitting = curveComponent->getCurrentFilterProfile(curveComponent->getSampleRate(), buttonsComponent->isLeft());

		fitting->getFilterData(index, *frequency, *gain, *q, filterType);
		*type = filterType;

		rc = 0;
	}

	return rc;
}

int set_eq_filter_parameters(int index, double frequency, double gain, double q, int type)
{
	MainCurveComponent* curveComponent = get_curve_component();
	ButtonsComponent* buttonsComponent;
	dtsCurveFittingLib* fitting;
	int rc = -1;

	if (curveComponent && (buttonsComponent = curveComponent->getButtonsComponent()))
	{
		fitting = curveComponent->getCurrentFilterProfile(curveComponent->getSampleRate(), buttonsComponent->isLeft());

		fitting->setFilterData(index, frequency, gain, q, dtsFilterTypes(type));

		fitting->hasChanged();
		buttonsComponent->getFilters()->loadData();
		buttonsComponent->getPlot()->refreshData();

		rc = 0;
	}

	return rc;
}

int add_eq_filter()
{
	MainCurveComponent* curveComponent = get_curve_component();
	ButtonsComponent* buttonsComponent;
	dtsCurveFittingLib* fitting;
	int rc = -1;

	if (curveComponent && (buttonsComponent = curveComponent->getButtonsComponent()))
	{
		fitting = curveComponent->getCurrentFilterProfile(curveComponent->getSampleRate(), buttonsComponent->isLeft());

		fitting->addFilter();

		fitting->hasChanged();
		buttonsComponent->getFilters()->setNumberOfFilters();
		buttonsComponent->getFilters()->loadData();
		buttonsComponent->getPlot()->refreshData();

		rc = 0;
	}

	return rc;
}

int remove_eq_filter(int index)
{
	MainCurveComponent* curveComponent = get_curve_component();
	ButtonsComponent* buttonsComponent;
	dtsCurveFittingLib* fitting;
	int rc = -1;

	if (curveComponent && (buttonsComponent = curveComponent->getButtonsComponent()))
	{
		fitting = curveComponent->getCurrentFilterProfile(curveComponent->getSampleRate(), buttonsComponent->isLeft());

		fitting->removeFilter(index);

		fitting->hasChanged();
		buttonsComponent->getFilters()->setNumberOfFilters();
		buttonsComponent->getFilters()->loadData();
		buttonsComponent->getPlot()->refreshData();

		rc = 0;
	}

	return rc;
}
