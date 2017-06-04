#pragma once

#include <string>
#include <sstream>

#include <chrono>
#include <vector>
#include <iostream>

#include "ByteCode.h"
///////////////////////////////////////////////////////////////////////////
///////////////////time measuring mechanism////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#define fw(what) std::forward<decltype(what)>(what)

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

		fw(func)(std::forward<Args>(args)...);

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

		fw(func)(std::forward<Args>(args)...);

		return std::chrono::duration_cast<TimeT>(ClockT::now() - start);
	}
};





class Statistics
{
public:
	struct measurement {
		std::string					mName;
		  long long				mDuration;

		  long mAccumaleted = 0;
		  long mAverage = 0;
		  long mMin = 0, mMax = 0;

		measurement() = default;
		measurement(const std::string& name,  long long  duration) :
			mName(name), mDuration(duration) {}
	};


public:

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
		std::cout << "Name\t---> \t Min\t\t Max\t\t Average\t Sum\n";
		std::cout << "---------------------------------------------------------------------------------\n";
		long double SumSum = 0, _Avg = 0;;
		long long _Min = 0,
			_Max = 0
			;
		for (int i = 1; i < vm::MAXCODE - 1; i++)
		{
			std::stringstream					buffer;
			long long Min = std::numeric_limits<long long>::max() , Max = std::numeric_limits<long long>::min(), Sum = 0;
			long double Average = 0.0f;
			size_t	count = 0;
			bool used = false;

			for (auto item : mMeasurements)
			{
				if (item.mName == vm::InstructionCode[i]->mName) // Filter current Instruction
				{
					used = true;
					if (Min > item.mDuration)
						Min = item.mDuration;
					if (Max < item.mDuration)
						Max = item.mDuration;
					Sum += item.mDuration;
					count++;
				}
			}
			if (!used)
			{
				Min = 0, Max = 0, Sum = 0, Average = 0.0f, count = 0;
				continue;
			}

			Average = ((long double)Sum) / count;
			//buffer << vm::InstructionCode[i]->mName << "\t---> [\t " << Min << "ns, " << Max << ", " << Average << ", "<<Sum <<"]\n";
			std::cout << std::right<< vm::InstructionCode[i]->mName << "\t---> [\t " << Min  << "ns,\t " << Max << "ns,\t " << Average << "ns,\t " << Sum  << "ns\t]\n";
			this->mResults.push_back(buffer.str());
			SumSum += Sum;
			_Min += Min;
			_Max += Max;
			_Avg += Average;
		}
		std::cout << "---------------------------------------------------------------------------------\n";
		std::cout <<"Sum\t\t "<<_Min*std::pow(10, -6) <<"ms\t "<<_Max*std::pow(10, -6) <<"ms\t "<<_Avg*std::pow(10, -6) <<"ms\t "<< SumSum*std::pow(10,-6)<<" ms\n\n";
		
	}

};