
#include <string>

#include "option.h"
#include "kifio.h"
#include "table.h"

namespace usage {
	ID_GAME(USAGE_STR)
	ID_BOARD(USAGE_STR)
}

const std::string Version = "1";

const std::string Usage =
	"usage: kif filename [options]\n"
	"   if - is given for filename, stdin is used as target. see -input\n"
	;

const std::string Format =
	"in -format, -info\n" + 
	usage::black_name     + usage::game_length    + "\n" + 
	usage::white_name     + usage::date           + "\n" + 
	usage::black_rate     + usage::start_at       + "\n" + 
	usage::white_rate     + usage::end_at         + "\n" + 
	usage::event_name     + usage::opening        + "\n" + 
	usage::title          + "\n"
	"\nin -format\n"        + 
	usage::ply            + usage::kakinoki_board + "\n" + 
	usage::last_move      + usage::jsa_board      + "\n" + 
	usage::next_move      + usage::csa_board      + "\n" + 
	usage::legal_moves    + usage::pico_board     + "\n" + 
	usage::legal_move_num + usage::othex_board    + "\n" + 
	usage::comment        + usage::svg_board      + "\n"
	;

int main(int argc, const char* argv[])
{
	Option opts(Usage, 25);
	opts.set_options()
		("help"                                         , "show this message")
		("version"                                      , "show version")
		("info"    , Option::Value("%d-%b(%B)%w(%W)%l") , "get game info")
		("format"  , Option::Value("%p%m")              , "get board info (use with for, forc)")
		("for"     , Option::Value("1..-1")             , "target boards for -format")
		("forc"    , Option::Value("1..-1")             , "target bordds for -format, only where comment is avialable")
		("record"  , Option::Value("csa")               , "convert record to one of csa, kif, ki2, pico, usi?, fsen?'")
		("board"   , Option::Value("-1")                , "show board at specified ply")
		("input"   , Option::Value("auto")              , "treat file type one of auto, csa, kif, ki2, pico, usi?, fsen?")
	;
	opts.parse(argc, argv);

	// check args
	if ( opts.has("help") )
	{
		std::cout << opts << "\n" << Format;
		exit(1);
	}
	else if ( opts.count() == 0 )
	{
		//opts.write_debug(std::cout);
		std::cout << Usage << "\n";
		exit(1);
	}

	// load file HACK stdin?
	kifio::Record rec;
	rec.load(opts[0]);

	// show
	if ( opts.has("info") )
	{
		const std::string format = opts["info"];
		rec.printinfo(format, std::cout);
		std::cout << "\n";
		exit(1);
	}
	else if ( opts.has("board") )
	{
		const int ply = opts.get<int>("board");

		for ( kifio::Record::Iterator it=rec.iterator(1, -1); it; it.next() )
			it->write_csa(std::cout);
	}
}
