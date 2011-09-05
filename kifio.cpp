
#include <cassert>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>

#include "kifio.h"

// kifio

namespace kifio // HACK to be merged
{
	namespace strings
	{
		const char* plus                           = "十";
		//
		const char* kuro_sankaku                   = "▲";                             // L_TEBAN_SEN
		const char* shiro_sankaku                  = "△";                             // L_TEBAN_GO
		const char* taikyukubu                     = "対局日：";                       // L_DATE
		const char* kaishinichiji                  = "開始日時：";                     // L_TIME_BEGIN
		const char* shuryonichiji                  = "終了日時：";                     // L_TIME_END
		const char* teaiwari                       = "手合割：";                       // L_TEAI
		const char* sente                          = "先手：";                         // L_NAME_SEN
		const char* gote                           = "後手：";                         // L_NAME_GO
		const char* hyodai                         = "表題：";                         // L_TITLE
		const char* kisen                          = "棋戦：";                         // L_EVENT
		const char* senkei                         = "戦型：";                         // L_OPENING
		const char* tesu_sashite_shohijikan        = "手数----指手---------消費時間--"; // L_RECORD_BEGIN
		const char* kyuhachinanarokugoyonsanniichi = "  ９ ８ ７ ６ ５ ４ ３ ２ １";   // L_EMPTY
		const char* space                          = " ";                              // L_MOVE
		const char* plusminusminusplus             = "+---------------------------+";  // L_EMPTY
		const char* made                           = "まで";                           // L_RECORD_END
		const char* sentenomochigoma               = "先手の持駒：";                   // L_HAND_SEN
		const char* gotenomochigoma                = "後手の持駒：";                   // L_HAND_GO
		const char* shitatenomochigomo             = "下手の持駒：";                   // L_HAND_SEN
		const char* uwatenomochigoma               = "上手の持駒：";                   // L_HAND_GO
		const char* senteban                       = "先手番";                         // L_TEBAN_SEN
		const char* goteban                        = "後手番";                         // L_TEBAN_GO
		const char* shitateban                     = "下手番";                         // L_TEBAN_SEN
		const char* uwateban                       = "上手番";                         // L_TEBAN_GO
		const char* empty                          = "";                               // L_UNKNOWN

		const char* douzenspace                    = "同　";
		const char* dou                            = "同";
		const char* zenspace                       = "　";
		const char* utsu                           = "打";
		const char* naru                           = "成";
		const char* hidari                         = "左";
		const char* choku                          = "直";
		const char* migi                           = "右";
		const char* agaru                          = "上";
		const char* yoru                           = "寄";
		const char* hiku                           = "引";
		const char* funari                         = "不成";
		const char* nashi                          = "なし";
		const char* nakaguro                       = "・";
		const char* nomochigoma                    = "の持駒：";
		const char* hirate                         = "平手";
		const char* sonota                         = "その他";
	}

	// computer shogi assosiation
	namespace csa
	{
		const char *STATE_CHAR[16] = {
			"* ", "FU", "KY", "KE", "GI", "KI", "KA", "HI", "OU", "TO", "NY", "NK", "NG", "NI", "UM", "RY",
		};
		const char *STATE_STR[16] = {
			"* ", "FU", "KY", "KE", "GI", "KI", "KA", "HI", "OU", "TO", "NY", "NK", "NG", "NI", "UM", "RY",
		};
		const char *SPECIAL_STR[13] = {
			"%NULL", "%TORYO", "%CHUDAN", "%SENNICHITE", "%TIME_UP", "%ILLEGAL_MOVE",
			"%JISHOGI", "%KACHI", "%HIKIWAKE", "%MATTA", "%TSUMI", "%FUDUMI", "%ERROR",
		};
	}

	// japan shogi association
	namespace jsa
	{
		const char *ZEN_SPACE = "　";

		const char *ZEN_SUJI[10] = {
			"０", "１", "２", "３", "４", "５", "６", "７", "８", "９",
		};
		const char *KAN_SUJI[11] = {
			"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十",
		};
		// 盤面用
		const char *STATE_CHAR[16] = {
			"・", "歩", "香", "桂", "銀", "金", "角", "飛", "玉", "と", "杏", "圭", "全", "人", "馬", "竜",
		};
		// 棋譜用
		const char *STATE_STR[16] = {
			"幻", "歩", "香", "桂", "銀", "金", "角", "飛", "玉", "と", "成香", "成桂", "成銀", "成金", "馬", "龍",
		};
		// 持ち駒用
		const char *HAND_CHAR[8] = {
			"幻", "歩", "香", "桂", "銀", "金", "角", "飛",
		};
		const alt_t STATE_ALT[2] = {
			{ "王", OU },
			{ "竜", RY },
		};
		const char *SPECIAL_STR[13] = {
			"(NULL)", "投了", "中断", "千日手？", "時間切れ？", "反則",
			"持将棋", "勝ち", "引き分け", "待った？", "詰み", "不詰み", "エラー？",
		};
		const char *TEBAN_CHAR[2] = {
			"▲", "△"
		};
		const char *MOTION_STR[4] = {
			"", "上", "寄", "引"
		};
		const char *POSITION_STR[4] = {
			"", "左", "直", "右",
		};
		const char *OPTION_STR[4] = {
			"", "成", "不成", "打"
		};
	}
	// universal shogi interface
	namespace usi
	{
		const char *STATE_STRS[2][16] =
		{
			{"[", "P", "L", "N", "S", "G", "B", "R", "K", "+P", "+L", "+N", "+S", "+G", "+B", "+R"} ,	//GOTE
			{"[", "p", "l", "n", "s", "g", "b", "r", "k", "+p", "+l", "+n", "+s", "+g", "+b", "+r"} ,	//SENTE
		};
	}
	namespace pico
	{
		const char LINE_CHARS[2][16] =
		{
			{'[', 'p', 'l', 'n', 's', 'g', 'b', 'r', 'k', 'q', 'i', 'm', 'z', '-', 'h', 'd'} ,	//SENTE
			{']', 'P', 'L', 'N', 'S', 'G', 'B', 'R', 'K', 'Q', 'I', 'M', 'Z', '-', 'H', 'D'} ,	//GOTE
		};

