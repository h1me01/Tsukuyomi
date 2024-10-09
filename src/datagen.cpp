#include "datagen.h"

std::vector<NetInput> loadDataset(int data_size) {
    std::vector<NetInput> dataset;
    std::ifstream file(DATA_PATH);

    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return dataset;
    }

    std::string header;
    std::getline(file, header);

    std::string line;
    for (int i = 0; std::getline(file, line) && data_size > i; ++i) {
        std::istringstream iss(line);
        std::string fen, eval;

        std::getline(iss, fen, ',');
        std::getline(iss, eval, ',');

        if (fen.empty()) {
            std::cerr << "Empty FEN at line " << i << std::endl;
            exit(1);
        }

        NetInput input;
        input.target = static_cast<float>(std::stoi(eval));

        Board board(fen);
        for (const PieceType pt: {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
            input.pieces[WHITE][pt] = board.getPieceBB(WHITE, pt);
            input.pieces[BLACK][pt] = board.getPieceBB(BLACK, pt);
        }

        input.stm = board.getTurn();
        dataset.push_back(input);
    }

    return dataset;
}

void saveNetInput(int datasize) {
    std::vector<NetInput> data = loadDataset(datasize);
    std::cout << "Loaded " << data.size() << " positions" << std::endl;

    // calculate size of file (in gb)
    std::cout << "Size of file: " << (sizeof(NetInput) * data.size()) / (1024 * 1024 * 1024) << " gb" << std::endl;

    const int batch_size = 4096;

    int num_batches = data.size() / batch_size;
    for(int i = 0; i < num_batches - 1; i++) {
        std::ofstream file(NET_DATA_PATH + "batch_" + std::to_string(i + 1) + ".bin", std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error writing in file" << std::endl;
            exit(1);
        }

        for (int j = 0; j < batch_size; j++) {
            file.write(reinterpret_cast<const char *>(&data[i * batch_size + j]), sizeof(NetInput));
        }

        file.close();
    }

    exit(0);
}
