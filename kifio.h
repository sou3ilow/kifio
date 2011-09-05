#ifndef KIFIO_H
#define KIFIO_H


#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

//inline void strcpy_s(char*dest, const char*src) { strcpy(dest, src); }

namespace kifio {
	// see kifuio_strings.cpp
	namespace strings
	{
		extern const char* plus;
		//
		extern const char* kuro_sankaku;
		extern const char* shiro_sankaku;
		extern const char* taikyukubu;
		extern const char* kaishinichiji;
		extern const char* shuryonichiji;
		extern const char* teaiwari;
		extern const char* sente;
		extern const char* gote;
		extern const char* hyodai;
		extern const char* kisen;
		extern const char* senkei;
		extern const char* tesu_sashite_shohijikan;
		extern const char* kyuhachinanarokugoyonsanniichi;
		extern const char* space;
		extern const char* plusminusminusplus;
		extern const char* made;
		extern const char* sentenomochigoma;
		extern const char* gotenomochigoma;
		extern const char* shitatenomochigomo;
		extern const char* uwatenomochigoma;
		extern const char* senteban;
		extern const char* goteban;
		extern const char* shitateban;
		extern const char* uwateban;
		extern const char* empty;

		extern const char* douzenspace;
		extern const char* dou;
		extern const char* zenspace;
		extern const char* utsu;
		extern const char* naru;
		extern const char* hidari;
		extern const char* choku;
		extern const char* migi;
		extern const char* agaru;
		extern const char* yoru;
		extern const char* hiku;
		extern const char* funari;
		extern const char* nashi;
		extern const char* nakaguro;
		extern const char* nomochigoma;
		extern const char* hirate;
		extern const char* sonota;
	}


	/* basic types */

	/// owner
	enum owner_t {
		SENTE, GOTE
	};

	/// promotion
	enum prom_t {
		SAME, PROMOTE
	};

	/// piece state
	enum state_t {
		EP, FU, KY, KE, GI, KI, KA, HI, OU, TO, NY, NK, NG, NI, UM, RY,
	};

	/// special moves
	enum special_t {
		S_TORYO, S_CHUDAN, S_SENNICHITE, S_TIME_UP, S_ILLEGAL_MOVE,
		S_JISHOGI, S_KACHI, S_HIKIWAKE, S_MATTA, S_TSUMI, S_FUDUMI, S_ERROR,
	};

	/// board initialization type
	enum board_t {
		B_EMPTY,
		B_HIRATE, B_KYOOCHI, B_HIOCHI, B_KAKUOCHI,
		B_2MAI, B_4MAI, B_6MAI, B_8MAI, B_TUMESHOGI,
	};

	/// JSA move notation
	enum jsa_move_t { 
		M_NASHI = 0, 
		M_DOU = -1,							// 同
		M_AGARU = 1, M_YORU, M_HIKU,		// 動作
		M_HIDARI = 1, M_CHOKU, M_MIGI,		// 位置
		M_NARU = 1, M_NARAZU, M_UTU			// 成・不成・打 
	};

	/// HACK what is it
	inline int position012(int p) 
	{
		assert ( p == M_HIDARI || p == M_CHOKU || M_MIGI );
		return p-1;
	}

	struct BaseMove;	
	struct PieceMove;
	struct FullMove;
	struct JSAMove;
	struct Board;
	struct GameInfo;

	/// basic move
	struct BaseMove
	{
		/// position piece from. see also utl::pos, is_drop()
		int from;

		/// position piece move to. see also utl::pos, is_special()
		int to;

		/// promotion
		prom_t promote;
	public:
		/// initialize
		void init();
		
		/// whether move is piece dropping move
		bool is_drop() const;
	
		/// return the dropping piece
		int drop_piece() const;
	
		/// set move as drop move
		void set_drop(int pc, int to_pos);

		/// get fliped move
		BaseMove getflipmove(bool flip) const;

		/// make move flip
		BaseMove& flip(bool flip);
	public:
		/// whether move is special
		bool is_special() const;

		/// set mvoe as special move
		void set_special(int special);

		/// get sprcial type
		int  special_move() const;

	public:
		/// write in usi format
		void write_usi(std::ostream& os=std::cout) const;
		
		/// dump move for debug
		void write_debug(std::ostream& os=std::cout) const;

		/// dump move for debug
		void output() const;
	};
	bool operator==(const BaseMove& b1, const BaseMove& b2);
	bool operator!=(const BaseMove& b1, const BaseMove& b2);
	bool operator<(const BaseMove& b1, const BaseMove& b2);


	/// basic move += previous piece state
	struct PieceMove : public BaseMove
	{
		int piece; // pre

		PieceMove(){}
		PieceMove(const BaseMove& bm, int piece_at)
			: BaseMove(bm)
			  , piece(piece_at)
		{
		}

		void init()
		{
			BaseMove::init();
			piece = 0;
		}

		void write_pico(std::ostream&os=std::cout)const;
	};

	/// basic move += previous state + capture piece
	struct FullMove : public BaseMove
	{
		int piece; // pre
		int get;

		FullMove(){}
		FullMove(const BaseMove& bm, int piece_at, int get_at)
			: BaseMove(bm)
			  , piece(piece_at)
			  , get(get_at)
		{
		}

		void init()
		{
			BaseMove::init();
			piece = 0;
			get = 0;
		}
	};

	/// JSA (Japan Shogi Association) format move
	struct JSAMove
	{	
		int teban;
		int to;	// ref. utl::pos, jsa_move_t::M_DOU
		int state_before;
		int joge;
		int sayu;
		int narunarazuutu;
	public:
		void init();
		void write_ki2(std::ostream& os=std::cout) const;
		void write_ki2sp(std::ostream& os=std::cout) const;
	};

	/// simple board, board + captured piece + turn to move
	struct Board
	{
		/// the board. see also utl::sq, utl::piece
		int ban[82];

		/// pieces in hand
		int hand[2][8];

		/// turn to move
		int turn;

	public:
		/// xy, pos-format access. see also utl::pos, utl::piece
		int& at(int pos);	
		int& at(int x, int y);
		const int& at(int pos) const;	
		const int& at(int x, int y) const;
		void at(int pos, int* ow, int* st) const;
		void at(int x, int y, int* ow, int* st) const;

	public:
		/// bo
		bool parse_load_kaki_hand(int teban, const char* str, const char**rest=NULL);
		bool parse_load_kaki_board(const char* str, const char**rest=NULL);

		/// hand string parser for CSA format
		bool parse_load_csa_hand(const char* str, const char**rest=NULL);
		
		/// board string parser for CSA format
		bool parse_load_csa_board(const char* str, const char**rest=NULL);

	public:
		/// move string parser for KIF format
		bool parse_kif_move(const char* str, BaseMove& mv, int previous_to, const char** rest=NULL) const;
		/// move string parser for CSA froamt
		bool parse_csa_move(const char* str, BaseMove& mv, const char** rest=NULL) const;
		/// move string parser for KI2 froamt
		bool parse_ki2_move(const char* str, BaseMove& mv, int previous_to, const char** rest=NULL) const;
		/// move string parser for KI2-JSA froamt??
		bool parse_ki2_jsamove(const char* str, JSAMove& mv, const char** rest=NULL) const;

	public:
		/// move converter JSA->BASE
		bool jsa2base(const JSAMove& jsa, BaseMove& base, int previous_to) const;
		/// move converter BASE->JSA
		bool base2jsa(const BaseMove& base, JSAMove& jsa) const;
	private:
		/// get list of pieces that can move to @param to
		// toに移動可能なteban側のstate駒のリストを作り返す．
		// 打てる場合は必ず先頭で値は0
		int froms_to(int teban, int state, int to, int froms[]) const;

	public:
		/// initialize board by given type
		void init(board_t type=B_EMPTY);

	public:
		/// load pico string
		bool load_pico(const char* line);
		/// load sfen string
		bool load_sfen(const char* line);

	public:
		/// write in pico format
		void write_pico(std::ostream& os = std::cout) const;
		/// write in sfen format
		void write_sfen(std::ostream& os = std::cout) const;
		/// write in kakinoki format (also for KI2)
		void write_kaki(std::ostream& os = std::cout) const;
		/// write in csa format
		void write_csa(std::ostream& os = std::cout) const;
		/// dump for debug 
		void output() const;

	private:
		/// write hand in sfen format
		int  write_sfen_hand(int ow, std::ostream& os = std::cout) const;
		/// write hand in kaki format
		void write_kaki_hand(int ow, std::ostream& os = std::cout) const;
		/// write hand in csa format
		void write_csa_hand(int ow, std::ostream& os = std::cout) const;

	public:
		/// whether board is the initial position
		bool is_hirate_shokikyokumen() const;
		/// invert board	
		void invert();

	public:
		/// make move on board
		bool make(const BaseMove& mv);
		/// unmake move on board (not implemented?)
		bool unmake(const FullMove& mv); // 未実装
	};

	/// game information
	struct GameInfo
	{
		/// length of game record
		int length;
		/// filename 
		std::string filename;
		/// playername 
		std::string playername[2];
		/// date game start
		std::string time_begin;    
		/// date game end
		std::string time_end;      
		/// date of game
		std::string date;          
		/// event name?
		std::string eventname;     
		/// record title?
		std::string title;         
		/// opening formation 
		std::string opening;       
	public:
		/// initialize
		void init();
		/// dump for debug
		void write_debug(std::ostream& os=std::cout) const;
	};

	/// game record
	template<typename MOVE>
		struct RecordT
		{
		public:
			/// record iterator
			struct Iterator
			{
	 			/// pointer to the original record
				const RecordT& record;
 			
