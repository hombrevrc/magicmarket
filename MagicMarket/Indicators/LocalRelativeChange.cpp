#include "LocalRelativeChange.h"
#include "Market.h"
#include "Stock.h"
#include "TimePeriod.h"
#include "Statistics.h"

#include "Helpers.h"

namespace MM
{
	namespace Indicators
	{
		void LocalRelativeChange::reset()
		{
			const double *lastValue = &lookbackDerivatives.back();
			lookbackDerivatives.clear();
			for (size_t i = 0; i < lookbackDurations.size(); ++i)
				lookbackDerivatives.push_back(std::numeric_limits<double>::quiet_NaN());
			// The memory location should not change.
			assert(lastValue == &lookbackDerivatives.back());
		}

		LocalRelativeChange::LocalRelativeChange(std::string currencyPair, std::vector<int> lookbackDurations) :
			currencyPair(currencyPair),
			lookbackDurations(lookbackDurations)
		{
			lookbackDerivatives.resize(lookbackDurations.size());
		}

		LocalRelativeChange::~LocalRelativeChange()
		{
		}

		void LocalRelativeChange::declareExports() const
		{
			std::string allLookbacks = "{";
			for (size_t i = 0; i < lookbackDurations.size(); ++i)
				allLookbacks += ((i == 0) ? "" : ", ") + std::to_string(lookbackDurations[i]);
			allLookbacks += "}";

			const std::string namePrefix = currencyPair + "_local_";
			size_t index = 0;
			for (int const &lookback : lookbackDurations)
			{
				const std::string name = namePrefix + std::to_string(lookback) + "s";
				const std::string desc = "Local relative change of " + currencyPair + " with lookback " + std::to_string(lookback) + " " + allLookbacks;
				statistics.addVariable(Variable(name, &lookbackDerivatives[index], desc));
				
				index += 1;
			}
		}

		void LocalRelativeChange::update(const std::time_t &secondsSinceStart, const std::time_t &time)
		{
			Stock *stock = market.getStock(currencyPair);
			if (stock == nullptr) return;

			int maxLookbackIndex = -1;
			double maxAbsValue = std::numeric_limits<double>::quiet_NaN();
			for (size_t i = 0; i < lookbackDurations.size(); ++i)
			{
				const int &lookback = lookbackDurations[i];
				double &value = lookbackDerivatives[i];

				MM::TimePeriod now = stock->getTimePeriod(time - lookback);
				const PossibleDecimal price = now.getOpen();
				if (!price.get())
				{
					value = std::numeric_limits<double>::quiet_NaN();
					continue;
				}
				else value = *price;

				// Figure out absolute maximum for normalization.
				const double currentAbsValue = std::abs(value);
				if (std::isnan(maxAbsValue) || maxAbsValue < currentAbsValue)
					maxAbsValue = currentAbsValue;

				// Figure out first value of series as the basis for the relative data.
				if (maxLookbackIndex == -1 || lookback > lookbackDurations[maxLookbackIndex])
				{
					maxLookbackIndex = i;
				}
			}

			// And normalize the data.
			const double firstDataValue = (maxLookbackIndex == -1) ? std::numeric_limits<double>::quiet_NaN() : lookbackDerivatives[maxLookbackIndex];
			for (double & value : lookbackDerivatives)
			{
				value = (value - firstDataValue) / maxAbsValue;
			}
		}
	};
};