#pragma once

#define UNIT  -0


#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>



#include "ByteCode.h"
///////////////////////////////////////////////////////////////////////////
///////////////////time measuring mechanism////////////////////////////////
///////////////////////////////////////////////////////////////////////////


#ifndef Forwarding
#define Forwarding(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#endif

/**
* @ class measure
* @ brief Class to measure the execution time of a callable
*/
template <
	typename TimeT = std::chrono::nanoseconds, class ClockT = std::chrono::high_resolution_clock
>
struct measure
{
	/**
	* @ fn    execution
	* @ brief Returns the quantity (count) of the elapsed time as TimeT units
	*/
	template<typename F, typename ...Args>
	static typename TimeT::rep execution(F&& func, Args&&... args)
	{
		auto start = ClockT::now();

		Forwarding(func)(std::forward<Args>(args)...);

		auto duration = std::chrono::duration_cast<TimeT>(ClockT::now() - start);

		return duration.count();
	}

	/**
	* @ fn    duration
	* @ brief Returns the duration (in chrono's type system) of the elapsed time
	*/
	template<typename F, typename... Args>
	static TimeT duration(F&& func, Args&&... args)
	{
		auto start = ClockT::now();

		Forwarding(func)(std::forward<Args>(args)...);

		return std::chrono::duration_cast<TimeT>(ClockT::now() - start);
	}
};






class Statistics
{
public:
	struct measurement {
		std::string					mName;
		std::time_t					mTimepoint;
		  long long				mDuration;

		  long mAccumaleted = 0;
		  long mAverage = 0;
		  long mMin = 0, mMax = 0;

		measurement() = default;
		measurement(const std::string& name,  long long  duration,std::time_t tp) :
			mName(name), mDuration(duration), mTimepoint{tp} {}
	};
public:
	Statistics() {
		//el::Loggers::configureFromGlobal("Logger.config");
	}
	std::vector<measurement>			mMeasurements;
	std::vector<std::string>			mResults;
	

public:

	void AddMeasurement( const measurement& m)
	{
		mMeasurements.push_back(m);
	}

	void doStats() 
	{
		std::cout << "\n\n";
		std::cout << "Name\t---> |\t Min\t\t| \t Max\t\t| \t Average\t| \t Sum\t\t|    count(ct)\t|        avrg/ct\t|\n";
		std::cout << "==================================================================================================="
			      <<"==============================================\n";
		long double 
			SumSum = 0,
			_Avg = 0,
			_Min = 0,
			_Max = 0,
			_count = 0,
			_avgcount=0;

		for (int i = 1; i < vm::MAXCODE - 1; i++)
		{
			//std::stringstream					buffer;
			long double //HACK: WHAT THE FUCK IS GOING ON WHY DOES std::numeric DON'T WORK ANYMORE IF easylogger IS INCLUDED
				Min = 1000000000,// std::numeric_limits<long double>::max() ,
				Max = -1000000000,//std::numeric_limits<long double>::min(),
				Sum = 0,
				Average = 0.0f;

			size_t	count = 0;
			bool used = false;

			mMeasurements.erase(
				std::remove_if(
					std::begin(mMeasurements),
					std::end(mMeasurements),
					[&](measurement& item) -> bool 
			{

				if (item.mName == vm::InstructionCode[i].mName) // Filter current Instruction
				{
					used = true;
					if (Min > item.mDuration)
						Min = item.mDuration;
					if (Max < item.mDuration)
						Max = item.mDuration;
					Sum += item.mDuration;
					count++;
					/*LOG(INFO) << ";"
						<< item.mName 
						<< ";"
						<< item.mDuration
						<<"\n";*/
					return true;

				}
				return false;

			}), std::end(mMeasurements));

			if (!used)
			{
				Min = 0, Max = 0, Sum = 0, Average = 0.0f, count = 0;
				continue;
			}

			Average = ((long double)Sum) / count;
			//buffer << vm::InstructionCode[i]->mName << "\t---> [\t " << Min << "ns, " << Max << ", " << Average << ", "<<Sum <<"]\n";
			std::cout.precision(2);
			std::cout << std::right
				<< vm::InstructionCode[i].mName << "\t---> |\t "
				<< Min << " ns \t|\t "
				<< Max << " ns \t|\t "
				<< Average << " ns \t|\t "
				<< Sum << " ns \t|\t"
				<< count << "\t| \t" << std::setprecision(6)
				<< Average / count<<" ns/ct \t|\n";

			//this->mResults.push_back(buffer.str());
			SumSum += Sum;
			_Min += Min;
			_Max += Max;
			_Avg += Average;
			_count += count;
			_avgcount += Average / count;
		}
		std::cout << "-----------------------------------------------------------------------------------------"
			<<"--------------------------------------------------------\n";
		std::cout.precision(7);
		std::cout << "Sum \t     |\t "
			<< _Min*std::pow(10, UNIT) << " ns\t| \t "
			<< _Max*std::pow(10, UNIT) << " ns\t| \t "
			<< _Avg*std::pow(10, UNIT) << " ns\t| \t "
			<< SumSum*std::pow(10, UNIT) << " ns\t| \t"
			<< _count << "\t| \t"
			<< _avgcount << " \t| \n";

		std::cout << "Average\t     |\t " 
			<< _Min*std::pow(10, UNIT) / _count << " ns/ct\t| \t " 
			<< _Max*std::pow(10, UNIT) / _count << " ns/ct\t| \t "
			<< _Avg*std::pow(10, UNIT) / _count << " ns/ct\t| \t "
			<< SumSum*std::pow(10, UNIT) / _count << "ns/ct\t| \t" <<  "\t|\n\n";
		
	}

};