				/// ply to begin iteration 
				const int ply_begin;
 				
				/// ply to end iterator
				const int ply_end;
				
				/// flag whether skip to next comment
				const bool stopon_comment;
			
				/// current board
				Board current_board;
				
				/// current ply
				int ply_current;

			private:
 				/// not implemented
				void operator=(const Iterator&);

			public:
				/// normal constructor
				Iterator(const RecordT& record, int ply_begin, int ply_end, bool stopon_comment)
					: record(record)
					, ply_begin(ply_begin)
					, ply_end(ply_end)
					, stopon_comment(stopon_comment)
				{
					init();
				}
				/// copy constructor
				Iterator(const Iterator& it)
					: record(it.record)
					, ply_begin(it.ply_begin)
					, ply_end(it.ply_end)
					, stopon_comment(it.stopon_comment)
				{
					init();
				}
			public:
				/// returns current board
				const Board& operator*() const
				{
					return current_board;
				}
				/// returns current board
				const Board* operator->() const
				{
					return &current_board;
				}
			public:
				/// go back to the begining ply
				void init()
				{
					current_board = record.start;
					for ( ply_current=0; ply_current<ply_begin; ply_current++ )
						current_board.make(record[ply_current]);
				}
				
				/// whether iterator is in range
				operator bool() const { return ply_current < ply_end; }

				/// go to next
				void next()
				{
					ply_current++;
					for ( ; ply_current<ply_end; ply_current++ )
					{
						current_board.make(record[ply_current]);
						if ( !stopon_comment )
						 	break;
						// else if ( record.comment[ply_current].has_comment() )
						// 	break;
					}
				}
			};

		public:
			/// game infromation
			GameInfo info;
			/// begining positoin of the record
			Board start;
			/// list of moves in the record
			std::vector<MOVE> moves;
		public:
			/// normal constructor
			RecordT() { init(); }
			/// initializer 
			void init(board_t type=B_EMPTY);
		public:
			/// record accessor (read only)
			const MOVE& operator[](int i) const
			{
				return moves[i];
			}
		public:
			/// get iterator in specified range
			Iterator iterator(int start, int end, bool stopon_comment=false)
			{
				assert ( start != 0 );
				assert ( end != 0 );
			
				const int real_start = ( 0 < start ) ? start - 1 : moves.size() - start;
				const int real_end = ( 0 < end ) ? end : moves.size() - end + 1;
				return Iterator(*this, real_start, real_end, stopon_comment);
			}
			


		public:
			/// write formatted information
			void printinfo(const std::string& format, std::ostream& os)const;

		private:
			/// generate moves?
			MOVE gen(const BaseMove& bm, int piece_at, int piece_get) const;
		public:
			/// load file (auto type detection)
			bool load(const std::string& filename);
			/// load kif format file
			bool load_kif(const std::string& filename);
			/// load ki2 format file
			bool load_ki2(const std::string& filename) { return load_kif(filename); }
			/// load csa format file
			bool load_csa(const std::string& filename);
			/// save file (auto type detection)
			bool save(const std::string& filename) const;
			///  save file in kif format
			bool save_kif(const std::string& filename) const;
			///  save file in ki2 format
			bool save_ki2(const std::string& filename) const;
			///  save file in csa format
			bool save_csa(const std::string& filename) const;
		public:
			/// read stream as csa format record
			bool read_csa(std::istream& is);
			/// read stream as kif format record
			bool read_kif(std::istream& is);
			/// read stream as ki2 format record (alias of read_kif())
			bool read_ki2(std::istream& is) { return read_kif(is); }
		public:
			/// write record in csa format
			bool write_csa(std::ostream& os=std::cout) const;
			/// write record in kif format
			bool write_kif(std::ostream& os=std::cout) const;
			/// write record in ki2 format
			bool write_ki2(std::ostream& os=std::cout) const;

		public:
			/// what?
			bool base2jsa(std::vector<JSAMove>& jsamoves) const;
		};

	///
	typedef RecordT<BaseMove> Record;

	/// record file extention
	enum recordext_t { EXT_UNKNOWN, EXT_CSA, EXT_KIF, EXT_KI2, };
	/// record file extention
	recordext_t whichext(const std::string& filename);
} // namespace


namespace kifio
{
	/// returns array size
	template<typename type_t, int size>
		inline int arr_size(const type_t (&a)[size])
		{
			return size;
		}
	
	/// decord?
	inline int dec(char c)
	{
		return c - '0';
	}
	
	/// test string
	inline bool begin_with(const char* line, const char* target, const char** rest=NULL)
	{
		size_t len = strlen(target);
		if ( 0 == strncmp(line, target, len) )
		{
			if ( rest )
				*rest = &line[len];
			return true;
		}
		return false;
	}

	inline int index_of(const char* line, const char* list[], int listlen, const char** rest=NULL)
	{
		for ( int i=0; i<listlen; i++ )
			if ( begin_with(line, list[i], rest) )
				return i;

		return -1;
	}

	template<int SIZE>
		inline int index_of(const char*line, const char* (&list)[SIZE], const char**rest=NULL)
		{
			return index_of(line, list, SIZE, rest);
		}

	inline const char* skip_white(const char* org)
	{
		while ( *org == ' ' )
			org++;
		return org;
	}

	namespace utl
	{
		// pos形式	0x11-0x99
		// sq形式	1-81
		inline int pos(int x, int y)
		{
			return (x << 4) | y;
		}
		inline int posx(int p)
		{
			return ( p >> 4 ) & 0xf;
		}
		inline int posy(int p)
		{
			return ( p ) & 0xf;
		}
		inline void pos2xy(int p, int*x, int*y)
		{
			*x = posx(p); *y = posy(p);
		}
		inline void pos2str(int p, std::ostream& os = std::cout)
		{
			int x, y; pos2xy(p, &x, &y);
			os << char( '0' + x ) << char( '0' + y );
		}
		inline void pos2usistr(int p, std::ostream& os = std::cout)
		{
			int x, y; pos2xy(p, &x, &y);
			os << char( '0' + x ) << char( 'a' - 1 + y );
		}
		inline bool on_board(int p)
		{
			int x, y; pos2xy(p, &x, &y);
			return 1 <= x && x <= 9 && 1 <= y && y <= 9;
		}
		inline int posflip(int p)
		{
			assert ( on_board(p) );
			return 0xaa-p;
		}
		inline int sq(int x, int y)
		{
			return 9 * ( y - 1 ) + ( x - 1 ) + 1;
		}
		inline int sq2xy(int square, int* x, int* y)
		{
			*x = ( square - 1 ) % 9 + 1;
			*y = ( square / 9 ) + 1;
		}
		inline int pos2sq(int p)
		{
			int x, y;
			pos2xy(p, &x, &y);
			return sq(x, y);
		}
		inline int sq2pos(int square)
		{
			int x, y;
			sq2xy(square, &x, &y);
			return pos(x, y);
		}
		inline int piece(int owner, int state)
		{
			return ( owner << 8 ) | state;
		}
		inline void piece2os(int pc, int*owner, int*state)
		{
			*owner = pc >> 8;
			*state = pc & 0xff;
		}
		inline int owner(int pc)
		{
			return pc >> 8;
		}
		inline int piece_flip_owner(int pc)
		{
			return pc == EP ? EP : pc ^ 0x100; 
		}
		inline int state(int pc)
		{
			return pc & 0xff;
		}
		inline int aite(int ow)
		{
			return ow ^ 1;
		}
		inline int promote(int pc, prom_t prom)
		{
			return prom==SAME ? pc : ( pc | 0x8 );
		}
		inline bool is_promotable(int st)
		{
			const bool arr[] = {
				false, true, true, true, true, false, true, true,
				false, false, false, false, false, false, false, false };
			return arr[st];
		}
		inline bool can_promote(int from, int to, int teban)
		{
			int fy, ty;
			if ( teban == SENTE ) {
				fy = posy(from); ty = posy(to);
			} else {
				fy = 10 - posy(from); ty = 10 - posy(to);
			}
			// 敵陣が1-3段目の時，移動元か移動先が3段目以内なら成れる
			return ( ty <= 3 || fy <= 3 );
		}
		inline bool can_drop(int state, int to, int teban)
		{
			assert ( FU <= state && state <= HI );
			if ( GI <= state ) return true;
			int ty = ( teban == SENTE ) ? posy(to) : 10 - posy(to);
			if ( 3 <= ty ) return true;
			if ( ty <= 1 ) return false;
			assert ( ty == 2 && ( state == FU || state == KY || state == KE ) );
			return ( state == KE ) ? false : true;
		}
		inline int kind(int pc)
		{
			int s = state(pc);
			return s > OU ? s - OU : s;
		}
		const int PIECE_NB[] = { 0, 18, 4, 4, 4, 4, 2, 2, 2 };
	}

	// computer shogi association
	namespace csa
	{
		extern const char *STATE_CHAR[16];
		extern const char *STATE_STR[16];
		extern const char *SPECIAL_STR[13];
	}

	// japan shogi association
	namespace jsa
	{
		extern const char *ZEN_SPACE;
		extern const char *ZEN_SUJI[10];
		extern const char *KAN_SUJI[11];
		// 盤面用
		extern const char *STATE_CHAR[16];
		// 棋譜用
		extern const char *STATE_STR[16];
		// 持ち駒用
		extern const char *HAND_CHAR[8];
		/// alternate notation
		struct alt_t { const char *str; int value; };
		extern const alt_t STATE_ALT[2];
		extern const char *SPECIAL_STR[13];
		inline 	void zen_pos(int pos, std::ostream&os)
		{
			int x, y; utl::pos2xy(pos, &x, &y);
			os << ZEN_SUJI[x] << KAN_SUJI[y];
		}
		extern const char *TEBAN_CHAR[2];
		extern const char *MOTION_STR[4];
		extern const char *POSITION_STR[4];
		extern const char *OPTION_STR[4];
	}


