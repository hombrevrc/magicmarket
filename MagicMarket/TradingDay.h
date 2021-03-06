#pragma once

#include <map>
#include <string>
#include <vector>

#include <ql/time/date.hpp>

#include "Tick.h"

namespace MM
{
	class Stock;

	namespace io
	{
		class DataConverter;
		class DataReader;
	};

	class TradingDay
	{
	public:
		TradingDay(QuantLib::Date date, Stock *stock);
		~TradingDay();

		std::string getCurrencyPair();
		TradingDay *getPreviousDay();
		TradingDay *getNextDay();

		QuantLib::Date getDate() { return date; }
		
		void receiveFreshTick(const Tick &tick);

		// saving & loading
		std::ostream& getSaveFile();
		bool loadFromFile();
		std::string getSavePath();
		static std::string getSavePath(Stock* stock, QuantLib::Date forDate);
		std::string getSaveFileName();
		static std::string getSaveFileName(QuantLib::Date forDate);
	private:
		QuantLib::Date date;
		Tick &getTickByIndex(size_t index) { return ticks[index]; }
		std::vector<Tick> ticks;
		void serializeTick(const Tick &tick);

		std::fstream *saveFile;
		Stock *stock;

		std::vector<Tick> & getTicks() { return ticks; }

		friend class TimePeriod;
		friend class VirtualMarket;
		friend class io::DataConverter;
		friend class io::DataReader;
	};


};