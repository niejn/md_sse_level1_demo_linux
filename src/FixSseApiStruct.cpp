#define _CRT_SECURE_NO_WARNINGS

#include "userfunc.hpp"
#include "FixSseApiStruct.hpp"
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <thread>

using namespace std;

#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>
//#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include <unistd.h>


namespace xele_md {

    uint64_t htonl_64(uint64_t val) {
#ifndef NOBYTEORD
#if __BYTE_ORDER == __LITTLE_ENDIAN
        union {
            uint32_t lv[2];
            uint64_t llv;
        } u;
        u.lv[0] = htonl(val >> 32);
        u.lv[1] = htonl(val & 0xFFFFFFFFULL);
        return u.llv;
#endif
#endif
        return val;
    }

    double ntoh_double(double val) {
        double ret = val;
#ifndef NOBYTEORD
#if __BYTE_ORDER == __LITTLE_ENDIAN
        uint64_t tmp = htonl_64(*((uint64_t *) &val));
        ret = *((double *) &tmp);
#endif
#endif
        return ret;
    }

    long long get_nanosecond_pivot() {
        auto now = chrono::high_resolution_clock::now();

        auto timeMillis = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()).count();
        long long t_nana_secs = timeMillis;
        return t_nana_secs;
    }

    int code_convert(const char *from_charset, const char *to_charset, char *inbuf, size_t inlen, char *outbuf,
                     size_t outlen) {
        iconv_t cd;
        char **pin = &inbuf;
        char **pout = &outbuf;

        cd = iconv_open(to_charset, from_charset);
        if (cd == 0) {
            return -1;
        }
        memset(outbuf, 0, outlen);
        if (iconv(cd, pin, &inlen, pout, &outlen) == (size_t) -1) {
            return -1;
        }

        iconv_close(cd);
        return 0;
    }

    int gbk_to_utf8(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
        return code_convert("gbk", "utf-8", inbuf, inlen, outbuf, outlen);
    }

    void Print_Sse_MarketData_Stock(FILE *logfd, struct Sse_MarketData_Stock obj) {
//         std::string space_gap = " ";
//         std::string line_gap = " ";
//         std::string atick = str();
        const char *space = " , ";
        const char *line = "\n";

        printf("\n");
        //printf("MdStreamID:%s %s",obj.MdStreamID, space);
        fprintf(logfd, "MdStreamID:%s %s", obj.MdStreamID, space);

        int k = 0;
        //printf("seq_num:%lu %s", htonl_64(obj.seq_num), space);
        fprintf(logfd, "seq_num:%lu %s", htonl_64(obj.seq_num), space);

        //printf("Timestamp:");
        fprintf(logfd, "Timestamp:");
        for (int i = 0; i < 8; ++i) {
            if (obj.Timestamp[i] != 0) {
                //printf("%c",obj.Timestamp[i]);
                fprintf(logfd, "%c", obj.Timestamp[i]);
            }
        }
        //printf("%s", space);
        fprintf(logfd, "%s", space);

        //printf("SecutrityID:");
        fprintf(logfd, "SecutrityID:");
        for (int i = 0; i < 8; ++i) {
            if (obj.SecutrityID[i] != 0) {
                //printf("%c",obj.SecutrityID[i]);
                fprintf(logfd, "%c", obj.SecutrityID[i]);
            }
        }
        //printf("%s", space);
        fprintf(logfd, "%s", space);

        //printf("Symbol:");
        fprintf(logfd, "Symbol:");
        k = 0;
        while (k < 8) {
            if (obj.Symbol[k] >= 0x00 && obj.Symbol[k] <= 0x7F) {
                //printf("%c",obj.Symbol[k]);
                fprintf(logfd, "%c", obj.Symbol[k]);
                ++k;
            } else {
                char str_gbk[3] = "";
                char buff[5] = "";
                str_gbk[0] = obj.Symbol[k];
                ++k;
                str_gbk[1] = obj.Symbol[k];
                gbk_to_utf8(str_gbk, strlen(str_gbk), buff, sizeof(buff));
                //printf("%s",buff);
                fprintf(logfd, "%s", buff);
                ++k;
            }
        }
        //printf("%s", space);
        fprintf(logfd, "%s", space);

        //printf("TradeVolume:%lu %s",htonl_64(obj.TradeVolume), space);
        fprintf(logfd, "TradeVolume:%lu %s", htonl_64(obj.TradeVolume), space);
        //printf("TotalValueTraded:%0.2lf %s",ntoh_double(obj.TotalValueTraded), space);
        fprintf(logfd, "TotalValueTraded:%0.2lf %s", ntoh_double(obj.TotalValueTraded), space);
        //printf("PreClosePrice:%0.2lf %s",ntoh_double(obj.PreClosePrice), space);
        fprintf(logfd, "PreClosePrice:%0.2lf %s", ntoh_double(obj.PreClosePrice), space);
        //printf("PrevSetPrice:%0.2lf %s",ntoh_double(obj.PrevSetPrice), space);
        fprintf(logfd, "PrevSetPrice:%0.2lf %s", ntoh_double(obj.PrevSetPrice), space);
        //printf("TotalLongPosition:%lu %s",htonl_64(obj.TotalLongPosition), space);
        fprintf(logfd, "TotalLongPosition:%lu %s", htonl_64(obj.TotalLongPosition), space);
        //printf("IndexValue:%0.2lf %s",ntoh_double(obj.IndexValue), space);
        fprintf(logfd, "IndexValue:%0.2lf %s", ntoh_double(obj.IndexValue), space);
        //printf("OpenPrice:%0.2lf %s",ntoh_double(obj.OpenPrice), space);
        fprintf(logfd, "OpenPrice:%0.2lf %s", ntoh_double(obj.OpenPrice), space);
        //printf("ClosePrice:%0.2lf %s",ntoh_double(obj.ClosePrice), space);
        fprintf(logfd, "ClosePrice:%0.2lf %s", ntoh_double(obj.ClosePrice), space);
        //printf("HighPrice:%0.2lf %s",ntoh_double(obj.HighPrice), space);
        fprintf(logfd, "HighPrice:%0.2lf %s", ntoh_double(obj.HighPrice), space);
        //printf("LowPrice:%0.2lf %s",ntoh_double(obj.LowPrice), space);
        fprintf(logfd, "LowPrice:%0.2lf %s", ntoh_double(obj.LowPrice), space);
        //printf("TradingPhaseCode:%s %s",obj.TradingPhaseCode, space);
        fprintf(logfd, "TradingPhaseCode:%s %s", obj.TradingPhaseCode, space);

        auto nano_secs = get_nanosecond_pivot();
        fprintf(logfd, "nano_secs:%llu ", nano_secs);
//        fprintf(logfd, "nano_secs:%I64u ", nano_secs);

//         printf("--------------------------------\n");
        fprintf(logfd, "%s", line);
        printf("Stock Modified Input 'q' to disconnect API:");
        fflush(stdout);
    }

    void Print_Sse_MarketData_Other(FILE *logfd, struct Sse_MarketData_Other obj) {
        const char *space = " , ";
        const char *line = "\n";


        printf("\n");
        //printf("MdStreamID:%s %s",obj.MdStreamID, space);
        fprintf(logfd, "MdStreamID:%s %s", obj.MdStreamID, space);

        int k = 0;
        //printf("seq_num:%lu %s", htonl_64(obj.seq_num), space);
        fprintf(logfd, "seq_num:%lu %s", htonl_64(obj.seq_num), space);

        //printf("Timestamp:");
        fprintf(logfd, "Timestamp:");
        for (int i = 0; i < 8; ++i) {
            if (obj.Timestamp[i] != 0) {
                //printf("%c",obj.Timestamp[i]);
                fprintf(logfd, "%c", obj.Timestamp[i]);
            }
        }
        //printf("%s", space);
        fprintf(logfd, "%s", space);

        //printf("SecutrityID:");
        fprintf(logfd, "SecutrityID:");
        for (int i = 0; i < 8; ++i) {
            if (obj.SecutrityID[i] != 0) {
                //printf("%c",obj.SecutrityID[i]);
                fprintf(logfd, "%c", obj.SecutrityID[i]);
            }
        }
        //printf("%s", space);
        fprintf(logfd, "%s", space);

        //printf("Symbol:");
        fprintf(logfd, "Symbol:");
        k = 0;
        while (k < 8) {
            if (obj.Symbol[k] >= 0x00 && obj.Symbol[k] <= 0x7F) {
                //printf("%c",obj.Symbol[k]);
                fprintf(logfd, "%c", obj.Symbol[k]);
                ++k;
            } else {
                char str_gbk[3] = "";
                char buff[5] = "";
                str_gbk[0] = obj.Symbol[k];
                ++k;
                str_gbk[1] = obj.Symbol[k];
                gbk_to_utf8(str_gbk, strlen(str_gbk), buff, sizeof(buff));
                //printf("%s",buff);
                fprintf(logfd, "%s", buff);
                ++k;
            }
        }
        //printf("%s", space);
        fprintf(logfd, "%s", space);

        //printf("TradeVolume:%lu %s",htonl_64(obj.TradeVolume), space);
        fprintf(logfd, "TradeVolume:%lu %s", htonl_64(obj.TradeVolume), space);
        //printf("TotalValueTraded:%0.2lf %s",ntoh_double(obj.TotalValueTraded), space);
        fprintf(logfd, "TotalValueTraded:%0.2lf %s", ntoh_double(obj.TotalValueTraded), space);
        //printf("PreClosePrice:%0.2lf %s",ntoh_double(obj.PreClosePrice), space);
        fprintf(logfd, "PreClosePrice:%0.2lf %s", ntoh_double(obj.PreClosePrice), space);
        //printf("PrevSetPrice:%0.2lf %s",ntoh_double(obj.PrevSetPrice), space);
        fprintf(logfd, "PrevSetPrice:%0.2lf %s", ntoh_double(obj.PrevSetPrice), space);
        //printf("TotalLongPosition:%lu %s",htonl_64(obj.TotalLongPosition), space);
        fprintf(logfd, "TotalLongPosition:%lu %s", htonl_64(obj.TotalLongPosition), space);
        //printf("BidPrice1:%0.2lf %s",ntoh_double(obj.BidPrice1), space);
        fprintf(logfd, "BidPrice1:%0.2lf %s", ntoh_double(obj.BidPrice1), space);
        //printf("BidVolume1:%lu %s",htonl_64(obj.BidVolume1), space);
        fprintf(logfd, "BidVolume1:%lu %s", htonl_64(obj.BidVolume1), space);
        //printf("BidPrice2:%0.2lf %s",ntoh_double(obj.BidPrice2), space);
        fprintf(logfd, "BidPrice2:%0.2lf %s", ntoh_double(obj.BidPrice2), space);
        //printf("BidVolume2:%lu %s",htonl_64(obj.BidVolume2), space);
        fprintf(logfd, "BidVolume2:%lu %s", htonl_64(obj.BidVolume2), space);
        //printf("BidPrice3:%0.2lf %s",ntoh_double(obj.BidPrice3), space);
        fprintf(logfd, "BidPrice3:%0.2lf %s", ntoh_double(obj.BidPrice3), space);
        //printf("BidVolume3:%lu %s",htonl_64(obj.BidVolume3), space);
        fprintf(logfd, "BidVolume3:%lu %s", htonl_64(obj.BidVolume3), space);
        //printf("BidPrice4:%0.2lf %s",ntoh_double(obj.BidPrice4), space);
        fprintf(logfd, "BidPrice4:%0.2lf %s", ntoh_double(obj.BidPrice4), space);
        //printf("BidVolume4:%lu %s",htonl_64(obj.BidVolume4), space);
        fprintf(logfd, "BidVolume4:%lu %s", htonl_64(obj.BidVolume4), space);
        //printf("BidPrice5:%0.2lf %s",ntoh_double(obj.BidPrice5), space);
        fprintf(logfd, "BidPrice5:%0.2lf %s", ntoh_double(obj.BidPrice5), space);
        //printf("BidVolume5:%lu %s",htonl_64(obj.BidVolume5), space);
        fprintf(logfd, "BidVolume5:%lu %s", htonl_64(obj.BidVolume5), space);
        //printf("AskPrice1:%0.2lf %s",ntoh_double(obj.AskPrice1), space);
        fprintf(logfd, "AskPrice1:%0.2lf %s", ntoh_double(obj.AskPrice1), space);
        //printf("AskVolume1:%lu %s",htonl_64(obj.AskVolume1), space);
        fprintf(logfd, "AskVolume1:%lu %s", htonl_64(obj.AskVolume1), space);
        //printf("AskPrice2:%0.2lf %s",ntoh_double(obj.AskPrice2), space);
        fprintf(logfd, "AskPrice2:%0.2lf %s", ntoh_double(obj.AskPrice2), space);
        //printf("AskVolume2:%lu %s",htonl_64(obj.AskVolume2), space);
        fprintf(logfd, "AskVolume2:%lu %s", htonl_64(obj.AskVolume2), space);
        //printf("AskPrice3:%0.2lf %s",ntoh_double(obj.AskPrice3), space);
        fprintf(logfd, "AskPrice3:%0.2lf %s", ntoh_double(obj.AskPrice3), space);
        //printf("AskVolume3:%lu %s",htonl_64(obj.AskVolume3), space);
        fprintf(logfd, "AskVolume3:%lu %s", htonl_64(obj.AskVolume3), space);
        //printf("AskPrice4:%0.2lf %s",ntoh_double(obj.AskPrice4), space);
        fprintf(logfd, "AskPrice4:%0.2lf %s", ntoh_double(obj.AskPrice4), space);
        //printf("AskVolume4:%lu %s",htonl_64(obj.AskVolume4), space);
        fprintf(logfd, "AskVolume4:%lu %s", htonl_64(obj.AskVolume4), space);
        //printf("AskPrice5:%0.2lf %s",ntoh_double(obj.AskPrice5), space);
        fprintf(logfd, "AskPrice5:%0.2lf %s", ntoh_double(obj.AskPrice5), space);
        //printf("AskVolume5:%lu %s",htonl_64(obj.AskVolume5), space);
        fprintf(logfd, "AskVolume5:%lu %s", htonl_64(obj.AskVolume5), space);
        //printf("TradePrice:%0.2lf %s",ntoh_double(obj.TradePrice), space);
        fprintf(logfd, "TradePrice:%0.2lf %s", ntoh_double(obj.TradePrice), space);
        //printf("OpenPrice:%0.2lf %s",ntoh_double(obj.OpenPrice), space);
        fprintf(logfd, "OpenPrice:%0.2lf %s", ntoh_double(obj.OpenPrice), space);
        //printf("ClosePrice:%0.2lf %s",ntoh_double(obj.ClosePrice), space);
        fprintf(logfd, "ClosePrice:%0.2lf %s", ntoh_double(obj.ClosePrice), space);
        //printf("HighPrice:%0.2lf %s",ntoh_double(obj.HighPrice), space);
        fprintf(logfd, "HighPrice:%0.2lf %s", ntoh_double(obj.HighPrice), space);
        //printf("LowPrice:%0.2lf %s",ntoh_double(obj.LowPrice), space);
        fprintf(logfd, "LowPrice:%0.2lf %s", ntoh_double(obj.LowPrice), space);

        if (strcmp((char *) obj.MdStreamID, "MD004") == 0) {
            //printf("IOPV:%0.2lf %s",ntoh_double(obj.IOPV), space);
            fprintf(logfd, "IOPV:%0.2lf %s", ntoh_double(obj.IOPV), space);

            //printf("PreCloseIOPV:%0.2lf %s",ntoh_double(obj.PreCloseIOPV), space);
            fprintf(logfd, "PreCloseIOPV:%0.2lf %s", ntoh_double(obj.PreCloseIOPV), space);
        }

        //printf("TradingPhaseCode:%s %s",obj.TradingPhaseCode, space);
        fprintf(logfd, "TradingPhaseCode:%s %s", obj.TradingPhaseCode, space);

        auto nano_secs = get_nanosecond_pivot();
        fprintf(logfd, "nano_secs:%llu ", nano_secs);

        //printf("--------------------------------\n");
        fprintf(logfd, "%s", line);
        printf("Other Input 'q' to disconnect API:");
        fflush(stdout);
    }

    void Print_Sse_MarketData_Option(FILE *logfd, struct Sse_MarketData_Option obj) {
        const char *space = " , ";
        const char *line = "\n";

        printf("\n");
        //printf("MdStreamID:%s %s",obj.MdStreamID, space);
        fprintf(logfd, "MdStreamID:%s %s", obj.MdStreamID, space);

        //printf("seq_num:%lu %s", htonl_64(obj.seq_num), space);
        fprintf(logfd, "seq_num:%lu %s", htonl_64(obj.seq_num), space);

        //printf("Timestamp:");
        fprintf(logfd, "Timestamp:");
        for (int i = 0; i < 8; ++i) {
            if (obj.Timestamp[i] != 0) {
                //printf("%c",obj.Timestamp[i]);
                fprintf(logfd, "%c", obj.Timestamp[i]);
            }
        }
        //printf("%s", space);
        fprintf(logfd, "%s", space);

        //printf("SecutrityID:");
        fprintf(logfd, "SecutrityID:");
        for (int i = 0; i < 8; ++i) {
            if (obj.SecutrityID[i] != 0) {
                //printf("%c",obj.SecutrityID[i]);
                fprintf(logfd, "%c", obj.SecutrityID[i]);
            }
        }
        //printf("%s", space);
        fprintf(logfd, "%s", space);

        //printf("TradeVolume:%lu %s",htonl_64(obj.TradeVolume), space);
        fprintf(logfd, "TradeVolume:%lu %s", htonl_64(obj.TradeVolume), space);
        //printf("TotalValueTraded:%0.4lf %s",ntoh_double(obj.TotalValueTraded), space);
        fprintf(logfd, "TotalValueTraded:%0.4lf %s", ntoh_double(obj.TotalValueTraded), space);
        //printf("PreClosePrice:%0.4lf %s",ntoh_double(obj.PreClosePrice), space);
        fprintf(logfd, "PreClosePrice:%0.4lf %s", ntoh_double(obj.PreClosePrice), space);
        //printf("PrevSetPrice:%0.4lf %s",ntoh_double(obj.PrevSetPrice), space);
        fprintf(logfd, "PrevSetPrice:%0.4lf %s", ntoh_double(obj.PrevSetPrice), space);
        //printf("TotalLongPosition:%lu %s",htonl_64(obj.TotalLongPosition), space);
        fprintf(logfd, "TotalLongPosition:%lu %s", htonl_64(obj.TotalLongPosition), space);
        //printf("BidPrice1:%0.4lf %s",ntoh_double(obj.BidPrice1), space);
        fprintf(logfd, "BidPrice1:%0.4lf %s", ntoh_double(obj.BidPrice1), space);
        //printf("BidVolume1:%lu %s",htonl_64(obj.BidVolume1), space);
        fprintf(logfd, "BidVolume1:%lu %s", htonl_64(obj.BidVolume1), space);
        //printf("BidPrice2:%0.4lf %s",ntoh_double(obj.BidPrice2), space);
        fprintf(logfd, "BidPrice2:%0.4lf %s", ntoh_double(obj.BidPrice2), space);
        //printf("BidVolume2:%lu %s",htonl_64(obj.BidVolume2), space);
        fprintf(logfd, "BidVolume2:%lu %s", htonl_64(obj.BidVolume2), space);
        //printf("BidPrice3:%0.4lf %s",ntoh_double(obj.BidPrice3), space);
        fprintf(logfd, "BidPrice3:%0.4lf %s", ntoh_double(obj.BidPrice3), space);
        //printf("BidVolume3:%lu %s",htonl_64(obj.BidVolume3), space);
        fprintf(logfd, "BidVolume3:%lu %s", htonl_64(obj.BidVolume3), space);
        //printf("BidPrice4:%0.4lf %s",ntoh_double(obj.BidPrice4), space);
        fprintf(logfd, "BidPrice4:%0.4lf %s", ntoh_double(obj.BidPrice4), space);
        //printf("BidVolume4:%lu %s",htonl_64(obj.BidVolume4), space);
        fprintf(logfd, "BidVolume4:%lu %s", htonl_64(obj.BidVolume4), space);
        //printf("BidPrice5:%0.4lf %s",ntoh_double(obj.BidPrice5), space);
        fprintf(logfd, "BidPrice5:%0.4lf %s", ntoh_double(obj.BidPrice5), space);
        //printf("BidVolume5:%lu %s",htonl_64(obj.BidVolume5), space);
        fprintf(logfd, "BidVolume5:%lu %s", htonl_64(obj.BidVolume5), space);
        //printf("AskPrice1:%0.4lf %s",ntoh_double(obj.AskPrice1), space);
        fprintf(logfd, "AskPrice1:%0.4lf %s", ntoh_double(obj.AskPrice1), space);
        //printf("AskVolume1:%lu %s",htonl_64(obj.AskVolume1), space);
        fprintf(logfd, "AskVolume1:%lu %s", htonl_64(obj.AskVolume1), space);
        //printf("AskPrice2:%0.4lf %s",ntoh_double(obj.AskPrice2), space);
        fprintf(logfd, "AskPrice2:%0.4lf %s", ntoh_double(obj.AskPrice2), space);
        //printf("AskVolume2:%lu %s",htonl_64(obj.AskVolume2), space);
        fprintf(logfd, "AskVolume2:%lu %s", htonl_64(obj.AskVolume2), space);
        //printf("AskPrice3:%0.4lf %s",ntoh_double(obj.AskPrice3), space);
        fprintf(logfd, "AskPrice3:%0.4lf %s", ntoh_double(obj.AskPrice3), space);
        //printf("AskVolume3:%lu %s",htonl_64(obj.AskVolume3), space);
        fprintf(logfd, "AskVolume3:%lu %s", htonl_64(obj.AskVolume3), space);
        //printf("AskPrice4:%0.4lf %s",ntoh_double(obj.AskPrice4), space);
        fprintf(logfd, "AskPrice4:%0.4lf %s", ntoh_double(obj.AskPrice4), space);
        //printf("AskVolume4:%lu %s",htonl_64(obj.AskVolume4), space);
        fprintf(logfd, "AskVolume4:%lu %s", htonl_64(obj.AskVolume4), space);
        //printf("AskPrice5:%0.4lf %s",ntoh_double(obj.AskPrice5), space);
        fprintf(logfd, "AskPrice5:%0.4lf %s", ntoh_double(obj.AskPrice5), space);
        //printf("AskVolume5:%lu %s",htonl_64(obj.AskVolume5), space);
        fprintf(logfd, "AskVolume5:%lu %s", htonl_64(obj.AskVolume5), space);
        //printf("TradePrice:%0.4lf %s",ntoh_double(obj.TradePrice), space);
        fprintf(logfd, "TradePrice:%0.4lf %s", ntoh_double(obj.TradePrice), space);
        //printf("OpenPrice:%0.4lf %s",ntoh_double(obj.OpenPrice), space);
        fprintf(logfd, "OpenPrice:%0.4lf %s", ntoh_double(obj.OpenPrice), space);
        //printf("ClosePrice:%0.4lf %s",ntoh_double(obj.ClosePrice), space);
        fprintf(logfd, "ClosePrice:%0.4lf %s", ntoh_double(obj.ClosePrice), space);
        //printf("SettPrice:%0.4lf %s",ntoh_double(obj.SettlPrice), space);
        fprintf(logfd, "SettPrice:%0.4lf %s", ntoh_double(obj.SettlPrice), space);
        //printf("HighPrice:%0.4lf %s",ntoh_double(obj.HighPrice), space);
        fprintf(logfd, "HighPrice:%0.4lf %s", ntoh_double(obj.HighPrice), space);
        //printf("LowPrice:%0.4lf %s",ntoh_double(obj.LowPrice), space);
        fprintf(logfd, "LowPrice:%0.4lf %s", ntoh_double(obj.LowPrice), space);
        //printf("AuctionPrice:%0.4lf %s",ntoh_double(obj.AuctionPrice), space);
        fprintf(logfd, "AuctionPrice:%0.4lf %s", ntoh_double(obj.AuctionPrice), space);
        //printf("AuctionQty:%lu %s",htonl_64(obj.AuctionQty), space);
        fprintf(logfd, "AuctionQty:%lu %s", htonl_64(obj.AuctionQty), space);
        //printf("TradingPhaseCode:%s %s",obj.TradingPhaseCode, space);
        fprintf(logfd, "TradingPhaseCode:%s %s", obj.TradingPhaseCode, space);

        auto nano_secs = get_nanosecond_pivot();
        fprintf(logfd, "nano_secs:%llu ", nano_secs);


        //printf("--------------------------------\n");
        fprintf(logfd, "%s", line);
        printf("Option Input 'q' to disconnect API:");
        fflush(stdout);
    }

}