	// universal shogi interface
	namespace usi
	{
		extern const char *STATE_STRS[2][16];
	}

	namespace pico
	{
		extern const char LINE_CHARS[2][16];
		inline int linepiece(char pc)
		{
			for ( int i=1; i<16; i++ )
				if ( LINE_CHARS[0][i]==pc )
					return i;
			return 0;
		}
		extern const char* LINEBOARD_STR[];
	}

	namespace dir
	{
		/// absolute dir
		enum adir_t
		{
			A_CNT = 0, A_UP = -0x01, A_RGT = -0x10, A_LFT = +0x10, A_DWN = +0x01,
			A_UL = A_UP + A_LFT, A_UR = A_UP + A_RGT, A_DL = A_DWN + A_LFT, A_DR = A_DWN + A_RGT,
			A_UUL = A_UP + A_UL, A_UUR = A_UP + A_UR, A_DDL = A_DWN + A_DL, A_DDR = A_DWN + A_DR
		};
		/// relative dir
		enum dir_t {
			CNT, UP, RGT, LFT, DWN, UL, UR, DL, DR, UUL, UUR, DDL, DDR
		};

		/// conversion table dir_t to adit_t
		extern const adir_t D2A[2][1+12];

		/// monility of piece
		struct mobility_t
		{
			dir_t walk[8+1]; /// walking mobility for directions
			dir_t slide[4+1]; /// sliding mobility for directions
		};

		/// mobility table where piece can move to
		extern mobility_t MOBILITY[];
		/// mobility table where piece can move form
		extern mobility_t MOBILITY_FROM[];
		///
		struct ido_t
		{
			// 移動した量を+1,0,-1で表す．
			// 先手の駒が99から11に移動した場合, -1, -1
			// 後手の駒が99から11に移動した場合, +1, +1
			int x;	// -1：右 +1：左
			int y;	// -1：上 +1：下

			int x012() const { return x + 1; }
			int y012() const { return y + 1; }


			int joge() const // 上寄引
			{
				static const int arr[] = { M_AGARU, M_YORU, M_HIKU };
				return arr[y+1];
			}
			// 右に移動した場合，その駒は「左」にいたのでそれを返す
			int sayu() const // 左直右
			{
				static const int arr[] = { M_HIDARI, M_CHOKU, M_MIGI };
				return arr[x+1];
			}
		};
		/// convert move to ido_t
		inline void convert(int origin, int pos, int teban, ido_t* j)
		{
			int ox, oy; utl::pos2xy(origin, &ox, &oy);
			int px, py; utl::pos2xy(pos, &px, &py);

			int dx = px - ox; dx = ( dx == 0 ) ? 0 : ( dx < 0 ? -1 : + 1 );
			int dy = py - oy; dy = ( dy == 0 ) ? 0 : ( dy < 0 ? -1 : + 1 );

			if ( teban == SENTE ) { j->y = +dy; j->x = +dx; }
			else                  { j->y = -dy; j->x = -dx; }
		}
	}

	inline void BaseMove::init()
	{
		to = 0;
		from = 0;
		promote = SAME;
	}
	inline int BaseMove::drop_piece() const
	{
		assert ( is_drop() );
		return from & 0x0f;
	}
	inline bool BaseMove::is_drop() const
	{
		return 0xf0 < from;
	}
	inline void BaseMove::set_drop(int pc, int to_pos)
	{
		to = to_pos;
		from = 0xf0 | pc;
		promote = SAME;
	}
	inline bool BaseMove::is_special() const
	{
		assert ( to != 0 || (promote == SAME && 0<= from) );
		return to == 0;
	}
	inline void BaseMove::set_special(int special)
	{
		to = 0;
		from = special;
		promote = SAME;
	}
	inline int BaseMove::special_move() const
	{
		assert ( is_special() );
		return from; // as special
	}
	inline BaseMove BaseMove::getflipmove(bool flip) const
	{
		if ( !flip || is_special() )
			return *this;

		BaseMove ret = *this;
		return ret.flip(flip);
	}
	inline BaseMove& BaseMove::flip(bool flip)
	{
		if ( !flip || is_special() )
			;
		else
		{
			to = utl::posflip(to);
			if ( !is_drop() )
				from = utl::posflip(from);
		}
		return *this;
	}
	inline void BaseMove::write_debug(std::ostream& os) const
	{
		if ( is_special() )
			os << "SP! " << from;
		else if ( is_drop() )
		{
			jsa::zen_pos(to, os);
			os << jsa::STATE_CHAR[drop_piece()];
		}
		else
		{
			jsa::zen_pos(to, os);
			int x, y;
			utl::pos2xy(from, &x, &y);
			os << '(' << x << y << ')' << ( promote == PROMOTE ? "!!" : "--");
		}
	}
	inline void PieceMove::write_pico(std::ostream&os) const	
	{
		using std::setfill;
		using std::hex;
		using std::setw;

		if ( is_special() )
			os << "SP! " << from;
		else if ( is_drop() )
		{
			os	<< "00"
				<< hex << setw(2) << setfill('0') << to
				<< char(pico::LINE_CHARS[0][drop_piece()])
				<< "-";
		}
		else
		{
			os	<< hex << setw(2) << setfill('0') << from
				<< hex << setw(2) << setfill('0') << to 
				<< char(pico::LINE_CHARS[0][piece])
				<< ( promote ? "*" : "-");
		}
	}


	inline void BaseMove::output() const
	{
		write_debug();
		std::cout << "\n";
	}
	inline void BaseMove::write_usi(std::ostream& os) const
	{
		assert ( !is_special() ); // TODO 非対応
		if ( is_drop() )
		{
			os << usi::STATE_STRS[drop_piece()] << '*';
			utl::pos2usistr(to, os);
		}
		else
		{
			utl::pos2usistr(from, os);
			utl::pos2usistr(to, os);
			if ( promote )
				os << '+';
		}
	}

	inline bool operator==(const BaseMove& b1, const BaseMove& b2)
	{
		return b1.from == b2.from && b1.to == b2.to && b1.promote == b2.promote;
	}
	inline bool operator!=(const BaseMove& b1, const BaseMove& b2)
	{
		return ! operator ==(b1, b2);
	}
	inline bool operator<(const BaseMove& b1, const BaseMove& b2)
	{
#define NEQ(ID)	if ( b1.ID != b2.ID ) return b1.ID < b2.ID
		NEQ(from);
		NEQ(to);
		NEQ(promote);
		return false; // equal
#undef NEQ
	}

