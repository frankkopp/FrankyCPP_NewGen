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

#include "chesscore/Position.h"
#include "engine/Search.h"
#include "engine/SearchConfig.h"
#include "init.h"
#include "types/types.h"

#include <engine/EvalConfig.h>
#include <gtest/gtest.h>
using testing::Eq;

using namespace std::chrono;
using namespace std;

class EngineSpeedTests : public ::testing::Test {
public:
  static void SetUpTestSuite() {
    NEWLINE;
    init::init();
    NEWLINE;
    Logger::get().TEST_LOG->set_level(spdlog::level::debug);
    Logger::get().SEARCH_LOG->set_level(spdlog::level::debug);
    Logger::get().TT_LOG->set_level(spdlog::level::debug);
    Logger::get().UCIHAND_LOG->set_level(spdlog::level::debug);
  }

protected:
  void SetUp() override {}
  void TearDown() override {}
};

// 21.6. Loaner laptop
// NPS: 8.924.349
TEST_F(EngineSpeedTests, npsTest) {
  SearchConfig::TT_SIZE_MB          = 64;
  SearchConfig::USE_BOOK            = false;
  SearchConfig::USE_ALPHABETA       = false;
  SearchConfig::USE_PVS             = false;
  SearchConfig::USE_TT              = false;
  SearchConfig::USE_TT_VALUE        = false;
  SearchConfig::USE_EVAL_TT         = false;
  SearchConfig::USE_MDP             = false;
  SearchConfig::USE_HISTORY_COUNTER = false;
  SearchConfig::USE_HISTORY_MOVES   = false;
  SearchConfig::USE_QUIESCENCE      = true;
  SearchConfig::USE_QS_STANDPAT_CUT = false;
  SearchConfig::USE_QS_SEE          = true;
  SearchConfig::USE_QS_TT           = false;

  EvalConfig::USE_MATERIAL   = true;
  EvalConfig::USE_POSITIONAL = false;
  EvalConfig::TEMPO          = 0;

  //  Position p{"2rr2k1/1p2qp1p/1pn1pp2/1N6/3P4/P6P/1P2QPP1/2R2RK1 w - - 0 1 "};
  Position p{};
  Search s{};
  s.isReady();
  SearchLimits sl{};
  sl.timeControl = true;
  sl.moveTime    = 60s;
  s.startSearch(p, sl);
  EXPECT_TRUE(s.isSearching());
  EXPECT_FALSE(s.hasResult());
  s.waitWhileSearching();
  EXPECT_TRUE(s.hasResult());
  fprintln("NPS: {:n}", nps(s.getLastSearchResult().nodes, s.getLastSearchResult().time));
}