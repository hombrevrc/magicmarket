#pragma once

#include "Base.h"
#include <string>

namespace MM
{
	class Stock;

	namespace Indicators
	{
		class SMA : public Base
		{
		public:
			SMA(std::string currencyPair, int history, int seconds);
			virtual ~SMA();

			virtual void declareExports() const;
			virtual void update(const std::time_t &secondsSinceStart, const std::time_t &time) override;
			virtual bool operator== (const Base &otherBase) const
			{
				const SMA* other = dynamic_cast<const SMA*>(&otherBase);
				if (other == nullptr) return false;
				return (other->history == history) && (other->seconds == seconds) && (other->currencyPair == currencyPair);
			}

			double getSMA() { return sma; }

		private:
			std::string currencyPair;
			int history;
			int seconds;
			double sma;
		};

	};
};