	inline int& Board::at(int pos) { return ban[utl::pos2sq(pos)]; }
	inline const int& Board::at(int pos) const { return ban[utl::pos2sq(pos)]; }
	inline void Board::at(int pos, int* ow, int* st) const
	{
		int pc = ban[utl::pos2sq(pos)];
		*ow = utl::owner(pc);
		*st = utl::state(pc);
	}
	inline int& Board::at(int x, int y) { return ban[utl::sq(x, y)]; }
	inline const int& Board::at(int x, int y) const { return ban[utl::sq(x, y)]; }
	inline void Board::at(int x, int y, int*ow, int*st) const
	{
		int pc = ban[utl::sq(x, y)];
		*ow = utl::owner(pc);
		*st = utl::state(pc);
	}
	inline void Board::init(board_t type)
	{
		if ( type == B_EMPTY )
		{
			memset( &ban, EP, sizeof ban );
			memset( &hand, 0, sizeof hand );
			turn = SENTE;
		}
		else
			load_pico(pico::LINEBOARD_STR[type]);
	}
	inline void Board::write_pico(std::ostream& os) const
	{
		os << ( turn == SENTE ? '+' : '-' ) << '/';
		for ( int i=1; i<=81; i++ )
		{
			int pc = ban[i];
			if ( pc==EP )
			{
				os << '.';
				continue;
			}

			int ow, st;
			utl::piece2os(ban[i], &ow, &st);
			os << pico::LINE_CHARS[ow][st];
		}

		for ( int ow=0; ow<2; ow++ )
		{
			os << '/';
			for ( int pc=HI; pc>FU; pc-- )
				os << hand[ow][pc];
			os << std::setw(2) << std::setfill('0') << hand[ow][FU];
		}
	}
	inline bool Board::load_pico(const char line[])
	{
		const char* p=line;

		turn = ( *p=='+' ) ? SENTE : GOTE;
		p++;
		p++;	// for /

		for ( int i=0; i<81; i++, p++ )
		{
			int ow = isupper(*p);
			int st = pico::linepiece(tolower(*p));
			ban[i+1]=utl::piece(ow, st);
		}
		for ( int ow=0; ow<2; ow++)
		{
			p++;	// for /
			for ( int pc=HI; pc>FU; pc--, p++ )
				hand[ow][pc]= dec( *p );

			hand[ow][FU] = dec( p[0] ) * 10 + dec( p[1] );
			p++;
			p++;
		}
		return true;
	}
	inline bool Board::load_sfen(const char* line)
	{
		this->init();

		// eg.
		// ln3k1nl/1r1sgsgb1/p1pp1p1pp/1p2p1p2/9/2PPP4/PPS2PPPP/1BG1GS1R1/LN1K3NL w - 1

		const char* p = line;

		// board
		{
			int x=1, y=1;
			while ( *p != ' ' )
			{
				if ( *p == '/' )
				{
					y++;
					x=1;
					p++; // feed
				}
				else if ( isdigit(*p) )
				{
					int n = dec(*p);
					for ( int i=0; i<n; i++, x++ )
						this->at(x, y) = EP;

					p++; // feed
				}
				else
				{
					int ow;

					// 成り駒判定
					if ( *p == '+' ) ow = isupper(p[1]) ? SENTE : GOTE;
					else             ow = isupper(p[0]) ? SENTE : GOTE;

					int st = index_of(p, usi::STATE_STRS[ow], &p); // feed 1or2;
					this->at(x, y) = utl::piece(ow, st);
					x++;
				}
			}

			p++; // feed ' '
		}

		// turn
		this->turn = ( *p == 'b' ) ? SENTE : GOTE;
		p++; // feed ;
		p++; // feed ' ';

		// hand
		while ( *p != ' ' )
		{
			if ( *p == '-' ) // 持ち駒無し
			{
				p++;
				continue;
			}

			int ow = isupper(*p) ? SENTE : GOTE;
			int st = index_of(p, usi::STATE_STRS[ow], &p); // feed 1or2;

			int n = 1;
			if ( isdigit(*p) )
			{
				n = atoi(p);

				if ( 10 <= n ) p += 2;
				else           p += 1;
			}

			this->hand[ow][st] = n;
		}

		p++; // feed ' '

		// 手数が残ってるんだが無視で．
		return true;
	}
	inline void Board::write_kaki(std::ostream& os) const
	{
		write_kaki_hand(GOTE, os);
		os << strings::kyuhachinanarokugoyonsanniichi << "\n";
		os << strings::plusminusminusplus            << "\n";

		for ( int y=1; y<=9; y++ )
		{
			os << "|";
			for ( int x=9; x>=1; x-- )
			{
				int pc = ban[utl::sq(x, y)];
				if ( pc == EP )
				{
					os << " " << strings::nakaguro;
					continue;
				}
				int ow, st;
				utl::piece2os(pc, &ow, &st);
				os << ( ow==SENTE ? ' ' : 'v' ) << jsa::STATE_CHAR[st];
			}
			os << "|" << jsa::KAN_SUJI[y] << "\n";
		}
		os << strings::plusminusminusplus            << "\n";
		write_kaki_hand(SENTE, os);
		os << ( this->turn == SENTE ? strings::senteban : strings::goteban ) << '\n';
	}
	inline void Board::write_kaki_hand(int ow, std::ostream& os) const
	{
		bool morethanone = false;

		// HACKどうも駒落ちは上手，下手，とやるらしい
		os << ( ow == SENTE ? strings::sente : strings::gote ) << strings::nomochigoma ;

		for ( int pc=HI; pc>FU; pc-- )
		{
			int num = hand[ow][pc];
			if ( num == 0 ) continue;
			os << jsa::STATE_CHAR[pc];

			if ( 2 <= num )
				os << jsa::KAN_SUJI[num];

			os << jsa::ZEN_SPACE;

			morethanone = true;
		}

		int num = hand[ow][FU];
		if ( 0<num )
			morethanone = true;
		if ( 1<num ) // 一の時は数字無し
		{
			os << jsa::STATE_CHAR[FU];
			if ( 10<=num )
			{
				os << jsa::KAN_SUJI[10];
				num -= 10;
			}
			if ( 0<num )
				os << jsa::KAN_SUJI[num];
		}

		if ( !morethanone )
			os << strings::nashi;

		os << '\n';
	}
	inline void Board::write_csa_hand(int ow, std::ostream& os) const
	{
		os << 'P' << ( ow == SENTE ? '+' : '-' );

		for ( int pc=HI; pc>=FU; pc-- )
		{
			int n=hand[ow][pc];
			while ( n-- )
				os << "00" << csa::STATE_CHAR[pc];
		}

		os << '\n';
	}
	inline bool Board::is_hirate_shokikyokumen() const
	{
		std::stringstream ss;
		this->write_pico(ss);
		return  0 == strcmp(pico::LINEBOARD_STR[B_HIRATE], ss.str().c_str());
	}
	inline void Board::invert()
	{
		turn = turn == SENTE ? GOTE : SENTE;
		{ // swap
			int tmp[8];
			memcpy(tmp, hand[0], sizeof(tmp));
			memcpy(hand[0], hand[1], sizeof(tmp));
			memcpy(hand[1], tmp, sizeof(tmp));
		}

		// 中心の40を挟んで反転
		for ( int p=1; p<82/2; p++ )  
		{
			int i=82-p;

			{ // swap
				int ip = ban[i];
				int pp = ban[p];
				ban[i] = utl::piece_flip_owner(pp);
				ban[p] = utl::piece_flip_owner(ip);
			}
		}
	}
	inline void Board::write_csa(std::ostream& os) const
	{
		for ( int y=1; y<=9; y++ )
		{
			os << 'P' << y;
			for ( int x=9; x>=1; x-- )
			{
				int pc = ban[utl::sq(x, y)];
				if ( pc == EP )
				{
					os << " * ";
					continue;
				}
				int ow, st;
				utl::piece2os(pc, &ow, &st);
				os << ( ow==SENTE ? '+' : '-' ) << csa::STATE_CHAR[st];
			}
			os << '\n';
		}
		write_csa_hand(SENTE, os);
		write_csa_hand(GOTE, os);
		os << ( turn == SENTE ? '+' : '-' ) << '\n';
	}
	inline void Board::write_sfen(std::ostream& os) const
	{
		for ( int y=1; y<=9; y++ )
		{
			int spaces = 0;
			for ( int x=9; x>=1; x-- )
			{
				int ow, st; this->at(x, y, &ow, &st);

				if ( st == EP )
					spaces ++;
				else
				{
					if ( spaces != 0 )
					{
						os << spaces;
						spaces = 0;
					}
					os << usi::STATE_STRS[ow][st];
				}
			}
			if ( spaces != 0 ) os << spaces;
			if ( y != 9 ) os << '/';
		}
		os << ' ' << ( turn == SENTE ? 'b' : 'w' );

		os << ' ';

		int s = write_sfen_hand(SENTE, os);
		int g = write_sfen_hand(GOTE, os);

		if ( s == 0 && g == 0 )
			os << '-';

		os << ' ' << 1; // 将棋所にならう
	}
	inline int Board::write_sfen_hand(int ow, std::ostream& os) const
	{
		// 持ち駒無しなら0を返す

		int sum = 0;

		for ( int kind=HI; kind>=FU; kind-- )
		{
			int n = this->hand[ow][kind];
			if ( n == 0 ) continue;

			os << usi::STATE_STRS[ow][kind];
			if ( 2 <= n )
				os << n;

			sum += n;
		}
		return sum;
	}
	inline void Board::output() const
	{
		write_kaki(std::cout);
	}
	inline bool Board::make(const BaseMove& m)
	{
		if ( m.is_drop() )
		{
			int koma = m.drop_piece();
			// 手駒あり
			if ( hand[turn][koma] <= 0 ) return false;
			// 空きマス
			if ( at(m.to) != EP ) return false;

			hand[turn][koma]--;
			at(m.to) = utl::piece(turn, koma);
		}
		else
		{
			int koma = at(m.from);
			// 自分の手番
			if ( koma == EP || utl::owner(koma) != turn )
				return false;
			// 空きマスor相手の駒
			if ( !( at(m.to) == EP || utl::owner( at(m.to) )==utl::aite(turn)) )
				return false;
			if ( at(m.to) != EP )
				hand[turn][ utl::kind( at(m.to) ) ]++;
			at(m.to) = utl::promote(at(m.from), m.promote);
			at(m.from) =EP;
		}
		turn = utl::aite(turn);
		return true;
	}
	inline void GameInfo::init()
	{
		length = -1;

#define CL(S) S.clear()
		CL(date);
		CL(filename);
		CL(playername[0]);
		CL(playername[1]);
		CL(time_begin);
		CL(time_end);
		CL(eventname);
		CL(opening);
		CL(title);
#undef CL
	}
	inline void GameInfo::write_debug(std::ostream& os)const
	{
		// 空でなければ出力
#define OT(S) if ( !S.empty() ) std::cout << '\t' << #S << ": " << S << "\n"
		OT(date);
		OT(filename);
		OT(playername[0]);
		OT(playername[1]);
		OT(time_begin);
		OT(time_end);
		OT(eventname);
		OT(opening);
		OT(title);
#undef OT
	}
	enum line_t
	{
		L_UNKNOWN,
		L_EMPTY, L_COMMENT, L_DATE, L_TIME_BEGIN, L_TIME_END, L_TEAI,
		L_NAME_SEN, L_NAME_GO, L_RECORD_BEGIN, L_RECORD_END, L_MOVECOMMENT,
		L_TEBAN_SEN, L_TEBAN_GO,
		L_EVENT, L_OPENING,
		// kaki only?
		L_MOVE, L_TITLE, L_HAND_SEN, L_HAND_GO,
		// csa only?
		L_PLACE, L_TIME, L_BOARD, L_MOVETIME,
		// ki2 only?
		L_MOVE_KI2,
		// csa only
		L_VERkioN,
	};
	namespace jsa
	{
		inline line_t linetype(const char line[], const char** param=NULL)
		{
			struct { char c; line_t type; } *cp, c_types[] =
			{
				{ '\0', L_EMPTY },
				{ '#', L_COMMENT },
				{ '*', L_MOVECOMMENT },
				{ '|', L_BOARD },
				{ '-', L_UNKNOWN },
			};
			for ( cp = c_types; cp->type!=L_UNKNOWN; cp++ )
				if ( line[0] == cp->c )
					return cp->type;

			struct { const char* s; line_t type; } *sp, s_type[] =
			{
				{ strings::kuro_sankaku                   , L_TEBAN_SEN }    , // ki2
				{ strings::shiro_sankaku                  , L_TEBAN_GO }     , // ki2
				{ strings::taikyukubu                     , L_DATE }         , 
				{ strings::kaishinichiji                  , L_TIME_BEGIN }   , 
				{ strings::shuryonichiji                  , L_TIME_END }     , 
				{ strings::teaiwari                       , L_TEAI }         , 
				{ strings::sente                          , L_NAME_SEN }     , 
				{ strings::gote                           , L_NAME_GO }      , 
				{ strings::hyodai                         , L_TITLE }        , 
				{ strings::kisen                          , L_EVENT }        , 
				{ strings::senkei                         , L_OPENING }      , 
				{ strings::tesu_sashite_shohijikan        , L_RECORD_BEGIN } , 
				{ strings::kyuhachinanarokugoyonsanniichi , L_EMPTY}         , 
				{ strings::space                          , L_MOVE }         , // HACK 評価順序がー．
				{ strings::plusminusminusplus             , L_EMPTY}         , 
				{ strings::made                           , L_RECORD_END }   , 
				{ strings::sentenomochigoma               , L_HAND_SEN }     , 
				{ strings::gotenomochigoma                , L_HAND_GO }      , 
				{ strings::shitatenomochigomo             , L_HAND_SEN }     , // ?
				{ strings::uwatenomochigoma               , L_HAND_GO }      , // 上に表示にして後tebanから始めてみる
				{ strings::senteban                       , L_TEBAN_SEN }    , 
				{ strings::goteban                        , L_TEBAN_GO }     , 
				{ strings::shitateban                     , L_TEBAN_SEN }    , 
				{ strings::uwateban                       , L_TEBAN_GO }     , 
				{ strings::empty                          , L_UNKNOWN }
			};
			for ( sp = s_type; sp->type!=L_UNKNOWN; sp++ )
				if ( begin_with(line, sp->s, param ) )
					return sp->type;

			std::cout << "unknown type: [" << line << "]\n";
			return L_UNKNOWN;
		}
	}
	inline bool Board::parse_kif_move(const char* str, BaseMove& mv, int previous_to, const char** rest) const
	{
		mv.init();

		const char *p = str + 5; // 空白+指し手数分すすめる

		// 特殊手？
		int special = index_of(p, jsa::SPECIAL_STR);
		if ( special != -1 )
		{
			mv.set_special(special);
			return true;
		}

		// 行き先
		int to;
		if ( begin_with(p, strings::douzenspace, &p) )
		{
			to = previous_to;
			begin_with(p, "　", &p); // 読み捨て
		}
		else
		{
			int tx = index_of(p, jsa::ZEN_SUJI, &p);
			int ty = index_of(p, jsa::KAN_SUJI, &p);
			if ( tx < 0 ) return false;
			if ( ty < 0 ) return false;
			to = utl::pos(tx, ty);
		}

		// 駒種
		int komashu = index_of(p, jsa::STATE_STR, &p);
		// HACK 王とか竜とかの表記用
		if ( komashu < 0 )
		{
			for ( int i=0; i<kifio::arr_size(jsa::STATE_ALT); i++ )
				if ( begin_with(p, jsa::STATE_ALT[i].str, &p) )
					komashu = jsa::STATE_ALT[i].value;

			if ( komashu < 0 )
				return false;
		}

		static const char *PUT_PROM[] = {
			strings::utsu, strings::naru	
		};
		switch ( index_of(p, PUT_PROM, &p) )
		{
			case 0:// 打
				mv.set_drop(komashu, to);
				break;
			case 1: // 成
				mv.promote = PROMOTE;
				// no break;
			default:// (xy)
				mv.to = to;
				mv.from = utl::pos(dec(p[1]), dec(p[2]));
				break;
		}

		if ( rest ) *rest = p;
		return true;
	}


