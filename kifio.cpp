
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
		const char* plus                           = "�\";
		//
		const char* kuro_sankaku                   = "��";                             // L_TEBAN_SEN
		const char* shiro_sankaku                  = "��";                             // L_TEBAN_GO
		const char* taikyukubu                     = "�΋Ǔ��F";                       // L_DATE
		const char* kaishinichiji                  = "�J�n�����F";                     // L_TIME_BEGIN
		const char* shuryonichiji                  = "�I�������F";                     // L_TIME_END
		const char* teaiwari                       = "�荇���F";                       // L_TEAI
		const char* sente                          = "���F";                         // L_NAME_SEN
		const char* gote                           = "���F";                         // L_NAME_GO
		const char* hyodai                         = "�\��F";                         // L_TITLE
		const char* kisen                          = "����F";                         // L_EVENT
		const char* senkei                         = "��^�F";                         // L_OPENING
		const char* tesu_sashite_shohijikan        = "�萔----�w��---------�����--"; // L_RECORD_BEGIN
		const char* kyuhachinanarokugoyonsanniichi = "  �X �W �V �U �T �S �R �Q �P";   // L_EMPTY
		const char* space                          = " ";                              // L_MOVE
		const char* plusminusminusplus             = "+---------------------------+";  // L_EMPTY
		const char* made                           = "�܂�";                           // L_RECORD_END
		const char* sentenomochigoma               = "���̎���F";                   // L_HAND_SEN
		const char* gotenomochigoma                = "���̎���F";                   // L_HAND_GO
		const char* shitatenomochigomo             = "����̎���F";                   // L_HAND_SEN
		const char* uwatenomochigoma               = "���̎���F";                   // L_HAND_GO
		const char* senteban                       = "����";                         // L_TEBAN_SEN
		const char* goteban                        = "����";                         // L_TEBAN_GO
		const char* shitateban                     = "�����";                         // L_TEBAN_SEN
		const char* uwateban                       = "����";                         // L_TEBAN_GO
		const char* empty                          = "";                               // L_UNKNOWN

		const char* douzenspace                    = "���@";
		const char* dou                            = "��";
		const char* zenspace                       = "�@";
		const char* utsu                           = "��";
		const char* naru                           = "��";
		const char* hidari                         = "��";
		const char* choku                          = "��";
		const char* migi                           = "�E";
		const char* agaru                          = "��";
		const char* yoru                           = "��";
		const char* hiku                           = "��";
		const char* funari                         = "�s��";
		const char* nashi                          = "�Ȃ�";
		const char* nakaguro                       = "�E";
		const char* nomochigoma                    = "�̎���F";
		const char* hirate                         = "����";
		const char* sonota                         = "���̑�";
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
		const char *ZEN_SPACE = "�@";

		const char *ZEN_SUJI[10] = {
			"�O", "�P", "�Q", "�R", "�S", "�T", "�U", "�V", "�W", "�X",
		};
		const char *KAN_SUJI[11] = {
			"��", "��", "��", "�O", "�l", "��", "�Z", "��", "��", "��", "�\",
		};
		// �Ֆʗp
		const char *STATE_CHAR[16] = {
			"�E", "��", "��", "�j", "��", "��", "�p", "��", "��", "��", "��", "�\", "�S", "�l", "�n", "��",
		};
		// �����p
		const char *STATE_STR[16] = {
			"��", "��", "��", "�j", "��", "��", "�p", "��", "��", "��", "����", "���j", "����", "����", "�n", "��",
		};
		// ������p
		const char *HAND_CHAR[8] = {
			"��", "��", "��", "�j", "��", "��", "�p", "��",
		};
		const alt_t STATE_ALT[2] = {
			{ "��", OU },
			{ "��", RY },
		};
		const char *SPECIAL_STR[13] = {
			"(NULL)", "����", "���f", "�����H", "���Ԑ؂�H", "����",
			"������", "����", "��������", "�҂����H", "�l��", "�s�l��", "�G���[�H",
		};
		const char *TEBAN_CHAR[2] = {
			"��", "��"
		};
		const char *MOTION_STR[4] = {
			"", "��", "��", "��"
		};
		const char *POSITION_STR[4] = {
			"", "��", "��", "�E",
		};
		const char *OPTION_STR[4] = {
			"", "��", "�s��", "��"
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

		// �s����ꏊ
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

		//  �����ꏊ
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

