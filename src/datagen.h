#ifndef DATAGEN_H
#define DATAGEN_H

#include <fstream>
#include "chess/board.h"

using namespace Chess;

const std::string DATA_PATH = "C:/Users/semio/Documents/programming/Astra-Data/val_data/chess_val_data1_d9.csv";
const std::string NET_DATA_PATH = "C:/Users/semio/Downloads/chess_data.bin";

struct NetInput {
    uint64_t pieces[NUM_COLORS][6]{};
    float target{};
    Color stm;
};

void saveNetInput(int datasize);

#endif //DATAGEN_H