	inline void JSAMove::init()
	{
		teban = SENTE;
		to = 0;
		state_before = EP;
		joge = M_NASHI;
		sayu = M_NASHI;
		narunarazuutu = M_NASHI;
	}
	inline void JSAMove::write_ki2(std::ostream& os) const
	{
		os << jsa::TEBAN_CHAR[teban];

		if ( to == M_DOU )
			os << strings::douzenspace; 
		else
		{
			int tx, ty; utl::pos2xy(to, &tx, &ty);
			os << jsa::ZEN_SUJI[tx] << jsa::KAN_SUJI[ty];
		}
		os	<< jsa::STATE_STR[state_before]
			<< jsa::POSITION_STR[sayu]
			<< jsa::MOTION_STR[joge]
			<< jsa::OPTION_STR[narunarazuutu];
	}
	inline void JSAMove::write_ki2sp(std::ostream& os) const
	{
		std::stringstream ss;
		write_ki2(ss);
		os << std::setw(12) << std::setfill(' ') << std::left << ss.str() << std::right;
	}
	inline bool Board::parse_ki2_jsamove(const char* str, JSAMove& move, const char** rest) const
	{
		move.init();

		const char*p = str;

		// 先後
		if ( begin_with(p, strings::kuro_sankaku, &p) )//|| begin_with(p, "▼", &p) )
			move.teban = SENTE;
		else if ( begin_with(p, strings::shiro_sankaku, &p) )//|| begin_with(p, "▽", &p) )
			move.teban = GOTE;

		// 行き先
		if ( begin_with(p, strings::dou, &p) )
		{
			begin_with(p, strings::zenspace, &p); // 読み捨て
			move.to = M_DOU;
		}
		else
		{
			int tx = index_of(p, jsa::ZEN_SUJI, &p);
			int ty = index_of(p, jsa::KAN_SUJI, &p);
			if ( tx < 0 )
				return false;
			if ( ty < 0 )
				return false;
			move.to = utl::pos(tx, ty);
		}

		// 駒種
		{
			int komashu = index_of(p, jsa::STATE_STR, &p);
			// HACK 王とか竜とかの表記用
			if ( komashu < 0 )
			{
				for ( int i=0; i<arr_size(jsa::STATE_ALT); i++ )
					if ( begin_with(p, jsa::STATE_ALT[i].str, &p) )
						komashu = jsa::STATE_ALT[i].value;

				if ( komashu < 0 )
					return false;
			}
			move.state_before = komashu;
		}

		static const char * A[] = { strings::hidari, strings::choku, strings::migi}; 
		static const char * B[] = { strings::agaru, strings::yoru, strings::hiku};
		static const char * C[] = { strings::naru, strings::funari, strings::utsu};

		int q;
		if ( 0 <= (q = index_of(p, A, &p)) ) move.sayu = q + 1;
		if ( 0 <= (q = index_of(p, B, &p)) ) move.joge = q + 1;
		if ( 0 <= (q = index_of(p, C, &p)) ) move.narunarazuutu = q + 1;

		if ( rest ) *rest = p;
		return true;
	}
	inline bool Board::parse_load_kaki_hand(int teban, const char* str, const char**rest)
	{
		const char * p = str;

		memset(hand[teban], 0, sizeof hand[teban]);
		if ( begin_with(p, strings::nashi, &p) )
			;	// やること無し
		else
			for (;;)
			{
				int komashu = index_of(p, jsa::HAND_CHAR, &p);
				if ( komashu == -1 ) break;

				int a = 0, b = 0;
				if ( begin_with(p, strings::plus, &p) ) a = 10;
				b = index_of(p, jsa::KAN_SUJI, &p);
				if ( b<0 ) b = 1; // 一は書かないので

				hand[teban][komashu] = a + b;

				// 空白読み飛ばし
				while ( begin_with(p, jsa::ZEN_SPACE, &p )
						|| begin_with(p, " ", &p ) )
					;
			}
		if ( rest ) *rest = p;
		return true;
	}
	inline bool Board::parse_load_kaki_board(const char* str, const char**rest)
	{
		const char *p = str + 1;
		const char *ystr = str + 2 + (3 * 9);//sizeof "| ・ ・ ・ ・ ・ ・ ・ ・ ・|";

		int y = index_of(ystr, jsa::KAN_SUJI, &ystr);
		if ( y == -1 ) return false;

		assert ( 1<=y && y<=9 );

		for ( int x=9; x>=1; x-- )
		{
			int ow = (p[0] == ' ') ? SENTE : GOTE;
			int sw = index_of(p+1, jsa::STATE_CHAR);
			if ( sw == -1 )
				return false;
			if ( sw != 0 )
				at(utl::pos(x, y)) = utl::piece(ow, sw);
			p += 3;
		}
		if ( rest ) *rest = ystr;
		return true;
	}
	inline bool Board::parse_load_csa_hand(const char* str, const char**rest)
	{
		// P+00FU
		const char *p = str + 1;
		int ow =  ( *p == '+' ) ? SENTE : GOTE;
		p++;

		while ( p[0] == '0' )
		{
			int koma = index_of(&p[2], csa::STATE_CHAR);
			if ( koma < FU )
				return false;
			hand[ow][koma] ++;
			p += 4;
		}
		return true;
	}
	inline bool Board::parse_load_csa_board(const char* str, const char**rest)
	{
		// P1-KY-KE-GI-KI-OU-KI-GI-KE-KY
		const char *p = str + 1;
		int y = dec(*p);
		p++;

		for ( int x=9; x>=1; x-- )
		{
			if ( *p == ' ' )
				;
			else
			{
				int ow, st;
				ow = ( p[0] == '+' ) ? SENTE : GOTE;
				st = index_of(&p[1], csa::STATE_STR);
				if ( st < FU )
					return false;

				this->at(utl::pos(x, y)) = utl::piece(ow, st);
			}
			p += 3;
		}
		if ( rest ) *rest = p;
		return true;
	}
	inline bool Board::jsa2base(const JSAMove& jsa, BaseMove& base, int previous_to) const
	{
		base.init();

		// とりあえず「打」なら確実
		if ( jsa.narunarazuutu == M_UTU )
		{
			base.set_drop(jsa.state_before, jsa.to);
			return true;
		}
		base.to = ( jsa.to == M_DOU ) ? previous_to : jsa.to;
		base.promote = (jsa.narunarazuutu == M_NARU) ? PROMOTE : SAME;

		// 候補手確認
		int froms[6];
		int num = this->froms_to(turn, jsa.state_before, base.to, froms);

		assert( 0 < num );

		if ( num == 1 )
		{
			if ( froms[0] == 0 ) // 「打」ではない駒打ち
				base.set_drop(jsa.state_before, jsa.to);
			else
				base.from = froms[0];
			return true;
		}
		// HACK眠たいはっく打てる状況下で動く

		else if ( num == 2 && froms[0] == 0 )
		{
			base.from = froms[1];
			return true;
		}

		assert ( jsa.joge != M_NASHI || jsa.sayu != M_NASHI );

		dir::ido_t relfroms[6];
		for ( int i=0; i<num; i++ )
			convert(froms[i], jsa.to, turn, &relfroms[i]);

		for ( int i=0; i<num; i++ )
			if ((jsa.joge == M_NASHI || relfroms[i].joge() == jsa.joge )
					&&	(jsa.sayu == M_NASHI || relfroms[i].sayu() == jsa.sayu ))
			{
				base.from = froms[i];
				return true;	// 一つに絞れるはずなので即戻る
			}

		assert(0); // 見つからなかった？
		return false;
	}
	inline bool Board::base2jsa(const BaseMove& base, JSAMove& jsa) const
	{
		jsa.init();

		if ( base.is_special() )
		{
			std::cout << "cannot convert special move"; // そのうち対応
			return false;
		}

		jsa.teban = this->turn;
		jsa.to = base.to;
		jsa.state_before = base.is_drop()
			?	base.drop_piece()
			:	utl::state(this->at(base.from));

		// M_UTUかどうかはまだ不明
		if ( !base.is_drop()
				&& utl::is_promotable(jsa.state_before)
				&& utl::can_promote(base.from, base.to, this->turn) )
			jsa.narunarazuutu = ( base.promote == PROMOTE )
				?	M_NARU
				:	M_NARAZU;

		// joge, sayu, M_UTU の設定
		jsa.joge = M_NASHI;
		jsa.sayu = M_NASHI;

		// 候補手確認
		int froms[6];
		int num = this->froms_to(turn, jsa.state_before, jsa.to, froms);

		// 移動手の開始位置
		int idou_start = ( froms[0] == 0 ) ? 1 : 0;

		// 一つしかない
		if ( num == 1 ) return true;

		// 移動も可能な場所だが打った
		if ( base.is_drop() )
		{
			jsa.narunarazuutu = M_UTU;
			return true;
		}
		// 以下打つはあり得ない
		// 移動は一つしか無く「打つ」ではなかった
		if ( num == 2 && idou_start == 1 )
			return true;

		// 何番目？
		int idx = -1;
		for ( int i=idou_start; i<num; i++ )
			if ( froms[i] == base.from )
			{
				idx = i;
				break;
			}
		assert ( 0<=idx );

		// more init
		int sum_y[3], sum_x[3];
		memset(sum_y, 0, sizeof sum_y);
		memset(sum_x, 0, sizeof sum_x);

		// jogesayuに変換
		dir::ido_t relfroms[6];
		for ( int i=idou_start; i<num; i++ )
		{
			dir::ido_t& c = relfroms[i];
			dir::convert(froms[i], base.to, this->turn, &c);
			sum_y[ c.y012() ] ++;
			sum_x[ c.x012() ] ++;
		}

		// 選ばれているもの
		const dir::ido_t sel = relfroms[idx];

		if ( sum_y[sel.y012()] == 1 )
			jsa.joge = sel.joge();
		else if ( sum_x[sel.x012()] == 1 )
		{
			jsa.sayu = sel.sayu();

			// 飛車角は直が無い
			if ( jsa.sayu == M_CHOKU )
				if ( utl::kind(jsa.state_before) == KA || utl::kind(jsa.state_before) == HI )
					jsa.sayu = ( 0 < sum_x[position012(M_MIGI)] ) ? M_HIDARI : M_MIGI;
			return true;
		}
		else
		{
			jsa.joge = sel.joge();
			jsa.sayu = sel.sayu();
		}
		return true;
	}
	inline int Board::froms_to(int teban, int state, int to, int froms[]) const
	{
		using dir::dir_t;

		const dir::adir_t* A = dir::D2A[teban];

		int ret = 0;

		// 持ち駒を打てるときは先頭に
		if ( state <= HI
				&& at(to) == EP
				&& 0 < hand[teban][state]
				&& utl::can_drop(state, teban, to) )
		{
			froms[ret++] = 0;

			// 二歩チェック
			if ( state == FU )
			{
				int tx = utl::posx(to);
				for ( int ty=1; ty<9; ty++ )
				{
					int ow, st;
					utl::piece2os(at(utl::pos(tx, ty)), &ow, &st);
					if ( st == FU && ow == teban )
					{
						ret--;
						break;
					}
				}
			}
		}

		const int piece_max = utl::PIECE_NB[state];

		dir::mobility_t mob = dir::MOBILITY_FROM[state];
		// walk
		for ( dir_t* d = mob.walk; *d != 0; ++d )
		{
			int from = to + A[*d];
			if ( !utl::on_board(from) ) continue;
			int piece = at(from);
			if ( piece == EP ) continue;
			int ow, st; utl::piece2os(piece, &ow, &st);
			if ( ow != teban || st != state ) continue;
			froms[ret++] = from;
			if ( piece_max <= ret ) return ret;
		}

		// slide
		for ( dir_t* d = mob.slide; *d != 0; ++d )
		{
			const dir::adir_t ad = A[*d];

			int from;
			int piece = EP;
			for ( from=to+ad; utl::on_board(from); from+=ad )
				if ( EP != (piece = at(from)) )
					break;

			if ( piece == EP ) continue; // 番外に出た
			int ow, st; utl::piece2os(piece, &ow, &st);
			if ( ow != teban || st != state ) continue;
			froms[ret++] = from;
			if ( piece_max <= ret ) return ret;
		}

		assert ( 0<ret );
		return ret;
	}
	inline bool Board::parse_ki2_move(const char* str, BaseMove& mv, int previous_to, const char** rest) const
	{
		JSAMove move;
		if ( !parse_ki2_jsamove(str, move, rest) )
			return false;

		if ( !this->jsa2base(move, mv, previous_to) )
			return false;
		return true;
	}
	inline bool Board::parse_csa_move(const char* str, BaseMove& mv, const char** rest) const
	{
		// "+7776FU,T0"
		mv.init();

		const char *p = str;

		// 特殊手？
		int special = index_of(p, csa::SPECIAL_STR);
		if ( special != -1 )
		{
			mv.set_special(special);
			return true;
		}
		// 移動元
		int fx = dec(p[1]);
		int fy = dec(p[2]);
		// 移動先
		int to = utl::pos( dec(p[3]), dec(p[4]) );

		p += 5;

		// 駒種
		int komashu = index_of(p, csa::STATE_STR, &p);
		if ( komashu < 0 ) return false;

		if ( fx == 0 && fy == 0 )
			mv.set_drop(komashu, to);
		else
		{
			mv.to = to;
			mv.from = utl::pos(fx, fy);

			// 成った？
			mv.promote = ( utl::state(this->at(mv.from)) == komashu )
				?	SAME
				:	PROMOTE;
		}

		if ( rest ) *rest = p;
		return true;
	}

