/*
 * MIT License
 *
 * Copyright (c) 2020 Frank Kopp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "bitboard.h"
#include "castlingrights.h"
#include "direction.h"
#include "macros.h"
#include <sstream>

Bitboard getAttacksBb(PieceType pt, Square sq, Bitboard occupied) {
  switch (pt) {
    case BISHOP:
      return Bitboards::bishopMagics[sq].attacks[Bitboards::bishopMagics[sq].index(occupied)];
    case ROOK:
      return Bitboards::rookMagics[sq].attacks[Bitboards::rookMagics[sq].index(occupied)];
    case QUEEN:
      return Bitboards::bishopMagics[sq].attacks[Bitboards::bishopMagics[sq].index(occupied)] | Bitboards::rookMagics[sq].attacks[Bitboards::rookMagics[sq].index(occupied)];
    case KNIGHT:
      return Bitboards::nonSliderAttacks[pt][sq];
    case KING:
      return Bitboards::nonSliderAttacks[pt][sq];
    default:
      return EMPTY_BB;
  }
}

std::string str(Bitboard b) {
  std::ostringstream os;
  os << std::bitset<64>(b);
  return os.str();
}

std::string strBoard(Bitboard b) {
  std::ostringstream os;
  os << "+---+---+---+---+---+---+---+---+\n";
  for (Rank r = RANK_8; r >= RANK_1; --r) {
    for (File f = FILE_A; f <= FILE_H; ++f) {
      os << (b & squareOf(f, r) ? "| X " : "|   ");
    }
    os << "|\n+---+---+---+---+---+---+---+---+\n";
  }
  return os.str();
}

std::string strGrouped(Bitboard b) {
  std::ostringstream os;
  for (uint16_t i = 0; i < 64; i++) {
    if (i > 0 && i % 8 == 0) {
      os << ".";
    }
    os << (b & (ONE_BB << i) ? "1" : "0");
  }
  os << " (" + std::to_string(b) + ")";
  return os.str();
}

// //////////////////////////////////
// Initialization
// //////////////////////////////////

void Bitboards::rankFileBbPreCompute() {
  for (int i = RANK_1; i <= RANK_8; i++) {
    rankBb[i] = Rank1BB << (8 * i);
  }
  for (int i = FILE_A; i <= FILE_H; i++) {
    fileBb[i] = FileABB << i;
  }
}

void Bitboards::squareBitboardsPreCompute() {
  for (Square sq = SQ_A1; sq <= SQ_H8; ++sq) {

    // square bitboard
    squareBb[sq] = ONE_BB << sq;


    // file and rank bitboards
    sqToFileBb[sq] = fileBb[fileOf(sq)];
    sqToRankBb[sq] = rankBb[rankOf(sq)];

    // square diagonals
    // @formatter:off
    if (DiagUpA8 & sq) squareDiagUpBb[sq] = DiagUpA8;
    else if (DiagUpA7 & sq) squareDiagUpBb[sq] = DiagUpA7;
    else if (DiagUpA6 & sq) squareDiagUpBb[sq] = DiagUpA6;
    else if (DiagUpA5 & sq) squareDiagUpBb[sq] = DiagUpA5;
    else if (DiagUpA4 & sq) squareDiagUpBb[sq] = DiagUpA4;
    else if (DiagUpA3 & sq) squareDiagUpBb[sq] = DiagUpA3;
    else if (DiagUpA2 & sq) squareDiagUpBb[sq] = DiagUpA2;
    else if (DiagUpA1 & sq) squareDiagUpBb[sq] = DiagUpA1;
    else if (DiagUpB1 & sq) squareDiagUpBb[sq] = DiagUpB1;
    else if (DiagUpC1 & sq) squareDiagUpBb[sq] = DiagUpC1;
    else if (DiagUpD1 & sq) squareDiagUpBb[sq] = DiagUpD1;
    else if (DiagUpE1 & sq) squareDiagUpBb[sq] = DiagUpE1;
    else if (DiagUpF1 & sq) squareDiagUpBb[sq] = DiagUpF1;
    else if (DiagUpG1 & sq) squareDiagUpBb[sq] = DiagUpG1;
    else if (DiagUpH1 & sq) squareDiagUpBb[sq] = DiagUpH1;

    if (DiagDownH8 & sq) squareDiagDownBb[sq] = DiagDownH8;
    else if (DiagDownH7 & sq) squareDiagDownBb[sq] = DiagDownH7;
    else if (DiagDownH6 & sq) squareDiagDownBb[sq] = DiagDownH6;
    else if (DiagDownH5 & sq) squareDiagDownBb[sq] = DiagDownH5;
    else if (DiagDownH4 & sq) squareDiagDownBb[sq] = DiagDownH4;
    else if (DiagDownH3 & sq) squareDiagDownBb[sq] = DiagDownH3;
    else if (DiagDownH2 & sq) squareDiagDownBb[sq] = DiagDownH2;
    else if (DiagDownH1 & sq) squareDiagDownBb[sq] = DiagDownH1;
    else if (DiagDownG1 & sq) squareDiagDownBb[sq] = DiagDownG1;
    else if (DiagDownF1 & sq) squareDiagDownBb[sq] = DiagDownF1;
    else if (DiagDownE1 & sq) squareDiagDownBb[sq] = DiagDownE1;
    else if (DiagDownD1 & sq) squareDiagDownBb[sq] = DiagDownD1;
    else if (DiagDownC1 & sq) squareDiagDownBb[sq] = DiagDownC1;
    else if (DiagDownB1 & sq) squareDiagDownBb[sq] = DiagDownB1;
    else if (DiagDownA1 & sq) squareDiagDownBb[sq] = DiagDownA1;
    // @formatter:on
  }
}

void Bitboards::nonSlidingAttacksPreCompute() {
  // @formatter:off
  // steps for kings, pawns, knight for WHITE - negate to get BLACK
  int steps[][5] = { {},
                     { NORTH_WEST, NORTH, NORTH_EAST, EAST }, // king
                     { NORTH_WEST, NORTH_EAST }, // pawn
                     { WEST+NORTH_WEST,          // knight
                       EAST+NORTH_EAST,
                       NORTH+NORTH_WEST,
                       NORTH+NORTH_EAST }};
  // @formatter:on

  // knight and king attacks
  for (Color c = WHITE; c <= BLACK; ++c) {
    for (PieceType pt : {KING, PAWN, KNIGHT}) {
      for (Square s = SQ_A1; s <= SQ_H8; ++s) {
        for (int i = 0; steps[pt][i]; ++i) {
          Square to = s + Direction(c == WHITE ? steps[pt][i] : -steps[pt][i]);
          if (validSquare(to) && distance(s, to) < 3) {
            if (pt == PAWN) { pawnAttacks[c][s] |= to; }
            else { nonSliderAttacks[pt][s] |= to; }
          }
        }
      }
    }
  }
}

void Bitboards::neighbourMasksPreCompute() {
  for (Square square = SQ_A1; square <= SQ_H8; ++square) {
    int f = int(fileOf(square));
    int r = int(rankOf(square));
    for (int j = 0; j <= 7; j++) {
      // file masks
      if (j < f) {
          filesWestMask[square] |= FileABB << j  ;
        }
      if (7-j > f) {
          filesEastMask[square] |= FileABB << (7 - j);
        }
      // rank masks
      if (7-j > r) {
          ranksNorthMask[square] |= Rank1BB << (8 * (7 - j));
        }
      if (j < r) {
          ranksSouthMask[square] |= Rank1BB << (8 * j);
        }
    }
    if (f > 0) {
      fileWestMask[square] = FileABB << (f - 1);
    }
    if (f < 7) {
      fileEastMask[square] = FileABB << (f + 1);
    }
    neighbourFilesMask[square] = fileEastMask[square] | fileWestMask[square];
  }
}

void Bitboards::raysPreCompute() {
  for (Square sq = SQ_A1; sq <= SQ_H8; ++sq) {
    rays[N][sq] =  getAttacksBb(ROOK, sq, EMPTY_BB) & ranksNorthMask[sq];
    rays[E][sq] =  getAttacksBb(ROOK, sq, EMPTY_BB) & filesEastMask[sq];
    rays[S][sq] =  getAttacksBb(ROOK, sq, EMPTY_BB) & ranksSouthMask[sq];
    rays[W][sq] =  getAttacksBb(ROOK, sq, EMPTY_BB) & filesWestMask[sq];
    rays[NW][sq] = getAttacksBb(BISHOP, sq, EMPTY_BB) & filesWestMask[sq] & ranksNorthMask[sq];
    rays[NE][sq] = getAttacksBb(BISHOP, sq, EMPTY_BB) & filesEastMask[sq] & ranksNorthMask[sq];
    rays[SE][sq] = getAttacksBb(BISHOP, sq, EMPTY_BB) & filesEastMask[sq] & ranksSouthMask[sq];
    rays[SW][sq] = getAttacksBb(BISHOP, sq, EMPTY_BB) & filesWestMask[sq] & ranksSouthMask[sq];
  }
}

void Bitboards::intermediatePreCompute() {
  for (Square from = SQ_A1; from <= SQ_H8; ++from) {
    for (Square to = SQ_A1; to <= SQ_H8; ++to) {
      Bitboard toBB = squareBb[to];
      for (int o = 0; o < 8; o++) {
         if ((rays[o][from] & toBB) != EMPTY_BB) {
             intermediateBb[from][to] |= rays[Orientation(o)][from] & ~rays[o][to] & ~toBB;
         }
      }
    }
  }
}

void Bitboards::maskPassedPawnsPreCompute() {
  for (Square square = SQ_A1; square <= SQ_H8; ++square) {
    int f = int(fileOf(square));
    int r = int(rankOf(square));
    // white pawn - ignore that pawns can'*t be on all squares
    passedPawnMask[WHITE][square] |= rays[N][square];
    if (f < 7 && r < 7) {
      passedPawnMask[WHITE][square] |= rays[N][square + EAST];
    }
    if (f > 0 && r < 7) {
      passedPawnMask[WHITE][square] |= rays[N][square + WEST];
    }
    // black pawn - ignore that pawns can'*t be on all squares
    passedPawnMask[BLACK][square] |= rays[S][square];
    if (f < 7 && r > 0) {
      passedPawnMask[BLACK][square] |= rays[S][square + EAST];
    }
    if (f > 0 && r > 0) {
      passedPawnMask[BLACK][square] |= rays[S][square + WEST];
    }
  }
}

void Bitboards::castleMasksPreCompute() {
  // castle masks
  kingSideCastleMask[WHITE] =  squareBb[SQ_F1] | squareBb[SQ_G1] | squareBb[SQ_H1];
  kingSideCastleMask[BLACK] =  squareBb[SQ_F8] | squareBb[SQ_G8] | squareBb[SQ_H8];
  queenSideCastleMask[WHITE] = squareBb[SQ_D1] | squareBb[SQ_C1] | squareBb[SQ_B1] | squareBb[SQ_A1];
  queenSideCastleMask[BLACK] = squareBb[SQ_D8] | squareBb[SQ_C8] | squareBb[SQ_B8] | squareBb[SQ_A8];
  castlingRights[SQ_E1] = WHITE_CASTLING;
  castlingRights[SQ_A1] = WHITE_OOO;
  castlingRights[SQ_H1] = WHITE_OO;
  castlingRights[SQ_E8] = BLACK_CASTLING;
  castlingRights[SQ_A8] = BLACK_OOO;
  castlingRights[SQ_H8] = BLACK_OO;
}

void Bitboards::squareColorsPreCompute() {
  for (Square sq = SQ_A1; sq <= SQ_H8; ++sq) {
    if ((int(fileOf(sq))+int(rankOf(sq)))%2 == 0) {
      squaresBb[BLACK] = squaresBb[BLACK] | sq;
    } else {
      squaresBb[WHITE] = squaresBb[WHITE] | sq;
    }
  }
  fprintln(strBoard(squareBb[WHITE]));
  fprintln(strBoard(squareBb[BLACK]));

}

/// xorshift64star Pseudo-Random Number Generator
/// This class is based on original code written and dedicated
/// to the public domain by Sebastiano Vigna (2014).
/// It has the following characteristics:
///
///  -  Outputs 64-bit numbers
///  -  Passes Dieharder and SmallCrush test batteries
///  -  Does not require warm-up, no zeroland to escape
///  -  Internal state is a single 64-bit integer
///  -  Period is 2^64 - 1
///  -  Speed: 1.60 ns/call (Core i7 @3.40GHz)
///
/// For further analysis see
///   <http://vigna.di.unimi.it/ftp/papers/xorshift.pdf>
class PRNG {
  uint64_t s;
  uint64_t rand64() {
    s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
    return s * 2685821657736338717LL;
  }
public:
  PRNG(uint64_t seed) : s(seed) { assert(seed); }
  template<typename T> T rand() { return T(rand64()); }
  /// Special generator used to fast init magic numbers.
  /// Output values only have 1/8th of their bits set on average.
  template<typename T> T sparse_rand() { return T(rand64() & rand64() & rand64()); }
};

Bitboard sliding_attack(Direction directions[], Square sq, Bitboard occupied) {
  Bitboard attack = 0;
  for (int i = 0; i < 4; ++i) {
    for (Square s = sq + directions[i];
         validSquare(s) && distance(s, s - directions[i]) == 1;
         s += directions[i]) {
      attack |= s;
      if (occupied & s)
        break;
    }
  }
  return attack;
}

void init_magics(Bitboard table[], Magic magics[], Direction directions[]) {

  // Optimal PRNG seeds to pick the correct magics in the shortest time
  int seeds[RANK_LENGTH] = {728, 10316, 55013, 32803, 12281, 15100, 16645, 255};

  Bitboard occupancy[4096], reference[4096], edges, b;
  int epoch[4096] = {}, cnt = 0, size = 0;

  for (Square s = SQ_A1; s <= SQ_H8; ++s) {
    // Board edges are not considered in the relevant occupancies
    edges = ((Rank1BB | Rank8BB) & ~Bitboards::rankBb[s]) | ((FileABB | FileHBB) & ~Bitboards::fileBb[s]);

    // Given a square 's', the mask is the bitboard of sliding attacks from
    // 's' computed on an empty board. The index must be big enough to contain
    // all the attacks for each possible subset of the mask and so is 2 power
    // the number of 1s of the mask. Hence we deduce the size of the shift to
    // apply to the 64 or 32 bits word to get the index.
    Magic& m = magics[s];
    m.mask   = sliding_attack(directions, s, 0) & ~edges;
    m.shift  = 64 - popcount(m.mask);

    // Set the offset for the attacks table of the square. We have individual
    // table sizes for each square with "Fancy Magic Bitboards".
    m.attacks = s == SQ_A1 ? table : magics[s - 1].attacks + size;

    // Use Carry-Rippler trick to enumerate all subsets of masks[s] and
    // store the corresponding sliding attack bitboard in reference[].
    b = size = 0;
    do {
      occupancy[size] = b;
      reference[size] = sliding_attack(directions, s, b);
      size++;
      b = (b - m.mask) & m.mask;
    } while (b);

    PRNG rng(seeds[rankOf(s)]);

    // Find a magic for square 's' picking up an (almost) random number
    // until we find the one that passes the verification test.
    for (int i = 0; i < size;) {
      for (m.magic = 0; popcount((m.magic * m.mask) >> 56) < 6;)
        m.magic = rng.sparse_rand<Bitboard>();

      // A good magic must map every possible occupancy to an index that
      // looks up the correct sliding attack in the attacks[s] database.
      // Note that we build up the database for square 's' as a side
      // effect of verifying the magic. Keep track of the attempt count
      // and save it in epoch[], little speed-up trick to avoid resetting
      // m.attacks[] after every failed attempt.
      for (++cnt, i = 0; i < size; ++i) {
        unsigned idx = m.index(occupancy[i]);
        if (epoch[idx] < cnt) {
          epoch[idx]     = cnt;
          m.attacks[idx] = reference[i];
        }
        else if (m.attacks[idx] != reference[i])
          break;
      }
    }
  }
}

// init_magics() computes all rook and bishop attacks at startup. Magic
// bitboards are used to look up attacks of sliding pieces. As a reference see
// www.chessprogramming.org/Magic_Bitboards. In particular, here we use the so
// called "fancy" approach.
void Bitboards::initMagicBitboards() {
  Direction rookDirections[] = { NORTH, EAST, SOUTH, WEST };
  Direction bishopDirections[] = { NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST };
  init_magics(Bitboards::rookTable, rookMagics, rookDirections);
  init_magics(bishopTable, bishopMagics, bishopDirections);
}