		const char* LINEBOARD_STR[] = {
			"+/................................................................................./00000000/00000000",
			"+/LNSGKGSNL.B.....R.PPPPPPPPP...........................ppppppppp.r.....b.lnsgkgsnl/00000000/00000000",
			"-/.NSGKGSNL.B.....R.PPPPPPPPP...........................ppppppppp.r.....b.lnsgkgsnl/00000000/00000000",
			"-/LNSGKGSNL.B.......PPPPPPPPP...........................ppppppppp.r.....b.lnsgkgsnl/00000000/00000000",
			"-/LNSGKGSNL.......R.PPPPPPPPP...........................ppppppppp.r.....b.lnsgkgsnl/00000000/00000000",
			"-/LNSGKGSNL.........PPPPPPPPP...........................ppppppppp.r.....b.lnsgkgsnl/00000000/00000000",
			"-/.NSGKGSN..........PPPPPPPPP...........................ppppppppp.r.....b.lnsgkgsnl/00000000/00000000",
			"-/..SGKGS...........PPPPPPPPP...........................ppppppppp.r.....b.lnsgkgsnl/00000000/00000000",
			"-/...GKG............PPPPPPPPP...........................ppppppppp.r.....b.lnsgkgsnl/00000000/00000000",
			"+/....K............................................................................/22444418/00000000",
		};
	}

	namespace dir
	{
		const adir_t D2A[2][1+12] = {
			{ A_CNT, A_UP, A_RGT, A_LFT, A_DWN, A_UL, A_UR, A_DL, A_DR, A_UUL, A_UUR, A_DDL, A_DDR },
			{ A_CNT, A_DWN, A_LFT, A_RGT, A_UP, A_DR, A_DL, A_UR, A_UL, A_DDR, A_DDL, A_UUR, A_UUL },
		};

		// 行ける場所
		mobility_t MOBILITY[] =
		{
			{{},{}}, // --
			{{UP},{}}, // FU
			{{},{UP}}, // KY
			{{UUL, UUR},{}}, // KE
			{{UP, UL, UR, DL, DR},{}}, // GI
			{{UP, UL, UR, LFT, RGT, DWN},{}}, // KI
			{{},{UL, UR, DL, DR}}, // KA
			{{},{UP, RGT, LFT, DWN}}, // HI
			{{UP, UL, UR, LFT, RGT, DWN, DL, DR},{}}, // OU
			{{UP, UL, UR, LFT, RGT, DWN},{}}, // TO
			{{UP, UL, UR, LFT, RGT, DWN},{}}, // NY
			{{UP, UL, UR, LFT, RGT, DWN},{}}, // NK
			{{UP, UL, UR, LFT, RGT, DWN},{}}, // NG
			{{},{}}, // --
			{{UP, RGT, LFT, DWN},{UL, UR, DL, DR}}, // UM
			{{UL, UR, DL, DR},{UP, RGT, LFT, DWN}}, // RY
		};

		//  これる場所
		mobility_t MOBILITY_FROM[] =
		{
			{{},{}}, // --
			{{DWN},{}}, // FU
			{{},{DWN}}, // KY
			{{DDL, DDR},{}}, // KE
			{{UL, UR, DL, DR, DWN},{}}, // GI
			{{UP, LFT, RGT, DWN, DL, DR},{}}, // KI
			{{},{UL, UR, DL, DR}}, // KA
			{{},{UP, RGT, LFT, DWN}}, // HI
			{{UP, UL, UR, LFT, RGT, DWN, DL, DR},{}}, // OU
			{{UP, LFT, RGT, DWN, DL, DR},{}}, // TO
			{{UP, LFT, RGT, DWN, DL, DR},{}}, // NY
			{{UP, LFT, RGT, DWN, DL, DR},{}}, // NK
			{{UP, LFT, RGT, DWN, DL, DR},{}}, // NG
			{{},{}}, // --
			{{UP, RGT, LFT, DWN},{UL, UR, DL, DR}}, // UM
			{{UL, UR, DL, DR},{UP, RGT, LFT, DWN}}, // RY
		};
	}
} // namespace */