	// RecordT
	template<typename MOVE>
	void RecordT<MOVE>::printinfo(const std::string& format, std::ostream& os)const
		{
			std::stringstream ret;
			for ( std::string::const_iterator it=format.begin(); it!=format.end(); ++it )
			{
				if ( it+1 == format.end() )
				{
					ret << *it;
					continue;
				}

				switch ( *it )
				{
				case '\\':
					switch ( *(++it) )
					{
					case 'n': ret << '\n'; break;
					case '\\': ret << '\\'; break; 
					default:
						throw std::runtime_error(std::string("unknown format char: \\") + *it );
					}
					break;
				case '%':
					switch ( *(++it) )
					{
					case 'd': ret << info.date; break;
					case 'b': ret << info.playername[0]; break;
					case 'w': ret << info.playername[1]; break;
					case 's': ret << info.time_begin; break;
					case 'e': ret << info.time_end; break;
					case 'E': ret << info.eventname; break;
					case 'o': ret << info.opening; break;
					case 't': ret << info.title; break;
					case 'l': ret << moves.size(); break;
					case '%': ret << '%'; break;
					default:
						throw std::runtime_error(std::string("unknown format char: %") + *it );
					}
					break;
				default: // normal characters
					ret << *it;
				}
			}
			os << ret.str();
		}
	template<typename MOVE>
		bool RecordT<MOVE>::load_kif(const std::string& filename)
		{
			std::ifstream inf(filename.c_str());

			if ( !inf.is_open() )
			{
				std::cout << "cannot open : " << filename;
				return false;
			}

			bool ret = read_kif(inf);

			info.filename = filename;
			return ret;
		}

