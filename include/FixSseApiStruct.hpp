#ifndef FIXSSEAPISTRUCT_HPP
#define FIXSSEAPISTRUCT_HPP

#define queue_length 999
#define max_buffer_length 500
namespace xele_md
{
#pragma pack(1)
	/* struct for MD001 message */
	struct Sse_MarketData_Stock
	{
		uint64_t seq_num;
		u_char   MdStreamID[8];
		u_char   Timestamp[8];
		u_char   SecutrityID[8];
		u_char   Symbol[8];
		uint64_t TradeVolume;
		double   TotalValueTraded;
		double   PreClosePrice;
		double   PrevSetPrice;
		uint64_t TotalLongPosition;
		double   IndexValue;
		double   OpenPrice;
		double   ClosePrice;
		double   HighPrice;
		double   LowPrice;
		u_char   TradingPhaseCode[8];
		u_char   reserved[24];
	};

	/* struct for MD002 MD003 MD004 message */
	struct Sse_MarketData_Other
	{
		uint64_t seq_num;
		u_char   MdStreamID[8];
		u_char   Timestamp[8];
		u_char   SecutrityID[8];
		u_char   Symbol[8];
		uint64_t TradeVolume;
		double   TotalValueTraded;
		double   PreClosePrice;
		double   PrevSetPrice;
		uint64_t TotalLongPosition;
		double   BidPrice1;
		uint64_t BidVolume1;
		double   BidPrice2;
		uint64_t BidVolume2;
		double   BidPrice3;
		uint64_t BidVolume3;
		double   BidPrice4;
		uint64_t BidVolume4;
		double   BidPrice5;
		uint64_t BidVolume5;
		double   AskPrice1;
		uint64_t AskVolume1;
		double   AskPrice2;
		uint64_t AskVolume2;
		double   AskPrice3;
		uint64_t AskVolume3;
		double   AskPrice4;
		uint64_t AskVolume4;
		double   AskPrice5;
		uint64_t AskVolume5;
		double   TradePrice;
		double   OpenPrice;
		double   ClosePrice;
		double   HighPrice;
		double   LowPrice;
		double   IOPV;
		double   PreCloseIOPV;
		u_char   TradingPhaseCode[8];
		u_char   reserved[24];
	};

	/* struct for M0301 message */
	struct Sse_MarketData_Option
	{
		uint64_t seq_num;
		u_char   MdStreamID[8];
		u_char   Timestamp[8];
		u_char   SecutrityID[8];
		u_char   Symbol[8];
		uint64_t TradeVolume;
		double   TotalValueTraded;
		double   PreClosePrice;
		double   PrevSetPrice;
		uint64_t TotalLongPosition;
		double   BidPrice1;
		uint64_t BidVolume1;
		double   BidPrice2;
		uint64_t BidVolume2;
		double   BidPrice3;
		uint64_t BidVolume3;
		double   BidPrice4;
		uint64_t BidVolume4;
		double   BidPrice5;
		uint64_t BidVolume5;
		double   AskPrice1;
		uint64_t AskVolume1;
		double   AskPrice2;
		uint64_t AskVolume2;
		double   AskPrice3;
		uint64_t AskVolume3;
		double   AskPrice4;
		uint64_t AskVolume4;
		double   AskPrice5;
		uint64_t AskVolume5;
		double   TradePrice;
		double   OpenPrice;
		double   ClosePrice;
		double   SettlPrice;
		double   HighPrice;
		double   LowPrice;
		double   AuctionPrice;
		uint64_t AuctionQty;
		u_char   TradingPhaseCode[8];
		u_char   reserved[24];
	};
#pragma pack()
}
#endif
