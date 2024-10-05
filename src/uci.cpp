#include "uci.h"

#include "chess/perft.h"
#include "eval/eval.h"
#include "syzygy/tbprobe.h"

namespace UCI {

    const std::string version = "3.2";

    Uci::Uci() : board(STARTING_FEN), engine(STARTING_FEN) {
        options["Hash"] = Option("spin", "64", "64", 1, 2048);
        options["EvalFile"] = Option("string", "nn-768-2x256-1.nnue", "nn-768-2x256-1.nnue", 0, 0);
        options["SyzygyPath"] = Option("string", "", "", 0, 0);

        applyOptions();
        engine.reset();
    }

    void Uci::updatePosition(std::istringstream &is) {
        std::string token, fen;

        is >> token;
        if (token == "startpos") {
            fen = STARTING_FEN;
        } else if (token == "fen") {
            while (is >> token && token != "moves") {
                fen += token + " ";
            }
        } else {
            std::cout << "Unknown command" << std::endl;
            return;
        }

        board = Board(fen);
        while (is >> token) {
            if (token != "moves") {
                board.makeMove(getMove(token));
            }
        }

        engine.board = board;
        engine.board.refreshAccumulator();
    }

    void Uci::go(std::istringstream &is) {
        Astra::Limits limit;

        int64_t w_time = 0, b_time = 0, move_time = 0;
        int w_inc = 0, b_inc = 0, moves_to_go = 0;

        std::string token;
        while (is >> token) {
            if (token == "perft") {
                int depth;
                if (!(is >> depth)) {
                    std::cout << "No depth value provided.\n";
                } else {
                    testPerft(depth);
                }
                return;
            }

            if (token == "wtime") {
                is >> w_time;
            } else if (token == "btime") {
                is >> b_time;
            } else if (token == "winc") {
                is >> w_inc;
            } else if (token == "binc") {
                is >> b_inc;
            } else if (token == "movestogo") {
                is >> moves_to_go;
            } else if (token == "movetime") {
                is >> move_time;
            } else if (token == "depth") {
                int depth;
                is >> depth;
                limit.depth = depth;
            } else if (token == "nodes") {
                U64 nodes;
                is >> nodes;
                limit.nodes = nodes;
            } else if (token == "infinite") {
                limit.infinite = true;
            } else {
                std::cout << "Unknown command" << std::endl;
                return;
            }
        }

        Color stm = engine.board.getTurn();
        const int64_t time_left = stm == WHITE ? w_time : b_time;
        const int inc = stm == WHITE ? w_inc : b_inc;

        if (move_time != 0) {
             limit.time = move_time;
        } else if (time_left != 0) {
             engine.time_manager.setOptimum(time_left, inc, moves_to_go);
        }

        engine.time_manager.init(limit);

        // start search
        Astra::SearchResult result = engine.bestMove(99);
        std::cout << "bestmove " << result.best_move << std::endl;
    }

    void Uci::loop() {
        std::string line;
        std::string token;

        while (std::getline(std::cin, line)) {
            std::istringstream is(line);
            token.clear();
            is >> std::skipws >> token;

            if (token == "uci") {
                std::cout << "id name Astra v" << version << std::endl;
                std::cout << "id author Semih Oezalp" << std::endl;
                printOptions();
                std::cout << "uciok" << std::endl;
            } else if (token == "isready") {
                std::cout << "readyok" << std::endl;
            } else if (token == "ucinewgame") {
                engine.reset();
            } else if (token == "position") {
                updatePosition(is);
            } else if (token == "eval") {
                std::cout << Eval::evaluate(board) << std::endl;
            } else if (token == "go") {
                go(is);
            } else if (token == "setoption") {
                setOption(is);
                applyOptions();
            } else if (token == "d") {
                engine.board.print(engine.board.getTurn());
            } else if (token == "quit") {
                tb_free();
                break;
            } else {
                std::cout << "Unknown Command" << std::endl;
            }
        }
    }

    // options functions
    void Uci::applyOptions() {
        auto path = getOption("SyzygyPath");
        if (!path.empty() && path != "<empty>") {
            bool success = tb_init(path.c_str());

            if (success && TB_LARGEST > 0) {
                engine.use_TB = true;
                std::cout << "info string successfully loaded syzygy path" << std::endl;
            } else {
                std::cout << "info string failed to load syzygy path " << path << std::endl;
            }
        }

        path = getOption("EvalFile");
        if (!path.empty() && path != "<empty>") {
            NNUE::nnue.init(path);
        }

        engine.tt->init(std::stoi(getOption("Hash")));
        engine.reset();
    }

    void Uci::setOption(std::istringstream &is) {
        std::vector<std::string> tokens = split(is.str(), ' ');
        const std::string name = tokens[2];
        const std::string value = tokens[4];

        if (tokens.size() < 5) {
            std::cout << "Invalid option command" << std::endl;
            return;
        }

        if (tokens[1] != "name") {
            std::cout << "Invalid option command";
            return;
        }

        if (tokens[3] != "value") {
            std::cout << "Invalid option command";
            return;
        }

        if (options.count(name)) {
            options[name] = value;
        } else {
            std::cout << "Unrecognized option: " << name << std::endl;
        }
    }

    std::string Uci::getOption(const std::string &str) const {
        auto it = options.find(str);
        if (it != options.end()) {
            return it->second.getValue();
        }
        return "";
    }

    void Uci::printOptions() const {
        for (const auto &elem: options) {
            Option option = elem.second;
            const std::string &value = option.getDefaultValue();

            std::cout << "option name " << elem.first
                    << " type " << option.getType()
                    << " default " << (value.empty() ? "<empty>" : value);

            if (option.getMin() != 0 && option.getMax() != 0) {
                std::cout << " min " << option.getMin()
                        << " max " << option.getMax() << std::endl;
            } else {
                std::cout << std::endl;
            }
        }
    }

} // namespace UCI