	template<typename MOVE>
		bool RecordT<MOVE>::load_csa(const std::string& filename)
		{
			std::ifstream inf(filename.c_str());

			if ( !inf.is_open() )
			{
				std::cout << "cannot open : " << filename;
				return false;
			}

			bool ret = read_csa(inf);

			info.filename = filename;
			return ret;
		}

#define DD(X) X
	//#define DD(X)

	template<typename MOVE>
		bool RecordT<MOVE>::read_kif(std::istream& is)
		{
			using namespace jsa;

			Board worker; worker.init();
			info.init();
			moves.clear();

			enum {
				S_INFOS,
				S_BOARD,
				S_RECORD,
				S_NORMAL_INIT,
				S_FORCE_INIT,
				S_FIN,
			} state = S_INFOS;

			bool is_board_initilized = false;

			char line[1024];
			const char* param;

			int	previous_to = 0x00;
			int rec_index = 0;
			board_t teai = B_HIRATE;

#define GOTO(X) state = X; goto reswitch;

			while ( is.getline(line, sizeof line) )
			{
				DD( line_t line_type = linetype(line, &param); ) // for debug
			reswitch:
				switch ( state )
				{
					case S_BOARD:
						switch ( linetype(line, &param) )
						{
							case L_HAND_SEN:
								worker.parse_load_kaki_hand(SENTE, param);
								break;
							case L_HAND_GO:
								worker.parse_load_kaki_hand(GOTE, param);
								break;
							case L_BOARD:
								worker.parse_load_kaki_board(line);
								break;
							case L_TEBAN_SEN:
								worker.turn = SENTE;
								start = worker;
								is_board_initilized = true;
								state = S_RECORD;
								break;
							case L_TEBAN_GO:
								worker.turn = GOTE;
								start = worker;
								is_board_initilized = true;
								state = S_RECORD;
								break;
							case L_EMPTY:
								break;
							default:
								DD( std::cout << "[s_board] no rule: [" << line <<"]");
								break;
						}
						break;

					case S_FORCE_INIT:
						// 強制的に初期化してもう一度遷移させる
						assert ( !is_board_initilized );
						worker.init(teai);
						this->start = worker;
						is_board_initilized = true;
						GOTO ( S_RECORD );

					case S_RECORD:
						switch ( linetype(line) )
						{
							case L_MOVECOMMENT:
								//DD(std::cout << "ignore move comment" << std::flush);
								break;
							case L_RECORD_END:
								DD(std::cout << "ignore RECORDEND\n");
								state = S_FIN;
								goto reswitch;
							case L_TEBAN_SEN:
							case L_TEBAN_GO:
								{	// ki2 format
									// 1行分をここで処理する
									const char* mv_str = line;
									do
									{
										BaseMove move;
										if ( !worker.parse_ki2_move(mv_str, move, previous_to, &mv_str) )
										{
											std::cout << "ki2:cannot parse: [" << mv_str << "]\n in [" << line << "]";
											break;
										}

										int piece = ( move.is_drop() ) ? move.drop_piece() : utl::state(worker.at(move.from));
										int getpc = ( move.is_drop() ) ? EP : worker.at(move.to);
										moves.push_back(gen(move, piece, getpc));

										bool res = worker.make(move);
										assert ( res );

										rec_index++;
										previous_to = move.to;

										// 区切りスペーススキップ
										while ( *mv_str == ' ' ) mv_str++;
									} while ( mv_str[0] != '\0' );
									break;
								}
							case L_MOVE:
								{
									BaseMove move;
									if ( !worker.parse_kif_move(line, move, previous_to) )
									{
										std::cout << "cannot parse => abort @" << rec_index << " : " << line;
										state = S_FIN;
										goto reswitch;
									}
									if ( move.is_special() )
									{
										//std::cout << "special move => abort @" << rec_index;
										state = S_FIN;
										goto reswitch;

									}

									int piece = ( move.is_drop() ) ? move.drop_piece() : utl::state(worker.at(move.from));
									int getpc = ( move.is_drop() ) ? EP : worker.at(move.to);
									moves.push_back(gen(move, piece, getpc));

									//move.write_debug();
									worker.make(move);
									//worker.write_kaki();

									rec_index++;
									previous_to = move.to;

									break;
								}
							case L_EMPTY:
								break;
							default:
								std::cout << "\n[s_record] no rule:[" << line << "] skipped" << std::endl;
								break;
						}// s_record switch
						break;

						// 色々対局情報
					case S_INFOS:
						switch( linetype(line, &param) )
						{
							case L_RECORD_BEGIN:
								if ( !is_board_initilized )
								{
									worker.init(teai);
									this->start = worker;
									is_board_initilized = true;
								}
								state = S_RECORD;
								break;

							case L_TEAI:
								{
									// HACK
									if ( begin_with(param, strings::hirate ) )     teai = B_HIRATE;
									else if ( begin_with(param, strings::sonota) ) teai = B_EMPTY;
									else
									{
										DD(std::cout << "unkown teai: " << param << "\n => set HIRATE\n" );
										assert(0);
										teai = B_HIRATE;
										return false;
									}
								} break;

							case L_NAME_SEN:  info.playername[0] = param; break;
							case L_NAME_GO:   info.playername[1] = param; break;
							case L_EVENT:     info.eventname     = param; break;
							case L_OPENING:   info.opening       = param; break;
							case L_DATE:      info.date          = param; break;
							case L_TIME_BEGIN:info.time_begin    = param; break;
							case L_TIME_END:  info.time_end      = param; break;

											  // なにすればいいんだか
							case L_TITLE:
											  break;

											  // ignore & continue
							case L_EMPTY:
											  break;

							case L_COMMENT:
											  //DD(std::cout << "comment ignored" );
											  break;

							case L_MOVE:
							case L_TEBAN_SEN:
							case L_TEBAN_GO:
							case L_MOVECOMMENT:
											  GOTO ( S_FORCE_INIT );

							case L_HAND_SEN:
							case L_HAND_GO:
											  GOTO ( S_BOARD );

							case L_UNKNOWN:
							default:
											  std::cout << "\n[s_info] no rule for line:[" << line << "] skipped" << std::endl;
											  std::cout << strings::kaishinichiji << "\n"; // DEBUG
											  break;
						}
						break;
					case S_FIN:
						break; // LOOP

					default:
						std::cout << "what?"  << std::endl;
				}// switch state
			}
#undef GOTO
			// end everything
			info.length = rec_index;

			return true;
		}


	namespace csa
	{
		inline line_t linetype(const char line[], const char** param=NULL)
		{
			struct { char c; line_t type; } *cp, c_types[] =
			{
				{ '\0', L_EMPTY },
				{ '+', L_TEBAN_SEN },
				{ '-', L_TEBAN_GO },
				{ 'T', L_MOVETIME },
				{ '\'', L_COMMENT },
				{ 'P', L_BOARD },
				{ '%', L_MOVE },
				{ '~', L_UNKNOWN }
			};
			for ( cp = c_types; cp->type!=L_UNKNOWN; cp++ )
				if ( line[0] == cp->c )
					return cp->type;

			struct { const char* s; line_t type; } *sp, s_type[] =
			{
				{ "$START_TIME:",	L_TIME_BEGIN },
				{ "$END_TIME",		L_TIME_END },
				{ "V",				L_VERkioN },
				{ "N+",				L_NAME_SEN },
				{ "N-",				L_NAME_GO },
				{ "$EVENT:",		L_EVENT },
				{ "$SITE:",			L_PLACE },
				{ "$TIME_LIMIT:",	L_TIME },
				{ "$OPENING:",		L_OPENING },
				{ "",				L_UNKNOWN }
			};
			for ( sp = s_type; sp->type!=L_UNKNOWN; sp++ )
				if ( begin_with(line, sp->s, param ) )
					return sp->type;

			std::cout << "unknown type: [" << line << "]\n";
			return L_UNKNOWN;
		}
	}

	template<typename MOVE>
		bool RecordT<MOVE>::read_csa(std::istream& is)
		{
			using namespace csa;

			info.init();
			moves.clear();
			Board worker; worker.init();

			//DD(std::cout << "aasdhaf" << std::endl << "とりあえず平手だけです" << std::flush);

			enum {
				S_VERkioN_NEEDED,
				S_INFOS,
				S_BOARD,
				S_RECORD,
				S_FIN,
			} state = S_VERkioN_NEEDED;

			bool is_board_initilized = false;

			int i_lines = 0;
			char line[1024];
			const char* param;

			int	previous_to = 0;
			int rec_index=0;

			for(;;)
			{
				std::string lnline;
				if ( !std::getline(is, lnline, '\n') ) break;
				std::stringstream ss(lnline);

				for (;;)
				{
					// カンマ前の空白スキップ
					char c; while ( ss.get(c) ) if ( c != ' ' ) { ss.unget(); break; }
					if ( !ss.getline(line, sizeof line, ',') ) break;

#define GOTO(X) state = X; goto reswitch;

					i_lines++;
					DD( line_t line_type = linetype(line, &param); ) // for debug
						reswitch:
						switch ( state )
						{
							case S_VERkioN_NEEDED:
								switch ( linetype(line, &param) )
								{
									case L_COMMENT:
										DD( std::cout << "comment before verkion: " << line << '\n' );
										break;
									case L_VERkioN:
										if ( atof(param) <= 2.2 ) // 多分ver2.2までok
											state = S_INFOS;
										else
										{
											std::cout << "unknown verkion: " << param;
											return false;
										}
										break;
									default:
										std::cout << "verkion needed: " << line;
										return false;
								}
							case S_BOARD:
								switch ( linetype(line, &param) )
								{
									case L_BOARD:
										if ( begin_with(line, "PI", &param) )
										{
											worker.init(B_HIRATE);

											// TODO "PI82HI22KA"とかいう書き方があるらしい
											while ( param[0] != '\0' )
											{
												int pos = utl::pos(dec(param[0]), dec(param[1]));

												if ( !utl::on_board(pos) )
												{
													std::cout << "cannot parse: " << line << "\n";
													break;
												}
												worker.at(pos) = EP;
												param += 4;
											}
										}
										else if ( begin_with(line, "P+") || begin_with(line, "P-") )
										{
											if ( false == worker.parse_load_csa_hand(line) )
												std::cout << "cannot parse: " << line << "\n";
										}
										else
										{
											if ( false == worker.parse_load_csa_board(line) )
												std::cout << "cannot parse: " << line << "\n";
										}
										break;
									case L_TEBAN_SEN:
										worker.turn = SENTE;
										this->start = worker;
										is_board_initilized = true;
										state = S_RECORD;
										break;
									case L_TEBAN_GO:
										worker.turn = GOTE;
										this->start = worker;
										is_board_initilized = true;
										state = S_RECORD;
										break;
								}
								break;
							case S_RECORD:
								switch ( linetype(line) )
								{
									case L_COMMENT:
										DD(std::cout << "ignore comment" << std::flush);
										break;

									case L_MOVETIME:
										//DD(std::cout << "ignore movetime" << std::flush);
										break;

									case L_TEBAN_SEN:
									case L_TEBAN_GO:
									case L_MOVE:	// specialなんで分けられそうだけど
										BaseMove move;
										if ( !worker.parse_csa_move(line, move) )
										{
											std::cout << "cannot parse => abort @" << rec_index << " : " << line;
											GOTO ( S_FIN );
										}
										if ( move.is_special() )
										{
											//std::cout << "special move => abort @" << rec_index;
											GOTO ( S_FIN );
										}

										int piece = ( move.is_drop() ) ? move.drop_piece() : utl::state(worker.at(move.from));
										int getpc = ( move.is_drop() ) ? EP : worker.at(move.to);
										moves.push_back(gen(move, piece, getpc));

										//move.write_debug();
										worker.make(move);
										//worker.write_kaki();
										previous_to = move.to;
										rec_index++;
								}
								break;

							case S_INFOS:
								switch( linetype(line, &param) )
								{
									case L_BOARD:
										GOTO ( S_BOARD );

										// faital err
									case L_MOVE:
										DD(std::cout << "unbelivable    " << std::endl);
										return false;

									case L_EVENT:      info.eventname     = param; break;
									case L_OPENING:    info.opening       = param; break;
									case L_NAME_SEN:   info.playername[0] = param; break;
									case L_NAME_GO:    info.playername[1] = param; break;
									case L_DATE:       info.date          = param; break;
									case L_TIME_BEGIN: info.time_begin    = param; break;
									case L_TIME_END:   info.time_end      = param; break;

													 // ignore & continue
									case L_EMPTY:
													 break;
									case L_COMMENT:	// 棋譜中のものではない
													 DD(std::cout << "comment ignored" << std::flush);
													 break;
									case L_UNKNOWN:
									default:
													 std::cout << "\nunknown line:[" << line << "] skipped" << std::endl;
													 break;
								}
								break;
							case S_FIN:
								break; // LOOP
							default:
								std::cout << "what?"  << std::endl;
						}// switch state
				} // カンマまで取得
			} // 行取得
			// end everything
			info.length = rec_index;
			return true;
		}

	inline recordext_t whichext(const std::string& filename)
	{
		size_t n = filename.size() - 3;

		if ( n < 0 ) return EXT_UNKNOWN;

		char ext[4];
		for ( int i=0; i<4; i++ )
			ext[i] = tolower(filename[n+i]);

		if ( 0 == strcmp(ext, "csa") ) return EXT_CSA;
		if ( 0 == strcmp(ext, "kif") ) return EXT_KIF;
		if ( 0 == strcmp(ext, "ki2") ) return EXT_KI2;
		return EXT_UNKNOWN;
	}

	template<typename MOVE>
		inline void RecordT<MOVE>::init(board_t type)
		{
			info.init();
			start.init(type);
			moves.clear();
		}
	template<>
		inline BaseMove RecordT<BaseMove>::gen(const BaseMove& bm, int, int) const
		{
			return bm;
		}
	template<>
		inline PieceMove RecordT<PieceMove>::gen(const BaseMove& bm, int piece_at, int) const
		{
			return PieceMove(bm, piece_at);
		}
	template<>
		inline FullMove RecordT<FullMove>::gen(const BaseMove& bm, int piece_at, int getpc) const
		{
			return FullMove(bm, piece_at, getpc);
		}


	template<typename MOVE>
		inline bool RecordT<MOVE>::load(const std::string& filename)
		{
			switch ( whichext(filename) )
			{
				case EXT_CSA:
					return load_csa(filename);

				case EXT_KIF: case EXT_KI2:
					return load_kif(filename);

				case EXT_UNKNOWN:
				default:
					return false;
			}
		}
	template<typename MOVE>
		inline bool RecordT<MOVE>::save(const std::string& filename) const
		{
			switch ( whichext(filename) )
			{
				case EXT_CSA: return save_csa(filename);
				case EXT_KIF: return save_kif(filename);
				case EXT_KI2: return save_ki2(filename);
				case EXT_UNKNOWN:
				default:
					return false;
			}
		}
	template<typename MOVE>
		inline bool RecordT<MOVE>::save_kif(const std::string& filename) const
		{
			std::ofstream of(filename);
			return of && write_kif(of);
		}
	template<typename MOVE>
		inline bool RecordT<MOVE>::save_ki2(const std::string& filename) const
		{
			std::ofstream of(filename);
			return of && write_ki2(of);
		}
	template<typename MOVE>
		inline bool RecordT<MOVE>::save_csa(const std::string& filename) const
		{
			std::ofstream of(filename);
			return of && write_csa(of);
		}
	template<typename MOVE>
		inline bool RecordT<MOVE>::write_csa(std::ostream& os) const
		{
			Board worker = this->start;

			os << "V2.2\n";

			if ( worker.is_hirate_shokikyokumen() )
				os << "PI\n+\n";
			else
				worker.write_csa(os);

			int len = moves.size();

			for ( int i=0; i<len; i++ )
			{
				os << ( worker.turn == SENTE ? '+' : '-' );

				const BaseMove& m = moves[i];

				if ( m.is_drop() )
				{
					utl::pos2str(0x00, os);
					utl::pos2str(m.to, os);
					os << csa::STATE_STR[m.drop_piece()];
				}
				else
				{
					utl::pos2str(m.from, os);
					utl::pos2str(m.to, os);
					os << csa::STATE_STR[utl::state(utl::promote(worker.at(m.from), m.promote))];
				}
				os << '\n';

				worker.make(m);
			}
			return true;
		}
	template<typename MOVE>
		inline bool RecordT<MOVE>::write_kif(std::ostream& os) const
		{
			Board worker = this->start;

			if ( worker.is_hirate_shokikyokumen() )
				os << strings::teaiwari << strings::hirate << "\n";
			else
				worker.write_kaki(os);
			os << strings::tesu_sashite_shohijikan << "\n";

			int len = moves.size();
			int prepos = -1;
			for ( int i=0; i<len; i++ )
			{
				const BaseMove& m = moves[i];

				os << std::setw(4) << std::setfill(' ') << std::right << (i+1) << ' ';

				std::stringstream ss;

				if ( m.to == prepos )
					ss << strings::dou << strings::zenspace;
				else
				{
					int x, y; utl::pos2xy(m.to, &x, &y);
					ss << jsa::ZEN_SUJI[x] << jsa::KAN_SUJI[y];
				}

				if ( m.is_drop() )
					ss << jsa::STATE_STR[m.drop_piece()] << strings::utsu; // ？
				else
				{
					ss << jsa::STATE_STR[utl::state(worker.at(m.from))]; // ？
					ss << ( m.promote == PROMOTE ? strings::naru : "" );
					ss << '(';
					utl::pos2str(m.from, ss);
					ss << ')';
				}
				os << std::setw(13) << std::setfill(' ') << std::left << ss.str();
				os << "( 0:00/00:00:00)\n";

				prepos = moves[i].to;
				worker.make(m);
			}

			os << std::right; // 後始末
			return true;
		}
	template<typename MOVE>
		inline bool RecordT<MOVE>::write_ki2(std::ostream& os) const
		{
			std::vector<JSAMove> jsamoves;
			this->base2jsa(jsamoves);

			Board worker = this->start;

			if ( worker.is_hirate_shokikyokumen() )
				os << strings::teaiwari << strings::hirate << "\n";
			else
				worker.write_kaki(os);

			int len = moves.size();

			for ( int i=0; i<len; i++ )
			{
				//jsamoves[i].write_ki2(os);
				//os << ( i == len-1 || i % 6 == 5 ? '\n': ' ');

				jsamoves[i].write_ki2sp(os);
				if ( i == len-1 || i % 6 == 5 ) os << '\n';
			}
			return true;
		}

	template<typename MOVE>
		inline bool RecordT<MOVE>::base2jsa(std::vector<JSAMove>& jsamoves) const
		{
			jsamoves.clear();

			Board worker = this->start;

			int len = this->moves.size();

			for ( int i=0; i<len; i++ )
			{
				JSAMove m;
				worker.base2jsa(this->moves[i], m);

				jsamoves.push_back(m);

				if ( !worker.make(this->moves[i]) )
					return false;
			}
			return true;
		}

} // kifio

#endif//KIFIO_H

